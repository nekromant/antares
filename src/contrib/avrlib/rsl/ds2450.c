//*****************************************************************************
// File Name	: ds2450.c
// Title		: Dallas 1-Wire DS2450 A2D Sensor Library
// Revision		: 5
// Notes		: 
// Target MCU	: Atmel AVR series
// Editor Tabs	: 4
// 
//*****************************************************************************

//----- Include Files ---------------------------------------------------------
#include <string.h>			// include string support
#include <ctype.h>
#include "timer128.h"
#include "dallas.h"			// include dallas support
#include "ds2450.h"			// include ds2450 support
#include "rprintf.h"

//----- Functions ---------------------------------------------------------------

/*--------------------------------------------------------------------------
 * ds2450Chan2Addr: converts the channel to the address in RAM
 * input........... channel - the channel to get the address for [A-D]
 *                  page - the page in RAM that we are dealing with
 *                  address - where the address is stored
 * returns......... the corresponding error or DALLAS_NO_ERROR
 *-------------------------------------------------------------------------*/
static u08 ds2450Chan2Addr(u08 channel, u08 page, u16 *address);

void ds2450Init(void)
{
	// initialize the dallas 1-wire
	dallasInit();
}

u08 *capitalize (u08 *input)
{
	if(islower(*input))
		*input = toupper(*input);
	return input;
}

u08 ds2450Setup(dallas_rom_id_T* rom_id, u08 channel, u08 resolution, u08 range)
{
	u08 error;
	u08 data[2];
	u16 address;

	// check resolution
	if (resolution > DS2450_RES_MAX)
		return DALLAS_RESOLUTION_ERROR;

	// check address
	error = dallasAddressCheck(rom_id, DS2450_FAMILY);
	if (error != DALLAS_NO_ERROR)
		return error;

	// get address
	capitalize(&channel);
	error = ds2450Chan2Addr(channel, DS2450_SETUP_PAGE, &address); 	//find starting address
	if (error != DALLAS_NO_ERROR)
		return error;

	// convert to valid resolution - 16 bits = 0x00
	if (resolution == 16)
		resolution = 0x00;

	// read in current digital output settings
	error = dallasReadRAM(rom_id, address, 1, data);
	if (error != DALLAS_NO_ERROR)
		return error;

	// maintain digital output portion and add new resolution
	data[0] = (data[0] & 0xF0) | resolution;
	
	// maintain alarm states and add new range
	data[1] = (data[1] & 0xFE) | range;

	// actually write config, handles CRC too
	error = dallasWriteRAM(rom_id, address, 2, data);
	if (error != DALLAS_NO_ERROR)
		return error;

	// Normally, the DS2450 is designed to run off of parasite power from the data line
	// Typically the master (us) strongly pulls high long enough to power the conversion, so
	// there is inherintly a long () delay introduced. Since the A2D code is designed to
	// work for devices that use external power, we can elliminate this delay by writting
	// the following byte per the DS2450 datasheet.
	data[0] = DS2450_VCC_FLAG;
	error = dallasWriteRAM(rom_id, DS2450_VCC_ADDR, 1, &data[0]);
	if (error != DALLAS_NO_ERROR)
		return error;

	// verify the data
	error = dallasReadRAM(rom_id, address, 2, data);
	if (error != DALLAS_NO_ERROR)
		return error;

	if ((data[0] & 0x0F) != resolution)
		return DALLAS_VERIFY_ERROR;
	if ((data[1] & 0x01) != range)
		return DALLAS_VERIFY_ERROR;

	return DALLAS_NO_ERROR;
}

u08 ds2450Start(dallas_rom_id_T* rom_id, u08 channel)
{
	u08 mask;
	u08 error;
	u08 crc[2];

	// check address
	error = dallasAddressCheck(rom_id, DS2450_FAMILY);
	if (error != DALLAS_NO_ERROR)
		return error;

	// make sure the channel is a capital letter
	capitalize(&channel);
	// convert to integer 0 to 3
	channel -= 'A';

	// make sure channel is a valid value
	if (channel > 3)
		return DALLAS_INVALID_CHANNEL;

	// shift over to construct input select mask
	mask = 0x01 << channel;

	// reset and select node
	error = dallasMatchROM(rom_id);
	if (error != DALLAS_NO_ERROR)
		return error;

	// send convert command
	dallasWriteByte(DS2450_CONVERT);
	// input select mask
	dallasWriteByte(mask);
	// shift over some more for "read-out" control
	mask = mask << channel;
	
	// set coresponding output buffer to zero	
	dallasWriteByte(mask);

	// we must read 2byte CRC16 to start the conversion:
	crc[0] = dallasReadByte();
	crc[1] = dallasReadByte();

	//replace with explicit CRC posibilities lookup table
//	if (crc[0] == 0xFF && crc[1] == 0xFF)
//		return DALLAS_DEVICE_ERROR;        //if CRC = all one's, no one is paying attention

	return DALLAS_NO_ERROR;
}

