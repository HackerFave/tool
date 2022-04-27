/**@file  proto_spi.c
* @brief  SPI �ӿ���·ͨ��Э�����������( ����SPI HED ͨ��Э��淶ʵ��)
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
uint8_t   g_bSPIHedOpenSeMode = HED20_SPI_OPEN_SE_RESET_REQ;               //������SE ʱ�Ĳ���ģʽ
uint8_t   g_bSPIHedRstSeMode = HED20_SPI_RESET_SE_RESET_REQ;                  //��λSE ʱ�Ĳ���ģʽ

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
* @brief ͨ��SPI�ӿڷ�������
* -# ͨ��port���豸ע��ĺ����б�ָ�룬����port��spi�ӿڵ�оƬѡ���� chip_select
* -# ͨ��port���豸ע��ĺ����б�ָ�룬����port��spi�ӿڵķ��ͺ���transmit
* @param [in] periph  �豸���
* @param [in] inbuf  ���������ݵ���ʼ��ַ
* @param [in] inbuf_len ���������ݵĳ���
* @return ��������״̬��	
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
* @brief ͨ��SPI�ӿڽ�������
* -# ͨ��port���豸ע��ĺ����б�ָ�룬����port��spi�ӿڵ�оƬѡ���� chip_select
* -# ͨ��port���豸ע��ĺ����б�ָ�룬����port��spi�ӿڵĽ��պ���receive
* @param [in] periph  �豸���
* @param [out] outbuf  �������ݵ���ʼ��ַ
* @param [out] outbuf_len �������ݵĳ���
* @return ��������״̬��	
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
  Description:  ����ָ������ָ�����ݵ�CRC���
  Input:	
            CRCType��CRC������sing
            Length��Ҫ��������ݳ���
            Data��Ҫ��������ݵ���ʼ��ַ
  Return:	crc������	
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
* @brief ����HED SPIͨ��Э���֡��ʽ������һ֡����
* @param [in] periph  �豸���
* @param [in] param  ͨ�Ų�����Ϣ
* @param [in] inbuf  ���������ݵ���ʼ��ַ
* @param [in] inbuf_len ���������ݵĳ���
* @return ��������״̬��	
* @note Э��ʱ�����WPT~T3 �����SE оƬҪ��
* @see ����  proto_spi_transmit
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
* @brief ����HED SPIͨ��Э���֡��ʽ������֡ͷ(��ʼ��PIB LEN)
* @param [in] periph  �豸���
* @param [out] outbuf  ������֡ͷ����ʼ��ַ
* @param [out] outbuf_len ������֡ͷ�ĳ���
* @return ��������״̬��	
* @note Э��ʱ�����T0~T3 �����SE оƬҪ��
* @see ����  proto_spi_receive
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
* @brief ����HED SPIͨ��Э���֡��ʽ������֡����(��Ϣ��/��ֹ��)
* @param [in] periph  �豸���
* @param [in] param  ͨ�Ų�����Ϣ
* @param [out] outbuf  ������֡���ݵ���ʼ��ַ
* @param [out] outbuf_len ������֡���ݵĳ���
* @return ��������״̬��	
* @note Э��ʱ�����T5 �����SE оƬҪ��
* @see ����  proto_spi_receive
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
* @brief ����HED SPIͨ��Э���֡��ʽ������֡����֡���ݣ��ȴ�����ʱ��֡�ȴ�ʱ�������������ݳ�ʱ�ļ�ʱ����
* @param [in] periph  �豸���
* @param [in] param  ͨ�Ų�����Ϣ
* @param [out] outbuf  �����ռ���֡����ʼ��ַ
* @param [out] outbuf_len �����ռ���֡�ĳ���
* @return ��������״̬��	
* @note Э��ʱ�����T4 �����SE оƬҪ��
* @see ����  proto_spi_receive_frame_head
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


	//���ý�������ʱ֡�ȴ��ĳ�ʱʱ��
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
	    //�ж��Ƿ�Ϊ����֡ 
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
		
		//���edc
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
			
			p_spi_periph->delay(SPI_SEND_BGT_TIME);	 //��ʱBGT
			retCode = proto_spi_send_frame(periph,param,(uint8_t *)cNAK_HED_SE,PROCESS_FRAME_LEN); //����豸����NAK֡
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
* @brief ���͸�λ����֡030004D3xxxxxx ������Ӧ��֡030004D3xxxxxxx
* @param [in] periph  �豸���
* @param [in] param  ͨ�Ų�����Ϣ
* @return ��������״̬��	
* @see ����  proto_spi_send_frame   proto_spi_receive_active_frame
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
		tmp_buf[ACTIVE_REQ_FRAME_LEN-2]  = (uint8_t)spi_edc&0xff;//���EDC_HIGH
		tmp_buf[ACTIVE_REQ_FRAME_LEN-1]  =(uint8_t)(spi_edc>>8)&0xff;//���EDC_LOW
		
	
	   //���͸�λ֡����
		ret_code = proto_spi_send_frame(periph, param, tmp_buf,ACTIVE_REQ_FRAME_LEN);
		if(ret_code!= SE_SUCCESS)
		{
			break;
		}
		bufsize = *rlen;//����buf��С		

		//���ո�λ���󷵻�֡
		ret_code = proto_spi_receive_active_frame(periph, param, rbuf, &bufsize);
		
		if(ret_code!= SE_SUCCESS)
		{
            if(ret_code == SE_ERR_TIMEOUT)//��ʱ�ط�һ��
            {   rec_time_out_count++;
			    if(rec_time_out_count < 2)
			    {
 			    	continue;
				}
			}
			break;
		}
        //����Ƿ�ΪNAK֡
        if(rbuf[DATA_OFFSET] == PIB_PROCESS_FRAME_NAK_CRC_INFO)
		{
			//ret_code = SE_ERR_RESET;
			rec_nak_count++;
			if(rec_nak_count > PROTO_SPI_RETRY_NUM )//�ط�3��reset֡
			{
              LOGE("Failed:communication cannot recover,  ErrCode-%08X.", SE_ERR_COMM);
			  ret_code = SE_ERR_COMM;
			  return SE_ERR_COMM ;
			}
			continue;
		}
		// ���Ӽ���Ƿ�ΪRESET��Ӧ
		if(rbuf[DATA_OFFSET] != PIB_ACTIVE_FRAME_RESET)
		{
			//ret_code = SE_ERR_RESET;
			ret_code = SE_ERR_DATA;
			break;
		}
		
		
		*rlen = bufsize;

		if(bufsize != ACTIVE_REQ_FRAME_LEN) //reset֡����
		{
			ret_code = SE_ERR_LEN;
			break;
		}
		break;

	}while(1);

	return ret_code;
}


/**
* @brief ����RATR�������У���ȡATR������030004E2XXXXXX������ATR��Ϣ��
* @param [in] periph  �豸���
* @param [in] param  ͨ�Ų�����Ϣ
* @param [out] rbuf  ������ATR����ʼ��ַ
* @param [out] rlen ������ATR�ĳ���
* @return ��������״̬��	
* @see ����  proto_spi_send_frame   proto_spi_receive_active_frame
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
		tmp_buf[ACTIVE_REQ_FRAME_LEN-2]  = spi_edc&0xff;//���EDC_HIGH
		tmp_buf[ACTIVE_REQ_FRAME_LEN-1]  = (spi_edc>>8)&0xff;//���EDC_LOW

		ret_code = proto_spi_send_frame(periph, param, tmp_buf,ACTIVE_REQ_FRAME_LEN);
		if(ret_code!= SE_SUCCESS)
		{
			break;
		}

		bufsize = *rlen;//����buf��С
		  
		ret_code = proto_spi_receive_active_frame(periph, param, rbuf, &bufsize);
         if(ret_code != SE_SUCCESS)
         {
			//��ʱ�ط�һ��
			 if(ret_code == SE_ERR_TIMEOUT)
	        {   rec_time_out_count++;
			    if(rec_time_out_count < 2)
			    {
				    	continue;
				}
			} 

			break;
		 }
			 
       //����Ƿ�ΪNAK֡
    	if(rbuf[DATA_OFFSET] == PIB_PROCESS_FRAME_NAK_CRC_INFO)
		{
			//ret_code = SE_ERR_RESET;
			rec_nak_count++;
			if(rec_nak_count > PROTO_SPI_RETRY_NUM )//�ط�ratr֡3��
			{
	          ret_code = SE_ERR_LRC_CRC;
			  break;
			}
			else
			{
			  continue;
			}
			
		}
		                
		// ���Ӽ���Ƿ�Ϊratr��Ӧ
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
* @brief ����HED SPIͨ��Э���֡��ʽ��SE�����շ����ݽ���
* @param [in] periph  �豸���
* @param [in] param  ͨ�Ų�����Ϣ
* @param [in] inbuf  ��������ָ��
* @param [in] inbuf_len �������ݳ���
* @param [out] outbuf  ������ݴ洢��ַ
* @param [out] outbuf_len ������ݳ���
* return ��������״̬��	
* @note �����ڼ�����ݽ���֡���ͼ�EDCУ��ȣ�������ʱ֡��NAK֡������ʱ֡�ط�����
* @see ����  proto_spi_send_frame   proto_spi_receive_frame_head  proto_spi_receive_frame_data
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

	//1.����֡����
	param->type = SPI_INFO;
	ret_code = proto_spi_send_frame(periph, param, inbuf, inbuf_len);
	if(ret_code != SE_SUCCESS)
		return ret_code;
	
   //2. ���ý�������ʱ֡�ȴ��ĳ�ʱʱ��
	timer.interval = SPI_RECEVIE_FRAME_WAIT_TIME; 
	p_spi_periph->timer_start(&timer);	
		
	do
	{	
        //3.����֡���ݵ���ʼ��֡�ȴ�ʱ����δ���յ�����ʱ
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

		//����һ�γ�ʱ��ʱ����Ҫ���г�ʱ�ط���Ϣ֡��
		if(re_tran_count==1)
		{  
			p_spi_periph->delay(SPI_SEND_BGT_TIME); 	//��ʱBGT
			ret_code = proto_spi_send_frame(periph, param, inbuf, inbuf_len);
			 //���¼�ʱ
		    timer.interval = SPI_RECEVIE_FRAME_WAIT_TIME; 
			p_spi_periph->timer_start(&timer);	
			continue;
		}
		//����Ȼ��ʱ���򷵻س�ʱ����״̬��
		else if(re_tran_count==2)
		{
            LOGE("Failed:receive frame overtime,  ErrCode-%08X.", SE_ERR_TIMEOUT);
			return SE_ERR_TIMEOUT;  	   
		}
		
		
		if(rec_len > (FRAME_DATA_LEN_MAX+EDC_LEN))
		{
			return SE_ERR_LEN;
		}

		//4. ����֡���ݵ���Ϣ�����ֹ��
		ret_code = proto_spi_receive_frame_data(periph, param, outbuf+FRAME_HEAD_LEN, rec_len);
		if(ret_code!= SE_SUCCESS)
		{
			return ret_code;
		}

		//5.У�����֡���ݵ�EDCֵ
		edc_value = 0;
		rec_len = rec_len+FRAME_HEAD_LEN;
		edc_value = proto_spi_crc16(CRC_B, rec_len-EDC_LEN, outbuf);
		if((((edc_value>>8) & 0xff)==outbuf[rec_len -EDC_LEN+1])&&((edc_value & 0xff)==outbuf[rec_len -EDC_LEN]))	//EDC ��ȷ
		{
			send_nak_count = 0;

			//6.������֡���ͣ�������֡���ͽ��д���
			if(((outbuf[PIB_OFFSET] == PIB_PROCESS_FRAME)&&(outbuf[DATA_OFFSET] == PIB_PROCESS_FRAME_NAK_CRC_INFO))||((outbuf[PIB_OFFSET] == PIB_PROCESS_FRAME)&&(outbuf[DATA_OFFSET] == PIB_PROCESS_FRAME_NAK_OTHER_INFO)))
			{
				//���յ���֡����ΪNAK֡�����ط�֡����;��������NAK�󣬷��ش����롣
				if(param->type == SPI_INFO)
				{
					rec_nak_count++;
					if(rec_nak_count > PROTO_SPI_RETRY_NUM)
					{
					return SE_ERR_LRC_CRC;	
					}
					p_spi_periph->delay(SPI_SEND_BGT_TIME);	 //��ʱBGT
					ret_code = proto_spi_send_frame(periph, param, inbuf, inbuf_len);  //�ط���Ϣ֡
				}
				else if(param->type == SPI_WTX)
				{
					p_spi_periph->delay(SPI_SEND_BGT_TIME);	 //��ʱBGT
					ret_code = proto_spi_send_frame(periph, param,(uint8_t *)cWTX_HED_SE,PROCESS_FRAME_LEN);	 //�ط�WTX֡
				}

				if(ret_code != SE_SUCCESS)	//�ط�����wtx ʧ��
				{
					return ret_code;
				}

			}

			else if((outbuf[PIB_OFFSET] == PIB_PROCESS_FRAME)&&(outbuf[DATA_OFFSET] == PIB_PROCESS_FRAME_WTX_INFO))
			{
				//���յ���֡����Ϊ��ʱ���룬������ʱ֡������Ӧ
				param->type = SPI_WTX;
				ret_code = proto_spi_send_frame(periph, param,(uint8_t *)cWTX_HED_SE,PROCESS_FRAME_LEN);
				if(ret_code!= SE_SUCCESS)
				{
					return ret_code;
				}
				//���¼Ƴ�ʱ
				timer.interval = SPI_RECEVIE_FRAME_WAIT_TIME; 
				p_spi_periph->timer_start(&timer);
			}

			else if(outbuf[PIB_OFFSET]	== PIB_INFORMATION_FRAME)
			{
				//���յ���֡����Ϊ��Ϣ֡���˳�֡�����շ���������
				*outbuf_len = rec_len;
				return SE_SUCCESS;
			}
			else
			{
				return SE_ERR_PARAM_INVALID;
			}
		}
		else   //EDC ����
		{
			send_nak_count++; 
			if(send_nak_count > PROTO_SPI_RETRY_NUM)
			{
			   return SE_ERR_LRC_CRC;	
			}
			
			p_spi_periph->delay(SPI_SEND_BGT_TIME);	 //��ʱBGT
			ret_code = proto_spi_send_frame(periph,param,(uint8_t *)cNAK_HED_SE,PROCESS_FRAME_LEN); //����豸����NAK֡
			if(ret_code!= SE_SUCCESS)
			{
				return ret_code;
			}
		}
	}while(1);

}


/**
* @brief ����HED SPIЭ���ʽ����˫�����������֡ͷ��֡β����
* -# ��˫�����ͷ����� PIB, LEN
* -# ��˫�����β�����LRC
* @param [in] periph  �豸���
* @param [in] inbuf ����˫����е���ʼ��ַ
* @param [in] inbuf_len ����˫����е��������ݳ���
* @return ��������״̬��	
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
		

		//��˫����зֱ������ʼ��(PIB, LEN)
		frame_len = (uint16_t)(inbuf_len +EDC_LEN);
		frame_head[0] = PIB_INFORMATION_FRAME;
		frame_head[1] = (frame_len>>8)&0xff;	//LEN_High
		frame_head[2] = frame_len&0xff; 		//LEN_Low
		util_queue_front_push(frame_head, FRAME_HEAD_LEN,queue_in);
		
		
		//����CRC16������˫�������� EDC 
		spi_edc = proto_spi_crc16(CRC_B,util_queue_size(queue_in), &queue_in->q_buf[queue_in->front_node]);	//4 //���� CRC
		tmp_buf[0]  = spi_edc&0xff;//���EDC_HIGH
		tmp_buf[1]  = (spi_edc>>8)&0xff;//���EDC_LOW
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
* @brief ����SPI ͨ��Э�������ʼ��
* -# FSM/FSS�Ȳ�����ʼ��
* -# ͨ��port���豸ע��ĺ����б�ָ�룬����port��spi�ӿڵĳ�ʼ������init
* @param [in] periph  �豸���
* @return ��������״̬��	
* @note no
* @see ����  port_spi_periph_init
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

	//�������ȴ��ĳ�ʱʱ��
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
* @brief ����SPI ͨ��Э�������ֹ��
* -# FSM/FSS�Ȳ����ָ�Ĭ��ֵ
* -# ͨ��port���豸ע��ĺ����б�ָ�룬����port��spi�ӿڵ� ��ֹ������deinit
* @param [in] periph  �豸���
* @return ��������״̬��	
* @note no
* @see ����    port_spi_periph_deinit
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

	//���õȴ��ĳ�ʱʱ��
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
* @brief ���Ӵ��豸ʱ�����ô˺������Ի�ȡ���豸��ATR
* -# ͨ��port���豸ע��ĺ����б�ָ�룬����port��spi�ӿڵ� control������SE���и�λ��׵
* -# ���͸�λ��������
* -# ����RATR�������У���ȡ���豸ATRֵ
* @param [in] periph  �豸���
* @param [out] rbuf  ������ATR����ʼ��ַ
* @param [out] rlen ������ATR�ĳ���
* @return ��������״̬��	
* @note no
* @see ����  port_spi_periph_control   proto_spi_reset_frame proto_spi_ratr_frame
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

	//���õȴ��ĳ�ʱʱ��
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
		
		
			//����SE ��RST���Ž��и�λ����
			//ret_code = p_spi_periph->control(p_spi_periph, PROTO_SPI_CTRL_RST, NULL, NULL);
			//if(ret_code != SE_SUCCESS)
		//	{
			//	LOGE("Failed:protocol rst io control,  ErrCode-%08X.", ret_code);
				//break;
			//}
		
        p_spi_periph->delay(SPI_PROTO_SE_RST_DELAY);//�ӳ�10msȷ��SE����
		dev_id = p_spi_periph->periph.id;
		
		if(g_bSPIHedOpenSeMode == HED20_SPI_OPEN_SE_RESET_REQ)
		{
			//����reset��������,��ȡ����ֵ
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
			//���ݻ���SPIЭ���޸�PFSS��PFSM,���߲�һ��ʱ��ȡ������Сֵ
		
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
			//����RATR �������У���ȡATRֵ
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

			//���ݻ���SPIЭ���޸�HBSS��HBSM,���߲�һ��ʱ��ȡ������Сֵ
			
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
* @brief Ӧ�ùر��豸ʱ�����ô˺���
* @param [in] periph  �豸���
* @return ��������״̬��	
* @note �޴���ʵ��
*/
se_error_t proto_spi_close(peripheral *periph) 
{
	se_error_t ret_code = SE_SUCCESS;

	return ret_code;
}


