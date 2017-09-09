#include "device.h"
#include "utility.h"
#include "motor.h"
#include "action.h"

void move(DIRECTION direction, unsigned int distance)
{
	run_mode = MOVE;
	motor_operation = 0;

	motorR.speed = 100;
	motorL.speed = 100;

	motorR.target_speed = 500;
	motorL.target_speed = 500;

	motorR_clock = 0;
	motorL_clock = 0;

	switch (direction) {
	case FORWARD:
		motor_acceleration = straight_acceleration;
		motorR_direction = MOTOR_R_FORWARD;
		motorL_direction = MOTOR_L_FORWARD;
		motorR.step = mm_to_step(distance);
		motorL.step = mm_to_step(distance);
		break;
	case BACK:
		motor_acceleration = straight_acceleration;
		motorR_direction = MOTOR_R_BACK;
		motorL_direction = MOTOR_L_BACK;
		motorR.step = mm_to_step(distance);
		motorL.step = mm_to_step(distance);
		break;
	case RIGHT:
		motor_acceleration = rotation_acceleration;
		motorR_direction = MOTOR_R_BACK;
		motorL_direction = MOTOR_L_FORWARD;
		motorR.step = deg_to_step(distance);
		motorL.step = deg_to_step(distance);
		break;
	case LEFT:
		motor_acceleration = rotation_acceleration;
		motorR_direction = MOTOR_R_FORWARD;
		motorL_direction = MOTOR_L_BACK;
		motorR.step = deg_to_step(distance);
		motorL.step = deg_to_step(distance);
		break;
	}

	MOTOR_R_ENABLE();
	MOTOR_L_ENABLE();

	while ((motorR.step != 0) || (motorL.step != 0));

	MOTOR_R_DISABLE();
	MOTOR_L_DISABLE();
	motorR.speed = 0;
	motorL.speed = 0;
}

void move_step(DIRECTION direction, unsigned int step)
{
	run_mode = SEARCH_OBJECT;
	motor_operation = 0;

	motorR.speed = 200;
	motorL.speed = 200;

	motorR_clock = 0;
	motorL_clock = 0;


	switch (direction) {
	case FORWARD:
		motorR_direction = MOTOR_R_FORWARD;
		motorL_direction = MOTOR_L_FORWARD;
		motorR.step = step;
		motorL.step = step;
		break;
	case BACK:
		motorR_direction = MOTOR_R_BACK;
		motorL_direction = MOTOR_L_BACK;
		motorR.step = step;
		motorL.step = step;
		break;
	case RIGHT:
		motorR_direction = MOTOR_R_BACK;
		motorL_direction = MOTOR_L_FORWARD;
		motorR.step = step;
		motorL.step = step;
		break;
	case LEFT:
		motorR_direction = MOTOR_R_FORWARD;
		motorL_direction = MOTOR_L_BACK;
		motorR.step = step;
		motorL.step = step;
		break;
	}

	MOTOR_R_ENABLE();
	MOTOR_L_ENABLE();

	while ((motorR.step != 0) || (motorL.step != 0));
}

void move_for_search(DIRECTION direction, unsigned int distance)
{
	run_mode = SEARCH_OBJECT;
	motor_operation = 0;

	motorR_clock = 0;
	motorL_clock = 0;

	switch (direction) {
	case FORWARD:
		motorR_direction = MOTOR_R_FORWARD;
		motorL_direction = MOTOR_L_FORWARD;
		motorR.step = mm_to_step(distance);
		motorL.step = mm_to_step(distance);
		break;
	case BACK:
		motorR_direction = MOTOR_R_BACK;
		motorL_direction = MOTOR_L_BACK;
		motorR.step = mm_to_step(distance);
		motorL.step = mm_to_step(distance);
		break;
	case RIGHT:
		motorR_direction = MOTOR_R_BACK;
		motorL_direction = MOTOR_L_FORWARD;
		motorR.step = deg_to_step(distance);
		motorL.step = deg_to_step(distance);
		break;
	case LEFT:
		motorR_direction = MOTOR_R_FORWARD;
		motorL_direction = MOTOR_L_BACK;
		motorR.step = deg_to_step(distance);
		motorL.step = deg_to_step(distance);
		break;
	}

	MOTOR_R_ENABLE();
	MOTOR_L_ENABLE();

	while ((motorR.step != 0) || (motorL.step != 0));

	MOTOR_R_DISABLE();
	MOTOR_L_DISABLE();

	motorR.speed = 0;
	motorL.speed = 0;
}

void touch_wall(void)
{
	run_mode = SLOW;
	
	motor_operation = 0;

	motorR_clock = 0;
	motorL_clock = 0;

	motorR_direction = MOTOR_R_BACK;
	motorL_direction = MOTOR_L_BACK;
	motorR.step = mm_to_step(1000);
	motorL.step = mm_to_step(1000);

	MOTOR_R_ENABLE();
	MOTOR_L_ENABLE();

	while (((motorR.step != 0) && (motorL.step != 0)) && ((micro_switchR == 0) || (micro_switchL == 0)));

	motorR.step = mm_to_step(50);
	motorL.step = mm_to_step(50);
	MOTOR_R_ENABLE();
	MOTOR_L_ENABLE();
	while ((motorR.step != 0) && (motorL.step != 0));

	MOTOR_R_DISABLE();
	MOTOR_L_DISABLE();
	motorR.step = 0;
	motorL.step = 0;
	pii(1);
}

void cleaning(void)
{
	run_mode = MOVE;
	motor_acceleration = 500;

	MOTOR_WAKE_UP();
	motorR_direction = MOTOR_R_FORWARD;
	motorL_direction = MOTOR_L_FORWARD;
	motorR.step = mm_to_step(10000);
	motorL.step = mm_to_step(10000);
	motorR.target_speed = 500;
	motorL.target_speed = 500;

	MOTOR_R_ENABLE();
	while ((motorR.step) && (ButtonA));
	MOTOR_R_DISABLE();
	motorR.speed = 0;
	pii(1);

	MOTOR_L_ENABLE();
	while ((motorL.step) && (ButtonA));
	MOTOR_L_DISABLE();
	motorL.speed = 0;
	pii(1);

	MOTOR_SLEEP();
}
