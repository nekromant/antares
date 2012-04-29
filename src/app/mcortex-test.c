//#define STM32F10X_LD
#include <arch/antares.h>
#include <stdio.h>
#include "stm32f10x.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_usart.h"
#include "stm32_eval_spi_flash.h"

#include "misc.h"

#include "mcortex-hw.h"

int stop_delay = 40; //время торможения
int radius_ref = 3550;  //расстояние от центра до колеса в тиках

void mdelay(int n)
{
        int i = n*10;													/* About 1/4 second delay */
        while (i-- > 0) {
                asm("nop");													/* This stops it optimising code out */
        }
}

//TODO: Tuning to custom freqs
void setup_freq()
{
  //Tune up to 76Mhz
  /* enable HSE */
  RCC->CR = RCC->CR | 0x00010001;
  while ((RCC->CR & 0x00020000) == 0); /* for it to come on */
  
  /* enable flash prefetch buffer */
  FLASH->ACR = 0x00000012;

  /* Configure PLL */
  RCC->CFGR = RCC->CFGR | 0x00110400;  /* pll=72Mhz,APB1=36Mhz,AHB=72Mhz */
  RCC->CR = RCC->CR     | 0x01000000;  /* enable the pll */
  while ((RCC->CR & 0x03000000) == 0);         /* wait for it to come on */
  
  /* Set SYSCLK as PLL */
  RCC->CFGR = RCC->CFGR | 0x00000002;
  while ((RCC->CFGR & 0x00000008) == 0); /* wait for it to come on */
}

void Usart1Init(void)
{
GPIO_InitTypeDef GPIO_InitStructure;
USART_InitTypeDef USART_InitStructure;
USART_ClockInitTypeDef USART_ClockInitStructure;
//enable bus clocks
RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
//Set USART1 Tx (PA.09) as AF push-pull
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_Init(GPIOA, &GPIO_InitStructure);
 
//Set USART1 Rx (PA.10) as input floating
 
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
 
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
 
GPIO_Init(GPIOA, &GPIO_InitStructure);
 
USART_ClockStructInit(&USART_ClockInitStructure);
 
USART_ClockInit(USART1, &USART_ClockInitStructure);
 
USART_InitStructure.USART_BaudRate = 115200;
 
USART_InitStructure.USART_WordLength = USART_WordLength_8b;
 
USART_InitStructure.USART_StopBits = USART_StopBits_1;
 
USART_InitStructure.USART_Parity = USART_Parity_No ;
 
USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
 
USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
 
//Write USART1 parameters
 
USART_Init(USART1, &USART_InitStructure);
 
//Enable USART1
 
USART_Cmd(USART1, ENABLE);
 
}


void SysTick_Handler()
{
    tmgr_tick();
}





//TODO: Totally rewrite it once a portable driver arrives
#define sFLASH_SPI                       SPI1
#define sFLASH_SPI_CLK                   RCC_APB2Periph_SPI1
#define sFLASH_SPI_SCK_PIN               GPIO_Pin_5                  /* PA.05 */
#define sFLASH_SPI_SCK_GPIO_PORT         GPIOA                       /* GPIOA */
#define sFLASH_SPI_SCK_GPIO_CLK          RCC_APB2Periph_GPIOA
#define sFLASH_SPI_MISO_PIN              GPIO_Pin_6                  /* PA.06 */
#define sFLASH_SPI_MISO_GPIO_PORT        GPIOA                       /* GPIOA */
#define sFLASH_SPI_MISO_GPIO_CLK         RCC_APB2Periph_GPIOA
#define sFLASH_SPI_MOSI_PIN              GPIO_Pin_7                  /* PA.07 */
#define sFLASH_SPI_MOSI_GPIO_PORT        GPIOA                       /* GPIOA */
#define sFLASH_SPI_MOSI_GPIO_CLK         RCC_APB2Periph_GPIOA
void mcortex_flash_init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /*!< sFLASH_SPI_CS_GPIO, sFLASH_SPI_MOSI_GPIO, sFLASH_SPI_MISO_GPIO 
       and sFLASH_SPI_SCK_GPIO Periph clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);

  /*!< sFLASH_SPI Periph clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
  
  /*!< Configure sFLASH_SPI pins: SCK */
  GPIO_InitStructure.GPIO_Pin = sFLASH_SPI_SCK_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(sFLASH_SPI_SCK_GPIO_PORT, &GPIO_InitStructure);

  /*!< Configure sFLASH_SPI pins: MOSI */
  GPIO_InitStructure.GPIO_Pin = sFLASH_SPI_MOSI_PIN;
  GPIO_Init(sFLASH_SPI_MOSI_GPIO_PORT, &GPIO_InitStructure);

  /*!< Configure sFLASH_SPI pins: MISO */
  GPIO_InitStructure.GPIO_Pin = sFLASH_SPI_MISO_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  
  GPIO_Init(sFLASH_SPI_MISO_GPIO_PORT, &GPIO_InitStructure);
  
  /*!< Configure sFLASH_CS_PIN pin: sFLASH Card CS pin */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_SetBits(GPIOB,GPIO_Pin_1|GPIO_Pin_0);
}

