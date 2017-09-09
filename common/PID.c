#include "PID.h"

#define kp 0.04
#define ki 0
#define kd 0.001//.1

#define kp_a 0.01
#define ki_a 0
#define kd_a 0

signed int calc_PID(signed int deviation)
{
	static signed int last_deviation = 0;
	static signed int integral = 0;
	signed int differential;
	signed int operation;

	integral += deviation;
	differential = deviation - last_deviation;

	operation = (signed int) (kp * deviation + ki * integral + kd * differential);

	last_deviation = deviation;

	return operation;
}

signed int integral_a[10] = {0};
signed int last_deviation_a = 0;
signed int integral_position_a = 0;

signed int calc_PID_a(signed int deviation)
{
	signed int differential;
	signed int operation;

	integral_a[0] += (signed int) (deviation * 0.001);
	differential = deviation - last_deviation_a;

	operation = (signed int) (kp_a * deviation + ki_a * integral_a[0] + kd_a * differential);

	last_deviation_a = deviation;

	return operation;
}

signed int PID(signed int deviation, double Kp, double Ki, double Kd)
{
	signed int differential;
	signed int operation;

	integral_a[0] += (signed int) (deviation * 0.001);
	differential = deviation - last_deviation_a;

	operation = (signed int) (Kp * deviation + Ki * integral_a[0] + Kd * differential);

	last_deviation_a = deviation;

	return operation;
}
