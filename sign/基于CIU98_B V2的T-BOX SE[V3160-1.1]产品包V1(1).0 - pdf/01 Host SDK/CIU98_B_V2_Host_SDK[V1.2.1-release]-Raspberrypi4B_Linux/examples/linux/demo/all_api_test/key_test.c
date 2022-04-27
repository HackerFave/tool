/**@file  key_test.c
* @brief  key_test interface declearation	 
* @author liangww
* @date  2021-05-28
* @version	V1.0
* @copyright  Copyright(C),CEC Huada Electronic Design Co.,Ltd.
*/

/***************************************************************************
* Include Header Files
***************************************************************************/
#include "key_test.h"
#include "soft_alg.h"
#include "crypto.h"
#include "pin.h"
/** @addtogroup SE_APP_TEST
  * @{
  */



/** @defgroup KEY_TEST KEY_TEST
  * @brief key_test interface api.
  * @{
  */



/* Exported functions --------------------------------------------------------*/

/** @defgroup KEY_TEST_Exported_Functions KEY_TEST Exported Functions
  * @{
  */


/**
* @brief 产生密钥对、导出公钥、更新密钥等示例
* @param no
* @return 参见error.h
* @note no
* @see 函数 update_mkey_test set_transkey_test generate_keypair_test
*/ 
se_error_t key_test (void)
{
	/*
	1、更新主控密钥测试update_mkey_test
	2、设置传输密钥测试set_transkey_test
	3、生成非对称密钥对测试generate_keypair_test
	4、生成对称密钥对测试generate_symkey_test
	5、密钥协商测试generate_shared_key_test
	6、导入密钥测试import_key_test
	7、导出密钥测试export_key_test
	8、删除密钥测试del_key_test
	9、获取密钥信息测试get_key_info_test
	*/
	se_error_t ret = 0;
	
	ret = update_mkey_test();
	if ( ret != SE_SUCCESS)
	{
		LOGE("failed to update_mkey_test!\n");
		return ret;
	}
	
	ret = set_transkey_test();
	if ( ret != SE_SUCCESS)
	{
		LOGE("failed to set_transkey_test!\n");
		return ret;
	}
	
  ret = generate_keypair_test();
	if ( ret != SE_SUCCESS)
	{
		LOGE("failed to generate_keypair_test!\n");
		return ret;
	}
	
	ret = generate_symkey_test();
	if ( ret != SE_SUCCESS)
	{
		LOGE("failed to generate_symkey_test!\n");
		return ret;
	}

	ret = generate_shared_key_test();
	if ( ret != SE_SUCCESS)
	{
		LOGE("failed to generate_symkey_test!\n");
		return ret;
	}
	
	ret = import_key_test();
	if ( ret != SE_SUCCESS)
	{
		LOGE("failed to import_key_test!\n");
		return ret;
	}
  ret = export_key_test();
	if ( ret != SE_SUCCESS)
	{
		LOGE("failed to import_key_test!\n");
		return ret;
	}
	ret = del_key_test();
	if ( ret != SE_SUCCESS)
	{
		LOGE("failed to import_key_test!\n");
		return ret;
	}
	ret = get_key_info_test();
	if ( ret != SE_SUCCESS)
	{
		LOGE("failed to import_key_test!\n");
		return ret;
	}

	return SE_SUCCESS;
}



/**
* @brief 更新设备主控密钥示例
* @param no
* @return 参见error.h
* @note 仅限于本地演示
* @see 函数 api_ext_auth api_update_mkey 
*/
se_error_t update_mkey_test(void)
{
	
	/*
	1、获得设备权限
	2、更新主控密钥 
	3、使用新的主控密钥获得设备权限
	4、更新为原主控密钥
	*/
	se_error_t ret = SE_SUCCESS;
	uint8_t mkey_old[16]={0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4A,0x4B,0x4C,0x4D,0x4E,0x4F};    
	uint8_t mkey_new[16]={0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5A,0x5B,0x5C,0x5D,0x5E,0x5F};
	uint8_t random[16]={0};
    sym_key_t mkey={0};
	uint8_t out_buf[16]={0};
	uint32_t out_buf_len;
	
	//1、获得设备权限
	//获取16字节随机数
    api_get_random (0x10, random);
    //对随机数进行SM4加密获得验证数据
	ex_sm4_enc_dec(random,0x10,mkey_old,0x10,ECB,ENCRYPT,out_buf,&out_buf_len);
	//调用设备认证接口
     ret = api_ext_auth(out_buf,out_buf_len);
    if ( ret != SE_SUCCESS)
	{
		LOGE("failed to auth_test!\n");
		return ret;
	}
	

	//2、更新主控密钥
	memcpy(mkey.val, mkey_new, 16);
	mkey.val_len = 16;
    ret =api_update_mkey(&mkey, false);
	if ( ret != SE_SUCCESS)
	{
		LOGE("failed to auth_test!\n");
		return ret;
	}


	//3、使用新的主控密钥获得设备权限
	//获取16字节随机数
    api_get_random (0x10, random);
    //对随机数进行SM4加密获得验证数据
	ex_sm4_enc_dec(random,0x10,mkey_new,0x10,ECB,ENCRYPT,out_buf,&out_buf_len);
	//调用设备认证接口
     ret = api_ext_auth(out_buf,out_buf_len);
    if ( ret != SE_SUCCESS)
	{
		LOGE("failed to auth_test!\n");
		return ret;
	}

	
	//4、更新为原主控密钥
	memcpy(mkey.val, mkey_old, 16);
	mkey.val_len = 16;
    ret =api_update_mkey(&mkey, false);
	if ( ret != SE_SUCCESS)
	{
		LOGE("failed to auth_test!\n");
		return ret;
	}
    
	return ret;

}




