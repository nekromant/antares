#include <arch/antares.h>
#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/times.h>
#include <sys/unistd.h>
#include <lib/xmodem.h>
#include "stm32f10x.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_fsmc.h"
#include "stm32_eval_spi_flash.h"
#include "misc.h"
#include "xsscu.h"

/*
JRST CCLK PB4
JTDO DIN PB3
JTCK PROGB PA14

JTDI INITB PA15
JTMS DONE PA13
*/


static void clk_ctl(char m)
{
	GPIO_WriteBit(GPIOB, GPIO_Pin_4, m ? 1 : 0);
}

static void progb_ctl(char m)
{
	GPIO_WriteBit(GPIOA, GPIO_Pin_14, m ? 1 : 0);
}

static void sout_ctl(char m)
{
	GPIO_WriteBit(GPIOB, GPIO_Pin_3, m ? 1 : 0);
}

static char get_done()
{
	return (char) GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_13);
}

static char get_initb()
{
	return (char) GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_15);
}

static void mdelay(int n)
{
        int i = n*100;													/* About 1/4 second delay */
        while (i-- > 0) {
                asm("nop");													/* This stops it optimising code out */
        }
}

static const struct xsscu_unit fpga0 = {
.name = "Xilinx XS3S100E",
.clk_ctl = clk_ctl,
.sout_ctl = sout_ctl,
.progb_ctl = progb_ctl,
.get_initb = get_initb,
.get_done = get_done,
.delay = mdelay,
.delay_pb = 1200,
.delay_clk = 0
};

static const GPIO_InitTypeDef xsscu_pai = {
.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_15,
.GPIO_Mode = GPIO_Mode_IPU,
};

static const GPIO_InitTypeDef xsscu_pao = {
.GPIO_Speed = GPIO_Speed_50MHz,
.GPIO_Pin = GPIO_Pin_14,
.GPIO_Mode = GPIO_Mode_Out_PP,
};

static const GPIO_InitTypeDef xsscu_pbo = {
.GPIO_Speed = GPIO_Speed_50MHz,
.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4,
.GPIO_Mode = GPIO_Mode_Out_PP,
};

static  const GPIO_InitTypeDef fpga_clk = {
	.GPIO_Speed = GPIO_Speed_50MHz,
	.GPIO_Pin = GPIO_Pin_8,
	.GPIO_Mode = GPIO_Mode_Out_PP,
};

void fpga_enable_clock()
{
	GPIO_SetBits(GPIOG,GPIO_Pin_8);
}

void fpga_disable_clock()
{
	GPIO_ResetBits(GPIOG,GPIO_Pin_8);
}


void mcortex_fpga_init()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOG|RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);
	GPIO_Init(GPIOA, (GPIO_InitTypeDef*) &xsscu_pao);
	GPIO_Init(GPIOA, (GPIO_InitTypeDef*) &xsscu_pai);
 	GPIO_Init(GPIOB, (GPIO_InitTypeDef*) &xsscu_pbo);
	GPIO_Init(GPIOG, (GPIO_InitTypeDef*) &fpga_clk);
	printk("fpga: Device 0 is  %s\n\r",fpga0.name);
	xsscu_reset(&fpga0);
	
}

#define MAGIC	0xf0f0f0
struct fpga_hdr
{
	uint32_t magic;
	uint32_t len;
};


static int pos=0;
int spi_writer(unsigned char* buffer, int size)
{
	sFLASH_WriteBuffer(buffer,pos,size);
	pos+=size;
	return size;
}


int try_getc();
void __putc(char c)
{
	putchar(c);
}


FSMC_NORSRAMTimingInitTypeDef  fsmc_timing = 
{
  .FSMC_AddressSetupTime = 2,
  .FSMC_AddressHoldTime = 12,
  .FSMC_DataSetupTime = 12,
//   .FSMC_BusTurnAroundDuration = 0,
  .FSMC_CLKDivision = 4,
//   .FSMC_DataLatency = 0,
  .FSMC_AccessMode = FSMC_AccessMode_A,
};

FSMC_NORSRAMInitTypeDef  fsmc_inits =
{
  .FSMC_Bank = FSMC_Bank1_NORSRAM3,
  .FSMC_DataAddressMux = FSMC_DataAddressMux_Disable,
  .FSMC_MemoryType = FSMC_MemoryType_SRAM,
  .FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_8b,
  .FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable,
  .FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable,
  .FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low,
  .FSMC_WrapMode = FSMC_WrapMode_Disable,
  .FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState,
  .FSMC_WriteOperation = FSMC_WriteOperation_Enable,
  .FSMC_WaitSignal = FSMC_WaitSignal_Disable,
  .FSMC_ExtendedMode = FSMC_ExtendedMode_Disable,
  .FSMC_WriteBurst = FSMC_WriteBurst_Disable,
  .FSMC_ReadWriteTimingStruct = &fsmc_timing,
  .FSMC_WriteTimingStruct = &fsmc_timing,
};

