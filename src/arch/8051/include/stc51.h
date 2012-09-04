/* Converted from original STC's include file via keil2sdcc.pl */

//--------------------------------------------------------------------------------
//新一代 1T 8051系列 单片机内核特殊功能寄存器 C51 Core SFRs
//                                          7     6      5       4     3    2    1     0   Reset Value
__sfr __at(0xE0) ACC; //Accumulator                                                              0000,0000
#define __SFR_ACC 0xE0
__sfr __at(0xF0) B; //B Register                                                               0000,0000
#define __SFR_B 0xF0
__sfr __at(0xD0) PSW; //Program Status Word      CY    AC    F0    RS1   RS0    OV    F1    P    0000,0000
#define __SFR_PSW 0xD0
//-----------------------------------
__sbit __at(__SFR_PSW^7) CY;
__sbit __at(__SFR_PSW^6) AC;
__sbit __at(__SFR_PSW^5) F0;
__sbit __at(__SFR_PSW^4) RS1;
__sbit __at(__SFR_PSW^3) RS0;
__sbit __at(__SFR_PSW^2) OV;
__sbit __at(__SFR_PSW^0) P;
//-----------------------------------
__sfr __at(0x81) SP; //Stack Pointer                                                            0000,0111
#define __SFR_SP 0x81
__sfr __at(0x82) DPL; //Data Pointer Low Byte                                                    0000,0000
#define __SFR_DPL 0x82
__sfr __at(0x83) DPH; //Data Pointer High Byte                                                   0000,0000
#define __SFR_DPH 0x83
//--------------------------------------------------------------------------------
//新一代 1T 8051系列 单片机系统管理特殊功能寄存器
//                                          7     6      5    4     3      2    1     0     Reset Value
__sfr __at(0x87) PCON; //Power Control        SMOD  SMOD0  LVDF  POF   GF1    GF0   PD   IDL    0001,0000
#define __SFR_PCON 0x87
//                                        7     6       5      4     3      2      1      0   Reset Value
__sfr __at(0x8E) AUXR; //Auxiliary Register  T0x12 T1x12 UART_M0x6 BRTR S2SMOD BRTx12 EXTRAM S1BRS  0000,0000
#define __SFR_AUXR 0x8E
//-----------------------------------
__sfr __at(0xA2) AUXR1; //Auxiliary Register 1  -  PCA_P4  SPI_P4  S2_P4  GF2    ADRJ   -    DPS  0000,0000
#define __SFR_AUXR1 0xA2
/*
PCA_P4:
    0, 缺省PCA 在P1 口
    1，PCA/PWM 从P1 口切换到P4 口: ECI 从P1.2 切换到P4.1 口，
                                   PCA0/PWM0 从P1.3 切换到P4.2 口
                                   PCA1/PWM1 从P1.4 切换到P4.3 口
SPI_P4:
    0, 缺省SPI 在P1 口
    1，SPI 从P1 口切换到P4 口: SPICLK 从P1.7 切换到P4.3 口
                               MISO 从P1.6 切换到P4.2 口
                               MOSI 从P1.5 切换到P4.1 口
                               SS 从P1.4 切换到P4.0 口
S2_P4: 
    0, 缺省UART2 在P1 口
    1，UART2 从P1 口切换到P4 口: TxD2 从P1.3 切换到P4.3 口
                                 RxD2 从P1.2 切换到P4.2 口
GF2: 通用标志位

ADRJ:
    0, 10 位A/D 转换结果的高8 位放在ADC_RES 寄存器, 低2 位放在ADC_RESL 寄存器
    1，10 位A/D 转换结果的最高2 位放在ADC_RES 寄存器的低2 位, 低8 位放在ADC_RESL 寄存器

DPS: 0, 使用缺省数据指针DPTR0
     1，使用另一个数据指针DPTR1
*/
//-----------------------------------
__sfr __at(0x8F) WAKE_CLKO; //附加的 SFR WAK1_CLKO
#define __SFR_WAKE_CLKO 0x8F
/*
      7            6          5          4          3       2       1      0         Reset Value
   PCAWAKEUP  RXD_PIN_IE  T1_PIN_IE  T0_PIN_IE  LVD_WAKE    _    T1CLKO  T0CLKO      0000,0000B

b7 - PCAWAKEUP : PCA 中断可唤醒 powerdown。
b6 - RXD_PIN_IE : 当 P3.0(RXD) 下降沿置位 RI 时可唤醒 powerdown(必须打开相应中断)。
b5 - T1_PIN_IE : 当 T1 脚下降沿置位 T1 中断标志时可唤醒 powerdown(必须打开相应中断)。
b4 - T0_PIN_IE : 当 T0 脚下降沿置位 T0 中断标志时可唤醒 powerdown(必须打开相应中断)。
b3 - LVD_WAKE : 当 CMPIN 脚低电平置位 LVD 中断标志时可唤醒 powerdown(必须打开相应中断)。
b2 - 
b1 - T1CLKO : 允许 T1CKO(P3.5) 脚输出 T1 溢出脉冲，Fck1 = 1/2 T1 溢出率
b0 - T0CLKO : 允许 T0CKO(P3.4) 脚输出 T0 溢出脉冲，Fck0 = 1/2 T1 溢出率
*/
//-----------------------------------
__sfr __at(0x97) CLK_DIV; //Clock Divder          -     -      -       -     -  CLKS2 CLKS1 CLKS0 xxxx,x000
#define __SFR_CLK_DIV 0x97
//-----------------------------------
__sfr __at(0xA1) BUS_SPEED; //Stretch register      -     -    ALES1   ALES0   -   RWS2  RWS1  RWS0 xx10,x011
#define __SFR_BUS_SPEED 0xA1
/*
ALES1 and ALES0:
00 : The P0 address setup time and hold time to ALE negative edge is one clock cycle
01 : The P0 address setup time and hold time to ALE negative edge is two clock cycles.
10 : The P0 address setup time and hold time to ALE negative edge is three clock cycles. (default)
11 : The P0 address setup time and hold time to ALE negative edge is four clock cycles.

RWS2,RWS1,RWS0:
  000 : The MOVX read/write pulse is 1 clock cycle. 
  001 : The MOVX read/write pulse is 2 clock cycles.
  010 : The MOVX read/write pulse is 3 clock cycles.
  011 : The MOVX read/write pulse is 4 clock cycles. (default)
  100 : The MOVX read/write pulse is 5 clock cycles.
  101 : The MOVX read/write pulse is 6 clock cycles.
  110 : The MOVX read/write pulse is 7 clock cycles.
  111 : The MOVX read/write pulse is 8 clock cycles.
*/
//--------------------------------------------------------------------------------
//新一代 1T 8051系列 单片机中断特殊功能寄存器
//有的中断控制、中断标志位散布在其它特殊功能寄存器中，这些位在位地址中定义
//其中有的位无位寻址能力，请参阅 新一代 1T 8051系列 单片机中文指南
//                                           7     6     5    4     3    2    1    0   Reset Value
__sfr __at(0xA8) IE;  //中断控制寄存器        EA  ELVD  EADC   ES   ET1  EX1  ET0  EX0  0x00,0000
#define __SFR_IE 0xA8
//-----------------------
__sbit __at(__SFR_IE^7) EA;
__sbit __at(__SFR_IE^6) ELVD; //低压监测中断允许位
__sbit __at(__SFR_IE^5) EADC; //ADC 中断允许位
__sbit __at(__SFR_IE^4) ES;
__sbit __at(__SFR_IE^3) ET1;
__sbit __at(__SFR_IE^2) EX1;
__sbit __at(__SFR_IE^1) ET0;
__sbit __at(__SFR_IE^0) EX0;
//-----------------------
__sfr __at(0xAF) IE2;  //Auxiliary Interrupt   -     -     -    -     -    -  ESPI  ES2  0000,0000B
#define __SFR_IE2 0xAF
//-----------------------
//                                          7     6     5    4    3    2    1    0    Reset Value
__sfr __at(0xB8) IP; //中断优先级低位      PPCA  PLVD  PADC  PS   PT1  PX1  PT0  PX0   0000,0000
#define __SFR_IP 0xB8
//--------
__sbit __at(__SFR_IP^7) PPCA;  //PCA 模块中断优先级
__sbit __at(__SFR_IP^6) PLVD;  //低压监测中断优先级
__sbit __at(__SFR_IP^5) PADC;  //ADC 中断优先级
__sbit __at(__SFR_IP^4) PS;
__sbit __at(__SFR_IP^3) PT1;
__sbit __at(__SFR_IP^2) PX1;
__sbit __at(__SFR_IP^1) PT0;
__sbit __at(__SFR_IP^0) PX0;
//-----------------------
//                                         7      6      5     4     3     2     1     0    Reset Value
__sfr __at(0xB7) IPH; //中断优先级高位       PPCAH  PLVDH  PADCH  PSH  PT1H  PX1H  PT0H  PX0H   0000,0000
#define __SFR_IPH 0xB7
__sfr __at(0xB5) IP2; //                       -      -      -     -     -     -   PSPI   PS2   xxxx,xx00
#define __SFR_IP2 0xB5
__sfr __at(0xB6) IPH2; //                       -      -      -     -     -     -   PSPIH  PS2H  xxxx,xx00
#define __SFR_IPH2 0xB6
//-----------------------
//新一代 1T 8051系列 单片机I/O 口特殊功能寄存器
//                                      7     6     5     4     3     2     1     0         Reset Value
__sfr __at(0x80) P0; //8 bitPort0          P0.7  P0.6  P0.5  P0.4  P0.3  P0.2  P0.1  P0.0       1111,1111
#define __SFR_P0 0x80
__sfr __at(0x94) P0M0; //                                                                         0000,0000
#define __SFR_P0M0 0x94
__sfr __at(0x93) P0M1; //                                                                         0000,0000
#define __SFR_P0M1 0x93
__sfr __at(0x90) P1; //8 bitPort1          P1.7  P1.6  P1.5  P1.4  P1.3  P1.2  P1.1  P1.0       1111,1111
#define __SFR_P1 0x90
__sfr __at(0x92) P1M0; //                                                                         0000,0000
#define __SFR_P1M0 0x92
__sfr __at(0x91) P1M1; //                                                                         0000,0000
#define __SFR_P1M1 0x91
__sfr __at(0x9D) P1ASF; //P1 analog special function
#define __SFR_P1ASF 0x9D
__sfr __at(0xA0) P2; //8 bitPort2          P2.7  P2.6  P2.5  P2.4  P2.3  P2.2  P2.1  P2.0       1111,1111
#define __SFR_P2 0xA0
__sfr __at(0x96) P2M0; //                                                                         0000,0000
#define __SFR_P2M0 0x96
__sfr __at(0x95) P2M1; //                                                                         0000,0000
#define __SFR_P2M1 0x95
__sfr __at(0xB0) P3; //8 bitPort3          P3.7  P3.6  P3.5  P3.4  P3.3  P3.2  P3.1  P3.0       1111,1111
#define __SFR_P3 0xB0
__sfr __at(0xB2) P3M0; //                                                                         0000,0000
#define __SFR_P3M0 0xB2
__sfr __at(0xB1) P3M1; //                                                                         0000,0000
#define __SFR_P3M1 0xB1
__sfr __at(0xC0) P4; //8 bitPort4          P4.7  P4.6  P4.5  P4.4  P4.3  P4.2  P4.1  P4.0       1111,1111
#define __SFR_P4 0xC0
__sfr __at(0xB4) P4M0; //                                                                         0000,0000
#define __SFR_P4M0 0xB4
__sfr __at(0xB3) P4M1; //                                                                         0000,0000
#define __SFR_P4M1 0xB3
//                                      7      6         5         4      3     2     1     0     Reset Value
__sfr __at(0xBB) P4SW; //Port-4 switch	    -   LVD_P4.6  ALE_P4.5  NA_P4.4   -     -     -     -	    x000,xxxx
#define __SFR_P4SW 0xBB

