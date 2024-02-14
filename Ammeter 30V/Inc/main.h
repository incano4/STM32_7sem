#ifndef __MAIN_H
#define	__MAIN_H

#include "stm32f1xx.h"

//#define reset()		GPIOA->ODR &= ~(GPIO_ODR_ODR0|GPIO_ODR_ODR1|GPIO_ODR_ODR4|GPIO_ODR_ODR5|GPIO_ODR_ODR6|GPIO_ODR_ODR7|GPIO_ODR_ODR8);
//#define count_0()	GPIOA->ODR |= (GPIO_ODR_ODR1|GPIO_ODR_ODR4|GPIO_ODR_ODR5|GPIO_ODR_ODR6|GPIO_ODR_ODR7|GPIO_ODR_ODR8);
////#define count_1()	GPIOA->ODR |= (GPIO_ODR_ODR0|GPIO_ODR_ODR5);
////#define count_1()	GPIOA->ODR |= (GPIO_ODR_ODR6|GPIO_ODR_ODR7);
//#define count_1()	GPIOA->ODR = 0xc0;
//#define count_2()	GPIOA->ODR |= (GPIO_ODR_ODR0|GPIO_ODR_ODR4|GPIO_ODR_ODR5|GPIO_ODR_ODR7|GPIO_ODR_ODR8);
//#define count_3()	GPIOA->ODR |= (GPIO_ODR_ODR0|GPIO_ODR_ODR5|GPIO_ODR_ODR6|GPIO_ODR_ODR7|GPIO_ODR_ODR8);
//#define count_4()	GPIOA->ODR |= (GPIO_ODR_ODR0|GPIO_ODR_ODR1|GPIO_ODR_ODR6|GPIO_ODR_ODR7);
//#define count_5()	GPIOA->ODR |= (GPIO_ODR_ODR0|GPIO_ODR_ODR1|GPIO_ODR_ODR5|GPIO_ODR_ODR6|GPIO_ODR_ODR8);
//#define count_6()	GPIOA->ODR |= (GPIO_ODR_ODR0|GPIO_ODR_ODR1|GPIO_ODR_ODR4|GPIO_ODR_ODR5|GPIO_ODR_ODR6|GPIO_ODR_ODR8);
//#define count_7()	GPIOA->ODR |= (GPIO_ODR_ODR6|GPIO_ODR_ODR7|GPIO_ODR_ODR8);
//#define count_8()	GPIOA->ODR |= (GPIO_ODR_ODR0|GPIO_ODR_ODR1|GPIO_ODR_ODR4|GPIO_ODR_ODR5|GPIO_ODR_ODR6|GPIO_ODR_ODR7|GPIO_ODR_ODR8);
//#define count_9()	GPIOA->ODR |= (GPIO_ODR_ODR0|GPIO_ODR_ODR1|GPIO_ODR_ODR5|GPIO_ODR_ODR6|GPIO_ODR_ODR7|GPIO_ODR_ODR8);
////#define reset_dot()	GPIOA->ODR &= ~(GPIO_ODR_ODR13);
////#define dot()	GPIOA->BSRR |= (GPIO_BSRR_BS13);
//#define dot()	GPIOA->ODR |= (GPIO_ODR_ODR13);

//#define reset()		GPIOC->ODR &= ~(GPIO_ODR_ODR0|GPIO_ODR_ODR1|GPIO_ODR_ODR2|GPIO_ODR_ODR3|GPIO_ODR_ODR4|GPIO_ODR_ODR5|GPIO_ODR_ODR6|GPIO_ODR_ODR7);
//#define disp_0()	GPIOC->ODR |= (GPIO_ODR_ODR0|GPIO_ODR_ODR1|GPIO_ODR_ODR2|GPIO_ODR_ODR3|GPIO_ODR_ODR4|GPIO_ODR_ODR5);
//#define disp_1()	GPIOC->ODR |= (GPIO_ODR_ODR0|GPIO_ODR_ODR5);
//#define disp_2()	GPIOC->ODR |= (GPIO_ODR_ODR0|GPIO_ODR_ODR1|GPIO_ODR_ODR3|GPIO_ODR_ODR4|GPIO_ODR_ODR6);
//#define disp_3()	GPIOC->ODR |= (GPIO_ODR_ODR0|GPIO_ODR_ODR1|GPIO_ODR_ODR4|GPIO_ODR_ODR5|GPIO_ODR_ODR6);
//#define disp_4()	GPIOC->ODR |= (GPIO_ODR_ODR0|GPIO_ODR_ODR2|GPIO_ODR_ODR5|GPIO_ODR_ODR6);
//#define disp_5()	GPIOC->ODR |= (GPIO_ODR_ODR1|GPIO_ODR_ODR2|GPIO_ODR_ODR4|GPIO_ODR_ODR5|GPIO_ODR_ODR6);
//#define disp_6()	GPIOC->ODR |= (GPIO_ODR_ODR1|GPIO_ODR_ODR2|GPIO_ODR_ODR3|GPIO_ODR_ODR4|GPIO_ODR_ODR5|GPIO_ODR_ODR6);
//#define disp_7()	GPIOC->ODR |= (GPIO_ODR_ODR0|GPIO_ODR_ODR1|GPIO_ODR_ODR5);
//#define disp_8()	GPIOC->ODR |= (GPIO_ODR_ODR0|GPIO_ODR_ODR1|GPIO_ODR_ODR2|GPIO_ODR_ODR3|GPIO_ODR_ODR4|GPIO_ODR_ODR5|GPIO_ODR_ODR6);
//#define disp_9()	GPIOC->ODR |= (GPIO_ODR_ODR0|GPIO_ODR_ODR1|GPIO_ODR_ODR2|GPIO_ODR_ODR4|GPIO_ODR_ODR5|GPIO_ODR_ODR6);
//#define dot()	GPIOA->ODR |= (GPIO_ODR_ODR13);