#define sFLASH_M25P128_ID         0x202018
#define sFLASH_M25P128_ID         0x202018
#define sFLASH_M25P32_ID          0x202016
#include "mcortex-sscu.h"

struct spi_flash_chip 
{
	char* name;
	uint32_t id;
	uint32_t pagesize;
	uint32_t erase_size;
	uint32_t size;
};

static struct spi_flash_chip sflash =
{
	.name = "ST M25P32",
	.id = 0x202016,
	.pagesize = 256,
	.erase_size = 512*1024,
	.size = 4 * 1024* 1024
};


int main()
{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);
		
		Usart1Init();
		SysTick_Config(SystemCoreClock / 1000);
		setvbuf(stdin, NULL, _IONBF, 0);
		setvbuf(stdout, NULL, _IONBF, 0);
		setvbuf(stderr, NULL, _IONBF, 0);
		printf("\xFF\n\r\n\r"); //text mode
		printk("Motor Cortex rev. 0.9 :: Antares uC system, v. %s\n\r", CONFIG_VERSION_STRING);
		printk("git commit: %s\n\r", CONFIG_VERSION_GIT);
		mcortex_flash_init();
		printk("sflash: lowlevel init performed\n\r");
		sFLASH_Init();
		uint32_t id = sFLASH_ReadID();
		printk("sflash: Detected %s (0x%x) %d bytes\n\r", sflash.name, sflash.id, sflash.size);
		mcortex_fpga_init();
		printk("bootmode: press 'f' to update fpga firmware in 1 sec\n\r");
		/*char m = getchar();
		if ('f' == m)
		{
			mcortex_fpga_update();
		}
 		printk("sflash: erasing everything...\n\r");
 		printk("sflash: done\n\r");
		*/
		int t = tmgr_get_uptime();
		mcortex_fpga_fromspi();
		mcortex_fpga_smc();
		mctx_init();
		mcortex_btns_init();
		mctx_console();
		t = tmgr_get_uptime() - t;
		printk("fpga: configuration done in %d ticks\n\r",t);
		// TODO: CODE HERE!!!
 		robot_move(0, 65535, 16000);
		
		printk("Moving completed\n\r");
		//mctx_console();
/*		tmgr_msleep(500);
		robot_rotate(0, 88, 32768);
		tmgr_msleep(500);
		robot_move(0, 65535, 4000);
		tmgr_msleep(500);
		robot_rotate(1, 92, 32768);
		tmgr_msleep(500);		
		robot_move(0, 65535, 16000);		
		printk("Rotate completed\n\r");
		tmgr_msleep(500);
		//robot_move(0, 65535, 4000);
		robot_rotate(0, 88, 32768);
		tmgr_msleep(500);
		robot_move(0, 65535, 2000);
		tmgr_msleep(500);		
		robot_rotate(0, 88, 32768);
		tmgr_msleep(500);		
		robot_move(0, 65535, 16000);*/		
