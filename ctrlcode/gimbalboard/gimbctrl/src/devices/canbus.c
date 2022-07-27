#include "main.h"

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    CAN_RxHeaderTypeDef rx_header;
    uint8_t rx_data[8];

    HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rx_header, rx_data);

    if (hcan->Instance == CAN1)
    {
        if (rx_header.StdId == fricmotid_1)
        {
            canrx2motinfo(rx_data, fric);
        }
        else if (rx_header.StdId == fricmotid_2)
        {
            canrx2motinfo(rx_data, fric+1);
        }
    }
    else if (hcan->Instance == CAN2)
    {
        if (rx_header.StdId == chasboardid)
        {
            canrx2comuinfo_comd(rx_data, &comuinfo);
        }
    }
}
