//*****************************************************************************
// File Name	: dallas.c
// Title		: Dallas 1-Wire Library
// Revision		: 6
// Notes		: 
// Target MCU	: Atmel AVR series
// Editor Tabs	: 4
// 
//*****************************************************************************
 
//----- Include Files ---------------------------------------------------------
#include <avr/io.h>				// include I/O definitions (port names, pin names, etc)
#include <avr/interrupt.h>		// include interrupt support
#include <string.h>				// include string support
#include "rprintf.h"			// include rprintf function library
#include "timer128.h"			// include timer function library
#include "dallas.h"				// include dallas support

//----- Global Variables -------------------------------------------------------
static u08 last_discrep = 0;	// last discrepancy for FindDevices
static u08 done_flag = 0;		// done flag for FindDevices

u08 dallas_crc;					// current crc global variable
u08 dallas_crc_table[] =		// dallas crc lookup table
{
	0, 94,188,226, 97, 63,221,131,194,156,126, 32,163,253, 31, 65,
	157,195, 33,127,252,162, 64, 30, 95, 1,227,189, 62, 96,130,220,
	35,125,159,193, 66, 28,254,160,225,191, 93, 3,128,222, 60, 98,
	190,224, 2, 92,223,129, 99, 61,124, 34,192,158, 29, 67,161,255,
	70, 24,250,164, 39,121,155,197,132,218, 56,102,229,187, 89, 7,
	219,133,103, 57,186,228, 6, 88, 25, 71,165,251,120, 38,196,154,
	101, 59,217,135, 4, 90,184,230,167,249, 27, 69,198,152,122, 36,
	248,166, 68, 26,153,199, 37,123, 58,100,134,216, 91, 5,231,185,
	140,210, 48,110,237,179, 81, 15, 78, 16,242,172, 47,113,147,205,
	17, 79,173,243,112, 46,204,146,211,141,111, 49,178,236, 14, 80,
	175,241, 19, 77,206,144,114, 44,109, 51,209,143, 12, 82,176,238,
	50,108,142,208, 83, 13,239,177,240,174, 76, 18,145,207, 45,115,
	202,148,118, 40,171,245, 23, 73, 8, 86,180,234,105, 55,213,139,
	87, 9,235,181, 54,104,138,212,149,203, 41,119,244,170, 72, 22,
	233,183, 85, 11,136,214, 52,106, 43,117,151,201, 74, 20,246,168,
	116, 42,200,150, 21, 75,169,247,182,232, 10, 84,215,137,107, 53
};

//----- Functions --------------------------------------------------------------

/*--------------------------------------------------------------------------
 * dallasFindNextDevice: find the next device on the bus
 * input................ rom_id - pointer to store the rom id found
 * returns.............. true or false if a device was found
 *-------------------------------------------------------------------------*/
u08 dallasFindNextDevice(dallas_rom_id_T* rom_id);

void dallasDelayUs(u16 us)
{
	while(us--) // 6 cycles
	{
		asm volatile ("nop");
		asm volatile ("nop");
		asm volatile ("nop");
		asm volatile ("nop");
		asm volatile ("nop");
		asm volatile ("nop");
		asm volatile ("nop");
	} // loop jump 2 cycles
}

void dallasInit(void)
{
}

u08 dallasReset(void)
{
	u08 presence = DALLAS_PRESENCE;

	cli();
	
	// pull line low
	sbi(DALLAS_DDR, DALLAS_PIN);
	cbi(DALLAS_PORT, DALLAS_PIN);
	
	// wait for presence
	dallasDelayUs(480);
	
	// allow line to return high
	cbi(DALLAS_DDR, DALLAS_PIN);
	sbi(DALLAS_PORT, DALLAS_PIN);
	
	// wait for presence
	dallasDelayUs(70);

	// if device is not present, pin will be 1
	if (inb(DALLAS_PORTIN) & 0x01<<DALLAS_PIN)
		presence = DALLAS_NO_PRESENCE;

	// wait for end of timeslot
	dallasDelayUs(410);

	sei();

	// now that we have reset, let's check bus health
	// it should be noted that a delay may be needed here for devices that
	// send out an alarming presence pulse signal after a reset
	cbi(DALLAS_DDR, DALLAS_PIN);
	sbi(DALLAS_PORT, DALLAS_PIN);
	//dallasDelayUs(200);
	if (!(inb(DALLAS_PORTIN) & (0x01<<DALLAS_PIN)))	// it should be pulled up to high
		return DALLAS_BUS_ERROR;

	return presence;
}