u08 ds2450Result(dallas_rom_id_T* rom_id, u08 channel, u16* result)
{
	u08 data[2];
	u08 error;
	u16 address;
	//u08 resolution;

	// check address
	error = dallasAddressCheck(rom_id, DS2450_FAMILY);
	if( error != DALLAS_NO_ERROR)
		return error;

	// get the RAM address for the data for the channel
	capitalize(&channel);
	error = ds2450Chan2Addr(channel, DS2450_DATA_PAGE, &address);
	if (error != DALLAS_NO_ERROR)
		return error;

	// read the RAM from the device to get the data
	error = dallasReadRAM(rom_id, address, 2, data);
	/*
	// get the address for the setup for the channel
	error = ds2450Chan2Addr(channel, DS2450_SETUP_PAGE, &address); 	//find starting address
	if (error != DALLAS_NO_ERROR)
		return error;
	
	// read the RAM from the device to get the resolution
	error = dallasReadRAM(rom_id, address, 1, &resolution);
	if (error != DALLAS_NO_ERROR)
		return error;

	// get the resultion part of the data
	resolution &=0x0F;
	*/
	// store the result by combining the 2 bytes
	// the result's MSB is always the same, so we may need to
	// shift the data over so that the LSB is at the first bit
	*result = 0;
	//*result = (((u16)data[1] << 8) | data[0]) >> (16 - resolution);
	*result = (((u16)data[1] << 8) | data[0]);

	return DALLAS_NO_ERROR;
}

u08 ds2450StartAndResult(dallas_rom_id_T* rom_id, u08 channel, u16 *result)
{
	u08 error;

	// start conversion
	error = ds2450Start(rom_id, channel);
	if (error != DALLAS_NO_ERROR)
		return error;
	
	// wait till conversion done
	dallasWaitUntilDone();
	
	// return results
	return ds2450Result(rom_id, channel, result);
}

u08 ds2450SetupAll(dallas_rom_id_T* rom_id, u08 resolution, u08 range)
{
	u08 i;
	u08 error;
	u08 data[8];
	u16 address;

	// check address
	error = dallasAddressCheck(rom_id, DS2450_FAMILY);
	if (error != DALLAS_NO_ERROR)
		return error;

	// check resolution
	if (resolution > DS2450_RES_MAX)
		return DALLAS_RESOLUTION_ERROR;

	// convert to valid resolution - 16 bits = 0x00
	if (resolution == 16)
		resolution = 0;

	// get address - start with channel A
	error = ds2450Chan2Addr('A', DS2450_SETUP_PAGE, &address);
	if (error != DALLAS_NO_ERROR)
		return error;

	// read in current settings so we can extract digital part
	error = dallasReadRAM(rom_id, address, 8, data);
	if (error != DALLAS_NO_ERROR)
		return error;

	// build up config data to write - increment by 2 b/c two bytes per channel
	for(i=0;i<8;i+= 2)
	{ 
		// maintain digital output settings
		data[i] &= 0xF0;	// extract digital output portion
		data[i+1] &= 0xFE;

		// write resolution byte and range
		data[i] |= resolution;
		data[i+1] |= range;
	}

	// actually write config - handles CRC too
	error = dallasWriteRAM(rom_id, address, 8, data);
	if (error != DALLAS_NO_ERROR)
		return error;

	// Normally, the DS2450 is designed to run off of parasite power from the data line
	// Typically the master (us) strongly pulls high long enough to power the conversion, so
	// there is inherintly a long () delay introduced. Since the A2D code is designed to
	// work for devices that use external power, we can elliminate this delay by writting
	// the following byte per the DS2450 datasheet.
	data[0] = DS2450_VCC_FLAG;
	error = dallasWriteRAM(rom_id, DS2450_VCC_ADDR, 1, &data[0]);
	if (error != DALLAS_NO_ERROR)
		return error;

	error = dallasReadRAM(rom_id,address,8,data);
	if (error != DALLAS_NO_ERROR)
		return error;

	for(i=0;i<8;i+=2)
	{
		if ((data[i] & 0x0F) != resolution)
			return DALLAS_VERIFY_ERROR;
		if ((data[i+1] & 0x01) != range)
			return DALLAS_VERIFY_ERROR;
	}

	return DALLAS_NO_ERROR;
}