__sfr __at(0xC8) P5; //8 bitPort5           -     -       -      -    P5.3  P5.2  P5.1  P5.0    xxxx,1111
#define __SFR_P5 0xC8
__sfr __at(0xCA) P5M0; //                                                                         0000,0000
#define __SFR_P5M0 0xCA
__sfr __at(0xC9) P5M1; //                                                                         0000,0000
#define __SFR_P5M1 0xC9
//--------------------------------------------------------------------------------
//新一代 1T 8051系列 单片机定时器特殊功能寄存器
//                                          7     6     5     4     3     2     1     0     Reset Value
__sfr __at(0x88) TCON; //T0/T1 Control           TF1   TR1   TF0   TR0   IE1   IT1   IE0   IT0    0000,0000
#define __SFR_TCON 0x88
//-----------------------------------
__sbit __at(__SFR_TCON^7) TF1;
__sbit __at(__SFR_TCON^6) TR1;
__sbit __at(__SFR_TCON^5) TF0;
__sbit __at(__SFR_TCON^4) TR0;
__sbit __at(__SFR_TCON^3) IE1;
__sbit __at(__SFR_TCON^2) IT1;
__sbit __at(__SFR_TCON^1) IE0;
__sbit __at(__SFR_TCON^0) IT0;
//-----------------------------------
__sfr __at(0x89) TMOD; //T0/T1 Modes             GATE1 C/T1  M1_1  M1_0  GATE0 C/T0  M0_1  M0_0   0000,0000
#define __SFR_TMOD 0x89
__sfr __at(0x8A) TL0; //T0 Low Byte                                                              0000,0000
#define __SFR_TL0 0x8A
__sfr __at(0x8C) TH0; //T0 High Byte                                                             0000,0000
#define __SFR_TH0 0x8C
__sfr __at(0x8B) TL1; //T1 Low Byte                                                              0000,0000
#define __SFR_TL1 0x8B
__sfr __at(0x8D) TH1; //T1 High Byte                                                             0000,0000
#define __SFR_TH1 0x8D
//--------------------------------------------------------------------------------
//新一代 1T 8051系列 单片机串行口特殊功能寄存器
//                                          7     6     5     4     3     2     1     0     Reset Value
__sfr __at(0x98) SCON; //Serial Control         SM0/FE SM1   SM2   REN   TB8   RB8    TI    RI    0000,0000
#define __SFR_SCON 0x98
//-----------------------------------
__sbit __at(__SFR_SCON^7) SM0;  //SM0/FE
__sbit __at(__SFR_SCON^6) SM1;
__sbit __at(__SFR_SCON^5) SM2;
__sbit __at(__SFR_SCON^4) REN;
__sbit __at(__SFR_SCON^3) TB8;
__sbit __at(__SFR_SCON^2) RB8;
__sbit __at(__SFR_SCON^1) TI;
__sbit __at(__SFR_SCON^0) RI;
//-----------------------------------
__sfr __at(0x99) SBUF; //Serial Data Buffer                                                     xxxx,xxxx
#define __SFR_SBUF 0x99
__sfr __at(0xB9) SADEN; //Slave Address Mask                                                    0000,0000
#define __SFR_SADEN 0xB9
__sfr __at(0xA9) SADDR; //Slave Address                                                         0000,0000
#define __SFR_SADDR 0xA9
//-----------------------------------
//                                7      6      5      4      3      2     1     0        Reset Value
__sfr __at(0x9A) S2CON; //S2 Control  S2SM0  S2SM1  S2SM2  S2REN  S2TB8  S2RB8  S2TI  S2RI      00000000B
#define __SFR_S2CON 0x9A
__sfr __at(0x9B) S2BUF; //S2 Serial Buffer                                                      xxxx,xxxx
#define __SFR_S2BUF 0x9B
__sfr __at(0x9C) BRT; //S2 Baud-Rate Timer                                                    0000,0000
#define __SFR_BRT 0x9C
//--------------------------------------------------------------------------------
//新一代 1T 8051系列 单片机看门狗定时器特殊功能寄存器
__sfr __at(0xC1) WDT_CONTR; //Watch-Dog-Timer Control register
#define __SFR_WDT_CONTR 0xC1
//                                      7     6     5      4       3      2   1   0     Reset Value
//                                  WDT_FLAG  -  EN_WDT CLR_WDT IDLE_WDT PS2 PS1 PS0    xx00,0000
//-----------------------

