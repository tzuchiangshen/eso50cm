//****************************************************************************
//                                  AUXILIAR
//****************************************************************************
#define     I2C_MASK     0x2D

//****************************************************************************
//                                  SSPBUF
//****************************************************************************
#byte       PIC_SSPBUF      = 0xFC9

//****************************************************************************
//                                  SSPCON
//****************************************************************************
struct SSPCONmap 
{
    unsigned int    SSPM:4;             //Synchronous Serial Port Mode Select bits
    BOOLEAN         CKP;                //SCK Realese Control bit
    BOOLEAN         SSPEN;              //Synchronous Serial Port Enable bit
    BOOLEAN         SSPOV;              //Receive Overflow Indicator bit,
    BOOLEAN         WCOL;               //Write Collision Detect bit
} PIC_SSPCONbits;
#byte       PIC_SSPCON      = 0xFC6
#byte       PIC_SSPCONbits  = 0xFC6

//****************************************************************************
//                                  SSPADD
//****************************************************************************
#byte       PIC_SSPADD      = 0xFC8

//****************************************************************************
//                                  SSPSTAT
//****************************************************************************
struct SSPSTATmap 
{
    BOOLEAN BF;                         //Buffer Full Status bit
    BOOLEAN UA;                         //Update Address (10 bit Salve mode only)
    BOOLEAN R_W;                        //Read/!Write bit information
    BOOLEAN S;                          //Start bit
    BOOLEAN P;                          //Stop bit
    BOOLEAN D_A;                        //Data/!Address bit
    BOOLEAN CKE;                        //SMBus Select bit
    BOOLEAN SMP;                        //Slew Rate Control bit
} PIC_SSPSTATbits;
#byte       PIC_SSPSTAT     = 0xFC7
#byte       PIC_SSPSTATbits = 0xFC7

//****************************************************************************
//                                  TRISC
//****************************************************************************
struct TRISCmap 
{
    BOOLEAN RC0;
    BOOLEAN RC1;
    BOOLEAN RC2;
    BOOLEAN RC3;
    BOOLEAN RC4;                        //SDA
    BOOLEAN RC5;                        //SCL
    BOOLEAN RC6;
    BOOLEAN RC7;
} PIC_TRISCbits;
#byte       PIC_TRISC       = 0xF94
#byte       PIC_TRISCbits   = 0xF94

//----------------------------------------------------------------------------
//                                  WriteI2C
//----------------------------------------------------------------------------
int WriteI2C( byte byte_out )
{
    PIC_SSPBUF= byte_out;
    PIC_SSPCONbits.CKP= 1;          //Realease SCL
    if( PIC_SSPCONbits.WCOL ) 
    {
        return -1;
    }
    else
    {
        while( PIC_SSPSTATbits.BF );
        return 0;
    }
}