/**
* @brief 设置传输密钥示例
* @param no
* @return 参见error.h
* @note 仅限于本地演示
* @see 函数 api_verify_pin api_set_transkey 
*/
se_error_t set_transkey_test(void)
{
	
	/*
	1、获得管理员权限更新为新传输密钥 
	2、获得管理员权限更新为原传输密钥
	*/
	se_error_t ret = SE_SUCCESS;
	uint8_t transk_old[16]={0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4A,0x4B,0x4C,0x4D,0x4E,0x4F};    
	uint8_t transk_new[16]={0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5A,0x5B,0x5C,0x5D,0x5E,0x5F};
    sym_key_t trankey={0};
    uint8_t pin_buf[16] = {0x12,0x34,0x56,0x78,0x9A,0xBC,0xDE,0xF0,0x12,0x34,0x56,0x78,0x9A,0xBC,0xDE,0xF0};
    pin_t pin={0};
	
	//1、获得管理员权限更新为新传输密钥 
	 memcpy(trankey.val,transk_new,16);
	 trankey.val_len = 16;
	 trankey.id = 0x02;//传输密钥kid必须为0x02
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


	//2、获得管理员权限更新为原传输密钥 
	 memcpy(trankey.val,transk_old,16);
	 trankey.val_len = 16;
	 trankey.id = 0x02;//传输密钥kid必须为0x02
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
    
	return ret;

}



/**
* @brief 产生密钥对示例
* @param no
* @return 参见error.h
* @note no
* @see 函数api_verify_pin  api_generate_keypair
*/
se_error_t generate_keypair_test (void)
{
	/*
	1、验证管理员pin
	2、产生密钥对
	*/

	se_error_t ret = 0;
	pub_key_t pub_key={0};
	pri_key_t pri_key={0};
	uint8_t pin_buf[16] = {0x12,0x34,0x56,0x78,0x9A,0xBC,0xDE,0xF0,0x12,0x34,0x56,0x78,0x9A,0xBC,0xDE,0xF0};
    pin_t pin={0};
	uint32_t i = 0;

	//验证管理员pin
	 pin.owner = ADMIN_PIN;
	 pin.pin_len = 0x10;
	 memcpy(pin.pin_value, pin_buf,pin.pin_len);
	 ret = api_verify_pin(&pin);//验证管理员pin
	 if(ret!=SE_SUCCESS)
	 { 	  
	   	  LOGE("failed to api_verify_pin\n");
	   	  return ret;
	 }
	 
	/****************************************generate key pair RSA1024 test*****************************************/
		
		//生成公钥kid为0x90,私钥kid为0x91的RSA1024密钥对
		pub_key.alg = ALG_RSA1024_STANDAND;
		pub_key.id	= 0x90;
		pri_key.id	= 0x91;
		ret = api_generate_keypair (&pub_key, &pri_key);
		if ( ret != SE_SUCCESS)
		{
			LOGE("failed to generate_keypair_test\n");
			return ret;
		}
		for(i = 0;i<pub_key.val_len;i++)
		{
			port_printf("%02x",pub_key.val[i]);
		}
		port_printf("\n");
	/****************************************generate key pair RSA2048 test*****************************************/
	//pub_key.alg = ALG_RSA1024_CRT;
	//pub_key.id  = 0xF0;
	//ret = api_generate_keypair (&pub_key);
	//if ( ret != SE_SUCCESS)
	//{
	//	LOGE("failed to scp02_test\n");
	//	return ret;
	//}
	//生成公私钥kid都为0x60的RSA2048密钥对
	pub_key.alg = ALG_RSA2048_STANDAND;
	pub_key.id  = 0x60;
	pri_key.id  = 0x60;
	ret = api_generate_keypair (&pub_key, &pri_key);
	if ( ret != SE_SUCCESS)
	{
		LOGE("failed to generate_keypair_test\n");
		return ret;
	}
	for(i = 0;i<pub_key.val_len;i++)
	{
		port_printf("%02x",pub_key.val[i]);
	}
	port_printf("\n");


    /****************************************generate key pair test*****************************************/
	//设置算法类型
	
	pub_key.alg = ALG_SM2;
	pub_key.id  = 0xF0;
    pri_key.id  = 0xF1;
	
	ret = api_generate_keypair (&pub_key, &pri_key);
	if ( ret != SE_SUCCESS)
	{
		LOGE("failed to generate_keypair_test\n");
		return ret;
	}

	port_printf("api_generate_keypair pub_key:\n");
	for(i = 0;i<pub_key.val_len;i++)
	{
		port_printf("%02x",pub_key.val[i]);
	}
	port_printf("\n");

	
  
	return SE_SUCCESS;
}



/**
* @brief 产生对称密钥示例
* @param no
* @return 参见error.h
* @note no
* @see 函数api_verify_pin  api_generate_symkey 
*/
se_error_t generate_symkey_test (void)
{
	/*
	1、验证管理员pin
	2、产生对称密钥
	*/

	se_error_t ret = 0;
	sym_key_t symkey ={0};
	uint8_t pin_buf[16] = {0x12,0x34,0x56,0x78,0x9A,0xBC,0xDE,0xF0,0x12,0x34,0x56,0x78,0x9A,0xBC,0xDE,0xF0};
    pin_t pin={0};
	//获取16字节随机数
	 pin.owner = ADMIN_PIN;
	 pin.pin_len = 0x10;
	 memcpy(pin.pin_value, pin_buf,pin.pin_len);
	 ret = api_verify_pin(&pin);//生成固定密钥需要获得安全文件的写权限，验证管理员pin
	 if(ret!=SE_SUCCESS)
	 { 	  
	   	  LOGE("failed to api_connect\n");
	   	  return ret;
	 }
	 
	/****************************************generate symkey key 3DES test*****************************************/
		
		//生成kid为0x92的3DES密钥
		symkey.alg = ALG_DES128;
		symkey.id	= 0x92;
		ret = api_generate_symkey (&symkey);
		if ( ret != SE_SUCCESS)
		{
			LOGE("failed to generate_symkey_test\n");
			return ret;
		}
		
	
   /****************************************generate symkey key AES test*****************************************/
		
		//生成kid为0x93的AES密钥
		symkey.alg = ALG_AES128;
		symkey.id	= 0x93;
		ret = api_generate_symkey (&symkey);
		if ( ret != SE_SUCCESS)
		{
			LOGE("failed to generate_symkey_test\n");
			return ret;
		}

  /****************************************generate symkey key SM4 test*****************************************/
		
		//生成kid为0x94的SM4密钥
		symkey.alg = ALG_SM4;
		symkey.id	= 0x94;
		ret = api_generate_symkey (&symkey);
		if ( ret != SE_SUCCESS)
		{
			LOGE("failed to generate_symkey_test\n");
			return ret;
		}
		
	
  
	return SE_SUCCESS;
}



