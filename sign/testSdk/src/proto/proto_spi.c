/**@file  proto_spi.c
* @brief  SPI 接口链路通信协议层驱动程序( 根据SPI HED 通信协议规范实现)
* @author  liangww
* @date  2021-04-28
* @version	V1.0
* @copyright  Copyright(C),CEC Huada Electronic Design Co.,Ltd.
*/

/***************************************************************************
* Include Header Files
***************************************************************************/

#include "proto_spi.h"
#include "error.h"
#include "log.h"
uint8_t reset_buf[PROTO_SPI_RERESET_MAX_LEN ] = {0};
uint8_t atr_buf[PROTO_SPI_ATR_MAX_LEN+4] = {0};


/**************************************************************************
* Variable Declaration
***************************************************************************/
const uint8_t cRESETAPDU_HED_SE[ACTIVE_REQ_FRAME_LEN] = {PIB_ACTIVE_FRAME,0x00,0x04,PIB_ACTIVE_FRAME_RESET,0x00,0x89,0xC4};
const uint8_t cRATRAPDU_HED_SE[ACTIVE_REQ_FRAME_LEN] = {PIB_ACTIVE_FRAME,0x00,0x04,PIB_ACTIVE_FRAME_RATR,0x00,0xF3,0x6B};
const uint8_t cNAK_HED_SE[PROCESS_FRAME_LEN]={PIB_PROCESS_FRAME,0x00,0x03,PIB_PROCESS_FRAME_NAK_CRC_INFO,0xD4,0x3A};
const uint8_t cWTX_HED_SE[PROCESS_FRAME_LEN]={PIB_PROCESS_FRAME,0x00,0x03,PIB_PROCESS_FRAME_WTX_INFO,0xD3,0x4C};
static spi_param_t g_spi_param[MAX_PERIPHERAL_DEVICE]={{0,0,0,0,SPI_DEFAULT},{0,0,0,0,SPI_DEFAULT},{0,0,0,0,SPI_DEFAULT},{0,0,0,0,SPI_DEFAULT}};
uint8_t   g_bSPIHedOpenSeMode = HED20_SPI_OPEN_SE_RESET_REQ;               //打开连接SE 时的操作模式
uint8_t   g_bSPIHedRstSeMode = HED20_SPI_RESET_SE_RESET_REQ;                  //复位SE 时的操作模式

static peripheral_bus_driver g_proto_spi= {
    PERIPHERAL_SPI,
   {NULL},
    proto_spi_init,
    proto_spi_deinit,
    proto_spi_open,
    proto_spi_close,
    proto_spi_transceive,
    proto_spi_reset,
    proto_spi_control,
    proto_spi_delay,
    NULL
};


PERIPHERAL_BUS_DRIVER_REGISTER(PERIPHERAL_SPI, g_proto_spi);


/** @addtogroup SE_Driver
  * @{
  */

/** @addtogroup PROTO 
  * @brief link protocol layer.
  * @{
  */


/** @defgroup PROTO_SPI PROTO_SPI
  * @brief hed spi communication driver.
  * @{
  */



/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function  -----------------------------------------------*/
/** @defgroup Proto_Spi_Private_Functions Proto_Spi Private Functions
 * @{
 */
 

/**
* @brief 通过SPI接口发送数据
* -# 通过port层设备注册的函数列表指针，调用port层spi接口的芯片选择函数 chip_select
* -# 通过port层设备注册的函数列表指针，调用port层spi接口的发送函数transmit
* @param [in] periph  设备句柄
* @param [in] inbuf  待发送数据的起始地址
* @param [in] inbuf_len 待发送数据的长度
* @return 函数操作状态码	
* @note no
* @see no
*/
se_error_t proto_spi_transmit(peripheral *periph, uint8_t *inbuf,uint32_t inbuf_len)
{
	se_error_t ret_code = SE_SUCCESS;

	HAL_SPI_PERIPHERAL_STRUCT_POINTER pSpiPeriph = (HAL_SPI_PERIPHERAL_STRUCT_POINTER)periph;

	do
	{
		if(periph == NULL)
		{
			ret_code =	SE_ERR_HANDLE_INVALID;
			break;
		}
		
		if ((inbuf == NULL) || (inbuf_len == 0U))
		{
			ret_code =	SE_ERR_PARAM_INVALID;
			break;
		}
		
		pSpiPeriph->chip_select(pSpiPeriph,TRUE);
		ret_code = pSpiPeriph->transmit(pSpiPeriph,inbuf,inbuf_len);	
		pSpiPeriph->chip_select(pSpiPeriph,FALSE);
		
	}while(0);

	return ret_code;
}



/**
* @brief 通过SPI接口接收数据
* -# 通过port层设备注册的函数列表指针，调用port层spi接口的芯片选择函数 chip_select
* -# 通过port层设备注册的函数列表指针，调用port层spi接口的接收函数receive
* @param [in] periph  设备句柄
* @param [out] outbuf  接收数据的起始地址
* @param [out] outbuf_len 接收数据的长度
* @return 函数操作状态码	
*/
se_error_t proto_spi_receive(peripheral *periph, uint8_t *outbuf, uint32_t outbuf_len)
{
	se_error_t ret_code = SE_SUCCESS;
	HAL_SPI_PERIPHERAL_STRUCT_POINTER pSpiPeriph = (HAL_SPI_PERIPHERAL_STRUCT_POINTER)periph;

	do
	{
		if(periph == NULL)
		{
			ret_code =	SE_ERR_HANDLE_INVALID;
			break;
		}
		
		if ((outbuf == NULL) || (outbuf_len == 0U))
		{
			ret_code =	SE_ERR_PARAM_INVALID;
			break;
		}
		
		memset(outbuf,0xff,outbuf_len);
		pSpiPeriph->chip_select(pSpiPeriph,TRUE);
		ret_code = pSpiPeriph->receive(pSpiPeriph,outbuf,&outbuf_len);	
		pSpiPeriph->chip_select(pSpiPeriph,FALSE);
		
	}while(0);

	return ret_code;
}


/*************************************************
  Function:	  proto_spi_crc16
  Description:  计算指定长度指定数据的CRC结果
  Input:	
            CRCType：CRC计算类sing
            Length：要计算的数据长度
            Data：要计算的数据的起始地址
  Return:	crc计算结果	
  Others:		
*************************************************/
uint16_t proto_spi_crc16(uint32_t CRCType,uint32_t Length ,uint8_t *Data)
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


