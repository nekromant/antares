//*****************************************************************************
// File Name	: ds18b20.c
// Title		: Dallas 1-Wire DS18B20 Temperature Sensor Library
// Revision		: 3
// Notes		: 
// Target MCU	: Atmel AVR series
// Editor Tabs	: 4
//
//*****************************************************************************
 
//----- Include Files ---------------------------------------------------------
#include "rprintf.h"		// include rprintf function library
#include "dallas.h"			// include dallas support
#include "ds18b20.h"		// include ds18b20 support

void ds18b20Init(void)
{
	// initialize the 1-wire
	dallasInit();
}

u08 ds18b20Setup(dallas_rom_id_T* rom_id, u08 resolution, s08 alarm_low, s08 alarm_high)
{
	u08 error;

	// check resolution
	if ((resolution < DS18B20_RES_MIN) || (resolution > DS18B20_RES_MAX))
		return DALLAS_RESOLUTION_ERROR;

	// check address
	error = dallasAddressCheck(rom_id, DS18B20_FAMILY);
	if (error != DALLAS_NO_ERROR)
		return error;

	// reset and select node
	error = dallasMatchROM(rom_id);
	if (error != DALLAS_NO_ERROR)
		return error;

	// starts writting at address 0x02, T_H
	dallasWriteByte(DS18B20_WRITE_SCRATCHPAD);
	dallasWriteByte(alarm_high);
	dallasWriteByte(alarm_low);

	// convert resolution to bitmask
	// valid value are 9-12 encoded as 0-4, resolution stored in bits 5&6 and bits 0-4 are always one
	resolution = ((resolution - 9) << 5) | 0x60;
	dallasWriteByte(resolution);

	// reset and select node
	error = dallasMatchROM(rom_id);
	if (error != DALLAS_NO_ERROR)
		return error;
	
	// start reading at address 0x00
	dallasWriteByte(DS18B20_READ_SCRATCHPAD);
	
	dallasReadByte();							// 0x00, temp lsb
	dallasReadByte();							// 0x01, temp msb

	// verify the data
	if ((s08)dallasReadByte() != alarm_high)	// 0x02, alarm high
		return DALLAS_VERIFY_ERROR;
	if ((s08)dallasReadByte() != alarm_low)		// 0x03, alarm low
		return DALLAS_VERIFY_ERROR;
	if (dallasReadByte() != resolution)			// 0x04, resolution
		return DALLAS_VERIFY_ERROR;

	return DALLAS_NO_ERROR;
}

u08 ds18b20Start(dallas_rom_id_T* rom_id)
{
	u08 error;

	// check address
	error = dallasAddressCheck(rom_id, DS18B20_FAMILY);
	if (error != DALLAS_NO_ERROR)
		return error;

	// reset and select node
	error = dallasMatchROM(rom_id);
	if (error != DALLAS_NO_ERROR)
		return error;
	
	// send convert command
	dallasWriteByte(DS18B20_CONVERT_TEMP);
	
	return DALLAS_NO_ERROR;
}

/*------ DallasTempGetResult ------*/
u08 ds18b20Result(dallas_rom_id_T* rom_id, u16 *result)
{
	u08 error;

	union int16_var_U
	{
		u16 i16;
		u08 i08[2];
	} int16_var;

	// check address
	error = dallasAddressCheck(rom_id, DS18B20_FAMILY);
	if (error != DALLAS_NO_ERROR)
		return error;

	// reset and select node
	error = dallasMatchROM(rom_id);
	if (error != DALLAS_NO_ERROR)
		return error;
	
	// send command
	dallasWriteByte(DS18B20_READ_SCRATCHPAD);
	
	// read results 1 byte at a time
	int16_var.i08[0] = dallasReadByte();
	int16_var.i08[1] = dallasReadByte();
	*result = int16_var.i16;

	return DALLAS_NO_ERROR;
}

u08 ds18b20StartAndResult(dallas_rom_id_T* rom_id, u16 *result)
{
	u08 error;

	// start
	error = ds18b20Start(rom_id);
	if(error != DALLAS_NO_ERROR)
		return error;

	// wait
	dallasWaitUntilDone();

	// return any errors - results passed by reference
	return ds18b20Result(rom_id,result);	
}

void ds18b20Print(u16 result, u08 resolution)
{
	// print raw value
	rprintfProgStrM(" 0x");
	rprintfu16(result);
	rprintfChar(' ');

	// print real temp
	rprintfNum(10, 4, TRUE , ' ', result>>4);
	rprintf(".");
	rprintfNum(10, 4, FALSE, '0', (10000*((u32)(result&0x000F)))/16 );
	rprintfProgStrM(" C");
}

/* OLD VERSION

void ds18b20Print(u16 result, u08 resolution)
{
	u08 numerator;

	// print header
	rprintfProgStrM(" 0x");
	rprintfu16(result);
	rprintfChar(' ');

	// extract fractional part
	numerator = result & 0x000F;

	// extract integer part
	result >>= 4;

	// deal with negative numbers
	if ((s08)result < 0)
	{
		// for negative fractions
		if (numerator)
		{
			numerator = 16-numerator;	// fractions are reversed		
			++result;					// the integer is one more than 2's complement
			if (!(s08)result)			// between -1 and 0
				rprintfChar('-');		// we have to take care of the negative sign
		}
	}

	// print integer part (signed)
	rprintfNum(10,4,1,'0',(s08)result);
	rprintfChar(' ');

	//Print fractional part. use numerator for numerator, resolution for denomiator
	// bit resolution    setup mask     degree resolution
	// --------------    ----------     -----------------
	//       9              0x00              1/2
	//      10              0x20              1/4
	//      11              0x40              1/8
	//      12              0x60              1/16
	numerator >>= (12-resolution);     // scale fractional part of result to resolution
	resolution = 2 << (resolution - 9);	// create denominator as 2^(res-9)
	rprintfNum(10,3,0,0,numerator);
	rprintfChar('/');
	rprintfNum(10,3,0,0,resolution);
}
*/
