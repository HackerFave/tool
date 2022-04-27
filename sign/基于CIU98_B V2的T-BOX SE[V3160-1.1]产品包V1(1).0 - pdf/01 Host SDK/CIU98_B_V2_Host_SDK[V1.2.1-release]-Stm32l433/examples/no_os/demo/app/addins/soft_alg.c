/**@file  soft_alg.c
* @brief  soft_alg interface declearation	 
* @author liuch
* @date  2020-06-02
* @version	V1.0
* @copyright  Copyright(C),CEC Huada Electronic Design Co.,Ltd.
*/

/***************************************************************************
* Include Header Files
***************************************************************************/
#include "soft_alg.h"

/** @addtogroup SE_APP_TEST
  * @{
  */


/** @addtogroup ADDINS
  * @{
  */


/** @defgroup SOFT_ALG SOFT_ALG
  * @brief soft_alg interface api.
  * @{
  */


/* Exported functions --------------------------------------------------------*/
/** @defgroup SOFT_ALG_Exported_Functions SOFT_ALG Exported Functions
  * @{
  */


/*************************************************
Function:	  fun_des_encryptbyte
Description:  des算法加密子函数
Input:	      in_buf               输入数据
		      in_buf_len           输入数据长度
		      key_buf              密钥
		      key_buf_len          密钥长度
		      iv_mode              IV模式 ECB or CBC
Output:	      out_buf              输出数据
		      out_buf_len          输出数据长度
Return:   	  参见error.h	
Others:	      no	
*************************************************/

uint32_t fun_des_encryptbyte(uint8_t *in_buf, uint32_t in_buf_len, uint8_t *key_buf, uint32_t key_buf_len, uint32_t iv_mode, uint8_t *out_buf, uint32_t *out_buf_len)
{
    uint32_t i = 0;
	uint32_t ptr = 0;
	uint8_t iv[8] = {0};
	uint8_t buf[8] = {0};

	mbedtls_des_context ctx;

	// Check para
	if((iv_mode != ICC_ECB) && (iv_mode != ICC_CBC))
	{
		return CER_MODE;                         // mode only is 0 or 1
	}
	if((in_buf_len % 8) != 0)
	{
		return CER_DATALEN;		                 // len of DATA must be the time of 8
	}
	if((iv_mode == ICC_CBC) && (in_buf_len < 16))
	{
		return CER_CBC;                          // len of DATA must be larger than 8
	}

	// Init seed
	if(iv_mode == ICC_CBC)
	{
		for(i = 0; i < 8; i++)
		{
			iv[i] = in_buf[i];
		}
		ptr = 8;
		in_buf_len = in_buf_len - 8;
	}
	else
	{
		ptr = 0;
	}
	*out_buf_len = in_buf_len;

	// Init & Set Key
	mbedtls_des_init(&ctx);
	if (key_buf_len == 8)	        // 64 bit
	{
		mbedtls_des_setkey_enc(&ctx, key_buf);
	}
	else
	{
		return CER_KEYLEN;		// len of KEY must be 8 byte
	}

	// Loop
	while(in_buf_len/8 != 0)
	{
		// get plain data & XOR
		for(i = ptr; i < ptr+8; i++)
		{
			buf[i-ptr] = in_buf[i];
		}

		if(iv_mode == ICC_CBC)
		{
			for(i = 0; i < 8; i++)
			{
				buf[i] = buf[i] ^ iv[i];
			}
		}

		// Encipher
		mbedtls_des_crypt_ecb(&ctx, buf, buf);

		if(iv_mode == ICC_CBC)
		{
			memcpy(iv, buf, 8);
		}

		// get result
		for(i = ptr; i < ptr+8; i++)
		{
			if(iv_mode == ICC_CBC)
			{
				out_buf[i-8] = buf[i%8];
			}
			else
			{
				out_buf[i] = buf[i%8];
			}
		}
		in_buf_len=in_buf_len-8;
	}

	mbedtls_des_free(&ctx);
	return EXCUTE_SUC;
}

/*************************************************
Function:	  fun_3des_encryptbyte
Description:  3des算法加密子函数
Input:	      in_buf               输入数据
			  in_buf_len           输入数据长度
			  key_buf              密钥
			  key_buf_len          密钥长度
			  iv_mode              IV模式 ECB or CBC
Output:	      out_buf              输出数据
			  out_buf_len          输出数据长度
Return:   	  参见error.h	
Others:	      no	
*************************************************/

