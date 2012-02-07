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
#use i2c(master, SDA=PIN_B0, SCL=PIN_B1, force_hw)
#use rs232(baud=57600, xmit=PIN_C6, rcv=PIN_C7)

#include <string.h>
#include <stdlib.h>
#include "LCD_4x16.h"
#define version 0x0004
#pragma id version
#define compiler __PCH__

//#define __DUMMY_TELESCOPE__
//#define __LCD_4x16__
//#define __PIC40_USB__
#define __TEST_UCU__
#undef  __TEST_UCU__


#byte   PIC_SSPCON = 0xFC6
#byte   PIC_SSPCON2= 0xFC5
#byte   PIC_SSPADD = 0xFC8

#byte   PORTA = 0xF80
#byte   PORTB = 0xF81
#byte   PORTC = 0xF82
#byte   PORTD = 0xF83
#byte   PORTE = 0xF84

#define DEV_SDA  PIN_B0
#define DEV_SCL  PIN_B1

#define rLED PIN_E1

#define alphaM  1
#define deltaM  2
#define alphaW  3
#define alphaA  4
#define deltaW  5
#define deltaA  6

#define LED_ON  output_high
#define LED_OFF output_low

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
int	myTmr1_2Hz_flag;

//----------------------------------------------------------------------------
// I2C vars
#define NUM_I2C_DEV     7

int i2c_address;
//int i2c_ack;
int i2c_memory;
int i2c_device[NUM_I2C_DEV];
int i2c_status[NUM_I2C_DEV];
char i2c_buffer[TTY_BUF_LEN];

int scan_alpha_worm_encoder_flag;
int scan_alpha_axis_encoder_flag;
int scan_delta_worm_encoder_flag;
int scan_delta_axis_encoder_flag;

//----------------------------------------------------------------------------
// Dummy Telesope vars
signed int32 data[NUM_I2C_DEV][6];


/** ************************************************************************** 
 *  Prototypes
 */
int test_i2c_device( int dev );
int write_i2c_data( char * buffer );
int read_i2c_data( char * buffer );

int scan_encoder( int i );

void textMessage( char * buffer, int length );
void binaryReadMessage( char * buffer );
void binaryWriteMessage( char * buffer ); 

void init_dummy_telescope( void );
void update_dummy_telescope( void );

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
		myTmr1_2Hz_flag = TRUE;
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
 * test_i2c_device
 * @param dev (int) i2c_address (even).
 * @return ack (int): 0 device present, 1 device not responding. 
 */
int test_i2c_device( int dev ) {
    int ack;

    ack = 0;
    return ack;
}

/** **************************************************************************
 * write_12c_data
 * @param buffer (char *).
 * @return ack (int): 0 operation was succesfull, > 0 device not responding. 
 */
int write_i2c_data( char * buffer ) {
    int ack;
    int i;
    ack = 0;
    for( i = 1; i < NUM_I2C_DEV; i ++ ) {
        if( i2c_device[i] == (0xFE & buffer[0]) ) {
            break;
        }
    }        
    data[i][buffer[1]] = * (signed int32 *) & buffer[2];
    return ack;
} 

/** **************************************************************************
 * read_12c_data
 * @param buffer (char *).
 * @return ack (int): 0 operation was succesfull, > 0 device not responding. 
 * Probably the function hangs if the device becomes off line during the
 * reading operation.
 */
int read_i2c_data( char * buffer ) {
    int ack;
    int i;
                
    ack = 0;
    for( i = 1; i < NUM_I2C_DEV; i ++ ) {
        if( i2c_device[i] == (0xFE & buffer[0]) ) {
            break;
        }
    }        
    * (signed int32 *) & buffer[2] = data[i][buffer[1]];
    return ack;
} 

/** ************************************************************************** 
 *  init_dummy_telescope
 */
void init_dummy_telescope( void ) {
    //Motors 
    //Alpha Motor
    //Q.Encoder
    data[alphaM][4] = 0;
    //Q.Encoder Rortation Rate Reference
    data[alphaM][1] = 0;           //300[tics/second] = Sidereal Rate
    //Delta Motor
    //Q.Encoder
    data[deltaM][4] = 0;
    //Q.Encoder Rortation Rate Reference
    data[deltaM][1] = 0;           //240[tics/second] = Sidereal Rate
    //Encoders Home position
    data[alphaW][4] = 0;
    data[alphaA][4] = 512;
    data[deltaW][4] = 0;
    data[deltaA][4] = 512;
}    

