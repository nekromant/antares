#include <arch/antares.h>
#include <avr/boot.h>
#include <generated/usbconfig.h>
#include <arch/vusb/usbportability.h>
#include <arch/vusb/usbdrv.h>
#include "uisp.h"


#ifndef ulong
#   define ulong    unsigned long
#endif
#ifndef uint
#   define uint     unsigned int
#endif

#if (FLASHEND) > 0xffff /* we need long addressing */
#   define addr_t           ulong
#else
#   define addr_t           uint
#endif


static addr_t           currentAddress; /* in bytes */
static uchar            offset;         /* data already processed in current transfer */
volatile static uchar            running=1;


const PROGMEM char usbHidReportDescriptor[42] = {
    0x06, 0x00, 0xff,              // USAGE_PAGE (Generic Desktop)
    0x09, 0x01,                    // USAGE (Vendor Usage 1)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x26, 0xff, 0x00,              //   LOGICAL_MAXIMUM (255)
    0x75, 0x08,                    //   REPORT_SIZE (8)

    0x85, 0x01,                    //   REPORT_ID (1)
    0x95, 0x06,                    //   REPORT_COUNT (6)
    0x09, 0x00,                    //   USAGE (Undefined)
    0xb2, 0x02, 0x01,              //   FEATURE (Data,Var,Abs,Buf)

    0x85, 0x02,                    //   REPORT_ID (2)
    0x95, 0x83,                    //   REPORT_COUNT (131)
    0x09, 0x00,                    //   USAGE (Undefined)
    0xb2, 0x02, 0x01,              //   FEATURE (Data,Var,Abs,Buf)
    
    0x85, 0x03,                    //   REPORT_ID (3)
    0x95, 0x05,                    //   REPORT_COUNT (0x3)
    0x09, 0x00,                    //   USAGE (Undefined)
    0xb2, 0x02, 0x01,              //   FEATURE (Data,Var,Abs,Buf)
    
    0xc0                           // END_COLLECTION
};

static char target;
static uchar    replyBuffer[7];   
uint8_t* ee_addr;
uchar   usbFunctionSetup(uchar data[8])
{
usbRequest_t    *rq = (void *)data;


    if(rq->bRequest == USBRQ_HID_SET_REPORT){
        if(rq->wValue.bytes[0] == 2){
            offset = 0;
			target = 0;
            return USB_NO_MSG;
        }else if(rq->wValue.bytes[0] == 3){
		    target = 1;
 			return USB_NO_MSG;
		}
        else{
            running=0;
        }
    }else if(rq->bRequest == USBRQ_HID_GET_REPORT){	
		if(rq->wValue.bytes[0] == 3){
			replyBuffer[0] = 3;
			replyBuffer[1] = ((int) ee_addr) & 0xff;
			replyBuffer[2] = ((int) ee_addr) >> 8;
			replyBuffer[4] = eeprom_read_byte(ee_addr);	
		}else
		{
		replyBuffer[0] = 1;
        replyBuffer[1] = SPM_PAGESIZE & 0xff;
        replyBuffer[2] = SPM_PAGESIZE >> 8;
        replyBuffer[3] = ((long)FLASHEND + 1) & 0xff;
        replyBuffer[4] = (((long)FLASHEND + 1) >> 8) & 0xff;
        replyBuffer[5] = (((long)FLASHEND + 1) >> 16) & 0xff;
        replyBuffer[6] = (((long)FLASHEND + 1) >> 24) & 0xff; 
		}
             /*
			  * if (rq->wValue.bytes[2]==0)
 			 {
 				 replyBuffer[0] = 3;
 				 replyBuffer[4] = eeprom_read_byte(addr);
 				 return 5;
 			 }else
 			 {
 				 eeprom_write_byte(addr,rq->wValue.bytes[4]);
 				 return USB_NO_MSG;
 			 } */         
        usbMsgPtr = replyBuffer;
        return 7;
	}
    
    return 0;
}

uchar usbFunctionWrite(uchar *data, uchar len)
{
	if (target)
	{
	ee_addr = (data[3] << 8 | data[2]);
	if (data[1])
	{
		eeprom_write_byte(ee_addr,data[4]);
	}	
	return len;
	}
union {
    addr_t  l;
    uint    s[sizeof(addr_t)/2];
    uchar   c[sizeof(addr_t)];
}       address;
uchar   isLast;

    address.l = currentAddress;
    if(offset == 0){
        address.c[0] = data[1];
        address.c[1] = data[2];
#if (FLASHEND) > 0xffff /* we need long addressing */
        address.c[2] = data[3];
        address.c[3] = 0;
#endif
        data += 4;
        len -= 4;
    }
    offset += len;
    isLast = offset & 0x80; /* != 0 if last block received */
    do{
        addr_t prevAddr;
#if SPM_PAGESIZE > 256
        uint pageAddr;
#else
        uchar pageAddr;
#endif
        pageAddr = address.s[0] & (SPM_PAGESIZE - 1);
        if(pageAddr == 0){              /* if page start: erase */
            cli();
            boot_page_erase(address.l); /* erase page */
            sei();
            boot_spm_busy_wait();       /* wait until page is erased */
        }
        cli();
        boot_page_fill(address.l, *(short *)data);
        sei();
        prevAddr = address.l;
        address.l += 2;
        data += 2;
        /* write page when we cross page boundary */
        pageAddr = address.s[0] & (SPM_PAGESIZE - 1);
        if(pageAddr == 0){
            cli();
            boot_page_write(prevAddr);
            sei();
            boot_spm_busy_wait();
        }
        len -= 2;
    }while(len);
    currentAddress = address.l;
    return isLast;
}

static void (*nullVector)(void) __attribute__((__noreturn__));
#ifndef TCCR0
#define TCCR0   TCCR0B
#endif
#ifndef GICR
#define GICR    MCUCR
#endif


static void leaveBootloader()
{
    cli();
    boot_rww_enable();
    USB_INTR_ENABLE = 0;
    USB_INTR_CFG = 0;       /* also reset config bits */
    TCCR0 = 0;              /* default value */
    GICR = (1 << IVCE);     /* enable change of interrupt vectors */
    GICR = (0 << IVSEL);    /* move interrupts to application flash section */
/* We must go through a global function pointer variable instead of writing
 *  ((void (*)(void))0)();
 * because the compiler optimizes a constant 0 to "rcall 0" which is not
 * handled correctly by the assembler.
 */
    nullVector();
}


void usbReconnect()
{
	DDRD=0xff;
    _delay_ms(250);
	DDRD=0;
}

ANTARES_APP(loader_app)
{
 	DDRC|=(1<<2);
  	PORTC|=0x7;
	GICR = (1 << IVCE);  /* enable change of interrupt vectors */
    GICR = (1 << IVSEL); /* move interrupts to boot flash section */
 	usbReconnect();	
  	usbInit();
	while (running)
	{
 	usbPoll();  	
  		if (!run_released())
 			break;
 	}
	DDRC=0xff;
	PORTC=0;
	usbReconnect();
	leaveBootloader();
}