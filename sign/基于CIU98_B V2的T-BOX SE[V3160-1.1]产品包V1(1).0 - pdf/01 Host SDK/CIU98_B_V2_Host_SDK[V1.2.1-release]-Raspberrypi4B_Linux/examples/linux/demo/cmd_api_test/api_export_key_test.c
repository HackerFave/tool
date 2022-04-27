/******************************************************************************
 Copyright(C),CEC Huada Electronic Design Co.,Ltd.
 File name: 		api_export_key_test.c
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
  	port_printf("\nHelp menu: api_export_key_test <option> ...<option>\n");
	  port_printf("option:- \n");
    port_printf("-a <alg>  : alg type: ALG_SM2    : 0x50  \n");        
    port_printf("                      ALG_ECC256_NIST : 0x70 \n"); 
    port_printf("                      ALG_SM4 : 0x40 \n");
    port_printf("                      ALG_RSA1024_STANDAND : 0x30 \n"); 
    port_printf("                      ALG_RSA1024_CRT : 0x31 \n");   
    port_printf("                      ALG_RSA2048_STANDAND : 0x32 \n"); 
    port_printf("                      ALG_RSA2048_CRT : 0x33 \n");   
	  port_printf("-k <kid>               : The kid for the import key   \n"); 
    port_printf("-t <export key type>   : key type: PUB : 0x01 \n");          
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
  uint8_t com_outbuf[4096] = {0};
  uint8_t in_buf[4096] = {0};
  uint8_t out_buf[4096] = {0};
  uint32_t in_buf_len = 0;
  uint32_t out_buf_len = 0;
  uint32_t outlen =0;	
  se_error_t ret = 0;
  uint32_t i = 0;
  uint32_t k_flag=0;
  uint32_t a_flag=0;
  uint32_t t_flag=0;
  int ch;
  uint8_t trankey_val[16]= {0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4A,0x4B,0x4C,0x4D,0x4E,0x4F};
  sym_key_t trankey={0};
  uint8_t pin_buf[16] = {0x12,0x34,0x56,0x78,0x9A,0xBC,0xDE,0xF0,0x12,0x34,0x56,0x78,0x9A,0xBC,0xDE,0xF0};
  pin_t pin={0};
  uint8_t random[16]={0};
  //uint8_t key_buf[16]={0};
  uint8_t kid;
   uint8_t alg_type;
   char *input_file = NULL;
   char file_in_buf[400] ;
   uint32_t len;
   FILE *fp;
   bool if_cipher = false;
	bool if_trasns_key = false;
  uint8_t key_type;
  unikey_t ukey = {0};
	unikey_t exkey = {0};
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
    
    
       while ((ch = getopt(argc, argv, "a:k:t:")) != -1)
       {
           switch (ch) 
        {
               case 'a':
                       a_flag = 1;
                       alg_type = trans_int(optarg);             
                       break;
                case 'k':
                       k_flag = 1;
                       kid = trans_int(optarg); 
      				       if(kid>0xFF)
      			        	{
                          port_printf("The key id must between 0x00 ~ 0xFF\n");
      						        exit(0);
      					      }
                       break;
                case 't':
                       t_flag = 1;
                       key_type = trans_int(optarg);
                       if(key_type != PUB)
                         {
                           port_printf("The key_type is error!\n");
                           exit(0);    
                         }
                                  
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
  /****************************************api_export_key*****************************************/	
	  
   if((a_flag == 1)&& (t_flag == 1)&& (k_flag == 1))
   {
   exkey.alg = alg_type;
	 exkey.id = kid;
	 exkey.type = key_type;
	 if_cipher = false;//���ĵ���
	 if_trasns_key = false;
	 ret =  api_export_key (&ukey,&exkey, if_cipher, if_trasns_key);//������Կ
	 if(ret!=SE_SUCCESS)
	 {		 
		  LOGE("failed to api_export_key\n");
		  return ret;
	 }
     port_printf("export pub_key:\n");
	 
      	if ( ret != SE_SUCCESS)
      	{
      		LOGE("failed to api_export_key\n");
      		return ret;
      	}
  
        port_printf("\n\n**************************************\n");
        switch(alg_type)
        {
          case ALG_RSA1024_STANDAND:
              port_printf("the alg is:%s\n","ALG_RSA1024_STANDAND");
              break;
          case ALG_RSA1024_CRT:
              port_printf("the alg is:%s\n","ALG_RSA1024_CRT");
              break;
          case ALG_RSA2048_STANDAND:
              port_printf("the alg is:%s\n","ALG_RSA2048_STANDAND");
              break;
          case ALG_RSA2048_CRT:
              port_printf("the alg is:%s\n","ALG_RSA2048_CRT");
              break; 
          case ALG_ECC256_NIST:
              port_printf("the alg is:%s\n","ALG_ECC256_NIST");
              break;
          case ALG_SM2:
              port_printf("the alg is:%s\n","ALG_SM2");
              break;                                                                                                              
        }
        port_printf("the export public key data length:%d\n",exkey.val_len);
        port_printf("the export public key data :\n");
        for(i = 0;i<exkey.val_len;i++)
      	 {
      		port_printf("%02x",exkey.val[i]);
      	 }
        port_printf("\napi_export_key successfully!");
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