u08 dallasReadBit(void)
{
	u08 bit = 0;
	
	// pull line low to start timeslot
	sbi(DALLAS_DDR, DALLAS_PIN);
	cbi(DALLAS_PORT, DALLAS_PIN);
	
	// delay appropriate time
	dallasDelayUs(6);

	// release the bus
	cbi(DALLAS_DDR, DALLAS_PIN);
	sbi(DALLAS_PORT, DALLAS_PIN);
	
	// delay appropriate time	
	dallasDelayUs(9);

	// read the pin and set the variable to 1 if the pin is high
	if (inb(DALLAS_PORTIN) & 0x01<<DALLAS_PIN)
		bit = 1;
	
	// finish read timeslot
	dallasDelayUs(55);
	
	return bit;
}

void dallasWriteBit(u08 bit)
{
	// drive bus low
	sbi(DALLAS_DDR, DALLAS_PIN);
	cbi(DALLAS_PORT, DALLAS_PIN);
	
	// delay the proper time if we want to write a 0 or 1
	if (bit)
		dallasDelayUs(6);
	else
		dallasDelayUs(60);

	// release bus
	cbi(DALLAS_DDR, DALLAS_PIN);
	sbi(DALLAS_PORT, DALLAS_PIN);

	// delay the proper time if we want to write a 0 or 1
	if (bit)
		dallasDelayUs(64);
	else
		dallasDelayUs(10);
}

u08 dallasReadByte(void)
{
	u08 i;
	u08 byte = 0;

	cli();
	
	// read all 8 bits
	for(i=0;i<8;i++)
	{
		if (dallasReadBit())
			byte |= 0x01<<i;

		// allow a us delay between each read
		dallasDelayUs(1);
	}

	sei();

	return byte;
}

void dallasWriteByte(u08 byte)
{
	u08 i;

	cli();

	// write all 8 bits
	for(i=0;i<8;i++)
	{
		dallasWriteBit((byte>>i) & 0x01);
		
		// allow a us delay between each write
		dallasDelayUs(1);
	}
	
	sei();
}

u08 dallasReadRAM(dallas_rom_id_T* rom_id, u16 addr, u08 len, u08 *data)
{
	u08 i;
	u08 error;

	union int16_var_U
	{
		u16 i16;
		u08 i08[2];
	} int16_var;

	// first make sure we actually have something to do
	if (data == NULL)
		return DALLAS_NULL_POINTER;
	if (len == 0)
		return DALLAS_ZERO_LEN;

	// reset the bus and request the device
	error = dallasMatchROM(rom_id);
	if (error != DALLAS_NO_ERROR)
		return error;
	
	// enter read mode
	dallasWriteByte(DALLAS_READ_MEMORY);
	
	// write address one byte at a time
	int16_var.i16 = addr;
	dallasWriteByte(int16_var.i08[0]);
	dallasWriteByte(int16_var.i08[1]);
	
	// read data from device 1 byte at a time
	for(i=0;i<len;i++)
		data[i] = dallasReadByte();

	return DALLAS_NO_ERROR;
}

u08 dallasWriteRAM(dallas_rom_id_T* rom_id, u16 addr, u08 len, u08* data)
{
	u08 i;
	u08 error;

	union int16_var_U
	{
		u16 i16;
		u08 i08[2];
	} int16_var;

	// first make sure we actually have something to do
	if (data == NULL)
		return DALLAS_NULL_POINTER;
	if (len == 0)
		return DALLAS_ZERO_LEN;

	// reset the bus and request the device
	error = dallasMatchROM(rom_id);
	if (error != DALLAS_NO_ERROR)
		return error;
	
	// enter write mode
	dallasWriteByte(DALLAS_WRITE_MEMORY);

	// write address one byte at a time
	int16_var.i16 = addr;
	dallasWriteByte(int16_var.i08[0]);
	dallasWriteByte(int16_var.i08[1]);
	
	// write data one byte at a time
	for(i=0;i<len;i++)
	{
		dallasWriteByte(data[i]);
		
		// future: Check CRC16, for now just read it so we can go on
		dallasReadByte();
		dallasReadByte();

		// verify the data
		if (dallasReadByte() != data[i])
			return DALLAS_VERIFY_ERROR;
	}

	return DALLAS_NO_ERROR;
}

