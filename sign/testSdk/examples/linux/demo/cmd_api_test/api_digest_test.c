/******************************************************************************
 Copyright(C),CEC Huada Electronic Design Co.,Ltd. 
 File name: 		api_digest_test.c
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
  	port_printf("\nHelp menu: api_digest_test <option> ...<option>\n");
	  port_printf("option:- \n");
    port_printf("-a <alg>  : alg type: ALG_SHA1   : 0x00 \n");        
    port_printf("                      ALG_SHA224 : 0x01 \n"); 
    port_printf("                      ALG_SHA256 : 0x02 \n");
    port_printf("                      ALG_SHA384 : 0x03 \n"); 
    port_printf("                      ALG_SHA512 : 0x04 \n");
    port_printf("                      ALG_SM3    : 0x05 \n");
    port_printf("-l <input data length> : The input data length \n");
    port_printf("-i <input data file>       : The input data file \n");    
    port_printf("-h                     : Print this help \n");
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
  uint8_t out_buf[2048] = {0};
  uint8_t in_buf[2048] = {0};
  uint32_t in_len = 0;
  uint32_t out_buf_len = 0;
  uint32_t outlen =0;	
  se_error_t ret = 0;
  uint32_t i = 0;
  uint32_t i_flag=0;
  uint32_t l_flag=0;
  uint32_t a_flag=0;
  int ch;
  uint8_t trankey_val[16]= {0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4A,0x4B,0x4C,0x4D,0x4E,0x4F};
  sym_key_t trankey={0};
  uint8_t pin_buf[16] = {0x12,0x34,0x56,0x78,0x9A,0xBC,0xDE,0xF0,0x12,0x34,0x56,0x78,0x9A,0xBC,0xDE,0xF0};
  pin_t pin={0};
  uint8_t random[16]={0};
  //uint8_t key_buf[16]={0};
  uint8_t alg_type;
  char *input_file = NULL;
  char file_in_buf[4096] ;
  uint32_t len;
  FILE *fp;
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
    
    
       while ((ch = getopt(argc, argv, "a:l:i:")) != -1)
       {
           switch (ch) 
        {
               case 'a':
                       a_flag = 1;
                       alg_type = trans_int(optarg);             
                       break;                
               case 'i':
                       if(l_flag != 1)
                       {
                        port_printf("wrong : need input the length of key before input the data!\n");
                        exit(0);
                       }  
                       i_flag = 1;
                       input_file = optarg; 
                       fp = fopen((const char *)optarg, "rb");
                       if(!fp)
                       {
                         port_printf("can not open the file\n");
                         return -1;
                       }        
                         fread(file_in_buf, 1,len*2, fp);  
                         fclose(fp);       
                       break;
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
  /****************************************api_digest*****************************************/	
	
   if((a_flag == 1)&& (i_flag == 1)&& (l_flag == 1))
   {
        StringToByte(file_in_buf,in_buf,2*len);
      	in_len = len;
        
      	ret = api_digest (alg_type, in_buf, in_len, out_buf, &out_buf_len);//��ϣ�㷨�ӿ�
      	if ( ret != SE_SUCCESS)
      	{
      		LOGE("failed to api_digest!\n");
      		return ret;
      	}
        port_printf("\n\n**************************************\n");
      	port_printf("api_digest in_buf:\n");
      	for(i = 0;i<in_len;i++)
      	{
      			port_printf("%02x",in_buf[i]);
      	}
      	port_printf("\n");	      
        switch(alg_type)
        {
          case ALG_SHA1:
              port_printf("the alg is:%s\n","ALG_SHA1");
              break;
          case ALG_SHA224:
              port_printf("the alg is:%s\n","ALG_SHA224");
              break;
          case ALG_SHA256:
              port_printf("the alg is:%s\n","ALG_SHA256");
              break;
          case ALG_SHA384:
              port_printf("the alg is:%s\n","ALG_SHA384");
              break;
          case ALG_SHA512:
              port_printf("the alg is:%s\n","ALG_SHA512");
              break;
          case ALG_SM3:
              port_printf("the alg is:%s\n","ALG_SM3");
              break;                                                                              
        }
        port_printf("\nthe output digested data length:%d\n",out_buf_len);
        port_printf("the digested data :\n");
        for(i = 0;i<out_buf_len;i++)
      	{
      		port_printf("%02x",out_buf[i]);
      	}
        port_printf("\napi_digest successfully!");
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
