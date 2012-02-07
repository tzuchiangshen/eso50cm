//****************************************************************************
//                                  T0CON
//****************************************************************************
struct T0CONmap 
{
    unsigned int    T0PS:3;             //Timer0 Prescaler
    BOOLEAN         PSA;                //Timer0 Prescaler assignment bit
    BOOLEAN         T0SE;               //Timer0 Source Edge Select bit
    BOOLEAN         T0CS;               //Timer0 Source Clock Select bit
    BOOLEAN         T016BITS;           //Timer0 16-bits Control bit
    BOOLEAN         TMR0ON;             //Timer0 On/Off Control bit
} PIC_T0CONbits;
#byte       PIC_T0CON       = 0xFD5
#byte       PIC_T0CONbits   = 0xFD5

//****************************************************************************
//                                  TMR0
//****************************************************************************
#byte       PIC_TMR0L       = 0xFD6
#byte       PIC_TMR0H       = 0xFD7

//int NTmr0;

//------------------------------------------------------------------------------
//  Setup Timer0
//      o                1    
//          Ttmt0 = ( -------- ) * PS * N    ( PS = 2 )
//                    (Fosc/4)
//
//        donde N es el numero de pasos para que el contador de TMR0 haga 
//        overflow, i.e: ( 0xFFFF - N )  
//      o Para 100[Hz], con Fosc/4 = 5[Mhz] y PS = 2 
//                  
//                    1        (Fosc/4)
//          N =   --------- * ---------- = 25000
//                 100[Hz]        PS
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//  Setup Timer0
//      o   Timero se utiliza para actualizar el DutyCicle de las sen~ales 
//          PWM.
//      o   El perido total de las sen~al sinusoidal generada es el perido
//          entre interrucpciones del Timer0 multiplicado por el numero de
//          entradas de la tabla SINE
//      o   Con Fosc = 20[MHz] (mas Pre y Post scalers) se tiene que para
//          generar una sen~al de 60[Hz] es necesario que Timer0 sea
//          2605 (0xFFFF - 2605)
//------------------------------------------------------------------------------
//void SetupTimer0( void )
//{
//    setup_timer_0(RTCC_INTERNAL|RTCC_DIV_2);
//    set_timer0( 0 );                    //ini value for Timer0   
//}
void SetupTMR0( void )
{
    setup_timer_0(RTCC_INTERNAL|RTCC_DIV_2);
    //set_timer0( 40535 );                    //ini value for Timer0 => 100[Hz]   
    set_timer0( 0 );

    //NTmr0 = 100;
}



