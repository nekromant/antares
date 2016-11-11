/* Converted from original STC's include file via keil2sdcc.pl */

//--------------------------------------------------------------------------------
// New Generation 1T 8051 Series MCU Special Function Registers C51 Core SFRs
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
// new generation 1T 8051 series MCU system management special function registers
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
    0, the default PCA is at port P1
    1, PCA / PWM switch from P1 port to P4 port: ECI switch from P1.2 to P4.1 port,
                                                 PCA0 / PWM0 switches from P1.3 to P4.2
                                                 PCA1 / PWM1 switches from P1.4 to P4.3
SPI_P4:
    0, the default SPI in the P1 port
    1, SPI switch from P1 port to P4 port: SPICLK switch from P1.7 to P4.3 port
                                           MISO switches from P1.6 to P4.2
                                           MOSI switches from P1.5 to P4.1
                                           SS switches from P1.4 to P4.0
S2_P4: 
    0, the default UART2 in the P1 port
    1, UART2 switches from P1 port to P4 port: TxD2 switches from P1.3 to P4.3
                                               RxD2 switches from P1.2 to P4.2
GF2: General purpose flag

ADRJ:
    0, the upper 8 bits of the 10-bit A/D conversion result are placed in the ADC_RES register and the lower 2 bits are placed in the ADC_RESL register
    1, the upper 2 bits of the 10-bit A/D conversion result are placed in the lower 2 bits of the ADC_RES register and the lower 8 bits in the ADC_RESL register

DPS: 0, use the default data pointer DPTR0
     1, use another data pointer DPTR1
