/**
* 
* Copyright (c) 2013 Andrew 'Necromant' Andrianov
* Copyright (c) 2011 CC Dharmani, Chennai (India), www.dharmanitech.com
* Distributed under the GNU GPL v2. For full terms see the file COPYING.
*
*/


#ifndef SPISD_H
#define SPISD_H


#define FLAG_READY               (1<<0)
#define FLAG_SDHC                (1<<1)

struct sd_card {
        void          (*cs)(int select);
        unsigned char (*xfer)(unsigned char b);
        void          (*set_speed)(int speed);
        char          flags;
        char          version;
        char          card_type;
};

#define sd_is_shdc(sd)  (sd->flags & FLAG_SDHC)
#define sd_is_ready(sd) (sd->flags & FLAG_READY)

unsigned char sd_init(struct sd_card *sd);
unsigned char sd_cmd(struct sd_card* sd, unsigned char cmd, unsigned long arg);
unsigned char sd_erase (struct sd_card *sd, unsigned long start_block, unsigned long num_blocks);
unsigned char sd_write(struct sd_card *sd, unsigned long block, char* buf);
unsigned char sd_read(struct sd_card *sd, unsigned long block, char* buf);

unsigned char sd_multiwrite(struct sd_card *sd, 
			    unsigned long start_block, 
			    unsigned long total_blocks, 
			    char* buffer);
unsigned char sd_multiread (struct sd_card *sd, 
			    unsigned long start_block, 
			    unsigned long block_count,
			    char* buffer);

#endif
