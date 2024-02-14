/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Auto-generated by STM32CubeIDE
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

#include <stdint.h>
#include <stdbool.h>
#include "main.h"

int count = 0;


#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

void init_portC(void)			/* не меняем */
{
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN; /*вкл тактирование порта с*/

	GPIOC->CRL &= ~(GPIO_CRL_CNF0|GPIO_CRL_MODE0); /*зануляем биты CRL (0000)*/
	GPIOC->CRL |= GPIO_CRL_MODE0; /*ставим маску 1100 - 50MHz analog mode*/

	GPIOC->CRL &= ~(GPIO_CRL_CNF1|GPIO_CRL_MODE1);
	GPIOC->CRL |= GPIO_CRL_MODE1;

	GPIOC->CRL &= ~(GPIO_CRL_CNF2|GPIO_CRL_MODE2);
	GPIOC->CRL |= GPIO_CRL_MODE2;

	GPIOC->CRL &= ~(GPIO_CRL_CNF3|GPIO_CRL_MODE3);
	GPIOC->CRL |= GPIO_CRL_MODE3;

	GPIOC->CRL &= ~(GPIO_CRL_CNF4|GPIO_CRL_MODE4);
	GPIOC->CRL |= GPIO_CRL_MODE4;

	GPIOC->CRL &= ~(GPIO_CRL_CNF5|GPIO_CRL_MODE5);
	GPIOC->CRL |= GPIO_CRL_MODE5;

	GPIOC->CRL &= ~(GPIO_CRL_CNF6|GPIO_CRL_MODE6);
	GPIOC->CRL |= GPIO_CRL_MODE6;

	GPIOC->CRL &= ~(GPIO_CRL_CNF7|GPIO_CRL_MODE7);
	GPIOC->CRL |= GPIO_CRL_MODE7;
}



void TIM2_IRQHandler(void)
{
	TIM2->SR &= ~TIM_SR_UIF;			//Сброс флага переполнения

	if (count == 15) {
		count = 0;
	}
	else count ++;
}




/* void TIM2_IRQHandler(void) // не меняем + задаем счетчик, кот. инкрементируется каждую секунду
{
	TIM2->SR &= ~TIM_SR_UIF;			//Сброс флага переполнения


}
*/

/**
  * @brief  Подпрограмма обработчика прерывания
  *			по внешнему импульсу на выводах (EXTI15-EXTI10)
  *			Нас интересует EXTI13, там висит кнопка.
  * @param  None
  * @retval None
  */


void EXTI15_10_IRQHandler(void)
{
	// Т.к. этот обработчик вызывается, если произшло одно из прерываний EXTI15-EXI10,
	// нужно проверить, кто из них его вызвал.
	if (EXTI->PR & EXTI_PR_PR13) 		// нас интересует EXTI13
	{
		EXTI->PR |= EXTI_PR_PR13;
		delay(10000);					//Задержка для защиты от дребезга контактов
		TIM2->CR1 ^= TIM_CR1_CEN;		//Инвертируем состояние таймера
	}
}



void initButton(void)
{
	//Включить тактирование порта GPIOC и альтернативных функций
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN | RCC_APB2ENR_AFIOEN;

	/* Настраиваем PC13 на вход, альтернативная функция */
	// Сбрасываем биты конфигурации порта...
	GPIOC->CRH &= ~(GPIO_CRH_MODE13 | GPIO_CRH_CNF13);
	//...и выставляем так, как нам нужно
	GPIOC->CRH |= GPIO_CRH_CNF13_1;			//Вход с подтяжкой
	GPIOC->BSRR |= GPIO_BSRR_BS13;			//Подтяжка к Vdd

	/* Настройка самого прерывания */

	// Настройка альтернативных фукнций портов.
	// Настройки портов с 12 по 15 хранятся в регистре AFIO_EXTICR4.
	// Регистры объединены в массив AFIO->EXTICR, нумерация массива начинается с нулевого элемента.
	// Поэтому настройки AFIO_EXTICR4 хранятся в AFIO->EXTICR[3]
	AFIO->EXTICR[3] |= AFIO_EXTICR4_EXTI13_PC;

	EXTI->FTSR |= EXTI_FTSR_TR13;			//Прерывание по спаду импульса (при нажатии на кнопку)
	EXTI->IMR |= EXTI_IMR_MR13;				//Выставляем маску - EXTI13

	NVIC_EnableIRQ(EXTI15_10_IRQn);			//Разрешаем прерывание
	NVIC_SetPriority(EXTI15_10_IRQn, 0);	//Выставляем приоритет
}



