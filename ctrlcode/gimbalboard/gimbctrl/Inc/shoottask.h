#ifndef SHOOTTASK_H
#define SHOOTTASK_H

#define shootfreq      24.f
#define forknum        8.f
#define trigspeed      shootfreq / forknum * 60.f
#define trigclickangle 360.f / forknum

#define fricwheelspeed -5000.f //-6000.f to 26

#define magopenpusle   500
#define magclosepusle  2200

extern MotorInfo fric[2], trig;
extern PID_regulator fricspid[2], trigapid, trigspid;

void init_shootmot_para(MotorInfo fric[2], MotorInfo *trig);
void init_shootmot_pid(PID_regulator fspid[2],
                       PID_regulator *tapid,
                       PID_regulator *tspid);
#endif // !SHOOT