void dallasWaitUntilDone(void)
{
	//timerPause(6);
	
	// wait until we recieve a one
	cli();
	while(!dallasReadBit());
	sei();
}

u08 dallasReadROM(dallas_rom_id_T* rom_id)
{
	u08 i;

	// reset the 1-wire bus and look for presence
	i = dallasReset();
	if (i != DALLAS_PRESENCE)
		return i;
	
	// send READ ROM command
	dallasWriteByte(DALLAS_READ_ROM);

	// get the device's ID 1 byte at a time
	for(i=0;i<8;i++)
		rom_id->byte[i] = dallasReadByte();

	return DALLAS_NO_ERROR;
}

u08 dallasMatchROM(dallas_rom_id_T* rom_id)
{
	u08 i;

	// reset the 1-wire and look for presence
	i = dallasReset();
	if (i != DALLAS_PRESENCE)
		return i;

	// send MATCH ROM command
	dallasWriteByte(DALLAS_MATCH_ROM);

	// write id one byte at a time
	for(i=0;i<8;i++)
		dallasWriteByte(rom_id->byte[i]);

	return DALLAS_NO_ERROR;
}

void dallasPrintROM(dallas_rom_id_T* rom_id)
{
	s08 i;

	// print out the rom in the format: xx xx xx xx xx xx xx xx
	for(i=7;i>=0;i--)
	{
		rprintfu08(rom_id->byte[i]);
		rprintfChar(' ');
	}

	// print out the rom in the format: 0xXXXXXXXXXXXXXXXX
	rprintfProgStrM(" (0x");
	for(i=7;i>=0;i--)
	{
		rprintfu08(rom_id->byte[i]);
	}
	rprintfProgStrM("ULL)");

}

u08 dallasAddressCheck(dallas_rom_id_T* rom_id, u08 family)
{
//	u08 i;

//	dallas_crc = 0;
	
//	for(i=1;i<7;i++)
//	{
//		dallasCRC(rom_id->byte[i]);
//		rprintfu08(rom_id->byte[i]);
//		rprintfChar(' ');
//	}
//	rprintfCRLF();

//	rprintfu08(dallas_crc);
//	rprintfCRLF();
	
	//run CRC on address

	//make sure we have the correct family
	if (rom_id->byte[DALLAS_FAMILY_IDX] == family)
		return DALLAS_NO_ERROR;
	
	return DALLAS_ADDRESS_ERROR;
}

u08 dallasCRC(u08 i)
{
	// update the crc global variable and return it
	dallas_crc = dallas_crc_table[dallas_crc^i];
	return dallas_crc;
}

u08 dallasFindDevices(dallas_rom_id_T rom_id[])
{
	u08 num_found = 0;
	dallas_rom_id_T id;

	// reset the rom search last discrepancy global
	last_discrep = 0;
	done_flag = FALSE;

	// check to make sure presence is detected before we start
	if (dallasReset() == DALLAS_PRESENCE)
	{
		// --> stang
		//while (dallasFindNextDevice(&rom_id[num_found]) && (num_found<DALLAS_MAX_DEVICES))
		//	num_found++;
		
		// continues until no additional devices are found
		while (dallasFindNextDevice(&id) && (num_found<DALLAS_MAX_DEVICES))
			memcpy(&rom_id[num_found++], &id, 8);
	}

	return num_found;
}