/** ************************************************************************** 
 *  update_dummy_telescope
 */
void update_dummy_telescope( void ) {
    signed int32 tmp;

    //Alpha Q. Encoder
    tmp =  data[alphaM][1];
    tmp /= 8;                   //Tmr0_8Hz_Flag
    data[alphaM][4] += tmp;
    data[alphaM][5] = data[alphaM][4] / 3000;    
    
    //Alpha Worm
    tmp = data[alphaM][4];
    tmp /= 48;
    data[alphaW][4] = tmp;
    
    //Alpha Axis
    tmp = data[alphaW][4];
    tmp /= 240;                 //alpha axiss to alpha axis encoder ratio 
    data[alphaA][4] = tmp;
    data[alphaA][4] += 512;     //initial position
    
    //Delta Q. Encoder
    tmp =  data[deltaM][1];
    tmp /= 8;                   //Tmr0_8Hz_Flag
    data[deltaM][4] += tmp;
    data[deltaM][5] = data[deltaM][4] / 3000;    
    
    //delta Worm
    tmp = data[deltaM][4];
    tmp /= 48;
    data[deltaW][4] = tmp;
    
    //Delta Axis
    tmp = data[deltaW][4];
    tmp /= 288;                 //delta axiss to alpha delta encoder ratio 
    data[deltaA][4] = tmp;
    data[deltaA][4] += 512;     //initial position
}    


/** ************************************************************************** 
 *  scan_encoder
 */
int scan_encoder( int i ) {
    //int retval;
    char buffer[8];
    buffer[0] = i2c_device[i]+1;
    buffer[1] = 4;
    if( (i2c_status[i] = read_i2c_data( buffer )) == 0 ){
        putc( ':' );
        putc( buffer[0] );
        putc( buffer[1] );
        putc( buffer[2] );
        putc( buffer[3] );
        putc( buffer[4] );
        putc( buffer[5] );
        putc( '#' );        
    } else {
        putc( ':' );
        putc( i2c_device[i] );
        putc( i2c_status[i] );
        putc( '#' );            
    }
    return i2c_status[i];
}     

/** ************************************************************************** 
 *  textMessage
 */
