/******************************************************************************
 Copyright(C),CEC Huada Electronic Design Co.,Ltd.
 File name: 		api_update_mkey_test.c
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
#include "comm.h"
#include "pin.h"
#include "soft_alg.h"
#include "string2byte.h"
#include "key.h"
#include "crypto.h"
#include "auth.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
void helpmenu(void)
{
	  port_printf("\nHelp menu: api_update_mkey_test <option> ...<option>\n");
	  port_printf("option:- \n");
    port_printf("-l <length>      : Mkey length \n");
	  port_printf("-k <mkey>        : Mkey \n");
    port_printf("-e <if_encrypt>  : encrypt by transkey     : 1 \n");
    port_printf("                 : not_encrypt by transkey : 0 \n");
    port_printf("-h               : Print this help \n");
}

static uint32_t trans_int(const char *aArg)
{
	uint32_t value;

	if (strncmp(aArg, "0x",2) == 0)
		sscanf(aArg,"%x",&value);
	else
		sscanf(aArg,"%d",&value);

	return value;
}

int main(int argc, char * argv[])
{
  uint8_t com_outbuf[300] = {0};
  uint8_t in_buf[500] = {0};
  uint8_t out_buf[500] = {0};
  uint32_t in_buf_len = 0;
  uint32_t out_buf_len = 0;
	uint32_t outlen =0;	
	se_error_t ret = 0;
  uint32_t i = 0;
   uint32_t mkey_len = 0;
  char mkey_input[16]; 
  uint32_t k_flag=0;
  uint32_t l_flag=0;
  uint32_t e_flag=0;
  uint32_t if_encryto=0;
  int ch;
  uint8_t trankey_val[16]= {0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4A,0x4B,0x4C,0x4D,0x4E,0x4F};
	sym_key_t trankey={0};
	uint8_t pin_buf[16] = {0x12,0x34,0x56,0x78,0x9A,0xBC,0xDE,0xF0,0x12,0x34,0x56,0x78,0x9A,0xBC,0xDE,0xF0};
  uint8_t mkey_old[16]={0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4A,0x4B,0x4C,0x4D,0x4E,0x4F};  
  pin_t pin={0};
  sym_key_t mkey={0};
  uint8_t random[16]={0};
    uint8_t key_buf[16]={0};
	//MCU初始化
	port_mcu_init();
   
   LOGI("test begin\n"); 
   
   if (argc < 2)
    {
      port_printf("\n");
      port_printf("--------------------------\n");
			helpmenu();
			exit(0);
		}
    
    
       while ((ch = getopt(argc, argv, "l:k:e:")) != -1)
       {
           switch (ch) 
        {
               case 'k':
                       k_flag = 2;
                       memcpy(mkey_input, optarg, 2*mkey_len );
                       StringToByte(mkey_input, key_buf, 2*mkey_len);//字符转为字节                  
                       break;
                case 'l':
                       l_flag = 1;
                          mkey_len = trans_int(optarg);                     
                       break;
                case 'e':
                       e_flag = 3;
                        if((strncmp (optarg, "0x", 2 )==0)||(strncmp (optarg, "0X", 2 )==0))
                          sscanf(optarg,"%x",&if_encryto);
                       else
                          sscanf(optarg,"%d",&if_encryto);       
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
  /****************************************api_update_mkey*****************************************/	
   /*
	1、获得设备权限
	2、更新主控密钥 
	*/
    //1、获得设备权限
     //获取16字节随机数
    api_get_random (0x10, random);
     //对随机数进行SM4加密获得验证数据
    ex_sm4_enc_dec(random,0x10,mkey_old,0x10,ECB,ENCRYPT,out_buf,&out_buf_len);
     //调用设备认证接口
     
    ret = api_ext_auth(out_buf,out_buf_len);
    if ( ret != SE_SUCCESS)
    {
      LOGE("failed to api_ext_auth!\n");
      return ret;
    }
    
   if((k_flag == 2)&& (l_flag == 1)&& (e_flag == 3))
   {
      memcpy(mkey.val, key_buf, mkey_len);
      mkey.val_len = mkey_len;
    //2、更新主控密钥
      if(if_encryto == 0)
       ret =api_update_mkey(&mkey, false);
      else 
       ret =api_update_mkey(&mkey, true);
      if ( ret != SE_SUCCESS)
      {
      LOGE("failed to api_update_mkey!\n");
      return ret;
      }
      port_printf("\n\n**************************************\n");
      port_printf("\napi_update_mkey successfully\n");
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
