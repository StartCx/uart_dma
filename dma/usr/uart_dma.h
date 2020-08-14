#ifndef __UART_DMA_H
#define	__UART_DMA_H

#include "uart_dma.h"
#include "usart.h"
#include <stdio.h>


typedef __packed struct
{
	/* ͷ */
	uint8_t   SOF;			//֡ͷ��ʼλ,�ݶ�0xA5
	uint8_t   CmdID;		//ָ��
	uint8_t   CRC8;			//֡ͷCRCУ��,��֤���͵�ָ������ȷ��
	
	/* ���� */
	float     pitch_angle;
	float     yaw_angle;
	float     distance;			//����
	uint8_t   centre_lock;		//�Ƿ���׼�����м�  0û��  1��׼����
	uint8_t	  identify_target;	//��Ұ���Ƿ���Ŀ��/�Ƿ�ʶ����Ŀ��   0��  1��	
	uint8_t   identify_buff;	//���ʱ�Ƿ�ʶ����Ŀ�꣬1�ǣ�2ʶ���л���װ�ף�0ûʶ��
	
	uint8_t	  blank_b;			//Ԥ��
	uint8_t	  auto_too_close;   //Ŀ�����̫��,�Ӿ���1������0
	
	
	/* β */
	uint16_t  CRC16;       
	
}extVisionRecvData_t;



void DMA_config(uint8_t *rx_buf, uint16_t dma_buf_num);
void UART_DMA(void);

#endif 