#define Bank1_SRAM3_ADDR    ((uint32_t)0x68000000)


void mcortex_fpga_smc()
{
	printk("fpga: Enabling SMC\n\r");
	GPIO_InitTypeDef GPIO_InitStructure;
	  /*!< SRAM Data lines configuration */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_8 | GPIO_Pin_9 |
                                GPIO_Pin_10 | GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOD, &GPIO_InitStructure); 
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 |
                                GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | 
                                GPIO_Pin_15;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | 
                                GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_12 | GPIO_Pin_13 | 
                                GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_Init(GPIOF, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | 
                                GPIO_Pin_4 | GPIO_Pin_5;
  GPIO_Init(GPIOG, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13; 
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  
    /*!< NOE and NWE configuration */  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 |GPIO_Pin_5;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  
    /*!< NE3 configuration */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 
  GPIO_Init(GPIOG, &GPIO_InitStructure);
  
	FSMC_NORSRAMInit(&fsmc_inits); 
	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM3, ENABLE);
	char* a = Bank1_SRAM3_ADDR;
	short* b = Bank1_SRAM3_ADDR;
	int* c = Bank1_SRAM3_ADDR;
	
	printk("fpga: chip id: 0x%hhx\n\r",*a++);
	a=Bank1_SRAM3_ADDR;
	printk("fpga: entering test loop\n\r");
	printk("fpga: usage: r/w addr [value]\n\r");
	int offset;
	char m;
	char value;
}
void mctx_console()
{
  	char* a = Bank1_SRAM3_ADDR;
	short* b = Bank1_SRAM3_ADDR;
	int* c = Bank1_SRAM3_ADDR;
	int offset;
	char m;
	char value;
	while (1)
	{
		printf("\n\rfpga: ");
		m=getchar();
		if (m=='u') {
			mcortex_fpga_xmodem();
			continue;
		}
		printf("%c ",m );
		scanf("%d",&offset);
		printf(" %d ",offset);
		a = Bank1_SRAM3_ADDR + offset;
		b = a;
		c = a; 
		if ((m!='r') && (m!='w') && (m!='m') && (m!='p')) continue;
		if (m=='w') 
		{
			scanf("%hhx",&value);
			*a = value;
			printf("OK\n\r");
		}
		if (m=='m')
		{
			mctx_enc_dumploop();
		}
		else
			if (m=='p')
			{
				mcortex_init_motorregs();	
			}
		else
		{
		printf("\n\r *%d == %hhx / %hx / %x\n\r", offset, *a, *b, *c );
		}
	}
}

void mcortex_init_prescaler()
{

}
void mcortex_init_servoregs()
{
	
}
void mcortex_init_motorregs()
{
	uint8_t* ctl = Bank1_SRAM3_ADDR+3;
	uint16_t* top =  Bank1_SRAM3_ADDR+14;
	uint16_t* pv =  Bank1_SRAM3_ADDR+16;
	*ctl = 0xff; //set rst high, enable all the stuff
	*top=1000;
	
	*ctl = 0x00;
	while (1)
	{
	*pv = (*pv) +  1;
	if (try_getc(1)>0) break;
	printf("PWM == %d\r", *pv);
	if (*pv == *top )
		*pv=0;
	mdelay(100);
	}
}


void mcortex_fpga_update()
{
	xmodem_init(spi_writer);
	printk("fpga: erasing SPI flash...\n\r");
  	sFLASH_EraseBulk();
	printk("fpga: start xmodem upload, plz\n\r");
	xmodem_get(0,200000);
}


int fwriter(unsigned char* buffer, int size)
{
	xsscu_write(&fpga0, buffer, size);
	return size;
}
void mcortex_fpga_xmodem()
{
	xmodem_init(fwriter);
	xsscu_reset(&fpga0);
	printk("fpga: directly sending via xmodem\n\r");
	printk("fpga: start xmodem upload, plz\n\r");
	xmodem_get(0,200000);	
}

void mcortex_fpga_fromspi()
{
	printk("fpga: config from spi flash\n\r");
	printk("fpga: ");
	xsscu_reset(&fpga0);
	int pos=0;
	int i;
	char buf[1024];
	for (i=0;i<80;i++)
	{
		sFLASH_ReadBuffer(buf,pos,1024);
		xsscu_write(&fpga0, buf, 1024);
		pos+=1024;
		printf("#");
	}
	printf("\n\r");
	xsscu_finalize(&fpga0, 1200);
	printk("fpga: ready\n\r");
	fpga_enable_clock();
	
}



#if 0
#include "fw.h"
void mcortex_fpga_fromflash()
{
	fpga_disable_clock();
	xsscu_reset(&fpga0);
	xsscu_write(&fpga0, fw, 200);
	mdelay(1000);
	xsscu_write(&fpga0, fw+200, fw_size-200);
	xsscu_finalize(&fpga0, 1200);
	fpga_enable_clock();
}
#endif