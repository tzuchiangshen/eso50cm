/**
 *                           -------------------
 *           < MCLR |-------| MCLR          RB7 |
 *                          | RA0           RB6 |
 *                          | RA1           RB5 |
 *                | >-------| RA2     P     RB4 |-------| >
 *                | >-------| RA3     I     RB3 |-------| > 
 *                | >-------| RA4     C     RB2 |-------| >
 *                          | RA5           RB1 |-------| SCL > (Yellow)
 *            < gLED|-------| RE0     1     RB0 |-------| SDA > (Green)
 *            < yLED|-------| RE1     8     Vdd |
 *            < rLED|-------| RE2     F     Vss |
 *                          | Vdd     4     RD7 |-------| >
 *                          | Vss     5     RD6 |-------| >
 *                          | OSC1    5     RD5 |-------| >
 *                          | OSC2    0     RD4 |
 *                 <|-------| RC0           RC7 |-------< RX  |
 *                 <|-------| RC1           RC6 |-------| TX  >
 *                          | RC2           RC5 |-------< USB >
 *        <USB +3.3V|-------| RC3           RC4 |-------< USB >
 *                < |-------| RD0           RD3 |-------| > 
 *                < |-------| RD1           RD2 |-------| >                         
 *                           -------------------
 *
 *
 *
 */ 
#include <18F4550.h>
#fuses HSPLL,NOWDT,NOPROTECT,NOLVP,NODEBUG,USBDIV,PLL5,VREGEN,NOPUT
#use delay(clock=48000000)
//#use i2c(master, SDA=PIN_B0, SCL=PIN_B1, force_hw)
#use rs232(baud=57600, xmit=PIN_C6, rcv=PIN_C7)

#include <string.h>
#include <stdlib.h>
#include "LCD_4x16.h"
#define version 0x0004
#pragma id version
#define compiler __PCH__

#define __TEST_UCU__
//#undef  __TEST_UCU__


#byte   PIC_SSPCON = 0xFC6
#byte   PIC_SSPCON2= 0xFC5
#byte   PIC_SSPADD = 0xFC8

#byte   PORTA = 0xF80
#byte   PORTB = 0xF81
#byte   PORTC = 0xF82
#byte   PORTD = 0xF83
#byte   PORTE = 0xF84


#define B_SLEW      PIN_B0
#define B_SET       PIN_B1
#define B_GUIDE     PIN_B2
#define B_OFFSET    PIN_B3
#define B_DELTA_N   PIN_B4
#define B_DELTA_P   PIN_B5
#define B_ALPHA_N   PIN_B6
#define B_ALPHA_P   PIN_B7

#define L_SLEW      PIN_A0
#define L_SET       PIN_A1
#define L_GUIDE     PIN_A2
#define L_OFFSET    PIN_A3
#define L_DELTA_N   PIN_A4
#define L_DELTA_P   PIN_A5
#define L_ALPHA_N   PIN_E0
#define L_ALPHA_P   PIN_E1

#define LED_ON  output_high
#define LED_OFF output_low

//----------------------------------------------------------------------------
// botones/leds vars
char botones;
char leds, leds_old, spds_leds_old, dirs_leds_old, ledsA, ledsE;
int  spds_leds_debounce;
int  dirs_leds_debounce;
//----------------------------------------------------------------------------
// tty vars
#define  TTY_BUF_LEN        16
char    tty_buffer[TTY_BUF_LEN];
char    msg_buffer[TTY_BUF_LEN];
int     msg_len;
int     tty_start_byte;
int     tty_end_byte;
int     tty_seek_byte;

//----------------------------------------------------------------------------
// tmr0 vars
int16 myTmr0_cnt;
int16 myTmr0_rv;
int16 myTmr0;
int	  myTmr0_flag;
int	  myTmr0_8Hz_flag;

//----------------------------------------------------------------------------
// tmr1 vars
int16 myTmr1_cnt;
int16 myTmr1_rv;
int16 myTmr1;
int myTmr1_flag;
int	myTmr1_100Hz_flag;

//----------------------------------------------------------------------------
// LCD Display
char display[4][16];
char display_old[4][16];


/** ************************************************************************** 
 *  Prototypes
 */
void textMessage( char * buffer, int length );
void binaryReadMessage( char * buffer );
void binaryWriteMessage( char * buffer ); 


/** ************************************************************************** 
 *  TIMER0_ISR
 */
#int_TIMER0
void  TIMER0_isr(void)  {
    set_timer0( myTmr0 );
	myTmr0_flag = TRUE;
	myTmr0_cnt --;
	if( myTmr0_cnt == 0 ) {
		myTmr0_cnt = myTmr0_rv;
		myTmr0_8Hz_flag = TRUE;
    }
}