u08 dallasFindNextDevice(dallas_rom_id_T* rom_id)
{
	u08 bit;
	u08 i = 0;
	u08 bit_index = 1;
	u08 byte_index = 0;
	u08 bit_mask = 1;
	u08 discrep_marker = 0;
	
	// reset the CRC
	dallas_crc = 0;

	if (done_flag || dallasReset() != DALLAS_PRESENCE)
	{
		// no more devices parts detected
		return FALSE;
	}

	// send search ROM command
	dallasWriteByte(DALLAS_SEARCH_ROM);
	
	// loop until through all 8 ROM bytes
	while(byte_index<8)
	{
		// read line 2 times to determine status of devices
		//    00 - devices connected to bus with conflicting bits
		//    01 - all devices have a 0 in this position
		//    10 - all devices ahve a 1 in this position
		//    11 - there are no devices connected to bus
		i = 0;
		cli();
		if (dallasReadBit())
			i = 2;				// store the msb if 1
		dallasDelayUs(120);
		if (dallasReadBit())
			i |= 1;				// store the lsb if 1
		sei();
		
		if (i==3)
		{
			// there are no devices on the 1-wire
			break;
		}
		else
		{
			if (i>0)
			{
				// all devices coupled have 0 or 1
				// shift 1 to determine if the msb is 0 or 1
				bit = i>>1;
			}
			else
			{
				// if this discrepancy is before the last discrepancy on a
				// previous FindNextDevice then pick the same as last time
				if (bit_index<last_discrep)
					bit = ((rom_id->byte[byte_index] & bit_mask) > 0);
				else
					bit = (bit_index==last_discrep);
				
				// if 0 was picked then record position with bit mask
				if (!bit)
					discrep_marker = bit_index;
			}

			// isolate bit in rom_id->byte[byte_index] with bit mask
			if (bit)
				rom_id->byte[byte_index] |= bit_mask;
			else
				rom_id->byte[byte_index] &= ~bit_mask;

			// ROM search write
			cli();
			dallasWriteBit(bit);
			sei();

			// ncrement bit index counter and shift the bit mask
			bit_index++; 
			bit_mask <<= 1;
			
			if (!bit_mask)
			{
				// if the mask is 0 then go to new ROM
				// accumulate the CRC and incriment the byte index and bit mask
				dallasCRC(rom_id->byte[byte_index]);
				byte_index++;
				bit_mask++;
			}
		}
	}

	if ((bit_index < 65) || dallas_crc)
	{
		// search was unsuccessful - reset the last discrepancy to 0 and return false
		last_discrep = 0;
		return FALSE;
	}

	// search was successful, so set last_discrep and done_flag
	last_discrep = discrep_marker;
	done_flag = (last_discrep==0);

	return TRUE;
}

void dallasPrintError(u08 error)
{
	// if there was not an error, return
	if (error == DALLAS_NO_ERROR)
		return;
	
	// print header message
	rprintfProgStrM("ERROR ");
	rprintfChar(error);
	rprintfProgStrM(": ");

	// print custom error message
	switch (error)
	{
		case DALLAS_NO_PRESENCE:
			rprintfProgStrM("no presence detected");
			break;
		case DALLAS_INVALID_CHANNEL:
			rprintfProgStrM("Invalid Chan");
			break;
		case DALLAS_VERIFY_ERROR:
			rprintfProgStrM("Verify");
			break;
		case DALLAS_ADDRESS_ERROR:
			rprintfProgStrM("Bad Addr");
			break;
		case DALLAS_CRC_ERROR:
			rprintfProgStrM("Data CRC");	
			break;
		case DALLAS_DEVICE_ERROR:
			rprintfProgStrM("No response");
			break;
		case DALLAS_FORMAT_ERROR:
			rprintfProgStrM("Bad return format");
			break;
		case DALLAS_NULL_POINTER:
			rprintfProgStrM("Null Pointer");
			break;
		case DALLAS_ZERO_LEN:
			rprintfProgStrM("RAM rd/wr 0 bytes");
			break;
		case DALLAS_BUS_ERROR:
			rprintfProgStrM("Bus error, check pullup");
			break;
		case DALLAS_RESOLUTION_ERROR:
			rprintfProgStrM("resolution out of range");
			break;
		default:
			rprintfProgStrM("Unknown");
	}
	rprintfCRLF();
}
