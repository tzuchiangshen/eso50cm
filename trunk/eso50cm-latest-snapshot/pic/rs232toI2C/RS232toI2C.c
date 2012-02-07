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
#fuses HSPLL,NOWDT,NOPROTECT,NOLVP,NODEBUG,USBDIV,PLL5,CPUDIV1,VREGEN,NOPUT
#use delay(clock=48000000)
#use i2c(master, SDA=PIN_B0, SCL=PIN_B1, force_hw)
//#use rs232(baud=38400, xmit=PIN_C6, rcv=PIN_C7)
#use rs232(baud=57600, xmit=PIN_C6, rcv=PIN_C7)

#include <string.h>
#include <stdlib.h>
#define version 0x0006
#pragma id version
#define compiler __PCH__
//#define __TEST_UCU__

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



#define gLED PIN_E0
#define yLED PIN_E1
#define rLED PIN_E2

#define LED_ON  output_high
#define LED_OFF output_low

//----------------------------------------------------------------------------
// tty vars
#define  TTY_BUF_LEN        32
char    tty_buffer[TTY_BUF_LEN];
char    msg_buffer[TTY_BUF_LEN];
int     msg_len;
int     msg_bin_start;
int     tty_start_byte;
int     tty_end_byte;
int     tty_seek_byte;

//----------------------------------------------------------------------------
// tmr0 vars
int16 myTmr0_cnt;
int16 myTmr0_rv;
int16 myTmr0;
int	  myTmr0_flag;
int	  myTmr0_4Hz_flag;

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
#define I2C_BUF_LEN     8

int i2c_address;
//int i2c_ack;
int i2c_memory;
int i2c_device[NUM_I2C_DEV];
int i2c_status[NUM_I2C_DEV];
char i2c_buffer[I2C_BUF_LEN];

int scan_alpha_worm_encoder_flag;
int scan_alpha_axis_encoder_flag;
int scan_delta_worm_encoder_flag;
int scan_delta_axis_encoder_flag;

int scan_all_encoders;

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
		myTmr0_4Hz_flag = TRUE;
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
    i2c_start();
    ack= i2c_write( dev );
    i2c_stop();
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
                
    i2c_start();
    ack  = i2c_write( buffer[0] );
    ack += i2c_write( buffer[1] );
    for( i = 2; i < 6; i ++ ) {
        ack += i2c_write( buffer[i] );
    }
    i2c_stop();
    return ack;
} 

/** **************************************************************************
 * read_12c_data
 * @param buffer (char *).
 * @return ack (int): 0 operation was succesfull, > 0 device not responding. 
 * Probably the function hangs if the device becomes off line during the
 * reading operation.
 */
int read_i2c_data( char * m_buffer ) {
    int ack;
    int i;
    //char c;
    
    i2c_start();
    ack  = i2c_write( 0xFE & m_buffer[0] );
    if( ack == 0 ) {
        ack += i2c_write( m_buffer[1] );
        if( ack == 0 ) {
            i2c_start();
            i2c_write( m_buffer[0] );
            for( i = 2; i < 6; i ++ ) {
                bit_set( PIC_SSPCON2, 3 );  //Este bit hablita Master para leer.
                m_buffer[i] = i2c_read( 1 );
            }
            bit_set( PIC_SSPCON2, 3 );      //Este bit habilita Master para leer.
            m_buffer[6] = i2c_read( 0 );                  //OJO CON ESTE i2c_read no hace ACK para terminar
        }
    }
    //if( m_buffer[5] != 0 ) {
    //    printf( ":Read ERROR 0x%2X[%d]=%2X %2X %2X %2X (%2X)#", 
    //    m_buffer[0], m_buffer[1], m_buffer[2], m_buffer[3], m_buffer[4], m_buffer[5], m_buffer[6] );
    //}
    i2c_stop();
    return ack;
} 


/** ************************************************************************** 
 *  scan_encoder
 */
int scan_encoder( int i ) {
    //int retval;
    char scan_buffer[8];
    char chksum;
    scan_buffer[0] = i2c_device[i]+1;
    scan_buffer[1] = 4;
    i2c_status[i] = read_i2c_data( scan_buffer );
    if( i2c_status[i] == 0 ){
        putc( ':' );
        putc( scan_buffer[0] );
        chksum  = scan_buffer[0];
        putc( scan_buffer[1] );
        chksum += scan_buffer[1];
        putc( scan_buffer[2] );
        chksum += scan_buffer[2];
        putc( scan_buffer[3] );
        chksum += scan_buffer[3];
        putc( scan_buffer[4] );
        chksum += scan_buffer[4];
        putc( scan_buffer[5] );
        chksum += scan_buffer[5];
        putc( chksum );
        putc( '#' );        
        putc( '.' );
        putc( '.' );
        putc( '.' );
    //} else {
    //    putc( ':' );
    //    putc( i2c_device[i] );
    //    putc( i2c_status[i] );
    //    putc( '#' );            
    }
    return i2c_status[i];
}     