/**
* @brief 按照HED SPI通信协议的帧格式，发送一帧数据
* @param [in] periph  设备句柄
* @param [in] param  通信参数信息
* @param [in] inbuf  待发送数据的起始地址
* @param [in] inbuf_len 待发送数据的长度
* @return 函数操作状态码	
* @note 协议时间参数WPT~T3 需符合SE 芯片要求
* @see 函数  proto_spi_transmit
*/
se_error_t proto_spi_send_frame(peripheral *periph, spi_param_t *param, uint8_t *inbuf, uint32_t inbuf_len)
{
	se_error_t ret_code = SE_SUCCESS;


	uint8_t bData[WAKEUP_DATA_LEN+1]={0};
	HAL_SPI_PERIPHERAL_STRUCT_POINTER p_spi_periph = (HAL_SPI_PERIPHERAL_STRUCT_POINTER)periph;

	do
	{
		if(periph == NULL)
		{
			ret_code =	SE_ERR_HANDLE_INVALID;
			break;
		}
		
		if ((param == NULL) || (inbuf == NULL) ||(inbuf_len < FRAME_HEAD_LEN))
		{
			ret_code =	SE_ERR_PARAM_INVALID;
			break;
		}
		
		memset(bData, 0x00, (WAKEUP_DATA_LEN+1));  //debug
		ret_code = proto_spi_transmit(periph,bData, (WAKEUP_DATA_LEN+1)); //send wakeup data
		if(ret_code !=SE_SUCCESS)
		{
			break;
		}
		
		p_spi_periph->delay(SPI_SEND_CS_WAKEUP_TIME);  //delay WPT
		
		ret_code = proto_spi_transmit(periph,inbuf, inbuf_len);	   //send PIB LEN DATA EDC 
		if(ret_code !=SE_SUCCESS)
		{
			break;
		}
			
		p_spi_periph->delay(SPI_SEND_DATA_OVER_WAIT_TIME);	//delay T3
	}while(0);

	return ret_code;
}


/**
* @brief 按照HED SPI通信协议的帧格式，接收帧头(起始域：PIB LEN)
* @param [in] periph  设备句柄
* @param [out] outbuf  待接收帧头的起始地址
* @param [out] outbuf_len 待接收帧头的长度
* @return 函数操作状态码	
* @note 协议时间参数T0~T3 需符合SE 芯片要求
* @see 函数  proto_spi_receive
*/
se_error_t proto_spi_receive_frame_head(peripheral *periph, uint8_t *outbuf, uint32_t *outbuf_len)
{
	se_error_t ret_code = SE_SUCCESS;

	do
	{
		if(periph == NULL)
		{
			ret_code =	SE_ERR_HANDLE_INVALID;
			break;
		}

		if ((outbuf == NULL) || (outbuf_len == NULL))
		{
			ret_code =	SE_ERR_PARAM_INVALID;
			break;
		}

		ret_code = proto_spi_receive(periph, outbuf, FRAME_HEAD_LEN);		   //recieve PIB LEN1 LEN2
		if(ret_code != SE_SUCCESS)
		{
					break;
		}
		*outbuf_len = (*(outbuf+LEN_OFFSET) <<8) + *(outbuf+LEN_OFFSET+1); 
		
	}while(0);

	 return ret_code;
}


/**
* @brief 按照HED SPI通信协议的帧格式，接收帧数据(信息域/终止域)
* @param [in] periph  设备句柄
* @param [in] param  通信参数信息
* @param [out] outbuf  待接收帧数据的起始地址
* @param [out] outbuf_len 待接收帧数据的长度
* @return 函数操作状态码	
* @note 协议时间参数T5 需符合SE 芯片要求
* @see 函数  proto_spi_receive
*/
se_error_t proto_spi_receive_frame_data(peripheral *periph, spi_param_t *param, uint8_t *outbuf, uint32_t outbuf_len)
{
	se_error_t ret_code = SE_SUCCESS;
	
	HAL_SPI_PERIPHERAL_STRUCT_POINTER p_spi_periph = (HAL_SPI_PERIPHERAL_STRUCT_POINTER)periph;

	do
	{
		if(periph == NULL)
		{
			ret_code =	SE_ERR_HANDLE_INVALID;
			break;
		}

		if ((param == NULL) || (outbuf == NULL) || (outbuf_len == NULL))
		{
			ret_code =	SE_ERR_PARAM_INVALID;
			break;
		}

	    p_spi_periph->delay(SPI_BEFORE_RECEIVE_DATA_WAIT_TIME); //delay T5
			
		ret_code = proto_spi_receive(periph, outbuf, outbuf_len);
		if(ret_code != SE_SUCCESS)
	   	{
	   		return ret_code;
	   	}	  

		
	}while(0);
			   
	return ret_code;
}


