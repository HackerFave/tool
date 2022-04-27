/******************************************************************************
 Copyright(C),CEC Huada Electronic Design Co.,Ltd.
 File name: 		main.c
 Author:			  liuch 
 Version:			  V1.0	
 Date:			    2020-04-01	
 Description:	  Main program body
 History:		

******************************************************************************/


/***************************************************************************
* Include Header Files
***************************************************************************/


#include "main.h"
#include "se.h"
#include "key.h"
#include "pin.h"
#include "port_config.h"


int main(void)
{
	uint8_t com_outbuf[300] = {0};
	uint32_t outlen =0;	
	se_error_t ret = 0;
    uint8_t trankey_val[16]= {0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4A,0x4B,0x4C,0x4D,0x4E,0x4F};
	sym_key_t trankey={0};
	uint8_t pin_buf[16] = {0x12,0x34,0x56,0x78,0x9A,0xBC,0xDE,0xF0,0x12,0x34,0x56,0x78,0x9A,0xBC,0xDE,0xF0};
    pin_t pin={0};

	//MCU初始化
	port_mcu_init();
	
	LOGI("test begin\n"); 
	
	/*
	
	SE正常通信之前需要执行： 
	1、注册 api_register，传入接口和SE号
	2、选择 api_select，传入接口和SE号
	3、连接 api_connect，需要保证1、2步正常执行
	4、设置传输密钥
	SE连接之后即可进行正常的应用演示
	
	SE应用演示完成之后可断开SE连接
	
	*/
	
	
	 /****************************************通信comm功能演示*****************************************/	
	ret = comm_test();
	if(ret!=SE_SUCCESS)
	{		
		 LOGE("failed to comm_test\n");
		 return ret;
	}	
	
	
  /****************************************SE注册、选择、连接和设置传输密钥*****************************************/	


	//---- 1. 注册设备----
	ret = api_register(PERIPHERAL_SPI, SPI_PERIPHERAL_SE0);
	if(ret!=SE_SUCCESS)
	{		
		 LOGE("failed to spi api_register\n");
		 return ret;
	}
	
	//---- 2. 选择设备 ----
	ret = api_select(PERIPHERAL_SPI, SPI_PERIPHERAL_SE0);
	if(ret!=SE_SUCCESS)
	{		
		 LOGE("failed to spi api_select\n");
		 return ret;
	}


	//---- 3. 连接，获取atr ----
	ret = api_connect(com_outbuf, &outlen);
	if(ret!=SE_SUCCESS)
     { 	  
	   	 ret = api_reset(com_outbuf, &outlen);     			 
		if(ret!=SE_SUCCESS)
		{		
			 LOGE("failed to spi api_reset\n");
			 return ret;
		}
     }
	
	 //---- 4. 设置传输密钥 ----
	 memcpy(trankey.val,trankey_val,16);
	 trankey.val_len = 16;
	 trankey.id = 0x02;
     pin.owner = ADMIN_PIN;
	 pin.pin_len = 0x10;
	 memcpy(pin.pin_value, pin_buf,pin.pin_len);
     ret = api_verify_pin(&pin);//验证管理员pin
	 if(ret!=SE_SUCCESS)
     { 	  
	   	  LOGE("failed to api_connect\n");
	   	  return ret;
     }
	 
	 ret =  api_set_transkey (&trankey);//设置传输密钥
	 if(ret!=SE_SUCCESS)
	 {		 
		  LOGE("failed to api_connect\n");
		  return ret;
	 }
     
	 /****************************************安全认证auth功能演示*****************************************/		
	
	ret = auth_test();
	if(ret!=SE_SUCCESS)
	{		
		 LOGE("failed to ctrl_test\n");
		 return ret;
	}
	
	
	/****************************************获取SE信息info功能演示*****************************************/		
	ret = info_test();
	if(ret!=SE_SUCCESS)
	{		
		 LOGE("failed to auth_test\n");
		 return ret;
	}
	
	/****************************************密钥key功能演示*****************************************/		
	
	ret = key_test();
	if(ret!=SE_SUCCESS)
	{		
		 LOGE("failed to key_test\n");
		 return ret;
	}
	
	/****************************************控制crypto功能演示*****************************************/	
  ret = crypto_test();
	if(ret!=SE_SUCCESS)
	{		
		 LOGE("failed to crypto_test\n");
		 return ret;
	}		

	
	
  /****************************************控制ctrl功能演示*****************************************/	
	ret = ctrl_test();
	if(ret!=SE_SUCCESS)
	{		
		 LOGE("failed to auth_test\n");
		 return ret;
	}	
	
	
  /****************************************文件fs功能演示*****************************************/	

	ret = fs_test();
	if(ret!=SE_SUCCESS)
	{		
		 LOGE("failed to fs_test\n");
		 return ret;
	}

  /****************************************pin功能演示*****************************************/	

	ret = pin_test();
	if(ret!=SE_SUCCESS)
	{		
		 LOGE("failed to pin_test\n");
		 return ret;
	}

	



  /****************************************控制update功能演示*****************************************/	
	//ret = update_test ();
	if(ret!=SE_SUCCESS)
	{		
		LOGE("failed to update_test\n");
		 return ret;
	}


  /********************************************SE断开连接**************************************************/			
  ret = api_disconnect ();
	if(ret!=SE_SUCCESS)
	{		
		 LOGE("failed to api_disconnect\n");
		 return ret;
	}
	
	LOGI("test end\n"); 

	while (1)
	{
		HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_7);
		HAL_Delay(1000);	
	}	
}






/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
