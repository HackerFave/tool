/******************************************************************************
 Copyright(C),CEC Huada Electronic Design Co.,Ltd.
 File name: 		proto_i2c.c
 Author:			zhengwd 
 Version:			V1.0	
 Date:			2021-04-26	
 Description:	        
 History:		

******************************************************************************/


/***************************************************************************
* Include Header Files
***************************************************************************/
//#include "port_stm32l433_i2c.h"
#include "proto_i2c.h"
#include "error.h"
#include "log.h"


/**************************************************************************
* Variable Declaration
***************************************************************************/
uint8_t   g_bI2cHedPfsmi=0;	 //������֡�ܽ��յ����Э��֡����ָ��: PFSMI����ֵ
uint16_t   g_sI2cHedPfsm=0;	 //������֡�ܽ��յ����Э��֡����PFSM = ��PFSMI*16����PFSMIΪ0��ΪͨѶ��֧����ʽ����
uint16_t   g_sI2cHedPfss=0;	 //�ӻ���֡�ܽ��յ����Э��֡����PFSS = ��PFSSI*16����PFSSIΪ0��ΪͨѶ��֧����ʽ����
uint8_t   g_bI2cHedOpenSeMode = HED20_I2C_OPEN_SE_RESET_REQ;               //������SE ʱ�Ĳ���ģʽ
uint8_t   g_bI2cHedRstSeMode = HED20_I2C_RESET_SE_RESET_REQ;                  //��λSE ʱ�Ĳ���ģʽ

static peripheral_bus_driver g_proto_i2c = {
    PERIPHERAL_I2C,
   {NULL},
    proto_i2c_init,
    proto_i2c_deinit,
    proto_i2c_open,
    proto_i2c_close,
    proto_i2c_transceive,
    proto_i2c_reset,
    proto_i2c_control,
    proto_i2c_delay,
    NULL
};


PERIPHERAL_BUS_DRIVER_REGISTER(PERIPHERAL_I2C, g_proto_i2c);

/*************************************************
  Function:	  proto_i2c_crc16
  Description:  ����ָ������ָ�����ݵ�CRC���
  Input:	
            CRCType��CRC��������
            Length��Ҫ��������ݳ���
            Data��Ҫ��������ݵ���ʼ��ַ
  Return:	crc������	
  Others:		
*************************************************/
uint16_t proto_i2c_crc16(uint32_t CRCType,uint32_t Length ,uint8_t *Data)
{
	uint8_t chBlock = 0;
	uint16_t wCrc = 0;

	wCrc = (CRCType == CRC_A) ? 0x6363 : 0xFFFF;	// CRC_A : ITU-V.41 , CRC_B : ISO 3309

	do
	{
		chBlock = *Data++;
		chBlock = (chBlock^(uint8_t)(wCrc & 0x00FF));
		chBlock = (chBlock^(chBlock<<4));
		wCrc= (wCrc >> 8)^((uint16_t)chBlock << 8)^((uint16_t)chBlock<<3)^((uint16_t)chBlock>>4);
	} while (--Length);

	if (CRCType != CRC_A)
	{
		wCrc = ~wCrc; // ISO 3309
	}

	return wCrc;
}



/****************************************************************** 
Function:		proto_i2c_send_atr_rs_frame
Description:	ͨ��I2C �ӿڷ���R ��S ֡��ATR����֡
		1.����֡���ͣ���֯R/S ֡����
		2.ͨ��port���豸ע��ĺ����б�ָ�룬����port��i2c�ӿڵ�
			���ͺ���transmit������֡����
Input:	  periph   �豸���
          	  frame_type  ֡����
		  frame_type   ֡����ΪS  ֡ʱ����ʾ֡����Ĳ�����֡����ΪR ֡��ATR����֡ʱ������ֵΪ0
Output: 	  no	
Return:       ��������״̬��
Others: 	   no 
******************************************************************/
se_error_t proto_i2c_send_atr_rs_frame(peripheral *periph, uint8_t frame_type, uint8_t  frame_param)
{
	se_error_t stErrCode = SE_SUCCESS;
	uint16_t sOff = 0;
	uint16_t sCrc = 0;
	uint8_t abFrameBuf[PROTO_I2C_RS_FRAME_SIZE] = {0};
	util_timer_t timer = {0};
	HAL_I2C_PERIPHERAL_STRUCT_POINTER pI2cPeriph = (HAL_I2C_PERIPHERAL_STRUCT_POINTER)periph;	

	if(pI2cPeriph == NULL)
	{
		return SE_ERR_HANDLE_INVALID;
	}

	pI2cPeriph->delay(PROTO_I2C_RECEIVE_TO_SEND_BGT);

	//1.׼��R, S ֡��ATR����֡��ʼ������
	abFrameBuf[PROTO_I2C_PIB_OFFSET] = frame_type|(frame_param&0x0F);
	abFrameBuf[PROTO_I2C_LEN_OFFSET] = 0x00;
	abFrameBuf[PROTO_I2C_LEN_OFFSET+1] = 0x00;
	sOff = sOff + PROTO_I2C_PIB_LEN_SIZE;

	//2.׼��R, S ֡��ATR����֡��ֹ������
	sCrc = proto_i2c_crc16(CRC_B,sOff, abFrameBuf); 	//4 
	abFrameBuf[sOff] = sCrc&0xFF;
	abFrameBuf[sOff+1] = (sCrc>>8)&0xFF;
	sOff += PROTO_I2C_EDC_SIZE;

	//�������ȴ��ĳ�ʱʱ��
	timer.interval = PROTO_I2C_COMM_MUTEX_WAIT_TIME; 
	pI2cPeriph->timer_start(&timer);

	do
	{
		if(pI2cPeriph->timer_differ(&timer) != SE_SUCCESS)
		{
			stErrCode = SE_ERR_TIMEOUT; 
			LOGE("Failed:open periph mutex,  ErrCode-%08X.", stErrCode);
			break;
		}

		//3.��SE����R, S ֡��ATR����֡����
		//g_bI2cFrameType = frame_type;
		stErrCode = pI2cPeriph->transmit(pI2cPeriph, abFrameBuf, sOff);
		if(stErrCode == SE_ERR_BUSY)
		{
			continue;
		}

		else
		{
			break;
		}
	}while(1);

	return stErrCode;
}