/**
* @brief 按照HED SPI通信协议的帧格式，接收帧激活帧数据，等待接收时按帧等待时间启动接收数据超时的计时操作
* @param [in] periph  设备句柄
* @param [in] param  通信参数信息
* @param [out] outbuf  待接收激活帧的起始地址
* @param [out] outbuf_len 待接收激活帧的长度
* @return 函数操作状态码	
* @note 协议时间参数T4 需符合SE 芯片要求
* @see 函数  proto_spi_receive_frame_head
*/
se_error_t proto_spi_receive_active_frame(peripheral *periph, spi_param_t *param, uint8_t *output, uint32_t *output_len)
{
	se_error_t retCode = SE_SUCCESS;
	util_timer_t timer={0};
	uint16_t edc_value ;
	uint32_t len = 0;
	uint16_t send_nak_count = 0; 
	HAL_SPI_PERIPHERAL_STRUCT_POINTER p_spi_periph = (HAL_SPI_PERIPHERAL_STRUCT_POINTER)periph;

	if(periph == NULL)
	{
		return SE_ERR_HANDLE_INVALID;
	}

	if ((param == NULL) || (output == NULL) || (*output_len < FRAME_HEAD_LEN))
	{
		return SE_ERR_PARAM_INVALID;
	}


	//设置接收数据时帧等待的超时时间
	timer.interval = SPI_RECEVIE_FRAME_WAIT_TIME; 
	p_spi_periph->timer_start(&timer);

	//receive frame head
	do
	{
		p_spi_periph->delay(SPI_RESET_POLL_SLAVE_INTERVAL_TIME);   //delay T4
		retCode = proto_spi_receive_frame_head(periph,output,&len);
		if(retCode != SE_SUCCESS)
			return retCode;

		if(output[PIB_OFFSET] == PIB_ACTIVE_FRAME)
		{
			break;
		}
	    //判断是否为过程帧 
        if(output[PIB_OFFSET] == PIB_PROCESS_FRAME)
		{
			break;
		}
		if(p_spi_periph->timer_differ(&timer) != SE_SUCCESS)
		{
			LOGE("Failed:receive frame overtime,  ErrCode-%08X.", SE_ERR_TIMEOUT);
			
			return SE_ERR_TIMEOUT;	
		}
	}while(1);

	do
	{
		if((len+FRAME_HEAD_LEN) > *output_len)
		{
			retCode= SE_ERR_LEN;
			break;
		}
		
		retCode = proto_spi_receive_frame_data(periph,param,output+FRAME_HEAD_LEN, len);
		if(retCode != SE_SUCCESS)
		{
			break;
		}
		
		//检查edc
		edc_value = 0;
		//len = len+FRAME_HEAD_LEN;
		edc_value = proto_spi_crc16(CRC_B, len+FRAME_HEAD_LEN-EDC_LEN, output);
		if(((edc_value>>8) & 0xff)!=output[len+FRAME_HEAD_LEN-EDC_LEN+1]||(edc_value & 0xff)!=output[len+FRAME_HEAD_LEN-EDC_LEN])
		{
					
            send_nak_count++; 
			if(send_nak_count > PROTO_SPI_RETRY_NUM)
			{
			   return SE_ERR_LRC_CRC;
			  
			}
			
			p_spi_periph->delay(SPI_SEND_BGT_TIME);	 //延时BGT
			retCode = proto_spi_send_frame(periph,param,(uint8_t *)cNAK_HED_SE,PROCESS_FRAME_LEN); //向从设备发送NAK帧
			if(retCode!= SE_SUCCESS)
			{
				return retCode;
			}
			continue;
         }
		
		*output_len = len+FRAME_HEAD_LEN;
		break;

	}while(1);

	return retCode;
}



/**
* @brief 发送复位请求帧030004D3xxxxxx ，接收应答帧030004D3xxxxxxx
* @param [in] periph  设备句柄
* @param [in] param  通信参数信息
* @return 函数操作状态码	
* @see 函数  proto_spi_send_frame   proto_spi_receive_active_frame
*/
se_error_t proto_spi_reset_frame(peripheral *periph, spi_param_t *param, uint8_t *rbuf, uint32_t *rlen)
{
	se_error_t ret_code = SE_SUCCESS;
	uint32_t bufsize = 0; 
	//uint8_t bApduData[ACTIVE_REQ_FRAME_LEN] = {0};	
	uint8_t tmp_buf[ACTIVE_REQ_FRAME_LEN] = {0};
	uint16_t spi_edc = 0;
	uint16_t rec_nak_count = 0;
	uint16_t rec_time_out_count = 0;

	do
	{
		if(periph == NULL)
		{
			ret_code =	SE_ERR_HANDLE_INVALID;
			break;
		}

		if (param == NULL)
		{
			ret_code =	SE_ERR_PARAM_INVALID;
			break;
		}
		memcpy((uint8_t *)tmp_buf, &cRESETAPDU_HED_SE[PIB_OFFSET], ACTIVE_REQ_FRAME_LEN);

		tmp_buf[4] = param->pfsm/16;
		spi_edc= proto_spi_crc16(CRC_B, ACTIVE_REQ_FRAME_LEN-2, tmp_buf);
		tmp_buf[ACTIVE_REQ_FRAME_LEN-2]  = (uint8_t)spi_edc&0xff;//填充EDC_HIGH
		tmp_buf[ACTIVE_REQ_FRAME_LEN-1]  =(uint8_t)(spi_edc>>8)&0xff;//填充EDC_LOW
		
	
	   //发送复位帧数据
		ret_code = proto_spi_send_frame(periph, param, tmp_buf,ACTIVE_REQ_FRAME_LEN);
		if(ret_code!= SE_SUCCESS)
		{
			break;
		}
		bufsize = *rlen;//设置buf大小		

		//接收复位请求返回帧
		ret_code = proto_spi_receive_active_frame(periph, param, rbuf, &bufsize);
		
		if(ret_code!= SE_SUCCESS)
		{
            if(ret_code == SE_ERR_TIMEOUT)//超时重发一次
            {   rec_time_out_count++;
			    if(rec_time_out_count < 2)
			    {
 			    	continue;
				}
			}
			break;
		}
        //检查是否为NAK帧
        if(rbuf[DATA_OFFSET] == PIB_PROCESS_FRAME_NAK_CRC_INFO)
		{
			//ret_code = SE_ERR_RESET;
			rec_nak_count++;
			if(rec_nak_count > PROTO_SPI_RETRY_NUM )//重发3次reset帧
			{
              LOGE("Failed:communication cannot recover,  ErrCode-%08X.", SE_ERR_COMM);
			  ret_code = SE_ERR_COMM;
			  return SE_ERR_COMM ;
			}
			continue;
		}
		// 增加检查是否为RESET响应
		if(rbuf[DATA_OFFSET] != PIB_ACTIVE_FRAME_RESET)
		{
			//ret_code = SE_ERR_RESET;
			ret_code = SE_ERR_DATA;
			break;
		}
		
		
		*rlen = bufsize;

		if(bufsize != ACTIVE_REQ_FRAME_LEN) //reset帧长度
		{
			ret_code = SE_ERR_LEN;
			break;
		}
		break;

	}while(1);

	return ret_code;
}


