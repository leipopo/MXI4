#include "main.h"

void DMA_RX_INIT(UART_HandleTypeDef *huartx,
				 DMA_HandleTypeDef *hdma_usartx_rx,
				 uint8_t *rx1_buf,
				 uint8_t *rx2_buf,
				 uint16_t dma_buf_num)
{
	SET_BIT(huartx->Instance->CR3, USART_CR3_DMAR);
	__HAL_UART_ENABLE_IT(huartx, UART_IT_IDLE);
	__HAL_DMA_DISABLE(huartx->hdmarx);
	while (hdma_usartx_rx->Instance->CR & DMA_SxCR_EN)
	{
		__HAL_DMA_DISABLE(huartx->hdmarx);
	}
	hdma_usartx_rx->Instance->PAR = (uint32_t) & (huartx->Instance->DR);
	hdma_usartx_rx->Instance->M0AR = (uint32_t)(rx1_buf);
	hdma_usartx_rx->Instance->M1AR = (uint32_t)(rx2_buf);
	hdma_usartx_rx->Instance->NDTR = dma_buf_num;
	SET_BIT(hdma_usartx_rx->Instance->CR, DMA_SxCR_DBM);
	__HAL_DMA_ENABLE(huartx->hdmarx);
}
