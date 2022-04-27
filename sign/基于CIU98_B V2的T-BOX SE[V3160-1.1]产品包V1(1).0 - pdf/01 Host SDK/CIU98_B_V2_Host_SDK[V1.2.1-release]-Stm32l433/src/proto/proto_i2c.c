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
uint8_t   g_bI2cHedPfsmi=0;	 //主机单帧能接收的最大协议帧长度指数: PFSMI设置值
uint16_t   g_sI2cHedPfsm=0;	 //主机单帧能接收的最大协议帧长度PFSM = （PFSMI*16），PFSMI为0意为通讯不支持链式传输
uint16_t   g_sI2cHedPfss=0;	 //从机单帧能接收的最大协议帧长度PFSS = （PFSSI*16），PFSSI为0意为通讯不支持链式传输
uint8_t   g_bI2cHedOpenSeMode = HED20_I2C_OPEN_SE_RESET_REQ;               //打开连接SE 时的操作模式
uint8_t   g_bI2cHedRstSeMode = HED20_I2C_RESET_SE_RESET_REQ;                  //复位SE 时的操作模式

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
  Description:  计算指定长度指定数据的CRC结果
  Input:	
            CRCType：CRC计算类型
            Length：要计算的数据长度
            Data：要计算的数据的起始地址
  Return:	crc计算结果	
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
Description:	通过I2C 接口发送R 或S 帧或ATR请求帧
		1.根据帧类型，组织R/S 帧数据
		2.通过port层设备注册的函数列表指针，调用port层i2c接口的
			发送函数transmit，发送帧数据
Input:	  periph   设备句柄
          	  frame_type  帧类型
		  frame_type   帧类型为S  帧时，表示帧请求的参数，帧类型为R 帧或ATR请求帧时，参数值为0
Output: 	  no	
Return:       函数操作状态码
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

	//1.准备R, S 帧或ATR请求帧起始域数据
	abFrameBuf[PROTO_I2C_PIB_OFFSET] = frame_type|(frame_param&0x0F);
	abFrameBuf[PROTO_I2C_LEN_OFFSET] = 0x00;
	abFrameBuf[PROTO_I2C_LEN_OFFSET+1] = 0x00;
	sOff = sOff + PROTO_I2C_PIB_LEN_SIZE;

	//2.准备R, S 帧或ATR请求帧终止域数据
	sCrc = proto_i2c_crc16(CRC_B,sOff, abFrameBuf); 	//4 
	abFrameBuf[sOff] = sCrc&0xFF;
	abFrameBuf[sOff+1] = (sCrc>>8)&0xFF;
	sOff += PROTO_I2C_EDC_SIZE;

	//设置锁等待的超时时间
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

		//3.向SE发送R, S 帧或ATR请求帧数据
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
Description:	通过I2C 接口发送I 块数据
		1.向双向队列头部添加PIB, LEN
		2.向双向队列尾部添加EDC, 组织I 帧格式的帧数据
		3.通过port层设备注册的函数列表指针，调用port层i2c接口的
			发送函数transmit，发送I 块数据
Input:	  periph 设备句柄
		  frame_type  帧类型
		  inbuf 输入双向队列的起始地址
		  inbuf_len 输入双向队列的输入数据长度
Output: 	  no	
Return: 	  函数操作状态码
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

	//1.准备信息帧起始域数据，即向双向队列添加PIB, LENH,LENL
	abFrameBuf[PROTO_I2C_PIB_OFFSET] = frame_type;
	abFrameBuf[PROTO_I2C_LEN_OFFSET]=(inbuf_len>>8)&0xFF;
	abFrameBuf[PROTO_I2C_LEN_OFFSET+1]=inbuf_len&0xFF;
	sOff = sOff + PROTO_I2C_PIB_LEN_SIZE;

	util_queue_front_push(abFrameBuf, sOff, queue_in);

	//2.准备信息帧终止域数据，即计算数据的CRC值，并向双向队列添加 CRC 
	p_input = &queue_in->q_buf[queue_in->front_node];
	sCrc = proto_i2c_crc16(CRC_B, util_queue_size(queue_in), p_input);	//4 //计算 CRC
	sOff = util_queue_size(queue_in);	
	util_queue_rear_push((uint8_t *)&sCrc, PROTO_I2C_EDC_SIZE, queue_in);
	sOff = sOff + PROTO_I2C_EDC_SIZE;

	//设置锁等待的超时时间
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

		//3.向SE发送信息帧数据
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
Description:	通过I2C 接口接收帧数据
		        1.通过port层设备注册的函数列表指针，调用port层i2c接口的
			接收函数receive，接收帧数据
