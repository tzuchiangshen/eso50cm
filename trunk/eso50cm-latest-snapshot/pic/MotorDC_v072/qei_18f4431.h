/** **************************************************************************
 * Quadrature Encoder Interface
 * o Tmr0 to Tmr5 ratio
 *    -- Both use Fosc/4 as input clock
 *    -- Tmr0 Prescaler:
 *         RTCC_DIV_2 => PS = 2
 *         => Period = PS/(Fosc/4) 
 *    -- Tmr5 Prescaler: 
 *         PIC_T5CONbits.T5PS    = 1;  =>      PS = 2
 *         => Period = PS/(Fosc/4)
 *    
 * o Velocity Postscaler
 *    -- PIC_QEICONbits.PDEC     = QEI_RED_1_16;     // = 1:16
 *       Every 16 pulses the Tmr5 is reseted.
 *
 * o Velocity Update mode
 *    -- PIC_CAP1CONbits.CAP1REN = TRUE;         //Reset TMR5
 *       PIC_CAP1CONbits.CAP1M   = 15;           //The trigger on every rising edge on CAP1 inp
 *
 * o Delay in the Input Capture 1 Interruption (Velocity Update)
 *    -- With this configuration there are a 31 Tics of diff betwwen the measured 
 *       pulse width and the real width. 
 */

//============================================================================
//                                  QEI
//============================================================================

#define     QEI_4X_ResetPERIOD  0x06
#define     QEI_4X_ResetINDX    0x05
#define     QEI_2X_ResetPERIOD  0x02
#define     QEI_2X_ResetINDX    0x01
#define     QEI_OFF             0x00

#define     QEI_RED_1_64        0x03
#define     QEI_RED_1_16        0x02
#define     QEI_RED_1_4         0x01
#define     QEI_RED_1_1         0x00

//****************************************************************************
//                                  CAP1CON
//****************************************************************************
struct CAP1CONmap {
    int             CAP1M:4;            // Input capture 1 mode select bits
    BOOLEAN         unused1;            // unimplemented read as 0
    BOOLEAN         unused2;            // unimplemented read as 0
    BOOLEAN         CAP1REN;            // Time Base reset enable bit
    BOOLEAN         unused3;
} PIC_CAP1CONbits;
#byte       PIC_CAP1CON      = 0xF63
#byte       PIC_CAP1CONbits  = 0xF63

//****************************************************************************
//                                  QEICON
//****************************************************************************
struct QEICONmap {
    int             PDEC:2;             // Velocity pulse reduction bit
    int             QEIM:3;             // QEI modes bits
    BOOLEAN         UPDOWN;             // Direction of rotation
    BOOLEAN         ERROR;              // QEI error: position counter overflow/underflow
    BOOLEAN         noVELM;             // Velocity mode bit
} PIC_QEICONbits;
#byte       PIC_QEICON      = 0xFB6
#byte       PIC_QEICONbits  = 0xFB6

//****************************************************************************
//                                  DFLTCON
//****************************************************************************
struct DFLTCONmap {
    int             FLTCK:3;
    BOOLEAN         FLT1EN;
    BOOLEAN         FLT2EN;
    BOOLEAN         FLT3EN;
    BOOLEAN         FLT4EN;
    BOOLEAN         unused;
} PIC_DFLTCONbits;
#byte       PIC_DFLTCON     = 0xF60
#byte       PIC_DFLTCONbits = 0xF60

//****************************************************************************
//                                  VREG
//****************************************************************************
#byte       PIC_VREGH       = 0xF69
#byte       PIC_VREGL       = 0xF68

//****************************************************************************
//                                  POSCNT
//****************************************************************************
#byte       PIC_POSCNTH     = 0xF67
#byte       PIC_POSCNTL     = 0xF66

//****************************************************************************
//                                  MAXCNT
//****************************************************************************
#byte       PIC_MAXCNTH     = 0xF65
#byte       PIC_MAXCNTL     = 0xF64

/** ***************************************************************************
 * Setup QEI
 */  