//#define reset()		GPIOC->ODR = 0x0;
//#define disp_0()	GPIOC->ODR |= (GPIO_ODR_ODR0|GPIO_ODR_ODR1|GPIO_ODR_ODR2|GPIO_ODR_ODR3|GPIO_ODR_ODR4|GPIO_ODR_ODR5);
//#define disp_1()	GPIOC->ODR |= (GPIO_ODR_ODR0|GPIO_ODR_ODR5);
//#define disp_2()	GPIOC->ODR |= (GPIO_ODR_ODR0|GPIO_ODR_ODR1|GPIO_ODR_ODR3|GPIO_ODR_ODR4|GPIO_ODR_ODR6);
//#define disp_3()	GPIOC->ODR |= (GPIO_ODR_ODR0|GPIO_ODR_ODR1|GPIO_ODR_ODR4|GPIO_ODR_ODR5|GPIO_ODR_ODR6);
//#define disp_4()	GPIOC->ODR |= (GPIO_ODR_ODR0|GPIO_ODR_ODR2|GPIO_ODR_ODR5|GPIO_ODR_ODR6);
//#define disp_5()	GPIOC->ODR |= (GPIO_ODR_ODR1|GPIO_ODR_ODR2|GPIO_ODR_ODR4|GPIO_ODR_ODR5|GPIO_ODR_ODR6);
//#define disp_6()	GPIOC->ODR |= (GPIO_ODR_ODR1|GPIO_ODR_ODR2|GPIO_ODR_ODR3|GPIO_ODR_ODR4|GPIO_ODR_ODR5|GPIO_ODR_ODR6);
//#define disp_7()	GPIOC->ODR |= (GPIO_ODR_ODR0|GPIO_ODR_ODR1|GPIO_ODR_ODR5);
//#define disp_8()	GPIOC->ODR |= (GPIO_ODR_ODR0|GPIO_ODR_ODR1|GPIO_ODR_ODR2|GPIO_ODR_ODR3|GPIO_ODR_ODR4|GPIO_ODR_ODR5|GPIO_ODR_ODR6);
//#define disp_9()	GPIOC->ODR |= (GPIO_ODR_ODR0|GPIO_ODR_ODR1|GPIO_ODR_ODR2|GPIO_ODR_ODR4|GPIO_ODR_ODR5|GPIO_ODR_ODR6);
//#define dot()	GPIOA->ODR |= (GPIO_ODR_ODR13);

#define reset()   GPIOC->ODR = 0x0;
#define disp_0()  GPIOC->ODR = 0x3F;  //0011 1111
#define disp_1()  GPIOC->ODR = 0x21;  //0010 0001
#define disp_2()  GPIOC->ODR = 0x5b;  //0101 1011
#define disp_3()  GPIOC->ODR = 0x73;  //0111 0011
#define disp_4()  GPIOC->ODR = 0x65;  //0110 0101
#define disp_5()  GPIOC->ODR = 0x76;  //0111 0110
#define disp_6()  GPIOC->ODR = 0x7e;  //0111 1110
#define disp_7()  GPIOC->ODR = 0x23;  //0010 0011
#define disp_8()  GPIOC->ODR = 0x7f;  //0111 1111
#define disp_9()  GPIOC->ODR = 0x77;  //0111 0111
#define dot()	  GPIOB->ODR = 0x01;  //0000 0001

#define indicator_reset() GPIOB->ODR = 0xe;
#define indicator_0()	GPIOB->ODR = 0xc;
#define indicator_1()	GPIOB->ODR = 0xa;
#define indicator_2()	GPIOB->ODR = 0x6;

//#define indicator_reset() GPIOA->ODR |= (GPIO_ODR_ODR9|GPIO_ODR_ODR10|GPIO_ODR_ODR11);
//#define indicator_0()	GPIOA->ODR &= ~(GPIO_ODR_ODR9);
//#define indicator_1()	GPIOA->ODR &= ~(GPIO_ODR_ODR10);
//#define indicator_2()	GPIOA->ODR &= ~(GPIO_ODR_ODR11);



void init_portC(void);
void init_portB(void);
void init_ADC(void);
void disp(int result);
void indicator(int i);
uint16_t ADC_proc(uint8_t n);
void delay(uint32_t value);


//uint16_t Read_ADC(uint8_t n);
//void count(int result);
//void display(int i);
//void led_ports_init(void);
//void init_ADC(void);
//void delay(uint32_t value);

#endif
