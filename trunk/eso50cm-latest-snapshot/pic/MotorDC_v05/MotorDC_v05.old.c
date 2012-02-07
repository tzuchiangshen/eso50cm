/** ***************************************************************************
 * 2010 04 16 14:30
 * MotorDC
 * 2010 04 21
 * o Working:
 *   i2c
 *   pPWM
 * o Functions
 *   o mem_add = 0...3:
 *     Stores number (signed int32) in data[mem_add].
 *   o mem_add = 4
 *     sets m_MtrCtrl = buffer[0] (first byte o 4 bytes received)
 *      o m_MtrlCtrl = 0: Stops pPWM.
 *   o mem_add = 5
 *     sets m_Vfin = fabs( * (signed int *) buffer )
 *     and m_dir = 0 if (* (signed int *) buffer)  < 0
 *         m_dir = 1 if (* (signed int *) buffer)  > 0
 *     Setting m_Vfin automaticaly sets m_MtrCtrl = NEWRAMPA
 *     In RAMPA mode the pPWM Duty Cycles are updated at 100[Hz] (determied 
 *     using m_Tmr0 and m_tmr0rv.
 *
 * 2010 04 22
 * o Working HA driver:
 *   o RAMPA: 1200 -> 14[V]
 *
 **************************************************************************** */ 
#include "./MotorDC_v05.h"
#define FIRMWARE_VERSION "20100406_1607"
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
#define RESET       6
#define STARTPID    7
#define STOP        8
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
signed int32 data[7];
char * pointer;

//----------------------------------------------------------------------------
// pPWM
int             run_flag;


<<<<<<< HEAD

=======
>>>>>>> f171001e38cb4b7b3c2d618b450e19736e715ae6
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

float   encVelRef;
float   Vpid;
float   dt;
float   error;
float   error_old;
float   derr;
float	interr;
float   p_Kp;         //4
float   p_Ki;         //4
float   p_Kd;         //4

//----------------------------------------------------------------------------
// QE
int old_POSCNTH;
int old_POSCNTL;
signed int16 encRev, encRevOld;
int16 POSCNT;
int16 POSCNTold;
signed int32 encVel, encVelOld;
signed int32 encVelavg;
signed int32 encPos, encPosOld;
signed int32 trgPos, diffPos;
float avg, n_avg;

int	  traking_flag;

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
void CalcNextPWMDutyCicles( void ) {
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
}



/** ***************************************************************************
 * Main
 */