/**
* @brief 产生协商密钥示例
* @param no
* @return 参见error.h
* @note no
* @see 函数 api_generate_shared_key
*/

se_error_t generate_shared_key_test(void)
{
	se_error_t ret = 0;
	char s_key_info[KEY_DATA_SIZE] = {0};
	uint32_t s_key_info_len = 0;
	uint8_t input[KEY_DATA_SIZE/2] = {0};
	uint32_t input_len = 0;
	unikey_t shared_key = {0};
	uint8_t sm2_s[64] = {0};
	bool if_return_key = true;
	bool if_return_s = true;
	pub_key_t pub_key={0};
	pri_key_t pri_key={0};
	

	/****************************************ECDH NIST256*****************************************/	
	//ECC密钥
	shared_key.alg = ALG_ECDH_ECC256;
	shared_key.id = 0x09;
	strcpy(s_key_info,"D17ED15EDFFFFBC6DB023E1F4E4B9A45C590A816F8CAE301633940E483791728C85E5C328BC059F7A30496B846B5932E7E3E1D34AB53B3CBFF1D98954EFDF511");
	s_key_info_len = 128;
	StringToByte(s_key_info,input,s_key_info_len);
	input_len = s_key_info_len/2;
	ret = api_generate_shared_key (input, input_len,&shared_key,sm2_s, if_return_key, if_return_s);
    if ( ret != SE_SUCCESS)
	{
		LOGE("failed to export_public_key_test!\n");
		return ret;
	}
	/****************************************ECDH SM2曲线*****************************************/	
	//SM2密钥
	shared_key.alg = ALG_ECDH_SM2;
	shared_key.id = 0x0F;
	strcpy(s_key_info,"160E12897DF4EDB61DD812FEB96748FBD3CCF4FFE26AA6F6DB9540AF49C942324A7DAD08BB9A459531694BEB20AA489D6649975E1BFCF8C4741B78B4B223007F");
	s_key_info_len = 128;
	StringToByte(s_key_info,input,s_key_info_len);
	input_len = s_key_info_len/2;
	ret = api_generate_shared_key (input, input_len,&shared_key,sm2_s, if_return_key, if_return_s);
    if ( ret != SE_SUCCESS)
	{
		LOGE("failed to export_public_key_test!\n");
		return ret;
	}
	/****************************************SM2*****************************************/	
	//产生临时公私钥对

	pub_key.alg = ALG_SM2;
	pub_key.id  = 0xF0;
    pri_key.id  = 0xF1;
	ret = api_generate_keypair (&pub_key, &pri_key);
	if ( ret != SE_SUCCESS)
	{
		LOGE("failed to generate_keypair_test\n");
		return ret;
	}
	//协商过程类别
	input[0] = 0x01;
	//临时对称密钥的密钥标识符（1字节）
	input[1] = 0xF7;
	//算法类型SM4
	input[2] = ALG_SM4;
	//临时公私钥、固定公私钥
	memcpy(input+3,"\xF0\xF1\x0F\x0F",4);
	//对方临时公钥
	memcpy(input+7,pub_key.val,64);
	//对方固有公钥
	strcpy(s_key_info,"160E12897DF4EDB61DD812FEB96748FBD3CCF4FFE26AA6F6DB9540AF49C942324A7DAD08BB9A459531694BEB20AA489D6649975E1BFCF8C4741B78B4B223007F");
	s_key_info_len = 128;
	StringToByte(s_key_info,input+71,s_key_info_len);
	input_len = s_key_info_len/2;
	memcpy(input+135,"\x01\xAA\x01\xBB",4);//双方ID
	input_len = 139;
	shared_key.alg = ALG_SM2_SM2;
	ret = api_generate_shared_key (input, input_len,&shared_key,sm2_s, if_return_key, if_return_s);
    if ( ret != SE_SUCCESS)
	{
		LOGE("failed to export_public_key_test!\n");
		return ret;
	}
	return SE_SUCCESS;

}