/****************************************************************** 
Function:		proto_i2c_send_i_frame
Description:	ͨ��I2C �ӿڷ���I ������
		1.��˫�����ͷ�����PIB, LEN
		2.��˫�����β�����EDC, ��֯I ֡��ʽ��֡����
		3.ͨ��port���豸ע��ĺ����б�ָ�룬����port��i2c�ӿڵ�
			���ͺ���transmit������I ������
Input:	  periph �豸���
		  frame_type  ֡����
		  inbuf ����˫����е���ʼ��ַ
		  inbuf_len ����˫����е��������ݳ���
Output: 	  no	
Return: 	  ��������״̬��
Others: 	   no 
******************************************************************/
se_error_t proto_i2c_send_i_frame(peripheral *periph, uint8_t frame_type, uint8_t* inbuf, uint16_t inbuf_len)
{
	se_error_t stErrCode = SE_SUCCESS;	
	uint16_t sOff = 0;
	uint16_t sCrc = 0;
	uint8_t abFrameBuf[PROTO_I2C_PIB_LEN_SIZE] = {0};
	double_queue queue_in = (double_queue)inbuf;
	HAL_I2C_PERIPHERAL_STRUCT_POINTER pI2cPeriph = (HAL_I2C_PERIPHERAL_STRUCT_POINTER)periph;	
	uint8_t* p_input = NULL;
	util_timer_t timer = {0};

	if(periph == NULL)
	{
		return SE_ERR_HANDLE_INVALID;
	}

	if((inbuf == NULL) ||(inbuf_len == 0U))
	{
		return SE_ERR_PARAM_INVALID;
	}

	pI2cPeriph->delay(PROTO_I2C_RECEIVE_TO_SEND_BGT);

	//1.׼����Ϣ֡��ʼ�����ݣ�����˫��������PIB, LENH,LENL
	abFrameBuf[PROTO_I2C_PIB_OFFSET] = frame_type;
	abFrameBuf[PROTO_I2C_LEN_OFFSET]=(inbuf_len>>8)&0xFF;
	abFrameBuf[PROTO_I2C_LEN_OFFSET+1]=inbuf_len&0xFF;
	sOff = sOff + PROTO_I2C_PIB_LEN_SIZE;

	util_queue_front_push(abFrameBuf, sOff, queue_in);

	//2.׼����Ϣ֡��ֹ�����ݣ����������ݵ�CRCֵ������˫�������� CRC 
	p_input = &queue_in->q_buf[queue_in->front_node];
	sCrc = proto_i2c_crc16(CRC_B, util_queue_size(queue_in), p_input);	//4 //���� CRC
	sOff = util_queue_size(queue_in);	
	util_queue_rear_push((uint8_t *)&sCrc, PROTO_I2C_EDC_SIZE, queue_in);
	sOff = sOff + PROTO_I2C_EDC_SIZE;

	//�������ȴ��ĳ�ʱʱ��
	timer.interval = PROTO_I2C_COMM_MUTEX_WAIT_TIME; 
	pI2cPeriph->timer_start(&timer);
	
	do
	{
		if(pI2cPeriph->timer_differ(&timer) != SE_SUCCESS)
		{
			stErrCode = SE_ERR_TIMEOUT; 
			LOGE("Failed:open periph mutex,  ErrCode-%08X.", stErrCode);
			break;
		}

		//3.��SE������Ϣ֡����
		//g_bI2cFrameType = frame_type;
		stErrCode = pI2cPeriph->transmit(pI2cPeriph, p_input, sOff);
		if(stErrCode == SE_ERR_BUSY)
		{
			continue;
		}

		else
		{
			break;
		}
	}while(1);

	return stErrCode;

}



