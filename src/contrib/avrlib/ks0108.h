/*! \file ks0108.h \brief Graphic LCD driver for HD61202/KS0108 displays. */
//*****************************************************************************
//
// File Name	: 'ks0108.h'
// Title		: Graphic LCD driver for HD61202/KS0108 displays
// Author		: Pascal Stang - Copyright (C) 2001-2003
// Date			: 10/19/2002
// Revised		: 5/1/2003
// Version		: 0.5
// Target MCU	: Atmel AVR
// Editor Tabs	: 4
//
// NOTE: This code is currently below version 1.0, and therefore is considered
// to be lacking in some functionality or documentation, or may not be fully
// tested.  Nonetheless, you can expect most functions to work.
//
///	\ingroup driver_hw
/// \defgroup ks0108 Graphic LCD Driver for HD61202/KS0108-based Displays (ks0108.c)
/// \code #include "ks0108.h" \endcode
/// \par Overview
///		This display driver performs the basic functions necessary to access
/// any graphic LCD based on the KS0108 or HD61202 controller chip.  For more
/// advanced functions, use this driver in conjunction with glcd.c.
/// KS0108/HD61202 displays typically range in size from 64x32 pixels to
/// 128x128 pixels and up to 3" square.  To determine whether a display is
/// compatible, you should look for the above controller chips to be mounted
/// on the PC board attached to the display glass.  The controller chips are
/// about 1/2" x 3/4" and have 80+ pins. On larger displays, you may also see
/// slave LCD driver chips with the numbers KS0107 or HD61203.  The display
/// will likely have an 18 or 20-pin interface.  The interface from the LCD
/// to an AVR processor does not require any additional hardware.  If you can
/// locate a datasheet for your display, that plus the information in the
/// ks0108conf.h file should be all you need to get hooked up.
//
// This code is distributed under the GNU Public License
//		which can be found at http://www.gnu.org/licenses/gpl.txt
//
//*****************************************************************************


#ifndef KS0108_H
#define KS0108_H

#include "global.h"

#include "ks0108conf.h"

// HD61202/KS0108 command set
#define GLCD_ON_CTRL		0x3E	// 0011111X: lcd on/off control
#define GLCD_ON_DISPLAY		0x01	//		DB0: turn display on

#define GLCD_START_LINE		0xC0	// 11XXXXXX: set lcd start line

#define GLCD_SET_PAGE		0xB8	// 10111XXX: set lcd page (X) address
#define GLCD_SET_Y_ADDR		0x40	// 01YYYYYY: set lcd Y address

#define GLCD_STATUS_BUSY	0x80	// (1)->LCD IS BUSY
#define GLCD_STATUS_ONOFF	0x20	// (0)->LCD IS ON
#define GLCD_STATUS_RESET	0x10	// (1)->LCD IS RESET

// determine the number of controllers
// (make sure we round up for partial use of more than one controller)
#define GLCD_NUM_CONTROLLERS	((GLCD_XPIXELS+GLCD_CONTROLLER_XPIXELS-1)/GLCD_CONTROLLER_XPIXELS)

// typedefs/structures
typedef struct struct_GrLcdCtrlrStateType
{
	unsigned char xAddr;
	unsigned char yAddr;
} GrLcdCtrlrStateType;

typedef struct struct_GrLcdStateType
{
	unsigned char lcdXAddr;
	unsigned char lcdYAddr;
	GrLcdCtrlrStateType ctrlr[GLCD_NUM_CONTROLLERS];
} GrLcdStateType;

// function prototypes
void glcdInitHW(void);
void glcdBusyWait(u08 controller);
void glcdControlWrite(u08 controller, u08 data);
u08  glcdControlRead(u08 controller);
void glcdDataWrite(u08 data);
u08  glcdDataRead(void);
void glcdSetXAddress(u08 xAddr);
void glcdSetYAddress(u08 yAddr);


//! Initialize the display, clear it, and prepare it for access
void glcdInit(void);
//! Clear the display
void glcdClearScreen(void);
//! Set display memory access point back to upper,left corner
void glcdHome(void);
//! Set display memory access point to row [line] and column [col] assuming 5x7 font
void glcdGotoChar(u08 line, u08 col);
//! Set display memory access point to [x] horizontal pixel and [y] vertical line
void glcdSetAddress(u08 x, u08 yLine);
//! Set display memory access point to row [line] and column [col] assuming 5x7 font
void glcdStartLine(u08 start);
//! Generic delay routine for timed glcd access
void glcdDelay(u16 p);
#endif