uint32_t fun_3des_encryptbyte(uint8_t *in_buf, uint32_t in_buf_len, uint8_t*key_buf, uint32_t key_buf_len, uint32_t iv_mode, uint8_t *out_buf, uint32_t*out_buf_len)
{
	uint32_t i = 0;
	uint32_t ptr = 0;
	uint8_t iv[8] = {0};
	uint8_t buf[8] = {0};

	mbedtls_des3_context ctx3;

	// Check para
	if((iv_mode != ICC_ECB) && (iv_mode != ICC_CBC))
	{
		return CER_MODE;                         // mode only is 0 or 1
	}
	if((in_buf_len % 8) != 0)
	{
		return CER_DATALEN;		                 // len of DATA must be the time of 8
	}
	if((iv_mode == ICC_CBC) && (in_buf_len < 16))
	{
		return CER_CBC;                          // len of DATA must be larger than 8
	}

	// Init seed
	if(iv_mode == ICC_CBC)
	{
		for(i = 0; i < 8; i++)
		{
			iv[i] = in_buf[i];
		}
		ptr = 8;
		in_buf_len = in_buf_len - 8;
	}
	else
	{
		ptr = 0;
	}
	*out_buf_len = in_buf_len;

	// Init & Set Key
	mbedtls_des3_init(&ctx3);
	if (key_buf_len == 16)	        // 112 bit
	{
		mbedtls_des3_set2key_enc(&ctx3, key_buf);
	}
	else if (key_buf_len == 24)	// 168 bit
	{
		mbedtls_des3_set3key_enc(&ctx3, key_buf);
	}
	else
	{
		return CER_KEYLEN;		// len of KEY must be 16 bytes or 24 byte
	}

	// Loop
	while(in_buf_len/8 != 0)
	{
		// get plain data & XOR
		for(i = ptr; i < ptr+8; i++)
		{
			buf[i-ptr] = in_buf[i];
		}

		if(iv_mode == ICC_CBC)
		{
			for(i = 0; i < 8; i++)
			{
				buf[i] = buf[i] ^ iv[i];
			}
		}

		// Encipher
		mbedtls_des3_crypt_ecb(&ctx3, buf, buf);

		if(iv_mode == ICC_CBC)
		{
			memcpy(iv, buf, 8);
		}

		// get result
		for(i = ptr; i < ptr+8; i++)
		{
			if(iv_mode == ICC_CBC)
			{
				out_buf[i-8] = buf[i%8];
			}
			else
			{
				out_buf[i] = buf[i%8];
			}
		}
		in_buf_len = in_buf_len - 8;
		ptr = ptr + 8;
	}

	mbedtls_des3_free(&ctx3);
	return EXCUTE_SUC;

}




/*************************************************
Function:	  fun_3des_decryptbyte
Description:  3des算法加密子函数
Input:	      in_buf               输入数据
			  in_buf_len           输入数据长度
			  key_buf              密钥
			  key_buf_len          密钥长度
			  iv_mode              IV模式 ECB or CBC
Output:	      out_buf              输出数据
			  out_buf_len          输出数据长度
Return:   	  参见error.h	
Others:	      no	
*************************************************/
uint32_t fun_3des_decryptbyte(uint8_t *in_buf, uint32_t in_buf_len, uint8_t *key_buf, uint32_t key_buf_len, uint32_t iv_mode, uint8_t *out_buf, uint32_t*out_buf_len)
{
	uint32_t i = 0;
	uint32_t ptr = 0;
	uint8_t iv[8] = {0};
	uint8_t buf[8] = {0};

	mbedtls_des3_context ctx3;

	// Check para
	if((iv_mode != ICC_ECB) && (iv_mode != ICC_CBC))
	{
		return CER_MODE;                         // mode only is 0 or 1
	}
	if((in_buf_len % 8) != 0)
	{
		return CER_DATALEN;		                 // len of DATA must be the time of 8
	}
	if((iv_mode == ICC_CBC) && (in_buf_len < 16))
	{
		return CER_CBC;                          // len of DATA must be larger than 8
	}

	// Init seed
	if(iv_mode == ICC_CBC)
	{
		for(i = 0; i < 8; i++)
		{
			iv[i] = in_buf[i];
		}
		ptr = 8;
		in_buf_len = in_buf_len - 8;
	}
	else
	{
		ptr = 0;
	}
	*out_buf_len = in_buf_len;

	// Init & Set Key
	mbedtls_des3_init(&ctx3);
	if (key_buf_len == 16)	        // 112 bit
	{
		mbedtls_des3_set2key_dec(&ctx3, key_buf);
	}
	else if (key_buf_len == 24)	// 168 bit
	{
		mbedtls_des3_set3key_dec(&ctx3, key_buf);
	}
	else
	{
		return CER_KEYLEN;		// len of KEY must be 16 bytes or 24 byte
	}

	// Loop
	while(in_buf_len/8 != 0)
	{
		// get plain data & XOR
		for(i = ptr; i < ptr+8; i++)
		{
			buf[i-ptr] = in_buf[i];
		}

		if(iv_mode == ICC_CBC)
		{
			for(i = 0; i < 8; i++)
			{
				buf[i] = buf[i] ^ iv[i];
			}
		}

		// Encipher
		mbedtls_des3_crypt_ecb(&ctx3, buf, buf);

		if(iv_mode == ICC_CBC)
		{
			memcpy(iv, buf, 8);
		}

		// get result
		for(i = ptr; i < ptr+8; i++)
		{
			if(iv_mode == ICC_CBC)
			{
				out_buf[i-8] = buf[i%8];
			}
			else
			{
				out_buf[i] = buf[i%8];
			}
		}
		in_buf_len = in_buf_len - 8;
		ptr = ptr + 8;
	}

	mbedtls_des3_free(&ctx3);
	return EXCUTE_SUC;

}




