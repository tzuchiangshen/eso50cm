/** ***************************************************************************
 * o Configuration bits
 *   - HS-PLL: H4
 *   - SCK/SCL SDA/SDI SDO RD3 RD2 RD1
 *
 * o 2011 01 12
 *   - TMR0         OK
 *   - RS232        OK
 *   - pPWM         OK
 *
 * o 2010 01 19
 *   - I2C          OK. 
 *     Compile using:
 *      #use i2c( slave, sda=PIN_C4, scl=PIN_C5, address=I2C_ADDRESS, force_hw )
 *      #define I2C_SDA   PIN_D2
 *      #define I2C_SCL   PIN_D3
 *     and check before programming the configuration bit "SSP I/O MUX".
 *
 * o 2011 01 20
 *   - QEI          OK.
 *
 **************************************************************************** */ 
#include "./MotorDC_v07.h"
#define version 701
#pragma id version
#define __PRODUCT__ "MotorDCdbg"


#priority SSP, RDA  

//----------------------------------------------------------------------------
#include "./tmr0_18f4431.h"

#include "./i2c_18f4431.h"
#include "./pPWM_18f4431.h"
#include "./qei_18f4431.h"

//#include "./adc_18f4431.h"

//----------------------------------------------------------------------------
#define gLED PIN_E0
#define yLED PIN_E1
#define rLED PIN_E2
//#define rLED PIN_E1

#define LED_ON  output_high
#define LED_OFF output_low

//----------------------------------------------------------------------------
// TMR0 vars
int16           tmr0cnt;
int16           p_tmr0rv;
int16           p_Tmr0;
int	            tmr0_flag;
int	            tmr0_XHz_flag;

//----------------------------------------------------------------------------
// TMR1  vars
//int16           p_Tmr1;

//----------------------------------------------------------------------------
// i2c vars
#define         buf_len         4
#define         NUM_DATA        8
int             i2c_rx_flag;
int             i2c_tx_flag;
byte            i2c_state;
int             buf_idx;
char            buffer[buf_len];
int             mem_add;
char            i2c_add;
signed int32    data[NUM_DATA];
signed int32    pram[NUM_DATA];
char *          pointer;

//----------------------------------------------------------------------------
// pPWM vars
int16           m_NextDuty0;
int16           m_NextDuty2;
int16           m_NextDuty4;
int16           m_NextDuty6;

int             m_MtrCtrl;
int             run_flag;

signed int16    m_V;
int             m_dir;

signed int16    p_Vfin;
int             p_dir;

signed int16    new_V;
int             new_dir;

//----------------------------------------------------------------------------
// tty vars
#define         TTY_BUF_LEN     32
char            tty_buffer[TTY_BUF_LEN];
char            msg_buffer[TTY_BUF_LEN];
char            ans_buffer[TTY_BUF_LEN];
int             msg_len;
int             ans_len;
int             msg_bin_start;
int             tty_start_byte;
int             tty_end_byte;
int             tty_seek_byte;

//----------------------------------------------------------------------------
// PID vars
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


float           v_Kp;
float           v_Ki;
float           v_Kd;
float           error;
float           derr;
float	        interr;

float           h_Kp;
float           h_Ki;
float           h_Kd;
float           h_error;
float           h_derr;
float	        h_interr;

float           encTrkVel;
float           encVelRef;
float           encVelRef_old;
float           encVelRef_new;
float           Vpid;
int             h_pid_flag;

signed int32    trgPos;
signed int32    trgThreshold;
signed int32    diffPos;
int             trgDir;
int             trgDirOld;

float           trgEncVelRef;

//----------------------------------------------------------------------------
// QEI vars.
int16           POSCNT;
signed int32    encRev, encVelOld;
signed int32    encPos, encPosOld;
signed int32    encVel;

signed int32    tics_per_sec;
signed int32    tics_counter;

float avg, n_avg;


int	            traking_flag;
int             goto_tracking_flag;
int             goto_flag;
/** ************************************************************************** 
 *  RDA_isr
 */