/**
* @brief ͨ��SPI�ӿ�����豸�������������Ӧʱ�����ô˺���
* -# ��˫����зֱ������ʼ��(PIB, LEN)
* -# ��˫����зֱ������ֹ��(EDC)
* -# ����proto_spi_handle���������������������Ӧ
* @param [in] periph  �豸���
* @param [in] sbuf ����˫����е���ʼ��ַ
* @param [in] slen ����˫����е��������ݳ���
* @param [out] rbuf  ���˫����е���ʼ��ַ
* @param [out] rlen ���˫����е�������ݳ��ȵ���ʼ��ַ
* @return ��������״̬��	
* @see ����  porto_spi_queue_in   proto_spi_handle
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

	//׼���շ�������
	ret_code =proto_spi_queue_in(periph, sbuf, slen);
	if(ret_code != SE_SUCCESS)
	{
		return ret_code;
	}

	p_input = &queue_in->q_buf[queue_in->front_node];
	p_output = &queue_out->q_buf[queue_out->front_node];
	dev_id = p_spi_periph->periph.id;

	//���õȴ��ĳ�ʱʱ��
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
		

		//����SPI���ݵ��շ�
		ret_code = proto_spi_handle(periph, &g_spi_param[dev_id], p_input,util_queue_size(queue_in),p_output,(uint32_t*)&rec_len);
		if(ret_code == SE_ERR_BUSY)
		{
			continue;
		}
		
		else if(ret_code == SE_ERR_LRC_CRC||ret_code == SE_ERR_TIMEOUT)
		{

			//����reset��������
		    p_spi_periph->delay(SPI_SEND_BGT_TIME); 	//��ʱBGT
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
		util_queue_front_pop(FRAME_HEAD_LEN, queue_out); //�Ƴ���ʼ������
		util_queue_rear_pop(EDC_LEN, queue_out);//�Ƴ���ֹ��
		*rlen = util_queue_size(queue_out);
		LOGI("Communication Success!");
		break;
	}while(1);

	
	return ret_code;	
}



/**
* @brief ���Ӵ��豸ʱ�����ô˺������Ի�ȡ���豸��ATR
* -# ͨ��port���豸ע��ĺ����б�ָ�룬����port��spi�ӿڵ� control������SE���и�λ��׵
* -# ���͸�λ��������
* -# ����RATR�������У���ȡ���豸ATRֵ
* @param [in] periph  �豸���
* @param [out] rbuf  ������ATR����ʼ��ַ
* @param [out] rlen ������ATR�ĳ���
* @return ��������״̬��	
* @note no
* @see ����  port_spi_periph_control proto_spi_reset_frame proto_spi_ratr_frame
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

	//���õȴ��ĳ�ʱʱ��
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
		

		//����SE ��RST���Ž��и�λ����
		ret_code = p_spi_periph->control(p_spi_periph, PROTO_SPI_CTRL_RST, NULL, NULL);
		if(ret_code != SE_SUCCESS)
		{
			LOGE("Failed:protocol rst io control,  ErrCode-%08X.", ret_code);
			break;
		}

		dev_id = p_spi_periph->periph.id;
		
		if(g_bSPIHedRstSeMode == HED20_SPI_RESET_SE_RESET_REQ)
		{
			//����reset��������,��ȡ����ֵ
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
			//����RATR �������У���ȡATRֵ
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
* @brief ͨ��SPI�ӿ�����豸����RST�������Ž��и�λ�����Ŀ��������������������
* @param [in] periph  �豸���
* @param [in] ctrlcode �������·
* @param [in] sbuf �������ݵ���ʼ��ַ
* @param [in] slen �������ݵĳ���
* @param [out] rbuf ������ݵ���ʼ��ַ
* @param [out] rlen ������ݳ��ȵ���ʼ��ַ
* @return ��������״̬��	
* @see ����  port_spi_periph_control 
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

	//���õȴ��ĳ�ʱʱ��
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
* @brief ʵ��΢���ʱ
* @param [in] periph  �豸���
* @param [in] us  ΢��
* @return ��������״̬��	
* @note no
* @see ����  port_spi_periph_delay  
*/
se_error_t proto_spi_delay(peripheral *periph , uint32_t us) 
{
	se_error_t ret_code = SE_SUCCESS;
	HAL_SPI_PERIPHERAL_STRUCT_POINTER p_spi_periph = (HAL_SPI_PERIPHERAL_STRUCT_POINTER)periph;
	
	if(p_spi_periph == NULL)
	{
		return SE_ERR_HANDLE_INVALID;
	}
	//��ʱָ��΢��ʱ��
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



