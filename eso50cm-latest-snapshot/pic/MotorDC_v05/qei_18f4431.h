#define     QEI_4X_ResetPERIOD  0x06
#define     QEI_4X_ResetINDX    0x05
#define     QEI_2X_ResetPERIOD  0x02
#define     QEI_2X_ResetINDX    0x01
#define     QEI_OFF             0x00

#define     QEI_RED_1_64        0x03
#define     QEI_RED_1_16        0x02
#define     QEI_RED_1_4         0x01
#define     QEI_RED_1_1         0x00

//------------------------------------------------------------------------------
//                                  CAP1CON
struct CAP1CONmap {
    int             CAP1M:4;            // Input capture 1 mode select bits
    BOOLEAN         unused1;            // unimplemented read as 0
    BOOLEAN         unused2;            // unimplemented read as 0
    BOOLEAN         CAP1REN;            // Time Base reset enable bit
    BOOLEAN         unused3;
} PIC_CAP1CONbits;
#byte       PIC_CAP1CON      = 0xF63
#byte       PIC_CAP1CONbits  = 0xF63

//------------------------------------------------------------------------------
//                                  QEICON
struct QEICONmap {
    int             PDEC:2;             // Velocity pulse reduction bit
    int             QEIM:3;             // QEI modes bits
    BOOLEAN         UPDOWN;             // Direction of rotation
    BOOLEAN         ERROR;              // QEI error: position counter overflow/underflow
    BOOLEAN         noVELM;             // Velocity mode bit
} PIC_QEICONbits;
#byte       PIC_QEICON      = 0xFB6
#byte       PIC_QEICONbits  = 0xFB6

//------------------------------------------------------------------------------
//                                  DFLTCON
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

//------------------------------------------------------------------------------
//                                  VREG
#byte       PIC_VREGH       = 0xF69
#byte       PIC_VREGL       = 0xF68

//------------------------------------------------------------------------------
//                                  POSCNT
#byte       PIC_POSCNTH     = 0xF67
#byte       PIC_POSCNTL     = 0xF66

//------------------------------------------------------------------------------
//                                  MAXCNT
#byte       PIC_MAXCNTH     = 0xF65
#byte       PIC_MAXCNTL     = 0xF64

/** ***************************************************************************
 * Setup QEI
 */  
void SetupQEI( void ) {
    PIC_QEICONbits.noVELM   = TRUE;             // Velocity mode enabled = 0
    PIC_QEICONbits.QEIM     = QEI_2X_ResetPERIOD;  
    PIC_QEICONbits.PDEC     = QEI_RED_1_1;      // = 1:1
    //PIC_QEICONbits.PDEC     = QEI_RED_1_4;      // = 1:4

    //PIC_QEICON = 0b00011000;

    PIC_MAXCNTH= 0x0B;                      //MAXCNT = 0...3000;
    PIC_MAXCNTL= 0xB8;                      //revisar si es 2999 3000

    PIC_DFLTCONbits.FLT4EN  = FALSE;
    PIC_DFLTCONbits.FLT3EN  = TRUE;         //QEB input
    PIC_DFLTCONbits.FLT2EN  = TRUE;         //QEA input
    PIC_DFLTCONbits.FLT1EN  = TRUE;         //INDX input
    PIC_DFLTCONbits.FLTCK   = 0x00;         //Noise filter clock divider 1:1

    //VEL MODE
    //PIC_CAP1CONbits.CAP1REN = TRUE;         //Reset TMR5
    //PIC_CAP1CONbits.CAP1M   = 15;           //The trigger on every rising edge on CAP1 inp
    PIC_CAP1CONbits.CAP1REN = FALSE;         //Reset TMR5
    PIC_CAP1CONbits.CAP1M   = 15;           //The trigger on every rising edge on CAP1 inp


    PIC_POSCNTH= 0;
    PIC_POSCNTL= 0;
    PIC_VREGH  = 0;
    PIC_VREGL  = 0;

}