#int_RDA
void  RDA_isr( void ) { 
    tty_buffer[tty_seek_byte] = getc();;
    tty_seek_byte ++;
    tty_seek_byte %= TTY_BUF_LEN;
}

/** ***************************************************************************
 * TIMER0_isr
 */
#int_TIMER0
void  TIMER0_isr(void)  {

    set_timer0( p_Tmr0 );
	tmr0_flag = TRUE;
	tmr0cnt --;
	if( tmr0cnt == 0 ) {
		tmr0cnt = p_tmr0rv;
		tmr0_XHz_flag = TRUE;
    }
}
/** ***************************************************************************
 * TIMER1_isr
 */
//#int_TIMER1
//void  TIMER1_isr(void)  {
//}

/** ***************************************************************************
 * SSP_isr (I2C)
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
 *  IC1_isr
 *      o   There is a differece between the real and tha capture period
 *          (delay) 31 tics for PreS = 1:2 and PostS = 1:1 
 *
//#INT_IC1
//void IC1_isr( void ) {
//}

/** ***************************************************************************
 *  UpdatePWMDutyCicles
 *      o   Recalcula los proximos DutyCicles a partir de la frecuencia
 *          Frotor
 */
void UpdatePWMDutyCycles( void ) {
    // Xtal=20[MHz],PPL x1=> Fosc/4 = 5[MHz], PWM_TMR = 50 -> 10[KHz]
    // Max m_V = 2048
    // Xtal=10[MHz],PPL x4=> Fosc/4 = 10[MHz], PWM_TMR = 1000 -> 10[KHz]
    // Max m_V = 4096
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




/** ************************************************************************** 
 *  localMessage
 */
void localMessage( char * buffer ) 
{
	int i;
	char chksum;
    signed int16 X16;
    chksum = 0;

    switch( buffer[1] ) {
        case 't':
            break;
        case 'N':
            sprintf( ans_buffer, "%05.2f", version/100.0 );
            break;
        case 'T':  
            sprintf( ans_buffer, "%s", __TIME__ );  
            break;
        case 'D':
            sprintf( ans_buffer, "%s", __DATE__ );    
            
            break;   
        case 'R':
            if( m_dir ) {
                m_dir = 0;
            } else {
                m_dir = 1;
            }
            UpdatePWMDutyCycles();
            sprintf( ans_buffer, "Reverse pPWM" );  
            break;
        /*
        case 'K':
            switch( buffer[2] ) {
                case 'p': 
                    if( buffer[3] != '?' ) {
                        X16 = (signed int16) atol( & buffer[3] ); 
                        p_Kp = (float) X16;
                        p_Kp /= 1000000.0;
                    }
                    sprintf( ans_buffer, "p_Kp=%11.8f", p_Kp );  
                    break;
                case 'i':
                    if( buffer[3] != '?' ) {
                        X16 = (signed int16) atol( & buffer[3] ); 
                        p_Ki = (float) X16;
                        p_Ki /= 1000000.0;
                    }
                    sprintf( ans_buffer, "p_Ki=%11.8f", p_Ki );  
                    break;
                case 'd':
                    if( buffer[3] != '?' ) {
                        X16 = (signed int16) atol( & buffer[3] ); 
                        p_Kd = (float) X16;
                        p_Kd /= 1000000.0;
                    }
                    sprintf( ans_buffer, "p_Kd=%11.8f", p_Kd );  
                    break;
            }
            break;                                
        case 'E':
            trgPos  = encPos;
            trgPos += 36000;
            sprintf( ans_buffer, "trgPos=%8ld", trgPos );
            //m_V = (signed int16) atol( & buffer[2] );
            //sprintf( ans_buffer, "Start pPWM %ld", m_V );  
            //UpdatePWMDutyCycles();
            p_interr = 0;
            goto_flag = 1;
            break; 
        case 'W':
            trgPos  = encPos;
            trgPos -= 36000;
            sprintf( ans_buffer, "trgPos=%8ld", trgPos );
            //m_V = (signed int16) atol( & buffer[2] );
            //sprintf( ans_buffer, "Start pPWM %ld", m_V );  
            //UpdatePWMDutyCycles();
            p_interr= 0;
            goto_flag = 1;
            break; 
        */
        case 'P':
            m_V = 0;
            UpdatePWMDutyCycles();
            sprintf( ans_buffer, "Stop pPWM" );  
            goto_flag = 0;
            break;
        case ' ':   

            break;   
        default:
            sprintf( ans_buffer, "Unknown Command!" );           
    }
    ans_len = strlen( ans_buffer );
    putc( ':' );
    for( i = 0; i < ans_len; i ++ ) {
        putc( ans_buffer[i] );
    }
    ans_len += 3;
    for( i = ans_len; i < 40 ; i ++ ) {
        putc( 0 );
    }
    putc( chksum );
    putc( '#' );
}

/** ************************************************************************** 
 *  wrongMessage
 */
void wrongMessage( char * buffer ) 
{
	int i;
    
    sprintf( ans_buffer, "Unknown address %c", buffer[1] );           

    putc( ':' );
    
    for( i = 0; i < ans_len; i ++ ) {
        putc( ans_buffer[i] );
    }
    for( i = ans_len; i < 40; i ++ ) {
        putc( 0 );
    }
    putc( '#' );

}


/** ***************************************************************************
 * Main
 */
void main()
{
	//int pa, pb, pc, pd, tmp;
	int             i;
	int             k;
	//int             d_sec;
	int             sec;
	signed int32    X32;
	float           tmp_float;
    //========================================================================
    //                               Setup uCU
    //========================================================================

    //------------------------------------------------------------------------
    //  I/O Setup
    PORTA = 0;
    PORTB = 0;
    PORTC = 0;
    PORTD = 0;
    PORTE = 0;
    set_tris_a( 0b00011110 );           //RA1, QEIndex, QEA, QEB are inputs
	set_tris_b( 0x00 );                 //PORTB all outputs
    set_tris_c( 0xC0 );                 //PORTC 
    set_tris_d( 0b00001100 );           //PORTD: I2C SDA & SCL inputs 
    set_tris_e( 0x00 );                 //PORTE all outputs
    
    //------------------------------------------------------------------------
    // pPWM Setup
    SetupPowerPWM();
    set_power_pwm0_duty( 0 );
    set_power_pwm2_duty( 0 );
    set_power_pwm4_duty( 0 );
    set_power_pwm6_duty( 0 );

    //------------------------------------------------------------------------
    // 
    setup_wdt( WDT_OFF );

    setup_timer_1( T1_DISABLED );
    setup_timer_2( T2_DISABLED, 0, 1 );
    

        
    //------------------------------------------------------------------------
    // QEI and others Setup
    SetupQEI();
    //SetupTMR5();
    //SetupADC();    

    //------------------------------------------------------------------------
    //  TMR0  
    SetupTMR0();
	p_Tmr0     = 15535;                 // => 100[Hz]
	p_tmr0rv   = 10;		            // => 10[Hz]
	set_timer0( p_Tmr0 );
	tmr0cnt = p_tmr0rv;


    //------------------------------------------------------------------------
    //    Enable Interruptions
    enable_interrupts(INT_TIMER0);      // TMR0
    //enable_interrupts(INT_TIMER1);      // TMR1
    enable_interrupts(INT_RDA);         // RS232
    enable_interrupts(INT_IC2QEI);      // QE Index event interruption
    //enable_interrupts(INT_IC2QEI);      // QE interruption
    //enable_interrupts(INT_IC1);         // Input Capture 1: QEI Mode/Velocity Mode =>  Velocity  Reg Updated
    
    //enable_interrupts(INT_CCP1);        // QE Change on position
    //enable_interrupts(INT_PWMTB);       // PWM Base Time
    //enable_interrupts(INT_TIMER5);
    //enable_interrupts(INT_AD);

    enable_interrupts(INT_SSP);         // I2C
    enable_interrupts(GLOBAL);
 
    //========================================================================
    //                             TESTS
    //========================================================================
    //------------------------------------------------------------------------
    // I/O Test
    /*
    PORTE = 0;
    for( k = 0; k < 3; k ++ ) {
        output_high( rLED );
        delay_ms( 100 );
        output_low( rLED );
        delay_ms( 900 );
    }    
    for( k = 0; k < 3; k ++ ) {
        output_high( yLED );
        delay_ms( 100 );
        output_low( yLED );
        delay_ms( 900 );
    }    
    for( k = 0; k < 3; k ++ ) {
        output_high( gLED );
        delay_ms( 100 );
        output_low( gLED );
        delay_ms( 900 );
    }    
    PORTE = 0;
    */
    //------------------------------------------------------------------------
    // RS232 TX Test
    printf( ":Hello World!#" );
    printf( ":VP =%s-0x%02X#", __PRODUCT__, I2C_ADDRESS );    
    printf( ":VN =%05.2f#", version/100.0 );    
    printf( ":VD =%s#", __DATE__ );    
    printf( ":VT =%s#", __TIME__ ); 
    printf( ":I2CADD=0x%02X#", I2C_ADDRESS );
    //------------------------------------------------------------------------
    // Timer Test 
	/*
	i = 6;
	//d_sec = 0;
	output_low( PIN_E0);
	output_low( PIN_E1 );
	output_low( PIN_E2 );
	while( i > 0 ) { 
		if( tmr0_XHz_flag )	{
			tmr0_XHz_flag = FALSE;

    	    output_toggle( PIN_E0 );
    	    output_toggle( PIN_E1 );
    	    output_toggle( PIN_E2 );
			i --;
		}
	}	
	//d_sec = 0;
	output_low( PIN_E0);
	output_low( PIN_E1 );
	output_low( PIN_E2 );
    */
    //========================================================================
    //                             INIT. VARS
    //========================================================================
    
    //------------------------------------------------------------------------
    // Init TMR0 vars
    tmr0_flag       = FALSE;
	tmr0_XHz_flag   = FALSE;
	sec             = p_tmr0rv / 2;

    
    //------------------------------------------------------------------------
    // Init PID vars
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
    
    //------------------------------------------------------------------------
    // Init pPWM pins
    setup_power_pwm_pins( PWM_COMPLEMENTARY, 
        PWM_COMPLEMENTARY,
        PWM_COMPLEMENTARY,
        PWM_COMPLEMENTARY ); 

    //------------------------------------------------------------------------
    // Init TTY vars
    tty_start_byte  = 0;
    tty_end_byte    = 0;
    tty_seek_byte   = 0;
    msg_len         = 0;
    for( i= 0; i < TTY_BUF_LEN; i ++ ) {
        tty_buffer[i] = 0;
        msg_buffer[i] = 0;
        ans_buffer[i] = 0;
    }
        
    //------------------------------------------------------------------------
    // Init I2C vars
    for( i= 0; i < buf_len; i ++ ) {
        buffer[i] = 0;
    }
    for( i= 0; i < NUM_DATA; i ++ ) {
        data[i] = 0;
        pram[i] = 0;
    }
    i2c_rx_flag = 0;
    i2c_tx_flag = 0;
    i2c_state   = 0;
    buf_idx     = 0;
    mem_add     = 0;
    i2c_add     = 0;
    
    //------------------------------------------------------------------------
    // Init QEI vars.
    PIC_POSCNTH = 0;
    PIC_POSCNTL = 0;
    POSCNT      = 0;
    encRev      = 0;
    encPos      = 0;
    encPosOld   = 0;
    
    tics_per_sec    = 0;
    tics_counter    = 0;
    
    //========================================================================
    //                              Main Loop     
    //========================================================================
    while( TRUE ){
        
        //--------------------------------------------------------------------
        // TMR0 routine
        if( tmr0_flag ) {
            tmr0_flag = FALSE;
            output_high( gLED );
            if( tmr0_XHz_flag ) {   // tmr0_XHz Routine
                tmr0_XHz_flag = FALSE;
                if( sec == 0 ){
                    output_toggle( rLED );
                    sec = p_tmr0rv / 2;
                } else {
                    sec --;
                }
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

            } // end tmr0_XHz Routine


		    		
		    output_low( gLED );
		} // END tmr0 

        //--------------------------------------------------------------------
        // I2C routine
        if( i2c_rx_flag  ) {
            if( mem_add < NUM_DATA ) {
                X32 = * (signed int32 *) buffer;
                data[mem_add] = X32;
                
                if( mem_add == 0 ) {
                    //New MtrCtrl value
                    m_MtrCtrl = buffer[0];
                } else if( mem_add == 1 ) {
                    //New value for Vfin in RAMPA
                    //X32 = * (signed int32 *) buffer;
                    if( X32 < 0  ){
                        p_Vfin = (int16) (- X32);
                        p_dir = 1;
                    } else {    
                        p_Vfin= (int16) X32;    
                        p_dir = 0; 
                    } 
                    m_MtrCtrl = NEWRAMPA;   
                } else if( mem_add == 3 ) {
                    //New Tracking Speed Refference Value
                    //X32 = * (signed int32 *) buffer;
                    
                    if( X32 == 0 ) {
                        traking_flag = false;
                        goto_tracking_flag = false;
                    } else {
                        traking_flag = true;
                        goto_tracking_flag = true;
                    }
                    encTrkVel = (float) X32;
                    m_MtrCtrl = NEWPID;
                } else if( mem_add == 4 ) {
                    // Reset Position Counter to a new position
                    //X32 = * (signed int32 *) buffer;

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
                    //encPosOld = encPos;
                    m_MtrCtrl = STOP;
                } else if( mem_add == 6 ) {
                    //New Speed Refference
                    //X32 = * (signed int32 *) buffer;
                    encVelRef = (float) X32;
                    m_MtrCtrl = NEWPID;          
                } else if( mem_add == 7 ) {
                    //New Position Refference
                    //X32 = * (signed int32 *) buffer;
                    if( X32 == 0 ) {
                        goto_flag = 0;
                    } else {
                        //trgPos  = encPos;
                        //trgPos += X32;
                        goto_flag = 1;
                        trgThreshold = 0;
                    }
                }
            }
            i2c_rx_flag = false;
        }
        
        if( i2c_tx_flag ) {
            if( mem_add < NUM_DATA ) {
                if( mem_add == 1 ) {
                    data[1] = tics_per_sec;
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
            } else {
                mem_add -= NUM_DATA;
                pointer = (char *) & pram[mem_add];
            }
            WriteI2C( pointer[0] );
            i2c_tx_flag = false;
        }   // END I2C routine
        
        //--------------------------------------------------------------------
        // RS232 routine
        while( tty_start_byte != tty_seek_byte ) {
            if( tty_start_byte < tty_seek_byte ) {
                tty_end_byte = tty_seek_byte;
            } else {
                tty_end_byte = TTY_BUF_LEN;;
            }
            for( i = tty_start_byte; i < tty_end_byte; i ++ ) {
                if( msg_len == 0 ) {
                    if( tty_buffer[i] == ':' ) {
                        msg_buffer[msg_len] = tty_buffer[i];
                        msg_len ++;
                        msg_bin_start = 1;
                    }
                } else {
                    msg_buffer[msg_len] = tty_buffer[i];
                    msg_len ++;
                    if( tty_buffer[i] == '#' ) {
                        msg_buffer[msg_len]= 0;
                        msg_buffer[msg_len-1]= 0;
                        localMessage( msg_buffer );
        				//} else if( msg_buffer[1] > 0xA1 ) {
            			//	//i2cMessage( msg_buffer );
            		    //} else {
                		//    wrongMessage( msg_buffer );
                		//}    

                        msg_len = 0;  
                    }
                }
            } //END for( i = tty_start_byte; i < tty_end_byte; i ++ )
            tty_start_byte  = i;
            tty_start_byte %= TTY_BUF_LEN;
        } // END while( tty_start_byte != tty_seek_byte )
 
    }// END while( TRUE )
    
}//END PROGRAM
