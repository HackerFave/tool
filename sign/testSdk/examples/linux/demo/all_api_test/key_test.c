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
* @brief ������Կ�ԡ�������Կ��������Կ��ʾ��
* @param no
* @return �μ�error.h
* @note no
* @see ���� update_mkey_test set_transkey_test generate_keypair_test
*/ 
se_error_t key_test (void)
{
	/*
	1������������Կ����update_mkey_test
	2�����ô�����Կ����set_transkey_test
	3�����ɷǶԳ���Կ�Բ���generate_keypair_test
	4�����ɶԳ���Կ�Բ���generate_symkey_test
	5����ԿЭ�̲���generate_shared_key_test
	6��������Կ����import_key_test
	7��������Կ����export_key_test
	8��ɾ����Կ����del_key_test
	9����ȡ��Կ��Ϣ����get_key_info_test
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
* @brief �����豸������Կʾ��
* @param no
* @return �μ�error.h
* @note �����ڱ�����ʾ
* @see ���� api_ext_auth api_update_mkey 
*/
se_error_t update_mkey_test(void)
{
	
	/*
	1������豸Ȩ��
	2������������Կ 
	3��ʹ���µ�������Կ����豸Ȩ��
	4������Ϊԭ������Կ
	*/
	se_error_t ret = SE_SUCCESS;
	uint8_t mkey_old[16]={0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4A,0x4B,0x4C,0x4D,0x4E,0x4F};    
	uint8_t mkey_new[16]={0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5A,0x5B,0x5C,0x5D,0x5E,0x5F};
	uint8_t random[16]={0};
    sym_key_t mkey={0};
	uint8_t out_buf[16]={0};
	uint32_t out_buf_len;
	
	//1������豸Ȩ��
	//��ȡ16�ֽ������
    api_get_random (0x10, random);
    //�����������SM4���ܻ����֤����
	ex_sm4_enc_dec(random,0x10,mkey_old,0x10,ECB,ENCRYPT,out_buf,&out_buf_len);
	//�����豸��֤�ӿ�
     ret = api_ext_auth(out_buf,out_buf_len);
    if ( ret != SE_SUCCESS)
	{
		LOGE("failed to auth_test!\n");
		return ret;
	}
	

	//2������������Կ
	memcpy(mkey.val, mkey_new, 16);
	mkey.val_len = 16;
    ret =api_update_mkey(&mkey, false);
	if ( ret != SE_SUCCESS)
	{
		LOGE("failed to auth_test!\n");
		return ret;
	}


	//3��ʹ���µ�������Կ����豸Ȩ��
	//��ȡ16�ֽ������
    api_get_random (0x10, random);
    //�����������SM4���ܻ����֤����
	ex_sm4_enc_dec(random,0x10,mkey_new,0x10,ECB,ENCRYPT,out_buf,&out_buf_len);
	//�����豸��֤�ӿ�
     ret = api_ext_auth(out_buf,out_buf_len);
    if ( ret != SE_SUCCESS)
	{
		LOGE("failed to auth_test!\n");
		return ret;
	}

	
	//4������Ϊԭ������Կ
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
* @brief ���ô�����Կʾ��
* @param no
* @return �μ�error.h
* @note �����ڱ�����ʾ
* @see ���� api_verify_pin api_set_transkey 
*/
se_error_t set_transkey_test(void)
{
	
	/*
	1����ù���ԱȨ�޸���Ϊ�´�����Կ 
	2����ù���ԱȨ�޸���Ϊԭ������Կ
	*/
	se_error_t ret = SE_SUCCESS;
	uint8_t transk_old[16]={0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4A,0x4B,0x4C,0x4D,0x4E,0x4F};    
	uint8_t transk_new[16]={0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5A,0x5B,0x5C,0x5D,0x5E,0x5F};
    sym_key_t trankey={0};
    uint8_t pin_buf[16] = {0x12,0x34,0x56,0x78,0x9A,0xBC,0xDE,0xF0,0x12,0x34,0x56,0x78,0x9A,0xBC,0xDE,0xF0};
    pin_t pin={0};
	
	//1����ù���ԱȨ�޸���Ϊ�´�����Կ 
	 memcpy(trankey.val,transk_new,16);
	 trankey.val_len = 16;
	 trankey.id = 0x02;//������Կkid����Ϊ0x02
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


	//2����ù���ԱȨ�޸���Ϊԭ������Կ 
	 memcpy(trankey.val,transk_old,16);
	 trankey.val_len = 16;
	 trankey.id = 0x02;//������Կkid����Ϊ0x02
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
    
	return ret;

}



/**
* @brief ������Կ��ʾ��
* @param no
* @return �μ�error.h
* @note no
* @see ����api_verify_pin  api_generate_keypair
*/
se_error_t generate_keypair_test (void)
{
	/*
	1����֤����Աpin
	2��������Կ��
	*/

	se_error_t ret = 0;
	pub_key_t pub_key={0};
	pri_key_t pri_key={0};
	uint8_t pin_buf[16] = {0x12,0x34,0x56,0x78,0x9A,0xBC,0xDE,0xF0,0x12,0x34,0x56,0x78,0x9A,0xBC,0xDE,0xF0};
    pin_t pin={0};
	uint32_t i = 0;

	//��֤����Աpin
	 pin.owner = ADMIN_PIN;
	 pin.pin_len = 0x10;
	 memcpy(pin.pin_value, pin_buf,pin.pin_len);
	 ret = api_verify_pin(&pin);//��֤����Աpin
	 if(ret!=SE_SUCCESS)
	 { 	  
	   	  LOGE("failed to api_verify_pin\n");
	   	  return ret;
	 }
	 
	/****************************************generate key pair RSA1024 test*****************************************/
		
		//���ɹ�ԿkidΪ0x90,˽ԿkidΪ0x91��RSA1024��Կ��
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
	//���ɹ�˽Կkid��Ϊ0x60��RSA2048��Կ��
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
	//�����㷨����
	
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
* @brief �����Գ���Կʾ��
* @param no
* @return �μ�error.h
* @note no
* @see ����api_verify_pin  api_generate_symkey 
*/
se_error_t generate_symkey_test (void)
{
	/*
	1����֤����Աpin
	2�������Գ���Կ
	*/

	se_error_t ret = 0;
	sym_key_t symkey ={0};
	uint8_t pin_buf[16] = {0x12,0x34,0x56,0x78,0x9A,0xBC,0xDE,0xF0,0x12,0x34,0x56,0x78,0x9A,0xBC,0xDE,0xF0};
    pin_t pin={0};
	//��ȡ16�ֽ������
	 pin.owner = ADMIN_PIN;
	 pin.pin_len = 0x10;
	 memcpy(pin.pin_value, pin_buf,pin.pin_len);
	 ret = api_verify_pin(&pin);//���ɹ̶���Կ��Ҫ��ð�ȫ�ļ���дȨ�ޣ���֤����Աpin
	 if(ret!=SE_SUCCESS)
	 { 	  
	   	  LOGE("failed to api_connect\n");
	   	  return ret;
	 }
	 
	/****************************************generate symkey key 3DES test*****************************************/
		
		//����kidΪ0x92��3DES��Կ
		symkey.alg = ALG_DES128;
		symkey.id	= 0x92;
		ret = api_generate_symkey (&symkey);
		if ( ret != SE_SUCCESS)
		{
			LOGE("failed to generate_symkey_test\n");
			return ret;
		}
		
	
   /****************************************generate symkey key AES test*****************************************/
		
		//����kidΪ0x93��AES��Կ
		symkey.alg = ALG_AES128;
		symkey.id	= 0x93;
		ret = api_generate_symkey (&symkey);
		if ( ret != SE_SUCCESS)
		{
			LOGE("failed to generate_symkey_test\n");
			return ret;
		}

  /****************************************generate symkey key SM4 test*****************************************/
		
		//����kidΪ0x94��SM4��Կ
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
* @brief ����Э����Կʾ��
* @param no
* @return �μ�error.h
* @note no
* @see ���� api_generate_shared_key
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
	//ECC��Կ
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
	/****************************************ECDH SM2����*****************************************/	
	//SM2��Կ
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
	//������ʱ��˽Կ��

	pub_key.alg = ALG_SM2;
	pub_key.id  = 0xF0;
    pri_key.id  = 0xF1;
	ret = api_generate_keypair (&pub_key, &pri_key);
	if ( ret != SE_SUCCESS)
	{
		LOGE("failed to generate_keypair_test\n");
		return ret;
	}
	//Э�̹������
	input[0] = 0x01;
	//��ʱ�Գ���Կ����Կ��ʶ����1�ֽڣ�
	input[1] = 0xF7;
	//�㷨����SM4
	input[2] = ALG_SM4;
	//��ʱ��˽Կ���̶���˽Կ
	memcpy(input+3,"\xF0\xF1\x0F\x0F",4);
	//�Է���ʱ��Կ
	memcpy(input+7,pub_key.val,64);
	//�Է����й�Կ
	strcpy(s_key_info,"160E12897DF4EDB61DD812FEB96748FBD3CCF4FFE26AA6F6DB9540AF49C942324A7DAD08BB9A459531694BEB20AA489D6649975E1BFCF8C4741B78B4B223007F");
	s_key_info_len = 128;
	StringToByte(s_key_info,input+71,s_key_info_len);
	input_len = s_key_info_len/2;
	memcpy(input+135,"\x01\xAA\x01\xBB",4);//˫��ID
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
* @brief ��Կ����ʾ��
* @param no
* @return �μ�error.h
* @note �����ڱ�����ʾ
* @see ���� api_verify_pin api_set_transkey 
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
	ret = api_verify_pin(&pin);//����̶���Կ������֤����Աpin
	if(ret!=SE_SUCCESS)
	{	  
		LOGE("failed to api_connect\n");
		return ret;
	}


	/****************************************����SM4�Գ���Կ*****************************************/	 

	//1�����ĵ���SM4�Գ���Կ
	 
	 inkey.alg = ALG_SM4;
	 inkey.id = 0x11;
	 inkey.type = SYM;
	 inkey.val_len = 0x10;
	 ukey.alg = NULL;
	 ukey.id = NULL;
     memset(inkey.val,0x00,16);
	 if_cipher = false;//���ĵ���
	 if_trasns_key = false;
	 ret =  api_import_key (&ukey,&inkey, if_cipher, if_trasns_key);//������Կ
	 if(ret!=SE_SUCCESS)
	 {		 
		  LOGE("failed to api_connect\n");
		  return ret;
	 }


	 
	 //2��������Կ���ܣ����ĵ���SM4�Գ���Կ
	 inkey.alg = ALG_SM4;
	 inkey.id = 0x11;
	 inkey.type = SYM;
	 inkey.val_len = 0x10;
	 ukey.alg = NULL;
	 ukey.id = NULL;
     memset(inkey.val,0x01,16);//��������ԿֵΪ��01010101010101010101010101010101��
	 if_cipher = true;//���ĵ���
	  if_trasns_key = true;
	 ret =  api_import_key (&ukey,&inkey, if_cipher, if_trasns_key);//������Կ
	 if(ret!=SE_SUCCESS)
	 {		 
		  LOGE("failed to api_connect\n");
		  return ret;
	 }



	 //3��SM4Ӧ����Կ���ܣ����ĵ���SM4�Գ���Կ
	 inkey.alg = ALG_SM4;
	 inkey.id = 0x11;
	 ukey.alg = ALG_SM4;
	 ukey.id = 0x06;
	 inkey.type = SYM;
	  inkey.val_len = 0x20;//����֮��ĳ���Ϊ32�ֽ�
	 if_cipher = true;//���ĵ���
	 if_trasns_key = false;//Ӧ����Կ���ܵ���
	 
     //��Կֵ����kid 0x06 SM4Ӧ����Կ����Ϊ���ġ� 
     memset(appkey,0x00,16);//Ӧ����ԿֵΪ ��00000000000000000000000000000000�� 

	 //������ֽ�Ld������������Կ�����ĳ���
	 in_buf[0] = 0x00;
	 in_buf[1] = 0x10;

	 //��������Կֵ����inbuf����ԿֵΪ��02020202020202020202020202020202��
	 memset(in_buf+2, 0x02,16);

	 //��1�ֽڡ�80��
	 in_buf[18] = 0x80;
	 //����00��ֱ��Ϊ16�ֽ�������
	 memset(in_buf+19,0x00,14);
	 //����Ӧ����Կ����������Կֵ
	 ex_sm4_enc_dec(in_buf,32,appkey,16,ECB,ENCRYPT,inkey.val,&inkey.val_len );//����֮������ݷ���inkey.val

	 ret =  api_import_key (&ukey,&inkey, if_cipher, if_trasns_key);//������Կ
	 if(ret!=SE_SUCCESS)
	 {		 
		  LOGE("failed to api_connect\n");
		  return ret;
	 }



	  //4��RSA1024�ǶԳ�Ӧ����Կ���ܣ����ĵ���SM4�Գ���Կ
     //�����SM4��ԿΪ��02020202020202020202020202020202��������pkcs1��λ�����RSA1024����֮�������Ϊ����1462DE010291E899AA2916447B4289B2858A3A71AB587546CC87431BF6022882752C56CE5555FB997F282020F2202FECAD025D15A8F2FE958513C66A8C7976818010A64682D79F17974D12FA03B1DCAE4947E73166FA77D949382C0A80243602AAF6635E3B256CCD77B802959B69DEA1B5DFF434871A6821040CD2850EE207AC��
	 strcpy(s_key_cipher,"1462DE010291E899AA2916447B4289B2858A3A71AB587546CC87431BF6022882752C56CE5555FB997F282020F2202FECAD025D15A8F2FE958513C66A8C7976818010A64682D79F17974D12FA03B1DCAE4947E73166FA77D949382C0A80243602AAF6635E3B256CCD77B802959B69DEA1B5DFF434871A6821040CD2850EE207AC");
	 s_key_cipher_len = 256;
	 StringToByte(s_key_cipher,inkey.val,s_key_cipher_len);
	 
	 inkey.alg = ALG_SM4;
	 inkey.id = 0x11;
	 ukey.alg = ALG_RSA1024_CRT;
	 ukey.id = 0x0B;//RSA1024��˽Կkid
	 inkey.type = SYM;
	 inkey.val_len = 0x80;//����֮��ĳ���Ϊ128�ֽ�
	 if_cipher = true;//���ĵ���
	 if_trasns_key = false;//Ӧ����Կ���ܵ���
	 
	
	 ret =  api_import_key (&ukey,&inkey, if_cipher, if_trasns_key);//������Կ
	 if(ret!=SE_SUCCESS)
	 {		 
		  LOGE("failed to api_connect\n");
		  return ret;
	 }

     /****************************************����RSA1024�ǶԳ���Կ*****************************************/	 
     //1�����ĵ���RSA1024��Կ
	  strcpy(s_appkey,"00010001BB6105E798FDBC7CD6DC2567EB890E0AA8CEA22F2D9123D06E1D29651EE81E3559A59C5153BBAD44B45EEE986DC347C103E20FF027E7A8B1160511FA419C5C7C11FC34B0C65FB27CCF4477233B859515FAE8BF92427D1C59C82A823C8A44065D97500CEF02D7B284C524403E85E12AD8159680AB4CBCB0623C3B4C43A6E7D6BF");
	 s_appkey_len = 264;
	 StringToByte(s_appkey,inkey.val,s_appkey_len); 
	 inkey.alg = ALG_RSA1024_CRT;
	 inkey.id = 0x07;
	 inkey.type = PUB;
	 inkey.val_len = 0x84;
	 ukey.alg = NULL;
	 ukey.id = NULL;
	 if_cipher = false;//���ĵ���
	 if_trasns_key = false;
	 ret =  api_import_key (&ukey,&inkey, if_cipher, if_trasns_key);//������Կ
	 if(ret!=SE_SUCCESS)
	 {		 
		  LOGE("failed to api_connect\n");
		  return ret;
	 }

	  //2�����ĵ���RSA1024˽Կ
	  strcpy(s_appkey,"00010001F7A8BDCC97CEC14C42A4DFE652BCA194B0AF6452EACECD7BC68AEDBD4A9C765BDEDCD848E1DF4277555564B9ABAD49C44779D98AD1E798EE5695AFE4CA4B9FBFC1B08F7A9A7B4449EF52AFE2C4918173E1CC47C09CCCCEB173CE6884639FCEEB8773928CC9B7CDE499C899CD765B6FB54EB9505EECBC94D8A5EDA2BE53A18901B1F6E9DCB941F3994F52972A354E23766BB8BD88EFFE879E683FB133130877ECF0AB1628D956CBB660F5AA19DA09FE36337E961AB1579187C364712F214752959FE7FE6C8001689D89A1C067FD550B84FA53C6A13452BA0A32CBA3EA38BD84FD5A7485F709CA5F876D96F9B3711588222D7E3CE6419867627D8AB52360B42A01D180FBBD1E6CFD8262C3BEE4F3EA8E8D485F05CC694E245CAA3EFAFAD1E2D05D4EA9A5B939D8F1768EF4E0F4765E566B285A33BCDFBC7DCBCA506B780D95FBC0");
	 s_appkey_len = 648;
	 StringToByte(s_appkey,inkey.val,s_appkey_len); 
	 inkey.alg = ALG_RSA1024_CRT;
	 inkey.id = 0x14;
	 inkey.type = PRI;
	 inkey.val_len = 0x0144;
	 ukey.alg = NULL;
	 ukey.id = NULL;
	 if_cipher = false;//���ĵ���
	 if_trasns_key = false;
	 ret =  api_import_key (&ukey,&inkey, if_cipher, if_trasns_key);//������Կ
	 if(ret!=SE_SUCCESS)
	 {		 
		  LOGE("failed to api_connect\n");
		  return ret;
	 }

	 //3��������Կ���ܣ����ĵ���RSA1024��Կ
	  strcpy(s_appkey,"00010001BB6105E798FDBC7CD6DC2567EB890E0AA8CEA22F2D9123D06E1D29651EE81E3559A59C5153BBAD44B45EEE986DC347C103E20FF027E7A8B1160511FA419C5C7C11FC34B0C65FB27CCF4477233B859515FAE8BF92427D1C59C82A823C8A44065D97500CEF02D7B284C524403E85E12AD8159680AB4CBCB0623C3B4C43A6E7D6BF");
	 s_appkey_len = 264;
	 StringToByte(s_appkey,inkey.val,s_appkey_len); 
	 inkey.alg = ALG_RSA1024_CRT;
	 inkey.id = 0x07;
	 inkey.type = PUB;
	 inkey.val_len = 0x84;
	 ukey.alg = NULL;
	 ukey.id = NULL;
	 if_cipher = true;//���ĵ���
	 if_trasns_key = true;//������Կ����
	 ret =  api_import_key (&ukey,&inkey, if_cipher, if_trasns_key);//������Կ
	 if(ret!=SE_SUCCESS)
	 {		 
		  LOGE("failed to api_connect\n");
		  return ret;
	 }

	  //4��������Կ���ܣ����ĵ���RSA1024˽Կ
	  strcpy(s_appkey,"00010001F7A8BDCC97CEC14C42A4DFE652BCA194B0AF6452EACECD7BC68AEDBD4A9C765BDEDCD848E1DF4277555564B9ABAD49C44779D98AD1E798EE5695AFE4CA4B9FBFC1B08F7A9A7B4449EF52AFE2C4918173E1CC47C09CCCCEB173CE6884639FCEEB8773928CC9B7CDE499C899CD765B6FB54EB9505EECBC94D8A5EDA2BE53A18901B1F6E9DCB941F3994F52972A354E23766BB8BD88EFFE879E683FB133130877ECF0AB1628D956CBB660F5AA19DA09FE36337E961AB1579187C364712F214752959FE7FE6C8001689D89A1C067FD550B84FA53C6A13452BA0A32CBA3EA38BD84FD5A7485F709CA5F876D96F9B3711588222D7E3CE6419867627D8AB52360B42A01D180FBBD1E6CFD8262C3BEE4F3EA8E8D485F05CC694E245CAA3EFAFAD1E2D05D4EA9A5B939D8F1768EF4E0F4765E566B285A33BCDFBC7DCBCA506B780D95FBC0");
	 s_appkey_len = 648;
	 StringToByte(s_appkey,inkey.val,s_appkey_len); 
	 inkey.alg = ALG_RSA1024_CRT;
	 inkey.id = 0x14;
	 inkey.type = PRI;
	 inkey.val_len = 0x0144;
	 ukey.alg = NULL;
	 ukey.id = NULL;
	 if_cipher = true;//���ĵ���
	 if_trasns_key = true;//������Կ����
	 ret =  api_import_key (&ukey,&inkey, if_cipher, if_trasns_key);//������Կ
	 if(ret!=SE_SUCCESS)
	 {		 
		  LOGE("failed to api_connect\n");
		  return ret;
	 }
	 
	/****************************************����ECC256NIST�ǶԳ���Կ*****************************************/	 
     //1�����ĵ���ECC��Կ
	  strcpy(s_appkey,"160E12897DF4EDB61DD812FEB96748FBD3CCF4FFE26AA6F6DB9540AF49C942324A7DAD08BB9A459531694BEB20AA489D6649975E1BFCF8C4741B78B4B223007F");
	 s_appkey_len = 128;
	 StringToByte(s_appkey,inkey.val,s_appkey_len); 
	 inkey.alg = ALG_ECC256_NIST;
	 inkey.id = 0x07;
	 inkey.type = PUB;
	 inkey.val_len = 0x40;
	 ukey.alg = NULL;
	 ukey.id = NULL;
	 if_cipher = false;//���ĵ���
	 if_trasns_key = false;
	 ret =  api_import_key (&ukey,&inkey, if_cipher, if_trasns_key);//������Կ
	 if(ret!=SE_SUCCESS)
	 {		 
		  LOGE("failed to api_connect\n");
		  return ret;
	 }

	  //2�����ĵ���ECC˽Կ
	  strcpy(s_appkey,"81EB26E941BB5AF16DF116495F90695272AE2CD63D6C4AE1678418BE48230029");
	 s_appkey_len = 128;
	 StringToByte(s_appkey,inkey.val,s_appkey_len); 
	 inkey.alg = ALG_ECC256_NIST;
	 inkey.id = 0x14;
	 inkey.type = PRI;
	 inkey.val_len = 0x20;
	 ukey.alg = NULL;
	 ukey.id = NULL;
	 if_cipher = false;//���ĵ���
	 if_trasns_key = false;
	 ret =  api_import_key (&ukey,&inkey, if_cipher, if_trasns_key);//������Կ
	 if(ret!=SE_SUCCESS)
	 {		 
		  LOGE("failed to api_connect\n");
		  return ret;
	 }

	 //3��������Կ���ܣ����ĵ���ECC��Կ
	  strcpy(s_appkey,"160E12897DF4EDB61DD812FEB96748FBD3CCF4FFE26AA6F6DB9540AF49C942324A7DAD08BB9A459531694BEB20AA489D6649975E1BFCF8C4741B78B4B223007F");
	 s_appkey_len = 128;
	 StringToByte(s_appkey,inkey.val,s_appkey_len); 
	 inkey.alg = ALG_ECC256_NIST;
	 inkey.id = 0x07;
	 inkey.type = PUB;
	 inkey.val_len = 0x40;
	 ukey.alg = NULL;
	 ukey.id = NULL;
	 if_cipher = true;//���ĵ���
	 if_trasns_key = true;//������Կ����
	 ret =  api_import_key (&ukey,&inkey, if_cipher, if_trasns_key);//������Կ
	 if(ret!=SE_SUCCESS)
	 {		 
		  LOGE("failed to api_connect\n");
		  return ret;
	 }

	  //4��������Կ���ܣ����ĵ���ECC˽Կ
	  strcpy(s_appkey,"81EB26E941BB5AF16DF116495F90695272AE2CD63D6C4AE1678418BE48230029");
	 s_appkey_len = 128;
	 StringToByte(s_appkey,inkey.val,s_appkey_len); 
	 inkey.alg = ALG_ECC256_NIST;
	 inkey.id = 0x14;
	 inkey.type = PRI;
	 inkey.val_len = 0x20;
	 ukey.alg = NULL;
	 ukey.id = NULL;
	 if_cipher = true;//���ĵ���
	 if_trasns_key = true;//������Կ����
	 ret =  api_import_key (&ukey,&inkey, if_cipher, if_trasns_key);//������Կ
	 if(ret!=SE_SUCCESS)
	 {		 
		  LOGE("failed to api_connect\n");
		  return ret;
	 }
	 
	return ret;

}


/**
* @brief ��Կ����ʾ��
* @param no
* @return �μ�error.h
* @note �����ڱ�����ʾ
* @see ���� api_verify_pin api_set_transkey 
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

    /****************************************����SM4��ʱ�Գ���Կ*****************************************/	 
    //����kidΪ0xF5��SM4��Կ
	symkey.alg = ALG_SM4;
	symkey.id	= 0xF5;
	ret = api_generate_symkey (&symkey);
	if ( ret != SE_SUCCESS)
	{
		LOGE("failed to generate_symkey_test\n");
		return ret;
	}
	
	/****************************************����SM4��ʱ�Գ���Կ*****************************************/	 

	//1�����ĵ���SM4��ʱ�Գ���Կ
	 exkey.alg = ALG_SM4;
	 exkey.id = 0xF5;
	 exkey.type = SYM;
	 if_cipher = false;//���ĵ���
	 if_trasns_key = false;
	 ret =  api_export_key (&ukey,&exkey, if_cipher, if_trasns_key);//������Կ
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
 
	 //2��������Կ���ܣ����ĵ���SM4��ʱ�Գ���Կ
	 exkey.alg = ALG_SM4;
	 exkey.id = 0xF5;
	 exkey.type = SYM;
	 if_cipher = true;//���ĵ���
	  if_trasns_key = true;
	 ret =  api_export_key (&ukey,&exkey, if_cipher, if_trasns_key);//������Կ
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


	 //3��SM4Ӧ����Կ���ܣ����ĵ���SM4��ʱ�Գ���Կ
	 exkey.alg = ALG_SM4;
	 exkey.id = 0xF5;
	 exkey.type = SYM;
	 ukey.alg = ALG_SM4;
	 ukey.id = 0x06;
	 if_cipher = true;//���ĵ���
	 if_trasns_key = false;//Ӧ����Կ���ܵ���
	 
	 ret =  api_export_key (&ukey,&exkey, if_cipher, if_trasns_key);//������Կ 
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
	 
	
     /****************************************����RSA1024��ʱ�ǶԳ���Կ*****************************************/	 
    //���ɹ�ԿkidΪ0xF0,˽ԿkidΪ0xF1��RSA1024��Կ��
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
	
	/****************************************����RSA1024��ʱ�ǶԳ���Կ*****************************************/	 

	//1�����ĵ���RSA1024��ʱ˽Կ
	 exkey.alg = ALG_RSA1024_STANDAND;
	 exkey.id = 0xF1;
	 exkey.type = PRI;
	 if_cipher = false;//���ĵ���
	 if_trasns_key = false;
	 ret =  api_export_key (&ukey,&exkey, if_cipher, if_trasns_key);//������Կ
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
 
	 //2��������Կ���ܣ����ĵ���RSA1024��ʱ˽Կ
	 exkey.alg = ALG_RSA1024_STANDAND;
	 exkey.id = 0xF1;
	 exkey.type = PRI;
	 if_cipher = true;//���ĵ���
	  if_trasns_key = true;
	 ret =  api_export_key (&ukey,&exkey, if_cipher, if_trasns_key);//������Կ
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

     //3�����ĵ���RSA1024��ʱ��Կ
	 exkey.alg = ALG_RSA1024_CRT;
	 exkey.id = 0xF0;
	 exkey.type = PUB;
	 if_cipher = false;//���ĵ���
	 if_trasns_key = false;
	 ret =  api_export_key (&ukey,&exkey, if_cipher, if_trasns_key);//������Կ
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
	
	 /****************************************����RSA1024�̶��ǶԳ���Կ*****************************************/	 

	//1�����ĵ���RSA1024�̶���Կ
	 exkey.alg = ALG_RSA1024_STANDAND;
	 exkey.id = 0x0B;
	 exkey.type = PUB;
	 if_cipher = false;//���ĵ���
	 if_trasns_key = false;
	 ret =  api_export_key (&ukey,&exkey, if_cipher, if_trasns_key);//������Կ
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

	
	 /****************************************����SM2��ʱ�ǶԳ���Կ*****************************************/	   
	  //���ɹ�ԿkidΪ0xF0,˽ԿkidΪ0xF1��SM2��Կ��
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
	  
	  /****************************************����SM2��ʱ�ǶԳ���Կ*****************************************/    
	 
	  //1�����ĵ���SM2��ʱ˽Կ
	   exkey.alg = ALG_SM2;
	   exkey.id = 0xF1;
	   exkey.type = PRI;
	   if_cipher = false;//���ĵ���
	   if_trasns_key = false;
	   ret =  api_export_key (&ukey,&exkey, if_cipher, if_trasns_key);//������Կ
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
	 
	   //2��������Կ���ܣ����ĵ���SM2��ʱ˽Կ
	   exkey.alg = ALG_SM2;
	   exkey.id = 0xF1;
	   exkey.type = PRI;
	   if_cipher = true;//���ĵ���
		if_trasns_key = true;
	   ret =  api_export_key (&ukey,&exkey, if_cipher, if_trasns_key);//������Կ
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
	 
	   //3�����ĵ���SM2��ʱ��Կ
	   exkey.alg = ALG_SM2;
	   exkey.id = 0xF0;
	   exkey.type = PUB;
	   if_cipher = false;//���ĵ���
	   if_trasns_key = false;
	   ret =  api_export_key (&ukey,&exkey, if_cipher, if_trasns_key);//������Կ
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
	  
	   /****************************************����SM2�̶��ǶԳ���Կ*****************************************/   
	 
	  //1�����ĵ���SM2�̶���Կ
	   exkey.alg = ALG_SM2;
	   exkey.id = 0x0F;
	   exkey.type = PUB;
	   if_cipher = false;//���ĵ���
	   if_trasns_key = false;
	   ret =  api_export_key (&ukey,&exkey, if_cipher, if_trasns_key);//������Կ
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


	   
     /****************************************����ECC��ʱ�ǶԳ���Կ*****************************************/	   
	  //���ɹ�ԿkidΪ0xF0,˽ԿkidΪ0xF1��ECC��Կ��
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
	  
	  /****************************************����ECC��ʱ�ǶԳ���Կ*****************************************/    
	 
	  //1�����ĵ���ECC��ʱ˽Կ
	   exkey.alg = ALG_ECC256_NIST;
	   exkey.id = 0xF1;
	   exkey.type = PRI;
	   if_cipher = false;//���ĵ���
	   if_trasns_key = false;
	   ret =  api_export_key (&ukey,&exkey, if_cipher, if_trasns_key);//������Կ
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
	 
	   //2��������Կ���ܣ����ĵ���ECC��ʱ˽Կ
	   exkey.alg = ALG_ECC256_NIST;
	   exkey.id = 0xF1;
	   exkey.type = PRI;
	   if_cipher = true;//���ĵ���
		if_trasns_key = true;
	   ret =  api_export_key (&ukey,&exkey, if_cipher, if_trasns_key);//������Կ
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
	 
	   //3�����ĵ���ECC��ʱ��Կ
	   exkey.alg = ALG_ECC256_NIST;
	   exkey.id = 0xF0;
	   exkey.type = PUB;
	   if_cipher = false;//���ĵ���
	   if_trasns_key = false;
	   ret =  api_export_key (&ukey,&exkey, if_cipher, if_trasns_key);//������Կ
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
	  
	   /****************************************����ECC�̶��ǶԳ���Կ*****************************************/   
	 
	  //1�����ĵ���ECC�̶���Կ
	   exkey.alg = ALG_ECC256_NIST;
	   exkey.id = 0x09;
	   exkey.type = PUB;
	   if_cipher = false;//���ĵ���
	   if_trasns_key = false;
	   ret =  api_export_key (&ukey,&exkey, if_cipher, if_trasns_key);//������Կ
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
* @brief ��ȡ��Կ��Ϣʾ��
* @param no
* @return �μ�error.h
* @note �����ڱ�����ʾ
* @see ���� api_get_key_info 
*/
se_error_t get_key_info_test(void)
{
	
	
	se_error_t ret = SE_SUCCESS;
	uint8_t out_buf[512]={0};
	uint32_t out_buf_len;
	bool if_app_key = true;
    uint32_t i = 0;
	
	//��ȡӦ����Կ��Ϣ
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

    //��ȡ������Կ��Ϣ
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
* @brief ��ȡ��Կ��Ϣʾ��
* @param no
* @return �μ�error.h
* @note �����ڱ�����ʾ
* @see ���� api_get_key_info 
*/
se_error_t del_key_test(void)
{
	
	
	se_error_t ret = SE_SUCCESS;
	uint8_t out_buf[512]={0};
	uint32_t out_buf_len;
    uint32_t i = 0;
   //��ȡӦ����Կ��Ϣ��kid 0x93����Կ����
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

	
	//ɾ��kidΪ0x93����Կ
    ret =api_del_key(0x93);
	if ( ret != SE_SUCCESS)
	{
		LOGE("failed to auth_test!\n");
		return ret;
	}
	
    //��ȡӦ����Կ��Ϣ��kid 0x93����Կ�Ѿ�ɾ��
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

