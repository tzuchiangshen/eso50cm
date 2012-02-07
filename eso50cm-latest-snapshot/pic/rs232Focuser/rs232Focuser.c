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
 *     PIN D2 -> Dir
 *     PIN D3 -> Step
 *
 * o Stepper Motor: coils' color code
 *   RED--------> A1
 *   GREEN------> A2 
 *   YUL--------> B1
 *   BLUE-------> B2
 */
#include <18F4550.h>
#device adc=10
#fuses HSPLL,NOWDT,NOPROTECT,NOLVP,NODEBUG,USBDIV,PLL5,VREGEN,NOPUT
#use delay(clock=48000000)
//#use i2c(master, SDA=PIN_B0, SCL=PIN_B1, force_hw)
#use rs232(baud=57600, xmit=PIN_C6, rcv=PIN_C7)

#include <string.h>
#include <stdlib.h>
//#include "LCD_4x16.h"
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
int	myTmr1_100Hz_flag;

//----------------------------------------------------------------------------
// ADC
int16 lecturaAD0;
int16 lecturaAD1;
float Temp0;
float Temp1;

int sec, min, hrs;
//----------------------------------------------------------------------------
//Stepper
signed int32 m_mtr_cnter;
int32   m_mtr_steps;
int     m_mtr_dir;
int     m_mtr_flag;
int     m_mtr_switches;
int     m_mtr_switches_debounce;
int     m_mtr_switches_old;
int     m_mtr_limit;
int32   m_mtr_limit1;
int32   m_mtr_limit2;  

//----------------------------------------------------------------------------
// LCD Display
//char display[4][16];
//char display_old[4][16];


/** ************************************************************************** 
 *  Prototypes
 */
void textMessage( char * buffer, int length );

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
        case 'H':
            printf( ":USO.....#\r\n" );
            printf( ":GVD     = Get version date#\r\n" );  
            printf( ":GVT     = Get version time#\r\n" );
            printf( ":GVN     = Get version number#\r\n" );    
            printf( ":F+<NNN>#= Acerca M2 a M1 NNN pasos#\r\n" );
            printf( ":F-<NNN>#= Aleja  M2 a M1 NNN pasos#\r\n" );  
            printf( ":GT#     = Temperatura#\r\n" );    
            printf( ":GT0#    = Temperatura Termistor 0#\r\n" );    
            printf( ":GT1#    = Temperatura Termistor 1#\r\n" );    
            printf( ":GA#     = Lectura analogica de los temitores#\r\n" );
            printf( ":GA0#    = Lectura analogica temitor 0#\r\n" );
            printf( ":GA1#    = Lectura analogica temitor 1#\r\n" );
            printf( ":GP#     = Posicion actual (respecto inicio)#\r\n" );
            printf( ":GL#     = Posicion actual (respecto inicio) limites#\r\n" );
            printf( ":GL0#    = Posicion actual (respecto inicio) limite 0#\r\n" );
            printf( ":GL1#    = Posicion actual (respecto inicio) limite 1#\r\n" );
            break;
        case 'F':
            switch( buffer[2] ) {
                case '+':
                   m_mtr_flag = true;
                   m_mtr_dir  = 1;
                   if( buffer[3] != '#' ){
                        buffer[length-1]= 0;
                        m_mtr_steps = atoi32( & buffer[3] );
                   } else {
                        m_mtr_steps = 200;       //one rev
                   }
                   break;
                case '0':
                    m_mtr_flag = false;
                    break;
                case '-':
                   m_mtr_flag = true;
                   m_mtr_dir  = 0;
                   if( buffer[3] != '#' ){
                        buffer[length-1]= 0;
                        m_mtr_steps = atoi32( & buffer[3] );
                   } else {
                        m_mtr_steps = 200;       //one rev
                   }
                   break;
            }
            break;
        case 'G':
            switch( buffer[2] ) {
                case 'V':
                    switch( buffer[3] ) {
                        case 'N':
                            printf( ":VN%05.2f#\r\n", version/100.0 );    
                            break;
                        case 'T':  
                            printf( ":VT%s#\r\n", __TIME__ );  
                            break;
                        case 'D':
                            printf( ":VD%s#\r\n", __DATE__ );    
                            break;
                    }
                    break;
                case 't':
                    printf( ":t=%02d %02d %02d#\r\n", hrs, min, sec );
                    break;
                case 'P':
                    printf( ":P=%ld#\r\n", m_mtr_cnter );
                    break;
                case 'A':
                    switch( buffer[3] ) {
                        case '#':
                            printf( ":ADC 0=%ld 1=%ld#\r\n", lecturaAD0, lecturaAD1 );
                            break;
                        case '0':
                            printf( ":ADC 0=%ld#\r\n", lecturaAD0 );
                            break;
                        case '1':
                            printf( ":ADC 1=%ld#\r\n", lecturaAD1 );
                            break;
                    } 
                    break;
                case 'T':
                    switch( buffer[3] ) {
                        case '#':
                            printf( ":T0=%f[oC] T1=%f[oC]#\r\n", Temp0, Temp1 );
                            break;
                        case '0':
                            printf( ":T0=%f[oC]#\r\n", Temp0 );
                            break;
                        case '1':
                            printf( ":T1=%f[oC]#\r\n", Temp1 );
                            break;
                    } 
                    break;
                case 'L':
                    switch( buffer[3] ) {
                        case '#':
                            printf( ":L1=%ld L2=%ld#\r\n", m_mtr_limit1, m_mtr_limit2 );
                            break;
                        case '1':
                            printf( ":L1=%ld#\r\n", m_mtr_limit1 );
                            break;
                        case '2':
                            printf( ":L2=%ld#\r\n", m_mtr_limit2 );
                            break;
                    } 
                    break;
            }
            break;
        case 'S':
            switch( buffer[2] ) {
                case 'P':
                    if( buffer[3] == '#' ) {
                        m_mtr_cnter = 0;
                    } else {
                        buffer[length-1]= 0;
                        m_mtr_cnter = atoi32( & buffer[3] );
                    }
                    printf( ":P=%ld\r\n#", m_mtr_cnter );
                    break;
            }
            break;
        case ' ':   
            buffer[length-1]= 0;
            printf( ":%s#\r\n", & buffer[2] );   
            break;   
        default:
            printf( ":Unknown command %c#\r\n", buffer[1] );   
    }
}