/** ************************************************************************** 
 *  scan_encoder_return_text
 */
int scan_encoder_return_text( int i ) {
    //int retval;
    char scan_buffer[8];
    scan_buffer[0] = i2c_device[i]+1;
    scan_buffer[1] = 4;
    i2c_status[i] = read_i2c_data( scan_buffer );
    if( i2c_status[i] == 0 ){
        printf( ":Read 0x%2X[%d]=%2X %2X %2X %2X (%2X)#", 
        scan_buffer[0], scan_buffer[1], scan_buffer[2], scan_buffer[3], scan_buffer[4], scan_buffer[5], scan_buffer[6] );
    } else {
        printf( ":[0x%X=%d]#", i2c_device[i], i2c_status[i] );
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
                putc( ':' );
                //putc( '\n' ); 
                for( i = 1; i < NUM_I2C_DEV; i ++ ) {
                    i2c_status[i] = test_i2c_device( i2c_device[i] );
                    printf( "[0x%X=%d]", i2c_device[i], i2c_status[i] );
                }
                //putc( '\n' ); 
                putc( '#' );
            } 
            break;
        case 'V':
            putc( ':' );
            //putc( '\n' );  
            switch( buffer[2] ) {
                case 'N':
                    printf( "VN%05.2f", version/100.0 );    
                    break;
                case 'T':  
                    printf( "VT%s", __TIME__ );  
                    break;
                case 'D':
                    printf( "VD%s", __DATE__ );    
                    break;
                default:
                    buffer[length-1]= 0;
                    printf( "Unknown command %s", & buffer[2] );  
            }
            //putc( '\n' ); 
            putc( '#' ); 
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
            putc( ':' ); 
            //putc( '\n' ); 
            if( 0 < i && i < NUM_I2C_DEV ) {
                i2c_memory = buffer[3] - '0';
                if( i2c_memory < 10 ) {
                    buffer[length-1]= 0;
                    numero = atoi32( & buffer[4] );
                    //printf( "[%d]=%ld", i2c_memory, numero ); 
                    i2c_buffer[0]= i2c_device[i];
                    i2c_buffer[1]= i2c_memory;
                    * (signed int32 *) & i2c_buffer[2] = numero;
                    //printf( "\nWriting 0x%X %d %ld...", i2c_buffer[0], i2c_buffer[1], * (signed int32 *) & i2c_buffer[2] );
                    i2c_status[i] = write_i2c_data( i2c_buffer );
                    printf( "[0x%2X=%d]", i2c_device[i], i2c_status[i] );
                } else {     
                    buffer[length-1]= 0;
                    printf( "Write 0x%X, wrong mem %c!", i2c_device[i], buffer[3] );
                }
            } else {
                printf( "Write 0xA%c, wrong device!", buffer[2]  ); 
            }    
            //putc( '\n' ); 
            putc( '#' ); 
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
            putc( ':' ); 
            //putc( '\n' ); 
            if( 0 < i && i < NUM_I2C_DEV ) {
                i2c_memory = buffer[3] - '0';
                if( i2c_memory < 10 ) {
                    //printf( "[%d]", i2c_memory ); 
                    i2c_buffer[0]= i2c_device[i] + 1;
                    i2c_buffer[1]= i2c_memory;
                    //printf( "\nReading 0x%X %d...", i2c_buffer[0], i2c_buffer[1] );
                    i2c_status[i] = read_i2c_data( i2c_buffer );
                    if( i2c_status[i] == 0 ) {
                        i2c_buffer[6]= 0;
                        numero = * (signed  int32 *) & i2c_buffer[2];
                        printf( "0x%2X[%d]=%ld", i2c_device[i], i2c_memory, numero );
                    } else {                
                        printf( "[0x%2X=%d]", i2c_device[i], i2c_status[i] );
                    }    
                } else {    
                    buffer[length-1]= 0; 
                    printf( "Read 0x%X, wrong mem %c!", i2c_device[i], buffer[3] );
                }
            } else {
                printf( "Read 0xA%c, wrong device!", buffer[2]  );     
            } 
            //putc( '\n' ); 
            putc( '#' );   
            break;
        case 'S':
            putc( ':' );
            //putc( '\n' ); 
            printf( "S: Nothing to do!"  );   
            //putc( '\n' ); 
            putc( '#' );
            break; 
        case 'P':
            putc( ':' );
            //putc( '\n' ); 
            printf( "P: Scanig all encoders!"  );  
            scan_all_encoders = true; 
            //putc( '\n' ); 
            putc( '#' );            
            break;      
        case ' ':   
            putc( ':' );
            //putc( '\n' ); 
            buffer[length-1]= 0;
            printf( "%s", & buffer[2] );   
            //putc( '\n' ); 
            putc( '#' );
            break;   
        default:
            putc( ':' );
            buffer[length-1]= 0;
            printf( "Unknown command %c", buffer[1] );   
            putc( '#' );
    }
}

