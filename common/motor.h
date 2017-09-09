#ifndef MOTOR_H
#define MOTOR_H

#define MOTOR_FREQ SYS_FREQ
#define SLOW_SPEED 100
#define SEARCH_SPEED 100

#define MOTOR_DAC_VALUE(ampere) ((uint16_t)((double)ampere * 4095 * 0.155 / 3.3))
#define MOTOR_VREF MOTOR_DAC_VALUE(1.5)

#define MOTOR_R_ENABLE() (TIM6->CR1 |= 1)
#define MOTOR_L_ENABLE() (TIM7->CR1 |= 1)
#define MOTOR_R_DISABLE()  (TIM6->CR1 &= (uint16_t) ~1)
#define MOTOR_L_DISABLE()  (TIM7->CR1 &= (uint16_t) ~1)

#define MOTOR_SLEEP() (DAC_SetChannel1Data(DAC_Align_12b_R, 4095))
#define MOTOR_WAKE_UP() (DAC_SetChannel1Data(DAC_Align_12b_R, MOTOR_VREF))

struct motor {
	uint16_t prescaler;
	uint16_t target_speed;
	double speed;
	uint32_t step;
	int32_t distance;
};

extern signed int motor_operation;

enum motorR_dir {
	MOTOR_R_FORWARD = 0,
	MOTOR_R_BACK = 1
};

enum motorL_dir {
	MOTOR_L_FORWARD = 1,
	MOTOR_L_BACK = 0
};

typedef enum {
	STOP = 0, SLOW,
	SEARCH_OBJECT,
	LINETRACE,
	MOVE
} RUN_MODE, *PRUN_MODE;

extern struct motor motorR;
extern struct motor motorL;

extern unsigned int motor_acceleration;
extern const unsigned int straight_acceleration;
extern const unsigned int rotation_acceleration;

extern RUN_MODE run_mode;

void set_last_ARR(void);

#endif /* MOTOR_H */