/**
* @brief 发送RATR命令序列，获取ATR（发送030004E2XXXXXX，接收ATR信息）
* @param [in] periph  设备句柄
* @param [in] param  通信参数信息
* @param [out] rbuf  待接收ATR的起始地址
* @param [out] rlen 待接收ATR的长度
* @return 函数操作状态码	
* @see 函数  proto_spi_send_frame   proto_spi_receive_active_frame
*/
se_error_t proto_spi_ratr_frame(peripheral *periph, spi_param_t *param, uint8_t *rbuf, uint32_t *rlen)
{
	se_error_t ret_code = SE_SUCCESS;
	uint32_t bufsize = 0;
	uint8_t tmp_buf[ACTIVE_REQ_FRAME_LEN] = {0};
	uint16_t spi_edc = 0;
    uint16_t rec_nak_count = 0;
	uint16_t rec_time_out_count = 0;
	uint16_t rec_len = 0;
	uint8_t reset_buf[PROTO_SPI_RERESET_MAX_LEN ] = {0};
	uint8_t dev_id = 0;
	HAL_SPI_PERIPHERAL_STRUCT_POINTER p_spi_periph = (HAL_SPI_PERIPHERAL_STRUCT_POINTER)periph;
	
	do
	{
		if(periph == NULL)
		{
			ret_code =	SE_ERR_HANDLE_INVALID;
			break;
		}

		if ((param == NULL) || (rbuf == NULL) || (rlen == NULL))
		{
			ret_code =	SE_ERR_PARAM_INVALID;
			break;
		}
		
		memcpy((uint8_t *)tmp_buf, &cRATRAPDU_HED_SE[PIB_OFFSET], ACTIVE_REQ_FRAME_LEN);

		tmp_buf[4] = param->hbsm/16;
		spi_edc = proto_spi_crc16(CRC_B, ACTIVE_REQ_FRAME_LEN-2, tmp_buf);
		tmp_buf[ACTIVE_REQ_FRAME_LEN-2]  = spi_edc&0xff;//填充EDC_HIGH
		tmp_buf[ACTIVE_REQ_FRAME_LEN-1]  = (spi_edc>>8)&0xff;//填充EDC_LOW

		ret_code = proto_spi_send_frame(periph, param, tmp_buf,ACTIVE_REQ_FRAME_LEN);
		if(ret_code!= SE_SUCCESS)
		{
			break;
		}

		bufsize = *rlen;//设置buf大小
		  
		ret_code = proto_spi_receive_active_frame(periph, param, rbuf, &bufsize);
         if(ret_code != SE_SUCCESS)
         {
			//超时重发一次
			 if(ret_code == SE_ERR_TIMEOUT)
	        {   rec_time_out_count++;
			    if(rec_time_out_count < 2)
			    {
				    	continue;
				}
			} 

			break;
		 }
			 
       //检查是否为NAK帧
    	if(rbuf[DATA_OFFSET] == PIB_PROCESS_FRAME_NAK_CRC_INFO)
		{
			//ret_code = SE_ERR_RESET;
			rec_nak_count++;
			if(rec_nak_count > PROTO_SPI_RETRY_NUM )//重发ratr帧3次
			{
	          ret_code = SE_ERR_LRC_CRC;
			  break;
			}
			else
			{
			  continue;
			}
			
		}
		                
		// 增加检查是否为ratr响应
		if(rbuf[DATA_OFFSET] != PIB_ACTIVE_FRAME_RATR_RESPONSE)
		{
			ret_code = SE_ERR_ATR;
			break;
		}
		
		*rlen = bufsize - FRAME_HEAD_LEN -EDC_LEN;
		break;
		
	}while(1);

	return ret_code;
}



/**
* @brief 按照HED SPI通信协议的帧格式与SE进行收发数据交互
* @param [in] periph  设备句柄
* @param [in] param  通信参数信息
* @param [in] inbuf  输入数据指针
* @param [in] inbuf_len 输入数据长度
* @param [out] outbuf  输出数据存储地址
* @param [out] outbuf_len 输出数据长度
* return 函数操作状态码	
* @note 交互期间需根据接收帧类型及EDC校验等，发送延时帧、NAK帧及错误时帧重发操作
* @see 函数  proto_spi_send_frame   proto_spi_receive_frame_head  proto_spi_receive_frame_data
*/
se_error_t proto_spi_handle(peripheral *periph, spi_param_t *param, uint8_t *inbuf, uint32_t inbuf_len, uint8_t *outbuf, uint32_t *outbuf_len)
{
	se_error_t ret_code = SE_SUCCESS;
	util_timer_t timer = {0};
	uint16_t edc_value ;
	uint32_t rec_len = 0;
	//uint8_t dev_id = 0;
	uint16_t send_nak_count = 0; 
	uint16_t rec_nak_count = 0;
	//uint16_t rec_wtx_count = 0;
	uint16_t re_tran_count = 0;
	HAL_SPI_PERIPHERAL_STRUCT_POINTER p_spi_periph = (HAL_SPI_PERIPHERAL_STRUCT_POINTER)periph;

	if(periph == NULL)
	{
		return SE_ERR_HANDLE_INVALID;
	}

	if ((param == NULL) || (inbuf == NULL) ||(inbuf_len > FRAME_LEN_MAX) ||(outbuf == NULL))
	{
		return SE_ERR_PARAM_INVALID;
	}

	//1.发送帧数据
	param->type = SPI_INFO;
	ret_code = proto_spi_send_frame(periph, param, inbuf, inbuf_len);
	if(ret_code != SE_SUCCESS)
		return ret_code;
	
   //2. 设置接收数据时帧等待的超时时间
	timer.interval = SPI_RECEVIE_FRAME_WAIT_TIME; 
	p_spi_periph->timer_start(&timer);	
		
	do
	{	
        //3.接收帧数据的起始域，帧等待时间内未接收到将超时
		do
		{
			ret_code = proto_spi_receive_frame_head(periph,outbuf,&rec_len);
			if(ret_code!= SE_SUCCESS)
			{
				return ret_code;
			}

			p_spi_periph->delay(SPI_RESET_POLL_SLAVE_INTERVAL_TIME);   //delay T4

			if(p_spi_periph->timer_differ(&timer) != SE_SUCCESS)
			{	
				re_tran_count++;
				break;
			}

		    if((outbuf[PIB_OFFSET] == PIB_INFORMATION_FRAME) ||(outbuf[PIB_OFFSET] == PIB_PROCESS_FRAME))
		    {
		       re_tran_count = 0;
			   break;
		    }

		}while(1);

		//当第一次超时的时候需要进行超时重发信息帧。
		if(re_tran_count==1)
		{  
			p_spi_periph->delay(SPI_SEND_BGT_TIME); 	//延时BGT
			ret_code = proto_spi_send_frame(periph, param, inbuf, inbuf_len);
			 //重新计时
		    timer.interval = SPI_RECEVIE_FRAME_WAIT_TIME; 
			p_spi_periph->timer_start(&timer);	
			continue;
		}
		//若仍然超时，则返回超时错误状态。
		else if(re_tran_count==2)
		{
            LOGE("Failed:receive frame overtime,  ErrCode-%08X.", SE_ERR_TIMEOUT);
			return SE_ERR_TIMEOUT;  	   
		}
		
		
		if(rec_len > (FRAME_DATA_LEN_MAX+EDC_LEN))
		{
			return SE_ERR_LEN;
		}

		//4. 接收帧数据的信息域和终止域
		ret_code = proto_spi_receive_frame_data(periph, param, outbuf+FRAME_HEAD_LEN, rec_len);
		if(ret_code!= SE_SUCCESS)
		{
			return ret_code;
		}

		//5.校验接收帧数据的EDC值
		edc_value = 0;
		rec_len = rec_len+FRAME_HEAD_LEN;
		edc_value = proto_spi_crc16(CRC_B, rec_len-EDC_LEN, outbuf);
		if((((edc_value>>8) & 0xff)==outbuf[rec_len -EDC_LEN+1])&&((edc_value & 0xff)==outbuf[rec_len -EDC_LEN]))	//EDC 正确
		{
			send_nak_count = 0;

			//6.检查接收帧类型，并根据帧类型进行处理
			if(((outbuf[PIB_OFFSET] == PIB_PROCESS_FRAME)&&(outbuf[DATA_OFFSET] == PIB_PROCESS_FRAME_NAK_CRC_INFO))||((outbuf[PIB_OFFSET] == PIB_PROCESS_FRAME)&&(outbuf[DATA_OFFSET] == PIB_PROCESS_FRAME_NAK_OTHER_INFO)))
			{
				//接收到的帧类型为NAK帧，将重发帧数据;接收三次NAK后，返回错误码。
				if(param->type == SPI_INFO)
				{
					rec_nak_count++;
					if(rec_nak_count > PROTO_SPI_RETRY_NUM)
					{
					return SE_ERR_LRC_CRC;	
					}
					p_spi_periph->delay(SPI_SEND_BGT_TIME);	 //延时BGT
					ret_code = proto_spi_send_frame(periph, param, inbuf, inbuf_len);  //重发信息帧
				}
				else if(param->type == SPI_WTX)
				{
					p_spi_periph->delay(SPI_SEND_BGT_TIME);	 //延时BGT
					ret_code = proto_spi_send_frame(periph, param,(uint8_t *)cWTX_HED_SE,PROCESS_FRAME_LEN);	 //重发WTX帧
				}

				if(ret_code != SE_SUCCESS)	//重发或发送wtx 失败
				{
					return ret_code;
				}

			}

			else if((outbuf[PIB_OFFSET] == PIB_PROCESS_FRAME)&&(outbuf[DATA_OFFSET] == PIB_PROCESS_FRAME_WTX_INFO))
			{
				//接收到的帧类型为延时申请，发送延时帧进行响应
				param->type = SPI_WTX;
				ret_code = proto_spi_send_frame(periph, param,(uint8_t *)cWTX_HED_SE,PROCESS_FRAME_LEN);
				if(ret_code!= SE_SUCCESS)
				{
					return ret_code;
				}
				//重新计超时
				timer.interval = SPI_RECEVIE_FRAME_WAIT_TIME; 
				p_spi_periph->timer_start(&timer);
			}

			else if(outbuf[PIB_OFFSET]	== PIB_INFORMATION_FRAME)
			{
				//接收到的帧类型为信息帧，退出帧数据收发交互处理
				*outbuf_len = rec_len;
				return SE_SUCCESS;
			}
			else
			{
				return SE_ERR_PARAM_INVALID;
			}
		}
		else   //EDC 错误
		{
			send_nak_count++; 
			if(send_nak_count > PROTO_SPI_RETRY_NUM)
			{
			   return SE_ERR_LRC_CRC;	
			}
			
			p_spi_periph->delay(SPI_SEND_BGT_TIME);	 //延时BGT
			ret_code = proto_spi_send_frame(periph,param,(uint8_t *)cNAK_HED_SE,PROCESS_FRAME_LEN); //向从设备发送NAK帧
			if(ret_code!= SE_SUCCESS)
			{
				return ret_code;
			}
		}
	}while(1);

}


