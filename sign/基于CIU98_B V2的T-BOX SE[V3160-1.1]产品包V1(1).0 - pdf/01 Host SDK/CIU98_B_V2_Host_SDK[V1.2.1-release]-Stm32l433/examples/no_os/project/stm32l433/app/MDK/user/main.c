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

	//MCU��ʼ��
	port_mcu_init();
	
	LOGI("test begin\n"); 
	
	/*
	
	SE����ͨ��֮ǰ��Ҫִ�У� 
	1��ע�� api_register������ӿں�SE��
	2��ѡ�� api_select������ӿں�SE��
	3������ api_connect����Ҫ��֤1��2������ִ��
	4�����ô�����Կ
	SE����֮�󼴿ɽ���������Ӧ����ʾ
	
	SEӦ����ʾ���֮��ɶϿ�SE����
	
	*/
	
	
	 /****************************************ͨ��comm������ʾ*****************************************/	
	ret = comm_test();
	if(ret!=SE_SUCCESS)
	{		
		 LOGE("failed to comm_test\n");
		 return ret;
	}	
	
	
  /****************************************SEע�ᡢѡ�����Ӻ����ô�����Կ*****************************************/	


	//---- 1. ע���豸----
	ret = api_register(PERIPHERAL_SPI, SPI_PERIPHERAL_SE0);
	if(ret!=SE_SUCCESS)
	{		
		 LOGE("failed to spi api_register\n");
		 return ret;
	}
	
	//---- 2. ѡ���豸 ----
	ret = api_select(PERIPHERAL_SPI, SPI_PERIPHERAL_SE0);
	if(ret!=SE_SUCCESS)
	{		
		 LOGE("failed to spi api_select\n");
		 return ret;
	}


	//---- 3. ���ӣ���ȡatr ----
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
	
	 //---- 4. ���ô�����Կ ----
	 memcpy(trankey.val,trankey_val,16);
	 trankey.val_len = 16;
	 trankey.id = 0x02;
     pin.owner = ADMIN_PIN;
	 pin.pin_len = 0x10;
	 memcpy(pin.pin_value, pin_buf,pin.pin_len);
     ret = api_verify_pin(&pin);//��֤����Աpin
	 if(ret!=SE_SUCCESS)
     { 	  
	   	  LOGE("failed to api_connect\n");
	   	  return ret;
     }
	 
	 ret =  api_set_transkey (&trankey);//���ô�����Կ
	 if(ret!=SE_SUCCESS)
	 {		 
		  LOGE("failed to api_connect\n");
		  return ret;
	 }
     
	 /****************************************��ȫ��֤auth������ʾ*****************************************/		
	
	ret = auth_test();
	if(ret!=SE_SUCCESS)
	{		
		 LOGE("failed to ctrl_test\n");
		 return ret;
	}
	
	
	/****************************************��ȡSE��Ϣinfo������ʾ*****************************************/		
	ret = info_test();
	if(ret!=SE_SUCCESS)
	{		
		 LOGE("failed to auth_test\n");
		 return ret;
	}
	
	/****************************************��Կkey������ʾ*****************************************/		
	
	ret = key_test();
	if(ret!=SE_SUCCESS)
	{		
		 LOGE("failed to key_test\n");
		 return ret;
	}
	
	/****************************************����crypto������ʾ*****************************************/	
  ret = crypto_test();
	if(ret!=SE_SUCCESS)
	{		
		 LOGE("failed to crypto_test\n");
		 return ret;
	}		

	
	
  /****************************************����ctrl������ʾ*****************************************/	
	ret = ctrl_test();
	if(ret!=SE_SUCCESS)
	{		
		 LOGE("failed to auth_test\n");
		 return ret;
	}	
	
	
  /****************************************�ļ�fs������ʾ*****************************************/	

	ret = fs_test();
	if(ret!=SE_SUCCESS)
	{		
		 LOGE("failed to fs_test\n");
		 return ret;
	}

  /****************************************pin������ʾ*****************************************/	

	ret = pin_test();
	if(ret!=SE_SUCCESS)
	{		
		 LOGE("failed to pin_test\n");
		 return ret;
	}

	



  /****************************************����update������ʾ*****************************************/	
	//ret = update_test ();
	if(ret!=SE_SUCCESS)
	{		
		LOGE("failed to update_test\n");
		 return ret;
	}


  /********************************************SE�Ͽ�����**************************************************/			
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
