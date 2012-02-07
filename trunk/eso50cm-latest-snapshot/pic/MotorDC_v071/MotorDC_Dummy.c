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
#include "./MotorDC_Dummy.h"
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
int16           p_Tmr1;

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
signed int16    m_V;
int             m_dir;
int16           m_NextDuty0;
int16           m_NextDuty2;
int16           m_NextDuty4;
int16           m_NextDuty6;
//----------------------------------------------------------------------------
// QE vars
int16           qe_period;
int             qe_period_flag;
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

float           v_Kp;         //4
float           v_Ki;         //4
float           v_Kd;         //4

float           h_Kp;         //4
float           h_Ki;         //4
float           h_Kd;         //4

//----------------------------------------------------------------------------
// QE
int16           POSCNT;
int16           POSCNTold;
signed int32    encRev, encRevOld;
signed int32    encPos, encPosOld;
signed int32    encVel, encVelOld;
signed int32    encVelavg;

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
#int_TIMER1
void  TIMER1_isr(void)  {

}
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
 */#INT_IC1
void IC1_isr( void ) {

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




/** ************************************************************************** 
 *  localMessage
 */
void localMessage( char * buffer ) 
{
	int i;
	char chksum;

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
        case 'W': 
            
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
        case 'S':
            m_V = (signed int16) atol( & buffer[2] );
            sprintf( ans_buffer, "Start pPWM %ld", m_V );  
            UpdatePWMDutyCycles();
            break; 
        case 'P':
            m_V = 0;
            UpdatePWMDutyCycles();
            sprintf( ans_buffer, "Stop pPWM" );  
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
	int             d_sec;
	signed int32    X32;
	float           tmp;
    //========================================================================
    //                               Setup uCU
    //========================================================================

    //------------------------------------------------------------------------
    //  I/O Setup
    set_tris_a( 0b00011110 );           //RA1, QEIndex, QEA, QEB are inputs
	set_tris_b( 0x00 );                 //PORTB all outputs
    set_tris_c( 0xC0 );                 //PORTC 
    set_tris_d( 0b00001100 );           //PORTD: I2C SDA & SCL inputs 
    set_tris_e( 0x00 );                 //PORTE all outputs
    
    //------------------------------------------------------------------------
    // I/O Test
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
    
    //------------------------------------------------------------------------
    // 
    setup_wdt( WDT_OFF );

    setup_timer_1( T1_DISABLED );
    setup_timer_2( T2_DISABLED, 0, 1 );
    
    //------------------------------------------------------------------------
    // pPWM Setup
    SetupPowerPWM();
    setup_power_pwm_pins( PWM_COMPLEMENTARY, 
        PWM_COMPLEMENTARY,
        PWM_COMPLEMENTARY,
        PWM_COMPLEMENTARY ); 
        
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
    //  TMR1  
    SetupTMR1();
	

    //------------------------------------------------------------------------
    //    Enable Interruptions
    enable_interrupts(INT_TIMER0);      // TMR0
    enable_interrupts(INT_TIMER1);      // TMR1
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
    // RS232 TX Test
    printf( ":Hello World!#" );
    printf( ":VP =%s-0x%02X#", __PRODUCT__, I2C_ADDRESS );    
    printf( ":VN =%05.2f#", version/100.0 );    
    printf( ":VD =%s#", __DATE__ );    
    printf( ":VT =%s#", __TIME__ ); 
    printf( ":I2CADD=0x%02X#", I2C_ADDRESS );
    //------------------------------------------------------------------------
    // Timer Test 
	i = 6;
	d_sec = 0;
	output_low( PIN_E0);
	output_low( PIN_E1 );
	output_low( PIN_E2 );
	while( i > 0 ) { 
		if( tmr0_XHz_flag )	{
			tmr0_XHz_flag = FALSE;
			d_sec ++;
			if( d_sec > 4 ) {
    			d_sec = 0;
			    output_toggle( PIN_E0 );
			    output_toggle( PIN_E1 );
			    output_toggle( PIN_E2 );
			    i --;
	        }
		}
	}	
	d_sec = 0;
	output_low( PIN_E0);
	output_low( PIN_E1 );
	output_low( PIN_E2 );

    //========================================================================
    //                             INIT. VARS
    //========================================================================
    
    //------------------------------------------------------------------------
    // Init TMR0 vars
    tmr0_flag       = FALSE;
	tmr0_XHz_flag   = FALSE;
	d_sec           = 0;

    //------------------------------------------------------------------------
    // Init v_PID vars
    m_V             = 0;
    m_dir           = 0;
    // Valore para empezar a 4Hz
    // v_Kp      = 0.03333;
    // v_Ki      = 0.08;
    // v_Kd      = 0.1;
    v_Kp      = 0.15;
    v_Ki      = 0.02;
    v_Kd      = 0.01;

    //------------------------------------------------------------------------
    // Init h_PID vars
    h_Kp      = 0.0015;
    h_Ki      = 0.0002;
    h_Kd      = 0.0001;

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
    
    //========================================================================
    //                              Main Loop     
    //========================================================================
    while( TRUE ){

		if( tmr0_XHz_flag ) {
			tmr0_XHz_flag = FALSE;
			d_sec ++;
			if( d_sec > 4 ) {
    			d_sec = 0;
			    output_toggle( gLED );
			}
			
			//----------------------------------------------------------------
			//  Update QEI
			* ( ( (char *) & POSCNT ) + 1 ) = PIC_POSCNTH;
			* ( ( (char *) & POSCNT ) )     = PIC_POSCNTL;
		    encPos  = encRev;
		    encPos *= 3000;
		    encPos += POSCNT;	
		    //if( encPos != encPosOld ) {
		    //    printf( ":POSCNT = %ld|REVCNT=%ld|POSTOT=%ld#", POSCNT, encRev, encPos );
		    //}
		    encPosOld = encPos;		
		}

        if( i2c_rx_flag  ) {
            if( mem_add < NUM_DATA ) {
                data[mem_add] = * (signed int32 *) buffer;
            }
            i2c_rx_flag = false;
        }
        
        if( i2c_tx_flag ) {
            if( mem_add < NUM_DATA ) {
                if( mem_add == 1 ) {
                    //data[1] = (signed int32) avg;
                } else if( mem_add == 2 ) {
                    data[2] = POSCNT;
                } else if( mem_add == 3 ) {
                    data[3] = encRev;
                } else if( mem_add == 4 ) {
                    data[4] = encPos;
                } else if( mem_add == 5 ) {
                    //data[5] = encVel;
                } else if( mem_add == 6 ) {
                    //data[6] = (signed int32) m_V;
                } else if( mem_add == 7 ) {
                    //data[7] = diffPos;
                } 
                pointer = (char *) & data[mem_add];
            } else {
                mem_add -= NUM_DATA;
                if( mem_add == 1 ) {
                    tmp = v_Kp * 100000.0;
                    pram[1] = (signed int32) tmp;
                } else if( mem_add == 2 ) {
                    tmp = v_Ki * 100000.0;
                    pram[2] = (signed int32) tmp;
                } else if( mem_add == 3 ) {
                    tmp = v_Kd * 100000.0;
                    pram[3] = (signed int32) tmp;
                } else if( mem_add == 4 ) {
                    tmp = h_Ki * 100000.0;
                    pram[4] = (signed int32) tmp;
                } else if( mem_add == 5 ) {
                    tmp = h_Kp * 100000.0;
                    pram[5] = (signed int32) tmp;
                } else if( mem_add == 6 ) {
                    tmp = h_Kd * 100000.0;
                    pram[6] = (signed int32) tmp;
                } else if( mem_add == 7 ) {
                    pram[7] = 0;
                }
                pointer = (char *) & pram[mem_add];
            }
            WriteI2C( pointer[0] );
            i2c_tx_flag = false;
        }   
        
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