/**
* @brief 根据HED SPI协议格式，向双向队列中增加帧头和帧尾数据
* -# 向双向队列头部添加 PIB, LEN
* -# 向双向队列尾部添加LRC
* @param [in] periph  设备句柄
* @param [in] inbuf 输入双向队列的起始地址
* @param [in] inbuf_len 输入双向队列的输入数据长度
* @return 函数操作状态码	
* @note no
* @see no
*/
se_error_t proto_spi_queue_in(peripheral *periph, uint8_t* inbuf, uint16_t inbuf_len)
{
	se_error_t ret_code = SE_SUCCESS;
	double_queue queue_in = (double_queue)inbuf;
	uint16_t frame_len = 0;
	uint8_t frame_head[3] = {0};
    uint16_t spi_edc = 0;
	uint8_t tmp_buf[2]={0};
	do
	{
		if(periph == NULL)
		{
			ret_code =	SE_ERR_HANDLE_INVALID;
			break;
		}
		
		if ((inbuf == NULL) ||(inbuf_len == 0U))
		{
			ret_code =	SE_ERR_PARAM_INVALID;
			break;
		}
		

		//向双向队列分别添加起始域(PIB, LEN)
		frame_len = (uint16_t)(inbuf_len +EDC_LEN);
		frame_head[0] = PIB_INFORMATION_FRAME;
		frame_head[1] = (frame_len>>8)&0xff;	//LEN_High
		frame_head[2] = frame_len&0xff; 		//LEN_Low
		util_queue_front_push(frame_head, FRAME_HEAD_LEN,queue_in);
		
		
		//计算CRC16，并向双向队列添加 EDC 
		spi_edc = proto_spi_crc16(CRC_B,util_queue_size(queue_in), &queue_in->q_buf[queue_in->front_node]);	//4 //计算 CRC
		tmp_buf[0]  = spi_edc&0xff;//填充EDC_HIGH
		tmp_buf[1]  = (spi_edc>>8)&0xff;//填充EDC_LOW
		util_queue_rear_push(tmp_buf, 2, queue_in);
		
	}while(0);

	return ret_code;
}


/**
  * @}
  */





/* Exported functions --------------------------------------------------------*/

/** @defgroup Proto_Spi_Exported_Functions Proto_Spi Exported Functions
  * @{
  */