/** ************************************************************************** 
 *  binaryReadMessage
 */
void binaryReadMessage( char * buffer ) {          
}


/** ************************************************************************** 
 *  binaryWriteMessage
 */
void binaryWriteMessage( char * buffer ) {       
}

/** ************************************************************************** 
 *  binaryTestMessage
 */
void binaryTestMessage( char * buffer ) {  
}

/** ************************************************************************** 
 *  binaryMessage
 */
void binaryMessage( char * buffer ) {
	int i;
    int j;
    int k;
    int quit;
    char chksum ;

    quit = 0;
    j = 1;
    //putc( ':' );
    do{
	    for( i = 0; (i < NUM_I2C_DEV) && (i2c_device[i] != (0xFE & buffer[j])); i ++ );
	    if( i == 0 ) {
    	    if( bit_test( buffer[j], 0 ) ) {
        	    if( buffer[j+1] == 0 ) {
            	    scan_all_encoders = true;
            	    for( k = 3; k < NUM_I2C_DEV; k ++ ) {
                	    i2c_buffer[0] = i2c_device[k]+1;
                	    i2c_buffer[1] = 4;
                	    i2c_status[k] = read_i2c_data( i2c_buffer );
                        if( i2c_status[k] == 0 ) {
                            putc( ':' );
                            putc( i2c_buffer[0] );
                            chksum  = i2c_buffer[0];
                            putc( i2c_buffer[1] );
                            chksum += i2c_buffer[1];
                            putc( i2c_buffer[2] );
                            chksum += i2c_buffer[2];
                            putc( i2c_buffer[3] );
                            chksum += i2c_buffer[3];
                            putc( i2c_buffer[4] );
                            chksum += i2c_buffer[4];
                            putc( i2c_buffer[5] );
                            chksum += i2c_buffer[5];
                            putc( chksum );
                            //putc( '#' );    
                            //putc( '.' );    
                            //putc( '.' );    
                            //putc( '.' );    
                        } else {                
                            putc( ':' );
                            putc( 0XFE & i2c_buffer[0] );                      
                            putc( i2c_status[k] );
                            //putc( '#' ); 
                        }   
        	            
                	}
            	}
                if( buffer[j+2] == '#' ) {
                    quit = 1;
                } else {
                    j += 2;
                }
            } else {
                if( buffer[j+6] == '#' ) {
                    quit = 1;
                } else {
                    j += 6;
                }
            }
        } else if( i <  NUM_I2C_DEV ) {
            i2c_buffer[0] = buffer[j];
            i2c_buffer[1] = buffer[j+1];
            
            if( bit_test( buffer[j], 0 ) ) {
                //printf( "R 0x%2X[%d]", (0xFE & buffer[j]), buffer[j+1] );
                i2c_status[i] = read_i2c_data( i2c_buffer );    //Address must be ODD
                if( i2c_status[i] == 0 ) {
                    putc( ':' );
                    putc( i2c_buffer[0] );
                    chksum  = i2c_buffer[0];
                    putc( i2c_buffer[1] );
                    chksum += i2c_buffer[1];
                    putc( i2c_buffer[2] );
                    chksum += i2c_buffer[2];
                    putc( i2c_buffer[3] );
                    chksum += i2c_buffer[3];
                    putc( i2c_buffer[4] );
                    chksum += i2c_buffer[4];
                    putc( i2c_buffer[5] );
                    chksum += i2c_buffer[5];
                    putc( chksum );
                    //putc( '#' );    
                    //putc( '.' );    
                    //putc( '.' );    
                    //putc( '.' );    
                } else {                
                    putc( ':' );
                    putc( 0XFE & i2c_buffer[0] );                      
                    putc( i2c_status[i] );
                    //putc( '#' ); 
                }   
	            
                if( buffer[j+2] == '#' ) {
                    quit = 1;
                } else {
                    j += 2;
                    //putc( ',' );
                    //putc( ' ' );
                }
            } else {
                //printf( "W 0x%2X[%d]=%ld", (0xFE & buffer[j]), buffer[j+1],  * (signed int32 *) & buffer[j+2]  );
                i2c_buffer[2] = buffer[j+2];
                i2c_buffer[3] = buffer[j+3];
                i2c_buffer[4] = buffer[j+4];
                i2c_buffer[5] = buffer[j+5];
                i2c_status[i] = write_i2c_data( i2c_buffer );   //Address must be EVEN
                putc( ':' );
                putc( i2c_buffer[0] );                        
                putc( i2c_status[i] );
                //putc( '#' ); 
                if( buffer[j+6] == '#' ) {
                    quit = 1;
                } else {
                    j += 6;
                    //putc( ',' );
                    //putc( ' ' );
                }
            }
        } 
	} while( ! quit );      
	putc( '#' );
	putc( '#' );
	putc( '#' );
	putc( '#' );       
}