*/
//-----------------------------------
__sfr __at(0x8F) WAKE_CLKO; //Additional SFR WAK1_CLKO
#define __SFR_WAKE_CLKO 0x8F
/*
      7            6          5          4          3       2       1      0         Reset Value
   PCAWAKEUP  RXD_PIN_IE  T1_PIN_IE  T0_PIN_IE  LVD_WAKE    _    T1CLKO  T0CLKO      0000,0000B

b7 - PCAWAKEUP : PCA interrupt can wake up powerdown.
b6 - RXD_PIN_IE : wakeup powerdown when RI is set on the falling edge of P3.0 (RXD) (interrupt must be enabled).
b5 - T1_PIN_IE : wake up powerdown when the T1 interrupt flag is set on the falling edge of the T1 pin (must be enabled).
b4 - T0_PIN_IE : wake up powerdown when T0 interrupt flag is set on the falling edge of T0 pin (corresponding interrupt must be turned on).
b3 - LVD_WAKE : wake-up powerdown when LVD interrupt flag is set low by CMPIN pin (corresponding interrupt must be turned on).
b2 - 
b1 - T1CLKO : enable T1CKO (P3.5) pin output T1 overflow pulse, Fck1 = 1/2 T1 overflow rate
b0 - T0CLKO : enable T0CKO (P3.4) pin output T0 overflow pulse, Fck0 = 1/2 T1 overflow rate
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
//new generation 1T 8051 series MCU interrupt special function register
//Some interrupt control, interrupt flag bits scattered in other special function registers, these bits defined in the bit address
//Some of them have no bit addressing capability, please refer to the new generation 1T 8051 MCU Chinese Guide
//                                           7     6     5    4     3    2    1    0   Reset Value
#define __SFR_IE 0xA8
__sfr __at(0xA8) IE;  //Interrupt control register        EA  ELVD  EADC   ES   ET1  EX1  ET0  EX0  0x00,0000
//-----------------------
__sbit __at(__SFR_IE^7) EA;
__sbit __at(__SFR_IE^6) ELVD; //Low-voltage monitoring interrupt enable bit
__sbit __at(__SFR_IE^5) EADC; //ADC interrupt enable bit
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
__sfr __at(0xB8) IP; //Interrupt priority low      PPCA  PLVD  PADC  PS   PT1  PX1  PT0  PX0   0000,0000
#define __SFR_IP 0xB8
//--------
__sbit __at(__SFR_IP^7) PPCA;  //PCA Module interrupt priority
__sbit __at(__SFR_IP^6) PLVD;  //Low-voltage monitoring interrupt priority
__sbit __at(__SFR_IP^5) PADC;  //ADC Interrupt priority
__sbit __at(__SFR_IP^4) PS;
__sbit __at(__SFR_IP^3) PT1;
__sbit __at(__SFR_IP^2) PX1;
__sbit __at(__SFR_IP^1) PT0;
__sbit __at(__SFR_IP^0) PX0;
//-----------------------
//                                         7      6      5     4     3     2     1     0    Reset Value
__sfr __at(0xB7) IPH; //Interrupt priority high       PPCAH  PLVDH  PADCH  PSH  PT1H  PX1H  PT0H  PX0H   0000,0000
#define __SFR_IPH 0xB7
__sfr __at(0xB5) IP2; //                       -      -      -     -     -     -   PSPI   PS2   xxxx,xx00
#define __SFR_IP2 0xB5
__sfr __at(0xB6) IPH2; //                       -      -      -     -     -     -   PSPIH  PS2H  xxxx,xx00
#define __SFR_IPH2 0xB6
//-----------------------
//New Generation 1T 8051 Series MCU I / O Port Special Function Registers
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
//New Generation 1T 8051 Series Single - Chip Microcomputer Timers Special Function Registers
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
//New Generation 1T 8051 Series MCU Serial Port Special Function Registers
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
//New Generation 1T 8051 Series MCU Watchdog Timer Special Function Registers
__sfr __at(0xC1) WDT_CONTR; //Watch-Dog-Timer Control register
#define __SFR_WDT_CONTR 0xC1
//                                      7     6     5      4       3      2   1   0     Reset Value
//                                  WDT_FLAG  -  EN_WDT CLR_WDT IDLE_WDT PS2 PS1 PS0    xx00,0000
//-----------------------

//--------------------------------------------------------------------------------
//New Generation 1T 8051 Series Single Chip PCA / PWM Special Function Registers
//                                         7     6     5     4     3     2     1     0     Reset Value
__sfr __at(0xD8) CCON;   //PCA Control register.    CF    CR    -     -     -     -    CCF1  CCF0   00xx,xx00
#define __SFR_CCON 0xD8
//-----------------------
__sbit __at(__SFR_CCON^7) CF; //PCA counter overflow flag, set by hardware or software, must be cleared by software.
__sbit __at(__SFR_CCON^6) CR; //1: Enables PCA counter to count and must be cleared by software.
//-
//-
__sbit __at(__SFR_CCON^1) CCF1; //PCA module 1 interrupt flag, set by hardware, must be cleared by software.
__sbit __at(__SFR_CCON^0) CCF0; //PCA module 0 interrupt flag, set by hardware, must be cleared by software.
//-----------------------
__sfr __at(0xD9) CMOD; //PCA Operating mode register.   CIDL   -     -     -   CPS2   CPS1  CPS0  ECF   0xxx,x000
#define __SFR_CMOD 0xD9
/*
CIDL: Idle state PCA counter is to continue counting, 0: continue to count, 1: stop counting.

CPS2: PCA Counter pulse source selection bit 2.
CPS1: PCA Counter pulse source selection bit 1.
CPS0: PCA Counter pulse source selection bit 0.
   CPS2   CPS1   CPS0
    0      0      0    System clock frequency fosc/12.
    0      0      1    System clock frequency fosc/2.
    0      1      0    Timer0 overflow.
    0      1      1    External clock input from ECI / P3.4 pin, max fosc/2.
    1      0      0    System clock frequency,  Fosc/1
    1      0      1    System clock frequency/4,Fosc/4
    1      1      0    System clock frequency/6,Fosc/6
    1      1      1    System clock frequency/8,Fosc/8

ECF: PCA Counter Overflow Interrupt Enable bit, 1 - Enable CF (CCON.7) to generate an interrupt.
*/
//-----------------------
__sfr __at(0xE9) CL; //PCA Counter low                                                        0000,0000
#define __SFR_CL 0xE9
__sfr __at(0xF9) CH; //PCA Counter high                                                      0000,0000
#define __SFR_CH 0xF9
//-----------------------
//                                         7     6      5      4     3     2     1     0     Reset Value
__sfr __at(0xDA) CCAPM0; //PCA Module 0 PWM register  -   ECOM0  CAPP0  CAPN0  MAT0  TOG0  PWM0  ECCF0   x000,0000
#define __SFR_CCAPM0 0xDA
__sfr __at(0xDB) CCAPM1; //PCA Module 1 PWM register  -   ECOM1  CAPP1  CAPN1  MAT1  TOG1  PWM1  ECCF1   x000,0000
#define __SFR_CCAPM1 0xDB

//ECOMn = 1:Allows comparison.
//CAPPn = 1:Enables the rising edge to trigger the capture function.
//CAPNn = 1:Falling edge triggering is enabled.
//MATn  = 1:CCFn in CCON is asserted when a match condition occurs.
//TOGn  = 1:When a match occurs, CEXn transitions.
//PWMn  = 1:Set CEXn to PWM output.
//ECCFn = 1:Enable CCFn in CCON to trigger an interrupt.

