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
#define version 61
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

#define alphaM  1
#define deltaM  2
#define alphaW  3
#define alphaA  4
#define deltaW  5
#define deltaA  6

//----------------------------------------------------------------------------
// tty vars
#define  TTY_BUF_LEN        32
char    tty_buffer[TTY_BUF_LEN];
char    msg_buffer[TTY_BUF_LEN];
char    ans_buffer[TTY_BUF_LEN];
int     msg_len;
int     ans_len;
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
int	  myTmr0_10Hz_flag;

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
int i2c_memory;
int i2c_device[NUM_I2C_DEV];
int i2c_status[NUM_I2C_DEV];
char i2c_buffer[I2C_BUF_LEN];

//----------------------------------------------------------------------------
// Dummy I2C vars
signed int32 data[NUM_I2C_DEV][8];


//int scan_alpha_worm_encoder_flag;
//int scan_alpha_axis_encoder_flag;
//int scan_delta_worm_encoder_flag;
//int scan_delta_axis_encoder_flag;

int scan_all_encoders;

/** ************************************************************************** 
 *  Prototypes
 */
int test_i2c_device( int dev );
int write_i2c_data( char * buffer );
int read_i2c_data( char * buffer );

void localMessage( char * buffer );
void i2cMessage( char * buffer );
void wrongMessage( char * buffer ); 

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
		myTmr0_10Hz_flag = TRUE;
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
    /*
    i2c_start();
    ack= i2c_write( dev );
    i2c_stop();
    */
    return ack;
}

/** **************************************************************************
 * write_12c_data
 * @param buffer (char *).
 * @return ack (int): 0 operation was succesfull, > 0 device not responding. 
 */
int write_i2c_data( char * m_buffer ) {
    int ack;
    int i;
    int m;
 
    m = m_buffer[1];
    for( i = 1; (i < NUM_I2C_DEV) && (i2c_device[i] != (0xFE & m_buffer[0])); i ++ );
    data[i][m] = *(signed int32 *) & m_buffer[2];
    ack = 0;
    
    /*   
    i2c_start();
    ack  = i2c_write( buffer[0] );
    ack += i2c_write( buffer[1] );
    for( i = 2; i < 6; i ++ ) {
        ack += i2c_write( buffer[i] );
    }
    i2c_stop();
    */
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
    int m;
    
    m = m_buffer[1];
    for( i = 1; (i < NUM_I2C_DEV) && (i2c_device[i] != (0xFE & m_buffer[0])); i ++ );
    *(signed int32 *) & m_buffer[2] = data[i][m];
    ack = 0;


    /*    
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
    i2c_stop();
    */
    return ack;
} 


/** ************************************************************************** 
 *  init_dummy_telescope
 */
void init_dummy_telescope( void ) {
    int i;
    int j;
    
    for( i = 0; i < NUM_I2C_DEV; i ++ ) {
        for( j = 0; j < 8; j ++ ) {
            data[i][j] = 0;
        }
    }
    //Motors 
    //Alpha Motor
    //Q.Encoder
    data[alphaM][4] = 0;//0xA2A2A2A2;
    //Q.Encoder Rortation Rate Reference
    data[alphaM][1] = 300;//0xA4A4A4A4;           //300[tics/second] = Sidereal Rate
    //Delta Motor
    //Q.Encoder
    data[deltaM][4] = 0;//0xA4A4A4A4;
    //Q.Encoder Rortation Rate Reference
    data[deltaM][1] = 0;           //240[tics/second] = Sidereal Rate
    //Encoders Home position
    data[alphaW][4] = 0;//0xA6A6A6A6;
    data[alphaA][4] = 512;//0xA8A8A8A8;
    data[deltaW][4] = 0;//0xABABABAB;
    data[deltaA][4] = 512;//0xADADADAD;
}    


/** ************************************************************************** 
 *  update_dummy_telescope
 *  o Motor's Axis Rotation Frequency:
 *    o data[alphaM][1] = f_alpha[tics/seconds]
 *    o data[deltaM][1] = f_delta[tics/seconds]
 *  
 *  o Quadrature encoder position counter
 *    o data[alphaM][4] 
 *    o data[deltaM][4]
 *
 *  o Worm's absolute encoder position counter
 *    o data[alphaW][4] 
 *    o data[deltaW][4]
 *
 *  o Axis's absolute encoder position counter
 *    o data[alphaA][4] 
 *    o data[deltaA][4]
 *
 *  
 */
