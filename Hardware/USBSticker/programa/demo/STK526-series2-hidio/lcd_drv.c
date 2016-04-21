#include <avr/io.h>
#include "lcd_drv.h"
#define F_CPU 8000000UL
#include <util/delay.h>
#include <string.h>

static const uint8_t Font[36*5]  =
{ 126, 17, 17, 17, 126,	//a
127, 73, 73, 73, 54,	//b
62, 65, 65, 65, 65,		//c
127, 65, 65, 65, 62,	//d
127, 73, 73, 73, 65,	//e
127, 9, 9, 9, 1,		//f
62, 65, 73, 73, 56,		//g
127, 8, 8, 8, 127,		//h
 0, 65, 127, 65, 0,		//i
65, 65, 65, 33, 31,		//j
127, 8, 20, 34, 65,		//k
127, 64, 64, 64, 0,		//l
127, 2, 4, 2, 127,		//m
127, 2, 4, 8, 127,		//n
62, 65, 65, 65, 62,		//o
127, 9, 9, 9, 6,		//p
62, 65, 65, 33, 94,		//q
127, 9, 25, 41, 70,		//r
70, 73, 73, 73, 49,		//s
1, 1, 127, 1, 1,		//t
63, 64, 64, 64, 63,		//u
15, 48, 64, 48, 15,		//v
31, 96, 24, 96, 31,		//w
99, 20, 8, 20, 99,		//x
3, 4, 120, 4, 3,		//y
97, 81, 73, 69, 67,		//z
62, 97, 73, 67, 62,		//0
72, 76, 126, 127, 64,	//1
66, 97, 81, 73, 70,		//2
34, 73, 73, 73, 62, 	//3
28, 18, 17, 127, 16,	//4
47, 73, 73, 73, 49,		//5
62, 73, 73, 73, 48,		//6
1, 97, 49, 13, 3, 		//7
 54, 73, 73, 73, 54,	//8
 6, 73, 73, 73, 62,		//9

 };
 //--------------------------------------------------------------------------
void Init_SPI()
{
	DDRB |= (1<<PB3)|(1<<PB5)|(1<<PB2);
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);//|(1<<SPR1);
}
//--------------------------------------------------------------------------
void SPI_Send(char cData,unsigned char cr)
{
	unsigned char tmp;
	tmp=SPCR;
	SPCR=cr;
	SSport&=~(1<<SSpin);
	SPDR = cData;
	while(!(SPSR & (1<<SPIF)));
	SSport|=(1<<SSpin);
	SPCR=tmp;
}
//--------------------------------------------------------------------------
void memfill(unsigned char *to,unsigned char value,int count)
{
	unsigned int i=0;
	for (i=0;i<count;i++)
		to[i]=value;
	
}
//--------------------------------------------------------------------------
unsigned char IntToStr(unsigned long sk,char *str)
{
	unsigned long i=10,n=0;
	while (i<=sk)	i*=10;
	while(i>=10)
	{
		str[n]=(unsigned long)10*sk/i%10+48;
		i/=10;
		n++;
	}
	str[n]=0;
	return n;
}
void lcd_WriteInt(unsigned char x,unsigned char y,unsigned long skaic)
{
	char st[9];
	int ilg;
	ilg=IntToStr(skaic,st);
	ilg=5;
	lcd_writeStringXY(x,y,"         ",ilg);
	lcd_writeStringXY(x,y,st,ilg);
}
unsigned char SPImod(unsigned char value)
{
	if (value == 0)
	{
		value=SPCR;
		SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
	}
	return SPCR;
};
 //--------------------------------------------------------------------------
char lcd_ASCII_to_IMG(char ascii,unsigned char *img)
{
	char min=0;
	memfill(img,0,6);
	switch (ascii){
		//dvitask
		case 58: 
			img[2]=2+4+16+32;
			img[3]=2+4+16+32;
		break;
		//zvaigzd
		case 42: 
			img[1]=10;
			img[2]=7;
			img[3]=10;
		break;
		default:
			if ((ascii >= 48) && (ascii <=57))
				memcpy(img,&Font[(ascii-48+26)*5],5);
			else 
			{	
				if ( (ascii >=65) && (ascii <=90) ) 
					min=65;
				else 
					if ( (ascii >=97) && (ascii <=122) ) 
						min=97;
					else 
						break;
					memcpy(img,&Font[(ascii-min)*5],5);
			}
		break;
		}
		return min;
}