//ECOMn  CAPPn  CAPNn  MATn  TOGn  PWMn  ECCFn
//  0      0      0     0     0     0     0   0x00   No features are enabled.
//  x      1      0     0     0     0     x   0x21   16-bit CEXn rising edge triggers the capture function.
//  x      0      1     0     0     0     x   0x11   16-bit CEXn falling edge triggers the capture function.
//  x      1      1     0     0     0     x   0x31   16-bit CEXn edge (upper and lower edge) triggers the capture function.
//  1      0      0     1     0     0     x   0x49   16-bit software timer.
//  1      0      0     1     1     0     x   0x4d   16-bit high-speed pulse output.
//  1      0      0     0     0     1     0   0x42   8-bit PWM.

//ECOMn  CAPPn  CAPNn  MATn  TOGn  PWMn  ECCFn
//  0      0      0     0     0     0     0   0x00   There is no such action
//  1      0      0     0     0     1     0   0x42   Ordinary 8-bit PWM, no interrupt
//  1      1      0     0     0     1     1   0x63   PWM output from low to high can generate interrupts
//  1      0      1     0     0     1     1   0x53   PWM output from high to low can generate an interrupt
//  1      1      1     0     0     1     1   0x73   PWM output from low to high or from high to low can generate interrupts

//-----------------------
__sfr __at(0xEA) CCAP0L; //PCA Module 0 lower 8 bits of the capture / compare register.                                    0000,0000
#define __SFR_CCAP0L 0xEA
__sfr __at(0xFA) CCAP0H; //PCA Module 0 upper 8 bits of the capture / compare register.                                    0000,0000
#define __SFR_CCAP0H 0xFA
__sfr __at(0xEB) CCAP1L; //PCA Module 1 lower 8 bits of the capture / compare register.                                    0000,0000
#define __SFR_CCAP1L 0xEB
__sfr __at(0xFB) CCAP1H; //PCA Module 1 upper 8 bits of the capture / compare register.                                    0000,0000
#define __SFR_CCAP1H 0xFB
//-----------------------
//                                                       7   6   5   4   3   2    1     0    Reset Value
__sfr __at(0xF2) PCA_PWM0; //PCA Module 0 PWM register.            -   -   -   -   -   -  EPC0H EPC0L   xxxx,xx00
#define __SFR_PCA_PWM0 0xF2
__sfr __at(0xF3) PCA_PWM1; //PCA Module 1 PWM register.            -   -   -   -   -   -  EPC1H EPC1L   xxxx,xx00
#define __SFR_PCA_PWM1 0xF3
//PCA_PWMn:    7      6      5      4      3      2      1      0
//             -      -      -      -      -      -    EPCnH  EPCnL
//B7-B2: Reserved
//B1(EPCnH): In PWM mode, 9 bits are formed with CCAPnH.
//B0(EPCnL): In PWM mode, 9 bits are formed with CCAPnL.
//--------------------------------------------------------------------------------
//New Generation 1T 8051 Series MCU Special Function Registers
//                                            7        6      5       4         3      2    1    0   Reset Value
__sfr __at(0xBC) ADC_CONTR; //A/D Conversion control register ADC_POWER SPEED1 SPEED0 ADC_FLAG ADC_START CHS2 CHS1 CHS0 0000,0000
#define __SFR_ADC_CONTR 0xBC
__sfr __at(0xBD) ADC_RES;  //A/D The conversion result is 8 bits high ADCV.9 ADCV.8 ADCV.7 ADCV.6 ADCV.5 ADCV.4 ADCV.3 ADCV.2	 0000,0000
#define __SFR_ADC_RES 0xBD
__sfr __at(0xBE) ADC_RESL;  //A/D Low conversion result 2 bits                                           ADCV.1 ADCV.0	 0000,0000
#define __SFR_ADC_RESL 0xBE
//--------------------------------------------------------------------------------
//New Generation 1T 8051 Series Single Chip Microcomputer SPI Special Function Registers
//                                      7     6     5     4     3     2     1     0    Reset Value
__sfr __at(0xCE) SPCTL; //SPI Control Register  SSIG  SPEN  DORD  MSTR  CPOL  CPHA  SPR1  SPR0  0000,0100
#define __SFR_SPCTL 0xCE
__sfr __at(0xCD) SPSTAT; //SPI Status Register   SPIF  WCOL   -     -     -     -     -     -    00xx,xxxx
#define __SFR_SPSTAT 0xCD
__sfr __at(0xCF) SPDAT; //SPI Data Register                                                     0000,0000
#define __SFR_SPDAT 0xCF
//--------------------------------------------------------------------------------
//New Generation 1T 8051 Series MCU IAP / ISP Special Function Registers
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
