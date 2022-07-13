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
			
			case 0x211:
			{
				
			}break;
			
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