/** ************************************************************************** 
 *  Main
 */
void main( void )  {
    int i, j, k;
    char c;
	
    int tc;
    
    
    char string[TTY_BUF_LEN];
    
    //------------------------------------------------------------------------
    //      setup uCU
    set_tris_a( 0xFF);                          // PORTA all inputs
    set_tris_b( 0xFF );                         // PORTB all inputs
    set_tris_c( 0xC0 );                         // PORTC all in/out-puts
    set_tris_d( 0x03 );                         // PORTD 
    set_tris_e( 0x00 );                         // PORTE all outputs

    setup_adc_ports( AN0_TO_AN1 | VSS_VDD ); // Pin A0 para lectura Análoga
    setup_adc( ADC_CLOCK_INTERNAL );
    //set_adc_channel( 0 );
   
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
    //lcd_init();
    //lcd_clear_display();

    //------------------------------------------------------------------------
    //  Inicializacion Global Interruptions
    enable_interrupts( GLOBAL );

 
    //-------------------------------------------------------------------------
    //RS232 Test
    //printf( ":VN%05.2f#", version/100.0 );    
    //printf( ":VT%s#", __TIME__ );
    //printf( ":VD%s#", __DATE__ );    

    //-------------------------------------------------------------------------
    //LCD Test
    //sprintf( display[0], "FV: %5.2f", version/100.0 );
    //lcd_goto_xy( 1, 1 );
    //lcd_print( display[0] );
    //sprintf( display[1], "FD: %s", __DATE__ );
    //lcd_goto_xy( 1, 2 );
    //lcd_print( display[1] );
    //sprintf( display[2], "FT: %s", __TIME__ );
    //lcd_goto_xy( 1, 3 );
    //lcd_print( display[2] );

    //------------------------------------------------------------------------
    //      Test uCU
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
    
    m_mtr_cnter = 0;
    m_mtr_steps = 0;
    m_mtr_dir   = 1;
    m_mtr_flag  = false;
    m_mtr_limit = false;
   
    m_mtr_switches = 0;
    m_mtr_switches_old = 0;
    m_mtr_switches_debounce = 0;

    output_high( pin_D2 );
    output_low( pin_D3 );
    //------------------------------------------------------------------------
    // Main Loop
    while( TRUE ) {
        /*
        m_mtr_switches = 0x03 & PORTD;
        if( m_mtr_switches != m_mtr_switches_old ) {
            if( m_mtr_switches == 0 ){
                printf( ":SW=0#" );  
            } else if( m_mtr_switches == 1 ){
                printf( ":SW=1#" );
            } else if( m_mtr_switches == 2 ){
                printf( ":SW=2#" );
            } else if( m_mtr_switches == 3 ){
                printf( ":SW=3#" );
            }
            m_mtr_switches_old = m_mtr_switches;
        }
        */
        
        if( myTmr1_100Hz_flag ) {
            myTmr1_100Hz_flag = false;
            
            m_mtr_switches = 0x03 & PORTD;
            if( ! m_mtr_limit ) {
                if( m_mtr_switches == m_mtr_switches_debounce ) {
                    if( m_mtr_switches != m_mtr_switches_old ) {
                        if( m_mtr_switches == 0 ){
                            printf( ":SW=0... That is imposible!#\r\n" );  
                        } else if( m_mtr_switches == 1 ){
                            m_mtr_limit1 = m_mtr_cnter;
                            printf( ":SW=%d, dir=%d#\r\n", m_mtr_switches, m_mtr_dir );
                            if( m_mtr_dir == 0 ) {
                                m_mtr_dir = 1;
                            } else {
                                m_mtr_Dir = 0;
                            }
                            m_mtr_steps = 200;
                            m_mtr_limit = true;
                        } else if( m_mtr_switches == 2 ){
                            m_mtr_limit2 = m_mtr_cnter; 
                            printf( ":SW=%d, dir=%d#\r\n", m_mtr_switches, m_mtr_dir );
                            if( m_mtr_dir == 0 ) {
                                m_mtr_dir = 1;
                            } else {
                                m_mtr_Dir = 0;
                            }
                            m_mtr_steps = 200;
                            m_mtr_limit = true;
                        } else if( m_mtr_switches == 3 ){
                            printf( ":SW=%d, dir=%d... This is the normal state#\r\n", m_mtr_switches, m_mtr_dir );
                        }
                        m_mtr_switches_old = m_mtr_switches;
                    } 
                } 
            }          
            m_mtr_switches_debounce = 0x03 & PORTD;
            
            if( m_mtr_flag ) {
                if( m_mtr_steps > 0 ){
                    m_mtr_steps --;
                    if( m_mtr_dir > 0 ) {
                        m_mtr_cnter ++;
                        output_high( PIN_D2 );
                    } else {
                        m_mtr_cnter --;
                        output_low( PIN_D2 );
                    } 
                    for( i = 0; i < 8; i ++ ){
                        output_high( PIN_D3 );
                        delay_us( 500 );
                        output_low( PIN_D3 );
                    }
                } else {
                    m_mtr_flag = false;
                    if( m_mtr_limit ) {
                        m_mtr_limit = false;
                        printf( ":Warning Limit reached, motor stopped#\r\n" );
                    } else {
                        printf( ":F=0K#\r\n" );   
                    }
                }
            } else {
                if( m_mtr_limit ) {
                    m_mtr_limit = false;
                    printf( ":Some one is joking with limits switches!#\r\n" );
                }
            }
        }      
        if( myTmr0_8Hz_flag ) {
            myTmr0_8Hz_flag = FALSE;
            
            
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

        set_adc_channel( 0 );
        delay_us(10);
        lecturaAD0=read_adc();    //Lectura AD pin A0
        set_adc_channel( 1 );
        delay_us(10);
        lecturaAD1=read_adc(); 
        
        Temp0  = (float) lecturaAD0;
        Temp0 -= 180.0;
        Temp0 *= 55./520.;
        Temp1  = (float) lecturaAD1;
        Temp1 -= 180.0;
        Temp1 *= 55./520.;

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
