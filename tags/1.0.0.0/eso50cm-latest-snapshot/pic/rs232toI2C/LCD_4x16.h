/**
 * As defined in the following structure the pin connection is as follows:
 *     RD0  rs
 *     RD1  rw
 *     RD2  enable
 *     RD3  unused
 *     RD4  D4
 *     RD5  D5
 *     RD6  D6
 *     RD7  D7
 *
 *   LCD pins D0-D3 are not used and PIC RD3 is not used.
 */
// Un-comment the following define to use port B
// #define use_portb_lcd TRUE

/**
 * Pin map of port used to comm with LCD.
 */
struct lcd_pin_map 
{
    BOOLEAN rs;                         //Pin RD0
    BOOLEAN rw;                         //Pin RD1
    BOOLEAN enable;                     //Pin RD2
    BOOLEAN unused;                     
    int     data : 4;                   //Pins RD4-RD7.
} lcd;

/**
 * lcd byte maped to the following address
 * PIC16F877A PORTD = 0x08
 * PIC18F4431 PORTD = 0xF83
 */
//#byte lcd = 0x08                 	    //On to port D (at address 8) PIC16f877A
#byte lcd = 0xF83                 	    //On to port D (at address 8) PIC18F4431 and PIC18F4550

#define set_tris_lcd(x) set_tris_d(x)

#define	BITS4_LINES4 0b00101000	//4 Bits interface, 4 lineas

#define DISPLAY_ON   0x0C
#define DISPLAY_OFF  0x08

#define CURSOR_ON    0b00001110	//0x0E
#define CURSOR_OFF   0b00001100	//0x0C

#define BLINKING_ON  0x0F
#define BLINKING_OFF 0x0E

#define CURSOR_INC   0x06
#define CURSOR_DEC   0x04

#define SHIFT_DISP   0x05
#define NOT_SHIFT    0x06

#define CLR_DISPLAY  0x01
#define RETURN_HOME  0x03

#define CURSOR_SKIP  0xC0
#define CURSOR_BACK  0x80


/**
 * The following are used for setting
 * the I/O port direction register.
 */
struct lcd_pin_map const LCD_WRITE = {0,0,0,0,0}; // For write mode all pins are out
struct lcd_pin_map const LCD_READ = {0,0,0,0,15}; // For read mode data pins are in

/**
 * Read byte from LCD
 * @return readed byte (BYTE = char)
 */
BYTE lcd_read_byte()
{
     BYTE low,high;
    
    set_tris_lcd(LCD_READ);
    lcd.rw = 1;
    delay_cycles(1);
    lcd.enable = 1;
    delay_cycles(1);
    high = lcd.data;
    lcd.enable = 0;
    delay_cycles(1);
    lcd.enable = 1;
    delay_us(1);
    low = lcd.data;
    lcd.enable = 0;
    set_tris_lcd(LCD_WRITE);
    return( (high<<4) | low);
}


/**
 * send nibble to LCD
 * @return void
 */
void lcd_send_nibble( BYTE n ) {
    //lcd.data = n;
    //delay_ms(5);
    //lcd.enable = 1;
    //delay_ms(5);
    //lcd.enable = 0;
    //delay_ms(10);
        
    lcd.enable = 1;
    delay_us(60);
    lcd.data = n;
    delay_us(500);
    lcd.enable = 0;
    delay_us(500);
}


/**
 * send byte to LCD
 * @return void
 */
void lcd_send_byte( BOOLEAN rs, BYTE n ) {
    lcd.rw = 0;
    lcd.rs = rs;
    delay_us(60);        //ojo
    lcd_send_nibble( n >> 4 );
    lcd_send_nibble( n & 0xf );
}

/**
 * Initialize LCD. Now configured as 4 lines x 16 chars LCD.
 * @return void
 */
void lcd_init( void ){
    BYTE i;

    set_tris_lcd(LCD_WRITE);
    lcd.rs = 0;
    lcd.rw = 0;
    lcd.enable = 0;

    delay_ms(15);
    for( i = 1; i <= 3; ++ i ) {
        lcd_send_nibble(3);
        delay_ms(5);
    }
    lcd_send_nibble(2);
    delay_ms(5);

    lcd_send_byte( 0, BITS4_LINES4 );
    lcd_send_byte( 0, DISPLAY_OFF );
    lcd_send_byte( 0, DISPLAY_ON );
    lcd_send_byte( 0, CURSOR_INC );
    lcd_send_byte( 0, CURSOR_OFF );
    //lcd_send_byte( 0, BLINKING_ON );
}

/**
 * Move cursor to (x,y)
 * @param x (BYTE) column
 * @param y (BYTE) row
 * @return void
 */
void lcd_goto_xy( BYTE x, BYTE y) {
    BYTE address;

    switch( y ) {
        case 1 : 
            address=0x00;
            break;
        case 2 : 
            address=0x40;
            break;
        case 3 : 
            address=0x10;
            break;
        case 4 : 
            address=0x50;
            break;
   }
   address += x - 1;
   lcd_send_byte( 0, 0x80 | address );
}


/**
 * Put char on LCD's cursor position.
 * Interprets special caracters: '\f', '\n', '\b'
 * @param c (char)
 * @return void
 */
void lcd_putc( char c) {
   switch (c) {
        case '\f'   : 
            lcd_send_byte(0,1);
            delay_ms(2);
            break;
        case '\n'   : 
            lcd_goto_xy(1,2);        
            break;
        case '\b'   : 
            lcd_send_byte(0,0x10);  
            break;
        default     : 
            lcd_send_byte(1,c);     
    }
}

/**
 * get char on LCD's cursor position.
 * @param x (BYTE) column
 * @param y (BYTE) row
 * @return readed char (char)
 */
char lcd_getc( BYTE x, BYTE y) {
    char value;

    lcd_goto_xy(x,y);
    while ( bit_test(lcd_read_byte(),7) ); // wait until busy flag is low
    lcd.rs=1;
    value = lcd_read_byte();
    lcd.rs=0;
    return(value);
}

/**
 * Prints string on LCD.
 * @param str (char *)
 * @return void
 */
void lcd_print( char *str ){
    int i;
    
    for( i= 0; i< 16 && str[i]!='\0'; i++ ){
        lcd_putc( str[i] );
    }
    while( i < 16 ) {
        lcd_putc( ' ' );
        i ++;
    }    
        
}

/**
 * Clear LCD.
 * @return void
 */
void lcd_clear_display( void ) {
    lcd_send_byte( 0, CLR_DISPLAY );
}