//--------------------------------------------------------------------------
void lcd_fill(unsigned char dat)
{
	int i,y;
	for (y = 0;y<7;y++)
	{
		PORTC&=~(1<<PC5);
		SPI_Send(0b01000000+y,(1<<SPE)|(1<<MSTR)|(1<<SPR0));
		SPI_Send(0b10000000,(1<<SPE)|(1<<MSTR)|(1<<SPR0));
		//tai duomenys
		PORTC|=(1<<PC5);
		for ( i = 0;i<84;i++)
		{
			SPI_Send(dat,(1<<SPE)|(1<<MSTR)|(1<<SPR0));
		}
	}

}
//--------------------------------------------------------------------------
void lcd_clear()
{
	lcd_fill(0x00);
}
//--------------------------------------------------------------------------
void lcd_gotoxy(unsigned char x,unsigned char y)
{
	PORTC&=~(1<<PC5);
	SPI_Send(0b01000000+y,(1<<SPE)|(1<<MSTR)|(1<<SPR0));
	SPI_Send(0b10000000+x,(1<<SPE)|(1<<MSTR)|(1<<SPR0));
	PORTC|=(1<<PC5);
}
//--------------------------------------------------------------------------
void write_ram(void)
{
	unsigned char x,y;
	for (y=0;y<6;y++)
	{
		lcd_gotoxy(0,y);
		PORTC|=(1<<PC5);
		for (x=0;x<84;x++);
	//		SPI_MasterTransmit(vaizdas[x][y]);
	}
}
void SPI_SendBuff(unsigned char *buf,int count)
{
	int i;
	for (i=0;i<count;i++)
		SPI_Send(buf[i],(1<<SPE)|(1<<MSTR)|(1<<SPR0));
}
//--------------------------------------------------------------------------
void writestring(int x,int y, char* text)
{
		unsigned char buff[10],n;
		lcd_gotoxy(x,y);
		//tai duomenys
		PORTC|=(1<<PC5);
		for (n=0;(n<13)&&(text[n]!=0);n++)
		{
				
				lcd_ASCII_to_IMG(text[n],buff);
				SPI_SendBuff(buff,6);
		}
}
void putimg(unsigned char x, unsigned char y, unsigned char *img)
{
	unsigned char xx;
	if (x + 6 <=84)
	{
	lcd_gotoxy(x,y/8);
	PORTC|=(1<<PC5);
	for (xx=0;xx<6;xx++)
		SPI_Send(img[xx]<<y%8,(1<<SPE)|(1<<MSTR)|(1<<SPR0));
	if (y%8 > 0)
	{
		lcd_gotoxy(x,y/8+1);
		PORTC|=(1<<PC5);
		for (xx=0;xx<6;xx++)
		SPI_Send(img[xx]>>(8-y%8),(1<<SPE)|(1<<MSTR)|(1<<SPR0));
	}
	}
}
void lcd_writeStringXY (unsigned char x, unsigned char y,  char *tekstas,unsigned char maxlen)
{
	int n=0;
	unsigned char img[6];
	while ((tekstas[n]!=0)&&(n<maxlen))
	{
		lcd_ASCII_to_IMG(tekstas[n],img);
		putimg(x+n*6,y,img);
		n++;
	}
}
//--------------------------------------------------------------------------
void lcd_init()
{
	RSTport&=~(1<<RSTpin);
	_delay_ms(500);
	RSTport|=(1<<RSTpin);
	//tai bus komandos
	DCport&=~(1<<DCpin);
	//function set H=1
	SPI_Send(0b00100001,(1<<SPE)|(1<<MSTR)|(1<<SPR0));
	//set Vop
	SPI_Send(0b10010000,(1<<SPE)|(1<<MSTR)|(1<<SPR0));
	//function set H=0
	SPI_Send(0b00100000,(1<<SPE)|(1<<MSTR)|(1<<SPR0));
	//display control D=1
	SPI_Send(0b00001100,(1<<SPE)|(1<<MSTR)|(1<<SPR0));
}
