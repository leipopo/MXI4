#ifndef GCTRL_H
#define GCTRL_H

#define yawreductionratio 60.f / 45.f
#define pitreductionratio 50.f / 20.f

extern MotorInfo yaw, pit;
void init_gimbmot_para(MotorInfo *pi, MotorInfo *yi);
void init_gimbmot_pid(PID_regulator *papid,
                      PID_regulator *yapid,
                      PID_regulator *pspid,
                      PID_regulator *yspid);
extern PID_regulator pitapid, yawapid, pitspid, yawspid;

#endif // !GCTRL_H