void initTIM2(void)
{
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;		//Включить тактирование TIM6

	//Частота APB1 для таймеров = APB1Clk * 2 = 32МГц * 2 = 64МГц
	TIM2->PSC = 64000-1;					//Предделитель частоты (64МГц/64000 = 1кГц)
	TIM2->ARR = 1000-1;						//Модуль счёта таймера (1кГц/1000 = 1с)
	TIM2->DIER |= TIM_DIER_UIE;				//Разрешить прерывание по переполнению таймера
	TIM2->CR1 |= TIM_CR1_CEN;				//Включить таймер

	NVIC_EnableIRQ(TIM2_IRQn);				//Рарзрешить прерывание от TIM2
	NVIC_SetPriority(TIM2_IRQn, 1);			//Выставляем приоритет
}


                                    /* не меняем */
void initClk(void)
{
	// Enable HSI
	RCC->CR |= RCC_CR_HSION;
	while(!(RCC->CR & RCC_CR_HSIRDY)){};

	// Enable Prefetch Buffer
	FLASH->ACR |= FLASH_ACR_PRFTBE;

	// Flash 2 wait state
	FLASH->ACR &= ~FLASH_ACR_LATENCY;
	FLASH->ACR |= FLASH_ACR_LATENCY_2;

	// HCLK = SYSCLK
	RCC->CFGR |= RCC_CFGR_HPRE_DIV1;

	// PCLK2 = HCLK
	RCC->CFGR |= RCC_CFGR_PPRE2_DIV1;

	// PCLK1 = HCLK
	RCC->CFGR |= RCC_CFGR_PPRE1_DIV2;

	// PLL configuration: PLLCLK = HSI/2 * 16 = 64 MHz
	RCC->CFGR &= ~RCC_CFGR_PLLSRC;
	RCC->CFGR |= RCC_CFGR_PLLMULL16;

	// Enable PLL
	RCC->CR |= RCC_CR_PLLON;

	// Wait till PLL is ready
	while((RCC->CR & RCC_CR_PLLRDY) == 0) {};

	// Select PLL as system clock source
	RCC->CFGR &= ~RCC_CFGR_SW;
	RCC->CFGR |= RCC_CFGR_SW_PLL;

	// Wait till PLL is used as system clock source
	while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL){};
}



int main(void)
{
	init_portC();
	initClk();
	initButton();
	initTIM2();
	while(true) {
        if (count == 0) {
            reset();
            disp_0();
        }

        else if (count == 1) {
        	reset();
        	disp_1();
            //reset ();
        }

        else if (count == 2) {
        	reset();
        	disp_2();
            //reset ();
        }

        else if (count == 3) {
        	reset();
        	disp_3();
            //reset ();
        }

        else if (count == 4) {
        	reset();
        	disp_4();
        	//reset ();
        }

        else if (count == 5) {
        	reset();
        	disp_5();
        	//reset ();
        }

        else if (count == 6) {
        	reset();
        	disp_6();
        	//reset ();
        }

        else if (count == 7) {
        	reset();
        	disp_7();
        	//reset ();
        }

        else if (count == 8) {
        	reset();
        	disp_8();
        	//reset ();
        }

        else if (count == 9) {
        	reset();
        	disp_9();
        	//reset ();
        }

        else if (count == 10) {
        	reset();
        	disp_10();
        	//reset ();
        }

        else if (count == 11) {
        	reset();
        	disp_11();
        	//reset ();
        }

        else if (count == 12) {
        	reset();
        	disp_12();
        	//reset ();
        }

        else if (count == 13) {
        	reset();
        	disp_13();
        	//reset ();
        }

        else if (count == 14) {
        	reset();
        	disp_14();
        	//reset ();
        }

        else if (count == 15) {
        	reset();
        	disp_15();
        	//reset ();
        }

	}
}

void delay(uint32_t value)
{
	for (uint32_t i=0;i<value;i++);
}