/**
* @brief 密钥导入示例
* @param no
* @return 参见error.h
* @note 仅限于本地演示
* @see 函数 api_verify_pin api_set_transkey 
*/
se_error_t import_key_test(void)
{
	
	se_error_t ret = SE_SUCCESS;
    uint8_t pin_buf[16] = {0x12,0x34,0x56,0x78,0x9A,0xBC,0xDE,0xF0,0x12,0x34,0x56,0x78,0x9A,0xBC,0xDE,0xF0};
    pin_t pin={0};
    bool if_cipher = false;
	bool if_trasns_key = false;
	unikey_t ukey = {0};
	unikey_t inkey = {0};
	uint8_t appkey[700] = {0};
	char s_appkey[1300]={0};
	uint32_t s_appkey_len = 0;
    char s_key_cipher[256] ={0}; 
    uint32_t s_key_cipher_len = 0;
    uint8_t in_buf[32] = {0};


	pin.owner = ADMIN_PIN;
	pin.pin_len = 0x10;
	memcpy(pin.pin_value, pin_buf,pin.pin_len);
	ret = api_verify_pin(&pin);//导入固定密钥，需验证管理员pin
	if(ret!=SE_SUCCESS)
	{	  
		LOGE("failed to api_connect\n");
		return ret;
	}


	/****************************************导入SM4对称密钥*****************************************/	 

	//1、明文导入SM4对称密钥
	 
	 inkey.alg = ALG_SM4;
	 inkey.id = 0x11;
	 inkey.type = SYM;
	 inkey.val_len = 0x10;
	 ukey.alg = NULL;
	 ukey.id = NULL;
     memset(inkey.val,0x00,16);
	 if_cipher = false;//明文导入
	 if_trasns_key = false;
	 ret =  api_import_key (&ukey,&inkey, if_cipher, if_trasns_key);//导入密钥
	 if(ret!=SE_SUCCESS)
	 {		 
		  LOGE("failed to api_connect\n");
		  return ret;
	 }


	 
	 //2、传输密钥加密，密文导入SM4对称密钥
	 inkey.alg = ALG_SM4;
	 inkey.id = 0x11;
	 inkey.type = SYM;
	 inkey.val_len = 0x10;
	 ukey.alg = NULL;
	 ukey.id = NULL;
     memset(inkey.val,0x01,16);//被导入密钥值为“01010101010101010101010101010101”
	 if_cipher = true;//密文导入
	  if_trasns_key = true;
	 ret =  api_import_key (&ukey,&inkey, if_cipher, if_trasns_key);//导入密钥
	 if(ret!=SE_SUCCESS)
	 {		 
		  LOGE("failed to api_connect\n");
		  return ret;
	 }



	 //3、SM4应用密钥加密，密文导入SM4对称密钥
	 inkey.alg = ALG_SM4;
	 inkey.id = 0x11;
	 ukey.alg = ALG_SM4;
	 ukey.id = 0x06;
	 inkey.type = SYM;
	  inkey.val_len = 0x20;//加密之后的长度为32字节
	 if_cipher = true;//密文导入
	 if_trasns_key = false;//应用密钥加密导入
	 
     //密钥值利用kid 0x06 SM4应用密钥加密为密文。 
     memset(appkey,0x00,16);//应用密钥值为 “00000000000000000000000000000000” 

	 //添加两字节Ld，即被导入密钥的明文长度
	 in_buf[0] = 0x00;
	 in_buf[1] = 0x10;

	 //被导入密钥值放入inbuf，密钥值为“02020202020202020202020202020202”
	 memset(in_buf+2, 0x02,16);

	 //补1字节“80”
	 in_buf[18] = 0x80;
	 //补“00”直到为16字节整数倍
	 memset(in_buf+19,0x00,14);
	 //利用应用密钥加密明文密钥值
	 ex_sm4_enc_dec(in_buf,32,appkey,16,ECB,ENCRYPT,inkey.val,&inkey.val_len );//加密之后的数据放入inkey.val

	 ret =  api_import_key (&ukey,&inkey, if_cipher, if_trasns_key);//导入密钥
	 if(ret!=SE_SUCCESS)
	 {		 
		  LOGE("failed to api_connect\n");
		  return ret;
	 }



	  //4、RSA1024非对称应用密钥加密，密文导入SM4对称密钥
     //导入的SM4密钥为“02020202020202020202020202020202”，采用pkcs1补位后进行RSA1024加密之后的数据为；“1462DE010291E899AA2916447B4289B2858A3A71AB587546CC87431BF6022882752C56CE5555FB997F282020F2202FECAD025D15A8F2FE958513C66A8C7976818010A64682D79F17974D12FA03B1DCAE4947E73166FA77D949382C0A80243602AAF6635E3B256CCD77B802959B69DEA1B5DFF434871A6821040CD2850EE207AC”
	 strcpy(s_key_cipher,"1462DE010291E899AA2916447B4289B2858A3A71AB587546CC87431BF6022882752C56CE5555FB997F282020F2202FECAD025D15A8F2FE958513C66A8C7976818010A64682D79F17974D12FA03B1DCAE4947E73166FA77D949382C0A80243602AAF6635E3B256CCD77B802959B69DEA1B5DFF434871A6821040CD2850EE207AC");
	 s_key_cipher_len = 256;
	 StringToByte(s_key_cipher,inkey.val,s_key_cipher_len);
	 
	 inkey.alg = ALG_SM4;
	 inkey.id = 0x11;
	 ukey.alg = ALG_RSA1024_CRT;
	 ukey.id = 0x0B;//RSA1024的私钥kid
	 inkey.type = SYM;
	 inkey.val_len = 0x80;//加密之后的长度为128字节
	 if_cipher = true;//密文导入
	 if_trasns_key = false;//应用密钥加密导入
	 
	
	 ret =  api_import_key (&ukey,&inkey, if_cipher, if_trasns_key);//导入密钥
	 if(ret!=SE_SUCCESS)
	 {		 
		  LOGE("failed to api_connect\n");
		  return ret;
	 }

     /****************************************导入RSA1024非对称密钥*****************************************/	 
     //1、明文导入RSA1024公钥
	  strcpy(s_appkey,"00010001BB6105E798FDBC7CD6DC2567EB890E0AA8CEA22F2D9123D06E1D29651EE81E3559A59C5153BBAD44B45EEE986DC347C103E20FF027E7A8B1160511FA419C5C7C11FC34B0C65FB27CCF4477233B859515FAE8BF92427D1C59C82A823C8A44065D97500CEF02D7B284C524403E85E12AD8159680AB4CBCB0623C3B4C43A6E7D6BF");
	 s_appkey_len = 264;
	 StringToByte(s_appkey,inkey.val,s_appkey_len); 
	 inkey.alg = ALG_RSA1024_CRT;
	 inkey.id = 0x07;
	 inkey.type = PUB;
	 inkey.val_len = 0x84;
	 ukey.alg = NULL;
	 ukey.id = NULL;
	 if_cipher = false;//明文导入
	 if_trasns_key = false;
	 ret =  api_import_key (&ukey,&inkey, if_cipher, if_trasns_key);//导入密钥
	 if(ret!=SE_SUCCESS)
	 {		 
		  LOGE("failed to api_connect\n");
		  return ret;
	 }

	  //2、明文导入RSA1024私钥
	  strcpy(s_appkey,"00010001F7A8BDCC97CEC14C42A4DFE652BCA194B0AF6452EACECD7BC68AEDBD4A9C765BDEDCD848E1DF4277555564B9ABAD49C44779D98AD1E798EE5695AFE4CA4B9FBFC1B08F7A9A7B4449EF52AFE2C4918173E1CC47C09CCCCEB173CE6884639FCEEB8773928CC9B7CDE499C899CD765B6FB54EB9505EECBC94D8A5EDA2BE53A18901B1F6E9DCB941F3994F52972A354E23766BB8BD88EFFE879E683FB133130877ECF0AB1628D956CBB660F5AA19DA09FE36337E961AB1579187C364712F214752959FE7FE6C8001689D89A1C067FD550B84FA53C6A13452BA0A32CBA3EA38BD84FD5A7485F709CA5F876D96F9B3711588222D7E3CE6419867627D8AB52360B42A01D180FBBD1E6CFD8262C3BEE4F3EA8E8D485F05CC694E245CAA3EFAFAD1E2D05D4EA9A5B939D8F1768EF4E0F4765E566B285A33BCDFBC7DCBCA506B780D95FBC0");
	 s_appkey_len = 648;
	 StringToByte(s_appkey,inkey.val,s_appkey_len); 
	 inkey.alg = ALG_RSA1024_CRT;
	 inkey.id = 0x14;
	 inkey.type = PRI;
	 inkey.val_len = 0x0144;
	 ukey.alg = NULL;
	 ukey.id = NULL;
	 if_cipher = false;//明文导入
	 if_trasns_key = false;
	 ret =  api_import_key (&ukey,&inkey, if_cipher, if_trasns_key);//导入密钥
	 if(ret!=SE_SUCCESS)
	 {		 
		  LOGE("failed to api_connect\n");
		  return ret;
	 }

	 //3、传输密钥加密，密文导入RSA1024公钥
	  strcpy(s_appkey,"00010001BB6105E798FDBC7CD6DC2567EB890E0AA8CEA22F2D9123D06E1D29651EE81E3559A59C5153BBAD44B45EEE986DC347C103E20FF027E7A8B1160511FA419C5C7C11FC34B0C65FB27CCF4477233B859515FAE8BF92427D1C59C82A823C8A44065D97500CEF02D7B284C524403E85E12AD8159680AB4CBCB0623C3B4C43A6E7D6BF");
	 s_appkey_len = 264;
	 StringToByte(s_appkey,inkey.val,s_appkey_len); 
	 inkey.alg = ALG_RSA1024_CRT;
	 inkey.id = 0x07;
	 inkey.type = PUB;
	 inkey.val_len = 0x84;
	 ukey.alg = NULL;
	 ukey.id = NULL;
	 if_cipher = true;//密文导入
	 if_trasns_key = true;//传输密钥加密
	 ret =  api_import_key (&ukey,&inkey, if_cipher, if_trasns_key);//导入密钥
	 if(ret!=SE_SUCCESS)
	 {		 
		  LOGE("failed to api_connect\n");
		  return ret;
	 }

	  //4、传输密钥加密，密文导入RSA1024私钥
	  strcpy(s_appkey,"00010001F7A8BDCC97CEC14C42A4DFE652BCA194B0AF6452EACECD7BC68AEDBD4A9C765BDEDCD848E1DF4277555564B9ABAD49C44779D98AD1E798EE5695AFE4CA4B9FBFC1B08F7A9A7B4449EF52AFE2C4918173E1CC47C09CCCCEB173CE6884639FCEEB8773928CC9B7CDE499C899CD765B6FB54EB9505EECBC94D8A5EDA2BE53A18901B1F6E9DCB941F3994F52972A354E23766BB8BD88EFFE879E683FB133130877ECF0AB1628D956CBB660F5AA19DA09FE36337E961AB1579187C364712F214752959FE7FE6C8001689D89A1C067FD550B84FA53C6A13452BA0A32CBA3EA38BD84FD5A7485F709CA5F876D96F9B3711588222D7E3CE6419867627D8AB52360B42A01D180FBBD1E6CFD8262C3BEE4F3EA8E8D485F05CC694E245CAA3EFAFAD1E2D05D4EA9A5B939D8F1768EF4E0F4765E566B285A33BCDFBC7DCBCA506B780D95FBC0");
	 s_appkey_len = 648;
	 StringToByte(s_appkey,inkey.val,s_appkey_len); 
	 inkey.alg = ALG_RSA1024_CRT;
	 inkey.id = 0x14;
	 inkey.type = PRI;
	 inkey.val_len = 0x0144;
	 ukey.alg = NULL;
	 ukey.id = NULL;
	 if_cipher = true;//密文导入
	 if_trasns_key = true;//传输密钥加密
	 ret =  api_import_key (&ukey,&inkey, if_cipher, if_trasns_key);//导入密钥
	 if(ret!=SE_SUCCESS)
	 {		 
		  LOGE("failed to api_connect\n");
		  return ret;
	 }
	 
	/****************************************导入ECC256NIST非对称密钥*****************************************/	 
     //1、明文导入ECC公钥
	  strcpy(s_appkey,"160E12897DF4EDB61DD812FEB96748FBD3CCF4FFE26AA6F6DB9540AF49C942324A7DAD08BB9A459531694BEB20AA489D6649975E1BFCF8C4741B78B4B223007F");
	 s_appkey_len = 128;
	 StringToByte(s_appkey,inkey.val,s_appkey_len); 
	 inkey.alg = ALG_ECC256_NIST;
	 inkey.id = 0x07;
	 inkey.type = PUB;
	 inkey.val_len = 0x40;
	 ukey.alg = NULL;
	 ukey.id = NULL;
	 if_cipher = false;//明文导入
	 if_trasns_key = false;
	 ret =  api_import_key (&ukey,&inkey, if_cipher, if_trasns_key);//导入密钥
	 if(ret!=SE_SUCCESS)
	 {		 
		  LOGE("failed to api_connect\n");
		  return ret;
	 }

	  //2、明文导入ECC私钥
	  strcpy(s_appkey,"81EB26E941BB5AF16DF116495F90695272AE2CD63D6C4AE1678418BE48230029");
	 s_appkey_len = 128;
	 StringToByte(s_appkey,inkey.val,s_appkey_len); 
	 inkey.alg = ALG_ECC256_NIST;
	 inkey.id = 0x14;
	 inkey.type = PRI;
	 inkey.val_len = 0x20;
	 ukey.alg = NULL;
	 ukey.id = NULL;
	 if_cipher = false;//明文导入
	 if_trasns_key = false;
	 ret =  api_import_key (&ukey,&inkey, if_cipher, if_trasns_key);//导入密钥
	 if(ret!=SE_SUCCESS)
	 {		 
		  LOGE("failed to api_connect\n");
		  return ret;
	 }

	 //3、传输密钥加密，密文导入ECC公钥
	  strcpy(s_appkey,"160E12897DF4EDB61DD812FEB96748FBD3CCF4FFE26AA6F6DB9540AF49C942324A7DAD08BB9A459531694BEB20AA489D6649975E1BFCF8C4741B78B4B223007F");
	 s_appkey_len = 128;
	 StringToByte(s_appkey,inkey.val,s_appkey_len); 
	 inkey.alg = ALG_ECC256_NIST;
	 inkey.id = 0x07;
	 inkey.type = PUB;
	 inkey.val_len = 0x40;
	 ukey.alg = NULL;
	 ukey.id = NULL;
	 if_cipher = true;//密文导入
	 if_trasns_key = true;//传输密钥加密
	 ret =  api_import_key (&ukey,&inkey, if_cipher, if_trasns_key);//导入密钥
	 if(ret!=SE_SUCCESS)
	 {		 
		  LOGE("failed to api_connect\n");
		  return ret;
	 }

	  //4、传输密钥加密，密文导入ECC私钥
	  strcpy(s_appkey,"81EB26E941BB5AF16DF116495F90695272AE2CD63D6C4AE1678418BE48230029");
	 s_appkey_len = 128;
	 StringToByte(s_appkey,inkey.val,s_appkey_len); 
	 inkey.alg = ALG_ECC256_NIST;
	 inkey.id = 0x14;
	 inkey.type = PRI;
	 inkey.val_len = 0x20;
	 ukey.alg = NULL;
	 ukey.id = NULL;
	 if_cipher = true;//密文导入
	 if_trasns_key = true;//传输密钥加密
	 ret =  api_import_key (&ukey,&inkey, if_cipher, if_trasns_key);//导入密钥
	 if(ret!=SE_SUCCESS)
	 {		 
		  LOGE("failed to api_connect\n");
		  return ret;
	 }
	 
	return ret;

}


