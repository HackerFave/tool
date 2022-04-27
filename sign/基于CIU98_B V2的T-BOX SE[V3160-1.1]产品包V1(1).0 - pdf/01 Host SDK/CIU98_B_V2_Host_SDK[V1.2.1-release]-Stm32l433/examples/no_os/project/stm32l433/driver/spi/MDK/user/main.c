/******************************************************************************
 Copyright(C),CEC Huada Electronic Design Co.,Ltd.
 File name: 		main.c
 Author:			zhengwd 
 Version:			V1.0	
 Date:			2020-04-01	
 Description:	        Main program body
 History:		

******************************************************************************/


/***************************************************************************
* Include Header Files
***************************************************************************/
#include "main.h"
#include "ial_spi.h"
#include "proto_spi.h"
#include "port_spi.h"
#include "port_config.h"
#include "port_util.h"
#include "comm.h"
#include "util.h"
#include "log.h"
#include "error.h"

#define HED_SPI_COMM_SWITCH


uint8_t se_atr[32]={0};
uint32_t se_atr_len=0;

uint8_t test_inbuf[300]={0};
uint8_t test_outbuf[300]={0};
uint32_t test_inlen=0;
uint32_t test_outlen=0;
uint32_t i=0;
uint32_t test_num=0;

	se_error_t ret_code = SE_SUCCESS;
int main(void)
{


	port_mcu_init();//MCU初始化

	//----delay 时间测试----
	//PORT_DEBUG_IO_HIGH();
	//port_delay(100);
	//PORT_DEBUG_IO_LOW();
	//-----------------------

	port_printf("spi common test begin\n");

	//---- debug---------------------
	do
	{
		//---- 1. 注册SPI 通信外设:  SE0 ----
		ret_code = api_register(PERIPHERAL_SPI, SPI_PERIPHERAL_SE0);
		if(ret_code != SE_SUCCESS)
		{
			port_printf("\nfailed to api_register!");
			break;
		}
		port_printf("\nsuccess to register spi se0!");

		//---- 2. 选择SPI 通信外设:  SE0 ----
		ret_code =api_select(PERIPHERAL_SPI, SPI_PERIPHERAL_SE0);
		if(ret_code != SE_SUCCESS)
		{
			port_printf("\nfailed to api_select!");
			break;
		}
		port_printf("\nsuccess to select spi se0!");

		//---- 3. 连接外设，获取atr ----
		
		ret_code = api_connect(se_atr, &se_atr_len);
		//ret_code = api_connect(NULL, NULL);
		if(ret_code != SE_SUCCESS)
		{
			port_printf("\nfailed to api_connect!");
		  break;
		}
		
		port_printf("\nsuccess to connect spi se0!");
		port_printf_buf("se_atr_buf:", se_atr, se_atr_len);

		//---- 4. 获取8字节随机数 ----
		test_inlen = 0x05;
		memcpy(test_inbuf, "\x00\x84\x00\x00\x08", test_inlen);
		port_printf("\n\nget 8 bytes random test");
		port_printf_buf("apdu_cmd:", test_inbuf, test_inlen);
		
		ret_code = api_transceive(test_inbuf, test_inlen, test_outbuf, &test_outlen);
		if(ret_code != SE_SUCCESS)
		{
			port_printf("\nfailed to get 8 bytes random!");
			port_printf("\nret_code:%8x",ret_code);
			break;
		}

		port_printf_buf("apdu_rsp:", test_outbuf, test_outlen);

		//---- 5. 热复位，获取atr ----
		memset(se_atr, 0x00, sizeof(se_atr));
		se_atr_len = 0;
		
		ret_code = api_reset(se_atr, &se_atr_len);
		//ret_code = api_reset(NULL, NULL);
		if(ret_code != SE_SUCCESS)
		{
			port_printf("\nfailed to api_reset!");
			break;
		}
		port_printf("\n\nsuccess to reset spi se0!");
		port_printf_buf("se_atr_buf:", se_atr, se_atr_len);

		//---- 6. 读取应用特征信息 ----
		memset(test_inbuf, 0x00, sizeof(test_outbuf));
		memset(test_outbuf, 0x00, sizeof(test_outbuf));
		test_inlen = 0x05;
		test_outlen = 0;
		memcpy(test_inbuf, "\x80\xE9\x01\x00\x0F", test_inlen);  
		port_printf("\n\nget se0 app info");
		port_printf_buf("apdu_cmd:", test_inbuf, test_inlen);
		
		ret_code = api_transceive(test_inbuf, test_inlen, test_outbuf, &test_outlen);
		if(ret_code != SE_SUCCESS)
		{
			port_printf("\nfailed to get se0 app info!");
			port_printf("\nret_code:%8x",ret_code);
			break;
		}

		port_printf_buf("apdu_rsp:", test_outbuf, test_outlen);

	


	//循环切换发送命令测试
	#if defined (HED_SPI_COMM_SWITCH) 
		port_printf("\n\ncmd switch test begin");
		test_num = 200;
		test_inlen = 0x05;
		
		for(i=0; i< test_num; i++)
		{
			//7.选择外设SE0 
			ret_code =api_select(PERIPHERAL_SPI, SPI_PERIPHERAL_SE0);
			if(ret_code != SE_SUCCESS)
			{
				port_printf("\nfailed to select spi se0!");
				break;
			}

			//8.获取SE0应用信息
			memcpy(test_inbuf, "\x80\xE9\x01\x00\x0F", test_inlen);  
			ret_code = api_transceive(test_inbuf, test_inlen, test_outbuf, &test_outlen);
			if(ret_code != SE_SUCCESS)
			{
				port_printf("\nfailed to get se0 app info!");
				port_printf("\nret_code:%8x",ret_code);
				break;
			}
			
			//9.获取8字节随机数
			memcpy(test_inbuf, "\x00\x84\x00\x00\x10", test_inlen);
		    ret_code = api_transceive(test_inbuf, test_inlen, test_outbuf, &test_outlen);
			if(ret_code != SE_SUCCESS)
			{
				port_printf("\nfailed to get 8 bytes random!");
				port_printf("\nret_code:%8x",ret_code);
				break;
			}
			
			port_printf("\nsuccess test times:%d", i);	
		}

	#endif
	
	}while(0);

	port_printf("\n\nspi common test end");

	while (1)
	{
		HAL_GPIO_TogglePin(PORT_SPI_LED_CTRL_PORT,PORT_SPI_LED_CTRL_IO_PIN);
		HAL_Delay(1000);	
	}
}





/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
