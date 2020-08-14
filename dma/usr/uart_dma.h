#ifndef __UART_DMA_H
#define	__UART_DMA_H

#include "uart_dma.h"
#include "usart.h"
#include <stdio.h>


typedef __packed struct
{
	/* 头 */
	uint8_t   SOF;			//帧头起始位,暂定0xA5
	uint8_t   CmdID;		//指令
	uint8_t   CRC8;			//帧头CRC校验,保证发送的指令是正确的
	
	/* 数据 */
	float     pitch_angle;
	float     yaw_angle;
	float     distance;			//距离
	uint8_t   centre_lock;		//是否瞄准到了中间  0没有  1瞄准到了
	uint8_t	  identify_target;	//视野内是否有目标/是否识别到了目标   0否  1是	
	uint8_t   identify_buff;	//打符时是否识别到了目标，1是，2识别到切换了装甲，0没识别到
	
	uint8_t	  blank_b;			//预留
	uint8_t	  auto_too_close;   //目标距离太近,视觉发1，否则发0
	
	
	/* 尾 */
	uint16_t  CRC16;       
	
}extVisionRecvData_t;



void DMA_config(uint8_t *rx_buf, uint16_t dma_buf_num);
void UART_DMA(void);

#endif 