/**
* @brief 密钥导入示例
* @param no
* @return 参见error.h
* @note 仅限于本地演示
* @see 函数 api_verify_pin api_set_transkey 
*/
se_error_t export_key_test(void)
{
	
	se_error_t ret = SE_SUCCESS;
    bool if_cipher = false;
	bool if_trasns_key = false;
	unikey_t ukey = {0};
	unikey_t exkey = {0};
	uint32_t i = 0;
	pub_key_t pub_key={0};
	pri_key_t pri_key={0};
	sym_key_t symkey ={0};

    /****************************************生成SM4临时对称密钥*****************************************/	 
    //生成kid为0xF5的SM4密钥
	symkey.alg = ALG_SM4;
	symkey.id	= 0xF5;
	ret = api_generate_symkey (&symkey);
	if ( ret != SE_SUCCESS)
	{
		LOGE("failed to generate_symkey_test\n");
		return ret;
	}
	
	/****************************************导出SM4临时对称密钥*****************************************/	 

	//1、明文导出SM4临时对称密钥
	 exkey.alg = ALG_SM4;
	 exkey.id = 0xF5;
	 exkey.type = SYM;
	 if_cipher = false;//明文导出
	 if_trasns_key = false;
	 ret =  api_export_key (&ukey,&exkey, if_cipher, if_trasns_key);//导出密钥
	 if(ret!=SE_SUCCESS)
	 {		 
		  LOGE("failed to api_connect\n");
		  return ret;
	 }
     port_printf("export pub_key:\n");
	 for(i = 0;i<exkey.val_len;i++)
	 {
		port_printf("%02x",exkey.val[i]);
	 }
	 port_printf("\n");
 
	 //2、传输密钥加密，密文导出SM4临时对称密钥
	 exkey.alg = ALG_SM4;
	 exkey.id = 0xF5;
	 exkey.type = SYM;
	 if_cipher = true;//密文导出
	  if_trasns_key = true;
	 ret =  api_export_key (&ukey,&exkey, if_cipher, if_trasns_key);//导出密钥
	 if(ret!=SE_SUCCESS)
	 {		 
		  LOGE("failed to api_connect\n");
		  return ret;
	 }
     port_printf("export pub_key:\n");
	 for(i = 0;i<exkey.val_len;i++)
	 {
		port_printf("%02x",exkey.val[i]);
	 }
	 port_printf("\n");


	 //3、SM4应用密钥加密，密文导出SM4临时对称密钥
	 exkey.alg = ALG_SM4;
	 exkey.id = 0xF5;
	 exkey.type = SYM;
	 ukey.alg = ALG_SM4;
	 ukey.id = 0x06;
	 if_cipher = true;//密文导出
	 if_trasns_key = false;//应用密钥加密导出
	 
	 ret =  api_export_key (&ukey,&exkey, if_cipher, if_trasns_key);//导出密钥 
	 if(ret!=SE_SUCCESS)
	 {		 
		  LOGE("failed to api_connect\n");
		  return ret;
	 }
	 port_printf("export pub_key:\n");
	 for(i = 0;i<exkey.val_len;i++)
	 {
		port_printf("%02x",exkey.val[i]);
	 }
	 port_printf("\n");
	 
	
     /****************************************生成RSA1024临时非对称密钥*****************************************/	 
    //生成公钥kid为0xF0,私钥kid为0xF1的RSA1024密钥对
	pub_key.alg = ALG_RSA1024_STANDAND;
	pub_key.id	= 0xF0;
	pri_key.id	= 0xF1;
	ret = api_generate_keypair (&pub_key, &pri_key);
	if ( ret != SE_SUCCESS)
	{
		LOGE("failed to generate_keypair_test\n");
		return ret;
	}
	for(i = 0;i<pub_key.val_len;i++)
	{
		port_printf("%02x",pub_key.val[i]);
	}
	port_printf("\n");
	
	/****************************************导出RSA1024临时非对称密钥*****************************************/	 

	//1、明文导出RSA1024临时私钥
	 exkey.alg = ALG_RSA1024_STANDAND;
	 exkey.id = 0xF1;
	 exkey.type = PRI;
	 if_cipher = false;//明文导出
	 if_trasns_key = false;
	 ret =  api_export_key (&ukey,&exkey, if_cipher, if_trasns_key);//导出密钥
	 if(ret!=SE_SUCCESS)
	 {		 
		  LOGE("failed to api_connect\n");
		  return ret;
	 }
     port_printf("export pub_key:\n");
	 for(i = 0;i<exkey.val_len;i++)
	 {
		port_printf("%02x",exkey.val[i]);
	 }
	 port_printf("\n");
 
	 //2、传输密钥加密，密文导出RSA1024临时私钥
	 exkey.alg = ALG_RSA1024_STANDAND;
	 exkey.id = 0xF1;
	 exkey.type = PRI;
	 if_cipher = true;//密文导出
	  if_trasns_key = true;
	 ret =  api_export_key (&ukey,&exkey, if_cipher, if_trasns_key);//导出密钥
	 if(ret!=SE_SUCCESS)
	 {		 
		  LOGE("failed to api_connect\n");
		  return ret;
	 }
     port_printf("export pub_key:\n");
	 for(i = 0;i<exkey.val_len;i++)
	 {
		port_printf("%02x",exkey.val[i]);
	 }
	 port_printf("\n");

     //3、明文导出RSA1024临时公钥
	 exkey.alg = ALG_RSA1024_CRT;
	 exkey.id = 0xF0;
	 exkey.type = PUB;
	 if_cipher = false;//明文导出
	 if_trasns_key = false;
	 ret =  api_export_key (&ukey,&exkey, if_cipher, if_trasns_key);//导出密钥
	 if(ret!=SE_SUCCESS)
	 {		 
		  LOGE("failed to api_connect\n");
		  return ret;
	 }
     port_printf("export pub_key:\n");
	 for(i = 0;i<exkey.val_len;i++)
	 {
		port_printf("%02x",exkey.val[i]);
	 }
	 port_printf("\n");
	
	 /****************************************导出RSA1024固定非对称密钥*****************************************/	 

	//1、明文导出RSA1024固定公钥
	 exkey.alg = ALG_RSA1024_STANDAND;
	 exkey.id = 0x0B;
	 exkey.type = PUB;
	 if_cipher = false;//明文导出
	 if_trasns_key = false;
	 ret =  api_export_key (&ukey,&exkey, if_cipher, if_trasns_key);//导出密钥
	 if(ret!=SE_SUCCESS)
	 {		 
		  LOGE("failed to api_connect\n");
		  return ret;
	 }
     port_printf("export pub_key:\n");
	 for(i = 0;i<exkey.val_len;i++)
	 {
		port_printf("%02x",exkey.val[i]);
	 }
	 port_printf("\n");

	
	 /****************************************生成SM2临时非对称密钥*****************************************/	   
	  //生成公钥kid为0xF0,私钥kid为0xF1的SM2密钥对
	  pub_key.alg = ALG_SM2;
	  pub_key.id  = 0xF0;
	  pri_key.id  = 0xF1;
	  ret = api_generate_keypair (&pub_key, &pri_key);
	  if ( ret != SE_SUCCESS)
	  {
		  LOGE("failed to generate_keypair_test\n");
		  return ret;
	  }
	  for(i = 0;i<pub_key.val_len;i++)
	  {
		  port_printf("%02x",pub_key.val[i]);
	  }
	  port_printf("\n");
	  
	  /****************************************导出SM2临时非对称密钥*****************************************/    
	 
	  //1、明文导出SM2临时私钥
	   exkey.alg = ALG_SM2;
	   exkey.id = 0xF1;
	   exkey.type = PRI;
	   if_cipher = false;//明文导出
	   if_trasns_key = false;
	   ret =  api_export_key (&ukey,&exkey, if_cipher, if_trasns_key);//导出密钥
	   if(ret!=SE_SUCCESS)
	   {	   
			LOGE("failed to api_connect\n");
			return ret;
	   }
	   port_printf("export pub_key:\n");
	   for(i = 0;i<exkey.val_len;i++)
	   {
		  port_printf("%02x",exkey.val[i]);
	   }
	   port_printf("\n");
	 
	   //2、传输密钥加密，密文导出SM2临时私钥
	   exkey.alg = ALG_SM2;
	   exkey.id = 0xF1;
	   exkey.type = PRI;
	   if_cipher = true;//密文导出
		if_trasns_key = true;
	   ret =  api_export_key (&ukey,&exkey, if_cipher, if_trasns_key);//导出密钥
	   if(ret!=SE_SUCCESS)
	   {	   
			LOGE("failed to api_connect\n");
			return ret;
	   }
	   port_printf("export pub_key:\n");
	   for(i = 0;i<exkey.val_len;i++)
	   {
		  port_printf("%02x",exkey.val[i]);
	   }
	   port_printf("\n");
	 
	   //3、明文导出SM2临时公钥
	   exkey.alg = ALG_SM2;
	   exkey.id = 0xF0;
	   exkey.type = PUB;
	   if_cipher = false;//明文导出
	   if_trasns_key = false;
	   ret =  api_export_key (&ukey,&exkey, if_cipher, if_trasns_key);//导出密钥
	   if(ret!=SE_SUCCESS)
	   {	   
			LOGE("failed to api_connect\n");
			return ret;
	   }
	   port_printf("export pub_key:\n");
	   for(i = 0;i<exkey.val_len;i++)
	   {
		  port_printf("%02x",exkey.val[i]);
	   }
	   port_printf("\n");
	  
	   /****************************************导出SM2固定非对称密钥*****************************************/   
	 
	  //1、明文导出SM2固定公钥
	   exkey.alg = ALG_SM2;
	   exkey.id = 0x0F;
	   exkey.type = PUB;
	   if_cipher = false;//明文导出
	   if_trasns_key = false;
	   ret =  api_export_key (&ukey,&exkey, if_cipher, if_trasns_key);//导出密钥
	   if(ret!=SE_SUCCESS)
	   {	   
			LOGE("failed to api_connect\n");
			return ret;
	   }
	   port_printf("export pub_key:\n");
	   for(i = 0;i<exkey.val_len;i++)
	   {
		  port_printf("%02x",exkey.val[i]);
	   }
	   port_printf("\n");


	   
     /****************************************生成ECC临时非对称密钥*****************************************/	   
	  //生成公钥kid为0xF0,私钥kid为0xF1的ECC密钥对
	  pub_key.alg = ALG_ECC256_NIST;
	  pub_key.id  = 0xF0;
	  pri_key.id  = 0xF1;
	  ret = api_generate_keypair (&pub_key, &pri_key);
	  if ( ret != SE_SUCCESS)
	  {
		  LOGE("failed to generate_keypair_test\n");
		  return ret;
	  }
	  for(i = 0;i<pub_key.val_len;i++)
	  {
		  port_printf("%02x",pub_key.val[i]);
	  }
	  port_printf("\n");
	  
	  /****************************************导出ECC临时非对称密钥*****************************************/    
	 
	  //1、明文导出ECC临时私钥
	   exkey.alg = ALG_ECC256_NIST;
	   exkey.id = 0xF1;
	   exkey.type = PRI;
	   if_cipher = false;//明文导出
	   if_trasns_key = false;
	   ret =  api_export_key (&ukey,&exkey, if_cipher, if_trasns_key);//导出密钥
	   if(ret!=SE_SUCCESS)
	   {	   
			LOGE("failed to api_connect\n");
			return ret;
	   }
	   port_printf("export pub_key:\n");
	   for(i = 0;i<exkey.val_len;i++)
	   {
		  port_printf("%02x",exkey.val[i]);
	   }
	   port_printf("\n");
	 
	   //2、传输密钥加密，密文导出ECC临时私钥
	   exkey.alg = ALG_ECC256_NIST;
	   exkey.id = 0xF1;
	   exkey.type = PRI;
	   if_cipher = true;//密文导出
		if_trasns_key = true;
	   ret =  api_export_key (&ukey,&exkey, if_cipher, if_trasns_key);//导出密钥
	   if(ret!=SE_SUCCESS)
	   {	   
			LOGE("failed to api_connect\n");
			return ret;
	   }
	   port_printf("export pub_key:\n");
	   for(i = 0;i<exkey.val_len;i++)
	   {
		  port_printf("%02x",exkey.val[i]);
	   }
	   port_printf("\n");
	 
	   //3、明文导出ECC临时公钥
	   exkey.alg = ALG_ECC256_NIST;
	   exkey.id = 0xF0;
	   exkey.type = PUB;
	   if_cipher = false;//明文导出
	   if_trasns_key = false;
	   ret =  api_export_key (&ukey,&exkey, if_cipher, if_trasns_key);//导出密钥
	   if(ret!=SE_SUCCESS)
	   {	   
			LOGE("failed to api_connect\n");
			return ret;
	   }
	   port_printf("export pub_key:\n");
	   for(i = 0;i<exkey.val_len;i++)
	   {
		  port_printf("%02x",exkey.val[i]);
	   }
	   port_printf("\n");
	  
	   /****************************************导出ECC固定非对称密钥*****************************************/   
	 
	  //1、明文导出ECC固定公钥
	   exkey.alg = ALG_ECC256_NIST;
	   exkey.id = 0x09;
	   exkey.type = PUB;
	   if_cipher = false;//明文导出
	   if_trasns_key = false;
	   ret =  api_export_key (&ukey,&exkey, if_cipher, if_trasns_key);//导出密钥
	   if(ret!=SE_SUCCESS)
	   {	   
			LOGE("failed to api_connect\n");
			return ret;
	   }
	   port_printf("export pub_key:\n");
	   for(i = 0;i<exkey.val_len;i++)
	   {
		  port_printf("%02x",exkey.val[i]);
	   }
	   port_printf("\n");
       
	 return ret;

}



