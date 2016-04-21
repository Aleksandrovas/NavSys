#include <avr/io.h>
#define RSTport PORTB
#define RSTpin PB2
#define DCport PORTB
#define DCpin PB3
#define SSport	PORTB
#define SSpin	PB4

void lcd_init();
void lcd_gotoxy(unsigned char x,unsigned char y);
void lcd_fill(unsigned char dat);
char lcd_ASCII_to_IMG(char ascii,unsigned char *img);
void lcd_clear();
void lcd_writeStringXY (unsigned char x, unsigned char y,  char *tekstas,unsigned char maxlen);
void lcd_WriteInt(unsigned char x,unsigned char y,unsigned long skaic);