Input:	      periph 设备句柄
                      headbuf 接收块数据的帧头存储地址
Output: 	  rbuf 接收块数据的存储地址
              rlen 块信息的长度
Return: 	  函数操作状态码
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

	//1. 设置接收数据时帧等待的超时时间
	timer.interval = PROTO_I2C_RECEVIE_FRAME_WAIT_TIME; 
	pI2cPeriph->timer_start(&timer);

	do
	{
		//2. 从SE 接收响应数据的 PIB , LEN
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

		//3. 检查接收数据参数正确性( PIB)
		if((rbuf[PROTO_I2C_PIB_OFFSET] != PROTO_I2C_I_FRAME_NO_LINK)&&
		    (rbuf[PROTO_I2C_PIB_OFFSET] != PROTO_I2C_S_FRAME_WTX)&&
		    (rbuf[PROTO_I2C_PIB_OFFSET] != PROTO_I2C_R_FRAME_ACK)&&
		    (rbuf[PROTO_I2C_PIB_OFFSET] != PROTO_I2C_R_FRAME_NAK)&&
		    (rbuf[PROTO_I2C_PIB_OFFSET] != PROTO_I2C_I_FRAME_LINK)) //PIB错误
		{
			stErrCode = SE_ERR_DATA;
			break;
		}

		//4. 检查接收数据长度正确性，若正确，重新读取（3+ LEN +2）字节长度的数据
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

		//5. 检查数据CRC 是否正确，若错误，继续接收数据，当3次错误后，返回CRC错误
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

		//6.若为WTX请求帧，重新计时，回复WTX响应数据，并继续接收数据
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

		//7. 输出信息域数据
		*rlen = sRspLen;
		memcpy(headbuf, rbuf, PROTO_I2C_FRONT_FRAME_SIZE);

		break;

	}while(1);

	return stErrCode;
}


