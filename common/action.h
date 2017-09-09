#ifndef ACTION_H
#define ACTION_H

typedef enum {
	FORWARD, BACK, RIGHT, LEFT
} DIRECTION, *PDIRECTION;

//void action_servo(char servo_i, int deg);
void move(DIRECTION directory, unsigned int distance);
void move_step(DIRECTION direction, unsigned int step);
void move_for_search(DIRECTION direction, unsigned int distance);
void touch_wall(void);
void cleaning(void);

#endif /* ACTION_H */
