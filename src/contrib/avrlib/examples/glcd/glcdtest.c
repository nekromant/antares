//*****************************************************************************
//
// File Name	: 'glcdtest.h'
// Title		: Graphic LCD test and examples code
// Author		: Pascal Stang
// Created		: 7/12/2001
// Revised		: 9/30/2002
// Version		: 1.1
// Target MCU	: Atmel AVR series
// Editor Tabs	: 4
//
//	Description : This code tests and demonstrates the AVRlib graphic LCD
//		driver, glcd.c.
//
// This code is distributed under the GNU Public License
//		which can be found at http://www.gnu.org/licenses/gpl.txt
//
//*****************************************************************************


#include <avr/io.h>

#include "global.h"
#include "uart.h"
#include "rprintf.h"
#include "timer.h"
#include "a2d.h"

#include "ks0108.h"
#include "glcd.h"

#define KEY1	0x01	// Key1
#define KEY2	0x02	// Key2
#define KEY3	0x04	// Key3
#define KEY4	0x08	// Key4
#define KEY5	0x10	// Key5
#define KEY6	0x20	// Key6
#define KEY7	0x40	// Key7
#define KEY8	0x80	// Key8

void lcdtest(void);
void oscope(void);

int main (void)
{
	// initialize
	uartInit();
	timerInit();
	a2dInit();
	glcdInit();
	outb(DDRA, 0x00);
	
	// send rprintf output to serial port
	rprintfInit(uartSendByte);
	// print welcome message to serial port
	rprintfProgStrM("\r\nWelcome to glcdtest...\r\n");
	// send rprintf output to lcd display
	rprintfInit(glcdWriteChar);

	// perform basic functionality tests
	rprintfProgStrM("All initialized...");

	glcdSetAddress(4,LINE2);
	glcdWriteChar('H');
	glcdWriteChar('E');
	glcdWriteChar('L');
	glcdWriteChar('L');
	glcdWriteChar('O');

	glcdSetAddress(4,LINE3);
	rprintfProgStrM("line 3");
	glcdSetAddress(4,LINE4);
	rprintfProgStrM("line 4");
	glcdSetAddress(4,LINE5);
	rprintfProgStrM("line 5");

	// run application program
	//oscope();
	lcdtest();

	return 0;   
}

void oscope(void)
{
	u08 i=0;
	u08 oldbuffer[128];
	u08 newbuffer[128];
	
	glcdClearScreen();

	while(1)
	{
		for(i=0; i<128; i++)
			oldbuffer[i] = newbuffer[i];

		for(i=0; i<128; i++)
			newbuffer[i] = a2dConvert8bit(0);

		for(i=0; i<128; i++)
		{
			glcdClearDot(i,oldbuffer[i]>>2);
			glcdSetDot(i,newbuffer[i]>>2);
		}
	}
}

void lcdtest(void)
{
	unsigned char key = 0;

	glcdClearScreen();

	glcdSetAddress(4,LINE2);
	glcdPutStr("Graphic LCD Test");
	glcdSetAddress(4,LINE3);
	glcdPutStr("HD61202/3 controller");
	glcdSetAddress(4,LINE4);
	glcdPutStr("KS0108/7 controller");
	glcdSetAddress(4,LINE5);
	glcdPutStr("Press buttons to");
	glcdSetAddress(4,LINE6);
	glcdPutStr("test functions...");
	glcdRectangle(0, 0, 64, 128);
  
	while(1)
	{
		timerPause(10);
		key = ~inb(PINA);
		glcdSetAddress(4,LINE7);
		rprintf("Button status: %x  ", key);

		if(key == KEY1)
		{
			unsigned char i ;
			glcdClearScreen();	
			for ( i=0; i<128; i+=3)
			{
				glcdSetDot(i,63 - i/2);
  				glcdDelay(0x5fff);
			}
			for ( i=0; i<128; i+=3)
			{
				glcdClearDot(i,63 - i/2);
  				glcdDelay(0x5fff);
			}
		}
		
		if(key == KEY2)
		{
			glcdClearScreen();	
  			glcdCircle(25,20,17);
  			glcdDelay(0xffff);
  			glcdCircle(90,30,15);
  			glcdDelay(0xffff);
  			glcdCircle(55,30,23);
  			glcdDelay(0xffff);
  			glcdCircle(100,48,15);
  			glcdDelay(0xffff);
  			glcdCircle(34,50,10);
  			glcdDelay(0xffff);
  			glcdCircle(60,55,8);
		}
      
		if(key == KEY3)
		{
			glcdClearScreen();	
			glcdRectangle(54, 41, 6 , 12);
  			glcdDelay(0xffff);
			glcdRectangle(34, 12, 32, 2);
  			glcdDelay(0xffff);
			glcdRectangle(23, 34, 17, 21);
  			glcdDelay(0xffff);
			glcdRectangle(62, 20, 42, 58);
  			glcdDelay(0xffff);
			glcdRectangle(4 , 30, 12, 12);
		}
		
		if(key == KEY4)
		{
			glcdClearScreen();	
			glcdSetAddress(0,LINE1);	
			glcdPutStr("LINE 1"); glcdDelay(0xffff);
			glcdSetAddress(5,LINE2);
			glcdPutStr("LINE 2"); glcdDelay(0xffff);
			glcdSetAddress(10,LINE3);
			glcdPutStr("LINE 3"); glcdDelay(0xffff);
			glcdSetAddress(15,LINE4);
			glcdPutStr("LINE 4"); glcdDelay(0xffff);
			glcdSetAddress(20,LINE5);
			glcdPutStr("LINE 5"); glcdDelay(0xffff);
			glcdSetAddress(25,LINE6);
			glcdPutStr("LINE 6"); glcdDelay(0xffff);
			glcdSetAddress(30,LINE7);
			glcdPutStr("LINE 7"); glcdDelay(0xffff);
			glcdSetAddress(35,LINE8);
			glcdPutStr("LINE 8"); glcdDelay(0xffff);
		}

		if(key == KEY5)
		{
			glcdClearScreen();	
			glcdSetAddress(0,LINE2);
  			glcdPutStr(" !");
  			glcdWriteChar('"');
  			glcdPutStr("#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ");
  			glcdPutStr("[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~");
		}
		
		if(key == KEY6)
		{
			unsigned char i;
			for (i=0; i<64; i++)
			{
				glcdStartLine(64- i - 1);
  				glcdDelay(0x5fff);
			}
		}
		
		if(key == KEY7)
		{
			unsigned char i;
			for (i=0; i<64; i++)
			{
				glcdStartLine(i + 1);
  				glcdDelay(0x5fff);
			}
		}
		
		if (key == KEY8)
		{
			//	glcdBackLight(OFF);	
			//	glcdBackLight(ON);	
		}   
	}
}
