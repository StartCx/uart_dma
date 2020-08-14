#include "uart_dma.h"

#include "usart.h"
#include <stdio.h>


#define RX_BUF_NUM 36u


extern UART_HandleTypeDef huart1;
extern DMA_HandleTypeDef hdma_usart1_rx;


uint8_t sbus_rx_buf[RX_BUF_NUM]={0};

int fputc(int ch, FILE *f)
{
	
	HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 1000);	
	
	return (ch);
}


int fgetc(FILE *f)
{
		
	int ch;
	HAL_UART_Receive(&huart1, (uint8_t *)&ch, 1, 1000);	
	return (ch);
}


void DMA_config(uint8_t *rx_buf, uint16_t dma_buf_num)
{
   
    SET_BIT(huart1.Instance->CR3, USART_CR3_DMAR);
	
    __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
    __HAL_DMA_DISABLE(&hdma_usart1_rx);
	
    while(hdma_usart1_rx.Instance->CR & DMA_SxCR_EN)
    {
        __HAL_DMA_DISABLE(&hdma_usart1_rx);
    }

    hdma_usart1_rx.Instance->PAR = (uint32_t) & (USART1->DR);
    
    hdma_usart1_rx.Instance->M0AR = (uint32_t)(rx_buf);
	hdma_usart1_rx.Instance->M1AR = (uint32_t)(rx_buf);
    
    hdma_usart1_rx.Instance->NDTR = dma_buf_num;
   
    SET_BIT(hdma_usart1_rx.Instance->CR, DMA_SxCR_DBM);

    __HAL_DMA_ENABLE(&hdma_usart1_rx);
}


void UART_DMA(void)
{
	DMA_config(&sbus_rx_buf[0], RX_BUF_NUM);
}


extVisionRecvData_t    VisionRecvData;

//static void sbus_to_rc(volatile const uint8_t *sbus_buf, RC_ctrl_t *rc_ctrl)
//{
//    if (sbus_buf == NULL || rc_ctrl == NULL)
//    {
//        return;
//    }

//    rc_ctrl->rc.ch[0] = (sbus_buf[0] | (sbus_buf[1] << 8)) & 0x07ff;       
//    rc_ctrl->rc.ch[1] = ((sbus_buf[1] >> 3) | (sbus_buf[2] << 5)) & 0x07ff; 
//    rc_ctrl->rc.ch[2] = ((sbus_buf[2] >> 6) | (sbus_buf[3] << 2) |          
//                         (sbus_buf[4] << 10)) &0x07ff;
//    rc_ctrl->rc.ch[3] = ((sbus_buf[4] >> 1) | (sbus_buf[5] << 7)) & 0x07ff; 
//    rc_ctrl->rc.s[0] = ((sbus_buf[5] >> 4) & 0x0003);                  
//    rc_ctrl->rc.s[1] = ((sbus_buf[5] >> 4) & 0x000C) >> 2;                       
//    rc_ctrl->mouse.x = sbus_buf[6] | (sbus_buf[7] << 8);                    
//    rc_ctrl->mouse.y = sbus_buf[8] | (sbus_buf[9] << 8);                    
//    rc_ctrl->mouse.z = sbus_buf[10] | (sbus_buf[11] << 8);                  
//    rc_ctrl->mouse.press_l = sbus_buf[12];                                  
//    rc_ctrl->mouse.press_r = sbus_buf[13];                                  
//    rc_ctrl->key.v = sbus_buf[14] | (sbus_buf[15] << 8);                   
//    rc_ctrl->rc.ch[4] = sbus_buf[16] | (sbus_buf[17] << 8);                


//}



void USART1_IRQHandler(void)
{
    if(huart1.Instance->SR & UART_FLAG_RXNE)//接收到数据
    {
        __HAL_UART_CLEAR_PEFLAG(&huart1);
    }
    else if(USART1->SR & UART_FLAG_IDLE)
    {
        static uint16_t this_time_rx_len = 0;

        __HAL_UART_CLEAR_PEFLAG(&huart1);

       
		__HAL_DMA_DISABLE(&hdma_usart1_rx);

		this_time_rx_len = RX_BUF_NUM - hdma_usart1_rx.Instance->NDTR;

		hdma_usart1_rx.Instance->NDTR = RX_BUF_NUM;

		DMA1_Stream1->CR &= ~(DMA_SxCR_CT);

		__HAL_DMA_ENABLE(&hdma_usart1_rx);

		if(this_time_rx_len == 18)
		{
			//sbus_to_rc(sbus_rx_buf[0], &rc_ctrl);
		}
    }
}



