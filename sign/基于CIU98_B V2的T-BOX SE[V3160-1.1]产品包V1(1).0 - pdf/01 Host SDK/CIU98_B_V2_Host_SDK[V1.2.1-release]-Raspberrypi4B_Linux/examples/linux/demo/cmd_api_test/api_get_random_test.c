/******************************************************************************
 Copyright(C),CEC Huada Electronic Design Co.,Ltd. 
 File name: 		api_get_random_test.c
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
  	port_printf("\nHelp menu: api_get_random <option> ...<option>\n");
	  port_printf("option:- \n");
    port_printf("-l <expected random length> : Input the expected random length \n");
    port_printf("-h                          : Print this help \n");
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
  uint8_t out_buf[500] = {0};
  uint8_t in_buf[500] = {0};
  uint32_t in_len = 0;
  uint32_t out_buf_len = 0;
  uint32_t outlen =0;	
  se_error_t ret = 0;
  uint32_t i = 0;
  uint32_t l_flag=0;
  int ch;
  uint8_t trankey_val[16]= {0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4A,0x4B,0x4C,0x4D,0x4E,0x4F};
  sym_key_t trankey={0};
  uint8_t pin_buf[16] = {0x12,0x34,0x56,0x78,0x9A,0xBC,0xDE,0xF0,0x12,0x34,0x56,0x78,0x9A,0xBC,0xDE,0xF0};
  pin_t pin={0};
  uint8_t random[16]={0};
  //uint8_t key_buf[16]={0};
  uint32_t len;
	//MCU��ʼ��
	port_mcu_init();
   
   LOGI("test begin\n"); 
   
   if (argc < 2)
    {
      port_printf("\n");
      port_printf("--------------------------\n");
			helpmenu();
			exit(0);
		}
    
    
       while ((ch = getopt(argc, argv, "l:")) != -1)
       {
           switch (ch) 
        {
                case 'l':
                       l_flag = 1;
                       len = trans_int(optarg);             
                       break;
              case 'h': 
		         	default:  
				      helpmenu();
				      exit(0);
              break;
         }
       }
  
  /****************************************SEע�ᡢѡ�����Ӻ����ô�����Կ*****************************************/	

   
	//---- 1. ע�ἰѡ���豸----
	ret = api_register(PERIPHERAL_SPI, SPI_PERIPHERAL_SE0);
	if(ret!=SE_SUCCESS)
	{		
		 LOGE("failed to spi api_register\n");
		 return ret;
	}
	
	//---- 2. ���ӣ���ȡatr ----
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
		 LOGE("failed to api_connect\n");
		 return ret;
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
	   	  LOGE("failed to api_verify_pin\n");
	   	  return ret;
     }
	   
	 ret =  api_set_transkey (&trankey);//���ô�����Կ
   
	 if(ret!=SE_SUCCESS)
	 {		 
		  LOGE("failed to api_set_transkey\n");
		  return ret;
	 }
  /****************************************api_get_random*****************************************/	
	
   if((l_flag == 1))
   {
      
    
      	ret = api_get_random (len, out_buf);//��ȡ������ӿ�
      	if ( ret != SE_SUCCESS)
      	{
      		LOGE("failed to api_get_random!\n");
      		return ret;
      	}
        port_printf("\n\n**************************************\n");
        port_printf("\nthe random length:%d\n",len);
        port_printf("the random data :\n");
        for(i = 0;i<len;i++)
      	{
      		port_printf("%02x",out_buf[i]);
      	}
        port_printf("\napi_get_random successfully!");
        port_printf("\n\n**************************************\n");
     }
     else
     {
        port_printf("\n\n**************************************\n");
        port_printf("\n\nthe input parameter is error!\n");
        port_printf("\n\n**************************************\n");
     }
    
     
	//---- 4. �Ͽ����� ----
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
