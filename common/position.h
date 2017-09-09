#ifndef POSITION_H
#define POSITION_H

#include <stdint.h>
#include "sensor.h"

enum area {
	START_AREA, RED_GOAL, YELLOW_GOAL, BLUE_GOAL, FREE_ZONE, OBJECT_ZONE //FREE_ZONE must be changed cuz of stupid name
};


enum object_area {
	A, D1, D2, D3, D4, B1, B2, C1, C2, SOMEWHERE
};

//原点はスタート台の端
struct position {
	int x, y;
};

extern enum area my_area;
extern enum area partner_area;

enum area identify_area(const struct position *position);
enum area convert_object_to_area(enum object object);

#endif /* POSITION_H */