//--------------------------------------------------------------------------------
//新一代 1T 8051系列 单片机PCA/PWM 特殊功能寄存器
//                                         7     6     5     4     3     2     1     0     Reset Value
__sfr __at(0xD8) CCON;   //PCA 控制寄存器。    CF    CR    -     -     -     -    CCF1  CCF0   00xx,xx00
#define __SFR_CCON 0xD8
//-----------------------
__sbit __at(__SFR_CCON^7) CF; //PCA计数器溢出标志,由硬件或软件置位,必须由软件清0。
__sbit __at(__SFR_CCON^6) CR; //1:允许 PCA 计数器计数, 必须由软件清0。
//-
//-
__sbit __at(__SFR_CCON^1) CCF1; //PCA 模块1 中断标志, 由硬件置位, 必须由软件清0。
__sbit __at(__SFR_CCON^0) CCF0; //PCA 模块0 中断标志, 由硬件置位, 必须由软件清0。
//-----------------------
__sfr __at(0xD9) CMOD; //PCA 工作模式寄存器。   CIDL   -     -     -   CPS2   CPS1  CPS0  ECF   0xxx,x000
#define __SFR_CMOD 0xD9
/*
CIDL: idle 状态时 PCA 计数器是否继续计数, 0: 继续计数, 1: 停止计数。

CPS2: PCA 计数器脉冲源选择位 2。
CPS1: PCA 计数器脉冲源选择位 1。
CPS0: PCA 计数器脉冲源选择位 0。
   CPS2   CPS1   CPS0
    0      0      0    系统时钟频率 fosc/12。
    0      0      1    系统时钟频率 fosc/2。
    0      1      0    Timer0 溢出。
    0      1      1    由 ECI/P3.4 脚输入的外部时钟，最大 fosc/2。
    1      0      0    系统时钟频率，  Fosc/1
    1      0      1    系统时钟频率/4，Fosc/4
    1      1      0    系统时钟频率/6，Fosc/6
    1      1      1    系统时钟频率/8，Fosc/8

ECF: PCA计数器溢出中断允许位, 1--允许 CF(CCON.7) 产生中断。
*/
//-----------------------
__sfr __at(0xE9) CL; //PCA 计数器低位                                                        0000,0000
#define __SFR_CL 0xE9
__sfr __at(0xF9) CH; //PCA 计数器高位                                                        0000,0000
#define __SFR_CH 0xF9
//-----------------------
//                                         7     6      5      4     3     2     1     0     Reset Value
__sfr __at(0xDA) CCAPM0; //PCA 模块0 PWM 寄存器  -   ECOM0  CAPP0  CAPN0  MAT0  TOG0  PWM0  ECCF0   x000,0000
#define __SFR_CCAPM0 0xDA
__sfr __at(0xDB) CCAPM1; //PCA 模块1 PWM 寄存器  -   ECOM1  CAPP1  CAPN1  MAT1  TOG1  PWM1  ECCF1   x000,0000
#define __SFR_CCAPM1 0xDB