void textMessage( char * buffer, int length  ) {
	int i;
	signed int32 numero;

    switch( buffer[1] ) {
        case 't':
            if( buffer[2] == '#' ){
                printf( ":" );
                for( i = 1; i < NUM_I2C_DEV; i ++ ) {
                    i2c_status[i] = test_i2c_device( i2c_device[i] );
                    printf( "[0x%X=%d]", i2c_device[i], i2c_status[i] );
                }
                printf( "#" );
            } else {
                i = 0;
                if( buffer[2] == '2' || buffer[2] == '4' || buffer[2] == '6'|| buffer[2] == '8' ) {
                    i = buffer[2] - '0';   
                    i /= 2;
                } else if( buffer[2] == 'A' || buffer[2] == 'C'  ) {
                    i = buffer[2] - 'A';   
                    i += 10;
                    i /= 2;
                }
                if( 0 < i && i < NUM_I2C_DEV ) {
                    printf( ":Testing Device 0x%X#", i2c_device[i] );
                    i2c_status[i] = test_i2c_device( i2c_device[i] );
                    printf( ":[0x%X=%d]#", i2c_device[i], i2c_status[i] );
                } else {     
                    printf( ":Wrong device %c#", buffer[2] );
                }    
            }    
            break;
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
        case 'W': 
            i = 0;
            if( buffer[2] == '2' || buffer[2] == '4' || buffer[2] == '6'|| buffer[2] == '8' ) {
                i = buffer[2] - '0';   
                i /= 2;
            } else if( buffer[2] == 'A' || buffer[2] == 'C'  ) {
                i = buffer[2] - 'A';   
                i += 10;
                i /= 2;
            }
            if( 0 < i && i < NUM_I2C_DEV ) {
                printf( ":Write 0x%2X", i2c_device[i] );
                i2c_memory = buffer[3] - '0';
                if( i2c_memory < 10 ) {
                    buffer[length-1]= 0;
                    numero = atoi32( & buffer[4] );
                    printf( "[%d]=%ld#", i2c_memory, numero ); 
                    i2c_buffer[0]= i2c_device[i];
                    i2c_buffer[1]= i2c_memory;
                    * (signed int32 *) & i2c_buffer[2] = numero;
                    printf( ":Writing 0x%X %d %ld...#", i2c_buffer[0], i2c_buffer[1], * (signed int32 *) & i2c_buffer[2] );
                    i2c_status[i] = write_i2c_data( i2c_buffer );
                    printf( ":[0x%2X=%d]#", i2c_device[i], i2c_status[i] );
                } else {     
                    printf( ", Wrong mem address %c#", buffer[3] );
                }
            } else {
                printf( ":Wrong device %c#", buffer[2]  );     
            }    
            break;       
        case 'R':
            i = 0;
            if( buffer[2] == '2' || buffer[2] == '4' || buffer[2] == '6'|| buffer[2] == '8' ) {
                i = buffer[2] - '0';   
                i /= 2;
            } else if( buffer[2] == 'A' || buffer[2] == 'C'  ) {
                i = buffer[2] - 'A';   
                i += 10;
                i /= 2;
            }
            if( 0 < i && i < NUM_I2C_DEV ) {
                printf( ":Read 0x%X", i2c_device[i] );
                i2c_memory = buffer[3] - '0';
                if( i2c_memory < 10 ) {
                    printf( "[%d]#", i2c_memory ); 
                    i2c_buffer[0]= i2c_device[i] + 1;
                    i2c_buffer[1]= i2c_memory;
                    printf( ":Reading 0x%X %d...#", i2c_buffer[0], i2c_buffer[1] );
                    i2c_status[i] = read_i2c_data( i2c_buffer );
                    if( i2c_status[i] == 0 ) {
                        i2c_buffer[6]= 0;
                        numero = * (signed  int32 *) & i2c_buffer[2];
                        printf( ":0x%2X[%d]=%ld#", i2c_device[i], i2c_memory, numero );
                    } else {                
                        printf( ":[0x%2X=%d]#", i2c_device[i], i2c_status[i] );
                    }    
                } else {     
                    printf( ", Wrong mem address %c#", buffer[3] );
                }
            } else {
                printf( ":Wrong device %c#", buffer[2]  );     
            }    
            break;
        
        case 'S':
            if( buffer[2] == 'A' ) {
                printf( ":Scanning alpha"  ); 
                if( buffer[3] == 'A' ) {
                    scan_alpha_axis_encoder_flag = 0;
                    printf( " axis#"  ); 
                } else if( buffer[3] == 'W' ) {
                    scan_alpha_worm_encoder_flag = 0;
                    printf( " worm#"  ); 
                } else {
                    printf( "wrong#" );
                }
            } else {
                printf( ":Scanning delta"  ); 
                if( buffer[2] == 'A' ) {
                    printf( " axis#"  ); 
                    scan_delta_axis_encoder_flag = 0;
                } else if( buffer[3] == 'W' ) {
                    printf( " worm#"  ); 
                    scan_delta_worm_encoder_flag = 0;
                } else {
                    printf( " wrong#"  );
                } 
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
 *  binaryReadMessage
 */
void binaryReadMessage( char * buffer ) {
	int i;

	i2c_buffer[0] = buffer[1];
	i2c_buffer[1] = buffer[2];
	for( i = 1; (i < NUM_I2C_DEV) && (i2c_device[i] != (0xFE & i2c_buffer[0])); i ++ );
    if( i <  NUM_I2C_DEV ) {
        //printf( ":Address 0x%2X detected#", i2c_device[i] );
        
        if( i2c_device[i] == 0xA6 && i2c_buffer[1] == 4 ) {
            scan_alpha_worm_encoder_flag = 0;
        } else if( i2c_device[i] == 0xA8 && i2c_buffer[1] == 4 ) {
            scan_alpha_axis_encoder_flag = 0;
        } else if( i2c_device[i] == 0xAA && i2c_buffer[1] == 4 ) {
            scan_delta_worm_encoder_flag = 0;
        } else if( i2c_device[i] == 0xAC && i2c_buffer[1] == 4 ) {
            scan_delta_axis_encoder_flag = 0;
        } else {
            i2c_status[i] = read_i2c_data( i2c_buffer );
            if( i2c_status[i] == 0 ) {
                //printf( ":Read 0x%2X[%d]=%ld#", i2c_buffer[0], i2c_buffer[1], * (signed int32 *) & i2c_buffer[2]  );
                putc( ':' );
                putc( i2c_buffer[0] );
                putc( i2c_buffer[1] );
                putc( i2c_buffer[2] );
                putc( i2c_buffer[3] );
                putc( i2c_buffer[4] );
                putc( i2c_buffer[5] );
                putc( '#' );                            
            } else {                
                //printf( ":[0x%2X=%d]#", i2c_buffer[0], i2c_status[i] );
            }   
        } 
	} else {    
	    printf( ":Unknown device 0x%2X#", i2c_buffer[0] );
	}            
}


/** ************************************************************************** 
 *  binaryWriteMessage
 */
void binaryWriteMessage( char * buffer ) {
	int i;

	i2c_buffer[0] = buffer[1];
	i2c_buffer[1] = buffer[2];
	for( i = 1; (i < NUM_I2C_DEV) && (i2c_device[i] != (0xFE & i2c_buffer[0])); i ++ );
    if( i <  NUM_I2C_DEV ) {
        //printf( ":Address 0x%2X detected#", i2c_device[i] );
        i2c_buffer[2] = buffer[3];
        i2c_buffer[3] = buffer[4];
        i2c_buffer[4] = buffer[5];
        i2c_buffer[5] = buffer[6];
        i2c_status[i] = write_i2c_data( i2c_buffer );
        if( i2c_status[i] == 0 ) {
            //printf( ":Write 0x%2X[%d]=%ld#", i2c_buffer[0], i2c_buffer[1], * (signed int32 *) & i2c_buffer[2]  );
            putc( ':' );
            putc( i2c_buffer[0] );
            putc( i2c_status[i] );
            putc( '#' );                            
        } else {                        
            //printf( ":[0x%2X=%d]#", i2c_buffer[0], i2c_status[i] );
        }    
	} else {    
	    printf( ":Unknown device 0x%2X#", i2c_buffer[0] );
	}            
}


/** ************************************************************************** 
 *  Main
 */
void main( void )  {
    int i, k;
    char    c;
	
    int tc, sec;
    char string[TTY_BUF_LEN];
    
    //------------------------------------------------------------------------
    //      setup uCU
    //set_tris_a( 0xFF );                         // PORTA all inputs
    //set_tris_b( 0xFF );                         // PORTB all inputs
    //set_tris_c( 0x00 );                         // PORTC all outputs
    //set_tris_d( 0x00 );                         // PORTD all outputs
    //set_tris_e( 0x00 );                         // PORTE all outputs
    set_tris_a( 0x00);                          // PORTA all outputs
    set_tris_b( 0xFF );                         // PORTB all inputs
    set_tris_c( 0xC0 );                         // PORTC all in/out-puts
    set_tris_d( 0x00 );                         // PORTD all outputs
    set_tris_e( 0x00 );                         // PORTE all outputs

    //------------------------------------------------------------------------
    //      Test uCU
#ifdef __TEST_UCU__
    for( k = 0; k < 8; k ++ ) {
        output_high( rLED );
        delay_ms(250);
        output_low( rLED );
        delay_ms(750);
    }
#endif
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
	myTmr1_rv  = 500;			                // => 2[Hz]
    myTmr1_2Hz_flag = FALSE;
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
    
    //-------------------------------------------------------------------------
    //RS232 Test
    //printf( "ESO50cm Firmware\n" );
    //printf( "Firmware version %5.2f\n", version/100.0  );
    //printf( "Compiled on %s %s \r\n", __DATE__, __TIME__ );
    //printf( "with compiler version %s\r\n", compiler );
    //printf( "File %s\r\n",__FILE__ );

    //-------------------------------------------------------------------------
    //LCD Test
    sprintf( string, "FV: %5.2f", version/100.0 );
    lcd_goto_xy( 1, 1 );
    lcd_print( string );
    sprintf( string, "FD: %s", __DATE__ );
    lcd_goto_xy( 1, 2 );
    lcd_print( string );
    sprintf( string, "FT: %s", __TIME__ );
    lcd_goto_xy( 1, 3 );
    lcd_print( string );


    //------------------------------------------------------------------------
    //      Init VArs Main Loop
    LED_OFF(rLED);
	i = 0;
	k = 0;
    tc = 0;

    // init i2c vars
    i2c_address= 0xA0;
    for( i = 0; i < NUM_I2C_DEV; i ++ ){
        i2c_device[i] = i2c_address;
        i2c_status[i] = 1;
        for( k = 0; k < 6; k ++ ) {
            data[i][k] = i * 2000 + k;
        }    
        i2c_address += 2;
    }
    
    // dummy telescope
    init_dummy_telescope();
    
    //scan ecoders flags
    scan_alpha_worm_encoder_flag = 4;
    scan_alpha_axis_encoder_flag = 4;
    scan_delta_worm_encoder_flag = 4;
    scan_delta_axis_encoder_flag = 4;
    
    // lcd info
    
    for( i = 0; i < 5; i ++ ) {
        output_high( rLED );
        delay_ms( 250 );
        output_low( rLED );
        delay_ms( 750 );
    }    
    sprintf( string, "AW %8ld", data[alphaW][4] );
    lcd_goto_xy( 1, 1 );
    lcd_print( string );
    sprintf( string, "AA %8ld", data[alphaA][4] );
    lcd_goto_xy( 1, 2 );
    lcd_print( string );
    sprintf( string, "DW %8ld", data[deltaW][4] );
    lcd_goto_xy( 1, 3 );
    lcd_print( string );
    sprintf( string, "DA %8ld", data[deltaA][4] );
    lcd_goto_xy( 1, 4 );
    lcd_print( string );
    

    // tty vars
    for( k = 0; k < TTY_BUF_LEN; k ++ ) {
		tty_buffer[k] = 0;
		msg_buffer[k] = 0;
	}
    
    tty_start_byte = 0;
    tty_end_byte   = 0;
    tty_seek_byte  = 0;
    
    //------------------------------------------------------------------------
    // Main Loop
    while( TRUE ) {
        if( myTmr0_8Hz_flag ) {
            myTmr0_8Hz_flag = FALSE;
            output_toggle( rLED );
            update_dummy_telescope();
            if( tc % 2 == 0 ) {
                if( scan_alpha_worm_encoder_flag < 4 ) {
                    scan_encoder( 3 );
                    scan_alpha_worm_encoder_flag ++;
                }
                if( scan_alpha_axis_encoder_flag < 4 ) {
                    scan_encoder( 4 );
                    scan_alpha_axis_encoder_flag ++;
                }    
                if( scan_delta_worm_encoder_flag < 4 ) {
                    scan_encoder( 5 );
                    scan_delta_worm_encoder_flag ++;
                }    
                if( scan_delta_axis_encoder_flag < 4 ) {
                    scan_encoder( 6 );
                    scan_delta_axis_encoder_flag ++;
                }
            }   
            if( tc % 8 == 0 ) {
                //output_toggle( rLED );
                sprintf( string, "AW %8ld", data[alphaW][4] );
                lcd_goto_xy( 1, 1 );
                lcd_print( string );
                sprintf( string, "AA %8ld", data[alphaA][4] );
                lcd_goto_xy( 1, 2 );
                lcd_print( string );
                                
                sprintf( string, "DW %8ld", data[deltaW][4] );
                lcd_goto_xy( 1, 3 );
                lcd_print( string );
                sprintf( string, "DA %8ld  %3d", data[deltaA][4], tc/8 );
                lcd_goto_xy( 1, 4 );
                lcd_print( string );
                sec ++;
            }    
            tc ++;
        }

        //if( kbhit() ){
        //    c = getc();
        //    tty_buffer[tty_seek_byte] = c;
        //    tty_seek_byte ++;
        //    tty_seek_byte %= TTY_BUF_LEN;
        //}
        
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
                    } else if( bit_test( msg_buffer[1], 0 ) && msg_len == 4 ) {   
                        msg_buffer[msg_len]= 0;
                        binaryReadMessage( msg_buffer );
                        msg_len = 0;
                    } else if( ! bit_test( msg_buffer[1], 0 ) && msg_len == 8 ) {
                        msg_buffer[msg_len]= 0;
                        binaryWriteMessage( msg_buffer );
                        msg_len = 0;
                    }
                }
            }
            tty_start_byte  = i;
            tty_start_byte %= TTY_BUF_LEN;
        } 
    } 
}//END PROGRAM
