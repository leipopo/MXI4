#include "main.h"

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    CAN_RxHeaderTypeDef rx_header;
    uint8_t rx_data[8];

    HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rx_header, rx_data);

    if (hcan->Instance == CAN1)
    {
        if (rx_header.StdId == whe[0].setup.motid)
        {
            canrx2motinfo(rx_data, &whe[0]);
            can1devsta = can_probe(whe[0].setup.motid, can1_idlist);
        }
        else if (rx_header.StdId == whe[1].setup.motid)
        {
            canrx2motinfo(rx_data, &whe[1]);
            can1devsta = can_probe(whe[1].setup.motid, can1_idlist);
        }
        else if (rx_header.StdId == whe[2].setup.motid)
        {
            canrx2motinfo(rx_data, &whe[2]);
            can1devsta = can_probe(whe[2].setup.motid, can1_idlist);
        }
        else if (rx_header.StdId == whe[3].setup.motid)
        {
            canrx2motinfo(rx_data, &whe[3]);
            can1devsta = can_probe(whe[3].setup.motid, can1_idlist);
        }
        else if (rx_header.StdId == yaw.setup.motid)
        {
            canrx2motinfo(rx_data, &yaw);
            can1devsta = can_probe(yaw.setup.motid, can1_idlist);
        }
        else if (rx_header.StdId == pit.setup.motid)
        {
            canrx2motinfo(rx_data, &pit);
            can1devsta = can_probe(pit.setup.motid, can1_idlist);
        }
    }
    else if (hcan->Instance == CAN2)
    {
        if (rx_header.StdId == capsid)
        {
            can2devsta = can_probe(capsid, can2_idlist);
        }
        else if (rx_header.StdId == gimbboardid_imuangle)
        {
            canrx2comuinfo_rximuangle(rx_data,&comuinfo);
            can2devsta = can_probe(gimbboardid_imuangle, can2_idlist);
        }
        // else if (rx_header.StdId == gimbboardid_imuspeed)
        // {
        //     canrx2comuinfo_rximuspeed(rx_data,&comuinfo);
        //     can2devsta = can_probe(gimbboardid_imuspeed, can2_idlist);
        // }
        else if (rx_header.StdId == gimbboardid_cv)
        {
            canrx2comuinfo_rxcv(rx_data,&comuinfo);
            can2devsta = can_probe(gimbboardid_cv, can2_idlist);
        }
    }
}