// 		mctx_servo_test();
		mctx_reset_dir();
		mctx_etest();
		
		//motors_test();
		//mctx_console();
		//encoders_test();
		while(1)
		{
// 			printk("tick!\n\r");
		}
        return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
//void mctx_enc_reset(int enc)
//void  mctx_enc_invert(int enc, int inv)
//int mctx_enc_read(int enc)
//void mctx_set_motor_dir(int motor, int dir)
//void mctx_set_servo_pwm(int servo, short value)
//void mctx_set_motor_pwm(int motor, short value)
void encoders_reset()
{
  mctx_enc_reset(0);
  mctx_enc_reset(1);
}
int encoder_get(int enc, int ch)
{
  int val;
  val = mctx_enc_read(enc);
  val = abs(val);
  return val;
  ch = ch;
}
void encoders_test()
{
  while(1)
  {
    printk("%d %d\n\r", encoder_get(0,0), encoder_get(1,0)); 
  }
}

//void loop() {
  //reset_dir(0);
//  robot_move(1, 32767, to_ticks(1500)); // 0 - назад, 1 - вперёд
//  delay(500);
//  robot_rotate(1, 90, 8191); // 1 - по часовой, 0 - против часовой
//  delay(500);
//  reset_dir(0);
//  robot_move(1, 16383, to_ticks(630));
//  delay(500);
//  robot_rotate(1, 95, 8191); // 1 - по часовой, 0 - против часовой
//  delay(500);
//  grip_write(0,0);
//  delay(500);
//  robot_move(1, 16383, to_ticks(600));

//  while(1);;;
//}
void motor_set_dir(int motor, int dir){
    if(dir == 0){
      mctx_set_motor_dir(motor, dir+1);
    }
    else{
      mctx_set_motor_dir(motor, dir+1);    
    }
}
//void encoder_test()
//{
//  Serial3.print(encoder_get(0,0));
//  Serial3.print("   ");
//  Serial3.print(encoder_get(0,1));
// Serial3.print("   ");
//  Serial3.print(encoder_get(1,0));
//  Serial3.print("   ");
//  Serial3.print(encoder_get(1,1));
//  Serial3.println("   ");
//}

void motor_set_speed(int motor, int speed){
  if(speed > 65535) speed = 65535;
  if(speed < 0) speed = 0;
  mctx_set_motor_pwm(motor, 1000*speed/65536);
}

int sp0 = 0, sp1 = 0;

void chassis_write(int dir0, int dir1, int speed0, int speed1)
{
  motor_set_dir(0, !dir0);
  motor_set_dir(1, !dir1);
  motor_set_speed(0, speed0);
  motor_set_speed(1, speed1);
}
void motor_stop(int num)
{
  mctx_set_motor_dir(num, 0);
}
void chassis_stop(int speed, int dir){
  motor_stop(0);
  motor_stop(1);
  
  chassis_write(inv(dir), inv(dir), 65535, 65535);
  tmgr_msleep(500);
  
  motor_stop(0);
  motor_stop(1);
}
void motors_test()
{
  motor_set_dir(0, 0);
  motor_set_speed(0, 62000);
  motor_set_dir(1, 1);
  motor_set_speed(1, 62000);
printk("register dump: dir: %hhx pwm0: %hx pwm1: %hx\n\r", *MCTX_MOTOR_CTL, *MCTX_MPWM, *(MCTX_MPWM+2));
  
}
//void reset_dir(int dir)
//{
//  chassis_write(dir, dir, 16373, 16373);   
//  delay(1000);
//  motor_stop(0);
//  motor_stop(1);
//}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//pid here