/** ************************************************************************** 
 *  TIMER1_ISR
 */
#INT_TIMER1
void  TIMER1_isr(void)  {
    set_timer1( myTmr1 );
	myTmr1_flag = TRUE;
	myTmr1_cnt --;
	if( myTmr1_cnt == 0 ) {
		myTmr1_cnt = myTmr1_rv;
		myTmr1_100Hz_flag = TRUE;
    }
}

/** ************************************************************************** 
 *  RDA_isr
 */
#int_RDA
void  RDA_isr( void ) { 
    tty_buffer[tty_seek_byte] = getc();;
    tty_seek_byte ++;
    tty_seek_byte %= TTY_BUF_LEN;
}



/** ************************************************************************** 
 *  textMessage
 */
void textMessage( char * buffer, int length  ) {
	int i;
	signed int32 numero;

    switch( buffer[1] ) {
        case 'V':
            switch( buffer[2] ) {
                case 'N':
                    printf( ":VN%05.2f#", version/100.0 );    
                    break;
                case 'T':  
                    printf( ":VT%s#", __TIME__ );  
                    break;
                case 'D':
                    printf( ":VD%s#", __DATE__ );    
                    break;
                default:
                    printf( ":Unknown command %s", & buffer[2] );  
            }
            break; 
        case ' ':   
            buffer[length-1]= 0;
            printf( ":%s#", & buffer[2] );   
            break;   
        default:
            printf( ":Unknown command %c#", buffer[1] );   
    }
}


/** ************************************************************************** 
 *  Main
 */
