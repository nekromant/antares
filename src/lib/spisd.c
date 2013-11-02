/**
* 
* Copyright (c) 2013 Andrew 'Necromant' Andrianov
* Copyright (c) 2011 CC Dharmani, Chennai (India), www.dharmanitech.com
* Distributed under the GNU GPL v2. For full terms see the file COPYING.
*
*/


#include <stdio.h>
#include <stdarg.h>
#include <lib/spisd.h>

#define GO_IDLE_STATE            0
#define SEND_OP_COND             1
#define SEND_IF_COND		 8
#define SEND_CSD                 9
#define STOP_TRANSMISSION        12
#define SEND_STATUS              13
#define SET_BLOCK_LEN            16
#define READ_SINGLE_BLOCK        17
#define READ_MULTIPLE_BLOCKS     18
#define WRITE_SINGLE_BLOCK       24
#define WRITE_MULTIPLE_BLOCKS    25
#define ERASE_BLOCK_START_ADDR   32
#define ERASE_BLOCK_END_ADDR     33
#define ERASE_SELECTED_BLOCKS    38
#define SD_SEND_OP_COND		 41
#define APP_CMD			 55
#define READ_OCR		 58
#define CRC_ON_OFF               59



/** 
 * Initialiaze the SD card
 * 
 * @param sd sd_card instance
 * 
 * @return 0 on success, 1 otherwise
 */
unsigned char sd_init(struct sd_card *sd)
{
        unsigned char i, response;
        unsigned int retry=0 ;

        sd->flags=0;
        sd->set_speed(400); /* 400 kHz by default */

        for(i=0; i<10; i++)
                sd->xfer(0xff);

        sd->cs(1);
        do {
                response = sd_cmd(sd, GO_IDLE_STATE, 0);
                retry++;
                if(retry>0x20)
                        return 1;

        } while (response != 0x01);

        sd->cs(0);
        sd->xfer (0xff);
        sd->xfer (0xff);

        retry = 0;

        sd->version = 2;
        do {
                /* Check power supply status, we must do that for SDHC card */
                response = sd_cmd(sd, SEND_IF_COND,0x000001AA);
                retry++;
                if ( retry > 0xfe) {
                        sd->version = 1;
                        sd->card_type = 1;
                        break;
                }

        } while (response != 0x01);

        retry = 0;

        do {
                response = sd_cmd(sd, APP_CMD, 0);
                response = sd_cmd(sd, SD_SEND_OP_COND, 0x40000000); 

                retry++;
                if(retry>0xfe) {
                        return 2;
                }

        } while(response != 0x00);


        retry = 0;

        if (sd->version == 2) {
                do {
                        response = sd_cmd(sd, READ_OCR,0);
                        retry++;
                        if (retry>0xfe) {
                                sd->card_type = 0;
                                break;
                        }

                } while (response != 0x00);

                if (sd->flags & FLAG_SDHC) 
			sd->card_type = 2;
                else 
			sd->card_type = 3;
        }

	sd_cmd(sd, CRC_ON_OFF, 0);
	sd_cmd(sd, SET_BLOCK_LEN, 512);
	sd->set_speed(20000);
        return 0;
}


/** 
 *  Send a command to SD card
 * 
 * @param sd sd_card instance
 * @param cmd command index
 * @param arg argument
 * 
 * @return SD Response
 */
unsigned char sd_cmd(struct sd_card* sd, unsigned char cmd, unsigned long arg)
{
        unsigned char response, retry=0, status;

/* 
 * SD card accepts byte address while SDHC accepts block address in multiples of 512
 * so, if it's SD card we need to convert block address into corresponding byte address by
 * multipying it with 512. which is equivalent to shifting it left 9 times
 * following 'if' loop does that
 */
	
        if((sd->flags & FLAG_SDHC) == 0)
                if(cmd == READ_SINGLE_BLOCK     ||
		   cmd == READ_MULTIPLE_BLOCKS  ||
		   cmd == WRITE_SINGLE_BLOCK    ||
		   cmd == WRITE_MULTIPLE_BLOCKS ||
		   cmd == ERASE_BLOCK_START_ADDR||
		   cmd == ERASE_BLOCK_END_ADDR ) {
                        arg = arg << 9;
                }
	
        sd->cs(1);
	
        sd->xfer(cmd | 0x40);
        sd->xfer(arg>>24);
        sd->xfer(arg>>16);
        sd->xfer(arg>>8);
        sd->xfer(arg);

	/* We need a valid CRC for CMD8 & CMD0 */
        if(cmd == SEND_IF_COND)	
                sd->xfer(0x87); 
        else
                sd->xfer(0x95);

        while((response = sd->xfer(0xff)) == 0xff) 
                if(retry++ > 0xfe) break; 

        if(response == 0x00 && cmd == 58) { 
                status = sd->xfer(0xff) & 0x40;
                if(status == 0x40) 
			sd->flags |= FLAG_SDHC; 
                else 
			sd->flags &= ~FLAG_SDHC;

                sd->xfer(0xff); 
                sd->xfer(0xff); 
                sd->xfer(0xff);
        }

        sd->xfer(0xff); 
        sd->cs(0);

        return response; 
}

/** 
 * Erase blocks on SD card
 * 
 * @param sd sd_card instance
 * @param start_block start block
 * @param num_blocks block count
 * 
 * @return 0 on success, card respose on failure
 */
