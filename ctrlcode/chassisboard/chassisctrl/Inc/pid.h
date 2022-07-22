#ifndef PID_H
#define PID_H

#define PID_COMPONENT_NOLIMIT 60000
#define PID_OUTPUT_MAX_LIMIT 14000
#define LIMIT(X, MIN, MAX) ((X) < (MIN) ? (MIN) : ((X) > (MAX) ? (MAX) : (X)))

typedef struct PID_regulator_t
{
	float tar;
	float cur;
	float err[2];
	float kp;
	float ki;
	float kd;
	float componentKp;
	float componentKi;
	float componentKd;
	float componentKpMax;
	float componentKiMax;
	float componentKdMax;
	float output;
	float outputMax;
} PID_regulator;

#define pid_default_config \
	{                      \
		0.f,               \
			0.f,           \
			{0, 0},        \
			0.f,           \
			0.f,           \
			0.f,           \
			0.f,           \
			0.f,           \
			0.f,           \
			0.f,           \
			0.f,           \
			0.f,           \
			0.f,           \
			0.f,           \
	}

void PID_clear(PID_regulator *pid);
void PID_calc(PID_regulator *pid, float timetick);
void PID_angle_calc(PID_regulator *pid, float timetick, float angle_parameter);
#endif
