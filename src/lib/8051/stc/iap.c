#include <arch/stc.h>
#include <arch/lib/delaylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <arch/lib/stc/iap.h>

#if CONFIG_F_CPU < 1000000
#define IAP_SPEED 0x07
#elif CONFIG_F_CPU < 2000000
#define IAP_SPEED 0x06
#elif CONFIG_F_CPU < 3000000
#define IAP_SPEED 0x05
#elif CONFIG_F_CPU < 6000000
#define IAP_SPEED 0x04
#elif CONFIG_F_CPU < 12000000
#define IAP_SPEED 0x03
#elif CONFIG_F_CPU < 20000000
#define IAP_SPEED 0x02
#elif CONFIG_F_CPU < 2400000
#define IAP_SPEED 0x01
#elif CONFIG_F_CPU < 3000000
#define IAP_SPEED 0x00
#else
#error "Unsupported CPU speed"
#endif


#define IAP_ENABLE (IAP_ENABLEB|IAP_SPEED)


void iap_disable() {
	IAP_CONTR=0;
	IAP_CMD=0;
	IAP_TRIG=0;
	IAP_ADDRH=0x80;
	IAP_ADDRL=0x80;
}

unsigned char iap_readbyte(uint16_t addr) {
	unsigned char ret;
	IAP_CONTR = IAP_ENABLE;
	IAP_CMD = CMD_READ;
	IAP_ADDRL=addr;
	IAP_ADDRH=addr>>8;
	IAP_TRIG=0x5a;
	IAP_TRIG=0xa5;
	__asm
		nop
		nop
	__endasm;
	ret = IAP_DATA;
	iap_disable();
	return ret;
}

void iap_erasepage(uint16_t addr) {
	IAP_CONTR = IAP_ENABLE;
	IAP_CMD = CMD_ERASE;
	IAP_ADDRL=addr;
	IAP_ADDRH=addr>>8;
	IAP_TRIG=0x5a;
	IAP_TRIG=0xa5;
	__asm
		nop
		nop
	__endasm;
	iap_disable();
}

void iap_writebyte(uint16_t addr, char dat) {
	IAP_CONTR = IAP_ENABLE;
	IAP_CMD = CMD_WRITE;
	IAP_ADDRL=addr;
	IAP_ADDRH=addr>>8;
	IAP_DATA=dat;
	IAP_TRIG=0x5a;
	IAP_TRIG=0xa5;
	__asm
		nop
		nop
	__endasm;
	iap_disable();
}
