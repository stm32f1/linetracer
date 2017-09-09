#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "common/device.h"
#include "action.h"
#include "capture.h"
#include "common/motor.h"
#include "common/sensor.h"
#include "common/utility.h"
#include "common/linetrace.h"
#include "common/servo.h"
#include "common/position.h"
#include "common/comutil.h"
#include "common/debug.h"

bool PET_captured = false;
bool Piramid_captured = false;
const unsigned int search_deg = 120;
const unsigned int accurate_search_deg = 30;
const double petbottle_limit_diff = 50; // mm


// PSDセンサが物体の有無を判定するときのしきい値
static const unsigned int psd_object_threashold_val = (unsigned int) mm_to_PSD(160);
// 物体を掴むときのPSDセンサの値
static const unsigned int psd_get_object_val = (unsigned int) mm_to_PSD(100); //60
struct rgb object_rgb = {0, 0, 0};

bool search_and_capture_object(void)
{
	int object_deg = get_object_direction(search_deg, true);

	if (object_deg == -1) {
		move_for_search(LEFT, search_deg / 2);
		return false;
	}
	move_for_search(LEFT, (unsigned int) object_deg);
	xbee_printf("オブジェクトにだいたい向いたゾ!\r\n");


	int accurate_object_deg = get_object_direction(accurate_search_deg, false);
	if (accurate_object_deg == -1) {
		move_for_search(LEFT, accurate_search_deg / 2);
		return false;
	}
	move_for_search(LEFT, (unsigned int) accurate_object_deg);
	xbee_printf("オブジェクトに正確に向いたぜ!\r\n");
	int32_t moved_step = get_close_to_object() + (int) mm_to_step(15);
	move_for_search(FORWARD, 15);
	get_object();

	// get_close_to_object()で移動した量を戻る
	if (moved_step >= 0)
		move_step(BACK, (unsigned int) moved_step);
	else
		move_step(FORWARD, (unsigned int) (-moved_step));

	//前を向く
	int operation_deg = (int) (search_deg + accurate_search_deg) / 2 - (object_deg + accurate_object_deg);
	if (operation_deg >= 0) {
		move_for_search(LEFT, (unsigned int) operation_deg);
	} else {
		move_for_search(RIGHT, (unsigned int) (-operation_deg));
	}

	return true;
}

int get_object_direction(unsigned int deg, bool ignore_petbottle)
{
	const unsigned int circular_times = 5;
	unsigned int psd_value[5] = {0};
	unsigned int psd_average, nearest_distance = 0;
	int nearest_object_deg = -1;
	unsigned int i = 0;

	lift_neck_top();
	move_for_search(LEFT, deg / 2);
	//xbee_printf("SEARCHING deg!!\r\n");

	run_mode = SEARCH_OBJECT;
	motorR_direction = MOTOR_R_BACK;
	motorL_direction = MOTOR_L_FORWARD;
	motorR.step = deg_to_step(deg);
	motorL.step = deg_to_step(deg);
	MOTOR_R_ENABLE();
	MOTOR_L_ENABLE();

	while ((motorR.step != 0) || (motorL.step != 0)) {
		Buzzer = 0;
		i = (i + 1) % circular_times;
		if (PET_captured == true && Piramid_captured == false) {
			psd_value[i] = sensor_val[PSD_HIGH];
		}else{
			psd_value[i] = sensor_val[PSD_LOW];
		}
		psd_average = sum_u16(psd_value, circular_times) / circular_times;

		if (ignore_petbottle) {
			unsigned int cur_deg = (int) step_to_deg((motorR.step + motorL.step) / 2);
			unsigned int real_deg = cur_deg + (unsigned int) ((180.0 - search_deg) / 2.0);
			double x = cos(toradians(real_deg)) * PSD_to_mm(psd_average);
			if (fabsl(x) <= petbottle_limit_diff) {
				delay_ms(1);
				continue;
			}
		}

		if (psd_average > psd_object_threashold_val) {
			Buzzer = 1;
			if (psd_average > nearest_distance) {
				nearest_distance = psd_average;
				nearest_object_deg = (int) step_to_deg((motorR.step + motorL.step) / 2);
			}
		}
		delay_ms(1);
	}
	Buzzer = 0;
	MOTOR_R_DISABLE();
	MOTOR_L_DISABLE();

	motorR.distance = 0;
	motorL.distance = 0;

	motorR.speed = 100;
	motorL.speed = 100;

	return nearest_object_deg;
}