/*************************************************
Function:	  fun_3des_macbyte
Description:  3des MAC子函数
Input:	      in_buf               输入数据
			  in_buf_len           输入数据长度
			  key_buf              密钥
			  key_buf_len          密钥长度
Output:	      out_buf              输出数据
			  out_buf_len          输出数据长度
Return:   	  参见error.h	
Others:	      no
*************************************************/
uint32_t fun_3des_macbyte(uint8_t *in_buf, uint32_t in_buf_len, uint8_t *key_buf, uint32_t key_buf_len, uint8_t *out_buf, uint32_t *out_buf_len)
{
	uint32_t i = 0;
	uint32_t ptr = 0;
	uint8_t iv[8] = {0};
	uint8_t buf[8] = {0};

	mbedtls_des_context ctx;

	// Check para
	if(in_buf_len % 8 != 0)
	{
		return CER_DATALEN;		// len of DATA must be the time of 8
	}
	if(in_buf_len < 16)
	{
		return CER_CBC;        // len of DATA must be larger than 16
	}

	// Init seed
	for(i = 0; i < 8; i++)
	{
		iv[i] = in_buf[i];
	}
	in_buf_len = in_buf_len - 8;
	ptr = 8;

	// Init
	mbedtls_des_init(&ctx);

	// Loop XOR & Encipher, use K1
	mbedtls_des_setkey_enc(&ctx, key_buf);
	while(in_buf_len/8 != 0)
	{
		// get plain data
		for(i = ptr; i < ptr+8; i++)
		{
			buf[i-ptr] = in_buf[i];
		}
		// XOR
		for(i = 0; i < 8; i++)
		{
			buf[i] = buf[i] ^ iv[i];
		}

		// Encipher, use K1
		mbedtls_des_crypt_ecb(&ctx, buf, buf);

		memcpy(iv, buf, 8);
		in_buf_len = in_buf_len - 8;
		ptr = ptr + 8;
	}

	// Decipher, use K2
	mbedtls_des_setkey_dec(&ctx, key_buf + 8);
	mbedtls_des_crypt_ecb(&ctx, buf, buf);

	// Encipher
	if (key_buf_len == 16)	        // 112 bit, use K1
	{
		mbedtls_des_setkey_enc(&ctx, key_buf);
	}
	else if (key_buf_len == 24)	// 168 bit, use K3
	{
		mbedtls_des_setkey_enc(&ctx, key_buf + 16);
	}
	else
	{
		return CER_KEYLEN;		// len of KEY must be 16 bytes or 24 byte
	}
	mbedtls_des_crypt_ecb(&ctx, buf, buf);

	// get result
	for(i = 0; i < 8; i++)
	{
		out_buf[i] = buf[i];
	}
	*out_buf_len = i;

	mbedtls_des_free(&ctx);
	return EXCUTE_SUC;
}



