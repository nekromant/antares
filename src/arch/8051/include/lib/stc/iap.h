#ifndef IAP_H
#define IAP_H

/* We need this for uint16_t */
#include <stdint.h>

#define IAP_ENABLEB 0x80
#define CMD_IDLE  0
#define CMD_READ  1
#define CMD_WRITE 2
#define CMD_ERASE 3

#define iap_reboot2isp() IAP_CONTR=0x60;

void iap_disable();
unsigned char iap_readbyte(uint16_t addr);
void iap_erasepage(uint16_t addr);
void iap_writebyte(uint16_t addr, char dat);

#endif