/**
* @brief 华大SPI 通信协议参数初始化
* -# FSM/FSS等参数初始化
* -# 通过port层设备注册的函数列表指针，调用port层spi接口的初始化函数init
* @param [in] periph  设备句柄
* @return 函数操作状态码	
* @note no
* @see 函数  port_spi_periph_init
*/
se_error_t proto_spi_init(peripheral *periph) 
{
	se_error_t ret_code = SE_SUCCESS;
	util_timer_t timer = {0};
	uint8_t dev_id = 0;
	
	HAL_SPI_PERIPHERAL_STRUCT_POINTER p_spi_periph = (HAL_SPI_PERIPHERAL_STRUCT_POINTER)periph;

	if(p_spi_periph == NULL)
	{
		return SE_ERR_HANDLE_INVALID;
	}

	//设置锁等待的超时时间
	timer.interval = SPI_COMM_MUTEX_WAIT_TIME; 
	p_spi_periph->timer_start(&timer);

	do
	{
		if(p_spi_periph->timer_differ(&timer) != SE_SUCCESS)
		{
			ret_code = SE_ERR_TIMEOUT;	
			LOGE("Failed:init mutex,  ErrCode-%08X.", ret_code);
			break;
		}
		
		
		
		dev_id = p_spi_periph->periph.id;
		
		g_spi_param[dev_id].pfsm= PROTO_SPI_PFSM_DEFAULT;
		g_spi_param[dev_id].pfss= PROTO_SPI_PFSS_DEFAULT;
		g_spi_param[dev_id].hbsm= PROTO_SPI_HBSM_DEFAULT;
		g_spi_param[dev_id].hbss= PROTO_SPI_HBSS_DEFAULT;
		
		ret_code = p_spi_periph->init(p_spi_periph);	
		if(ret_code != SE_SUCCESS)
		{
			LOGE("Failed:spi potocol,  ErrCode-%08X.", ret_code);
		}
		else
		{
			LOGI("Success!");
		}
		break;
	}while(1);
	


	return ret_code;
}


/**
* @brief 华大SPI 通信协议参数终止化
* -# FSM/FSS等参数恢复默认值
* -# 通过port层设备注册的函数列表指针，调用port层spi接口的 终止化函数deinit
* @param [in] periph  设备句柄
* @return 函数操作状态码	
* @note no
* @see 函数    port_spi_periph_deinit
*/
se_error_t proto_spi_deinit(peripheral *periph) 
{
	se_error_t ret_code = SE_SUCCESS;
	util_timer_t timer = {0};

	HAL_SPI_PERIPHERAL_STRUCT_POINTER p_spi_periph = (HAL_SPI_PERIPHERAL_STRUCT_POINTER)periph;

	if(p_spi_periph == NULL)
	{
		return SE_ERR_HANDLE_INVALID;
	}

	//设置等待的超时时间
	timer.interval = SPI_COMM_MUTEX_WAIT_TIME; 
	p_spi_periph->timer_start(&timer);


	do
	{
		if(p_spi_periph->timer_differ(&timer) != SE_SUCCESS)
		{
			ret_code = SE_ERR_TIMEOUT;	
			LOGE("Failed:deinit mutex,  ErrCode-%08X.", ret_code);
			break;
		}
		
	

		ret_code = p_spi_periph->deinit(p_spi_periph);
		if(ret_code != SE_SUCCESS)
		{
			LOGE("Failed:spi potocol,  ErrCode-%08X.", ret_code);
		}
		else
		{
			LOGI("Success!");
		}
		break;

	}while(1);

	
	return ret_code;
}


/**
* @brief 连接从设备时，调用此函数可以获取从设备的ATR
* -# 通过port层设备注册的函数列表指针，调用port层spi接口的 control函数对SE进行复位操椎
* -# 发送复位命令序列
* -# 发送RATR命令序列，获取从设备ATR值
* @param [in] periph  设备句柄
* @param [out] rbuf  待接收ATR的起始地址
* @param [out] rlen 待接收ATR的长度
* @return 函数操作状态码	
* @note no
* @see 函数  port_spi_periph_control   proto_spi_reset_frame proto_spi_ratr_frame
*/
se_error_t proto_spi_open(peripheral *periph , uint8_t *rbuf, uint32_t *rlen) 
{
	se_error_t ret_code = SE_SUCCESS;
	util_timer_t timer = {0};
	uint32_t rec_len = 0;
	uint8_t dev_id = 0;
	//uint8_t atr_buf[PROTO_SPI_ATR_MAX_LEN+4] = {0};
	//uint8_t reset_buf[PROTO_SPI_RERESET_MAX_LEN ] = {0};
	HAL_SPI_PERIPHERAL_STRUCT_POINTER p_spi_periph = (HAL_SPI_PERIPHERAL_STRUCT_POINTER)periph;
	uint16_t pfss = 0;
	uint16_t hbss = 0;
	uint16_t pfssi[16] = {0,16,32,64,128,256,272,384,512,1024,2048,4096,8192,16384,16384,16384};	
	uint8_t index = 0;
	if(p_spi_periph == NULL)
	{
		return SE_ERR_HANDLE_INVALID;
	}

	if (((rbuf == NULL) && (rlen != NULL))||((rbuf != NULL) && (rlen == NULL)))
	{
		return  SE_ERR_PARAM_INVALID;
	}

	//设置等待的超时时间
	timer.interval = SPI_COMM_MUTEX_WAIT_TIME; 
	p_spi_periph->timer_start(&timer);

	do
	{
		if(p_spi_periph->timer_differ(&timer) != SE_SUCCESS)
		{
			ret_code = SE_ERR_TIMEOUT;	
			LOGE("Failed:open periph mutex,  ErrCode-%08X.", ret_code);
			break;
		}
		
		
			//控制SE 的RST引脚进行复位操作
			//ret_code = p_spi_periph->control(p_spi_periph, PROTO_SPI_CTRL_RST, NULL, NULL);
			//if(ret_code != SE_SUCCESS)
		//	{
			//	LOGE("Failed:protocol rst io control,  ErrCode-%08X.", ret_code);
				//break;
			//}
		
        p_spi_periph->delay(SPI_PROTO_SE_RST_DELAY);//延迟10ms确定SE唤醒
		dev_id = p_spi_periph->periph.id;
		
		if(g_bSPIHedOpenSeMode == HED20_SPI_OPEN_SE_RESET_REQ)
		{
			//发送reset命令序列,获取返回值
			rec_len = PROTO_SPI_RERESET_MAX_LEN ;
			ret_code = proto_spi_reset_frame(periph, &g_spi_param[dev_id], reset_buf, &rec_len);
			if(ret_code == SE_ERR_BUSY)
			{
				continue;
			}
			else if(ret_code != SE_SUCCESS)
			{
				LOGE("Failed:protocol reset  frame,  ErrCode-%08X.", ret_code);
				break;
			}
			//根据华大SPI协议修改PFSS和PFSM,两者不一致时，取两者最小值
		
			//pfss = (uint16_t)reset_buf[DATA_OFFSET+1]*16;
			
			index = reset_buf[DATA_OFFSET+1];
			if(index > 16)
			{
			  index = 16;
			}
			pfss = pfssi[index];
			if(g_spi_param[dev_id].pfsm>=pfss)
			{
				g_spi_param[dev_id].pfsm = pfss;	
				g_spi_param[dev_id].pfss = pfss;	
			}
			else
			{
				g_spi_param[dev_id].pfss = g_spi_param[dev_id].pfsm;	
			}
			
		}
		
		if((rbuf != NULL)&&(rlen != NULL))
		{
			p_spi_periph->delay(SPI_SEND_BGT_TIME);
			//发送RATR 命令序列，获取ATR值
			rec_len = PROTO_SPI_ATR_MAX_LEN;
			ret_code = proto_spi_ratr_frame(periph, &g_spi_param[dev_id], atr_buf, &rec_len);
			if(ret_code == SE_ERR_BUSY)
			{
				continue;
			}
			else if(ret_code != SE_SUCCESS)
			{
				LOGE("Failed:protocol ratr  frame,  ErrCode-%08X.", ret_code);
				break;
			}

			memmove(rbuf,atr_buf+DATA_OFFSET,rec_len);

			//根据华大SPI协议修改HBSS和HBSM,两者不一致时，取两者最小值
			
			hbss = (uint16_t)atr_buf[DATA_OFFSET+2]*16;
			if(g_spi_param[dev_id].hbsm>=hbss)
			{
				g_spi_param[dev_id].hbsm = hbss;	
				g_spi_param[dev_id].hbss = hbss;	
			}
			else
			{
				g_spi_param[dev_id].hbss = g_spi_param[dev_id].hbsm;	
			}

			//rbuf[2] = 0x11;
			*rlen = rec_len;
		}
		LOGI("Open Periph Success!");
		break;
	}while(1);

	

	return ret_code;
}


