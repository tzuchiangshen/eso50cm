/** ***************************************************************************

 **************************************************************************** */
#include "./MotorDC_v06.h"
//#define FIRMWARE_VERSION "20100406_1607"
#define rLED PIN_E1

//#priority SSP, TIMER0, AD  

//----------------------------------------------------------------------------
#include "./tmr0_18f4431.h"
#include "./i2c_18f4431.h"

#include "./pPWM_18f4431.h"
#include "./tmr5_18f4431.h"
#include "./qei_18f4431.h"
//#include "./adc_18f4431.h"

//----------------------------------------------------------------------------
#define DISABLE     0
#define ENABLE      1
#define NEWPID      2
#define PID         3
#define NEWRAMPA    4
#define RAMPA       5
//#define RESET       6
#define RESETPID    7
#define STOP        8
#define STOPSLOWLY  9
#define IDLE        255


//----------------------------------------------------------------------------
// TMR0:
int16 tmr0cnt;
int16 p_tmr0rv;
int16 p_Tmr0;
int	  tmr0_flag;
int	  tmr0_XHz_flag;

//----------------------------------------------------------------------------
// i2c
#define buf_len 4
int i2c_rx_flag;
int i2c_tx_flag;
byte i2c_state;
int buf_idx;
char buffer[buf_len];
int mem_add;
char i2c_add;
signed int32 data[8];
char * pointer;

//----------------------------------------------------------------------------
// pPWM
int             run_flag;


signed int16    p_Vfin;
int             p_dir;

int             m_MtrCtrl;    //1
signed int16    m_V;
signed int16    new_V;
int             m_dir;
int             new_dir;
int16           m_NextDuty0;
int16           m_NextDuty2;
int16           m_NextDuty4;
int16           m_NextDuty6;

//----------------------------------------------------------------------------
// PID

float   encTrkVel;
float   encVelRef;
float   encVelRef_old;
float   encVelRef_new;
float   Vpid;

float   error;
float   derr;
float	interr;
float   v_Kp;         //4
float   v_Ki;         //4
float   v_Kd;         //4

float   h_error;
float   h_derr;
float	h_interr;
float   h_Kp;         //4
float   h_Ki;         //4
float   h_Kd;         //4

int     h_pid_flag;

//Position
signed int32    trgPos;
signed int32    trgThreshold;
signed int32    diffPos;
int             trgDir;
int             trgDirOld;

float   posError;
float   trgEncVelRef;

//----------------------------------------------------------------------------
// QE
//int old_POSCNTH;
//int old_POSCNTL;
signed int32 encRev, encRevOld;
int16 POSCNT;
int16 POSCNTold;
signed int32 encVel, encVelOld;
signed int32 encVelavg;
signed int32 encPos, encPosOld;




float avg, n_avg;

int	  traking_flag;
int   goto_tracking_flag;
int   goto_flag;
/** ***************************************************************************
 * TIMER0_isr
 */
#int_TIMER0
void  TIMER0_isr(void)  {
    set_timer0( p_Tmr0 );
	tmr0_flag = TRUE;
	//output_toggle( rLED );
	tmr0cnt --;
	if( tmr0cnt == 0 ) {
		tmr0cnt = p_tmr0rv;
		tmr0_XHz_flag = TRUE;
    }
}
/** ***************************************************************************
 * SSP_isr
 */
#int_SSP
void  SSP_isr(void)  {
   
    i2c_state = i2c_isr_state();
	if( i2c_state == 0 )  {
		buf_idx = 0;
        if( PIC_SSPSTATbits.BF ) {
            i2c_add = i2c_read();               //vacia buffer, lleno con address
        }
    } else if( i2c_state == 1 ) {
		mem_add = i2c_read();
    } else if( i2c_state < 5 ) {                  
		buffer[buf_idx] = i2c_read();
		buf_idx ++;
		buffer[buf_idx]= 0;
    } else if( i2c_state == 5 ) {
		buffer[buf_idx] = i2c_read();
		i2c_rx_flag = true;
	} else if( i2c_state == 0x80 ) {                //Master is requesting data
        if( PIC_SSPSTATbits.BF ) {
            i2c_add = i2c_read();               //vacia buffer, lleno con address
        }
        i2c_tx_flag = TRUE;                         //Prepare data for TX
		//pointer = (char *) & data[mem_add];
        //WriteI2C( pointer[0] );
        buf_idx= 0;
    } else if( i2c_state < 0x86 ) {                 //Sending  data's last 3 bytes
        if( buf_idx < 3 ) {
            buf_idx ++;
            WriteI2C( pointer[buf_idx] );
        } else {
            WriteI2C( '#' );
        }
    }
}

