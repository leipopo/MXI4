#include "main.h"

int8_t NUC_ON = 0;
NUC_DATA NUC_data;
uint8_t NUC_rx_buf[2][NUC_RX_BUF_NUM];

void NUC_INIT(void)
{
    DMA_RX_INIT(&huart1, &hdma_usart1_rx, NUC_rx_buf[0],NUC_rx_buf[1], NUC_RX_BUF_NUM);
}

void NUC_RX_Decoder(uint8_t *NUC_buf,NUC_DATA *nd)
{
    nd->yaw = ((int16_t)((NUC_buf[1] << 8) | NUC_buf[2]))*100.f/32767.f;
    nd->pitch = ((int16_t)((NUC_buf[3] << 8) | NUC_buf[4]))*100.f/32767.f;
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
}
