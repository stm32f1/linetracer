#include <stdint.h>
#include "device.h"
#include "motor.h"


struct motor motorR = {100, 1200, 100, 0, 0};
struct motor motorL = {100, 1200, 100, 0, 0};

signed int motor_operation = 0;

static const double step_distance = 0.4084067; //52 * 3.142 * (200*2)
static const unsigned int v_o = 100;
unsigned int motor_acceleration = 500;

const unsigned int straight_acceleration = 400;
const unsigned int rotation_acceleration = 100;

RUN_MODE run_mode;

void TIM6_IRQHandler(void)
{
	double controlled_v = 0; //[mm/s]
	static double dt = 0; //[s]
	double limit_step_to_stop = (motorR.speed * motorR.speed - v_o * v_o) / 2 / motor_acceleration / step_distance;//止まるまでに最低必要なステップ数

	TIM6->SR &= (uint16_t) ~1;
	motorR_clock = !motorR_clock;

	if (motorR_clock == 0) {
		if (run_mode >= 3) {
			motorR.prescaler = 100;
		} else if (run_mode == SLOW) {
			motorR.prescaler = 100;
			motorR.target_speed = SLOW_SPEED;
			motorR.speed = SLOW_SPEED;
			motor_operation = 0;

			if (motorR_direction == MOTOR_R_FORWARD) motorR.distance++;
			else motorR.distance--;

		} else if (run_mode == SEARCH_OBJECT) {
			motorR.prescaler = 100;
			motorR.target_speed = SEARCH_SPEED;
			motorR.speed = SEARCH_SPEED;

			if (motorR_direction == MOTOR_R_FORWARD) motorR.distance++;
			else motorR.distance--;
		} else if (run_mode == STOP) {
			Break_Lump = 1;
		}

		dt = step_distance / motorR.speed;
		motorR.speed = motorR.step > limit_step_to_stop ? motorR.speed + motor_acceleration * dt < motorR.target_speed ? motorR.speed + motor_acceleration * dt : motorR.target_speed : motorR.speed - motor_acceleration*dt;
		controlled_v = (motor_operation >= motorR.speed) ? 10 : ((-motor_operation) >= motorR.speed) ? 2 * motorR.speed : motorR.speed - motor_operation;
		double controlled_dt = step_distance / controlled_v;

		TIM6->PSC = (uint16_t) (motorR.prescaler - 1);
		TIM6->ARR = (uint16_t) (MOTOR_FREQ / motorR.prescaler * controlled_dt);

	} else if (motorR_clock == 1) {

		if (run_mode == MOVE || run_mode == SLOW || run_mode == SEARCH_OBJECT) {
			motorR.step--;
			Break_Lump = 1;
			if (motorR.step == 0) {
				TIM6->CR1 &= (uint16_t) ~1;
				return;
			}
		} else if (run_mode == LINETRACE) {
			motorR.step++;
			Break_Lump = 0;
		} else if (run_mode == STOP) {
			TIM6->CR1 &= (uint16_t) ~1;
			Break_Lump = 1;
			motorR.step = 0;
			motorR.speed = 100;
			return;
		}
	}
}

void TIM7_IRQHandler(void)
{
	double controlled_v = 0; //[mm/s]
	static double dt = 0; //[s]
	double limit_step_to_stop = (motorL.speed * motorL.speed - v_o * v_o) / 2 / motor_acceleration / step_distance;

	TIM7->SR &= (uint16_t) ~1;
	motorL_clock = !motorL_clock;

	if (motorL_clock == 0) {
		if (run_mode >= 3) {
			motorL.prescaler = 100;
		} else if (run_mode == SLOW) {
			motorL.prescaler = 100;
			motorL.target_speed = SLOW_SPEED;
			motorL.speed = SLOW_SPEED;
			motor_operation = 0;

			if (motorL_direction == MOTOR_L_FORWARD) motorL.distance++;
			else motorL.distance--;

		} else if (run_mode == SEARCH_OBJECT) {
			motorL.prescaler = 100;
			motorL.target_speed = SEARCH_SPEED;
			motorL.speed = SEARCH_SPEED;

			if (motorL_direction == MOTOR_L_FORWARD) motorL.distance++;
			else motorL.distance--;
		} else if (run_mode == STOP) {
			Break_Lump = 1;
		}
		dt = step_distance / motorL.speed;
		motorL.speed = motorL.step > limit_step_to_stop ? motorL.speed + motor_acceleration * dt < motorL.target_speed ? motorL.speed + motor_acceleration * dt : motorL.target_speed : motorL.speed - motor_acceleration*dt;
		controlled_v = (motor_operation >= motorL.speed) ? 2 * motorL.speed : ((-motor_operation) >= motorL.speed) ? 10 : motorL.speed + motor_operation;
		double controlled_dt = step_distance / controlled_v;

		TIM7->PSC = (uint16_t) (motorL.prescaler - 1);
		TIM7->ARR = (uint16_t) (MOTOR_FREQ / motorL.prescaler * controlled_dt);

	} else if (motorL_clock == 1) {

		if (run_mode == MOVE || run_mode == SLOW || run_mode == SEARCH_OBJECT) {
			motorL.step--;
			Break_Lump = 1;
			if (motorL.step == 0) {
				TIM7->CR1 &= (uint16_t) ~1;
				return;
			}
		} else if (run_mode == LINETRACE) {
			motorL.step++;
			Break_Lump = 0;
		} else if (run_mode == STOP) {
			TIM7->CR1 &= (uint16_t) ~1;
			Break_Lump = 1;
			motorL.step = 0;
			motorL.speed = 100;
			return;
		}
	}
}
