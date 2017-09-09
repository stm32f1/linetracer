#include <stdio.h>  
#include <stdint.h>
#include <stdbool.h>
#include "common/device.h"
#include "common/action.h"
#include "capture.h"
#include "common/motor.h"
#include "common/servo.h"
#include "common/sensor.h"
#include "common/linetrace.h"
#include "subroutine.h"
#include "common/comutil.h"
#include "common/position.h"
#include "common/xbee.h"
#include "common/utility.h"
#include "common/debug.h"

void main_routine(void)
{
	debug_printf("LET'S GO!!\r\n");
	wake_up_all_servo();
	start_sensor_calibration();
	MOTOR_WAKE_UP();

	linetrace(1);
	
	MOTOR_SLEEP();
}

void go_object_zone_from_start_zone(void)
{
	touch_wall();
	move(FORWARD, 50);
	move(RIGHT, 90);
	touch_wall();
	move(FORWARD, 50);
	move(LEFT, 90);
	move(FORWARD, 700);
	move(LEFT, 90);
	open_arm();
	delay_s(1);
	move(BACK, 20);
	close_arm();
	move(RIGHT, 140);
	sleep_all_servo();
	move(FORWARD, 350);
	move(LEFT, 50);
	get_on_line(100);
	linetrace(3);
	run_mode = STOP;
}

void go_home(void)
{
	move(FORWARD, 200);
	move(RIGHT, 180);
	linetrace(4);
	move(LEFT, 135);
	move(BACK, 350);
	move(LEFT, 45);
	touch_wall();
	MOTOR_SLEEP();
}

void throw_away_object(enum object object)
{
	switch (object) {
	case RED_BALL:
		linetrace(4);
		move(RIGHT, 90);
		move(FORWARD, 200);
		down_neck();
		open_arm();
		lift_neck();
		delay_ms(300);
		close_arm();
		move(BACK, 200);
		move(RIGHT, 90);
		get_on_line(50);
		linetrace(3);
		break;
	case BLUE_BALL:
		linetrace(2);
		move(FORWARD, 500);
		down_neck();
		open_arm();
		lift_neck();
		delay_ms(300);
		close_arm();
		move(RIGHT, 185);
		get_on_line(400);
		linetrace(2);
		break;
	case UNKNOWN_OBJECT:
		//とりあえず捨てとく
                linetrace(3);
                move(RIGHT, 90);
                move(FORWARD, 200);
                down_neck();
                open_arm();
                lift_neck();
                delay_ms(300);
                close_arm();
                move(BACK, 200);
                move(RIGHT, 90);
                get_on_line(50);
                linetrace(2);
                break;
	case YELLOW_CAN:
		linetrace(3);
		move(RIGHT, 90);
		move(FORWARD, 200);
		down_neck();
		open_arm();
		lift_neck();
		delay_ms(300);
		close_arm();
		move(BACK, 200);
		move(RIGHT, 90);
		get_on_line(50);
		linetrace(2);
		break;
	case PETBOTTLE:
		move(BACK, 200);
		move(RIGHT, 90);
		lift_neck();
		move(FORWARD, 450); //must be fixed
		action_servo(NECK, 35);
                delay_ms(750);
                open_arm();
                delay_ms(200);
                lift_neck();
		move(BACK, 450);
		move(LEFT, 90);
		move(BACK, 900); //???
		touch_wall();
		move(FORWARD, 250);
		move(RIGHT, 90);
		touch_wall();
		move(FORWARD, 460); //460 - 後ろから中心
		move(LEFT, 90);
		move(FORWARD, 250);
		break;
	default:
		break;
	}
	sleep_all_servo();
	if (object != PETBOTTLE) {
		my_area = OBJECT_ZONE;
		send_area(my_area);
	}
}