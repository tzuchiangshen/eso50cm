//****************************************************************************
//                                  CONFIG REGISTERs
//****************************************************************************
#byte PIC_ADRESH        = 0xFC4
#byte PIC_ADRESL        = 0xFC3
#byte PIC_ADCON0        = 0xFC2
#byte PIC_ADCON1        = 0xFC1
#byte PIC_ADCON2        = 0xFC0
#byte PIC_ADCON3        = 0xF9A
#byte PIC_ADCHS         = 0xF99
#byte PIC_ANSEL0        = 0xFB8
#byte PIC_ANSEL1        = 0xFB9



//****************************************************************************
//                                  Subrutinas
//****************************************************************************

//------------------------------------------------------------------------------
//  SetupADC
//------------------------------------------------------------------------------
void SetupADC( void )
{
    //Configuring analog pisn
    PIC_ANSEL0  = 0b00000010;               //pin RA1 = AN1
    PIC_ANSEL1  = 0b00000000;

    //Selecting a/d input chanels
    PIC_ADCHS   = 0b00000000;               //chanel = AN1 (single chanel)

    //Selecting a/d auto-conversion mode
    PIC_ADCON0  = 0b00000100;                //Single shot + Single channle + Group B
    //PIC_ADCON0  = 0b00100100;                //Continous mode + Single channle + Group B

    //Selecting voltage level & buffer
    PIC_ADCON1  = 0b00000000;

    //Selctiong a/d conversion clock
    PIC_ADCON2  = 0b10000100;                //Right justified+No delay+Fosc/4

    //Selecting conversion trigger
    //PIC_ADCON3  = 0b00000000;                //interupion on each word + all triggers disabled
    PIC_ADCON3  = 0b01000000;                //interupion on 2nd and 4th word + all triggers disabled

    //No interruption settings
    //PIC_ADCON0 |= 0b00000001;                //start adc 

    delay_ms( 1 );
}
