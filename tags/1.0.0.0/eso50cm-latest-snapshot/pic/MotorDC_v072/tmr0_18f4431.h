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


//------------------------------------------------------------------------------
//  Setup Timer0
//      o                1    
//          Ttmt0 = ( -------- ) * PS * N    ( PS = RTCC_DIV_X )
//                    (Fosc/4)
//
//        donde N es el numero de pasos para que el contador de TMR0 haga 
//        overflow, i.e: ( 0xFFFF - N )  
//      o Para 100[Hz], con Fosc/4 = 5[Mhz] PS = 2
//                  
//                    1        (Fosc/4)
//          N =   --------- * ---------- = 25000
//                 100[Hz]        PS
//
//      o Resolucion
//                    PS
//          dt =  ----------
//                 (Fosc/4)
//     
//
//          Dt = 65535 * dt
//
//          o PS=  1:   dt = 0.2 [us]       Dt = 13.107[ms]
//          o PS=  2:   dt = 0.4 [us]       Dt = 26.214[ms]
//          o PS=  4:   dt = 0.8 [us]       Dt = 52.428[ms]
//            .
//            .
//            .
//          o PS=256:   dt =51.2[us]        Dt =  3.356[s]
//------------------------------------------------------------------------------

void SetupTMR0( void ) {
    setup_timer_0( RTCC_INTERNAL | RTCC_DIV_2 );
    set_timer0( 0 );
}