unsigned char sd_erase (struct sd_card *sd, unsigned long start_block, unsigned long num_blocks)
{
        unsigned char response;

        response = sd_cmd(sd, ERASE_BLOCK_START_ADDR, start_block); 
        if(response != 0x00) 
                return response;

        response = sd_cmd(sd, ERASE_BLOCK_END_ADDR,(start_block + num_blocks - 1)); 
        if(response != 0x00)
                return response;

        response = sd_cmd(sd, ERASE_SELECTED_BLOCKS, 0);
        if(response != 0x00)
                return response;

        return 0;
}


/** 
 * Read a block from SD card to a user-supplied buffer. 
 * The buffer must have at least 512 bytes available
 * 
 * @param sd sd_card instance
 * @param block block number
 * @param buf target buffer
 * 
 * @return 0 on success, card response otherwise
 */
unsigned char sd_read(struct sd_card *sd, unsigned long block, char* buf)
{
        unsigned char response;
        unsigned int i, retry=0;

        response = sd_cmd(sd, READ_SINGLE_BLOCK, block); 

        if(response != 0x00) 
		return response; 

        sd->cs(1);

        retry = 0;
        while(sd->xfer(0xff) != 0xfe) 
                if(retry++ > 0xfffe) {
                        sd->cs(0);    
                        return 1;
                }
	
        for(i=0; i<512; i++) 
                buf[i] = sd->xfer(0xff);
	
        sd->xfer(0xff); /* TODO: Check CRC16 */
        sd->xfer(0xff);

        sd->xfer(0xff);
        sd->cs(0);

        return 0;
}


/** 
 * Write a single block to the card from a user-supplied buffer.  
 * The source buffer must have at least 512 bytes.
 * 
 * @param sd sd_card instance
 * @param block blocknumber
 * @param buf source buffer
 * 
 * @return 0 on success, card response otherwise
 */
unsigned char sd_write(struct sd_card *sd, unsigned long block, char* buf)
{
        unsigned char response;
        unsigned int i, retry=0;

        response = sd_cmd(sd, WRITE_SINGLE_BLOCK, block); 

        if(response != 0x00) 
		return response;

        sd->cs(1);

        sd->xfer(0xfe);

        for(i=0; i<512; i++)
                sd->xfer(buf[i]);

        sd->xfer(0xff);     /* TODO: CRC16 Here */
        sd->xfer(0xff);

        response = sd->xfer(0xff);

        if( (response & 0x1f) != 0x05) {
                sd->cs(0);              
                return response;
        }

        while(!sd->xfer(0xff)) 
                if(retry++ > 0xfffe) {
                        sd->cs(0);
                        return 1;
                }

        sd->cs(0);
        sd->xfer(0xff);
        sd->cs(1);

        while(!sd->xfer(0xff))
                if(retry++ > 0xfffe) {
                        sd->cs(0);
                        return 1;
                }
        sd->cs(0);

        return 0;
}

/** 
 * Read multiple blocks from SD card to user buffer
 * The buffer must have at least block_count*512 bytes available.
 *
 * @param sd sd_card instance
 * @param start_block starting block
 * @param block_count number of blocks to read
 * @param buffer target buffer
 * 
 * @return 0 on success, card response otherwise
 */
unsigned char sd_multiread (struct sd_card *sd, 
			    unsigned long start_block, 
			    unsigned long block_count,
			    char* buffer)
{
        unsigned char response;
        unsigned int i, j=0, retry=0;

        retry = 0;
	
        response = sd_cmd(sd, READ_MULTIPLE_BLOCKS, start_block); 

        if(response != 0x00) 
		return response; 

        sd->cs(1);

        while( block_count ) {
                retry = 0;
                while(sd->xfer(0xff) != 0xfe) 
                        if(retry++ > 0xfffe) {
                                sd->cs(0);    
                                return 1;
                        }

                for(i=0; i<512; i++) 
                        buffer[j++] = sd->xfer(0xff);

                sd->xfer(0xff);
                sd->xfer(0xff);

                sd->xfer(0xff);

                block_count--;
        }

        sd_cmd(sd, STOP_TRANSMISSION, 0); 
        sd->cs(0);
        sd->xfer(0xff); 

        return 0;
}

/** 
 * Write multiple blocks to SD from a user-supplied buffer
 * 
 * @param sd sd_card instance
 * @param start_block start block
 * @param total_blocks number of blocks
 * @param buffer source buffer
 * 
 * @return 0 on success, card response otherwise
 */
unsigned char sd_multiwrite(struct sd_card *sd, 
			    unsigned long start_block, 
			    unsigned long total_blocks, 
			    char* buffer)
{
        unsigned char response;
        unsigned int i, j=0, retry=0;

        response = sd_cmd(sd, WRITE_MULTIPLE_BLOCKS, start_block);

        if(response != 0x00) 
		return response;

        sd->cs(1);
	
        while( total_blocks ) {
                i=0;
                sd->xfer(0xfc); 

                for(i=0; i<512; i++) 
                        sd->xfer( buffer[j++] );

                sd->xfer(0xff);
                sd->xfer(0xff);

                response = sd->xfer(0xff);
                if( (response & 0x1f) != 0x05) { 
                        sd->cs(0);
                        return response;
                }

                while (!sd->xfer(0xff)) 
                        if(retry++ > 0xfffe) {
                                sd->cs(0);
                                return 1;
                        }

                sd->xfer(0xff);
                total_blocks--;
        }

        sd->xfer(0xfd);

        retry = 0;

        while(!sd->xfer(0xff)) 
                if(retry++ > 0xfffe) {
                        sd->cs(0);
                        return 1;
                }

        sd->cs(0);
        sd->xfer(0xff); 
        sd->cs(1); 

        while(!sd->xfer(0xff)) 
                if(retry++ > 0xfffe) {
                        sd->cs(0);
                        return 1;
                }
        sd->cs(0);
	
        return 0;
}
