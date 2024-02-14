
#include <stdint.h>
#include <stdbool.h>
#include "main.h"

char RxBuffer[RX_BUFF_SIZE];					//Буфер приёма USART
char TxBuffer[TX_BUFF_SIZE];					//Буфер передачи USART
bool ComReceived;								//Флаг приёма строки данных

int count = 0;

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

void USART2_IRQHandler(void) {
	if ((USART2->SR & USART_SR_RXNE)!=0) {		//Прерывание по приёму данных
		uint8_t pos = strlen(RxBuffer);			//Вычисляем позицию свободной ячейки
		RxBuffer[pos] = USART2->DR;				//Считываем содержимое регистра данных

		if ((RxBuffer[pos]== 0x0A) && (RxBuffer[pos-1]== 0x0D))	{						//Если это символ конца строки
			ComReceived = true;					//- выставляем флаг приёма строки
			return;								//- и выходим
		}
	}
}

void init_portC(void) {
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

void TIM2_IRQHandler(void) {
	TIM2->SR &= ~TIM_SR_UIF;			//Сброс флага переполнения

	if (count == 15) {
		count = 0;
	}
	else count ++;
}

void EXTI15_10_IRQHandler(void) {
	// Т.к. этот обработчик вызывается, если произшло одно из прерываний EXTI15-EXI10,
	// нужно проверить, кто из них его вызвал.
	if (EXTI->PR & EXTI_PR_PR13) { 		// нас интересует EXTI13

		EXTI->PR |= EXTI_PR_PR13;
		delay(10000);					//Задержка для защиты от дребезга контактов
		TIM2->CR1 ^= TIM_CR1_CEN;		//Инвертируем состояние таймера
	}
}

void initButton(void) {
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

void initTIM2(void) {
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;		//Включить тактирование TIM6

	//Частота APB1 для таймеров = APB1Clk * 2 = 32МГц * 2 = 64МГц
	TIM2->PSC = 64000-1;					//Предделитель частоты (64МГц/64000 = 1кГц)
	TIM2->ARR = 1000-1;						//Модуль счёта таймера (1кГц/1000 = 1с)
	TIM2->DIER |= TIM_DIER_UIE;				//Разрешить прерывание по переполнению таймера
	TIM2->CR1 |= TIM_CR1_CEN;				//Включить таймер

	NVIC_EnableIRQ(TIM2_IRQn);				//Рарзрешить прерывание от TIM2
	NVIC_SetPriority(TIM2_IRQn, 1);			//Выставляем приоритет
}

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

void initUSART2(void) {
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;						//включить тактирование альтернативных ф-ций портов
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;					//включить тактирование UART2

	GPIOA->CRL &= ~(GPIO_CRL_MODE2 | GPIO_CRL_CNF2);		//PA2 на выход
	GPIOA->CRL |= (GPIO_CRL_MODE2_1 | GPIO_CRL_CNF2_1);

	GPIOA->CRL &= ~(GPIO_CRL_MODE3 | GPIO_CRL_CNF3);		//PA3 - вход
	GPIOA->CRL |= GPIO_CRL_CNF3_0;

	/*****************************************
	Скорость передачи данных - 115200
	Частота шины APB1 - 32МГц

	1. USARTDIV = 32'000'000/(16*9600) = 208.3
	2. 208 = 0xD0
	3. 16*0.3 = 4.8
	4. Итого 0xD04
	*****************************************/
	USART2->BRR = 0xD04;

	USART2->CR1 |= USART_CR1_RE | USART_CR1_TE | USART_CR1_UE;
	USART2->CR1 |= USART_CR1_RXNEIE;						//разрешить прерывание по приему байта данных

	NVIC_EnableIRQ(USART2_IRQn);
}

void txStr(char *str, bool crlf) {
	uint16_t i;

	if (crlf)												//если просят,
		strcat(str,"\r\n");									//добавляем символ конца строки

	for (i = 0; i < strlen(str); i++) {
		USART2->DR = str[i];								//передаём байт данных
		while ((USART2->SR & USART_SR_TC)==0) {};			//ждём окончания передачи
	}
}

void ExecuteCommand(void) {
//	txStr(RxBuffer, false);
	memset(TxBuffer,0,sizeof(TxBuffer));					//Очистка буфера передачи

	/* Обработчик команд */
	if (strncmp(RxBuffer,"*IDN?",5) == 0)	{				//Это команда "*IDN?"
		//Она самая, возвращаем строку идентификации
			strcpy(TxBuffer,"Dychakovskaya A.A. IU4-72");
	}

	else if (strncmp(RxBuffer,"START",5) == 0)	 {			//Команда запуска таймера?
		TIM2->CR1 |= TIM_CR1_CEN;
		strcpy(TxBuffer, "OK");
	}

	else if (strncmp(RxBuffer,"STOP",4) == 0)	{			//Команда остановки таймера?
		TIM2->CR1 &= ~TIM_CR1_CEN;
		strcpy(TxBuffer, "OK");
	}


	else if (strncmp(RxBuffer,"PERIOD?",7) == 0) {				//Команда остановки таймера?
		TIM2->CR1 &= ~TIM_CR1_CEN;
		sprintf(TxBuffer, "%ld",TIM2->ARR);
		}


	else if (strncmp(RxBuffer,"PERIOD",6) == 0)	{			//Команда изменения периода таймера?
		uint16_t tim_value;
		sscanf(RxBuffer,"%*s %hu", &tim_value);				//преобразуем строку в целое число

		if ((100 <= tim_value) && (tim_value <= 5000))		//параметр должен быть в заданных пределах!
		{
			TIM2->ARR = tim_value;
			TIM2->CNT = 0;

			strcpy(TxBuffer, "OK");
		}

		else
			strcpy(TxBuffer, "Parameter is out of range");	//ругаемся
	}










	else if (strncmp(RxBuffer,"SET",3) == 0)	{			//Команда изменения периода таймера?
		short count;
		sscanf(RxBuffer,"%*s %hx", &count);				//преобразуем строку в целое число


	  if (count == 0x01)
	  {
        reset();
	   disp_1();         //вывести значение на семисегментник
	   strcpy(TxBuffer, "OK");
	  }
	  else if (count == 0x02)
	  {
          reset();
		  disp_2();         //вывести значение на семисегментник
	   strcpy(TxBuffer, "OK");
	  }

	  else if (count == 0x03)
	  {
        reset();
		  disp_3();         //вывести значение на семисегментник
	   strcpy(TxBuffer, "OK");
	  }

	  else if (count == 0x04)
	  {
        reset();
		  disp_4();         //вывести значение на семисегментник
	   strcpy(TxBuffer, "OK");
	  }

	  else if (count == 0x05)
	  {
        reset();
		  disp_5();         //вывести значение на семисегментник
	   strcpy(TxBuffer, "OK");
	  }

    	  else if (count == 0x06)
	  {
        reset();
		  disp_6();         //вывести значение на семисегментник
	   strcpy(TxBuffer, "OK");
	  }


	  else if (count == 0x07)
	  {
        reset();
		  disp_7();         //вывести значение на семисегментник
	   strcpy(TxBuffer, "OK");
	  }



	  else if (count == 0x08)
	  {
        reset();
		  disp_8();         //вывести значение на семисегментник
	   strcpy(TxBuffer, "OK");
	  }



	  else if (count == 0x09)
	  {
        reset();
		  disp_9();         //вывести значение на семисегментник
	   strcpy(TxBuffer, "OK");
	  }


	  else if (count == 0x0A)
	  {
        reset();
		  disp_10();         //вывести значение на семисегментник
	   strcpy(TxBuffer, "OK");
	  }


	  else if (count == 0x0B)
	  {
        reset();
		  disp_11();         //вывести значение на семисегментник
	   strcpy(TxBuffer, "OK");
	  }


      	  else if (count == 0x0C)
	  {
        reset();
		  disp_12();         //вывести значение на семисегментник
	   strcpy(TxBuffer, "OK");
	  }


      	  else if (count == 0x0D)
	  {
        reset();
		  disp_13();         //вывести значение на семисегментник
	   strcpy(TxBuffer, "OK");
	  }


      	  else if (count == 0x0E)
	  {
        reset();
		  disp_14();         //вывести значение на семисегментник
	   strcpy(TxBuffer, "OK");
	  }

      	  else if (count == 0x0F)
	  {
        reset();
		  disp_15();         //вывести значение на семисегментник
	   strcpy(TxBuffer, "OK");
	  }


	  else strcpy(TxBuffer, "Invalid value");

	}


	else
		strcpy(TxBuffer,"Invalid Command");					//Если мы не знаем, чего от нас хотят, ругаемся в ответ

	// Передача принятой строки обратно одним из двух способов
		txStr(TxBuffer, true);

	memset(RxBuffer,0,RX_BUFF_SIZE);						//Очистка буфера приёма
	ComReceived = false;									//Сбрасываем флаг приёма строки
}






int main(void) {

	initClk();
	init_portC();
	initButton();
	initTIM2();
	initUSART2();


	while(true) {

		if (ComReceived)				//Ждём приема строки
			ExecuteCommand();


		void display(int count);


//        if (count == 0) {
//            reset();
//            disp_0();
//        }
//
//        else if (count == 1) {
//        	reset();
//        	disp_1();
//            //reset ();
//        }
//
//        else if (count == 2) {
//        	reset();
//        	disp_2();
//            //reset ();
//        }
//
//        else if (count == 3) {
//        	reset();
//        	disp_3();
//            //reset ();
//        }
//
//        else if (count == 4) {
//        	reset();
//        	disp_4();
//        	//reset ();
//        }
//
//        else if (count == 5) {
//        	reset();
//        	disp_5();
//        	//reset ();
//        }
//
//        else if (count == 6) {
//        	reset();
//        	disp_6();
//        	//reset ();
//        }
//
//        else if (count == 7) {
//        	reset();
//        	disp_7();
//        	//reset ();
//        }
//
//        else if (count == 8) {
//        	reset();
//        	disp_8();
//        	//reset ();
//        }
//
//        else if (count == 9) {
//        	reset();
//        	disp_9();
//        	//reset ();
//        }
//
//        else if (count == 10) {
//        	reset();
//        	disp_10();
//        	//reset ();
//        }
//
//        else if (count == 11) {
//        	reset();
//        	disp_11();
//        	//reset ();
//        }
//
//        else if (count == 12) {
//        	reset();
//        	disp_12();
//        	//reset ();
//        }
//
//        else if (count == 13) {
//        	reset();
//        	disp_13();
//        	//reset ();
//        }
//
//        else if (count == 14) {
//        	reset();
//        	disp_14();
//        	//reset ();
//        }
//
//        else if (count == 15) {
//        	reset();
//        	disp_15();
//        	//reset ();
//        }

	}
}

void delay(uint32_t value) {
	for (uint32_t i=0;i<value;i++);
}