/****************************************************************** 
Function:		proto_i2c_receive_frame
Description:	ͨ��I2C �ӿڽ���֡����
		        1.ͨ��port���豸ע��ĺ����б�ָ�룬����port��i2c�ӿڵ�
			���պ���receive������֡����
Input:	      periph �豸���
                      headbuf ���տ����ݵ�֡ͷ�洢��ַ
Output: 	  rbuf ���տ����ݵĴ洢��ַ
              rlen ����Ϣ�ĳ���
Return: 	  ��������״̬��
Others: 	   no 
******************************************************************/
se_error_t proto_i2c_receive_frame(peripheral *periph, uint8_t *headbuf, uint8_t *rbuf, uint16_t *rlen)
{
	se_error_t stErrCode = SE_SUCCESS;
	HAL_I2C_PERIPHERAL_STRUCT_POINTER pI2cPeriph = (HAL_I2C_PERIPHERAL_STRUCT_POINTER)periph;	
	util_timer_t timer = {0};
	uint8_t bRetryNum=0;
	uint8_t bReceiveHeadFlag = 0;
	uint16_t sRspLen = 3; 
	uint16_t sCrc;

	if(pI2cPeriph == NULL)
	{
		return SE_ERR_HANDLE_INVALID;
	}

	if((headbuf == NULL)||(rbuf == NULL)||(rlen == NULL))
	{
		return SE_ERR_PARAM_INVALID;
	}

	//1. ���ý�������ʱ֡�ȴ��ĳ�ʱʱ��
	timer.interval = PROTO_I2C_RECEVIE_FRAME_WAIT_TIME; 
	pI2cPeriph->timer_start(&timer);

	do
	{
		//2. ��SE ������Ӧ���ݵ� PIB , LEN
		do
		{
			pI2cPeriph->delay(PROTO_I2C_RECEVIE_POLL_TIME);   //delay poll time
			
			stErrCode = pI2cPeriph->receive(pI2cPeriph, rbuf, (uint32_t *)&sRspLen);	
			if(stErrCode == SE_SUCCESS)
			{
				break;
			}

			if(pI2cPeriph->timer_differ(&timer) != SE_SUCCESS)
			{
				LOGE("Failed:receive frame overtime,  ErrCode-%08X.", SE_ERR_TIMEOUT);
				return SE_ERR_TIMEOUT;	
			}

		}while(1);

		//3. ���������ݲ�����ȷ��( PIB)
		if((rbuf[PROTO_I2C_PIB_OFFSET] != PROTO_I2C_I_FRAME_NO_LINK)&&
		    (rbuf[PROTO_I2C_PIB_OFFSET] != PROTO_I2C_S_FRAME_WTX)&&
		    (rbuf[PROTO_I2C_PIB_OFFSET] != PROTO_I2C_R_FRAME_ACK)&&
		    (rbuf[PROTO_I2C_PIB_OFFSET] != PROTO_I2C_R_FRAME_NAK)&&
		    (rbuf[PROTO_I2C_PIB_OFFSET] != PROTO_I2C_I_FRAME_LINK)) //PIB����
		{
			stErrCode = SE_ERR_DATA;
			break;
		}

		//4. ���������ݳ�����ȷ�ԣ�����ȷ�����¶�ȡ��3+ LEN +2���ֽڳ��ȵ�����
		sRspLen = (*(rbuf+PROTO_I2C_LEN_OFFSET) <<8) + *(rbuf+PROTO_I2C_LEN_OFFSET+1) +PROTO_I2C_PIB_LEN_EDC_SIZE; 
		if(sRspLen >  PROTO_I2C_FRAME_MAX_SIZE)
		{
			stErrCode = SE_ERR_LEN;	
			break;
		}

		if(bReceiveHeadFlag ==0)
		{
			bReceiveHeadFlag = 1;
			continue;
		}

		//5. �������CRC �Ƿ���ȷ�������󣬼����������ݣ���3�δ���󣬷���CRC����
		sCrc = proto_i2c_crc16(CRC_B, sRspLen-PROTO_I2C_EDC_SIZE, rbuf); 	//4 
		if((rbuf[sRspLen-PROTO_I2C_EDC_SIZE]!=(sCrc&0xff))||
		    (rbuf[sRspLen-PROTO_I2C_EDC_SIZE+1]!=sCrc>>8))
		{
			if(bRetryNum>=PROTO_I2C_CRC_ERROR_RETRY_MAX_NUM)
			{
				stErrCode = SE_ERR_LRC_CRC;
				break;
			}
			bRetryNum++;

			//stErrCode = proto_i2c_send_atr_rs_frame(periph, PROTO_I2C_R_FRAME_NAK, NULL);
			//if(stErrCode != SE_SUCCESS)
			//{
			//	break;
			//}
			bReceiveHeadFlag =0;
			sRspLen = 3;
			continue;
		} 

		//6.��ΪWTX����֡�����¼�ʱ���ظ�WTX��Ӧ���ݣ���������������
		if((rbuf[PROTO_I2C_PIB_OFFSET]&0xC0) == PROTO_I2C_S_FRAME_WTX)
		{
			//stErrCode = proto_i2c_send_atr_rs_frame(periph, PROTO_I2C_S_FRAME_WTX, NULL);
			//if(stErrCode != SE_SUCCESS)
			//{
			//	break;
			//}
	
			bReceiveHeadFlag =0;
			sRspLen = 3;
			
			timer.interval = PROTO_I2C_RECEVIE_FRAME_WAIT_TIME; 
			pI2cPeriph->timer_start(&timer);
			continue;
		}

		//7. �����Ϣ������
		*rlen = sRspLen;
		memcpy(headbuf, rbuf, PROTO_I2C_FRONT_FRAME_SIZE);

		break;

	}while(1);

	return stErrCode;
}