/**
* @brief 应用关闭设备时，调用此函数
* @param [in] periph  设备句柄
* @return 函数操作状态码	
* @note 无代码实现
*/
se_error_t proto_spi_close(peripheral *periph) 
{
	se_error_t ret_code = SE_SUCCESS;

	return ret_code;
}


/**
* @brief 通过SPI接口向从设备命令及接收命令响应时，调用此函数
* -# 向双向队列分别添加起始域(PIB, LEN)
* -# 向双向队列分别添加终止域(EDC)
* -# 调用proto_spi_handle函数发送命令及接收命令响应
* @param [in] periph  设备句柄
* @param [in] sbuf 输入双向队列的起始地址
* @param [in] slen 输入双向队列的输入数据长度
* @param [out] rbuf  输出双向队列的起始地址
* @param [out] rlen 输出双向队列的输出数据长度的起始地址
* @return 函数操作状态码	
* @see 函数  porto_spi_queue_in   proto_spi_handle
*/
se_error_t proto_spi_transceive(peripheral *periph, uint8_t *sbuf, uint32_t  slen, uint8_t *rbuf, uint32_t *rlen)
{
	se_error_t ret_code = SE_SUCCESS;
//	se_error_t ret_code_bak = SE_SUCCESS;
	util_timer_t timer = {0};
	HAL_SPI_PERIPHERAL_STRUCT_POINTER p_spi_periph = (HAL_SPI_PERIPHERAL_STRUCT_POINTER)periph;
	double_queue queue_in = (double_queue)sbuf;
	double_queue queue_out = (double_queue)rbuf;
	uint16_t rec_len = 0;
	uint8_t dev_id = 0;
	uint8_t* p_input = NULL;
	uint8_t* p_output = NULL;
	uint8_t reset_buf[PROTO_SPI_RERESET_MAX_LEN ] = {0};

	if(p_spi_periph == NULL)
	{
		return SE_ERR_HANDLE_INVALID;
	}

	if ((sbuf == NULL) || (rbuf == NULL) || (slen == 0U) || (rlen == NULL))
	{
		return  SE_ERR_PARAM_INVALID;
	}

	//准备收发的数据
	ret_code =proto_spi_queue_in(periph, sbuf, slen);
	if(ret_code != SE_SUCCESS)
	{
		return ret_code;
	}

	p_input = &queue_in->q_buf[queue_in->front_node];
	p_output = &queue_out->q_buf[queue_out->front_node];
	dev_id = p_spi_periph->periph.id;

	//设置等待的超时时间
	timer.interval = SPI_COMM_MUTEX_WAIT_TIME; 
	p_spi_periph->timer_start(&timer);

	do
	{
		if(p_spi_periph->timer_differ(&timer) != SE_SUCCESS)
		{
			ret_code = SE_ERR_TIMEOUT;	
			LOGE("Failed:communication mutex,  ErrCode-%08X.", ret_code);
		    break;
		}
		

		//启动SPI数据的收发
		ret_code = proto_spi_handle(periph, &g_spi_param[dev_id], p_input,util_queue_size(queue_in),p_output,(uint32_t*)&rec_len);
		if(ret_code == SE_ERR_BUSY)
		{
			continue;
		}
		
		else if(ret_code == SE_ERR_LRC_CRC||ret_code == SE_ERR_TIMEOUT)
		{

			//发送reset命令序列
		    p_spi_periph->delay(SPI_SEND_BGT_TIME); 	//延时BGT
		    rec_len = PROTO_SPI_RERESET_MAX_LEN ;
	     
			if((proto_spi_reset_frame(periph, &g_spi_param[dev_id], reset_buf,(uint32_t*) &rec_len))!= SE_SUCCESS)
			{
				LOGE("Failed:communication cannot recover,  ErrCode-%08X.", SE_ERR_COMM);
				return SE_ERR_COMM ;
			}
			else
			{
                switch (ret_code) {
				case SE_ERR_LRC_CRC:		
			        LOGE("Failed:check lrc,  ErrCode-%08X.", SE_ERR_LRC_CRC);
					break;
				case SE_ERR_TIMEOUT:
			        LOGE("Failed:receive frame overtime,  ErrCode-%08X.", SE_ERR_TIMEOUT);
					break;
				}
			}
			break;
		}
		
		else if(ret_code != SE_SUCCESS)
		{
			LOGE("Failed:protocol communication,  ErrCode-%08X.", ret_code);
			break;
		}
		
		queue_out->q_buf_len = rec_len;
		queue_out->rear_node =  queue_out->front_node + rec_len;
		util_queue_front_pop(FRAME_HEAD_LEN, queue_out); //移除起始域数据
		util_queue_rear_pop(EDC_LEN, queue_out);//移除终止域
		*rlen = util_queue_size(queue_out);
		LOGI("Communication Success!");
		break;
	}while(1);

	
	return ret_code;	
}



