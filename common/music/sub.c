#include "device.h"
#include "utility.h"
#include "sub.h"
#include "music.h"

BLINKER_MODE blinker_mode = STOP_BLINKER;

void TIM2_IRQHandler(void)
{
	TIM2->SR &= (uint16_t) ~1;

	Piezo_Buzzer = !Piezo_Buzzer;
}

void TIM5_IRQHandler(void)
{
	static uint16_t timer_count = 0;

	TIM5->SR &= (uint16_t) ~1;

	if (blinker_mode == STOP_BLINKER) {
		Left_Blinker = 0;
		Right_Blinker = 0;
		timer_count = 0;
		TIM2->CR1 &= (uint16_t) ~1;
		TIM5->CR1 &= (uint16_t) ~1;
		return;
	}
	if (timer_count == 0) {
		if (blinker_mode == LEFT_BLINKER) {
			Left_Blinker = 1;
			Right_Blinker = 0;
			if (!sound_status) TIM2->CR1 |= 1;
		} else if (blinker_mode == LEFT_BLINKER_SILENT) {
			Left_Blinker = 1;
			Right_Blinker = 0;
			if (!sound_status) TIM2->CR1 &= (uint16_t) ~1;
		} else if (blinker_mode == RIGHT_BLINKER) {
			Left_Blinker = 0;
			Right_Blinker = 1;
			if (!sound_status) TIM2->CR1 &= (uint16_t) ~1;
		} else if (blinker_mode == HAZARD) {
			Left_Blinker = 1;
			Right_Blinker = 1;
			if (!sound_status) TIM2->CR1 &= (uint16_t) ~1;
		} else if (blinker_mode == BACK_BLINKER) {
			Left_Blinker = 1;
			Right_Blinker = 1;
			if (!sound_status) TIM2->CR1 |= 1;
		}
	} else if (timer_count == 15) {
		if (!sound_status) {
			if (blinker_mode == BACK_BLINKER)
				TIM2->CR1 |= 1;
			else TIM2->CR1 &= (uint16_t) ~1;
		}
	} else if (timer_count == 18) {
		if (!sound_status) {
			if (blinker_mode == LEFT_BLINKER || blinker_mode == BACK_BLINKER) TIM2->CR1 |= 1;
			else TIM2->CR1 &= (uint16_t) ~1;
		}
	} else if (timer_count == 33) {
		if (!sound_status) TIM2->CR1 &= (uint16_t) ~1;
		Left_Blinker = 0;
		Right_Blinker = 0;
	} else if (timer_count == 68) {
		if (!sound_status) TIM2->CR1 &= (uint16_t) ~1;
		timer_count = 0;
		return;
	}
	timer_count++;
}

void test_blinker(void)
{
	blinker_mode = LEFT_BLINKER;
	TIM5->PSC = 7200 - 1;
	TIM5->ARR = 100;
	TIM5->CR1 |= 1;
	while (1);
}

void start_blinker(TIM5_MODE blinker_mode_input)
{
	blinker_mode = blinker_mode_input;
	set_musical_freq(D, 8);
	TIM5->CR1 |= 1;
}

void stop_blinker(void)
{
	blinker_mode = STOP_BLINKER;
}