void update_dummy_telescope( void ) {
    float tmp;

    //Alpha Q. Encoder
    tmp =  (float) data[alphaM][6];
    tmp /= 10.0;                   //Tmr0_10Hz_Flag
    data[alphaM][4] += tmp;
    data[alphaM][3] = data[alphaM][4] / 3000;    
    
    //Alpha Worm
    tmp = (float) data[alphaM][4];
    tmp /= 3000.;
    tmp /= 48.;
    tmp *= 1024.0;
    data[alphaW][4] = tmp;
    
    //Alpha Axis
    tmp = (float) data[alphaW][4];
    tmp /= 240.0;                 //alpha axiss to alpha axis encoder ratio 
    data[alphaA][4] =  tmp;
    data[alphaA][4] += 512;     //initial position
    
    //Delta Q. Encoder
    tmp =  (float) data[deltaM][6];
    tmp /= 10.0;                   //Tmr0_10Hz_Flag
    data[deltaM][4] += tmp;
    data[deltaM][3] = data[deltaM][4] / 3000;    
    
    //delta Worm
    tmp = (float) data[deltaM][4];
    tmp /= 3000.;
    tmp /= 48.;
    tmp *= 1024.;
    data[deltaW][4] = tmp;
    
    //Delta Axis
    tmp = (float) data[deltaW][4];
    tmp /= 288.;                 //delta axiss to alpha delta encoder ratio 
    data[deltaA][4] = tmp;
    data[deltaA][4] += 512;     //initial position
}    



/** ************************************************************************** 
 *  localMessage
 */
