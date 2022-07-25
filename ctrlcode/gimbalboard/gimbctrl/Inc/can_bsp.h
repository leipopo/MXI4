#ifndef CAN_BSP_H
#define CAN_BSP_H
#include "can.h"

extern int16_t zeromes[4];

void can_filter_init(void);

void CAN_send(uint32_t address,
              CAN_HandleTypeDef hcanx,
              int16_t message[4]);

#endif