//ECOMn = 1:允许比较功能。
//CAPPn = 1:允许上升沿触发捕捉功能。
//CAPNn = 1:允许下降沿触发捕捉功能。
//MATn  = 1:当匹配情况发生时, 允许 CCON 中的 CCFn 置位。
//TOGn  = 1:当匹配情况发生时, CEXn 将翻转。
//PWMn  = 1:将 CEXn 设置为 PWM 输出。
//ECCFn = 1:允许 CCON 中的 CCFn 触发中断。

//ECOMn  CAPPn  CAPNn  MATn  TOGn  PWMn  ECCFn
//  0      0      0     0     0     0     0   0x00   未启用任何功能。
//  x      1      0     0     0     0     x   0x21   16位CEXn上升沿触发捕捉功能。
//  x      0      1     0     0     0     x   0x11   16位CEXn下降沿触发捕捉功能。
//  x      1      1     0     0     0     x   0x31   16位CEXn边沿(上、下沿)触发捕捉功能。
//  1      0      0     1     0     0     x   0x49   16位软件定时器。
//  1      0      0     1     1     0     x   0x4d   16位高速脉冲输出。
//  1      0      0     0     0     1     0   0x42   8位 PWM。

//ECOMn  CAPPn  CAPNn  MATn  TOGn  PWMn  ECCFn
//  0      0      0     0     0     0     0   0x00   无此操作
//  1      0      0     0     0     1     0   0x42   普通8位PWM, 无中断
//  1      1      0     0     0     1     1   0x63   PWM输出由低变高可产生中断
//  1      0      1     0     0     1     1   0x53   PWM输出由高变低可产生中断
//  1      1      1     0     0     1     1   0x73   PWM输出由低变高或由高变低都可产生中断

