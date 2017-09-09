#ifndef SENSOR_H
#define SENSOR_H

#include <stdio.h>
#include <stdint.h>
#include "device.h"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#pragma GCC diagnostic warning "-Wsign-conversion"

#define COLOR_ADDTIME 400 //ms カラーセンサーの積算時間

#define NUMBER_OF_SENSORS 6 + 2 + 1 //IR + PSD

//shit not used
#define PSD_LOW   4
#define PSD_HIGH  5
#define PSD_JUDGE_CAPTURED 6


#define GOAL_MARKER 6
#define CORNER_MARKER 7

extern __IO uint16_t sensor_val[NUMBER_OF_SENSORS];
extern __IO uint16_t fix_sensor_val[NUMBER_OF_SENSORS];
extern int32_t fix_sensors_value;
extern unsigned int judge_color_standard;

extern struct object_aggregation aggregation;

enum object {
	UNKNOWN_OBJECT = -1, NONE = 0, RED_BALL = 1, BLUE_BALL, YELLOW_CAN, PETBOTTLE
};

struct object_aggregation {
	size_t red_ball, yellow_can, blue_ball, petbottle;
	size_t unknown_object;
	uint8_t collected_pyramid_flag;
};

struct rgb {
	uint16_t red, green, blue;
};

struct hsv {
	uint16_t hue, saturation, value;
};

void start_sensor_calibration(void);
void sensor_test(void);

void read_color(struct rgb *color);
enum object identify_object(const struct rgb *object_rgb_color);
void rgb2hsv(const struct rgb *rgb_color, struct hsv *hsv_color);

void aggregate_identified_object(struct object_aggregation *object_aggregation, enum object identified_object);

#endif /* SENSOR_H */
