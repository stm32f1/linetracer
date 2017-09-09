#ifndef XBEE_H
#define XBEE_H

#include <stdio.h>
#include <stdint.h>
#include "sensor.h"
#include "position.h"

void send_area(enum area area);
void send_captured_object(enum object object);
void proccess_received_data(const uint8_t *data);

#endif /* XBEE_H */
