#include "device.h"
#include "utility.h"
#include "servo.h"
#include "capture.h"

void action_servo(SERVO_NAME servo_name, int deg)
{
	uint16_t pulse = (uint16_t) (800 + (1400 * deg / 180));
	switch (servo_name) {
	case NECK:
		TIM3->CCR1 = pulse; // PC6

#ifdef ALPHA
		//uint16_t pulse_2 = (uint16_t) (800 + (1400 * (180 - deg) / 180));
		uint16_t pulse_2 = (uint16_t) (800 + 40 + (1400 * (180 - deg) / 180));
		TIM3->CCR2 = pulse_2; // PC7
#endif
		break;
	case RIGHT_ARM:
		TIM3->CCR3 = pulse; // PC8
		break;
	case LEFT_ARM:
		TIM3->CCR4 = pulse; // PC9
		break;
	default:
		/* WTF! */
		break;
	}
}

void wake_up_all_servo(void)
{
	TIM3->CCER = 0b0001000100010001;
}

void sleep_all_servo(void)
{
	TIM3->CCER = 0b0000000000000000;
}

void open_arm(void)
{
	action_servo(RIGHT_ARM, 100); //135
	action_servo(LEFT_ARM, 80); //45
}

void close_arm(void)
{
	action_servo(RIGHT_ARM, 40); //45
	action_servo(LEFT_ARM, 140); //135
}

void lift_neck(void)
{
	action_servo(NECK, 70); //30
}

void down_neck(void)
{
	action_servo(NECK, 0); //90
}

void lift_neck_top(void)
{
	action_servo(NECK, 90);
}

void get_object(void)
{
	open_arm();
	delay_ms(200);

	if (PET_captured == true && Piramid_captured == false) {
		action_servo(NECK,40);
		Piramid_captured = true;
	} else {
		down_neck();
	}
	delay_ms(500);

	close_arm();
	delay_ms(200);

	lift_neck();
	pipipi(1);
}

void release_object(void)
{
	down_neck();
	delay_ms(750);

	open_arm();
	delay_ms(200);

	lift_neck();
	pipipi(1);
}

void normalize_arm(void)
{
	lift_neck();
	close_arm();
}