/**
* @brief 连接从设备时，调用此函数可以获取从设备的ATR
* -# 通过port层设备注册的函数列表指针，调用port层spi接口的 control函数对SE进行复位操椎
* -# 发送复位命令序列
* -# 发送RATR命令序列，获取从设备ATR值
* @param [in] periph  设备句柄
* @param [out] rbuf  待接收ATR的起始地址
* @param [out] rlen 待接收ATR的长度
* @return 函数操作状态码	
* @note no
* @see 函数  port_spi_periph_control proto_spi_reset_frame proto_spi_ratr_frame
*/
se_error_t proto_spi_reset(peripheral *periph , uint8_t *rbuf, uint32_t *rlen) 
{
	se_error_t ret_code = SE_SUCCESS;
	util_timer_t timer = {0};
	uint32_t rec_len = 0;
	uint8_t dev_id = 0;
	HAL_SPI_PERIPHERAL_STRUCT_POINTER p_spi_periph = (HAL_SPI_PERIPHERAL_STRUCT_POINTER)periph;
	

	if(p_spi_periph == NULL)
	{
		return SE_ERR_HANDLE_INVALID;
	}

	if (((rbuf == NULL) && (rlen != NULL))||((rbuf != NULL) && (rlen == NULL)))
	{
		return  SE_ERR_PARAM_INVALID;
	}

	//设置等待的超时时间
	timer.interval = SPI_COMM_MUTEX_WAIT_TIME; 
	p_spi_periph->timer_start(&timer);

	do
	{
		if(p_spi_periph->timer_differ(&timer) != SE_SUCCESS)
		{
			ret_code = SE_ERR_TIMEOUT;	
			LOGE("Failed:open periph mutex,  ErrCode-%08X.", ret_code);
			break;
		}
		

		//控制SE 的RST引脚进行复位操作
		ret_code = p_spi_periph->control(p_spi_periph, PROTO_SPI_CTRL_RST, NULL, NULL);
		if(ret_code != SE_SUCCESS)
		{
			LOGE("Failed:protocol rst io control,  ErrCode-%08X.", ret_code);
			break;
		}

		dev_id = p_spi_periph->periph.id;
		
		if(g_bSPIHedRstSeMode == HED20_SPI_RESET_SE_RESET_REQ)
		{
			//发送reset命令序列,获取返回值
			rec_len = PROTO_SPI_RERESET_MAX_LEN ;
			ret_code = proto_spi_reset_frame(periph, &g_spi_param[dev_id], reset_buf, &rec_len);
			if(ret_code == SE_ERR_BUSY)
			{
				continue;
			}
			else if(ret_code != SE_SUCCESS)
			{
				LOGE("Failed:protocol reset  frame,  ErrCode-%08X.", ret_code);
				break;
			}
		}

		if((rbuf != NULL)&&(rlen != NULL))
		{
			p_spi_periph->delay(SPI_SEND_BGT_TIME);
			//发送RATR 命令序列，获取ATR值
			rec_len = PROTO_SPI_ATR_MAX_LEN;
			ret_code = proto_spi_ratr_frame(periph, &g_spi_param[dev_id], atr_buf, &rec_len);
			if(ret_code == SE_ERR_BUSY)
			{
				continue;
			}
			else if(ret_code != SE_SUCCESS)
			{
				LOGE("Failed:protocol ratr  frame,  ErrCode-%08X.", ret_code);
				break;
			}

			memmove(rbuf,atr_buf+DATA_OFFSET,rec_len);

			//rbuf[2] = 0x11;
			*rlen = rec_len;
		}
		LOGI("Open Periph Success!");
		break;
	}while(1);

	

	return ret_code;
}


/**
* @brief 通过SPI接口向从设备发送RST控制引脚进行复位操作的控制命令或其它控制命令
* @param [in] periph  设备句柄
* @param [in] ctrlcode 命令控制路
* @param [in] sbuf 输入数据的起始地址
* @param [in] slen 输入数据的长度
* @param [out] rbuf 输出数据的起始地址
* @param [out] rlen 输出数据长度的起始地址
* @return 函数操作状态码	
* @see 函数  port_spi_periph_control 
*/
se_error_t proto_spi_control(peripheral *periph , uint32_t ctrlcode, uint8_t *sbuf, uint32_t slen, uint8_t  *rbuf, uint32_t *rlen)
{
	se_error_t ret_code = SE_SUCCESS;
	util_timer_t timer = {0};
	HAL_SPI_PERIPHERAL_STRUCT_POINTER p_spi_periph = (HAL_SPI_PERIPHERAL_STRUCT_POINTER)periph;

	if(p_spi_periph == NULL)
	{
		return SE_ERR_HANDLE_INVALID;
	}

	if (ctrlcode == 0U)
	{
		return  SE_ERR_PARAM_INVALID;
	}

	//设置等待的超时时间
	timer.interval = SPI_COMM_MUTEX_WAIT_TIME; 
	p_spi_periph->timer_start(&timer);

	do
	{
		if(p_spi_periph->timer_differ(&timer) != SE_SUCCESS)
		{
			ret_code = SE_ERR_TIMEOUT;	
			LOGE("Failed:control periph mutex,  ErrCode-%08X.", ret_code);
			break;
		}	
		ret_code = p_spi_periph->control(p_spi_periph, ctrlcode, sbuf,&slen);
		if(ret_code != SE_SUCCESS)
		{
			LOGE("Failed:spi potocol,  ErrCode-%08X.", ret_code);
		}
		else
		{
			LOGI("Success!");
		}
		break;
	}while(1);

	
	return ret_code;
}


/**
* @brief 实现微妙级延时
* @param [in] periph  设备句柄
* @param [in] us  微秒
* @return 函数操作状态码	
* @note no
* @see 函数  port_spi_periph_delay  
*/
se_error_t proto_spi_delay(peripheral *periph , uint32_t us) 
{
	se_error_t ret_code = SE_SUCCESS;
	HAL_SPI_PERIPHERAL_STRUCT_POINTER p_spi_periph = (HAL_SPI_PERIPHERAL_STRUCT_POINTER)periph;
	
	if(p_spi_periph == NULL)
	{
		return SE_ERR_HANDLE_INVALID;
	}
	//延时指定微秒时间
	p_spi_periph->delay(us);
	return ret_code;
}


/**
  * @}
  */

/**
  * @}
  */



/**
  * @}
  */

/**
  * @}
  */