/*********************************************************************** 
Function:       proto_i2c_reset_request 
Description:   华大I2C 通信协议之复位帧请求及相应的操作
                     1. 调用proto_i2c_send_atr_rs_frame函数，输入参数为PROTO_I2C_S_FRAME_RESET，向SE发送Reset 请求帧。
                     2. 启动接收超时的计时，在设置时间内，无请求帧的响应，将超时
                     3. 循环尝试接收Reset 请求帧的响应数据
                     4. 若接收到响应，调用proto_i2c_crc16 函数校验数据，成功后设置PFSM参数。
Input:        periph 设备句柄 
Return:      函数操作状态码
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
	
	//1. 向SE 发送reset 请求
	stErrCode = proto_i2c_send_atr_rs_frame(periph, PROTO_I2C_S_FRAME_RESET, g_bI2cHedPfsmi);
	if(stErrCode != SE_SUCCESS)
	{
		return stErrCode;
	}


	//设置接收数据时帧等待的超时时间
	timer.interval = PROTO_I2C_RECEVIE_FRAME_WAIT_TIME; 
	pI2cPeriph->timer_start(&timer);

	do
	{
		//2. 从SE 接收响应数据的 PIB , LEN
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

		//3. 检查接收数据参数正确性(reset帧长度,  PIB , LEN)
		if((bResetRsp[PROTO_I2C_PIB_OFFSET] != PROTO_I2C_I_FRAME_NO_LINK)&&
		    (bResetRsp[PROTO_I2C_PIB_OFFSET] != PROTO_I2C_S_FRAME_RESET)&&
		    (bResetRsp[PROTO_I2C_PIB_OFFSET] != PROTO_I2C_R_FRAME_NAK))
		{
			stErrCode = SE_ERR_DATA;
			break;
		}

		//4. 重新读取（3+ LEN +2）字节长度的数据
		sRspLen = PROTO_I2C_RESET_RSP_SIZE; 
		if(bReceiveHeadFlag ==0)
		{
			bReceiveHeadFlag = 1;
			continue;
		}		

		//5. 检查数据CRC 是否正确
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

		//6.若为NAK 错误帧，重发当前帧，当3次错误后，返回NAK错误
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

		
		//7. 检查数据参数是否正确
		bPfssi = bResetRsp[PROTO_I2C_PIB_OFFSET]&0x0F;
		if((bPfssi == 0x00)||(g_bI2cHedPfsmi == 0x00))
		{
			//当PFSMI/PFSSI任意一个为时，两参数均设置为0
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
Description:   华大I2C 通信协议之ATR 帧请求及相应的操作
                     1. 调用proto_i2c_send_atr_rs_frame函数，输入参数为PROTO_I2C_I_FRAME_ATR，向SE发送ATR 请求帧。
                     2. 启动接收超时的计时，在设置时间内，无请求帧的响应，将超时
                     3. 循环尝试接收ATR 请求帧的响应数据
                     4. 若接收到响应，调用proto_i2c_crc16 函数校验数据，成功后返回ATR 数据
Input:        periph 设备句柄 
Output:      rbuf  待接收ATR的起始地址
		  rlen  待接收ATR的长度
Return:      函数操作状态码
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


	//1. 向SE 发送Atr 请求帧
	stErrCode = proto_i2c_send_atr_rs_frame(periph, PROTO_I2C_I_FRAME_ATR, 0x00);
	if(stErrCode != SE_SUCCESS)
	{
		return stErrCode;
	}


	//设置接收数据时帧等待的超时时间
	timer.interval = PROTO_I2C_RECEVIE_FRAME_WAIT_TIME; 
	pI2cPeriph->timer_start(&timer);

	do
	{
		//2. 从SE 接收响应数据的 PIB , LEN
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

		//3. 检查接收数据参数正确性(reset帧长度,  PIB , LEN)
		if((bAtrRsp[PROTO_I2C_PIB_OFFSET] != PROTO_I2C_I_FRAME_NO_LINK)&&
		    (bAtrRsp[PROTO_I2C_PIB_OFFSET] != PROTO_I2C_R_FRAME_NAK))
		{
			stErrCode = SE_ERR_DATA;
			break;
		}

		//4. 检查接收数据长度正确性，若正确，重新读取（3+ LEN +2）字节长度的数据
		sRspLen = (*(bAtrRsp+PROTO_I2C_LEN_OFFSET) <<8) + *(bAtrRsp+PROTO_I2C_LEN_OFFSET+1) + PROTO_I2C_PIB_LEN_EDC_SIZE; 
		if(sRspLen >  PROTO_I2C_ATR_RSP_SIZE)  //LEN错误
		{
			stErrCode = SE_ERR_LEN;
			break;
		}

		if(bReceiveHeadFlag ==0)
		{
			bReceiveHeadFlag = 1;
			continue;
		}		

		//5. 检查数据CRC 是否正确
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

		//6.若为NAK 错误帧，重发当前帧，当3次错误后，返回NAK错误
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
		
		//7. 检查数据参数是否正确
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
Description:    按照HED I2C 通信协议的帧格式与SE进行收发数据交互。
		     1. 调用proto_i2c_send_i_frame函数，输入参数为PROTO_I2C_I_FRAME_NO_LINK，向SE 发送非链式信息帧
                     2. 调用proto_i2c_receive_frame函数，接收SE 返回的响应数据
                     3. 判断接收的响应数据是否NAK错误，若NAK错误重发当前帧，且当3次错误后，Master端需要发送RESET帧尝试恢复链路
Input:          periph 设备句柄 
		   input  输入数据指针
		   input_len 输入数据长度
Output:       output输出数据存储地址
		   outbuf_len输出数据长度地址
Return:         no
Others:         实现的I2C 通讯不支持链式传输
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
		if((g_sI2cHedPfsm == 0) || (g_sI2cHedPfss == 0))	   //值为0时，通讯不支持链式传输
		{
			//======= 不支持链式通信格式===========

			
			//1. 向SE 发送非链式信息帧
			stErrCode = proto_i2c_send_i_frame(periph, PROTO_I2C_I_FRAME_NO_LINK, inbuf, (uint32_t)inbuf_len);
			if(stErrCode != SE_SUCCESS)
			{
				return stErrCode;
			}

			//2. 从SE 接收帧数据
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


			//3.若为NAK 错误帧，重发当前帧，当3次错误后，
			//Master端需要发送RESET帧尝试恢复链路，若尝试
			//1次RESET帧后，仍通讯异常返回NAK错误
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

			//4. 发送和接收均正确
			queue_out->q_buf_len = sRecvInfLen;
			queue_out->rear_node =  queue_out->front_node + sRecvInfLen;
			util_queue_front_pop(PROTO_I2C_PIB_LEN_SIZE, queue_out); //移除 PIB, LEN
			util_queue_rear_pop(PROTO_I2C_EDC_SIZE, queue_out);//移除CRC 
			*outbuf_len = util_queue_size(queue_out);

			break;	
		}

		else
		{
			//======= 若为链式通信格式，直接返回错误===========
			stErrCode = SE_ERR_COMM;
		}
	}while(1);

	return stErrCode;
}



/************************************************************************************
Function:       proto_i2c_init 
Description:    华大I2C 通信协议实现初始化
		      1.通过port层设备注册的函数列表指针，调用port层i2c接口的
		      初始化函数init
Input:         periph 设备句柄 
Output:        no  
Return:        函数操作状态码
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
Description:    华大I2C 通信协议参数终止化
		      1.通过port层设备注册的函数列表指针，调用port层i2c接口的
		      终止化函数deinit
Input:          periph 设备句柄 
Output:        no  
Return:        函数操作状态码
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
Description:   I2C通信协议之打开设备的操作
	             1. 通过port层设备注册的函数列表指针，调用port层的control函数，控制SE 的RST引脚进行复位操作
		     2. 若需RESET 请求，调用proto_i2c_reset_request函数向SE 发送RESET请求帧及接收响应
                     3. 若需ATR 请求，调用proto_i2c_atr_request 函数向SE 发送 ATR 请求帧及接收ATR 数据
Input:        periph 设备句柄 
Output:      rbuf  待接收ATR的起始地址
		  rlen  待接收ATR的长度地址
Return:      函数操作状态码
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
		//控制SE 的RST引脚进行复位操作
		//stErrCode = pI2cPeriph->control(pI2cPeriph, (uint32_t)PROTO_I2C_CTRL_RST, NULL, (uint32_t)NULL);
		//if(stErrCode != SE_SUCCESS)
		//{
			//LOGE("Failed:protocol rst io control,  ErrCode-%08X.", stErrCode);
			//break;
		//}
        pI2cPeriph->delay(PROTO_I2C_SE_RST_DELAY);//延迟10ms确定SE唤醒
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
* @brief 应用关闭设备时，调用此函数
* @param [in] periph  设备句柄
* @return 函数操作状态码	
* @note 无代码实现
*/
se_error_t proto_i2c_close(peripheral *periph) 
{
	se_error_t ret_code = SE_SUCCESS;

	return ret_code;
}