void main()
{
	//int pa, pb, pc, pd, tmp;
	int i, sec;
	signed int32 X32, Y32;
    
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
    //------------------------------------------------------------------------
    // PID
    // Valore para empezar a 4Hz
    // p_Kp      = 0.03333;
    // p_Ki      = 0.08;
    // p_Kd      = 0.1;
    p_Kp      = 0.15;
    p_Ki      = 0.02;
    p_Kd      = 0.01;

	error      = 0;
	encVelRef  = 0;
	encVelOld  = 0;
    derr       = 0;
    interr     = 0; 			

    m_dir = 1;
    n_avg = 1;
    sec = p_tmr0rv;
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

	        sec --;
	        if( sec == 0 ) {
    	        output_toggle( rLED );
    	        
    	      	diffPos  = encPos;
	            diffPos -= trgPos;  
	            
	            sec = p_tmr0rv;
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
              	        if( m_V - p_Vfin > 5 ){
                  	        m_V -= 10;
        		        } else if(  p_Vfin - m_V > 5 ) {
      	  			        m_V += 10;
      			        } else {
          			        m_V = p_Vfin;
          			    }
      			    } else {
      			        if( m_V > 5 ) {
          			        m_V -= 10;
          			    } else {
              			    m_V = 0;
          			        m_dir = p_dir;
          			    }
      			    }
          			CalcNextPWMDutyCicles();
      		        UpdatePWMDutyCycles();   

                    break;
                case NEWPID:        //Reset PID
                    error      = 0;
                    encVelOld  = 0;
                    derr       = 0;
                    interr     = 0;
                    m_V        = 0 ;
                    run_flag   = TRUE; 
                    m_MtrCtrl  = PID;
                    break;
                case PID:
        	        //Vel Tracking = 3000/5 [tics/s] = 600 [tics/s]
        	        error  = encVel;
        	        error -= encVelRef;
        	        derr   = encVel - encVelOld;
        	        
        	        Vpid  = interr + error;
        	        Vpid *= p_Ki;
        	        Vpid += p_Kp * error;
        	        Vpid += p_Kd * derr;
                
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
                	
                	if( m_dir != new_dir && encVel != 0 ){
                      	m_V = 50;
                  	} else {
                  	    m_V = new_V;
                  	    m_dir = new_dir;
                    }
      			    CalcNextPWMDutyCicles();
      		        UpdatePWMDutyCycles();	  
                    break;
                case STOP:
                    interr = 0;
                    m_V = 0;
      			    CalcNextPWMDutyCicles();
      		        UpdatePWMDutyCycles();   
      		        m_MtrCtrl  = IDLE;                 
                    break;
/*
                case 230:
                    if( encVel == 0  && m_V < new_V ) {
                        m_V += 2;
                    } else {
                        m_MtrCtrl  = PID;
                    }
          			CalcNextPWMDutyCicles();
      		        UpdatePWMDutyCycles();   
                    break;    
                case 240:
  			        if( m_V > 5 ) {
      			        m_V -= 10;
      			    } else {
                        error      = 0;
                        encVelOld  = 0;
                        derr       = 0;
                        interr     = 0;
          			    m_V = 0;
      			        m_dir = new_dir;
      			        m_MtrCtrl  = PID;
      			    }
          			CalcNextPWMDutyCicles();
      		        UpdatePWMDutyCycles();   
                    break; 
*/
  		        case IDLE:
                    break;
            }

	        //Updating old for next iteration
	        encPosOld = encPos;
	        encVelOld = encVel;
	    }
		
        if( i2c_rx_flag  ) {
            i2c_rx_flag = false;
            if( mem_add < 7 ) {
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
                        if( 0 < encVelRef && encVelRef < 1200 ) {
                            m_MtrCtrl = STOP;
                            encVelRef = 0;
                        }
                    } else {
                        traking_flag = true;
                        if( encVelRef == 0 ) {
                            encVelRef = 600;
                            m_MtrCtrl = PID;
                        }
                    }
                } else if( mem_add == 4 ) {
                    X32 = * (signed int32 *) buffer;
                    Y32 = X32 / 3000;
                    if( X32 < 0  ) {
                        Y32 -= 1;
                    }
                    encPos = X32 - Y32 * 3000;
                    POSCNT = (int16) encPos;
                    encRev = (signed int16) Y32;
                    * (int16 *) & PIC_POSCNTL = POSCNT;
                    encPos = encRev * 3000 + POSCNT;    
                } else if( mem_add == 6 ) {
                    X32 = * (signed int32 *) buffer;
                    if( traking_flag ) {
                        X32 += 600;
                    }
                    if( X32 == 0 ){
                        if( -19200 < encVelRef && encVelRef < 19200 ) {
                            m_MtrCtrl = STOP;
                        } else {
                            p_Vfin = 0;
                            p_dir = m_dir;
                            m_MtrCtrl = NEWRAMPA;
                        }
                    } else {
                        m_MtrCtrl = PID;
                    }
                    encVelRef = (float) X32;
                    avg = 0;
                    n_avg = 1;
                } else if( mem_add == 6 ) {
                    trgPos = * (signed int32 *) buffer;
                    
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
            } 
            pointer = (char *) & data[mem_add];
            WriteI2C( pointer[0] );
            i2c_tx_flag = false;
        }   
        
    }
}