//-----------------------
__sfr __at(0xEA) CCAP0L; //PCA 模块 0 的捕捉/比较寄存器低 8 位。                                    0000,0000
#define __SFR_CCAP0L 0xEA
__sfr __at(0xFA) CCAP0H; //PCA 模块 0 的捕捉/比较寄存器高 8 位。                                    0000,0000
#define __SFR_CCAP0H 0xFA
__sfr __at(0xEB) CCAP1L; //PCA 模块 1 的捕捉/比较寄存器低 8 位。                                    0000,0000
#define __SFR_CCAP1L 0xEB
__sfr __at(0xFB) CCAP1H; //PCA 模块 1 的捕捉/比较寄存器高 8 位。                                    0000,0000
#define __SFR_CCAP1H 0xFB
//-----------------------
//                                                       7   6   5   4   3   2    1     0    Reset Value
__sfr __at(0xF2) PCA_PWM0; //PCA 模块0 PWM 寄存器。            -   -   -   -   -   -  EPC0H EPC0L   xxxx,xx00
#define __SFR_PCA_PWM0 0xF2
__sfr __at(0xF3) PCA_PWM1; //PCA 模块1 PWM 寄存器。            -   -   -   -   -   -  EPC1H EPC1L   xxxx,xx00
#define __SFR_PCA_PWM1 0xF3
//PCA_PWMn:    7      6      5      4      3      2      1      0
//             -      -      -      -      -      -    EPCnH  EPCnL
//B7-B2: 保留
//B1(EPCnH): 在 PWM 模式下，与 CCAPnH 组成 9 位数。
//B0(EPCnL): 在 PWM 模式下，与 CCAPnL 组成 9 位数。
//--------------------------------------------------------------------------------
//新一代 1T 8051系列 单片机 ADC 特殊功能寄存器
//                                            7        6      5       4         3      2    1    0   Reset Value
__sfr __at(0xBC) ADC_CONTR; //A/D 转换控制寄存器 ADC_POWER SPEED1 SPEED0 ADC_FLAG ADC_START CHS2 CHS1 CHS0 0000,0000
#define __SFR_ADC_CONTR 0xBC
__sfr __at(0xBD) ADC_RES;  //A/D 转换结果高8位 ADCV.9 ADCV.8 ADCV.7 ADCV.6 ADCV.5 ADCV.4 ADCV.3 ADCV.2	 0000,0000
#define __SFR_ADC_RES 0xBD
__sfr __at(0xBE) ADC_RESL;  //A/D 转换结果低2位                                           ADCV.1 ADCV.0	 0000,0000
#define __SFR_ADC_RESL 0xBE
//--------------------------------------------------------------------------------
//新一代 1T 8051系列 单片机 SPI 特殊功能寄存器
//                                      7     6     5     4     3     2     1     0    Reset Value
__sfr __at(0xCE) SPCTL; //SPI Control Register  SSIG  SPEN  DORD  MSTR  CPOL  CPHA  SPR1  SPR0  0000,0100
#define __SFR_SPCTL 0xCE
__sfr __at(0xCD) SPSTAT; //SPI Status Register   SPIF  WCOL   -     -     -     -     -     -    00xx,xxxx
#define __SFR_SPSTAT 0xCD
__sfr __at(0xCF) SPDAT; //SPI Data Register                                                     0000,0000
#define __SFR_SPDAT 0xCF
//--------------------------------------------------------------------------------
//新一代 1T 8051系列 单片机 IAP/ISP 特殊功能寄存器
__sfr __at(0xC2) IAP_DATA;
#define __SFR_IAP_DATA 0xC2
__sfr __at(0xC3) IAP_ADDRH;
#define __SFR_IAP_ADDRH 0xC3
__sfr __at(0xC4) IAP_ADDRL;
#define __SFR_IAP_ADDRL 0xC4
//                                                7    6    5      4    3    2    1     0    Reset Value
__sfr __at(0xC5) IAP_CMD; //IAP Mode Table          0    -    -      -    -    -   MS1   MS0   0xxx,xx00
#define __SFR_IAP_CMD 0xC5
__sfr __at(0xC6) IAP_TRIG;
#define __SFR_IAP_TRIG 0xC6
__sfr __at(0xC7) IAP_CONTR; //IAP Control Register  IAPEN SWBS SWRST CFAIL  -   WT2  WT1   WT0   0000,x000
#define __SFR_IAP_CONTR 0xC7
//--------------------------------------------------------------------------------
