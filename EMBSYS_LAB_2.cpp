#include "hal.h"

using namespace std;
// ----------------------------------------T ---------------------------------------------
int T = 50;
// -------------------------------- COUNTER FOR FRAMES -----------------------------------
int cnt = 1;
unsigned int leds_num[] = {GPIO_PIN_3, GPIO_PIN_4, GPIO_PIN_5,
                      GPIO_PIN_6, GPIO_PIN_8, GPIO_PIN_9,
                      GPIO_PIN_11, GPIO_PIN_12};

unsigned int sw_num[] = {GPIO_PIN_4, GPIO_PIN_8,GPIO_PIN_10, GPIO_PIN_12};

// -------------------------- TO CALCULATE FRAME SWITCHING TIME --------------------------
int timer() {
	int time = 0;
	for(int i = 3; i >= 0; i--) {
		int k = 1;
		for(int j = 3 - i; j > 0; j--) {
			k *= 2;
		}
		time += (HAL_GPIO_ReadPin(GPIOE, sw_num[i]) * k);
	}
	return time;
}
// -------------------------------- LED SWITCHING METHOD ---------------------------------
void led(GPIO_PinState led_1, GPIO_PinState led_2, GPIO_PinState led_3, GPIO_PinState led_4, GPIO_PinState led_5, GPIO_PinState led_6, GPIO_PinState led_7, GPIO_PinState led_8) {	
	HAL_GPIO_WritePin(GPIOD, leds_num[0], led_1);
	HAL_GPIO_WritePin(GPIOD, leds_num[1], led_2);
	HAL_GPIO_WritePin(GPIOD, leds_num[2], led_3);
	HAL_GPIO_WritePin(GPIOD, leds_num[3], led_4);
	HAL_GPIO_WritePin(GPIOD, leds_num[4], led_5);
	HAL_GPIO_WritePin(GPIOD, leds_num[5], led_6);
	HAL_GPIO_WritePin(GPIOD, leds_num[6], led_7);
	HAL_GPIO_WritePin(GPIOD, leds_num[7], led_8);
}

void TIM6_IRQ_Handler() {
	switch (cnt) {
		case 1:
// -------------------------------------- FRAME 1 --------------------------------------
			led(GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_SET, GPIO_PIN_SET, GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_RESET);
			break;
		case 2:
// -------------------------------------- FRAME 2 --------------------------------------
			led(GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_SET, GPIO_PIN_SET, GPIO_PIN_SET, GPIO_PIN_SET, GPIO_PIN_RESET, GPIO_PIN_RESET);
			break;
		case 3:
// -------------------------------------- FRAME 3 --------------------------------------
			led(GPIO_PIN_RESET, GPIO_PIN_SET, GPIO_PIN_SET, GPIO_PIN_SET, GPIO_PIN_SET, GPIO_PIN_SET, GPIO_PIN_SET, GPIO_PIN_RESET);
			break;
		case 4:
// -------------------------------------- FRAME 4 --------------------------------------
			led(GPIO_PIN_SET, GPIO_PIN_SET, GPIO_PIN_SET, GPIO_PIN_SET, GPIO_PIN_SET, GPIO_PIN_SET, GPIO_PIN_SET, GPIO_PIN_SET);
			break;
		case 5:
// -------------------------------------- FRAME 5 --------------------------------------
			led(GPIO_PIN_RESET, GPIO_PIN_SET, GPIO_PIN_SET, GPIO_PIN_SET, GPIO_PIN_SET, GPIO_PIN_SET, GPIO_PIN_SET, GPIO_PIN_RESET);
			break;
		case 6:
// -------------------------------------- FRAME 6 --------------------------------------
			led(GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_SET, GPIO_PIN_SET, GPIO_PIN_SET, GPIO_PIN_SET, GPIO_PIN_RESET, GPIO_PIN_RESET);
			break;
		case 7:
// -------------------------------------- FRAME 7 --------------------------------------
			led(GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_SET, GPIO_PIN_SET, GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_RESET);
			break;
		case 8:
// -------------------------------------- FRAME 8 ---------------------------------------	
			led(GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_RESET);
			break;
	}
	
	cnt++;
}

void TIM7_IRQ_Handler() {
	led(GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_RESET);
	if((cnt - 1) == 8) {
		//__disable_irq();
		cnt = 1;
	}
}

int umain() {
	int t = 0;
		
		//HAL_Delay(1);
		
		registerTIM6_IRQHandler(TIM6_IRQ_Handler);
		registerTIM7_IRQHandler(TIM7_IRQ_Handler);
	
		__enable_irq();
	
		//WRITE_REG(TIM6_ARR, t);
		WRITE_REG(TIM6_DIER, TIM_DIER_UIE);
		WRITE_REG(TIM6_PSC, 0);
		//WRITE_REG(TIM7_ARR, t);
		WRITE_REG(TIM7_DIER, TIM_DIER_UIE);
		WRITE_REG(TIM7_PSC, 0);
		
		
		while(true) {
			t = 500 + (T * timer());
			
			WRITE_REG(TIM6_ARR, t);
			WRITE_REG(TIM7_ARR, t);
			
			WRITE_REG(TIM6_CR1, 0x1);
			WRITE_REG(TIM7_CR1, 0x1);
		}
	return 0;
}
