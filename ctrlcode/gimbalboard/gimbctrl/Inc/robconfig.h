#ifndef ROBCFG_H
#define ROBCFG_H

#define fricmotid_1 0x00000201
#define fricmotid_2 0x00000202
#define trigmotid 0x00000203

#define yawmotid 0x0000020A
#define pitmotid 0x0000020B
#define wheid 0x00000201
#define steid 0x00000205

#define capsid 0x00000211

#define chasboardid 0x00000233
#define gimbboardid_angle 0x00000666
#define gimbboardid_speed 0x00000333
#define gimbboardid_cv 0x00000999
#define mottaskperi 2
#define comutaskperi 2
#define infotaskperi 2
#define reftaskperi 50
#define UIdrawperi 50
#define capstaskperi 100

#define fre(peri) 1000/peri


#endif // !ROBCFG_H
