#include <stdio.h>
#include "device.h"
#include "utility.h"
#include "sensor.h"
#include "motor.h"

__IO uint16_t sensor_val[NUMBER_OF_SENSORS] = {0};
__IO uint16_t fix_sensor_val[NUMBER_OF_SENSORS] = {0};
int32_t fix_sensors_value;

unsigned int judge_color_standard = 800;

struct object_aggregation aggregation = {0};

void start_sensor_calibration(void)
{
	fix_sensors_value = 0;
	delay_ms(1000);
	for (size_t i = 0; i < 5; i++) {
		for (size_t j = 0; j < NUMBER_OF_SENSORS;)fix_sensor_val[j] += sensor_val[j];

		fix_sensors_value += sensor_val[0]*2 + sensor_val[1]
				- sensor_val[2] - sensor_val[3]*2;
		delay_ms(10);
	}
	for (size_t j = 0; j < NUMBER_OF_SENSORS;)fix_sensor_val[j] /= 5;
	fix_sensors_value /= 5;
	pii(1);
}

void sensor_test(void)
{
	signed int control_input;
	start_sensor_calibration();
	while (1) {
		control_input = (signed int) (sensor_val[0]*2 + sensor_val[1] - sensor_val[2]
				- sensor_val[3]*2 - fix_sensors_value);
		printf(" sensor0  %u\n sensor1  %u\n sensor2  %u\n sensor3  %u\n  control_input  %d\n\n"
				, (unsigned int) sensor_val[0], (unsigned int) sensor_val[1]
				, (unsigned int) sensor_val[2], (unsigned int) sensor_val[3], control_input);
		pii(1);
		delay_ms(1000);
	}
}

void read_color(struct rgb *color)
{
	color->red = 0;
	color->green = 0;
	color->blue = 0;

	color_LED = 1;
	delay_ms(5);
	color_GATE = 0;
	color_CK = 0;
	delay_us(10);
	color_GATE = 1;
	delay_ms(COLOR_ADDTIME);
	color_GATE = 0;
	delay_us(10);

	for (uint16_t i = 0; i < 12; i++) {
		color_CK = 1;
		color->red |= (uint16_t) (color_DOUT << i);
		delay_us(1);
		color_CK = 0;
		delay_us(1);
	}
	delay_us(5);

	for (uint16_t i = 0; i < 12; i++) {
		color_CK = 1;
		color->green |= (uint16_t) (color_DOUT << i);
		delay_us(1);
		color_CK = 0;
		delay_us(1);
	}
	delay_us(5);

	for (uint16_t i = 0; i < 12; i++) {
		color_CK = 1;
		color->blue |= (uint16_t) (color_DOUT << i);
		delay_us(1);
		color_CK = 0;
		delay_us(1);
	}
	delay_us(5);

	color_LED = 0;
	return;
}

enum object identify_object(const struct rgb *object_rgb_color)
{
	struct hsv hsv_color;
	rgb2hsv(object_rgb_color, &hsv_color);

	if (hsv_color.value >= 100) {
		if ((hsv_color.hue >= 40)&&(hsv_color.hue < 160))
			return YELLOW_CAN;
		if ((hsv_color.hue >= 160)&&(hsv_color.hue < 280))
			return BLUE_BALL;
		if ((hsv_color.hue >= 280) || (hsv_color.hue < 40))
			return RED_BALL;
	}

	return UNKNOWN_OBJECT;
}

void rgb2hsv(const struct rgb *rgb_color, struct hsv *hsv_color)
{
	uint16_t r = rgb_color->red;
	uint16_t g = rgb_color->green;
	uint16_t b = rgb_color->blue;
	uint16_t s, v;
	int16_t h; // 計算過程で負の数になることが有る為

	uint16_t max = 0, min = 255;

	r /= 16;
	g /= 16;
	b /= 16;

	min = (r < g) ? r : g;
	min = (min < b) ? min : b;
	max = (r > g) ? r : g;
	max = (max > b) ? max : b;

	if (max == min) {
		h = 0;
	} else {
		h = (int16_t) (60 * ((max - r) ? (max - g) ? (r - g) : (b - r) : (g - b)) / (max - min)+((max - r) ? (max - g) ? 240 : 120 : 0));
		if (h < 0) {
			h = (int16_t) (h + 360);
		}
	}
	if (max == 0) {
		s = 0;
	} else {
		s = (uint16_t) (255 * (max - min) / max);
	}
	v = max;

	hsv_color->hue = (uint16_t) h;
	hsv_color->saturation = s;
	hsv_color->value = v;
}

void aggregate_identified_object(struct object_aggregation *object_aggregation, enum object identified_object)
{
	switch (identified_object) {
	case RED_BALL:
		object_aggregation->red_ball++;
		break;
	case BLUE_BALL:
		object_aggregation->blue_ball++;
		break;
	case YELLOW_CAN:
		object_aggregation->yellow_can++;
		break;
	case PETBOTTLE:
		object_aggregation->petbottle++;
		break;
	case UNKNOWN_OBJECT:
		object_aggregation->unknown_object++;
		break;
	default: inform_error();
		break;
	}
}