/**
* @brief 获取密钥信息示例
* @param no
* @return 参见error.h
* @note 仅限于本地演示
* @see 函数 api_get_key_info 
*/
se_error_t get_key_info_test(void)
{
	
	
	se_error_t ret = SE_SUCCESS;
	uint8_t out_buf[512]={0};
	uint32_t out_buf_len;
	bool if_app_key = true;
    uint32_t i = 0;
	
	//获取应用密钥信息
    ret =api_get_key_info(if_app_key, out_buf,&out_buf_len);
	if ( ret != SE_SUCCESS)
	{
		LOGE("failed to auth_test!\n");
		return ret;
	}
	for(i = 0;i<out_buf_len;i++)
	{
		port_printf("%02x",out_buf[i]);
	}
	port_printf("\n");

    //获取管理密钥信息
    if_app_key = false;
    ret =api_get_key_info(true, out_buf,&out_buf_len);
	if ( ret != SE_SUCCESS)
	{
		LOGE("failed to auth_test!\n");
		return ret;
	}
	for(i = 0;i<out_buf_len;i++)
	{
		port_printf("%02x",out_buf[i]);
	}
	port_printf("\n");
	return ret;

}



/**
* @brief 获取密钥信息示例
* @param no
* @return 参见error.h
* @note 仅限于本地演示
* @see 函数 api_get_key_info 
*/
se_error_t del_key_test(void)
{
	
	
	se_error_t ret = SE_SUCCESS;
	uint8_t out_buf[512]={0};
	uint32_t out_buf_len;
    uint32_t i = 0;
   //获取应用密钥信息，kid 0x93的密钥存在
    ret =api_get_key_info(true, out_buf,&out_buf_len);
	if ( ret != SE_SUCCESS)
	{
		LOGE("failed to auth_test!\n");
		return ret;
	}
	for(i = 0;i<out_buf_len;i++)
	{
		port_printf("%02x",out_buf[i]);
	}
	port_printf("\n");

	
	//删除kid为0x93的密钥
    ret =api_del_key(0x93);
	if ( ret != SE_SUCCESS)
	{
		LOGE("failed to auth_test!\n");
		return ret;
	}
	
    //获取应用密钥信息，kid 0x93的密钥已经删除
    ret =api_get_key_info(true, out_buf,&out_buf_len);
	if ( ret != SE_SUCCESS)
	{
		LOGE("failed to auth_test!\n");
		return ret;
	}
	for(i = 0;i<out_buf_len;i++)
	{
		port_printf("%02x",out_buf[i]);
	}
	port_printf("\n");
	return ret;

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

