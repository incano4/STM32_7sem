#include "main.h"


void init_portC(void) {

      //пины PC0-PC7 отвечают за сегменты индикатора
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN; /*вкл тактирование порта с*/

	GPIOC->CRL &= ~(GPIO_CRL_CNF0|GPIO_CRL_MODE0); /*зануляем биты CRL (0000)*/
	GPIOC->CRL |= GPIO_CRL_MODE0; /*ставим маску 1100 - 50MHz analog mode*/  //00 осталось с предыдущей строчки, 11 - добавили только что

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

//	GPIOC->CRL &= ~(GPIO_CRL_CNF7|GPIO_CRL_MODE7);
//	GPIOC->CRL |= GPIO_CRL_MODE7;
}


void init_portB(void) {

	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN; /*вкл тактирование порта В*/

	//пин PB0 отвечает за точку 2-го индикатора
	GPIOB->CRL &= ~(GPIO_CRL_CNF0|GPIO_CRL_MODE0);
	GPIOB->CRL |= GPIO_CRL_MODE0;

	//пины PB1-PB3 отвечают за землю индикаторов
	GPIOB->CRL &= ~(GPIO_CRL_CNF1|GPIO_CRL_MODE1);
	GPIOB->CRL |= GPIO_CRL_MODE1;

	GPIOB->CRL &= ~(GPIO_CRL_CNF2|GPIO_CRL_MODE2);
	GPIOB->CRL |= GPIO_CRL_MODE2;

	GPIOB->CRL &= ~(GPIO_CRL_CNF3|GPIO_CRL_MODE3);
	GPIOB->CRL |= GPIO_CRL_MODE3;





	//пин PA0 отвечает за вход АЦП
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN; /*вкл тактирование порта В*/
	GPIOA->CRL &= ~(GPIO_CRL_CNF0|GPIO_CRL_MODE0);
}


void init_ADC(void){

	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;  				//Включить тактирование АЦП

	//Настройка времени преобразования каналов
	ADC1->SMPR2 |= ADC_SMPR2_SMP0;						//Канал 0 - 239.5 тактов

	ADC1->CR2 |= ADC_CR2_EXTSEL;       					//Выбрать в качестве источника запуска SWSTART
	ADC1->CR2 |= ADC_CR2_ADON;         					//Включить АЦП

	delay(10);									//Задержка перед калибровкой
	ADC1->CR2 |= ADC_CR2_CAL;					//Запуск калибровки
	while ((ADC1->CR2 & ADC_CR2_CAL) != 0);		//ожидание окончания калибровки
}


void disp(int result) {	//отображаем число на одном индикаторе
	if(result == 0){
		reset();
		disp_0();
	}

	else if(result == 1) {
		reset();
		disp_1();
	}

	else if(result == 2) {
		reset();
		disp_2();
	}

	else if(result == 3) {
		reset();
	}

	else if(result == 4) {
		reset();
		disp_4();
	}

	else if(result == 5) {
		reset();
		disp_5();
	}

	else if(result == 6) {
		reset();
		disp_6();
	}

	else if(result == 7) {
		reset();
		disp_7();
	}

	else if(result == 8) {
		reset();
		disp_8();
	}

	else if(result == 9) {
		reset();
		disp_9();
	}
}


void indicator(int i) {		//включаем определенный семисегментник

	if(i == 0) {
		indicator_reset();
		indicator_0();
	}

	else if(i == 1) {
		indicator_reset();
		indicator_1();
		dot();
	}

	else if (i == 2) {
		indicator_reset();
		indicator_2();
	}
}


uint16_t ADC_proc(uint8_t n) {
	ADC1->SQR3 = n;									//Записываем номер канала в регистр SQR3
	ADC1->CR2 |= ADC_CR2_SWSTART;					//Запускаем преобразование в регулярном канале
	while((ADC1->SR & ADC_SR_EOC) != 0);			//Ждем окончания преобразования
	return ADC1->DR;								//Читаем результат
}


int main(void) {

	init_portC();
	init_portB();
//	init_ADC();

	while(1) {

		 disp_8();

//		uint16_t result = ADC_proc(0);		//в result считываем показания с АЦП
//		result = result*2000/4096;			///////////!!!!!!!!!!!!!/////
//
//		indicator(0);
//		disp(result / 10 % 10);
//		delay(1000);
//
//		indicator(1);
//		disp(result / 100 % 10);
//		delay(1000);
//
//		indicator(2);
//		disp(result / 1000);
//		delay(1000);







//		for (int i=0; i<3; i++) {
//			display(i);
//
//			if(i == 0) count(Current / 10 % 10);
//			else if( i==1 ) count(Current / 100 % 10);
//			else count(Current / 1000);
//			delay(DYNAMIC_SWITCH_TIME);
//		}


//		for (int i=0; i<3; i++) {
//		display(i);
//		delay(1000);
//		count (Current %10);
//		Current = Current / 10;
//		}
	}
}


void delay(uint32_t value) {
	for (uint32_t i=0;i<value;i++);
}