/** ***************************************************************************
 * QE Index (event) ISR
 */
#INT_IC2QEI
void QEIF_isr( void ) {
    //encPERIOD = make16( old_POSCNTH, old_POSCNTL );
    if( PIC_QEICONbits.UPDOWN ) {
        encRev ++;
    } else {
        encRev --;
    }
}

/** ***************************************************************************
 *  UpdatePWMDutyCicles
 *      o   Recalcula los proximos DutyCicles a partir de la frecuencia
 *          Frotor
 */
void UpdatePWMDutyCycles( void ) {
    if( m_dir  ) {
        m_NextDuty0 = m_V;                     
        m_NextDuty2 = 0; 
        m_NextDuty4 = m_V;
        m_NextDuty6 = 0;
    } else {
        m_NextDuty0 = 0;
        m_NextDuty2 = m_V;
        m_NextDuty4 = 0;
        m_NextDuty6 = m_V;
    }
    set_power_pwm0_duty( m_NextDuty0 );
    set_power_pwm2_duty( m_NextDuty2 );
    set_power_pwm4_duty( m_NextDuty4 );
    set_power_pwm6_duty( m_NextDuty6 );
}

/** ***************************************************************************
 * CalcNextPWMDutyCicles
 *     Para Fpwm = 10KHz, maxDutyCycle ~ 2007
 *     MaxDC = 1800, MinDC = 200;
 *     Si V = 64 => MaxDC = 1800
 */
//void CalcNextPWMDutyCicles( void ) {
//    if( m_dir  ) {
//        m_NextDuty0 = m_V;                     
//        m_NextDuty2 = 0; 
//        m_NextDuty4 = m_V;
//        m_NextDuty6 = 0;
//    } else {
//        m_NextDuty0 = 0;
//        m_NextDuty2 = m_V;
//        m_NextDuty4 = 0;
//        m_NextDuty6 = m_V;
//    }
//}



/** ***************************************************************************
 * Main
 */