u08 ds2450StartAll(dallas_rom_id_T* rom_id)
{
	u08 error;
	u16 crc;

	// check address
	error = dallasAddressCheck(rom_id, DS2450_FAMILY);
	if (error != DALLAS_NO_ERROR)
		return error;

	// reset and select node
	error = dallasMatchROM(rom_id);
	if (error != DALLAS_NO_ERROR)
		return error;

	dallasWriteByte(DS2450_CONVERT);			// send convert command
	dallasWriteByte(DS2450_CONVERT_ALL4_MASK);  	// select all 4 inputs
	dallasWriteByte(DS2450_CLEAR_ALL4_MASK);		// set all output buffers to zero

	// we must read 2byte CRC16 to start the conversion:
	crc = dallasReadByte() | ((u16)dallasReadByte() << 8);

	// replace with explicit CRC posibilities lookup table
//	if (crc == 0xFFFF)
//		return DALLAS_DEVICE_ERROR;        // if CRC = all one's, no one is paying attention
	
	return DALLAS_NO_ERROR;
}

u08 ds2450ResultAll(dallas_rom_id_T* rom_id, u16 result[4])
{
	//const u08 bytes_to_read = 10;		// read 10bytes = 2/ch*4ch + CRC
	u08 bytes_to_read = 10;
	u08 i;
	u08 error;
	u08 data[10];
	u08 resolution[10];
	u16 address;

	// check address
	
	error = dallasAddressCheck(rom_id, DS2450_FAMILY);
	if (error != DALLAS_NO_ERROR)
		return error;

	// start address with channel A
	error = ds2450Chan2Addr('A', DS2450_DATA_PAGE, &address);
	if (error != DALLAS_NO_ERROR)
		return error;

	// read the conversion data
	error = dallasReadRAM(rom_id, address, bytes_to_read, data);
	if (error != DALLAS_NO_ERROR)
		return error;

	//FUTURE: do a real CRC16 check

	// start address with channel A
	error = ds2450Chan2Addr('A', DS2450_SETUP_PAGE, &address);
	if (error != DALLAS_NO_ERROR)
		return error;

	// read the resolution data
	error = dallasReadRAM(rom_id, address, bytes_to_read, resolution);
	if (error != DALLAS_NO_ERROR)
		return error;

	// check crc?

	// store the result by combining the 2 bytes
	// the result's MSB is always the same, so we may need to
	// shift the data over so that the LSB is at the first bit
	error=0;
	for(i=0;i<8;i+=2)
	{
		resolution[i] &= 0x0F;
		if (!resolution[i])
			resolution[i] = 16;

		result[error] = 0;
		//result[error] = (((u16)data[i+1] << 8) | data[i]) >> (16 - resolution[i]);
		result[error] = (((u16)data[i+1] << 8) | data[i]);
		error++;
	}

	return DALLAS_NO_ERROR;
}

u08 ds2450StartAndResultAll(dallas_rom_id_T* rom_id, u16 result[4])
{
	u08 error;

	// start Conversion
	error = ds2450StartAll(rom_id);
	if (error != DALLAS_NO_ERROR)
		return error;
	
	// wait until conversion done
	dallasWaitUntilDone();
	
	// return any error - results passed by reference
	return ds2450ResultAll(rom_id, result);
}

void ds2450Print(u16 result, u08 range)
{
	u16 vscale;

	rprintfProgStrM(" 0x");
	rprintfu16(result);
	rprintf("  ");
	if(range)
		vscale = 12800;
	else
		vscale = 25600;

	rprintfNum(10, 4, TRUE , ' ', result/vscale);
	rprintf(".");
	rprintfNum(10, 4, FALSE, '0', (((u32)(result%vscale))*10000)/vscale );
	rprintfProgStrM(" Volts");
}

u08 ds2450DigitalOut(dallas_rom_id_T* rom_id, u08 channel, dallas_a2d_out_T state)
{
	u08 error;
	u08 old_resolution;
	u16 address;

	// check address
	error = dallasAddressCheck(rom_id, DS2450_FAMILY);
	if (error != DALLAS_NO_ERROR)
		return error;

	// get the address for the channel in the setup page
	error = ds2450Chan2Addr(channel, DS2450_SETUP_PAGE, &address);
	if (error != DALLAS_NO_ERROR)
		return error;

	// read in current resolution
	error = dallasReadRAM(rom_id, address, 1, &old_resolution);
	if (error != DALLAS_NO_ERROR)
		return error;
	
	// extract resolution portion
	old_resolution &= 0x0F;

	// write new setup byte
	state |= old_resolution;
	error = dallasWriteRAM(rom_id, address, 1, ((u08*)&state));
	if (error != DALLAS_NO_ERROR)
		return error;

	return DALLAS_NO_ERROR;
}

static u08 ds2450Chan2Addr(u08 channel, u08 page, u16 *address)
{
	// make sure the channel is a capital letter
	capitalize(&channel);

	//convert to integer 0 to 3 and check to see if it is valid
	channel -= 'A';
	if (channel > 3)
		return DALLAS_INVALID_CHANNEL;

	// use corresponding memory address
	*address = (channel<<1) + page;			// channel<<1 == channel*2, but faster

	return DALLAS_NO_ERROR;
}