/*********************************************************************** 
Function:       proto_i2c_reset_request 
Description:   ����I2C ͨ��Э��֮��λ֡������Ӧ�Ĳ���
                     1. ����proto_i2c_send_atr_rs_frame�������������ΪPROTO_I2C_S_FRAME_RESET����SE����Reset ����֡��
                     2. �������ճ�ʱ�ļ�ʱ��������ʱ���ڣ�������֡����Ӧ������ʱ
                     3. ѭ�����Խ���Reset ����֡����Ӧ����
                     4. �����յ���Ӧ������proto_i2c_crc16 ����У�����ݣ��ɹ�������PFSM������
Input:        periph �豸��� 
Return:      ��������״̬��
Others:      no 
************************************************************************/
se_error_t proto_i2c_reset_request(peripheral *periph) 
{
	se_error_t stErrCode = SE_SUCCESS;
	util_timer_t timer = {0};
	uint8_t bNakRetryNum=0;
	uint8_t bNakRevNum=0;
	uint8_t bPfssi = 0;
	uint8_t bReceiveHeadFlag = 0;
	uint16_t sRspLen = 3; 
	uint16_t sCrc;
	uint8_t bResetRsp[PROTO_I2C_RESET_RSP_SIZE] = {0};
	HAL_I2C_PERIPHERAL_STRUCT_POINTER pI2cPeriph = (HAL_I2C_PERIPHERAL_STRUCT_POINTER)periph;


	if(periph == NULL)
	{
		return SE_ERR_HANDLE_INVALID;
	}

i2c_reset_request:
	
	//1. ��SE ����reset ����
	stErrCode = proto_i2c_send_atr_rs_frame(periph, PROTO_I2C_S_FRAME_RESET, g_bI2cHedPfsmi);
	if(stErrCode != SE_SUCCESS)
	{
		return stErrCode;
	}


	//���ý�������ʱ֡�ȴ��ĳ�ʱʱ��
	timer.interval = PROTO_I2C_RECEVIE_FRAME_WAIT_TIME; 
	pI2cPeriph->timer_start(&timer);

	do
	{
		//2. ��SE ������Ӧ���ݵ� PIB , LEN
		do
		{
			pI2cPeriph->delay(PROTO_I2C_RECEVIE_POLL_TIME);   //delay poll time
			
			stErrCode = pI2cPeriph->receive(pI2cPeriph, bResetRsp, (uint32_t *)&sRspLen);	
			if(stErrCode == SE_SUCCESS)
			{
				break;
			}

			if(pI2cPeriph->timer_differ(&timer) != SE_SUCCESS)
			{
				LOGE("Failed:receive frame overtime,  ErrCode-%08X.", SE_ERR_TIMEOUT);
				return SE_ERR_TIMEOUT;	
			}

		}while(1);

		//3. ���������ݲ�����ȷ��(reset֡����,  PIB , LEN)
		if((bResetRsp[PROTO_I2C_PIB_OFFSET] != PROTO_I2C_I_FRAME_NO_LINK)&&
		    (bResetRsp[PROTO_I2C_PIB_OFFSET] != PROTO_I2C_S_FRAME_RESET)&&
		    (bResetRsp[PROTO_I2C_PIB_OFFSET] != PROTO_I2C_R_FRAME_NAK))
		{
			stErrCode = SE_ERR_DATA;
			break;
		}

		//4. ���¶�ȡ��3+ LEN +2���ֽڳ��ȵ�����
		sRspLen = PROTO_I2C_RESET_RSP_SIZE; 
		if(bReceiveHeadFlag ==0)
		{
			bReceiveHeadFlag = 1;
			continue;
		}		

		//5. �������CRC �Ƿ���ȷ
		sCrc = proto_i2c_crc16(CRC_B, sRspLen-PROTO_I2C_EDC_SIZE, bResetRsp); 	//4 
		if((bResetRsp[sRspLen-PROTO_I2C_EDC_SIZE]!=(sCrc&0xff))||
		    (bResetRsp[sRspLen-PROTO_I2C_EDC_SIZE+1]!=sCrc>>8))
		{
			if(bNakRetryNum>=PROTO_I2C_CRC_ERROR_RETRY_MAX_NUM)
			{
				stErrCode = SE_ERR_LRC_CRC;
				break;
			}
			bNakRetryNum++;
			bReceiveHeadFlag =0;
			bNakRevNum = 0;
			sRspLen = 3;
			continue;
		} 

		//6.��ΪNAK ����֡���ط���ǰ֡����3�δ���󣬷���NAK����
		if(bResetRsp[PROTO_I2C_PIB_OFFSET] == PROTO_I2C_R_FRAME_NAK)
		{
			if(bNakRevNum>=PROTO_I2C_CRC_ERROR_RETRY_MAX_NUM)
			{
				stErrCode = SE_ERR_LRC_CRC;
				break;
			}
			bNakRevNum++;
			goto i2c_reset_request;
		}

		
		//7. ������ݲ����Ƿ���ȷ
		bPfssi = bResetRsp[PROTO_I2C_PIB_OFFSET]&0x0F;
		if((bPfssi == 0x00)||(g_bI2cHedPfsmi == 0x00))
		{
			//��PFSMI/PFSSI����һ��Ϊʱ��������������Ϊ0
			bPfssi = 0x00;
			g_bI2cHedPfsmi = 0x00;
		}
		else
		{
			if(g_bI2cHedPfsmi>=bPfssi)
			{
				g_bI2cHedPfsmi = bPfssi;
			}
		}
	
		g_sI2cHedPfsm =(uint16_t) g_bI2cHedPfsmi<<4;
		g_sI2cHedPfss = g_sI2cHedPfsm;
		break;
	}while(1);

	return stErrCode;
}