/** ************************************************************************** 
 *  Main
 */
void main( void )  {
    int i, k;
    int sec;
	//char * pointer;
	//signed int32 numero;

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
    for( k = 0; k < 8; k ++ ) {
        PORTE = 0x07 & k;
        delay_ms(250);
        PORTE = 0;
        delay_ms(750);
    }
    //------------------------------------------------------------------------
    //      setup tmr0
    // PS    = RTCC_DIV_X
    // HS    = Fosc/4 = 5[Mhz]........ 1/(Fosc/4) = 0.2[us]
    // HSPLL = Fosc/4 = 2 * 5[Mhz].... 1/(Fosc/4) = 1/12[us]
    //      o                1    
    //          DTmr0 = ( -------- ) * PS * N    ( PS = 4 )
    //                    (Fosc/4)
    //
    //        donde N es el numero de pasos para que el contador de TMR0 haga 
    //        overflow, i.e: ( 0xFFFF - N )  
    //      o Para 100[Hz], con Fosc/4 = 10[Mhz] y PS = 4 
    //                  
    //                    1        (Fosc/4)                12 * 10^6
    //          N =   --------- * ---------- = 10^(-2) * -------------- 
    //                 100[Hz]        4                        4
    //         
    //          N = = 3 x 10^4 = 30000
    //
    setup_timer_0(RTCC_INTERNAL|RTCC_DIV_4);
	myTmr0     = 35535;		                    // => 100[Hz]  
	myTmr0_rv  = 25;			                // => 4[Hz]
    myTmr0_4Hz_flag = FALSE;
    myTmr0_flag = FALSE;
    
	myTmr0_cnt = myTmr0_rv;
    set_timer0( myTmr0 );
    enable_interrupts( INT_TIMER0 );
    

    //------------------------------------------------------------------------
    //      setup tmr1
    setup_timer_1(T1_INTERNAL|T1_DIV_BY_4);
	myTmr1     = 62535;		                    // => 1000[Hz]  
	myTmr1_rv  = 500;			                // => 2[Hz]
    myTmr1_2Hz_flag = FALSE;
    myTmr1_flag = FALSE;

    myTmr1_cnt = myTmr1_rv;                   
    set_timer1( myTmr1 );   
    enable_interrupts( INT_TIMER1 );

    //------------------------------------------------------------------------
    //  Enable RS232 interruption
    enable_interrupts( int_rda );

    //------------------------------------------------------------------------
    //  Inicializacion Global Interruptions
    enable_interrupts( GLOBAL );

#ifdef __TEST_UCU__
    //------------------------------------------------------------------------
    // Test Tmr0
    LED_OFF(gLED);
    LED_OFF(yLED);
    LED_OFF(rLED);
    k = 10;
    while( k > 0 ) {
        if( myTmr0_4Hz_flag ) {
            myTmr0_4Hz_flag = FALSE;
            output_toggle(gLED);
            output_toggle(yLED);
            k --;
        }    
    }    
    delay_ms( 1000 );
    //------------------------------------------------------------------------
    // Test Tmr1
    LED_OFF(gLED);
    LED_OFF(yLED);
    LED_OFF(rLED);
    k = 10;
    while( k > 0 ) {
        if( myTmr1_2Hz_flag ) {
            myTmr1_2Hz_flag = FALSE;
            output_toggle(rLED);
            output_toggle(yLED);
            k --;
        }    
    }    
    delay_ms( 1000 );
#endif    
    //------------------------------------------------------------------------
    // init i2c vars
    i2c_address= 0xA0;
    for( i = 0; i < NUM_I2C_DEV; i ++ ){
        i2c_device[i] = i2c_address;
        i2c_status[i] = 1;
        i2c_address += 2;
    }

    //------------------------------------------------------------------------
    //      Init VArs Main Loop
    LED_OFF(gLED);
    LED_OFF(yLED);
    LED_OFF(rLED);
    for( k = 0; k < TTY_BUF_LEN; k ++ ) {
		tty_buffer[k] = 0;
		i2c_status[i] = 1;
	}

    scan_alpha_worm_encoder_flag = 4;
    scan_alpha_axis_encoder_flag = 4;
    scan_delta_worm_encoder_flag = 4;
    scan_delta_axis_encoder_flag = 4;
    
    i = 0;
	k = 0;
	sec = 0;
    // tty vars
    for( k = 0; k < TTY_BUF_LEN; k ++ ) {
		tty_buffer[k] = 0;
		msg_buffer[k] = 0;
	}
    
    tty_start_byte = 0;
    tty_end_byte   = 0;
    tty_seek_byte  = 0;
      
    /*
    putc( ':' );  
    printf( "ESO50cm" ); 
    printf( "VN%05.2f", version/100.0 );    
    printf( "VT%s", __TIME__ );  
    printf( "VD%s", __DATE__ );   
    putc( '#' );
    */
    putc( ':' );  
    putc( 0xA0 );
    putc( 0 );
    putc( '#' );
    scan_all_encoders = false;    
    //------------------------------------------------------------------------
    // Main Loop
    while( TRUE ) {
        if( myTmr0_4Hz_flag ) {
            myTmr0_4Hz_flag = FALSE;
            output_toggle( yLED );
            if( sec % 2 == 0 ) {
  
                if( scan_all_encoders ) {
                    scan_all_encoders = false;
                    putc( ':' );
                    putc( 0xA0 );
                    putc( 0 );
                    putc( '#' );
                    //putc( '.' );
                    //putc( '.' );
                    //putc( '.' );
                }
                
            }
            sec ++;
            
            /*
            if( scan_all_encoders ) {
                for( i = 3; i< 7; i ++ ) {
                    scan_encoder_return_text( i );
                }
            } else {
                if( scan_alpha_worm_encoder_flag < 1 ) {
                    scan_encoder( 3 );
                    scan_alpha_worm_encoder_flag ++;
                }
                if( scan_alpha_axis_encoder_flag < 1 ) {
                    scan_encoder( 4 );
                    scan_alpha_axis_encoder_flag ++;
                }    
                if( scan_delta_worm_encoder_flag < 1 ) {
                    scan_encoder( 5 );
                    scan_delta_worm_encoder_flag ++;
                }    
                if( scan_delta_axis_encoder_flag < 1 ) {
                    scan_encoder( 6 );
                    scan_delta_axis_encoder_flag ++;
                }
            }
            */
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
                    if( msg_buffer[1] < 0xA0 ) {
                        if( tty_buffer[i] == '#' ) {
                            msg_buffer[msg_len]= 0;
            				textMessage( msg_buffer, msg_len );
                            msg_len = 0;
                        }  
                    } else if( msg_len == msg_bin_start + 3 ) {   
                        if( bit_test( msg_buffer[msg_bin_start], 0 ) ) {
                            if( msg_buffer[msg_bin_start+2] == '#' ) {
                                binaryMessage( msg_buffer );
                                msg_len = 0;
                            } else {
                                msg_bin_start += 2;
                            }
                        }
                    } else if( msg_len == msg_bin_start + 7 ) {
                        if( ! bit_test( msg_buffer[msg_bin_start], 0 ) ) {
                            if( msg_buffer[msg_bin_start+6] == '#' ) {
                                binaryMessage( msg_buffer );
                                msg_len = 0;
                            } else {
                                msg_bin_start += 6;
                            }
                        } 
                    }
                    
                    /*  
                    } else if( bit_test( msg_buffer[1], 0 ) && msg_len == 4 ) {   
                        msg_buffer[msg_len]= 0;
                        if( msg_buffer[2] == 255 ) {
                            binaryTestMessage( msg_buffer );
                        } else {
                            binaryReadMessage( msg_buffer );
                        }
                        msg_len = 0;
                    } else if( ! bit_test( msg_buffer[1], 0 ) && msg_len == 8 ) {
                        msg_buffer[msg_len]= 0;
                        binaryWriteMessage( msg_buffer );
                        msg_len = 0;
                    }
                    */
                }
            } //END for( i = tty_start_byte; i < tty_end_byte; i ++ )
            tty_start_byte  = i;
            tty_start_byte %= TTY_BUF_LEN;
        } // END while( tty_start_byte != tty_seek_byte )
        
    } // END while( TRUE )

}//END PROGRAM
