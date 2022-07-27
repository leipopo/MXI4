#ifndef ROBCFG_H
#define ROBCFG_H

#define fircmotid_1 0x00000201
#define fircmotid_2 0x00000202
#define trigmotid 0x00000201

#define yawmotid 0x00000205
#define pitmotid 0x00000206
#define wheid 0x00000201
#define steid 0x00000205

#define capsid 0x00000211

#define chasboardid 0x00000233
#define gimbboardid_imu 0x00000222
#define gimbboardid_cv 0x00000333

#define mottaskperi 2
#define comutaskperi 2
#define infotaskperi 2
#define reftaskperi 10
#define UIdrawperi 50
#define capstaskperi 100

#define fre(peri) 1000/peri


#endif // !ROBCFG_H
