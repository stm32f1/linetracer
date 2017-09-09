#ifndef CAPTURE_H
#define CAPTURE_H

#include <stdbool.h>
#include "sensor.h"

extern const unsigned int search_deg;
extern bool PET_captured;
extern bool Piramid_captured;
bool search_and_capture_object(void);

int get_object_direction(unsigned int deg, bool ignore_petbottle);
int32_t get_close_to_object(void);
bool judge_get_object(void);
unsigned int object_distance(void);

void capture_top_of_piramid(void);

#endif /* CAPTURE_H */

