#ifndef __IAP_H
#define __IAP_H

/* We need this for uint16_t */
#include <stdint.h>

#define IAP_ENABLEB 0x80
#define CMD_IDLE  0
#define CMD_READ  1
#define CMD_WRITE 2
#define CMD_ERASE 3

void iap_reboot2isp();
void iap_disable();
unsigned char iap_readbyte(uint16_t addr);
void iap_erasepage(uint16_t addr);
void iap_writebyte(uint16_t addr, char data);

#endif