/**
* @brief des加解密软算法
* @param [in] in_buf  输入数据
* @param [in] in_buf_len  输入数据长度
* @param [in] key_buf  密钥
* @param [in] key_buf_len   密钥长度
* @param [in] ecb_cbc_mode  加解模式
* @param [out] out_buf  输出数据
* @param [out] out_buf_len  输出数据长度
* @return 见soft_alg.h
* @note no
* @see 函数fun_3des_encryptbyte fun_3des_decryptbyte
*/
uint32_t ex_des_enc_dec(uint8_t* in_buf,uint32_t in_buf_len,uint8_t* key_buf,uint32_t key_buf_len,uint32_t ecb_cbc_mode,uint8_t* out_buf,uint32_t*out_buf_len)
{
	uint32_t ret = 0x9000;
	uint32_t u_out_buf_len = 0;
	if(ecb_cbc_mode==ECB)//ECB
	{
		ret = fun_des_encryptbyte(in_buf, in_buf_len,key_buf,key_buf_len,ECB,out_buf,&u_out_buf_len);
	}
	else//CBC
	{
		ret = fun_des_encryptbyte(in_buf, in_buf_len,key_buf,key_buf_len,CBC,out_buf,&u_out_buf_len);
	}
	if (ret != 0x9000)
	{
		return 0xFFFFFFFF;
	}

	*out_buf_len = u_out_buf_len;

	return (uint32_t)(*out_buf_len);
}


/**
* @brief 3des加解密软算法
* @param [in] in_buf  输入数据
* @param [in] in_buf_len  输入数据长度
* @param [in] key_buf  密钥
* @param [in] key_buf_len   密钥长度
* @param [in] ecb_cbc_mode  加解模式
* @param [out] out_buf  输出数据
* @param [out] out_buf_len  输出数据长度
* @return 见soft_alg.h
* @note no
* @see 函数fun_3des_encryptbyte fun_3des_decryptbyte
*/
uint32_t ex_3des_enc_dec(uint8_t* in_buf,uint32_t in_buf_len,uint8_t* key_buf,uint32_t key_buf_len,uint32_t ecb_cbc_mode,uint32_t enc_dec_mode,uint8_t* out_buf,uint32_t*out_buf_len)
{
    uint32_t ret = 0x9000;
    uint32_t u_out_buf_len = 0;
	switch (enc_dec_mode){
		case ENCRYPT://encrypt
			{
                ret = fun_3des_encryptbyte(in_buf, in_buf_len,key_buf,key_buf_len,ecb_cbc_mode,out_buf,&u_out_buf_len);
				break;
			}
		case 1://decrypt
			{

                ret = fun_3des_decryptbyte(in_buf, in_buf_len,key_buf,key_buf_len,ecb_cbc_mode,out_buf,&u_out_buf_len);
				break;
			}
        default:
            *out_buf_len=in_buf_len;
            memset(out_buf,0x00,in_buf_len);
            break;

	}

	if (ret != 0x9000)
	{
		return 0xFFFFFFFF;
	}
    *out_buf_len = u_out_buf_len;

	return (uint32_t)(*out_buf_len);
}


/**
* @brief 3des MAC软算法
* @param [in] in_buf  输入数据
* @param [in] in_buf_len  输入数据长度
* @param [in] iv  输入数据
* @param [in] iv_len  输入数据长度
* @param [in] key_buf  密钥
* @param [in] key_buf_len   密钥长度
* @param [out] out_buf  输出数据
* @param [out] out_buf_len  输出数据长度
* @return 见soft_alg.h
* @note no
* @see 函数fun_3des_macbyte
*/
uint32_t ex_3des_mac(uint8_t* in_buf,uint32_t in_buf_len,uint8_t* iv,uint32_t iv_len,uint8_t* key_buf,uint32_t key_buf_len,uint8_t* out_buf,uint32_t*out_buf_len)
{

	uint8_t buf[BUFFER_MAX_LEN] = {0};
	uint32_t u_out_buf_len = 0;
	memcpy(buf, iv, iv_len);
	memcpy(buf+iv_len, in_buf, in_buf_len);
	if (fun_3des_macbyte(buf,in_buf_len+iv_len, key_buf,key_buf_len,out_buf,&u_out_buf_len) != 0x9000)
	{
		return 0xFFFFFFFF;
	}
	*out_buf_len = u_out_buf_len;
	return 8* 2;

}


/*************************************************
Function:	  ex_SHA256_digest
Description:  SHA256软算法，examples中测试直接调用
Input:	      in_buf               输入数据
			  in_buf_len           输入数据长度
Output:	      out_buf              输出数据
Return:   	  参见error.h	
Others:	      no	
*************************************************/

