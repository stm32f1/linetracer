#include <stdio.h>
#include "common/device.h"
#include "common/utility.h"
#include "mode.h"
#include "capture.h"
#include "common/motor.h"
#include "action.h"
#include "common/sensor.h"
#include "common/linetrace.h"
#include "common/servo.h"
#include "subroutine.h"
#include "comutil.h"
#include "sub.h"
#include "music.h"

static void mode1(void);
static void mode2(void);
static void mode3(void);
static void mode4(void);
static void mode5(void);
static void mode6(void);
static void mode7(void);
static void mode8(void);


void (*mode_funcp[8])(void) = {
	mode1, mode2, mode3, mode4, mode5, mode6, mode7, mode8
};

int mode_select(int total_mode)
{
	int i = 0;

	delay_ms(100);

	pipipi(i + 1);

	while (1) {
		if (ButtonB == 0) {
			pii(((i + 1) - ((i + 1) % 4)) / 4);
			pipipi((i + 1) % 4);
			break;
		} else if (ButtonA == 0) {
			i = (i + 1) % total_mode;
			pii(((i + 1) - ((i + 1) % 4)) / 4);
			pipipi((i + 1) % 4);
			while (ButtonA == 0);
		}
	}

	return i;
}

static void mode1(void)
{
	main_routine();
	go_home();
}

static void mode2(void)
{
	capture_top_of_piramid();

	while (1) {
		play_music(&japari, 1);
	}
	while (1) {
		xbee_printf("LOW: %04u\tHIGH: %04u\tJUDGE: %04u\r\n", sensor_val[PSD_LOW], sensor_val[PSD_HIGH], sensor_val[PSD_JUDGE_CAPTURED]);
		delay_ms(200);
	}
}

static void mode3(void)
{
	start_sensor_calibration();
	while (!ButtonA);
	while (ButtonA);


	MOTOR_WAKE_UP();
	MOTOR_R_ENABLE();
	MOTOR_L_ENABLE();
	motorR_direction = MOTOR_R_FORWARD;
	motorL_direction = MOTOR_L_FORWARD;

	run_mode = LINETRACE;

	motorR.target_speed = 1200;
	motorL.target_speed = 1200;

	motorR_direction = MOTOR_R_FORWARD;
	motorL_direction = MOTOR_L_FORWARD;

	MOTOR_R_ENABLE();
	MOTOR_L_ENABLE();

	while (1) {
		if (motorR.speed > motorR.target_speed)MOTOR_R_DISABLE();
		if (motorL.speed > motorL.target_speed)MOTOR_L_DISABLE();

		//if ((motorR.speed > motorR.target_speed) && (motorL.speed > motorL.target_speed)) break;
	}



	linetrace(-1);

}

static void mode4(void)
{
	enum object object;
	while (1) {
		struct rgb rgb_color;
		read_color(&rgb_color);
		object = identify_object(&rgb_color);
		
		if (object == UNKNOWN_OBJECT) pii(3);
		else pipipi(object);
	}
	return;
}

static void mode5(void)
{
	
}

static void mode6(void)
{
	wake_up_all_servo();

	//action_servo(NECK, 45);
	//return;

	while (1) {
		lift_neck();
		LED_A = 1;
		delay_ms(1000);
		LED_A = 0;
		open_arm();
		delay_ms(1000);
		down_neck();
		LED_A = 1;
		delay_ms(1000);
		LED_A = 0;
		close_arm();
		delay_ms(1000);
	}

	sleep_all_servo();
}

static void mode7(void)
{
	//sensor_test();
	while (1) {
		struct rgb rgb_color;
		struct hsv hsv_color;
		enum object object;
		rgb2hsv(&rgb_color, &hsv_color);
		read_color(&rgb_color);
		object = identify_object(&rgb_color);
		xbee_printf("R: %u,\tG: %u,\tB: %u,\tOBJ: %d\r\n", rgb_color.red, rgb_color.green, rgb_color.blue, object);
		xbee_printf("H: %u,\tS: %u,\tV: %u,\tOBJ: %d\r\n", hsv_color.hue, hsv_color.saturation, hsv_color.value, object);

		delay_ms(200);
	}
}

static void mode8(void)
{
}