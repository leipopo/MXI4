#ifndef CAN_BSP_H
#define CAN_BSP_H
#include "can.h"

void can_filter_init(void);

void CAN_send(uint32_t address,\
							CAN_HandleTypeDef hcanx,\
							int16_t message[4]
							);
                            

#endif
