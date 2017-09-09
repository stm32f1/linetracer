#ifndef SERVO_H
#define SERVO_H

typedef enum {
	NECK, RIGHT_ARM, LEFT_ARM
} SERVO_NAME;

void wake_up_all_servo(void);
void sleep_all_servo(void);
void action_servo(SERVO_NAME servo_name, int deg);
void open_arm(void);
void close_arm(void);
void lift_neck(void);
void down_neck(void);
void lift_neck_top(void);
void get_object(void);
void get_top_of_pyramid(void);
void release_object(void);
void normalize_arm(void);

#endif /* SERVO_H */

