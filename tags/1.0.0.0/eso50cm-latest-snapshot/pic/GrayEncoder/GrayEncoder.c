/** ***************************************************************************
 * 2010 03 25 16:10
 * GrayEncoder works like TestPWM:
 * 1) receive memory_address where save/recall data
 * 2) save number (int32) sent by master in data[memory_address] 
 *    transmit data[memory_address] when master ask for it.
 * 3) Do fancy things with timer1 and data[]
 *
 * 2010 06 21
 * New functions
 * o grayCodeToBinary( int16 Xg )
 *   Transforms the 10bits gray code to 10 bits number.
 *   The result is stored in data[0];
 *
 * 2010 06 22
 * Working
 * o enc_total: acumulative encoder position, positive ha > 0
 * o enc_dir;
 * o added debouncing to encoder lecture, data[] is updates only if a new 
 *   data was read.
 * o data[0] = encB
 * o data[1] = enc_total
 * o gLED on <=> enc_dir = 1
 *   yLED on <=> enc_dir = 0
 *   rLED switching on off <=> encoder running 
 *   rLED period = encoder's rate of change
 *
 * 2010 04 29
 * o Solved: problem of superposition of saving and transmiting encoder data 
 *           at same time 
 *   
 **************************************************************************** */ 
#include "./GrayEncoder.h"
#define FIRMWARE_VERSION "20100406_1607"
#define gLED PIN_E0
#define yLED PIN_E1
#define rLED PIN_E2

//#priority SSP, TIMER0, AD  

//----------------------------------------------------------------------------
#include "./tmr0_18f4431.h"
#include "./i2c_18f4431.h"

//#include "./pPWM_18f4431.h"
//#include "./tmr5_18f4431.h"
//#include "./qei_18f4431.h"
//#include "./adc_18f4431.h"
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
// TMR0:
int16 tmr0cnt;
int16 DriveParams_tmr0rv;
int16 DriveParams_Tmr0;
int	  tmr0_flag;
int	  tmr0_2Hz_flag;

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
char * pointer;


//----------------------------------------------------------------------------
// Gray Encoder
int enc_dir;
int16 encB, encG, tmpG1, tmpG2, tmpG3;
int16 old_encB;
int16 encG_old, encG_or, encG_ph2;
signed int32 enc_revs;
signed int32 enc_total;
signed int32 data[6];
int enc_idx;


/** ***************************************************************************
 * TIMER0_isr
 */
