#ifndef PEZIO_H
#define	PEZIO_H


enum {
	OFF, ON
} TOGGLE;


typedef enum {
	NOTHING, MUSIC, BEEP
} TIM4_MODE;

typedef enum {
	BLINKER
} TIM5_MODE;

typedef enum {
	STOP_BLINKER, LEFT_BLINKER, LEFT_BLINKER_SILENT, RIGHT_BLINKER, HAZARD, BACK_BLINKER
} BLINKER_MODE;

extern BLINKER_MODE blinker_mode;

void test_blinker(void);
void start_blinker(TIM5_MODE blinker_mode_input);
void stop_blinker(void);

#endif	/* PEZIO_H */