/*********************************************************************** 
Function:       proto_i2c_atr_request 
Description:   ����I2C ͨ��Э��֮ATR ֡������Ӧ�Ĳ���
                     1. ����proto_i2c_send_atr_rs_frame�������������ΪPROTO_I2C_I_FRAME_ATR����SE����ATR ����֡��
                     2. �������ճ�ʱ�ļ�ʱ��������ʱ���ڣ�������֡����Ӧ������ʱ
                     3. ѭ�����Խ���ATR ����֡����Ӧ����
                     4. �����յ���Ӧ������proto_i2c_crc16 ����У�����ݣ��ɹ��󷵻�ATR ����
Input:        periph �豸��� 
Output:      rbuf  ������ATR����ʼ��ַ
		  rlen  ������ATR�ĳ���
Return:      ��������״̬��
Others:      no 
************************************************************************/
se_error_t proto_i2c_atr_request(peripheral *periph, uint8_t *rbuf, uint32_t *rlen) 
{
	se_error_t stErrCode = SE_SUCCESS;
	util_timer_t timer = {0};
	uint8_t bNakRetryNum=0;
	uint8_t bNakRevNum=0;
	uint8_t bReceiveHeadFlag = 0;
	uint16_t sRspLen = 3; 
	uint16_t sCrc;
	uint8_t bAtrRsp[PROTO_I2C_ATR_RSP_SIZE] = {0};	
	HAL_I2C_PERIPHERAL_STRUCT_POINTER pI2cPeriph = (HAL_I2C_PERIPHERAL_STRUCT_POINTER)periph;


	if(periph == NULL)
	{
		return SE_ERR_HANDLE_INVALID;
	}

i2c_atr_request:


	//1. ��SE ����Atr ����֡
	stErrCode = proto_i2c_send_atr_rs_frame(periph, PROTO_I2C_I_FRAME_ATR, 0x00);
	if(stErrCode != SE_SUCCESS)
	{
		return stErrCode;
	}


	//���ý�������ʱ֡�ȴ��ĳ�ʱʱ��
	timer.interval = PROTO_I2C_RECEVIE_FRAME_WAIT_TIME; 
	pI2cPeriph->timer_start(&timer);

	do
	{
		//2. ��SE ������Ӧ���ݵ� PIB , LEN
		do
		{
			pI2cPeriph->delay(PROTO_I2C_RECEVIE_POLL_TIME);   //delay poll time
			
			stErrCode = pI2cPeriph->receive(pI2cPeriph, bAtrRsp, (uint32_t *)&sRspLen);	
			if(stErrCode == SE_SUCCESS)
			{
				break;
			}

			if(pI2cPeriph->timer_differ(&timer) != SE_SUCCESS)
			{
				LOGE("Failed:receive frame overtime,  ErrCode-%08X.", SE_ERR_TIMEOUT);
				return SE_ERR_TIMEOUT;	
			}

		}while(1);

		//3. ���������ݲ�����ȷ��(reset֡����,  PIB , LEN)
		if((bAtrRsp[PROTO_I2C_PIB_OFFSET] != PROTO_I2C_I_FRAME_NO_LINK)&&
		    (bAtrRsp[PROTO_I2C_PIB_OFFSET] != PROTO_I2C_R_FRAME_NAK))
		{
			stErrCode = SE_ERR_DATA;
			break;
		}

		//4. ���������ݳ�����ȷ�ԣ�����ȷ�����¶�ȡ��3+ LEN +2���ֽڳ��ȵ�����
		sRspLen = (*(bAtrRsp+PROTO_I2C_LEN_OFFSET) <<8) + *(bAtrRsp+PROTO_I2C_LEN_OFFSET+1) + PROTO_I2C_PIB_LEN_EDC_SIZE; 
		if(sRspLen >  PROTO_I2C_ATR_RSP_SIZE)  //LEN����
		{
			stErrCode = SE_ERR_LEN;
			break;
		}

		if(bReceiveHeadFlag ==0)
		{
			bReceiveHeadFlag = 1;
			continue;
		}		

		//5. �������CRC �Ƿ���ȷ
		sCrc = proto_i2c_crc16(CRC_B, sRspLen-PROTO_I2C_EDC_SIZE, bAtrRsp);	//4 
		if((bAtrRsp[sRspLen-PROTO_I2C_EDC_SIZE]!=(sCrc&0xff))||
			(bAtrRsp[sRspLen-PROTO_I2C_EDC_SIZE+1]!=sCrc>>8))
		{
			if(bNakRetryNum>=PROTO_I2C_CRC_ERROR_RETRY_MAX_NUM)
			{
				stErrCode = SE_ERR_LRC_CRC;
				break;
			}
			bNakRetryNum++;
			bReceiveHeadFlag =0;
			bNakRevNum = 0;
			sRspLen = 3;
			continue;
		} 

		//6.��ΪNAK ����֡���ط���ǰ֡����3�δ���󣬷���NAK����
		if(bAtrRsp[PROTO_I2C_PIB_OFFSET] == PROTO_I2C_R_FRAME_NAK)
		{
			if(bNakRevNum>=PROTO_I2C_CRC_ERROR_RETRY_MAX_NUM)
			{
				stErrCode = SE_ERR_LRC_CRC;
				break;
			}
			bNakRevNum++;
			goto i2c_atr_request;
		}
		
		//7. ������ݲ����Ƿ���ȷ
		if(bAtrRsp[PROTO_I2C_DATA_OFFSET] != 0x3B) 
		{
			stErrCode = SE_ERR_DATA;;
			break;
		}
		
		*rlen = sRspLen-PROTO_I2C_PIB_LEN_EDC_SIZE;
		memcpy(rbuf, bAtrRsp+PROTO_I2C_DATA_OFFSET, *rlen);
		
		break;

	}while(1);

	return stErrCode;
}


