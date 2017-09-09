#include "position.h"
#include "sensor.h"
#include "comutil.h"
#include "xbee.h"



void send_area(enum area area)
{
	switch (area) {
		case START_AREA:
			xbee_printf("AS\n");
			// Area + Start_area
			break;
		case RED_GOAL:
			xbee_printf("AR\n");
			// Area + Red_goal
			break;
		case YELLOW_GOAL:
			xbee_printf("AY\n");
			// Area + Yellow_goal
			break;
		case BLUE_GOAL:
			xbee_printf("AB\n");
			// Area + Blue_goal
			break;
		case FREE_ZONE:
			xbee_printf("AF\n");
			// Area + Free_zone
			break;
		case OBJECT_ZONE:
			xbee_printf("AO\n");
			// Area + Object_zone
			break;
	}
}


void send_captured_object(enum object object)
{
	switch (object) {
		case RED_BALL:
			xbee_printf("OR\n");
			// Object + Red_ball
			break;
		case BLUE_BALL:
			xbee_printf("OB\n");
			// Object + Blue_ball
			break;
		case YELLOW_CAN:
			xbee_printf("OY\n");
			// Object + Yellow_can
			break;
		case PETBOTTLE:
			xbee_printf("OP\n");
			// Object + Petbottle
			break;
		case UNKNOWN_OBJECT:
			xbee_printf("OU\n");
			// Object + Unknown_object
			break;
		default: inform_error();break;
	}
}



void proccess_received_data(const uint8_t *data)
{
	if (data[0] == 'A') {
		switch (data[1]) {
			case 'S':
				partner_area = START_AREA;
				break;
			case 'R':
				partner_area = RED_GOAL;
				break;
			case 'Y':
				partner_area = YELLOW_GOAL;
				break;
			case 'B':
				partner_area = BLUE_GOAL;
				break;
			case 'F':
				partner_area = FREE_ZONE;
				break;
			case 'O':
				partner_area = OBJECT_ZONE;
				break;
		}
	} else if (data[0] == 'O') {
		switch (data[1]) {
			case 'R':
				aggregate_identified_object(&aggregation, RED_BALL);
				break;
			case 'B':
				aggregate_identified_object(&aggregation, BLUE_BALL);
				break;
			case 'Y':
				aggregate_identified_object(&aggregation, YELLOW_CAN);
				break;
			case 'P':
				aggregate_identified_object(&aggregation, PETBOTTLE);
				break;
			case 'U':
				aggregate_identified_object(&aggregation, UNKNOWN_OBJECT);
				break;
		}
	}
}