void SetupQEI( void ) {
    PIC_QEICONbits.noVELM   = FALSE;                // Velocity mode enabled = 0
    PIC_QEICONbits.QEIM     = QEI_4X_ResetPERIOD;  
    PIC_QEICONbits.PDEC     = QEI_RED_1_1;          // = 1:1
    //PIC_QEICONbits.PDEC     = QEI_RED_1_16;         // = 1:16
    //PIC_QEICONbits.PDEC     = QEI_RED_1_64;         // = 1:64


    //PIC_MAXCNTH= 0x0B;                            //MAXCNT = 0...3000;
    //PIC_MAXCNTL= 0xB8;                      
    PIC_MAXCNTH= 0x17;                              //MAXCNT = 0...6000;
    PIC_MAXCNTL= 0x70;                      

    PIC_DFLTCONbits.FLT4EN  = FALSE;
    PIC_DFLTCONbits.FLT3EN  = TRUE;         //QEB input
    PIC_DFLTCONbits.FLT2EN  = TRUE;         //QEA input
    PIC_DFLTCONbits.FLT1EN  = TRUE;         //INDX input
    PIC_DFLTCONbits.FLTCK   = 0x00;         //Noise filter clock divider 1:1

    //VEL MODE
    PIC_CAP1CONbits.CAP1REN = TRUE;         //Reset TMR5
    PIC_CAP1CONbits.CAP1M   = 15;           //The trigger on every rising edge on CAP1 inp


    PIC_POSCNTH= 0;
    PIC_POSCNTL= 0;
    PIC_VREGH  = 0;
    PIC_VREGL  = 0;

}
//============================================================================
//                                  TMR5
// Configuration for continuous QE-Velocity capture
//============================================================================

//****************************************************************************
//                                  T5CON
//****************************************************************************
struct T5CONmap 
{
    BOOLEAN         TMR5ON;             // Timer 5 On bit
    BOOLEAN         TMR5CS;             // Timer 5 clock source select bit
    BOOLEAN         noT5SYNC;           // Timer 5 external clock input synchronization select bit
    int             T5PS:2;             // Timer 5 input clock prescaler
    BOOLEAN         T5MOD;              // Timer 5 mode bit
    BOOLEAN         noRESEN;            // Special event reset bit
    BOOLEAN         T5SEN;              // Timer 5 sleep enable bit
} PIC_T5CONbits;
#byte       PIC_T5CON      = 0xFB7
#byte       PIC_T5CONbits  = 0xFB7

//****************************************************************************
//                                  PR5
//****************************************************************************
#byte       PIC_PR5L       = 0xF90
#byte       PIC_PR5H       = 0xF91

//------------------------------------------------------------------------------
//  SetupTMR5
//------------------------------------------------------------------------------
void SetupTMR5( void )
{
    PIC_T5CONbits.TMR5ON   = TRUE;        // On
    PIC_T5CONbits.TMR5CS   = FALSE;       // Internal clock Tcy
    PIC_T5CONbits.noT5SYNC = TRUE;        // no syncronize external clock input
    //PIC_T5CONbits.T5PS     = 0;           // prescaler = 1:1
    PIC_T5CONbits.T5PS     = 1;           // prescaler = 1:2
    //PIC_T5CONbits.T5PS     = 2;           // prescaler = 1:4
    //PIC_T5CONbits.T5PS     = 3;           // prescaler = 1:8
    PIC_T5CONbits.T5MOD    = FALSE;       // continuous mode count
    PIC_T5CONbits.noRESEN  = FALSE;       // special event reset enabled
    PIC_T5CONbits.T5SEN    = FALSE;       // TMR5 disabled during sleep

    PIC_PR5H = 0xFF;
    PIC_PR5L = 0xFF;
}



//============================================================================
//                                  TMR1
//============================================================================

//------------------------------------------------------------------------------
//  Setup Timer1
//      o                1    
//          Ttmr1 = ( -------- ) * PS * N    ( PS = T1_DIV_BY_X )
//                    (Fosc/4)
//
//        donde N es el numero de pasos para que el contador de TMR1 haga 
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


void SetupTMR1( void ) {
    setup_timer_1( T1_INTERNAL | T1_DIV_BY_2 );
    set_timer1( 0 );
}
