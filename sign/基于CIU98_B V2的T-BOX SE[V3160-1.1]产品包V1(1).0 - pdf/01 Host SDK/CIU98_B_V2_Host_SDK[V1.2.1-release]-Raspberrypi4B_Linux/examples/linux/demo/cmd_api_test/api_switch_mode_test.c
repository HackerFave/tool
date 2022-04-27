/******************************************************************************
 Copyright(C),CEC Huada Electronic Design Co.,Ltd.
 File name: 		api_switch_mode_test.c
 Author:			  liangww 
 Version:			  V1.0	
 Date:			    2021-06-25	
 Description:	  Main program body
 History:		

******************************************************************************/


/***************************************************************************
* Include Header Files
***************************************************************************/


#include "se.h"
#include "ctrl.h"
#include "pin.h"
#include "key.h"
void helpmenu(void)
{
	printf("\nHelp menu: api_switch_mode_test <option> ...<option>\n");
	printf("option:- \n");
  printf("                STANDBY :  0x00 \n");
  printf("-h           : Print this help \n");
}

int main(int argc, char * argv[])
{
  uint8_t com_outbuf[300] = {0};
	uint32_t outlen =0;	
	se_error_t ret = 0;
  uint32_t i = 0;
  uint32_t option = 0;
  work_mode   mode_type = STANDBY ;
  uint8_t trankey_val[16]= {0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4A,0x4B,0x4C,0x4D,0x4E,0x4F};
	sym_key_t trankey={0};
	uint8_t pin_buf[16] = {0x12,0x34,0x56,0x78,0x9A,0xBC,0xDE,0xF0,0x12,0x34,0x56,0x78,0x9A,0xBC,0xDE,0xF0};
  pin_t pin={0};
  int ch;
	//MCU初始化
	port_mcu_init();
   
   LOGI("test begin\n"); 
   
   if (argc < 2)
    {
      printf("\n");
      printf("--------------------------\n");
			helpmenu();
			exit(0);
		}
    
    
       while ((ch = getopt(argc, argv, "m:")) != -1)
       {
           switch (ch) 
        {
               case 'm':
                       sscanf(optarg,"%x",&option);
                       break;
              case 'h': 
		         	default:  
				      helpmenu();
				      exit(0);
              break;
         }
       }
  /****************************************SE注册、选择、连接和设置传输密钥*****************************************/	


	//---- 1. 注册及选择设备----
	ret = api_register(PERIPHERAL_SPI, SPI_PERIPHERAL_SE0);
	if(ret!=SE_SUCCESS)
	{		
		 LOGE("failed to spi api_register\n");
		 return ret;
	}
	
	//---- 2. 连接，获取atr ----
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
		 LOGE("failed to api_connect\n");
		 return ret;
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
	   	  LOGE("failed to api_verify_pin\n");
	   	  return ret;
     }
	 
	 ret =  api_set_transkey (&trankey);//设置传输密钥
	 if(ret!=SE_SUCCESS)
	 {		 
		  LOGE("failed to api_set_transkey\n");
		  return ret;
	 }
  /****************************************模式切换*****************************************/	
  if(option == 0x00)
  {
     mode_type = STANDBY;
     ret = api_switch_mode (mode_type);
     if ( ret != SE_SUCCESS)
  	 {
  		LOGE("failed to api_switch_mode!\n");
  		return ret;
     }
     port_printf("\n\n**************************************\n");
     port_printf("\n\napi_switch_mode STANDBY successfully\n");
     port_printf("\n\n**************************************\n");
  }     
  else
  {
      port_printf("\n\n**************************************\n");
      port_printf("\n\nthe input parameter is error!\n");
      port_printf("\n\n**************************************\n");
  }  
	
  
	//---- 4. 断开连接 ----
	ret = api_disconnect ();
	if(ret!=SE_SUCCESS)
	{		
		 LOGE("failed to api_disconnect\n");
		 return ret;
	}
	
 LOGI("test end\n"); 	
	
	return SE_SUCCESS;
  

	
}






/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
