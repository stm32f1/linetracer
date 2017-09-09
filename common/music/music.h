#ifndef MUSIC_H
#define MUSIC_H

#define set_musical_freq(interval, octave) TIM2->ARR = \
	(uint16_t) ((100000000 / (Note_Freq[(interval)] * (octave))) / 2)

#define set_tempo(tempo)  TIM4->ARR = (uint16_t) ((10000 / (tempo) * 60 / 24) - 1)
//#define set_tempo(tempo)  TIM4->ARR = (uint16_t) ((10000 / (tempo) * 60 / 12) - 1)

typedef enum {
	REST, C, C_, D, D_, E, F, F_, G, G_, A, A_, B
} CODE_NOTE;

typedef enum {
	SIXTEENTH_NOTE = 6, DOTTEDSIXTEENTH_NOTE = 9, EIGHTH_NOTE = 12, DOTTED_EIGHTH_NOTE = 18,
			QUARTER = 24, DOTTED_QUARTER = 36, HALF = 48, WHOLE = 96
} NOTE_LENGTH;

/*
typedef enum {
	SIXTEENTH_NOTE = 3, EIGHTH_NOTE = 6, THIRD_NOTE = 9, QUARTER = 12, HALF = 24, WHOLE = 48
} NOTE_LENGTH;
*/

typedef const struct {
	uint8_t tempo;
	uint8_t length;
	uint8_t score[][3];
} music;

extern music azamino, gota_del_vient, kaigan, machinami,
		see_you_again, spring_box, water_crown, japari;


extern const uint16_t Note_Freq[];
extern uint8_t sound_status;

void play_music(music *title, int times);
void play_sound(CODE_NOTE code_note, uint16_t time);

#endif /* MUSIC_H */