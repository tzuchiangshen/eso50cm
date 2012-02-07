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
