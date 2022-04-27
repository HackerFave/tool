/******************************************************************************
 Copyright(C),CEC Huada Electronic Design Co.,Ltd. 
 File name: 		api_import_key_test.c
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
  	port_printf("\nHelp menu: api_import_key_test <option> ...<option>\n");
	  port_printf("option:- \n");
    port_printf("-a <alg>  : alg type: ALG_AES128 : 0x60 \n");        
    port_printf("                      ALG_DES128 : 0x00 \n"); 
    port_printf("                      ALG_SM4 : 0x40 \n");
    port_printf("                      ALG_RSA1024_STANDAND : 0x30 \n"); 
    port_printf("                      ALG_RSA1024_CRT : 0x31 \n");   
    port_printf("                      ALG_RSA2048_STANDAND : 0x32 \n"); 
    port_printf("                      ALG_RSA2048_CRT : 0x33 \n");  
    port_printf("                      ALG_ECC256_NIST : 0x70 \n");
    port_printf("                      ALG_SM2 : 0x50 \n");    
	  port_printf("-k <kid>               : The kid for the import key   \n");
    port_printf("-l <import key length> : The import key length \n");
    port_printf("-i <import key file>   : The value of import key \n");    
    port_printf("-t <import key type>   : key type: PRI : 0x00 \n");          
    port_printf("                                   PUB : 0x01 \n"); 
    port_printf("                                   SYM : 0x03 \n");
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
  uint32_t i_flag=0;
  uint32_t l_flag=0;
  uint32_t a_flag=0;
  uint32_t t_flag=0;
  uint32_t if_encryto=0;
  uint32_t ch;
  uint8_t trankey_val[16]= {0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4A,0x4B,0x4C,0x4D,0x4E,0x4F};
  sym_key_t trankey={0};
  uint8_t pin_buf[16] = {0x12,0x34,0x56,0x78,0x9A,0xBC,0xDE,0xF0,0x12,0x34,0x56,0x78,0x9A,0xBC,0xDE,0xF0};
  pin_t pin={0};
  uint8_t random[16]={0};
  //uint8_t key_buf[16]={0};
  uint8_t kid;
   uint8_t alg_type;
   char *input_file = NULL;
   char file_in_buf[4096] ;
   uint32_t len;
   FILE *fp;
   uint8_t input[4096] = {0};
  	uint32_t input_len = 0;
   bool if_cipher = false;
	bool if_trasns_key = false;
	unikey_t ukey = {0};
	unikey_t inkey = {0};
  uint8_t key_type;
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
    
    
       while ((ch = getopt(argc, argv, "a:k:l:i:t:")) != -1)
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
                       
               case 'i':
                       if(l_flag != 1)
                       {
                        port_printf("wrong : need input the length of key before input the key data!\n");
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
                case 't':
                       t_flag = 1;
                       key_type = trans_int(optarg);             
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
  /****************************************api_import_key*****************************************/	
   /*
	1����ð�ȫ�ļ��Ķ�дȨ��
	2��������Կ
	*/
    //1����ù���ԱȨ��
	 pin.owner = ADMIN_PIN;
	 pin.pin_len = 0x10;
	 memcpy(pin.pin_value, pin_buf,pin.pin_len);
	 ret = api_verify_pin(&pin);//��֤����Աpin
	 if(ret!=SE_SUCCESS)
	 { 	  
	   	  LOGE("failed to api_connect\n");
	   	  return ret;
	 }
	  
   if((a_flag == 1)&& (i_flag == 1)&& (l_flag == 1)&& (t_flag == 1)&& (k_flag == 1))
   {
       inkey.alg = alg_type;
	     inkey.id = kid;
	     inkey.type = key_type;
	     inkey.val_len = len;
	     //ukey.alg = NULL;
	     //ukey.id = NULL;
       StringToByte(file_in_buf,input,2*len);
       memcpy( inkey.val,input, len );
	     if_cipher = true;//���ĵ���
	     if_trasns_key = true;
	     ret =  api_import_key (&ukey,&inkey, if_cipher, if_trasns_key);//������Կ
      	if ( ret != SE_SUCCESS)
      	{
      		LOGE("failed to api_import_key\n");
      		return ret;
      	}
  
        port_printf("\n\n**************************************\n");
        switch(alg_type)
        {
          case ALG_AES128:
              port_printf("the alg is:%s\n","ALG_AES128");
              break;
          case ALG_DES128:
              port_printf("the alg is:%s\n","ALG_DES128");
              break;
          case ALG_SM4:
              port_printf("the alg is:%s\n","ALG_SM4");
              break;
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
        port_printf("the import key process protected by trans_key\n",len);
        port_printf("the input key data length:%d\n",len);
        port_printf("the input key data :\n");
        for(i = 0;i<len;i++)
      	{
      		port_printf("%02x",inkey.val[i]);
      	}
        port_printf("\napi_import_key successfully!");
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
