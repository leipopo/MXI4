#ifndef ROBCFG_H
#define ROBCFG_H

#define fircmotid_1 0x00000201
#define fircmotid_2 0x00000202
#define trigmotid 0x00000201

#define yawmotid 0x00000207
#define pitmotid 0x00000208
#define wheid 0x00000201
#define steid 0x00000205

#define mpuid 0x00000123
#define capsid 0x00000211

#define mottaskperi 2
#define comutaskperi 1
#define infotaskperi 1
#define reftaskperi 10
#define UIdrawperi 50
#define capstaskperi 100

#define fre(peri) 1000/peri


#endif // !ROBCFG_H
