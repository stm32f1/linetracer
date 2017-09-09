#include "device.h"
#include "sensor.h"
#include "position.h"

enum area my_area = START_AREA;
enum area partner_area = START_AREA;  //volatile要らない気がするけど

enum area identify_area(const struct position *position)
{
	if (position->x >= 0 && position->x <= 600) {
		if (position->y >= 0 && position->y <= 500)
			return START_AREA;
		if ((position->y >= 500) && (position->y <= 1075))
			return RED_GOAL;
		if ((position->y >= 1075) && (position->y <= 1725))
			return YELLOW_GOAL;
		if ((position->y >= 1725) && (position->y <= 2300))
			return BLUE_GOAL;
	} else if (position->x >= 880 &&
			position->x <= 1800 &&
			position->y >= 500 &&
			position->y <= 1800) {
		return OBJECT_ZONE;
	}
	return FREE_ZONE;
}



enum object_area identify_object_area(const struct position *position)
{
	if (position->x <= 1340) {
		if (position->y < (500+380)) {
			return C1;
		}
		if (position->y < (500+380+300)) {
			return B1;
		}
		if (position->y < (500+380+300*2)) {
			return D3;
		}
		if (position->y < (500+380+300*3)) {
			return D1;
		}
	} else {
		if (position->y < (500+380)) {
			return C2;
		}
		if (position->y < (500+380+300)) {
			return B2;
		}
		if (position->y < (500+380+300*2)) {
			return D4;
		}
		if (position->y < (500+380+300*3)) {
			return D2;
		}
	}
	return SOMEWHERE;
}



enum area convert_object_to_area(enum object object)
{
	enum area area;
	switch (object) {
		case RED_BALL:
			area = RED_GOAL;
			break;
		case BLUE_BALL:
			area = BLUE_GOAL;
			break;
		case YELLOW_CAN:
			area = YELLOW_GOAL;
			break;
		case PETBOTTLE:
			area = OBJECT_ZONE;
			break;
		case UNKNOWN_OBJECT:
			area = RED_GOAL;
			// 後でまた考える
			break;
	}
	return area;
}