void localMessage( char * buffer ) 
{
	int i;
	char chksum;

    chksum = 0;
    if( bit_test( buffer[1], 0 ) ) {
        /** Read data from this dev. */
        switch( buffer[2] ) {
            case 't':
                break;
            case 'N':
                sprintf( ans_buffer, "%05.2f", version/100.0 );
                ans_len = strlen( ans_buffer ); 
                break;
            case 'T':  
                sprintf( ans_buffer, "%s", __TIME__ );  
                ans_len = strlen( ans_buffer );
                break;
            case 'D':
                sprintf( ans_buffer, "%s", __DATE__ );    
                ans_len = strlen( ans_buffer );
                break;
            case 'W': 
                
                break;       
            case 'R':
                
                break;
            case 'S':

                break; 
            case 'P':
                ans_len = 0;
                for( i = 3; i < NUM_I2C_DEV; i ++ ) {
                    i2c_buffer[0] = i2c_device[i]+1;
                	i2c_buffer[1] = 4;
                    i2c_status[i] = read_i2c_data( i2c_buffer );
                    if( i2c_status[i] == 0 ) {
                        ans_buffer[ans_len] = i2c_buffer[0];            // mes_len = 1
                        chksum += i2c_buffer[0];
                        ans_len ++;
                        
                        ans_buffer[ans_len] = i2c_buffer[1];            // mes_len = 2
                        chksum += i2c_buffer[1];
                        ans_len ++;
                        
                        ans_buffer[ans_len] = i2c_buffer[2];            // mes_len = 3
                        chksum += i2c_buffer[2];
                        ans_len ++;
                        
                        ans_buffer[ans_len] = i2c_buffer[3];            // mes_len = 4
                        chksum += i2c_buffer[3];
                        ans_len ++;
                        
                        ans_buffer[ans_len] = i2c_buffer[4];            // mes_len = 5
                        chksum += i2c_buffer[4];
                        ans_len ++;
                        
                        ans_buffer[ans_len] = i2c_buffer[5];            // mes_len = 6
                        chksum += i2c_buffer[5];
                        ans_len ++;
                   } else {                
                        ans_buffer[ans_len] = 0XFE & i2c_buffer[0];     // mes_len = 1                     
                        chksum += ans_buffer[ans_len];
                        ans_len ++;
                        
                        ans_buffer[ans_len] = i2c_status[i];            // mes_len = 2
                        chksum += ans_buffer[ans_len];
                        ans_len ++;
                        
                        ans_buffer[ans_len] = 0;                        // mes_len = 3
                        ans_len ++;
                        
                        ans_buffer[ans_len] = 0;                        // mes_len = 4
                        ans_len ++;
                        
                        ans_buffer[ans_len] = 0;                        // mes_len = 5
                        ans_len ++;
                        
                        ans_buffer[ans_len] = 0;                        // mes_len = 6
                        ans_len ++;

                   }
                }
                scan_all_encoders = true;
                break;      
            case ' ':   

                break;   
            default:
                sprintf( ans_buffer, "Unknown Command!" );           
        }
        putc( ':' );
        putc( buffer[1] );
        putc( buffer[2] );
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

/** ************************************************************************** 
 *  i2cMessage
 */
void i2cMessage( char * buffer ) {
	int i;
    char chksum ;

    for( i = 0; (i < NUM_I2C_DEV) && (i2c_device[i] != (0xFE & buffer[1])); i ++ );
    if( 0 < i && i <  NUM_I2C_DEV ) {
        putc( ':' );                              // mes_len = 1
        i2c_buffer[0] = buffer[1];
        i2c_buffer[1] = buffer[2];
           
        if( bit_test( buffer[1], 0 ) ) {
            i2c_status[i] = read_i2c_data( i2c_buffer );
            if( i2c_status[i] == 0 ) {
                putc( i2c_buffer[0] );            // mes_len = 2
                chksum  = i2c_buffer[0];
                putc( i2c_buffer[1] );            // mes_len = 3
                chksum += i2c_buffer[1];
                putc( i2c_buffer[2] );            // mes_len = 4
                chksum += i2c_buffer[2];
                putc( i2c_buffer[3] );            // mes_len = 5
                chksum += i2c_buffer[3];
                putc( i2c_buffer[4] );            // mes_len = 6
                chksum += i2c_buffer[4];
                putc( i2c_buffer[5] );            // mes_len = 7
                chksum += i2c_buffer[5];
                putc( chksum );                   // mes_len = 8
                putc( 0 );                        // mes_len = 9
           } else {                
                putc( 0XFE & i2c_buffer[0] );     // mes_len = 2                     
                putc( i2c_status[i] );            // mes_len = 3
                putc( 0 );                        // mes_len = 4
                putc( 0 );                        // mes_len = 5
                putc( 0 );                        // mes_len = 6
                putc( 0 );                        // mes_len = 7
                putc( 0 );                        // mes_len = 8
                putc( 0 );                        // mes_len = 9
           }   
        } else {
                //printf( "W 0x%2X[%d]=%ld", (0xFE & buffer[j]), buffer[j+1],  * (signed int32 *) & buffer[j+2]  );
                i2c_buffer[2] = buffer[3];
                i2c_buffer[3] = buffer[4];
                i2c_buffer[4] = buffer[5];
                i2c_buffer[5] = buffer[6];
                i2c_status[i] = write_i2c_data( i2c_buffer );
                putc( i2c_buffer[0] );            // mes_len = 2        
                putc( i2c_status[i] );            // mes_len = 3
                putc( 0 );                        // mes_len = 4
                putc( 0 );                        // mes_len = 5
                putc( 0 );                        // mes_len = 6
                putc( 0 );                        // mes_len = 7
                putc( 0 );                        // mes_len = 8
                putc( 0 );                        // mes_len = 9
        }
        putc( '#' );                              // mes_len = 10
	} 
	     
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
        /*
        PORTE = 0x07 & k;
        delay_ms(250);
        PORTE = 0;
        delay_ms(750);
        */
        output_high( PIN_A0 );
        delay_ms( 100 );
        output_low( PIN_A0 );
        delay_ms( 900 );
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
	myTmr0_rv  = 10;			                // => 10[Hz]
    myTmr0_10Hz_flag = FALSE;
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

    //scan_alpha_worm_encoder_flag = 4;
    //scan_alpha_axis_encoder_flag = 4;
    //scan_delta_worm_encoder_flag = 4;
    //scan_delta_axis_encoder_flag = 4;
    
    i = 0;
	k = 0;
	sec = 0;
    // tty vars
    for( k = 0; k < TTY_BUF_LEN; k ++ ) {
		tty_buffer[k] = 0;
		msg_buffer[k] = 0;
		ans_buffer[k] = 0;
	}
    
    tty_start_byte = 0;
    tty_end_byte   = 0;
    tty_seek_byte  = 0;
      
    init_dummy_telescope();

    putc( ':' );      //Start byte 
    putc( 0xA0 );     //I2C address
    putc( 'P' );      //Memory address
    putc( 0 );        //Byte 0
    putc( 0 );        //Byte 1
    putc( 0 );        //Byte 2
    putc( 0 );        //Byte 3
    putc( 0 );        //Chksum
    putc( 0 );        //?
    putc( '#' );      //End byte
    
    scan_all_encoders = false;    
    //------------------------------------------------------------------------
    // Main Loop
    sec = 10;
    while( TRUE ) {
        if( myTmr0_10Hz_flag ) {
            myTmr0_10Hz_flag = FALSE;
            //output_toggle( yLED );
            update_dummy_telescope();
            if( sec == 0 ) {
                sec = 10;
                output_high( PIN_A0 );
                if( scan_all_encoders ) {
                    scan_all_encoders = false;
                    putc( ':' );      //Start byte 
                    putc( 0xA0 );     //I2C address
                    putc( 'P' );      //Memory address
                    putc( 0 );        //Byte 0
                    putc( 0 );        //Byte 1
                    putc( 0 );        //Byte 2
                    putc( 0 );        //Byte 3
                    putc( 0 );        //Chksum
                    putc( 0 );        //?
                    putc( '#' );      //End byte
                }
                
            } else {
                output_low( PIN_A0 );
                sec --;
            }
        }
        /**
         *      in message                  mes_len
         *      -----------------------------------
         *      ':'                         1
         *      [0xAX]                      2
         *      [madd]                      3
         *      [num0][num1][num2][num3]    7
         *      [chks]                      8
         *      [0x00]                      9
         *      '#'                         10
         */
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
                    if( msg_len == 10 ) {
                        if( tty_buffer[i] == '#' ) {
                            msg_buffer[msg_len]= 0;
                            if( (0xFE & msg_buffer[1]) == 0xA0 ) {
            				    localMessage( msg_buffer );
            				} else if( msg_buffer[1] > 0xA1 ) {
                				i2cMessage( msg_buffer );
                		    } else {
                    		    wrongMessage( msg_buffer );
                    		}    
                        }
                        msg_len = 0;  
                    }
                }
            } //END for( i = tty_start_byte; i < tty_end_byte; i ++ )
            tty_start_byte  = i;
            tty_start_byte %= TTY_BUF_LEN;
        } // END while( tty_start_byte != tty_seek_byte )
        
    } // END while( TRUE )

}//END PROGRAM
