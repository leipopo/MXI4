#include "main.h"

int8_t NUC_ON = 0;
NUC_DATA NUC_data;
uint8_t NUC_rx_buf[2][NUC_RX_BUF_NUM];

void NUC_INIT(void)
{
    DMA_RX_INIT(&huart1, &hdma_usart1_rx, NUC_rx_buf[0],NUC_rx_buf[1], NUC_RX_BUF_NUM);
}

void NUC_RX_Decoder(uint8_t *NUC_buf, NUC_DATA *nd)
{
    nd->yaw   = ((int16_t)((NUC_buf[1] << 8) | NUC_buf[2])) * 100.f / 32767.f;
    nd->pitch = ((int16_t)((NUC_buf[3] << 8) | NUC_buf[4])) * 100.f / 32767.f;
}

void USART1_IRQHandler(void)
{
    if (huart1.Instance->SR & UART_FLAG_RXNE)
    {
        __HAL_UART_CLEAR_IDLEFLAG(&huart1);
    }
    else if (huart1.Instance->SR & UART_FLAG_IDLE)
    {
        static uint16_t this_time_rx_len = 0;
        __HAL_UART_CLEAR_PEFLAG(&huart1);
        this_time_rx_len = NUC_RX_BUF_NUM - hdma_usart1_rx.Instance->NDTR;
        hdma_usart1_rx.Instance->NDTR = NUC_RX_BUF_NUM;
        if ((hdma_usart1_rx.Instance->CR & DMA_SxCR_CT) == RESET)
        {
            hdma_usart1_rx.Instance->CR |= DMA_SxCR_CT;
            __HAL_DMA_ENABLE(&hdma_usart1_rx);
            if (this_time_rx_len == NUC_FRAME_LENGTH)
            {
                NUC_RX_Decoder(NUC_rx_buf[0], &NUC_data);
            }
        }
        else
        {
            DMA2_Stream5->CR &= ~(DMA_SxCR_CT);
            __HAL_DMA_ENABLE(&hdma_usart1_rx);
            if (this_time_rx_len == NUC_FRAME_LENGTH)
            {
                NUC_RX_Decoder(NUC_rx_buf[1], &NUC_data);
            }
        }
    }
    // if(__HAL_UART_GET_FLAG(&huart1,UART_FLAG_IDLE)!= RESET)//Èç¹û½ÓÊÜµ½ÁËÒ»Ö¡Êý¾Ý
	// { 
    //     __HAL_UART_CLEAR_IDLEFLAG(&huart1); // 清除标志位
    //     HAL_UART_DMAStop(&huart1);          //
    //     // temp                             = __HAL_DMA_GET_COUNTER(&hdma_usart1_rx); // 获取DMA中未传输的数据个数
    //     //  temp  = hdma_usart1_rx.Instance->NDTR;//读取NDTR寄存器 获取DMA中未传输的数据个数，
    //     // 这句和上面那句等效
    //     // static uint16_t this_time_rx_len = 0;
    //     // this_time_rx_len                 = NUC_RX_BUF_NUM - temp; // 总计数减去未传输的数据个数，得到已经接收的数据个数
    //     NUC_RX_Decoder(NUC_rx_buf, &NUC_data);
    // }
    HAL_UART_IRQHandler(&huart1);
}