/********************************************************************************* 
Function:       proto_i2c_handle 
Description:    ����HED I2C ͨ��Э���֡��ʽ��SE�����շ����ݽ�����
		     1. ����proto_i2c_send_i_frame�������������ΪPROTO_I2C_I_FRAME_NO_LINK����SE ���ͷ���ʽ��Ϣ֡
                     2. ����proto_i2c_receive_frame����������SE ���ص���Ӧ����
                     3. �жϽ��յ���Ӧ�����Ƿ�NAK������NAK�����ط���ǰ֡���ҵ�3�δ����Master����Ҫ����RESET֡���Իָ���·
Input:          periph �豸��� 
		   input  ��������ָ��
		   input_len �������ݳ���
Output:       output������ݴ洢��ַ
		   outbuf_len������ݳ��ȵ�ַ
Return:         no
Others:         ʵ�ֵ�I2C ͨѶ��֧����ʽ����
*********************************************************************************/
se_error_t proto_i2c_handle(peripheral *periph, uint8_t* inbuf, uint32_t inbuf_len, uint8_t* outbuf, uint32_t* outbuf_len)
{	
	se_error_t stErrCode = SE_SUCCESS;
	util_timer_t timer = {0};
	uint8_t bWtxRetryNum=0;
	uint8_t bNakRetryNum=0;
	uint16_t sRecvInfLen=0;
	uint8_t abFrontBuf[PROTO_I2C_FRONT_FRAME_SIZE] = {0};
	HAL_I2C_PERIPHERAL_STRUCT_POINTER pI2cPeriph = (HAL_I2C_PERIPHERAL_STRUCT_POINTER)periph;	
	double_queue queue_out = (double_queue)outbuf;
	uint8_t* p_output = NULL;

	p_output = &queue_out->q_buf[queue_out->front_node];
	do
	{
		if((g_sI2cHedPfsm == 0) || (g_sI2cHedPfss == 0))	   //ֵΪ0ʱ��ͨѶ��֧����ʽ����
		{
			//======= ��֧����ʽͨ�Ÿ�ʽ===========

			
			//1. ��SE ���ͷ���ʽ��Ϣ֡
			stErrCode = proto_i2c_send_i_frame(periph, PROTO_I2C_I_FRAME_NO_LINK, inbuf, (uint32_t)inbuf_len);
			if(stErrCode != SE_SUCCESS)
			{
				return stErrCode;
			}

			//2. ��SE ����֡����
			stErrCode = proto_i2c_receive_frame(periph, abFrontBuf, p_output, &sRecvInfLen);
			if(stErrCode != SE_SUCCESS)
			{
				if(stErrCode == SE_ERR_TIMEOUT)
				{
					if(bWtxRetryNum>=PROTO_I2C_TIMEOUT_RETRY_MAX_NUM)
					{
						stErrCode = proto_i2c_reset_request(periph);
						if(stErrCode == SE_SUCCESS)
						{
							stErrCode = SE_ERR_TIMEOUT;
						}
						else
						{
							stErrCode = SE_ERR_COMM;
						}
						break;
					}
					bWtxRetryNum++;
					continue;
				}
				break;
			}


			//3.��ΪNAK ����֡���ط���ǰ֡����3�δ����
			//Master����Ҫ����RESET֡���Իָ���·��������
			//1��RESET֡����ͨѶ�쳣����NAK����
			if(abFrontBuf[PROTO_I2C_PIB_OFFSET] == PROTO_I2C_R_FRAME_NAK)
			{
				if(bNakRetryNum>=PROTO_I2C_CRC_ERROR_RETRY_MAX_NUM)
				{
					stErrCode = proto_i2c_reset_request(periph);
					if(stErrCode == SE_SUCCESS)
					{
						stErrCode = SE_ERR_LRC_CRC;
					}
					else
					{
						stErrCode = SE_ERR_COMM;
					}
					break;
				}
				bNakRetryNum++;
				continue;
			}

			//4. ���ͺͽ��վ���ȷ
			queue_out->q_buf_len = sRecvInfLen;
			queue_out->rear_node =  queue_out->front_node + sRecvInfLen;
			util_queue_front_pop(PROTO_I2C_PIB_LEN_SIZE, queue_out); //�Ƴ� PIB, LEN
			util_queue_rear_pop(PROTO_I2C_EDC_SIZE, queue_out);//�Ƴ�CRC 
			*outbuf_len = util_queue_size(queue_out);

			break;	
		}

		else
		{
			//======= ��Ϊ��ʽͨ�Ÿ�ʽ��ֱ�ӷ��ش���===========
			stErrCode = SE_ERR_COMM;
		}
	}while(1);

	return stErrCode;
}



/************************************************************************************
Function:       proto_i2c_init 
Description:    ����I2C ͨ��Э��ʵ�ֳ�ʼ��
		      1.ͨ��port���豸ע��ĺ����б�ָ�룬����port��i2c�ӿڵ�
		      ��ʼ������init
Input:         periph �豸��� 
Output:        no  
Return:        ��������״̬��
Others:        no 
**************************************************************************************/
se_error_t proto_i2c_init(peripheral *periph) 
{
	se_error_t stErrCode = SE_SUCCESS;
	HAL_I2C_PERIPHERAL_STRUCT_POINTER pI2cPeriph = (HAL_I2C_PERIPHERAL_STRUCT_POINTER)periph;

	if(pI2cPeriph == NULL)
	{
		return SE_ERR_HANDLE_INVALID;
	}

	stErrCode = pI2cPeriph->init(pI2cPeriph);	
	if(stErrCode != SE_SUCCESS)
	{
		LOGE("Failed:i2c protocol init,  ErrCode-%08X.", stErrCode);
	}
	else
	{
		LOGI("Success!");
	}

	return stErrCode;
}


