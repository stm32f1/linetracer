#ifndef PID_H
#define PID_H

signed int calc_PID(signed int deviation);
signed int calc_PID_a(signed int deviation);
signed int PID(signed int deviation, double Kp, double Ki, double Kd);

extern signed int integral_a[10];
extern signed int last_deviation_a;
extern signed int integral_position_a;

#endif /* PID_H */
