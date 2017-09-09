#include <stdbool.h>
#include "device.h"
#include "utility.h"
#include "linetrace.h"
#include "motor.h"
#include "sensor.h"
#include "PID.h"
#include "action.h"
#include "comutil.h"

static bool judge_cross(void);
static bool judge_goal(void);

unsigned int corner_count = 0;

void linetrace(int cycle)
{
	int32_t control_input = 0;
	unsigned int goal_detection = 2;

	run_mode = LINETRACE;
	motor_acceleration = straight_acceleration;

	motorR.speed = 300;
	motorL.speed = 300;

	motorR.target_speed = 1000;
	motorL.target_speed = 1000;

	motorR_direction = MOTOR_R_FORWARD;
	motorL_direction = MOTOR_L_FORWARD;

	MOTOR_R_ENABLE();
	MOTOR_L_ENABLE();

	while (1) {
		if (!(goal_detection -= judge_goal())) break;
		if (judge_cross());

		control_input = (signed int) (fix_sensors_value + (sensor_val[0]*5 + sensor_val[1]*3
				+ sensor_val[2] - sensor_val[3] - sensor_val[4]*3 - sensor_val[5]*5));
		motor_operation = calc_PID(control_input);
	}
	motor_operation = 0;

}

void linetrace_mm(unsigned int distance)
{
	signed int control_input = 0;

	motorR.target_speed = 500;
	motorL.target_speed = 500;
	motorR.speed = 100;
	motorL.speed = 100;

	motorR_direction = MOTOR_R_FORWARD;
	motorL_direction = MOTOR_L_FORWARD;
	motorR.step = mm_to_step(distance);
	motorL.step = mm_to_step(distance);

	run_mode = MOVE;
	motor_acceleration = straight_acceleration;

	MOTOR_R_ENABLE();
	MOTOR_L_ENABLE();

	while ((motorR.step) || (motorL.step)) {
		control_input = (signed int)
				((sensor_val[0]*2 + sensor_val[1] - sensor_val[2] - sensor_val[3]*2) - fix_sensors_value);
		motor_operation = calc_PID_a(control_input);
	}
	motorR.distance = 0;
	motorL.distance = 0;

	MOTOR_R_DISABLE();
	MOTOR_L_DISABLE();
	motorR.speed = 0;
	motorL.speed = 0;
}

void get_on_line(unsigned int distance)
{
	motor_operation = 0;
	distance = mm_to_step(distance);
	MOTOR_WAKE_UP();
	run_mode = LINETRACE;
	motor_acceleration = straight_acceleration;
	motorR_direction = MOTOR_R_FORWARD;
	motorL_direction = MOTOR_L_FORWARD;
	MOTOR_R_ENABLE();
	MOTOR_L_ENABLE();
	while ((motorR.step <= distance) && (motorL.step <= distance));
}

static bool judge_cross(void)
{
	if (sum_u16((unsigned int*) sensor_val, 6) > judge_color_standard * 6) {
		motor_operation = 0;
		pipipi(1);
		return true;
	}
	return false;
}

static bool judge_goal(void)
{
	xbee_printf("CROSS LINE!!\r\n");
	const uint16_t sensor_threshold = 800;

	/*
	static unsigned int cycle_count = 0;
	static unsigned int sensor_buff[5] = {0};

	sensor_buff[cycle_count++] = (unsigned int)(sensor_val[GOAL_MARKER] - fix_sensor_val[GOAL_MARKER]);
	if (cycle_count == 5) cycle_count = 0;
	if (sum_u16(sensor_buff, 5) / 5 > goal_threshold) return true;
	 */

	if ((sensor_val[GOAL_MARKER] - fix_sensor_val[GOAL_MARKER]) > sensor_threshold) {
		pipipi(1);
		return true;
	}
	return false;
}

static bool judge_corner(void)
{
	xbee_printf("CROSS LINE!!\r\n");
	const uint16_t sensor_threshold = 800;

	/*
	static unsigned int cycle_count = 0;
	static unsigned int sensor_buff[5] = {0};

	sensor_buff[cycle_count++] = (unsigned int)(sensor_val[GOAL_MARKER] - fix_sensor_val[GOAL_MARKER]);
	if (cycle_count == 5) cycle_count = 0;
	if (sum_u16(sensor_buff, 5) / 5 > goal_threshold) return true;
	 */

	if ((sensor_val[CORNER_MARKER] - fix_sensor_val[CORNER_MARKER]) > sensor_threshold) {
		corner_count;
		pipipi(1);
		return true;
	}
	return false;
}

void calc_curvature(unsigned int corner_count)
{
}

void linetrace_to_crossline(int cycle, unsigned int distance)
{
	int32_t control_input;
	bool cross_flag = false;

	motorR_direction = MOTOR_R_FORWARD;
	motorL_direction = MOTOR_L_FORWARD;
	MOTOR_R_ENABLE();
	MOTOR_L_ENABLE();

	linetrace(cycle - 1);

	run_mode = MOVE;
	motor_acceleration = straight_acceleration;
	motorR.step = mm_to_step(distance);
	motorL.step = mm_to_step(distance);

	while ((motorR.speed > SLOW_SPEED) || (motorL.speed > SLOW_SPEED)) {
		control_input = (signed int) (fix_sensors_value - (sensor_val[0]*2 + sensor_val[1]
				- sensor_val[2] - sensor_val[3]*2));
		motor_operation = calc_PID(control_input);
		if (judge_cross()) {
			MOTOR_R_DISABLE();
			MOTOR_L_DISABLE();
			cross_flag = true;
			break;
		}

		motor_operation = 0;
	}
	if (cross_flag == false) {
		run_mode = SLOW;
		motorR.step = mm_to_step(100);
		motorL.step = mm_to_step(100);

		while (!judge_cross());
	}

	MOTOR_R_DISABLE();
	MOTOR_L_DISABLE();
}

void accel_linetrace(DIRECTION direction, int distance)
{
	run_mode = LINETRACE;
	motor_acceleration = straight_acceleration;

	switch (direction) {

	case BACK:
		motorL_direction = MOTOR_L_BACK;
		motorR_direction = MOTOR_R_BACK;
		break;
	case FORWARD:
	default:
		motorL_direction = MOTOR_L_FORWARD;
		motorR_direction = MOTOR_R_FORWARD;
		break;
	}

	motorR.step = 0;
	motorL.step = 0;

	unsigned int run_step = mm_to_step(distance);

	MOTOR_R_ENABLE();
	MOTOR_L_ENABLE();

	while ((motorR.step <= run_step)&&(motorL.step <= run_step)) {
		const double kp_max = 0;
		double kp = 0;
		signed int control_input = 0;

		kp = kp_max * motorR.speed / motorR.target_speed;
		control_input = (signed int) (fix_sensors_value - (sensor_val[0]*2 + sensor_val[1]
				- sensor_val[2] - sensor_val[3]*2));
		motor_operation = PID(control_input, kp, 0, 0);
	}
	motorR.step = 0;
	motorL.step = 0;

	return;
}