/************************************************************************************
Function:       proto_i2c_deinit 
Description:    ����I2C ͨ��Э�������ֹ��
		      1.ͨ��port���豸ע��ĺ����б�ָ�룬����port��i2c�ӿڵ�
		      ��ֹ������deinit
Input:          periph �豸��� 
Output:        no  
Return:        ��������״̬��
Others:        no 
**************************************************************************************/
se_error_t proto_i2c_deinit(peripheral *periph) 
{
	se_error_t stErrCode = SE_SUCCESS;
	HAL_I2C_PERIPHERAL_STRUCT_POINTER pI2cPeriph = (HAL_I2C_PERIPHERAL_STRUCT_POINTER)periph;

	if(pI2cPeriph == NULL)
	{
		return SE_ERR_HANDLE_INVALID;
	}

	stErrCode = pI2cPeriph->deinit(pI2cPeriph);
	if(stErrCode != SE_SUCCESS)
	{
		LOGE("Failed:i2c protocol deinit,  ErrCode-%08X.", stErrCode);
	}
	else
	{
		LOGI("Success!");
	}

	return stErrCode;
}


/*********************************************************************** 
Function:       proto_i2c_open 
Description:   I2Cͨ��Э��֮���豸�Ĳ���
	             1. ͨ��port���豸ע��ĺ����б�ָ�룬����port���control����������SE ��RST���Ž��и�λ����
		     2. ����RESET ���󣬵���proto_i2c_reset_request������SE ����RESET����֡��������Ӧ
                     3. ����ATR ���󣬵���proto_i2c_atr_request ������SE ���� ATR ����֡������ATR ����
Input:        periph �豸��� 
Output:      rbuf  ������ATR����ʼ��ַ
		  rlen  ������ATR�ĳ��ȵ�ַ
Return:      ��������״̬��
Others:      no 
************************************************************************/
se_error_t proto_i2c_open(peripheral *periph, uint8_t *rbuf, uint32_t *rlen) 
{
	se_error_t stErrCode = SE_SUCCESS;
	HAL_I2C_PERIPHERAL_STRUCT_POINTER pI2cPeriph = (HAL_I2C_PERIPHERAL_STRUCT_POINTER)periph;

	if(pI2cPeriph == NULL)
	{
		return SE_ERR_HANDLE_INVALID;
	}

	if (((rbuf == NULL) && (rlen != NULL))||((rbuf != NULL) && (rlen == NULL)))
	{
		return  SE_ERR_PARAM_INVALID;
	}

	do
	{
		//����SE ��RST���Ž��и�λ����
		//stErrCode = pI2cPeriph->control(pI2cPeriph, (uint32_t)PROTO_I2C_CTRL_RST, NULL, (uint32_t)NULL);
		//if(stErrCode != SE_SUCCESS)
		//{
			//LOGE("Failed:protocol rst io control,  ErrCode-%08X.", stErrCode);
			//break;
		//}
        pI2cPeriph->delay(PROTO_I2C_SE_RST_DELAY);//�ӳ�10msȷ��SE����
		if(g_bI2cHedOpenSeMode == HED20_I2C_OPEN_SE_RESET_REQ)
		{
			stErrCode = proto_i2c_reset_request(periph);
			if(stErrCode != SE_SUCCESS)
			{
				LOGE("Failed:protocol reset request,	ErrCode-%08X.", stErrCode);
				break;
			}
		}

		if((rbuf != NULL)&&(rlen != NULL))
		{
			stErrCode = proto_i2c_atr_request(periph, rbuf, rlen);
			if(stErrCode != SE_SUCCESS)
			{
				LOGE("Failed:protocol atr request,	ErrCode-%08X.", stErrCode);
				break;
			}	
		}

		LOGI("Open Periph Success!");
		break;

	}while(0);

	return stErrCode;
}


/**
* @brief Ӧ�ùر��豸ʱ�����ô˺���
* @param [in] periph  �豸���
* @return ��������״̬��	
* @note �޴���ʵ��
*/
se_error_t proto_i2c_close(peripheral *periph) 
{
	se_error_t ret_code = SE_SUCCESS;

	return ret_code;
}


/****************************************************************** 
Function:		proto_i2c_transceive 
Description:	ͨ��I2C �ӿ�����豸������������������Ӧʱ�����ô˺���
			1������ HED_I2C ͨ��Э�飬����proto_i2c_handle ���������ͼ�����Э��֡���ݡ�
Input:	   periph �豸���
		   sbuf ����˫����е���ʼ��ַ
		   slen ����˫����е��������ݳ���
Output:       rbuf ���˫����е���ʼ��ַ
		   rlen  ���˫����е�������ݳ��ȵ���ʼ��ַ
Return:        ��������״̬��
Others:  no 
******************************************************************/
se_error_t proto_i2c_transceive(peripheral *periph, uint8_t *sbuf,  uint32_t  slen, uint8_t *rbuf, uint32_t *rlen)
{
	se_error_t stErrCode = SE_SUCCESS;
	if(periph == NULL)
	{
		return SE_ERR_HANDLE_INVALID;
	}

	if ((sbuf == NULL) || (rbuf == NULL) || (slen == 0U) || (rlen == NULL))
	{
		return  SE_ERR_PARAM_INVALID;
	}

	stErrCode = proto_i2c_handle(periph, sbuf, slen, rbuf, rlen);
	if(stErrCode != SE_SUCCESS)
	{
		LOGE("Failed:protocol communication,  ErrCode-%08X.", stErrCode);
	}
	else
	{
		LOGI("Communication Success!");
	}
	return stErrCode;	
}

