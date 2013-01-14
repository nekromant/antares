#ifndef __BDEF_H
#define __BDEF_H


#define GPIO_DIR_REG            0xbfe00c02
#define GPIO_STATE_REG          0xbfe00b00
#define LED0_MASK               0x200
#define LED1_MASK               0x400
#define LED2_MASK               0x800
#define LED3_MASK               0x1000
#define LED4_MASK               0x2000
#define LED5_MASK               0x4000
#define LED6_MASK               0x8000
#define LED7_MASK               0x80    /* wtf? */
#define MISSED_LED_MASK         0x100

#define BUTTONS_STATE_REG       0xbff70002
#define BUTTON0_MASK            0x8
#define BUTTON1_MASK            0x10
#define BUTTON2_MASK            0x20
#define BUTTON3_MASK            0x40
#define BUTTON4_MASK            0x80


#define GPIO_DIR    *(volatile uint16_t*) GPIO_DIR_REG
#define GPIO_STATE  *(volatile uint16_t*) GPIO_STATE_REG
#define BTN_STATE   *(volatile uint16_t*) BUTTONS_STATE_REG

#endif