/**
* @brief SHA256软算法
* @param [in] in_buf  输入数据
* @param [in] in_buf_len  输入数据长度
* @param [out] out_buf  输出数据
* @return 见soft_alg.h
* @note no
* @see 函数mbedtls_sha256_init mbedtls_sha256_update_ret mbedtls_sha256_finish_ret mbedtls_sha256_free
*/
uint32_t ex_SHA256_digest (uint8_t *in_buf,uint32_t in_buf_len,uint8_t *output)
{
	uint32_t ret = 0;
	mbedtls_sha256_context ctx;
	mbedtls_sha256_init( &ctx );
	if( ( ret = mbedtls_sha256_starts_ret( &ctx,0) ) != 0 )
	{
		return CER_UNKNOWN;
	}
	ret = mbedtls_sha256_update_ret( &ctx, in_buf,in_buf_len);
	if( ret != 0 )
		return CER_DATALEN;
	ret = mbedtls_sha256_finish_ret( &ctx, output);
	if( ret != 0 )
		return CER_RESULT;
	mbedtls_sha256_free( &ctx );
	return EXCUTE_SUC;

}

/**
* @brief sm4加解密软算法
* @param [in] in_buf  输入数据
* @param [in] in_buf_len  输入数据长度
* @param [in] key_buf  密钥
* @param [in] key_buf_len   密钥长度
* @param [in] ecb_cbc_mode  加解模式
* @param [out] out_buf  输出数据
* @param [out] out_buf_len  输出数据长度
* @return 见soft_alg.h
* @note no
* @see 函数sm4_crypt_ecb sm4_crypt_cbc
*/
uint32_t ex_sm4_enc_dec(uint8_t* in_buf,uint32_t in_buf_len,uint8_t* key_buf,uint32_t key_buf_len,uint32_t ecb_cbc_mode,uint32_t enc_dec_mode,uint8_t* out_buf,uint32_t*out_buf_len)
{

	//uint32_t usRet = 0x9000;
	uint8_t iv[16]={0x00};
	sm4_context ctx;
	sm4_setkey_enc(&ctx,key_buf);
	switch (enc_dec_mode){
		case ENCRYPT://encrypt
			{
				if(ecb_cbc_mode==ECB)
				{
					sm4_crypt_ecb( &ctx,1,in_buf_len,in_buf,out_buf);
					*out_buf_len = in_buf_len;
				}
				else
				{
					memcpy(iv,in_buf,16);
					sm4_crypt_cbc( &ctx,1,in_buf_len-16,iv,in_buf+16,out_buf);
					*out_buf_len = in_buf_len-16;
				}
				break;
			}
		case DECRYPT://decrypt
			{
				if(ecb_cbc_mode==ECB)
				{
					sm4_crypt_ecb( &ctx,0,in_buf_len,in_buf,out_buf);
					*out_buf_len = in_buf_len;
				}
				else
				{
					memcpy(iv,in_buf,16);
					sm4_crypt_cbc( &ctx,0,in_buf_len-16,iv,in_buf+16,out_buf);
					*out_buf_len = in_buf_len-16;
				}
				break;
			}

	}



	return *out_buf_len;
}


/**
* @brief SM4 MAC软算法
* @param [in] in_buf  输入数据
* @param [in] in_buf_len  输入数据长度
* @param [in] iv  输入数据
* @param [in] iv_len  输入数据长度
* @param [in] key_buf  密钥
* @param [in] key_buf_len   密钥长度
* @param [out] out_buf  输出数据
* @param [out] out_buf_len  输出数据长度
* @return 见soft_alg.h
* @note no
* @see 函数sm4_setkey_enc sm4_crypt_cbc
*/
uint32_t ex_sm4_mac(uint8_t* in_buf,uint32_t in_buf_len,uint8_t* iv,uint32_t iv_len,uint8_t* key_buf,uint32_t key_buf_len,uint8_t* out_buf,uint32_t*out_buf_len)
{

	sm4_context ctx;
	uint8_t OutData[BUFFER_MAX_LEN] = {0};

	if ((in_buf == NULL) || (iv == NULL))
		return 0xFFFFFFFF;

	sm4_setkey_enc(&ctx,key_buf);
	sm4_crypt_cbc( &ctx,1,in_buf_len,iv,in_buf,OutData);
	memcpy(out_buf, OutData+in_buf_len-key_buf_len, key_buf_len);

	*out_buf_len = key_buf_len;

	return *out_buf_len;
}

/**
* @brief SM3软算法
* @param [in] in_buf  输入数据
* @param [in] in_buf_len  输入数据长度
* @param [out] out_buf  输出数据
* @return 见soft_alg.h
* @note no
* @see 函数sm3
*/
uint32_t ex_SM3_digest( uint8_t *in_buf, uint32_t in_buf_len,uint8_t *output)
{
    if ((in_buf == NULL) ||(in_buf_len==0)|| (output == NULL))
		return 0xFFFFFFFF;
	sm3( in_buf, in_buf_len,output);
	return EXCUTE_SUC;
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

/**
  * @}
*/