/****************************************************************** 
Function:		proto_i2c_transceive 
Description:	通过I2C 接口向从设备命令发送命令及接收命令响应时，调用此函数
			1、根据 HED_I2C 通信协议，调用proto_i2c_handle 函数，发送及接收协议帧数据。
Input:	   periph 设备句柄
		   sbuf 输入双向队列的起始地址
		   slen 输入双向队列的输入数据长度
Output:       rbuf 输出双向队列的起始地址
		   rlen  输出双向队列的输出数据长度的起始地址
Return:        函数操作状态码
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
Description:   I2C 通信协议之复位设备的操作
                    1. 通过port层设备注册的函数列表指针，调用port层的control函数，控制SE 的RST引脚进行复位操作
                    2. 若需RESET 请求，调用proto_i2c_reset_request函数向SE 发送RESET请求帧及接收响应
                    3. 若需ATR 请求，调用proto_i2c_atr_request 函数向SE 发送 ATR 请求帧及接收ATR 数据
Input:        periph 设备句柄 
Output:      rbuf  待接收ATR的起始地址
		  rlen  待接收ATR的长度
Return:      函数操作状态码
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
		//控制SE 的RST引脚进行复位操作
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
Description:	通过i2c接口向从设备发送控制命令
Input:	   periph 设备句柄
		   ctrlcode 命令控制码
		   sbuf 输入数据的起始地址
		   slen 输入数据的长度
Output:    rbuf 输出数据的起始地址
		   rlen  输出数据长度的起始地址
Return:    函数操作状态码
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
Description:  实现微秒级延时
Input:        periph 设备句柄 
              us     延时微秒数
Return:      函数操作状态码
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
	//延时指定微秒时间
	pI2cPeriph->delay(us);
	return stErrCode;
}




