#include "device.h"
#include "utility.h"
#include "music.h"
#include "sub.h"
#include "titles.h"

const uint16_t Note_Freq[] = {
	0, 26163, 27718, 29366, 31112, 32963, 34923,
	36999, 39200, 41530, 44000, 46616, 49388
};

TIM4_MODE TIM4_mode = NOTHING;

static NOTE_LENGTH note_length = 0;
static uint8_t melody_length = 0;
uint8_t sound_status = OFF;
const uint8_t (*p_music)[3];

void TIM4_IRQHandler(void)
{
	static uint16_t note_count = 0;

	TIM4->SR &= (uint16_t) ~1;

	if (TIM4_mode == NOTHING) {
		note_length = 0;
		note_count = 0;
		sound_status = OFF;
		TIM2->CR1 &= (uint16_t) ~1;
		TIM4->CR1 &= (uint16_t) ~1;
	} else if (TIM4_mode == MUSIC) {
		if (!note_length) {
			set_musical_freq(p_music[note_count][0], p_music[note_count][1]);
			note_length = p_music[note_count][2];
			if (!p_music[note_count][0]) TIM2->CR1 &= (uint16_t) ~1;
			else TIM2->CR1 |= 1;
			note_count++;
		} else if (note_length == 1) TIM2->CR1 &= (uint16_t) ~1;
		note_length--;
		if (note_count > melody_length) {
			TIM4_mode = NOTHING;
			TIM2->CR1 &= (uint16_t) ~1;
		}
	} else if (TIM4_mode == BEEP) {
		TIM2->CR1 |= 1;
		if (!(--note_length)) {
			TIM2->CR1 &= (uint16_t) ~1;
			TIM4_mode = NOTHING;
		}
	} else {
		inform_error();
	}
	return;
}

void play_music(music *title, int times)
{
	p_music = title->score;
	set_tempo(title->tempo);
	melody_length = title->length;
	while (times--) {
		TIM4_mode = MUSIC;
		sound_status = ON;
		TIM4->CR1 |= 1;
		while (TIM4->CR1 & 1);
	}
	return;
}

void play_sound(CODE_NOTE code_note, uint16_t time)
{
	sound_status = ON;
	TIM4_mode = BEEP;
	TIM4->PSC = 7200 - 1;
	TIM4->ARR = 1000;
	set_musical_freq(code_note, 8);
	note_length = time;
	TIM4->CR1 |= 1;
	while (TIM4->CR1 & 1);
}