void main( void )  {
    int i, j, k;
    char c;
	
    int tc;
    int sec, min, hrs;
    
    char string[TTY_BUF_LEN];
    
    //------------------------------------------------------------------------
    //      setup uCU
    set_tris_a( 0x00);                          // PORTA all outputs
    set_tris_b( 0xFF );                         // PORTB all inputs
    set_tris_c( 0xC0 );                         // PORTC all in/out-puts
    set_tris_d( 0x00 );                         // PORTD all outputs
    set_tris_e( 0x00 );                         // PORTE all outputs

    //------------------------------------------------------------------------
    //      setup tmr0
    // PS    = RTCC_DIV_X
    // HS    = 20[MHz]/PLL5 = 4[Mhz] -->PLL--> 96[MHz] 
    // Fosc = 96[MHz]/CPUDIV2 = 48[MHz]
    // HSPLL = Fosc/4 = 12[MHz].... 1/(Fosc/4) = 1/12[us]
    //      o                1    
    //          DTmr0 = ( -------- ) * PS * N    ( PS = 4 )
    //                    (Fosc/4)
    //
    //        donde N es el numero de pasos para que el contador de TMR0 haga 
    //        overflow, i.e: ( 0xFFFF - N )  
    //      o Para 1000[Hz], con Fosc/4 = 12[Mhz] y PS = 2
    //                  
    //                    1        (Fosc/4)               12 * 10^6
    //          N =   --------- * ---------- =  10^-3 * -------------- 
    //                  1000          2                      2
    //         
    //          N = = 3000
    //
    setup_timer_0(RTCC_INTERNAL|RTCC_DIV_2);
	myTmr0     = 59535;		                    // => 1000[Hz]  
	myTmr0_rv  = 125;			                // => 8[Hz]
    myTmr0_8Hz_flag = FALSE;
    myTmr0_flag = FALSE;
    
	myTmr0_cnt = myTmr0_rv;
    set_timer0( myTmr0 );
    enable_interrupts( INT_TIMER0 );
    

    //------------------------------------------------------------------------
    //      setup tmr1
    setup_timer_1(T1_INTERNAL|T1_DIV_BY_2);
	myTmr1     = 59535;		                    // => 1000[Hz]  
	myTmr1_rv  = 10;			                // => 100[Hz]
    myTmr1_100Hz_flag = FALSE;
    myTmr1_flag = FALSE;

    myTmr1_cnt = myTmr1_rv;                   
    set_timer1( myTmr1 );   
    enable_interrupts( INT_TIMER1 );

    //------------------------------------------------------------------------
    //  Enable RS232 interruption
    enable_interrupts( int_rda );

    //-------------------------------------------------------------------------
    // Init LCD
    lcd_init();
    lcd_clear_display();

    //------------------------------------------------------------------------
    //  Inicializacion Global Interruptions
    enable_interrupts( GLOBAL );

 
    //-------------------------------------------------------------------------
    //RS232 Test
    //printf( "ESO50cm Firmware\n" );
    //printf( "Firmware version %5.2f\n", version/100.0  );
    //printf( "Compiled on %s %s \r\n", __DATE__, __TIME__ );
    //printf( "with compiler version %s\r\n", compiler );
    //printf( "File %s\r\n",__FILE__ );

    //-------------------------------------------------------------------------
    //LCD Test
    sprintf( display[0], "FV: %5.2f", version/100.0 );
    lcd_goto_xy( 1, 1 );
    lcd_print( display[0] );
    sprintf( display[1], "FD: %s", __DATE__ );
    lcd_goto_xy( 1, 2 );
    lcd_print( display[1] );
    sprintf( display[2], "FT: %s", __TIME__ );
    lcd_goto_xy( 1, 3 );
    lcd_print( display[2] );

    //------------------------------------------------------------------------
    //      Test uCU
#ifdef __TEST_UCU__
    //output_high( PIN_E2 );
    for( k = 0; k < 3; k ++ ){
        output_low( PIN_A0 );
        output_low( PIN_A1 );
        output_low( PIN_A2 );
        output_low( PIN_A3 );
        output_low( PIN_A4 );
        output_low( PIN_A5 );
        output_low( PIN_E0 );
        output_low( PIN_E1 );
        delay_ms( 250 );
        output_high( PIN_A0 );
        output_high( PIN_A1 );
        output_high( PIN_A2 );
        output_high( PIN_A3 );
        output_high( PIN_A4 );
        output_high( PIN_A5 );
        output_high( PIN_E0 );
        output_high( PIN_E1 );
        delay_ms(750);
    }
#endif
    //lcd_clear_display();
    
/*
#ifdef __TEST_UCU__
    //------------------------------------------------------------------------
    // Test Tmr0
    LED_OFF(rLED);
    k = 10;
    while( k > 0 ) {
        if( myTmr0_4Hz_flag ) {
            myTmr0_4Hz_flag = FALSE;
            output_toggle(rLED);
            k --;
        }    
    }    
    delay_ms( 1000 );
    //------------------------------------------------------------------------
    // Test Tmr1
    LED_OFF(rLED);
    k = 10;
    while( k > 0 ) {
        if( myTmr1_2Hz_flag ) {
            myTmr1_2Hz_flag = FALSE;
            output_toggle(rLED);
            k --;
        }    
    }    
    delay_ms( 1000 );
#endif
*/   

    //------------------------------------------------------------------------
    //      Init VArs Main Loop

	i = 0;
	j = 0;
	k = 0;
    
    for( i = 0; i < 4; i ++ ) {
        for( j = 0; j < 16; j ++ ) {
            display[i][j] = ' ';
            display_old[i][j] = ' ';
            lcd_goto_xy( j+1, i+1 );
            lcd_putc( display[i][j] );
        }
    }
                
    // tty vars
    for( k = 0; k < TTY_BUF_LEN; k ++ ) {
		tty_buffer[k] = 0;
		msg_buffer[k] = 0;
	}
	
    tc  = 0;
    sec = 0;
    min = 0;
    hrs = 0;
    
    tty_start_byte = 0;
    tty_end_byte   = 0;
    tty_seek_byte  = 0;
    PORTA    = 0xFF;
    PORTE    = 0xFF;
    leds     = 0xFF;
    leds_old = leds;
    spds_leds_old      = 0x0F;
    dirs_leds_old      = 0xF0;
    spds_leds_debounce = 0;
    dirs_leds_debounce = 0;
    //------------------------------------------------------------------------
    // Main Loop
    while( TRUE ) {
        if( myTmr1_100Hz_flag ) {
            myTmr1_100Hz_flag = false;
            if( botones == PORTB ) {
                if( spds_leds_debounce == 0 ) {
                    for( k = 0; k < 4; k ++ ) {
                        if( ! bit_test( botones, k ) ) {
                            if( bit_test( leds, k ) ) {
                                bit_clear( leds, k );
                            } else {
                                bit_set( leds, k );
                            }
                        }
                    }
                    if( spds_leds_old != (0x0F & leds) ) {
                        spds_leds_debounce = 50;
                        spds_leds_old = (0x0F & leds);
                    }
                } else {
                    spds_leds_debounce --;
                }
                if( dirs_leds_debounce == 0 ) {
                    for( k = 4; k < 8; k ++ ) {
                        if( ! bit_test( botones, k ) ) {
                            bit_clear( leds, k );
                        } else {
                            bit_set( leds, k );
                        }
                    }
                    if( dirs_leds_old != (0xF0 & leds) ) {
                        dirs_leds_debounce = 10;
                        dirs_leds_old = (0xF0 & leds);
                    }                    
                } else {
                    dirs_leds_debounce --;
                }
            }
            leds  = dirs_leds_old | spds_leds_old;
            ledsA = 0b00111111 & leds;
            ledsE = 0b11000000 & leds;
            ledsE = ledsE >> 6;
            PORTA = ledsA;
            PORTE = ledsE;
            
            botones = PORTB;
        }      
        sprintf( & display[3][8], "%02d:%02d:%02d", hrs, min, sec );
        if( myTmr0_8Hz_flag ) {
            myTmr0_8Hz_flag = FALSE;
            if( leds ^ leds_old ) {
                if( bit_test( leds ^ leds_old, 0 ) ) {
                    if( ! bit_test( leds, 0 ) ) {
                        sprintf( display[3], "SLEW    " );
                        printf( ":SwS#" );
                    } else {
                        sprintf( display[3], "        " );
                        printf( ":Sw0#" );
                    }     
                } else if( bit_test( leds ^ leds_old, 1 ) ) {
                    if( ! bit_test( leds, 1 ) ) {
                        sprintf( display[3], "SET     " );
                        printf( ":Sws#" );
                    } else {
                        sprintf( display[3], "        " );
                        printf( ":Sw0#" );
                    }     
                } else if( bit_test( leds ^ leds_old, 2 ) ) {
                    if( ! bit_test( leds, 2 ) ) {
                        sprintf( display[3], "GUIDE   " );
                        printf( ":SwG#" );
                    } else {
                        sprintf( display[3], "        " );
                        printf( ":Sw0#" );
                    }     
                } else if( bit_test( leds ^ leds_old, 3 ) ) {
                    if( ! bit_test( leds, 3 ) ) {
                        sprintf( display[3], "OFFSET  " );
                        printf( ":SwO#" );
                    } else {
                        sprintf( display[3], "        " );
                        printf( ":Sw0#" );
                    }     
                }
                if( bit_test( leds ^ leds_old, 7 ) ) {
                    if( ! bit_test( leds, 7 ) ) {
                        display[1][13] = 'W';
                        printf( ":Mw#" );
                    } else {
                        display[1][13] = ' ';
                        printf( ":Qw#" );
                    }
                }
                if( bit_test( leds ^ leds_old, 6 ) ) {
                    if( ! bit_test( leds, 6 ) ) {
                        display[1][15] = 'E';
                        printf( ":Me#" );
                    } else {
                        display[1][15] = ' ';
                        printf( ":Qe#" );
                    }        
                }    
                if( bit_test( leds ^ leds_old, 5 ) ) {
                    if( ! bit_test( leds, 5 ) ) {
                        display[0][14] = 'N';
                        printf( ":Mn#" );
                    } else {
                        display[0][14] = ' ';
                        printf( ":Qn#" );
                    }
                }
                if( bit_test( leds ^ leds_old, 4 ) ) {
                    if( ! bit_test( leds, 4 ) ) {
                        display[2][14] = 'S';
                        printf( ":Ms#" );
                    } else {
                        display[2][14] = ' ';
                        printf( ":Qs#" );
                    }
                }
                leds_old = leds;
            }
            
            for( i = 0; i < 4; i ++ ) {
                for( j = 0; j < 16; j ++ ) {
                    if( display_old[i][j] != display[i][j] ){
                        lcd_goto_xy( j+1, i+1 );
                        lcd_putc( display[i][j] );
                        display_old[i][j] = display[i][j];
                    }
                }
            }
            if( tc % 2 == 0 ) {
            }   
            if( tc % 8 == 0 ) {
                sec ++;
                if( sec == 60 ) {
                    sec = 0;
                    min ++;
                    if( min == 60 ) {
                        min = 0;
                        hrs ++;
                        if( hrs == 24 ) {
                            hrs = 0;
                        }
                    }
                }
            }    
            tc ++;
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
                    }
                } else {
                    msg_buffer[msg_len] = tty_buffer[i];
                    msg_len ++;
                    if( msg_buffer[1] < 0xA0 ) {
                        if( tty_buffer[i] == '#' ) {
                            msg_buffer[msg_len]= 0;
            				textMessage( msg_buffer, msg_len );
                            msg_len = 0;
                        }    
                    } 
                }
            }
            tty_start_byte  = i;
            tty_start_byte %= TTY_BUF_LEN;
        } 
        
    } 
}//END PROGRAM
