//****************************************************************************
//                                  OVCOND
//****************************************************************************
#byte       PIC_OVCONS      = 0xF6A

//****************************************************************************
//                                  OVCOND
//****************************************************************************
#byte       PIC_OVCOND      = 0xF6B

//------------------------------------------------------------------------------
//                                  Constantes
#DEFINE     PTMR_1KHz       4095            // Aprox Fpwm = 1[KHz]
#DEFINE     PTMR_10KHz      500             // Aprox Fpwm = 10[KHz]
#DEFINE     PTMR_20KHz      250             // Aprox Fpwm = 20[KHz]
    
/** **************************************************************************
 *
 *  Setup modulos powerPWM
 *      o   Se usan los modulos 0 y 1 para generar dos sen~ales PWM
 *      o   Cada modulo genera, en sus correspondientes pines, 
 *          se~nales complementarias
 *      o   Periodo base del ciclo PWM es **250**, que (con Fosc = 20[MHz] 
 *          mas Pre y Post scalers), genera una sen~al de 20[KHz]
 *          **250** son 8[bits] lo que implica un PWMduty util de 10[bits],
 *          que es un numero entre 0 (%100 off) y 1023(%100 on)
 *
 *                  ( PTMR + 1 ) x PTMR_PstS
 *      o  Tpwm = -----------------------------
 *                          Fosc / 4
 *
 */
void SetupPowerPWM( void ) {
    setup_power_pwm( 
        PWM_CLOCK_DIV_4 | 
        PWM_FREE_RUN | 
        PWM_DEAD_CLOCK_DIV_4,
        1,                              // postscale
        0,                              // time_base = ini val for PTMR
        PTMR_10KHz,                     // PTPERH PTPERL = pPWM period
        0,                              //compare
        1,                              //compare_postscale
        0                               //dead_time
    );
   
    set_power_pwm0_duty( 0 );
    set_power_pwm2_duty( 0 );
    set_power_pwm4_duty( 0 );
    set_power_pwm6_duty( 0 );
    
    PIC_OVCONS= 0x00;                   //Valores de los pines en caso de
                                        //overide cond. (Lo somplementarios 
                                        //siguen siendo complentarios) 
    
    //setup_power_pwm_pins( 
    //    PWM_COMPLEMENTARY,
    //    PWM_COMPLEMENTARY,
    //    PWM_COMPLEMENTARY,
    //    PWM_COMPLEMENTARY
    //);
    
    
}