/*********************************************************************** 
Function:       proto_i2c_reset 
Description:   I2C ͨ��Э��֮��λ�豸�Ĳ���
                    1. ͨ��port���豸ע��ĺ����б�ָ�룬����port���control����������SE ��RST���Ž��и�λ����
                    2. ����RESET ���󣬵���proto_i2c_reset_request������SE ����RESET����֡��������Ӧ
                    3. ����ATR ���󣬵���proto_i2c_atr_request ������SE ���� ATR ����֡������ATR ����
Input:        periph �豸��� 
Output:      rbuf  ������ATR����ʼ��ַ
		  rlen  ������ATR�ĳ���
Return:      ��������״̬��
Others:      no 
************************************************************************/
se_error_t proto_i2c_reset(peripheral *periph, uint8_t *rbuf, uint32_t *rlen) 
{
	se_error_t stErrCode = SE_SUCCESS;
	HAL_I2C_PERIPHERAL_STRUCT_POINTER pI2cPeriph = (HAL_I2C_PERIPHERAL_STRUCT_POINTER)periph;

	if(pI2cPeriph == NULL)
	{
		return SE_ERR_HANDLE_INVALID;
	}

	if (((rbuf == NULL) && (rlen != NULL))||((rbuf != NULL) && (rlen == NULL)))
	{
		return  SE_ERR_PARAM_INVALID;
	}

	do
	{
		//����SE ��RST���Ž��и�λ����
		stErrCode = pI2cPeriph->control(pI2cPeriph, (uint32_t)PROTO_I2C_CTRL_RST, NULL, (uint32_t)NULL);
		if(stErrCode != SE_SUCCESS)
		{
			LOGE("Failed:protocol rst io control,  ErrCode-%08X.", stErrCode);
			break;
		}

		if(g_bI2cHedOpenSeMode == HED20_I2C_RESET_SE_RESET_REQ)
		{
			stErrCode = proto_i2c_reset_request(periph);
			if(stErrCode != SE_SUCCESS)
			{
				LOGE("Failed:protocol reset request,	ErrCode-%08X.", stErrCode);
				break;
			}

		}

		if((rbuf != NULL)&&(rlen != NULL))
		{
			stErrCode = proto_i2c_atr_request(periph, rbuf, rlen);
			if(stErrCode != SE_SUCCESS)
			{
				LOGE("Failed:protocol atr request,	ErrCode-%08X.", stErrCode);
				break;
			}	
		}

		LOGI("Reset Periph Success!");
		break;

	}while(0);

	return stErrCode;
}



/****************************************************************** 
Function:		proto_i2c_control
Description:	ͨ��i2c�ӿ�����豸���Ϳ�������
Input:	   periph �豸���
		   ctrlcode ���������
		   sbuf �������ݵ���ʼ��ַ
		   slen �������ݵĳ���
Output:    rbuf ������ݵ���ʼ��ַ
		   rlen  ������ݳ��ȵ���ʼ��ַ
Return:    ��������״̬��
Others:    no 
******************************************************************/
se_error_t proto_i2c_control(peripheral *periph , uint32_t ctrlcode, uint8_t *sbuf, uint32_t slen, uint8_t  *rbuf, uint32_t *rlen)
{
	se_error_t stErrCode = SE_SUCCESS;
	HAL_I2C_PERIPHERAL_STRUCT_POINTER pI2cPeriph = (HAL_I2C_PERIPHERAL_STRUCT_POINTER)periph;

	if(pI2cPeriph == NULL)
	{
		return SE_ERR_HANDLE_INVALID;
	}

	if ( ctrlcode== 0U)
	{
		return  SE_ERR_PARAM_INVALID;
	}

	stErrCode = pI2cPeriph->control(pI2cPeriph, ctrlcode, sbuf, slen);
	if(stErrCode != SE_SUCCESS)
	{
		LOGE("Failed:i2c protocol control,  ErrCode-%08X.", stErrCode);
	}
	else
	{
		LOGI("Success!");
	}
	
	return stErrCode;
}



/*********************************************************************** 
Function:     proto_i2c_delay 
Description:  ʵ��΢�뼶��ʱ
Input:        periph �豸��� 
              us     ��ʱ΢����
Return:      ��������״̬��
Others:      no 
************************************************************************/
se_error_t proto_i2c_delay(peripheral *periph , uint32_t us) 
{
	se_error_t stErrCode = SE_SUCCESS;
	HAL_I2C_PERIPHERAL_STRUCT_POINTER pI2cPeriph = (HAL_I2C_PERIPHERAL_STRUCT_POINTER)periph;
	
	if(pI2cPeriph == NULL)
	{
		return SE_ERR_HANDLE_INVALID;
	}
	//��ʱָ��΢��ʱ��
	pI2cPeriph->delay(us);
	return stErrCode;
}