void main()
{
	//int pa, pb, pc, pd, tmp;
	int i, sec;
	signed int32 X32;//, Y32;
	float tmp_float;
    
    //------------------------------------------------------------------------
    //                               Setup uCU
    //------------------------------------------------------------------------
    set_tris_a( 0b00011111 );           //RA0, RA1, QEIndex, QEA, QEB are inputs
	//set_tris_b( 0x00 );               //PORTB all outputs
    set_tris_c( 0b00110100 );           //PORTC 4,5,2 inputs
    set_tris_d( 0x00 );                 //PORTD all outputs
    set_tris_e( 0x00 );                 //PORTE all outputs
 
    setup_wdt( WDT_OFF );

    setup_timer_1( T1_DISABLED );
    setup_timer_2( T2_DISABLED, 0, 1 );
    
    //------------------------------------------------------------------------
    // pPWM
    SetupPowerPWM();
    setup_power_pwm_pins( PWM_COMPLEMENTARY, 
        PWM_COMPLEMENTARY,
        PWM_COMPLEMENTARY,
        PWM_COMPLEMENTARY );  
        
    //------------------------------------------------------------------------
    // QEI
    SetupQEI();
    //SetupTMR5();
    //SetupADC();

    //------------------------------------------------------------------------
    //  TMR0 Vars; 
    SetupTMR0();
	p_Tmr0     = 40535;		            // => 100[Hz]  (tmr0  = 0.4us Xtal= 20MHz / 4 / 2 )
	p_tmr0rv   = 10;		            // => 10[Hz]
	set_timer0( p_Tmr0 );
	tmr0cnt = p_tmr0rv;

    //------------------------------------------------------------------------
    //    Enable Interruptions
    enable_interrupts(INT_TIMER0);      // TMR0
    enable_interrupts(INT_SSP);         // I2C
    enable_interrupts(INT_IC2QEI);      // QE Index event interruption
    
    //enable_interrupts(INT_CCP1);        // QE Change on position
    //enable_interrupts(INT_IC1);         // Input Capture 1: QEI Mode/Velocity Mode =>  Velocity  Reg Updated
    //enable_interrupts(INT_PWMTB);       // PWM Base Time
    //enable_interrupts(INT_TIMER5);
    //enable_interrupts(INT_AD);

    enable_interrupts(GLOBAL);

    //------------------------------------------------------------------------
    //                              Test Loop
	i = 10;
	while( i > 0 ) { 
		if( tmr0_XHz_flag )	{
    		tmr0_XHz_flag = FALSE;
			output_toggle( rLED );
			i --;
		}
	}	
	output_low( rLED );

    //------------------------------------------------------------------------
    //                              Main Loop 
	tmr0_flag = FALSE;
    tmr0_XHz_flag = FALSE;
    p_tmr0rv   = 10;		            // => 10[Hz]
    
	i2c_rx_flag = FALSE;
	i2c_tx_flag = FALSE;
	run_flag = FALSE;
    
    m_V = 0;
    m_MtrCtrl = IDLE;
    encRev = 0;
    encRevOld = 0;
    * ( ( (char *) & POSCNTold ) + 1 ) = PIC_POSCNTH;
    * ( ( (char *) & POSCNTold ) )     = PIC_POSCNTL;
    encPosOld  = encRevOld;
    encPosOld *= 3000;
	encPosOld += POSCNTold;   
	encVelavg = 0;
	traking_flag = false;
	goto_tracking_flag = false;
	goto_flag    = false;
    //------------------------------------------------------------------------
    // v_PID
    // Valore para empezar a 4Hz
    // v_Kp      = 0.03333;
    // v_Ki      = 0.08;
    // v_Kd      = 0.1;
    v_Kp      = 0.15;
    v_Ki      = 0.02;
    v_Kd      = 0.01;

	error           = 0;
	encVelRef       = 0;
	encVelRef_old   = 0;
	encVelRef_new   = 0;
	encVelOld       = 0;
    derr            = 0;
    interr          = 0; 
    encTrkVel       = 600.0;		
 
    //------------------------------------------------------------------------
    // h_PID
    h_Kp      = 0.0015;
    h_Ki      = 0.0002;
    h_Kd      = 0.0001;

	h_error      = 0;
    h_derr       = 0;
    h_interr     = 0; 
 
    h_pid_flag = false;
    //------------------------------------------------------------------------
    m_dir = 1;
    n_avg = 1;
    avg = 0;
    sec = p_tmr0rv;
    PIC_POSCNTH = 0;
    PIC_POSCNTL = 0;
    POSCNT = 0;
    encRev = 0;
    encPos = 0;
    trgPos = 0;
    while( TRUE ){
        //old_POSCNTL = PIC_POSCNTL;
        //old_POSCNTH = PIC_POSCNTH;
        
		if( tmr0_XHz_flag ) {
			tmr0_XHz_flag = FALSE;
			
			//POSCNT = make16( PIC_POSCNTH, PIC_POSCNTL );
			* ( ( (char *) & POSCNT ) + 1 ) = PIC_POSCNTH;
			* ( ( (char *) & POSCNT ) )     = PIC_POSCNTL;
		    encPos  = encRev;
		    encPos *= 3000;
		    encPos += POSCNT;
		    encVel  = encPos;
		    encVel -= encPosOld;
	        encVel *= 10;               //10Hz
	        
	        avg = avg * n_avg;
	        avg += (float)encVel;
	        n_avg += 1;
	        avg /= n_avg;

            if( encVel != 0 ){
                output_toggle( rLED );
            }
            

 	        if( goto_flag == 1 ) {
                if( goto_tracking_flag ) {
                    trgPos += 60;
                }
                if( encPos < trgPos ) {
  	                trgDir = 1;
  	                diffPos = trgPos - encPos;
  	            } else {
      	            trgDir = 0;
      	            diffPos = encPos - trgPos;
      	        }
                if( diffPos < 19200 ) {
                    trgThreshold = 25;
                    if( diffPos < 50 )  {
                        trgEncVelRef = 0;
                        goto_flag = 0;
                	} else if( diffPos < 19200 ) {
                    	trgEncVelRef  = diffPos;
                    	trgEncVelRef *= 0.5;
                    	trgEncVelRef += 600;
                    	goto_flag = 2;
                  	} else {
                      	trgEncVelRef  = 9600;
                      	goto_flag = 2;
                    }
                } else { //if( diffPos >= 19200 ) 
              	    if( diffPos < 38400 ) {
              	        trgEncVelRef = 9600;
              	        trgThreshold = 9600;
              	    } else if( diffPos < 76800 ) {
              	        trgEncVelRef = 19200;
              	        trgThreshold = 19200;
              	    } else if( diffPos < 153600 ) {
              	        trgEncVelRef = 38400;
              	        trgThreshold = 38400;
              	    } else if( diffPos < 307200 ) {
              	        trgEncVelRef = 76800;
              	        trgThreshold = 76800;
              	    } else {
              	        trgEncVelRef = 76800;
              	        trgThreshold = 76800;
              	    } 
              	    goto_flag = 2;
                }
                
                if( goto_tracking_flag ) {
                    if( trgEncVelRef > 4800 ){
                        traking_flag = false;
                    } else {
                        traking_flag = true;
                    }
                }

        	    if( trgDir > 0 ) {
      	            encVelRef = trgEncVelRef;
      	        } else {
                    encVelRef = -trgEncVelRef;
                }
                trgDirOld = trgDir;
                sec = p_tmr0rv;
                
                m_MtrCtrl = NEWPID;
            } else if( goto_flag == 2 ) {
                if( trgDir ==  1 ) {
                    if( encPos + trgThreshold > trgPos ) {
                        encVelRef = 0;
                        if( traking_flag ) {
                            goto_flag = 1;
                        } else {
                            goto_flag = 3;
                        }
                        m_MtrCtrl = NEWPID;
                    } else {
                        if( sec == 0 ) {
                            goto_flag = 1;
  	                    }
  	                }                
                } else { //if( trgDir !=  1 )
                    if( encPos < trgPos + trgThreshold  ) {
                        encVelRef = 0;
                        if( traking_flag ) {
                            goto_flag = 1;
                        } else {
                            goto_flag = 3;
                        }
                        m_MtrCtrl = NEWPID;
                    } else {
                        if( sec == 0 ) {
                            goto_flag = 1;
                        }                        
                    }
                }
                sec --;
            }
                

  	        
  	        
  	        
  	        
  	        
    	    switch( m_MtrCtrl ) {
                case DISABLE:
                    setup_power_pwm_pins( PWM_OFF, PWM_OFF, PWM_OFF, PWM_OFF );
                    
                    m_MtrCtrl = IDLE;
                    m_V = 0;
                    break;
                case ENABLE:
                    setup_power_pwm_pins( PWM_COMPLEMENTARY, 
                        PWM_COMPLEMENTARY,
                        PWM_COMPLEMENTARY,
                        PWM_COMPLEMENTARY );
                   			
                    m_MtrCtrl = IDLE;
                    m_V = 0;
                    break;                    
                case NEWRAMPA:
                    if( ! run_flag ) {
                        run_flag = TRUE;
                        m_V = 0 ;
                    }                    
                    m_MtrCtrl = RAMPA;
                    break;
                case RAMPA:
  			        if( p_dir == m_dir ) {
              	        if( m_V - p_Vfin > 10 ){
                  	        m_V -= 20;
        		        } else if(  p_Vfin - m_V > 10 ) {
      	  			        m_V += 20;
      			        } else {
          			        m_V = p_Vfin;
          			    }
      			    } else {
      			        if( m_V > 10 ) {
          			        m_V -= 20;
          			    } else {
              			    m_V = 0;
          			        m_dir = p_dir;
          			    }
      			    }
          			//CalcNextPWMDutyCicles();
      		        UpdatePWMDutyCycles();   

                    break;
                case RESETPID:
                    //Reset PID Integral Term
                    interr     = 0;
                    h_interr     = 0;

                    m_MtrCtrl  = PID;                
                case NEWPID:
                    tmp_float = encVelRef;      
                    if( traking_flag ) {
                        tmp_float += encTrkVel;
                    }             
                    if( tmp_float == 0 ){
                        if( h_pid_flag ) {
                            m_MtrCtrl = STOPSLOWLY;
                        } else {
                            m_MtrCtrl = STOP;
                        }
                    } else {
                        if( -10200 <  encVelRef && encVelRef < 10200 ) {
                            h_pid_flag = false;
                        } else {
                            h_pid_flag = true;
                        }
                        if( encVel != 0 ) {
                            if( encVelRef > encVelRef_old ) {
                                if( encVelRef - encVelRef_old > 4800 ) {
                                    if( h_pid_flag ) {
                                        m_MtrCtrl = STOPSLOWLY; 
                                    } else {
                                        m_MtrCtrl = STOP;
                                    }
                                } else {
                                    m_MtrCtrl = PID;
                                }
                            } else {
                                if( encVelRef_old - encVelRef > 4800 ) {
                                    if( h_pid_flag ) {
                                        m_MtrCtrl = STOPSLOWLY; 
                                    } else {
                                        m_MtrCtrl = STOP;
                                    }
                                } else {
                                    m_MtrCtrl = PID;
                                }
                            }      
                        } else {
                            m_MtrCtrl = PID;
                        }                         
                    }
                    encVelRef_old = encVelRef_new;
                    encVelRef_new = tmp_float;
                    avg = 0;
                    n_avg = 1;
                    break;
                case PID:
                    if( h_pid_flag ) {
            	        h_error  = encVel;
            	        h_error -= encVelRef_new;
            	        h_derr   = encVel - encVelOld;
            	        
            	        Vpid  = h_interr + h_error;
            	        Vpid *= h_Ki;
            	        Vpid += h_Kp * h_error;
               	        Vpid += h_Kd * h_derr;
                    } else {  
      	                //Vel Tracking = 3000/5 [tics/s] = 600 [tics/s]
          	            error  = encVel;
      	                error -= encVelRef_new;
              	        derr   = encVel - encVelOld;
          	        
         	            Vpid  = interr + error;
          	            Vpid *= v_Ki;
          	            Vpid += v_Kp * error;
           	            Vpid += v_Kd * derr;
                    }
                                 
        	        /*
            	    if( Vpid < -1800.0  ) {
            	        new_V = 1800.0;
            	        new_dir = 0;
            	    } else if( Vpid < 0 ) {
                	    interr  = interr + error;
                	    new_V = (int16) -Vpid;
                	    new_dir = 0;
                	} else if ( Vpid < 1800. ) {
                    	interr  = interr + error;
            	        new_V = (int16) Vpid;
            	        new_dir = 1;
            	    } else {
                	    new_V = 1800.0;
                	    new_dir = 1;
                	}  
                	*/
          	        if( encVelRef_new == 0 && encVel == 0 ) {
              	        Vpid = 0;
              	    }
                  	    
                	if( Vpid < 0 ) {
                    	new_V = (int16) -Vpid;
                	    new_dir = 0;
                    } else {
                        new_V = (int16) Vpid;
            	        new_dir = 1;
                    }
                    
                    //Limite superior
                    if( new_V > 1800 ) {
                        new_V = 1800;
                    } else {
                        if( h_pid_flag ) {
                            h_interr  = h_interr + h_error;
                        } else {
                            interr  = interr + error;
                        }
                	}
                	//Limite inferior (no cambia de direccion de giro)
                	if( m_dir != new_dir && encVel != 0 ){
                      	m_V = 50;
                  	} else {
                  	    m_V = new_V;
                  	    m_dir = new_dir;
                    }
      			    //CalcNextPWMDutyCicles();
      		        UpdatePWMDutyCycles();	  
                    break;
                case STOP:
                    m_V = 0;
      		        if( encVel == 0 ) {
          			    interr     = 0;
      			        m_MtrCtrl  = PID;
      			        if( goto_flag == 3 ) {
          			        goto_flag = 1;
          			    }
      			    }
                    UpdatePWMDutyCycles(); 
                    break;
                case STOPSLOWLY:
                       
                    m_V= 0;
                    /*
                    if( m_V  > 10 ){
              	        m_V -= 20;
    		        } else if( m_V < -10 ) {
  	  			        m_V += 20;
  			        } else {
      			        m_V = 0;
      			        
      			    } 
      			    */     
      			    if( encVel == 0 ) {
          			    h_interr   = 0;
      			        m_MtrCtrl  = PID;
      			        if( goto_flag == 3 ) {
          			        goto_flag = 1;
          			    }
      			    }
      			    UpdatePWMDutyCycles();          
                    break;
  		        case IDLE:
                    break;
            }

	        //Updating old for next iteration
	        encPosOld = encPos;
	        encVelOld = encVel;
	    }
		
        if( i2c_rx_flag  ) {
            i2c_rx_flag = false;
            if( mem_add < 8 ) {
                data[mem_add] = * (signed int32 *) buffer;
                if( mem_add == 0 ) {
                    m_MtrCtrl = buffer[0];
                } else if( mem_add == 1 ) {
                    X32 = * (signed int32 *) buffer;
                    if( X32 < 0  ){
                        p_Vfin = (int16) (- X32);
                        p_dir = 1;
                    } else {    
                        p_Vfin= (int16) X32;    
                        p_dir = 0; 
                    } 
                    m_MtrCtrl = NEWRAMPA;   
                } else if( mem_add == 3 ) {
                    X32 = * (signed int32 *) buffer;
                    
                    if( X32 == 0 ) {
                        traking_flag = false;
                        goto_tracking_flag = false;
                    } else {
                        traking_flag = true;
                        goto_tracking_flag = true;
                    }
                    encTrkVel = (float) X32;
                    avg = 0;
                    n_avg = 1;
                    m_MtrCtrl = NEWPID;
                } else if( mem_add == 4 ) {
                    X32 = * (signed int32 *) buffer;

                    if( X32 < 0 ) {
                        encRev  = X32 - 3000;
                    }  else {
                        encRev  = X32;
                    }
                    encRev /= 3000;
                    
                    encPos  = encRev;
                    encPos *= 3000;
                    encPos  = X32 - encPos;
                    
                    POSCNT  = (int32) encPos;
                    
                    PIC_POSCNTL = * ( ( (char *) & POSCNT ) );
                    PIC_POSCNTH = * ( ( (char *) & POSCNT ) + 1 );
                
                    encPos = X32;
                    encPosOld = encPos;
                    m_MtrCtrl = STOP;
   
                } else if( mem_add == 6 ) {
                    X32 = * (signed int32 *) buffer;
  
                    m_MtrCtrl = NEWPID;
                    encVelRef = (float) X32;              
                    avg = 0;
                    n_avg = 1;
                    
                } else if( mem_add == 7 ) {
                    X32 = * (signed int32 *) buffer;
                    //diffPos = X32;
                    trgPos  = encPos;
                    trgPos += X32;
                    
           
                    if( X32 == 0 ) {
                        goto_flag = 0;
                    } else {
                        goto_flag = 1;
                        trgThreshold = 0;
                    }
                    output_low( rLED );
                }
            }    
        }

        if( i2c_tx_flag ) {
            if( mem_add == 1 ) {
                data[1] = (signed int32) avg;
            } else if( mem_add == 2 ) {
                data[2] = POSCNT;
            } else if( mem_add == 3 ) {
                data[3] = encRev;
            } else if( mem_add == 4 ) {
                data[4] = encPos;
            } else if( mem_add == 5 ) {
                data[5] = encVel;
            } else if( mem_add == 6 ) {
                data[6] = (signed int32) m_V;
            } else if( mem_add == 7 ) {
                data[7] = diffPos;
            } 
            pointer = (char *) & data[mem_add];
            WriteI2C( pointer[0] );
            i2c_tx_flag = false;
        }   
        
    }
}
