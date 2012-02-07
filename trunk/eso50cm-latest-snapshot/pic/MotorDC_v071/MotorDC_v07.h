#include <18F4431.h>
#device adc=10

#FUSES NOWDT                    //No Watch Dog Timer
#FUSES WDT128                   //Watch Dog Timer uses 1:128 Postscale
#FUSES H4                       //High speed PLL
#FUSES NOPROTECT                //Code not protected from reading
#FUSES IESO                     //Internal External Switch Over mode enabled
#FUSES NOBROWNOUT               //No brownout reset
#FUSES BORV27                   //Brownout reset at 2.7V
#FUSES NOPUT                    //No Power Up Timer
#FUSES NOCPD                    //No EE protection
#FUSES STVREN                   //Stack full/underflow will cause reset
#FUSES NODEBUG                  //No Debug mode for ICD
#FUSES NOLVP                    //No Low Voltage Programming on B3(PIC16) or B5(PIC18)
#FUSES NOWRT                    //Program memory not write protected
#FUSES NOWRTD                   //Data EEPROM not write protected
#FUSES FCMEN                    //Fail-safe clock monitor enabled
#FUSES NOWINEN                  //WDT Timer Window Disabled
#FUSES T1LOWPOWER               //Timer1 low power operation when in sleep
#FUSES HPOL_HIGH                //High-Side Transistors Polarity is Active-High (PWM 1,3,5 and 7)
#FUSES NOWRTC                   //configuration not registers write protected
#FUSES NOWRTB                   //Boot block not write protected
#FUSES NOEBTR                   //Memory not protected from table reads
#FUSES NOEBTRB                  //Boot block not protected from table reads
#FUSES NOCPB                    //No Boot Block code protection
#FUSES LPOL_HIGH                //Low-Side Transistors Polarity is Active-High (PWM 0,2,4 and 6)
#FUSES PWMPIN                   //PWM outputs disabled upon Reset
#FUSES MCLR                     //Master Clear pin enabled
#FUSES FLTAC1                   //FLTA input is multiplexed with RC1
#FUSES SSP_RD                   //SCK/SCL=RD3, SDA/SDI=RD2, SDO=RD1
#FUSES PWM4D5                   //PWM4 output is multiplexed on RD5
#FUSES EXCLKC3                  //TMR0/T5CKI external clock input is muliplexed with RC3

#include <stdlib.h>
#include <string.h>

#define I2C_ADDRESS 0xA2        //MotorRA
//#define I2C_ADDRESS 0xA4        //MotorDEC

#use delay(clock=40000000)
#use rs232( baud=57600, xmit=PIN_C6, rcv=PIN_C7 ) 
#use i2c( slave, sda=PIN_C4, scl=PIN_C5, address=I2C_ADDRESS, force_hw )
//#use i2c( slave, sda=PIN_D2, scl=PIN_D3, address=I2C_ADDRESS )
#define I2C_SDA   PIN_D2
#define I2C_SCL   PIN_D3

#byte   PORTA = 0xF80
#byte   PORTB = 0xF81
#byte   PORTC = 0xF82
#byte   PORTD = 0xF83
#byte   PORTE = 0xF84

//                           -------------------
//           < MCLR |-------| MCLR          RB7 |
//                          | RA0           RB6 |
//                          | RA1           RB5 |
//            | QEI >-------| RA2     P     RB4 |-------| PWM5* >
//            | QEA >-------| RA3     I     RB3 |-------| PWM3* > 
//            | QEB >-------| RA4     C     RB2 |-------| PWM2  >
//                          | RA5           RB1 |-------| PWM1* >
//    (Debug) < QEA |-------| RE0     1     RB0 |-------| PWM0  >
//    (Debug) < QEB |-------| RE1     8     Vdd |
//    (Debug) < QEI |-------| RE2     F     Vss |
//                          | Vdd     4     RD7 |-------| PWM7* >
//                          | Vss     4     RD6 |-------| PWM6  >
//                          | OSC1    3     RD5 |-------| PWM4  >
//                          | OSC2    1     RD4 |
//                 <|-------| RC0           RC7 |-------< RX | 
//                          | RC1           RC6 |-------| TX >  
//                          | RC2           RC5 |
//                 <|-------| RC3           RC4 |
//                 <|-------| RD0           RD3 |-------< SCL |  
//           |  SD0 >-------| RD1           RD2 |-------< SDA |                 
//                           -------------------
