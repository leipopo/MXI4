#ifndef ROBCFG_H
#define ROBCFG_H

#define fircmotid_1 0x00000201
#define fircmotid_2 0x00000202
#define trigmotid 0x00000201

#define yawmotid 0x0000020A
#define pitmotid 0x0000020B
#define wheid 0x00000201
#define steid 0x00000205

#define capsid 0x00000211

#define chasboardid 0x00000233
#define gimbboardid_yaw 0x00000666
#define gimbboardid_pit 0x00000333
#define gimbboardid_cv 0x00000999

#define mottaskperi 2
#define comutaskperi 50
#define infotaskperi 2
#define reftaskperi 10
#define UIdrawperi 100
#define capstaskperi 100

#define fre(peri) 1000/peri


#endif // !ROBCFG_H