#int_TIMER0
void  TIMER0_isr(void)  {
    set_timer0( DriveParams_Tmr0 );
	tmr0_flag = TRUE;
	//output_toggle( rLED );
	tmr0cnt --;
	if( tmr0cnt == 0 ) {
		tmr0cnt = DriveParams_tmr0rv;
		tmr0_2Hz_flag = TRUE;
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
 * grayCodeToBinary
 * 10 bits gray code to binary
 */
int16 grayCodeToBinary( int16 Xg ) {
    int i;
    int16 Xb, tmp;
    int16 ph2, tmp_ph2;

    ph2 = 0;
    tmp = Xg;
    tmp_ph2= Xg;
    for( i = 0; i < 10; i ++ ) {
        tmp = tmp >> 1;
        bit_clear( tmp, 9 );
        Xb = Xg ^ tmp;
        tmp = Xb;

        tmp_ph2 = tmp_ph2 >> 1;
        bit_clear( tmp_ph2, 9 );
        ph2 = ph2 ^ (0x0001 & tmp_ph2);

    }
    encG_ph2 = ph2;
    return Xb; 
}



/** ***************************************************************************
 * Main
 */
void main() {
	int pa, pd;
	int i;

    //------------------------------------------------------------------------
    //                               Setup uCU
    set_tris_a( 0b11111111 );           //RA0, RA1, QEIndex, QEA, QEB are inputs
	set_tris_b( 0b11111111 );           //PORTB all inputs
    set_tris_c( 0b11111111 );           //PORTC all inputs
    set_tris_d( 0xFF );                 //PORTD all inputs
    set_tris_e( 0x00 );                 //PORTE all outputs
 
    //setup_wdt( WDT_OFF );

    //setup_timer_1( T1_DISABLED );
    //setup_timer_2( T2_DISABLED, 0, 1 );
    
    //SetupPowerPWM();
    //SetupTMR5();
    //SetupQEI();
    //SetupADC();

   
    //------------------- initial output -----------------------
    //output_d( 0x00 );
    //output_e( 0x00 );

    //------------------------------------------------------------------------
    //  TMR0 Vars; 
    SetupTMR0();
	//DriveParams.Tmr0    = 63035;	    // =>1000[Hz] (tmr0  = 0.4us)
	//DriveParams.tmr0rv  = 5;							// => 200[Hz]
	
	//DriveParams_Tmr0     = 40535;		// =>100[Hz]  (tmr0  = 0.4us Xtal= 20MHz / 4 / 2 )
	//DriveParams_tmr0rv   = 10;		// => 10[Hz]

	//DriveParams_Tmr0     = 15535;		// =>100[Hz]  (tmr0  = 0.2us Xtal= 10MHz / 4 / 2 * 4 )
	//DriveParams_tmr0rv   = 50;		// => 2[Hz]

	DriveParams_Tmr0     = 60535;		// =>1000[Hz]  (tmr0  = 0.2us Xtal= 10MHz / 4 / 2 * 4 )
	DriveParams_tmr0rv   = 500;			// => 2[Hz]
	set_timer0( DriveParams_Tmr0 );
	tmr0cnt = DriveParams_tmr0rv;

    //------------------------------------------------------------------------
    //    Enable Interruptions
    enable_interrupts(INT_TIMER0);      //TMR0
    enable_interrupts(INT_SSP);         //I2C

    //enable_interrupts(INT_IC2QEI);      // ??
    //enable_interrupts(INT_CCP1);        // ??
    //enable_interrupts(INT_IC1);         //Input Capture 1: QEI Mode/Velocity Mode =>  Velocity  Reg Updated
    //enable_interrupts(INT_PWMTB);       //PWM Base Time
    //enable_interrupts(INT_TIMER5);
    //enable_interrupts(INT_AD);

    enable_interrupts(GLOBAL);

    //------------------------------------------------------------------------
    //                              Test Loop
    output_low( rLED );
    output_low( yLED );
    output_low( gLED );
    
	i = 2;
	while( i > 0 ) { 
		if( tmr0_2Hz_flag )	{
			tmr0_2Hz_flag = FALSE;
			output_toggle( gLED );
			i --;
		}
	}	
	i = 2;
	while( i > 0 ) { 
		if( tmr0_2Hz_flag )	{
			tmr0_2Hz_flag = FALSE;
			output_toggle( yLED );
			i --;
		}
	}	
	i = 2;
	while( i > 0 ) { 
		if( tmr0_2Hz_flag )	{
			tmr0_2Hz_flag = FALSE;
			output_toggle( rLED );
			i --;
		}
	}	

  
    //------------------------------------------------------------------------
    //                              Main Loop 
	tmr0_flag = FALSE;
	i2c_rx_flag = false;
	i2c_tx_flag = FALSE;

	
	pd  = PORTD;
	pa  = 0b00000011& PORTA;
	encG = make16( pa, pd );
	encB = grayCodeToBinary( encG );
    enc_total = encB;
    
    output_high( gLED );
    while( TRUE ){
    	pd  = PORTD;
    	pa  = 0b00000011& PORTA;
    	tmpG1= make16( pa, pd );
    	
    	pd  = PORTD;
    	pa  = 0b00000011& PORTA;
    	tmpG2= make16( pa, pd );

    	pd  = PORTD;
    	pa  = 0b00000011& PORTA;
    	tmpG3= make16( pa, pd );
    	
    	if( tmpG1 == tmpG2 && tmpG2 == tmpG3 ){
        	
        	encG = tmpG3;
        	encG_or = encG ^ encG_old;
    	    encB = grayCodeToBinary( encG );
    	    encG_old = encG;
        }    	
    	 

        switch( encG_or  ) {
			case 0:
				break;
			case 1:
				if( bit_test( encG, 0 ) ) {
					if( encG_ph2 == 0 ) {
						enc_dir = 0; //positivo
						enc_total ++;
					} else {
						enc_dir = 1; //negativo
						enc_total --;
					}
				} else {
					if( encG_ph2 == 0 ) {
						enc_dir = 1; //negativo
						enc_total --;
					} else {
						enc_dir = 0; //positivo
						enc_total ++;
					}
				}
				output_toggle( rLED );
				break;
			case 2:
			case 4:
			case 8:
			case 16:
			case 32:
			case 64:
			case 128:
			case 256:
			case 512:
				if( encG_ph2 == 0 ) {
					if( bit_test( encG, 0 ) ) {
						enc_dir = 1; //negativo
						enc_total --;
					} else {
						enc_dir = 0; // positivo
						enc_total ++;
					}
				} else {
					if( bit_test( encG, 0 ) ) {
						enc_dir = 0; //positivo
						enc_total ++;
					} else {
						enc_dir = 1; //negativo
						enc_total --;
					}
				}
				output_toggle( rLED );
				break;
		}

        
        
        if( enc_dir ) {
            output_high( gLED );
            //output_low( yLED );
        } else {    
            //output_low( gLED );
            output_high( yLED );
        }
        
        
		if( tmr0_flag ) {
			tmr0_flag = FALSE;
			//output_toggle( rLED );
		}
		
	    if( tmr0_2Hz_flag ) {
		    tmr0_2Hz_flag = FALSE;
	        if( enc_dir ) {
		        output_toggle( yLED );
		    } else {
		        output_toggle( gLED );
		    }    
		}     

        if( i2c_rx_flag ) {
            i2c_rx_flag = false;
            if( mem_add == 1 ) {
                data[mem_add] = * (signed int32 *) buffer;
            }  else if( mem_add == 2 ) {
                data[mem_add] = * (signed int32 *) buffer;      
            }  else if( mem_add == 3 ) {
                data[mem_add] = * (signed int32 *) buffer;
                enc_revs = data[mem_add];
                enc_total  = 1024;
                enc_total *= enc_revs;
                enc_total += encB;
            }  else if( mem_add == 4 ) {
                data[mem_add] = * (signed int32 *) buffer;
            }
                  
        }
        
        if( i2c_tx_flag ) {
            if( mem_add == 2 ) {
                data[2] = encB;
            } else if( mem_add == 3 ) {
                enc_revs = enc_total / 1024;
                if( enc_total < 0 ) {
                    enc_revs -= 1;
                }
                data[3] = enc_revs;
            } else if( mem_add == 4 ) {
                data[4] = enc_total;
            }
            pointer = (char *) & data[mem_add];
            WriteI2C( pointer[0] );
            i2c_tx_flag = false;
        }    
            
    }
}
