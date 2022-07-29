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
            can1devsta= can_probe(fricmotid_1,can1_idlist);
        }
        else if (rx_header.StdId == fricmotid_2)
        {
            canrx2motinfo(rx_data, fric+1);
            can1devsta= can_probe(fricmotid_2,can1_idlist);
        }
    }
    else if (hcan->Instance == CAN2)
    {
        if (rx_header.StdId == chasboardid)
        {
            canrx2comuinfo_comd(rx_data, comuinfo);
            can2devsta= can_probe(chasboardid,can2_idlist);
        }
        else if (rx_header.StdId == trigmotid)
        {
            canrx2motinfo(rx_data, &trig);
            can2devsta= can_probe(trigmotid,can2_idlist);
        }
    }
}