void robot_move(int dir, uint16_t speed, uint32_t distance){
  float p_gain = 64;
  float i_gain = 0.001;
  int i_max = 32768;
  int i_min = -32768;
  int i_mem = 0;
  float d_gain = 0;
  float e_gain = 1;
  int d_mem = 1;
  
  int i_term = 0, d_term = 0, change = 0;
  uint32_t l_dist = 0, r_dist = 0;
  int error = 0;
  uint16_t rqspeed = speed;
  
  // Start the robot!
  encoders_reset();
  chassis_write(dir, dir, speed, speed);
  while(l_dist <= distance && r_dist <= distance)
  {
    l_dist = encoder_get(0,0);
    r_dist = encoder_get(1,0);
    error = (int) e_gain*(l_dist - r_dist);
    
    i_mem += error;
    if(i_mem > i_max){
      i_mem = i_max;
    }
    if(i_mem < i_min){
      i_mem = i_min;
    }
    i_term = (int) i_mem*i_gain;
    
    d_term = (int) d_gain*d_mem;
    d_mem = error;
    
    change = p_gain*(error + i_term - d_term);
    if(change > speed) change = speed;
    if(change < -speed) change = -speed;
    int l = speed-change;
    int r = speed+change;
    if(l > 65535){
      l = 65535;
      r += change;
    }
    if(r > 65535){
      r = 65535;
      l -= change;
    }
    chassis_write(dir, dir, l, r);
	
	if (!dir)
	{
		if (mctx_odct(0))
		{
			mctx_set_motor_pwm(0,0);
			mctx_set_motor_pwm(1,0);
		}else
		{
			chassis_write(dir, inv(dir), speed, speed);
		}
	}
    /*Serial1.print(l_dist);
    Serial1.print(" ");
    Serial1.print(r_dist);
    Serial1.print(" ");
    Serial1.print(l);
    Serial1.print(" ");
    Serial1.print(r);
    Serial1.print(" ");
    Serial1.print(i_mem);
    Serial1.print(" ");
    Serial1.println(change);*/
    printk("%d %d %hhx\n\r", encoder_get(0,0), encoder_get(1,0), mctx_odct(0));
  }
  chassis_stop(dir, speed);
}
int inv(int dir){
  if(dir == 0) return 1;
  else return 0;
}
void robot_rotate(int dir, int angle, int speed){
  int radius = radius_ref; // расстояние от центра вращения до середины колеса - в тиках энкодера
  int distance = radius * angle/57; // длина пути по окружности
  
  float p_gain = 64;
  float i_gain = 0.001;
  int i_max = 32768;
  int i_min = -32768;
  int i_mem = 0;
  float d_gain = 0;
  float e_gain = 1;
  int d_mem = 1;
  
  int i_term = 0, d_term = 0, change = 0;
  uint32_t l_dist = 0, r_dist = 0;
  int error = 0;
  int rqspeed = speed;
  
  // Start the robot!
  encoders_reset();
  chassis_write(dir, inv(dir), speed, speed);
  
  printk("Rotating robot: angle %d, direction %d, distance %d\n\r", angle, dir, distance);
  
  while(l_dist <= distance && r_dist <= distance){
    l_dist = encoder_get(0,0);
    r_dist = encoder_get(1,0);
    error = (int) e_gain*(l_dist - r_dist);
    
    if(((distance - l_dist) + (distance - r_dist)) >> 1 < (rqspeed >> 4))
    {
      speed -= 64;
    }
    
    i_mem += error;
    if(i_mem > i_max){
      i_mem = i_max;
    }
    if(i_mem < i_min){
      i_mem = i_min;
    }
    i_term = (int) i_mem*i_gain;
    
    d_term = (int) d_gain*d_mem;
    d_mem = error;
    
    change = p_gain*(error + i_term - d_term);
    if(change > speed) change = speed;
    if(change < -speed) change = -speed;
    int l = speed-change;
    int r = speed+change;
    if(l > 65535){
      l = 65535;
      r += change;
    }
    if(r > 65535){
      r = 65535;
      l -= change;
    }
    chassis_write(dir, inv(dir), l, r);
    printk("%d %d\n\r", l_dist, r_dist);
  }
  
  chassis_write(inv(dir), dir, 65535, 65535);
  mdelay(stop_delay);
  motor_stop(0);
  motor_stop(1);
}


int to_ticks(int mm){
  return (int) mm*29.4;
}
