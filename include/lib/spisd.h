/**
* 
* Copyright (c) 2013 Andrew 'Necromant' Andrianov
* Copyright (c) 2011 CC Dharmani, Chennai (India), www.dharmanitech.com
* Distributed under the GNU GPL v2. For full terms see the file COPYING.
*
*/


#ifndef SPISD_H
#define SPISD_H

#include <stdint.h>

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


struct sd_info
{
    /**
     * A manufacturer code globally assigned by the SD card organization.
     */
    uint8_t manufacturer;
    /**
     * A string describing the card's OEM or content, globally assigned by the SD card organization.
     */
    uint8_t oem[3];
    /**
     * A product name.
     */
    uint8_t product[6];
    /**
     * The card's revision, coded in packed BCD.
     *
     * For example, the revision value \c 0x32 means "3.2".
     */
    uint8_t revision;
    /**
     * A serial number assigned by the manufacturer.
     */
    uint32_t serial;
    /**
     * The year of manufacturing.
     *
     * A value of zero means year 2000.
     */
    uint8_t manufacturing_year;
    /**
     * The month of manufacturing.
     */
    uint8_t manufacturing_month;
    /**
     * The card's total capacity in bytes.
     */
    uint64_t capacity;
    /**
     * Defines wether the card's content is original or copied.
     *
     * A value of \c 0 means original, \c 1 means copied.
     */
    uint8_t flag_copy;
    /**
     * Defines wether the card's content is write-protected.
     *
     * \note This is an internal flag and does not represent the
     *       state of the card's mechanical write-protect switch.
     */
    uint8_t flag_write_protect;
    /**
     * Defines wether the card's content is temporarily write-protected.
     *
     * \note This is an internal flag and does not represent the
     *       state of the card's mechanical write-protect switch.
     */
    uint8_t flag_write_protect_temp;
    /**
     * The card's data layout.
     *
     * See the \c SD_RAW_FORMAT_* constants for details.
     *
     * \note This value is not guaranteed to match reality.
     */
    uint8_t format;
};

#define sd_is_shdc(sd)  ((sd)->flags & FLAG_SDHC)
#define sd_is_ready(sd) ((sd)->flags & FLAG_READY)

unsigned char sd_init(struct sd_card *sd);
int sd_read_info(struct sd_card *sd, struct sd_info *info);
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
