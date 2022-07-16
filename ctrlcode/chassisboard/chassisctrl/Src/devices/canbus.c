#include "main.h"

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    CAN_RxHeaderTypeDef rx_header;
	uint8_t rx_data[8];

	HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rx_header, rx_data);

	if(hcan->Instance==CAN1)
	{
		switch(rx_header.StdId)
		{
			canitproc(whe[0],serid(wheid,0x00000000),can1_idlist);
			canitproc(whe[1],serid(wheid,0x00000001),can1_idlist);
			canitproc(whe[2],serid(wheid,0x00000002),can1_idlist);
			canitproc(whe[3],serid(wheid,0x00000003),can1_idlist);
			canitproc(ste[0],serid(steid,0x00000000),can1_idlist);
			canitproc(ste[1],serid(steid,0x00000001),can1_idlist);
			canitproc(ste[2],serid(steid,0x00000002),can1_idlist);
			canitproc(ste[3],serid(steid,0x00000003),can1_idlist);
		}
		//HAL_IWDG_Refresh(&hiwdg);
	}
	else if (hcan->Instance==CAN2)
	{
		switch(rx_header.StdId)
		{
			
			
        }
    }
}