int32_t get_close_to_object(void)
{
	int32_t moved_step;
	const unsigned int circular_times = 5;
	unsigned int psd_value[5] = {0};
	unsigned int psd_average;
	size_t count = 0;

	run_mode = SEARCH_OBJECT;
	motorR.step = mm_to_step(150 - 20); //45
	motorL.step = mm_to_step(150 - 20); //45

	motorR.distance = 0;
	motorL.distance = 0;

	int moving_forward;

	if (PET_captured == true && Piramid_captured == false) {
		if (sensor_val[PSD_HIGH] < psd_get_object_val) {
			motorR_direction = MOTOR_R_FORWARD;
			motorL_direction = MOTOR_L_FORWARD;
			moving_forward = 1;
		} else {
			motorR_direction = MOTOR_R_BACK;
			motorL_direction = MOTOR_L_BACK;
			moving_forward = 0;
		}
	} else {
		if (sensor_val[PSD_LOW] < psd_get_object_val) {
			motorR_direction = MOTOR_R_FORWARD;
			motorL_direction = MOTOR_L_FORWARD;
			moving_forward = 1;
		} else {
			motorR_direction = MOTOR_R_BACK;
			motorL_direction = MOTOR_L_BACK;
			moving_forward = 0;
		}
	}

	MOTOR_R_ENABLE();
	MOTOR_L_ENABLE();

	while ((motorL.step != 0) || (motorR.step != 0)) {
		count++;
		if (PET_captured == true && Piramid_captured == false) {
			psd_value[count % 5] = sensor_val[PSD_HIGH];
		} else {
			psd_value[count % 5] = sensor_val[PSD_LOW];

		}
		psd_average = sum_u16(psd_value, circular_times) / circular_times;
		if (count < 5) {
			delay_ms(1);
			continue;
		}
		if ((moving_forward == 1 && psd_average > psd_get_object_val) ||
				(moving_forward == 0 && psd_average <= psd_get_object_val)) {
			break;
		}
		delay_ms(1);
	}

	MOTOR_R_DISABLE();
	MOTOR_L_DISABLE();

	moved_step = motorR.distance;
	motorR.distance = 0;
	motorL.distance = 0;

	return moved_step;
}

bool judge_get_object(void)
{
	const unsigned int max_permissible_diff = 150; //100 // 最大許容誤差
	const unsigned int psd_judge_get_object_val = (int) mm_to_PSD(80); //60mmで判定
	if (((psd_judge_get_object_val - max_permissible_diff < sensor_val[PSD_JUDGE_CAPTURED])) &&
			(sensor_val[PSD_JUDGE_CAPTURED] < (psd_judge_get_object_val + max_permissible_diff))) {
		pii(1);
		debug_printf("get an object SUCESSFUL\r\n");
		return true;
	} else {
		down_neck();
		delay_ms(500);
		open_arm();
		delay_ms(500);
		lift_neck_top();
		pipipi(3);
		debug_printf("get an object FAILED\r\n");
		return false;
	}
}

unsigned int object_distance(void)
{
	unsigned int distance = (unsigned int) ((motorR.distance + motorL.distance) / 2);
	motorR.distance = 0;
	motorL.distance = 0;

	return distance;
}

void capture_top_of_piramid(void)
{
	open_arm();
	delay_ms(200);
	action_servo(NECK, 35);
	delay_ms(500);
	close_arm();
	delay_ms(500);
	lift_neck();
	delay_ms(1000);
	return;
}

void put_bottle(void)
{
	action_servo(NECK, 35);
	delay_ms(750);
	open_arm();
	delay_ms(200);
	lift_neck();
	pipipi(1);

	return;
}