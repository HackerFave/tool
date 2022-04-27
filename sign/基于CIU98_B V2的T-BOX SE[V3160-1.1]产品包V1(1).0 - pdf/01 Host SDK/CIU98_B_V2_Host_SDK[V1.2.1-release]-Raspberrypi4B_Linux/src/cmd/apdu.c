/**@file  apdu.c
* @brief  apdu interface definition
* @author  liangww
* @date  2021-04-28
* @version	V1.0
* @copyright  Copyright(C),CEC Huada Electronic Design Co.,Ltd.
*/

/***************************************************************************
* Include Header Files
***************************************************************************/
#include <string.h>
#include "apdu.h"
#include "sm4.h"
#include "sm3.h"
sm4_context sm4_ctxt;

/** @addtogroup SE_Service
  * @{
  */

/** @addtogroup CMD
  * @brief Command layer.
  * @{
  */

/** @defgroup APDU APDU
  * @brief apdu command pack , unpack.
  * @{
  */



/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/** @defgroup APDU_Exported_Functions APDU Exported Functions
  * @{
  */



/**
* @brief 利用传输密钥对数据进行加密后放入双端队列
* @param [in] input  源数据
* @param [in] input_len   源数据长度
* @param [in] off 双端队列中参与加密运算数据的偏移      
* @param [in] in_queue  输入的双端队列地址
* @return 参见error.h
* @note no
* @see 函数  util_queue_rear_push 
*/
se_error_t apdu_trans_encrypt	(uint8_t *input, uint32_t input_len,uint32_t off,double_queue in_queue)
{
	
	uint8_t len_high=0x00;
	uint8_t len_low =0x00;
	uint8_t pading[16]={0}; 
	
	sm4_ctxt.mode = SM4_ENCRYPT;
	sm4_setkey_enc(&sm4_ctxt,trans_key);
	//被加密数据长度入双端队列
	len_high=input_len>>8;
	util_queue_rear_push(&len_high,1, in_queue);
	len_low=input_len&0xff;
	util_queue_rear_push(&len_low,1, in_queue);
	//输入数据进入双端队列
	util_queue_rear_push((uint8_t *)input,input_len, in_queue);
	//判断是否补80
	if((in_queue->q_buf_len-off)% 16!=0)
	{   //补80
		pading[0] = 0x80;
		util_queue_rear_push(pading,1, in_queue);
		//判断是否补00
		if((in_queue->q_buf_len-off) % 16!=0)
		{
		  memset(pading,0x00,16);
		  util_queue_rear_push(pading,16-(in_queue->q_buf_len-off)%16, in_queue);
		}			
	}
	//SM4传输密钥加密数据
	sm4_crypt_ecb(&sm4_ctxt,SM4_ENCRYPT,in_queue->q_buf_len-off, (unsigned char *) &(in_queue->q_buf[in_queue->front_node+off]), (unsigned char *) &(in_queue->q_buf[in_queue->front_node+off])); 

	return SE_SUCCESS;
}


/**
* @brief 利用传输密钥对加密数据进行解密后放入双端队列     
* @param [in] in_queue  输入的双端队列地址
* @return 参见error.h
* @note no
* @see 函数  util_queue_rear_push 
*/
se_error_t apdu_trans_decrypt	(double_queue in_queue)
{
	sm4_ctxt.mode = SM4_DECRYPT;
	sm4_setkey_dec(&sm4_ctxt,trans_key);
	//SM4传输密钥解密数据
	sm4_crypt_ecb(&sm4_ctxt,SM4_DECRYPT,in_queue->q_buf_len, (unsigned char *) &(in_queue->q_buf[in_queue->front_node]), (unsigned char *) &(in_queue->q_buf[in_queue->front_node])); 

	return SE_SUCCESS;
}


/**
* @brief 利用pin加密后放入双端队列
* @param [in] pin->owner pin属主类型
* @param [in] pin->pin_value pin的值
* @param [in] pin->pin_len  取值为0x06-0x10
* @param [in] in_buf 输入数据
* @param [in] in_buf_len 输入数据长度
* @param [in] if_xor 是否进行异或计算（veryfypin的时候只计算sm3摘要并取前16字节）
* @param [out] queue_out  输出的双端队列地址
* @return 参见error.h
* @note no
* @see 函数  util_queue_rear_push    sm3  tpdu_execute 
*/
se_error_t apdu_pin_encrypt	(pin_t *pin, const uint8_t *in_buf, uint32_t in_buf_len, double_queue queue_out, bool if_xor)
{
	iso_command_apdu_t command = {0};
	se_error_t ret = 0;
	uint8_t sm3_buf[32]={0}; 
	uint8_t pin_buf[32]={0}; 
	uint32_t i = 0;
	uint32_t j = 0;	
	uint32_t out_len = 0;
	double_queue_node queue_in ={0} ;
	util_queue_init(&queue_in);

	//发送取随机数指令并放入双端队列queue_out
	//设置命令头
	tpdu_init_with_id(&command,CMD_GET_RANDOM);
	//设置le
	tpdu_set_le(&command, 0x10);
	ret = tpdu_execute(&command, (uint8_t *)&queue_in, util_queue_size (&queue_in), (uint8_t *)queue_out, &out_len);
	if(ret!=SE_SUCCESS)
		return ret;
	
	//16字节随机数+pin值拼接放入双端队列queue_out
	util_queue_rear_push(pin->pin_value,pin->pin_len, queue_out);
	
	//sm3摘要计算
	sm3(queue_out->q_buf+queue_out->front_node, queue_out->q_buf_len, sm3_buf);
    
	if(if_xor)
	{
		//在新pin前添加两字节Ld，pin_buf[0]=0。
		pin_buf[1]=in_buf_len&0xff;
		//将新pin的值放入pin_buf
		memcpy(pin_buf+2,in_buf,in_buf_len);
		in_buf_len = in_buf_len + 2;	

		//判断是否补80
		if((in_buf_len)% 16!=0)
		{   //补80
			pin_buf[in_buf_len] = 0x80;
			in_buf_len = in_buf_len + 1;
			//判断是否补00
			if(in_buf_len % 16!=0)
			{
			  if(in_buf_len<16)
			  	in_buf_len = 16;//由于pin值的长度小于等于16字节，补完80仍小于16字节，则长度取16字节
			  else if(16<in_buf_len)
			  	in_buf_len = 32;//由于pin值的长度大于等于16字节，补完80仍大于16字节，则长度去取32字节
			}			
		}
		
		//利用摘要计算的前16字节作为密钥与新pin值进行异或
		for( j = 0;j < in_buf_len/16; j++)
		{
			for( i = 0; i < 16; i++)
				pin_buf[i+j*16] = sm3_buf[i] ^ pin_buf[i+j*16];
		}

		//将计算结果放入双端队列 
		util_queue_init(queue_out);
	    util_queue_rear_push(pin_buf,in_buf_len, queue_out);
	}
    else
    {
    	//将计算结果放入双端队列 
		util_queue_init(queue_out);
	    util_queue_rear_push(sm3_buf,16, queue_out);
	    
	}
	return SE_SUCCESS;
}



/**
* @brief 对称算法补位
* @param [in] padding  补位方式
* @param [in] input  源数据
* @param [in] input_len   源数据长度
* @param [out] output  补位输出数据
* @param [out] output_len  补位输出数据长度
* @return 参见error.h
* @note no
* @see 函数  util_queue_rear_push 
*/
se_error_t apdu_sym_padding	(uint32_t padding, uint8_t *input, uint32_t input_len, uint8_t *output, uint32_t *output_len)
{
	uint32_t buffer_len = *output_len;
	uint32_t fill = 0;
	uint32_t end = 0;
	double_queue queue_out=(double_queue)input;
	uint8_t temp_buf[16] = {0x00};

	if(padding!=PADDING_NOPADDING&&padding!=PADDING_PKCS7)
	{
		return SE_ERR_PARAM_INVALID;
	}

	if (input == NULL || input_len==0 || output == NULL)
	{
		return SE_ERR_PARAM_INVALID;
	}


	switch (padding) {
	case PADDING_PKCS7:
		fill = buffer_len - input_len;
		end = input_len;
		break;
	case PADDING_NOPADDING:
		fill = 0;
		end = input_len;
	default:
		fill = 0;
		end = input_len;
		break;
	}

	memset(temp_buf, fill, buffer_len - end);
	util_queue_rear_push(temp_buf,buffer_len - input_len, queue_out);
	output = (uint8_t*)queue_out;
	*output_len = buffer_len;


	return SE_SUCCESS;
}



/**
* @brief 对称算法去补位
* @param [in] padding  补位方式
* @param [in] input  源数据
* @param [in] input_len   源数据长度
* @param [out] output  去补位数据
* @param [out] output_len  去补位数据长度
* @return 参见error.h
* @note no
* @see 函数  util_queue_rear_pop 
*/
se_error_t apdu_sym_unpadding(uint32_t padding, uint8_t *input, uint32_t input_len, uint8_t *output, uint32_t *output_len)
{
	uint32_t buffer_len = input_len;
	uint32_t offset = 0;
	double_queue queue_in=(double_queue)input;
	double_queue queue_out=(double_queue)input;
	uint32_t off = queue_in->rear_node;

	if (input == NULL || input_len <= 0 || output == NULL || output_len == NULL) {
		return SE_ERR_PARAM_INVALID;
	}

	switch (padding)
	{
		case PADDING_PKCS7:
			buffer_len = input_len - queue_in->q_buf[off-1];
			offset = queue_in->q_buf[off-1];
			break;
		case PADDING_NOPADDING:
			buffer_len = input_len;
			offset = 0;
			break;
		default:
			buffer_len = input_len;
			offset = 0;
			break;
	}
    util_queue_rear_pop(offset, queue_out);
	output = (uint8_t*)queue_out;
	*output_len = buffer_len;

	return SE_SUCCESS;
}


/**
* @brief 获取补位长度
* @param [in] alg  算法类型
* @param [in] padding  补位方式
* @param [in] input_len   原始长度
* @return 补位后的长度	
* @note no
* @see no
*/
uint32_t apdu_sym_padding_length(uint32_t alg, uint32_t padding, uint32_t input_len)
{
	uint32_t buffer_len = 0;
	uint32_t align_size = 0;
	//设置分组长度
	if(alg==ALG_DES128)
		align_size = 8;
	else
		align_size = 16;

	switch (padding)
	{
		case PADDING_NOPADDING:
			buffer_len = input_len;
			break;
		case PADDING_PKCS7:
			if ((input_len == 0) || (input_len%align_size) == 0)
				buffer_len = input_len + 1;
			else
				buffer_len = input_len;
			break;
		default:
			buffer_len = input_len;
			break;
	}

	while ((buffer_len%align_size) != 0)
	{
		buffer_len++;
	}

	return buffer_len;
}



/**
* @brief 切换工作模式命令封装
* @param [in] type  工作模式
* @return 参见error.h	
* @note no
* @see 函数  util_queue_init  tpdu_init_with_id  tpdu_set_p1p2  tpdu_execute_no_response
*/
se_error_t apdu_switch_mode (work_mode  type)
{
	iso_command_apdu_t command = {0};
	se_error_t ret = 0;
	double_queue_node queue_in ={0} ;
	uint32_t p2 = 0;
	
	//双端队列初始化
	util_queue_init(&queue_in);
	//设置命令头
	tpdu_init_with_id(&command,CMD_ENTER_LOWPOWER);
	//设置P1P2
	p2 = 0x00;
	tpdu_set_p1p2 (&command,0x00,p2);

	ret = tpdu_execute_no_response(&command, (uint8_t *)&queue_in, util_queue_size (&queue_in));
	if(ret!=SE_SUCCESS)
		return ret;	
	
	
	return SE_SUCCESS;
}


/**
* @brief 命令控制命令封装
* @param [in] ctrlcode  控制类型
* @param [in] in_buf  输入数据
* @param [in] in_buf_len   输入数据长度 
* @param [out] out_buf  输出数据
* @param [out] out_buf_len  输出数据长度
* @return 参见error.h
* @note no
* @see no
*/
se_error_t apdu_control(ctrl_type ctrlcode, const uint8_t *in_buf, uint32_t in_buf_len, uint8_t *out_buf, uint32_t *out_buf_len)
{
	return SE_SUCCESS;
}


/**
* @brief 设备认证命令封装
* @param [in] in_buf  输入数据
* @param [in] in_buf_len   输入数据长度 
* @return 参见error.h
* @note no
* @see no
*/
se_error_t apdu_ext_auth(const uint8_t *in_buf, uint32_t in_buf_len)
{

	iso_command_apdu_t command = {0};
	se_error_t ret = 0;
	double_queue_node queue_in ={0} ;
	//检查参数
	if(in_buf==NULL||in_buf_len==0)
	{  
		LOGE("failed to apdu_ext_auth input params!\n");
		return SE_ERR_PARAM_INVALID;
	}

	//双端队列初始化
	util_queue_init(&queue_in);
	//输入数据存入双端队列
    util_queue_rear_push((uint8_t *)in_buf,in_buf_len, &queue_in);
	//设置命令头
	tpdu_init_with_id(&command,CMD_EXTTERN_AUTH);
	ret = tpdu_execute_no_response(&command, (uint8_t *)&queue_in, util_queue_size (&queue_in));
	if(ret!=SE_SUCCESS)
		return ret;

	return SE_SUCCESS;
}


/**
* @brief 写入密钥或者PIN命令封装
* @param [in] in_buf  主控密钥值/密钥信息
* @param [in] in_buf_len   输入数据长度
* @param [in] if_encrypt  是否加密
* @param [in] if_write  是否写入密钥 
* @param [in] if_update_mkey  是否更新主控密钥
* @return 参见error.h
* @note no
* @see 函数  util_queue_init	 util_queue_rear_push  tpdu_init_with_id  tpdu_set_cla  tpdu_set_p1p2  tpdu_execute_no_response  util_queue_size
*/
se_error_t apdu_write_key(const uint8_t *in_buf, uint32_t in_buf_len, bool if_encrypt, bool if_write, bool if_update_mkey )
{
  iso_command_apdu_t command = {0};
	se_error_t ret = 0;
	double_queue_node queue_in ={0} ;
	//检查参数
	if(in_buf==NULL||in_buf_len==0)
	{  
		LOGE("failed to apdu_write_key input params!\n");
		return SE_ERR_PARAM_INVALID;
	}
	if(if_write==true&&if_update_mkey==true)
	{  
		LOGE("failed to apdu_write_key bool params!\n");
		return SE_ERR_PARAM_INVALID;
	}
	//双端队列初始化
	util_queue_init(&queue_in);
	//设置命令头
	tpdu_init_with_id(&command,CMD_WRITE_KEY);
	//设置P1P2
	//区分生成和更新，生成P1=0x00、更新P1=0x01、更新主控密钥P2=0x00、更新其他密钥P2=0x01
	if(if_write==true)
		tpdu_set_p1p2 (&command,0x00,0x00);
	else if(if_update_mkey==true)
		tpdu_set_p1p2 (&command,0x01,0x00);
	else if(if_update_mkey==false)
	    tpdu_set_p1p2 (&command,0x01,0x01);
	//判断是否为密文传输，若为密文传输对数据进行加密
	if(if_encrypt)
	{
		ret =  apdu_trans_encrypt((uint8_t *)in_buf, in_buf_len,0,&queue_in);//若需要传输密钥加密保护，则对数据加密后进入双端队列	  
		//设置CLA
		tpdu_set_cla (&command,CMD_CLA_CASE4);

	}
    else
	{
	    //输入数据存入双端队列
	    util_queue_rear_push((uint8_t *)in_buf,in_buf_len, &queue_in);
	    //设置CLA
	    tpdu_set_cla (&command,CMD_CLA_CASE3);
    }
	ret = tpdu_execute_no_response(&command, (uint8_t *)&queue_in, util_queue_size (&queue_in));
	if(ret!=SE_SUCCESS)
		return ret;

	return SE_SUCCESS;

}




/**
* @brief 产生/更新非对称公私钥对命令封装
* @param [in] pub_key->alg 算法类型 
* @param [in] pub_key->id 公钥id
* @param [in] pri_key->id 私钥id
* @param [in] symkey->alg 对称算法类型
* @param [in] symkey->id 对称密钥id
* @param [out] pub_key->val 公钥值
* @param [out] pub_key->val_len 公钥值长度
* @return 参见error.h
* @note no
* @see 函数  util_queue_init  util_queue_rear_push  tpdu_init_with_id  tpdu_set_p1p2  tpdu_execute  util_queue_size
*/
se_error_t  apdu_generate_key(pub_key_t *pub_key,pri_key_t *pri_key, sym_key_t * symkey )
{
	iso_command_apdu_t command = {0};
	se_error_t ret = 0;
	uint32_t out_len = 0;
	uint32_t off = 0;
	double_queue_node queue_in ={0} ;
	double_queue_node queue_out ={0} ;
	uint8_t temp_buf[16] = {0};
	uint8_t pri_offset =8;

	//双端队列初始化
	util_queue_init(&queue_in);
	util_queue_init(&queue_out);
    //设置命令头
	tpdu_init_with_id(&command,CMD_GENERATE_KEY);
	//转换为内部密钥类型
	temp_buf[KEYUSAGE] = 0x02;//公钥或对称密钥密钥属性用途为应用密钥
    if(symkey != NULL)
    {
        temp_buf[KID] =  symkey->id;
		switch(symkey->alg)
		{
			case ALG_AES128:
				temp_buf[KALG] = 0x60;
				break;
			case ALG_DES128:
				temp_buf[KALG] = 0x00;
				break;
			case ALG_SM4:
				temp_buf[KALG] = 0x40;
				break;
		}
		util_queue_rear_push(temp_buf,8, &queue_in);
	}
	else
	{
	if(pub_key->id != pri_key->id)//不同的kid生成非对称密钥
	{
        //公钥kid取值
        temp_buf[KID] = pub_key->id;
        //私钥kid取值
        temp_buf[KID+pri_offset] = pri_key->id;
		//私钥属性用途为应用密钥
    	temp_buf[KEYUSAGE+pri_offset] = 0x02;
        //算法类型和模长取值设置
		switch(pub_key->alg)
		{
			case ALG_ECC256_NIST:
				temp_buf[KALG] = 0xA0;
				temp_buf[KMODEL_LEN] = 0x20;
			    temp_buf[KALG+pri_offset] = 0xA1;
				temp_buf[KMODEL_LEN+pri_offset] = 0x20;
				break;
			case ALG_SM2:
				temp_buf[KALG] = 0x90;
				temp_buf[KMODEL_LEN] = 0x20;
			    temp_buf[KALG+pri_offset] = 0x91;
				temp_buf[KMODEL_LEN+pri_offset] = 0x20;
				break;
			case ALG_RSA1024_CRT:
				temp_buf[KALG] = 0x80;
				temp_buf[KMODEL_LEN] = 0x20;
			    temp_buf[KALG+pri_offset] = 0x82;
				temp_buf[KMODEL_LEN+pri_offset] = 0x20;
				break;
			case ALG_RSA2048_CRT:
				temp_buf[KALG] = 0x80;
				temp_buf[KMODEL_LEN] = 0x40;
			    temp_buf[KALG+pri_offset] = 0x82;
				temp_buf[KMODEL_LEN+pri_offset] = 0x40;
				break;
			case ALG_RSA1024_STANDAND:
				temp_buf[KALG] = 0x80;
				temp_buf[KMODEL_LEN] = 0x20;
			    temp_buf[KALG+pri_offset] = 0x81;
				temp_buf[KMODEL_LEN+pri_offset] = 0x20;
				break;
			case ALG_RSA2048_STANDAND:
				temp_buf[KALG] = 0x80;
				temp_buf[KMODEL_LEN] = 0x40;
			    temp_buf[KALG+pri_offset] = 0x81;
				temp_buf[KMODEL_LEN+pri_offset] = 0x40;
				break;
		}
		util_queue_rear_push(temp_buf,16, &queue_in);
	}
	else if(pub_key->id == pri_key->id)//相同的kid生成非对称密钥对儿
	{
		  //公私钥对儿kid取值
		  temp_buf[KID] = pub_key->id;
		  switch(pub_key->alg)
		  {
            case ALG_ECC256_NIST:
				temp_buf[KALG] = 0xA2;
				temp_buf[KMODEL_LEN] = 0x20;
				break;
			case ALG_SM2:
				temp_buf[KALG] = 0x92;
				temp_buf[KMODEL_LEN] = 0x20;
				break;
			case ALG_RSA1024_CRT:
				temp_buf[KALG] = 0x84;
				temp_buf[KMODEL_LEN] = 0x20;
				break;
			case ALG_RSA2048_CRT:
				temp_buf[KALG] = 0x84;
				temp_buf[KMODEL_LEN] = 0x40;
				break;
			case ALG_RSA1024_STANDAND:
				temp_buf[KALG] = 0x83;
				temp_buf[KMODEL_LEN] = 0x20;
				break;
			case ALG_RSA2048_STANDAND:
				temp_buf[KALG] = 0x83;
				temp_buf[KMODEL_LEN] = 0x40;
				break;
			}
		   util_queue_rear_push(temp_buf,8, &queue_in);
	}
	}

	ret = tpdu_execute(&command, (uint8_t *)&queue_in, util_queue_size (&queue_in), (uint8_t *)&queue_out, &out_len);
	if(ret!=SE_SUCCESS)
		return ret;

	//从双端队列拷贝回输出数据
	off = queue_out.front_node;
	if(symkey == NULL)
	{
	    if(pub_key->alg==ALG_ECC256_NIST||pub_key->alg==ALG_SM2)
		{
			memcpy(pub_key->val,&queue_out.q_buf[off],64);
			pub_key->val_len = 64;
		}
		else if(pub_key->alg==ALG_RSA1024_CRT||pub_key->alg==ALG_RSA1024_STANDAND)
		{
			memcpy(pub_key->val,&queue_out.q_buf[off],132);
			pub_key->val_len = 132;
		}
		else if(pub_key->alg==ALG_RSA2048_CRT||pub_key->alg==ALG_RSA2048_STANDAND)
		{
			memcpy(pub_key->val,&queue_out.q_buf[off],260);
			pub_key->val_len = 260;
		}
	}
	

	return SE_SUCCESS;
}



/**
* @brief 产生shared key命令封装
* @param [in] shared_key->alg  算法类型  
* @param [in] input  计算数据
* @param [in] input_len   计算数据长度
* @param [in] if_return_key  是否返回key
* @param [in] if_return_s  是否返回s
* @param [out] shared_key  shared key值
* @param [out] sm2_s  s值
* @return 参见error.h
* @note no
* @see 函数  util_queue_init  tpdu_init_with_id  tpdu_set_p1p2  util_queue_rear_push  tpdu_execute
*/
se_error_t apdu_generate_shared_key (uint8_t *in_buf, uint32_t in_buf_len, unikey_t *shared_key, uint8_t *sm2_s, bool if_return_key, bool if_return_s)
{
	iso_command_apdu_t command = {0};
	se_error_t ret = 0;
	uint32_t off = 0;
	double_queue_node queue_in ={0} ;
	double_queue_node queue_out ={0} ;
	uint8_t p1 = 0;
	uint8_t p2 = 0;
	uint8_t return_key_flag = 0;
	uint8_t return_s_flag = 0;
	//uint8_t temp_buf[4] = {0};
	uint32_t out_len = 0;


	//双端队列初始化
	util_queue_init(&queue_in);
	util_queue_init(&queue_out);

	//设置命令头
	tpdu_init_with_id(&command,CMD_GEN_SHARED_KEY);
	
	//key是否导出 0x00-导出 0x80不导出
	return_key_flag = (if_return_key==true)?0x10:0x00;
	//s是否导出 0x01-导出 0x00不导出
	return_s_flag =  (if_return_s==true)?0x20:0x00;
	//设置P1P2
	 switch(shared_key->alg)
		  {
            case ALG_ECDH_ECC256:
				p1 =return_key_flag|return_s_flag|0x00;
				break;
			case ALG_ECDH_SM2:
				p1 =return_key_flag|return_s_flag|0x01;
				break;
			case ALG_SM2_SM2:
				p1 =return_key_flag|return_s_flag|0x02;
				break;
			}
	 if(shared_key->alg==ALG_ECDH_ECC256||shared_key->alg==ALG_ECDH_SM2)
	 {
	      p2 =shared_key->id;
	 }
	tpdu_set_p1p2 (&command,p1,p2);

	//输入数据存入双端队列
	util_queue_rear_push(in_buf,in_buf_len,&queue_in);

	ret = tpdu_execute(&command, (uint8_t *)&queue_in, util_queue_size (&queue_in), (uint8_t *)&queue_out, &out_len);
	if(ret!=SE_SUCCESS)
		return ret;

	//从双端队列拷贝回输出数据
	off = queue_out.front_node;

	if(shared_key->alg==ALG_ECDH_ECC256||shared_key->alg==ALG_ECDH_SM2)
	{
		//ECC 返回32字节key
		if(if_return_key==true)
		{
			memcpy(shared_key->val,&queue_out.q_buf[off],32);
			shared_key->val_len = 32;
		}
	}
	else
	{
		if(if_return_key==true)
		{
			memcpy(shared_key->val,&queue_out.q_buf[off],16);
			shared_key->val_len = 16;
		}
		if(if_return_s==true)
			memcpy(sm2_s,&queue_out.q_buf[off+16],64);		
	}

	return SE_SUCCESS;

}


/**
* @brief 删除密钥命令封装
* @param [in] id 被删除密钥id
* @return 参见error.h
* @note no
* @see no
*/
se_error_t apdu_delete_key(uint8_t id)
{

	iso_command_apdu_t command = {0};
	se_error_t ret = 0;
	double_queue_node queue_in ={0} ;

	//双端队列初始化
	util_queue_init(&queue_in);
	//设置命令头
	tpdu_init_with_id(&command,CMD_DEL_KEY);
	//设置P1P2
	tpdu_set_p1p2 (&command,0x00,id);
	
	ret = tpdu_execute_no_response(&command, (uint8_t *)&queue_in, util_queue_size (&queue_in));
	if(ret!=SE_SUCCESS)
		return ret;

	return SE_SUCCESS;
}


/**
* @brief 导入密钥命令封装
* @param [in] ukey->alg 算法类型(ALG_RSA1024_CRT/ALG_RSA2048_CRT/ALG_ECC256_NIST/ALG_SM2/ALG_AES128/ALG_DES128/ALG_SM4)
* @param [in] ukey->id  应用密钥id（解密用私钥KID或解密用对称密钥的kid）
* @param [in] inkey->alg 算法类型(ALG_RSA1024_CRT/ALG_RSA2048_CRT/ALG_ECC256_NIST/ALG_SM2/ALG_AES128/ALG_DES128/ALG_SM4)
* @param [in] inkey->id   密钥标识符，范围为0x01-0xFF
* @param [in] inkey->type 密钥类型
* @param [in] inkey->val  密钥值
* @param [in] inkey->val_len  会话密钥值长度
* @param [in] if_cipher  true-密文 false-明文
* @param [in] if_trasns_key  是否采用传输密钥加密导入密钥
* @return 参见error.h
* @note no
* @see 函数  util_queue_init	 util_queue_rear_push  tpdu_init_with_id  tpdu_set_p1p2	 tpdu_execute	util_queue_size
*/
se_error_t  apdu_import_key (unikey_t *ukey, unikey_t *inkey, bool if_cipher, bool if_trasns_key)
{
	se_error_t ret = 0;
	uint8_t p1 = 0x00;
	uint8_t p2 = 0x00;
	double_queue_node queue_in ={0} ;
	double_queue_node queue_out ={0} ;
	iso_command_apdu_t command = {0};
	uint8_t temp_buf[8] = {0};
	//uint32_t temp_len = 0;
	//uint32_t off = 0;
	uint32_t cipher_alg= 0;
	

	//双端队列初始化
	util_queue_init(&queue_in);
	util_queue_init(&queue_out);
	
	//p1、p2取值初始化
	if(if_cipher)
	{
        //采用密文导入，p1 bit5置1
		p1=p1|0x10;
		//采用传输密钥加密传输，p1 bit6置1
		if (if_trasns_key)
		{
			p1=p1|0x20;
			//传输密钥算法采用SM4算法，p1 bit4~bit1取值为'0100'
			p1=p1|0x04;
			//p2取值为传输密钥id '0x02'
			p2=0x02;
		}
		else
		{
	        //按照应用加密密钥算法设置p1
			switch(ukey->alg)
			{
				case ALG_RSA1024_CRT:
					cipher_alg = 0x08;
					break;
				case ALG_RSA2048_CRT:
					cipher_alg = 0x08;
					break;
				case ALG_ECC256_NIST:
					cipher_alg = 0x0A;
					break;
				case ALG_SM2:
					cipher_alg = 0x09;
					break;
				case ALG_DES128:
					cipher_alg = 0x00;
					break;
				case ALG_SM4:
					cipher_alg = 0x04;
					break;
				case ALG_AES128:
					cipher_alg = 0x06;
					break;
				
			}
            //依据用于加密的应用密钥算法类型设置p1参数
			p1=p1|cipher_alg;
		    //p2取值为应用密钥id
		    p2=ukey->id;

		}
	    
	}

	//组织数据域数据（密钥属性+密钥值明文/密文），并放入双端队列
	temp_buf[KEYUSAGE]=0x02;
	temp_buf[KID]=inkey->id;
	switch(inkey->alg)
		{
			case ALG_AES128:
				temp_buf[KALG] = 0x60;
	            temp_buf[KLEN_LOW]=0x10;
				break;
			case ALG_DES128:
				temp_buf[KALG] = 0x00;
	            temp_buf[KLEN_LOW]=0x10;
				break;
			case ALG_SM4:
				temp_buf[KALG] = 0x40;
	            temp_buf[KLEN_LOW]=0x10;
				break;
			case ALG_ECC256_NIST:
                if(inkey->type==PRI)
                {
  				  temp_buf[KALG] = 0xA1;
  				  temp_buf[KMODEL_LEN] = 0x20;
	              temp_buf[KLEN_LOW]=0x20;
				}
				else if(inkey->type==PUB)
				{
                  temp_buf[KALG] = 0xA0;
				  temp_buf[KMODEL_LEN] = 0x20;
	              temp_buf[KLEN_LOW]=0x40;
				}
			    else if(inkey->type==PRI_PUB_PAIR)
			    {
				   temp_buf[KALG] = 0xA2;
				   temp_buf[KMODEL_LEN] = 0x20;
	               temp_buf[KLEN_LOW]=0x60;
				}					    
				break;
			case ALG_SM2:
				if(inkey->type==PRI)
                {
  				  temp_buf[KALG] = 0x91;
  				  temp_buf[KMODEL_LEN] = 0x20;
	              temp_buf[KLEN_LOW]=0x20;
				}
				else if(inkey->type==PUB)
				{
                  temp_buf[KALG] = 0x90;
				  temp_buf[KMODEL_LEN] = 0x20;
	              temp_buf[KLEN_LOW]=0x40;
				}
			    else if(inkey->type==PRI_PUB_PAIR)
			    {
				   temp_buf[KALG] = 0x92;
				   temp_buf[KMODEL_LEN] = 0x20;
	               temp_buf[KLEN_LOW]=0x60;
				}			
				break;
			case ALG_RSA1024_CRT:
				if(inkey->type==PRI)
                {
  				  temp_buf[KALG] = 0x82;
  				  temp_buf[KMODEL_LEN] = 0x20;
				  temp_buf[KLEN_HIGH]=0x01;
	              temp_buf[KLEN_LOW]=0x44;
				}
				else if(inkey->type==PUB)
				{
                  temp_buf[KALG] = 0x80;
				  temp_buf[KMODEL_LEN] = 0x20;
	              temp_buf[KLEN_LOW]=0x84;
				}
			    else if(inkey->type==PRI_PUB_PAIR)
			    {
				   temp_buf[KALG] = 0x84;
				   temp_buf[KMODEL_LEN] = 0x20;
				   temp_buf[KLEN_HIGH]=0x01;
	               temp_buf[KLEN_LOW]=0xC4;
				}			
				break;
			case ALG_RSA2048_CRT:
				if(inkey->type==PRI)
                {
  				  temp_buf[KALG] = 0x82;
  				  temp_buf[KMODEL_LEN] = 0x40;
				  temp_buf[KLEN_HIGH]=0x02;
	              temp_buf[KLEN_LOW]=0x84;
				}
				else if(inkey->type==PUB)
				{
                  temp_buf[KALG] = 0x80;
				  temp_buf[KMODEL_LEN] = 0x40;
				  temp_buf[KLEN_HIGH]=0x01;
	              temp_buf[KLEN_LOW]=0x04;
				}
			    else if(inkey->type==PRI_PUB_PAIR)
			    {
				   temp_buf[KALG] = 0x84;
				   temp_buf[KMODEL_LEN] = 0x40;
				   temp_buf[KLEN_HIGH]=0x03;
	               temp_buf[KLEN_LOW]=0x84;
				}			
				break;
			case ALG_RSA1024_STANDAND:
				if(inkey->type==PRI)
                {
  				  temp_buf[KALG] = 0x81;
  				  temp_buf[KMODEL_LEN] = 0x20;
				  temp_buf[KLEN_HIGH]=0x01;
	              temp_buf[KLEN_LOW]=0x00;
				}
				else if(inkey->type==PUB)
				{
                  temp_buf[KALG] = 0x80;
				  temp_buf[KMODEL_LEN] = 0x20;
	              temp_buf[KLEN_LOW]=0x84;
				}
			    else if(inkey->type==PRI_PUB_PAIR)
			    {
				   temp_buf[KALG] = 0x83;
				   temp_buf[KMODEL_LEN] = 0x20;
				   temp_buf[KLEN_HIGH]=0x01;
	               temp_buf[KLEN_LOW]=0x04;
				}			
				break;
			case ALG_RSA2048_STANDAND:
				if(inkey->type==PRI)
                {
  				  temp_buf[KALG] = 0x81;
  				  temp_buf[KMODEL_LEN] = 0x40;
				  temp_buf[KLEN_HIGH]=0x02;
	              temp_buf[KLEN_LOW]=0x00;
				}
				else if(inkey->type==PUB)
				{
                  temp_buf[KALG] = 0x80;
				  temp_buf[KMODEL_LEN] = 0x40;
				  temp_buf[KLEN_HIGH]=0x01;
	              temp_buf[KLEN_LOW]=0x04;
				}
			    else if(inkey->type==PRI_PUB_PAIR)
			    {
				   temp_buf[KALG] = 0x83;
				   temp_buf[KMODEL_LEN] = 0x40;
				   temp_buf[KLEN_HIGH]=0x02;
	               temp_buf[KLEN_LOW]=0x04;
				}			
				break;
		}
	
	//密钥属性存入双端队列
	util_queue_rear_push((uint8_t *)temp_buf,8, &queue_in);

	//输入数据进入双端队列
	if(if_trasns_key)
	{
	  ret =  apdu_trans_encrypt((uint8_t *)inkey->val, inkey->val_len,8,&queue_in);//若需要传输密钥加密保护，则对数据加密后进入双端队列		
	  if(ret!=SE_SUCCESS)
		return ret;	 
	}
    else
    {
	   util_queue_rear_push((uint8_t *)inkey->val,inkey->val_len, &queue_in);
	}
	
	
	//设置命令头
	tpdu_init_with_id(&command,CMD_IMPORT_KEY);
	if((queue_in.q_buf_len+8)>0xff)
	{
	  command.isoCase = ISO_CASE_3E;
	}
	else
	{
      command.isoCase = ISO_CASE_3S;
	}
	//设置P1P2
	tpdu_set_p1p2 (&command,p1,p2);
	ret = tpdu_execute_no_response(&command, (uint8_t *)&queue_in, util_queue_size(&queue_in));
    if(ret!=SE_SUCCESS)
   	 return ret;

	return SE_SUCCESS;

}


/**
* @brief 导出密钥命令封装
* @param [in] ukey->alg 算法类型(ALG_RSA1024_CRT/ALG_RSA2048_CRT/ALG_ECC256_NIST/ALG_SM2/ALG_AES128/ALG_DES128/ALG_SM4)
* @param [in] ukey->id  应用密钥id（加密用公钥KID或加密用对称密钥的kid）
* @param [in] exkey->alg 算法类型(ALG_RSA1024_CRT/ALG_RSA2048_CRT/ALG_ECC256_NIST/ALG_SM2/ALG_AES128/ALG_DES128/ALG_SM4)
* @param [in] exkey->id   密钥标识符，范围为0x01-0xFF
* @param [in] exkey->type 密钥类型
* @param [in] if_cipher  true-密文 false-明文
* @param [out] exkey->val  密钥值
* @param [out] exkey->val_len  会话密钥值长度
* @param [in] if_trasns_key  是否采用传输密钥加密导入密钥
* @return 参见error.h
* @note no
* @see 函数  util_queue_init	 util_queue_rear_push  tpdu_init_with_id  tpdu_set_p1p2  tpdu_execute  util_queue_size
*/
se_error_t  apdu_export_key (unikey_t *ukey, unikey_t *exkey, bool if_cipher, bool if_trasns_key)
{
	se_error_t ret = 0;
	uint8_t p1 = 0x00;
	uint8_t p2 = 0x00;
	double_queue_node queue_in ={0} ;
	double_queue_node queue_out ={0} ;
	iso_command_apdu_t command = {0};
	uint32_t off = 0;
	uint32_t temp_len = 0;
	uint8_t temp_buf[8] = {0};
	uint8_t cipher_alg = 0x00;
	//p1、p2取值初始化

	if(if_cipher==true)
	{
        //采用密文导出，p1 bit5置1
		p1=p1|0x10;
		//采用传输密钥加密传输，p1 bit6置1
		if (if_trasns_key)
		{
			p1=p1|0x20;
			//传输密钥算法采用SM4算法，p1 bit4~bit1取值为'0100'
			p1=p1|0x04;
			//p2取值为传输密钥id '0x02'
			p2=0x02;
		}
		else
		{
	        //按照应用加密密钥算法设置p1
		switch(ukey->alg)
			{
				case ALG_RSA1024_CRT:
					cipher_alg = 0x08;
					break;
				case ALG_RSA2048_CRT:
					cipher_alg = 0x08;
					break;
				case ALG_ECC256_NIST:
					cipher_alg = 0x0A;
					break;
				case ALG_SM2:
					cipher_alg = 0x09;
					break;
				case ALG_DES128:
					cipher_alg = 0x00;
					break;
				case ALG_SM4:
					cipher_alg = 0x04;
					break;
				case ALG_AES128:
					cipher_alg = 0x06;
					break;
				
			}
            //依据用于加密的应用密钥算法类型设置p1参数
			p1=p1|cipher_alg;
		    //p2取值为应用密钥id
		    p2=ukey->id;

		}
	    
	}
	//组织数据域数据（密钥属性+密钥值明文/密文），并放入双端队列
		temp_buf[0]=exkey->id;
	    switch(exkey->alg)
			{
				case ALG_AES128:
					temp_buf[1] = 0x60;
					break;
				case ALG_DES128:
					temp_buf[1] = 0x00;
					break;
				case ALG_SM4:
					temp_buf[1] = 0x40;
					break;
				case ALG_ECC256_NIST:
					if(exkey->type==PRI)
					{
					  temp_buf[1] = 0xA1;
					}
					else if(exkey->type==PUB)
					{
					  temp_buf[1]  = 0xA0;
					}		
					break;
				case ALG_SM2:
					if(exkey->type==PRI)
					{
					  temp_buf[1] = 0x91;
					}
					else if(exkey->type==PUB)
					{
					  temp_buf[1] = 0x90;
					}		
					break;
				case ALG_RSA1024_CRT:
					if(exkey->type==PRI)
					{
					  temp_buf[1] = 0x82;
					}
					else if(exkey->type==PUB)
					{
					  temp_buf[1] = 0x80;
					}		
					break;
				case ALG_RSA2048_CRT:
					if(exkey->type==PRI)
					{
					  temp_buf[1] = 0x82;
					}
					else if(exkey->type==PUB)
					{
					  temp_buf[1] = 0x80;
					}			
					break;
				case ALG_RSA1024_STANDAND:
					if(exkey->type==PRI)
					{
					  temp_buf[1] = 0x81;
					}
					else if(exkey->type==PUB)
					{
					  temp_buf[1] = 0x80;
					}		
					break;
				case ALG_RSA2048_STANDAND:
					if(exkey->type==PRI)
					{
					  temp_buf[1] = 0x81;
					}
					else if(exkey->type==PUB)
					{
					  temp_buf[1] = 0x80;
					}		
					break;
			}


	//双端队列初始化
	util_queue_init(&queue_in);
	//设置命令头
	tpdu_init_with_id(&command,CMD_EXPORT_KEY);
	//设置P1P2
	tpdu_set_p1p2 (&command,p1,p2);
	//输入数据进入双端队列	
	util_queue_rear_push((uint8_t *)temp_buf,2, &queue_in);
	
	ret = tpdu_execute(&command, (uint8_t *)&queue_in, util_queue_size (&queue_in), (uint8_t *)&queue_out, &temp_len);
		if(ret!=SE_SUCCESS)
			return ret;
	if(if_trasns_key)
	{
	  ret =  apdu_trans_decrypt(&queue_out);
	  if(ret!=SE_SUCCESS)
		return ret;	 
	  off = queue_out.front_node;
	  exkey->val_len = 0;
	  exkey->val_len = exkey->val_len|queue_out.q_buf[off];
      exkey->val_len = (exkey->val_len<<8)|queue_out.q_buf[off+1];	  
	  memcpy(&exkey->val,&queue_out.q_buf[off+2],exkey->val_len);
	}
    else
    {
       off = queue_out.front_node;
	   memcpy(&exkey->val,&queue_out.q_buf[off],queue_out.q_buf_len);
	   exkey->val_len = queue_out.q_buf_len;   
	}
	
	if(exkey->val==NULL||exkey->val_len<16||exkey->val_len>656)
	{
	
		return SE_ERR_DATA;
	}
	return SE_SUCCESS;
}



/**
* @brief 读取密钥信息指令封装
* @param [in] if_app_key  是否读取应用密钥信息
* @param [out] out_buf 
* @param [out] out_buf_len  输出数据长度
* @return 参见error.h
* @note no
* @see 函数  util_queue_init   tpdu_init_with_id  tpdu_set_p1p2  tpdu_execute  util_queue_size
*/
se_error_t  apdu_get_key_info (bool if_app_key, uint8_t *out_buf,uint32_t *out_buf_len)
{
	iso_command_apdu_t command = {0};
	se_error_t ret = 0;
	uint32_t out_len = 0;
	uint32_t off = 0;
	double_queue_node queue_in ={0} ;
	double_queue_node queue_out ={0} ;
	uint32_t Le = 0;
	uint8_t p1 = 0x00;
	
	//双端队列初始化
	util_queue_init(&queue_in);
	util_queue_init(&queue_out);
	//设置命令头
	tpdu_init_with_id(&command,CMD_GET_KEY_INFO);
	if (if_app_key == true)
	{
	    p1 = 0x01;
	}
	else
	{
		p1 = 0x00;
	}
	tpdu_set_p1p2(&command,p1,0x00);
	//设置Le
	tpdu_set_le(&command, Le);
	ret = tpdu_execute(&command, (uint8_t *)&queue_in, util_queue_size (&queue_in), (uint8_t *)&queue_out, &out_len);
	if(ret!=SE_SUCCESS)
		return ret;

	//从双端队列拷贝回输出数据
	off = queue_out.front_node;
	memcpy(out_buf,&queue_out.q_buf[off],queue_out.q_buf_len);
    *out_buf_len = queue_out.q_buf_len;
	if(*out_buf_len!=0x200)
	{	
		LOGE("failed to call apdu_get_key_info!\n");
		return SE_ERR_LENGTH;
	}
	return SE_SUCCESS;
}



/**
* @brief 对称加解密命令封装
* @param [in]  key->alg 算法类型  
* @param [in]  key->id   密钥id
* @param [in]  sym_param->mode 对称加密参数
* @param [in]  sym_param->iv   初始向量
* @param [in]  sym_param->iv_len  初始向量长度
* @param [in]  sym_param->padding_type 补位方式
* @param [in]  in_buf 输入数据
* @param [in]  in_buf_len 输入数据长度
* @param [in]  if_first_block 是否为第一包数据
* @param [in]  if_last_block 是否为最后一包数据
* @param [in] if_enc 加密or解密
* @param [out] out_buf 输出数据
* @param [out] out_buf_len 输出数据长度
* @return 参见error.h
* @note no
* @see 函数  util_queue_init  util_queue_rear_push  tpdu_init_with_id  tpdu_set_p1p2  tpdu_execute  util_queue_size
*/
se_error_t apdu_sym_enc_dec (sym_key_t *key, alg_sym_param_t *sym_param, const uint8_t *in_buf, uint32_t in_buf_len, uint8_t *out_buf, uint32_t *out_buf_len, bool if_first_block, bool if_last_block, bool if_enc)
{
	iso_command_apdu_t command = {0};
	se_error_t ret = 0;
	double_queue_node queue_in ={0} ;
	double_queue_node queue_out ={0} ;
    uint8_t p1 = 0;
	uint8_t p2 = 0;
	uint32_t off = 0;
	uint32_t temp_len =0;
	
	//双端队列初始化
	util_queue_init(&queue_in);
	util_queue_init(&queue_out);
	
	
	//设置p1p2
	//判断是否有级联数据设置p1
	if(if_last_block==false)
	{
	    p1=p1|0x80;
	}
	
	//按照对称算法设置p1
	switch(key->alg)
	{
		case ALG_DES128:
			p1=p1|0x00;
			break;
		case ALG_SM4:
			p1=p1|0x40;
			break;
		case ALG_AES128:
			p1=p1|0x60;
			break;	
	}
	//按照计算类型设置p1
	if(if_enc)
	{
		p1=p1|0x00;
	}
	else
	{
		p1=p1|0x04;
	}
	//按照加解密模式设置p1
	switch(sym_param->mode)
	{
		case SYM_MODE_CBC:
			p1=p1|0x01;
			break;
		case SYM_MODE_ECB:
			p1=p1|0x00;
			break;
	}

	//根据应用密钥KID设置P2
	p2 = key->id;
	
	//判断首包拼接命令头
	if(if_first_block)
	{
		//拼接iv值
		if(sym_param->mode==SYM_MODE_CBC )
			util_queue_rear_push(sym_param->iv,sym_param->iv_len, &queue_in);
	}

	//拼接数据
	util_queue_rear_push((uint8_t *)in_buf,in_buf_len, &queue_in);
	//加密
	if(if_enc)
	{
		//最后一包补位
		if(if_last_block)
		{
			if(sym_param->padding_type==PADDING_PKCS7)
			{
				ret = apdu_sym_padding(sym_param->padding_type, (uint8_t*)&queue_in, in_buf_len,(uint8_t*)&queue_in, out_buf_len);		
				if(ret!=SE_SUCCESS)
					return ret;
			}
		}

	}
    //设置命令头
	tpdu_init_with_id(&command,CMD_CIPHER_DATA);
	//设置p1 p2
	tpdu_set_p1p2 (&command,p1,p2);
    if(queue_in.q_buf_len>0xff)
	{
	  command.isoCase = ISO_CASE_3E;
	}
	else
	{
      command.isoCase = ISO_CASE_3S;
	}
	ret = tpdu_execute(&command, (uint8_t *)&queue_in, util_queue_size (&queue_in), (uint8_t *)&queue_out, &temp_len);
	if(ret!=SE_SUCCESS)
		return ret;


	//解密去补位
	if(!if_enc)
	{
		if(if_last_block)
		{
			if(sym_param->padding_type==PADDING_PKCS7)
			{
				off = queue_out.rear_node;						
				//去补位
				ret = apdu_sym_unpadding(sym_param->padding_type, (uint8_t*)&queue_out,queue_out.q_buf_len ,(uint8_t*)&queue_out, &temp_len);
				if(ret!=SE_SUCCESS)
					return ret;
			}

		}

	}

	off = queue_out.front_node;
	//从双端队列中拷贝出数据
	memcpy(out_buf,&queue_out.q_buf[off],temp_len);
	*out_buf_len = temp_len;

	return SE_SUCCESS;
}




/**
* @brief MAC命令封装
* @param [in]  key->alg 算法类型  
* @param [in]  key->id   密钥id
* @param [in]  sym_param->mode 对称加密参数
* @param [in]  sym_param->iv   初始向量
* @param [in]  sym_param->iv_len  初始向量长度
* @param [in]  sym_param->padding_type 补位方式
* @param [in]  in_buf 输入数据
* @param [in]  in_buf_len 输入数据长度
* @param [in]  mac MAC
* @param [in]  mac_len MAC长度（verify时有效）
* @param [in]  if_first_block 是否为第一包数据
* @param [in]  if_last_block 是否为最后一包数据
* @param [in] if_mac 是否计算mac
* @param [out] mac MAC
* @param [out] mac_len MAC长度（MAC时为有效）
* @return 参见error.h
* @note no
* @see 函数  util_queue_init	 util_queue_rear_push  tpdu_init_with_id  tpdu_set_p1p2  tpdu_execute	util_queue_size
*/
se_error_t apdu_mac (sym_key_t *key, alg_sym_param_t *sym_param, const uint8_t *in_buf, uint32_t in_buf_len, uint8_t *mac, uint32_t *mac_len, bool if_first_block, bool if_last_block, bool if_mac)
{
	iso_command_apdu_t command = {0};
	se_error_t ret = 0;
	double_queue_node queue_in ={0} ;
	double_queue_node queue_out ={0} ;
    uint8_t p1 = 0;
	uint8_t p2 = 0;
	uint32_t off = 0;
	uint32_t temp_len =0;
	
	//双端队列初始化
	util_queue_init(&queue_in);
	util_queue_init(&queue_out);
	//设置命令头
	tpdu_init_with_id(&command,CMD_CIPHER_DATA);
	//设置p1p2
	
	//判断是否有级联数据设置p1
	if(if_last_block==false)
	{
	    p1=p1|0x80;
	}
	
	//按照对称算法设置p1
	switch(key->alg)
	{
		case ALG_DES128:
			p1=p1|0x00;
			break;
		case ALG_SM4:
			p1=p1|0x40;
			break;
		case ALG_AES128:
			p1=p1|0x60;
			break;	
	}
	//按照计算类型设置p1
	if(if_mac)
	{
		p1=p1|0x08;
	}
	else
	{
		p1=p1|0x0C;
	}
	//按照MAC填充方式设置p1
	switch(sym_param->padding_type)
	{
		case PADDING_NOPADDING:
			p1=p1|0x00;
			break;
		case PADDING_ISO9797_M1:
			p1=p1|0x01;
			break;
		case PADDING_ISO9797_M2:
			p1=p1|0x02;
			break;
	}
	//根据应用密钥KID设置P2
	p2 = key->id;
	tpdu_set_p1p2 (&command,p1,p2);
	//判断首包拼接命令头
	if(if_first_block)
	{
		//拼接iv值
		if(sym_param->iv_len!=0)
			util_queue_rear_push(sym_param->iv,sym_param->iv_len, &queue_in);
	}

	//拼接数据
	util_queue_rear_push((uint8_t *)in_buf,in_buf_len, &queue_in);
	if(!if_mac)
	{
		if(if_last_block)
		{

     		//验证MAC，将mac拼接到最后
			util_queue_rear_push((uint8_t *)mac,*mac_len, &queue_in);					
		}

	}
	
	if(queue_in.q_buf_len>0xff)
	   {
		 command.isoCase = ISO_CASE_3E;
	   }
	   else
	   {
		 command.isoCase = ISO_CASE_3S;
	   }

	ret = tpdu_execute(&command, (uint8_t *)&queue_in, util_queue_size (&queue_in), (uint8_t *)&queue_out, &temp_len);
	if(ret!=SE_SUCCESS)
		return ret;


	//计算MAC
	if(if_mac)
	{
		if(if_last_block)
		{
			off = queue_out.front_node;
			//从双端队列中拷贝出数据
			memcpy(mac,&queue_out.q_buf[off],temp_len);
			*mac_len = temp_len;						

		}

	}

	return SE_SUCCESS;

}





/**
* @brief 非对称加密命令封装
* @param [in] key->alg  非对称算法类型(ALG_RSA1024_CRT/ALG_RSA2048_CRT/ALG_ECC256/ALG_SM2)
* @param [in] key->id  公钥id值
* @param [in] asym_param->padding_type  补位方式(PADDING_NOPADDING/PADDING_PKCS1)(仅RSA算法时有效)
* @param [in] in_buf  明文数据 
* @param [in] in_buf_len  明文数据长度
* @param [in] if_last_block  是否为最后一包数据
* @param [out] out_buf  密文数据
* @param [out] out_buf_len  密文数据长度
* @return 参见error.h
* @note no
* @see 函数  util_queue_init	 util_queue_rear_push  tpdu_init_with_id  tpdu_set_p1p2  tpdu_execute	util_queue_size
*/
se_error_t apdu_asym_enc (pub_key_t *key, const alg_asym_param_t *asym_param,const uint8_t *in_buf,uint32_t in_buf_len, uint8_t * out_buf, uint32_t * out_buf_len, bool if_last_block)
{
	iso_command_apdu_t command = {0};
	se_error_t ret = 0;
	double_queue_node queue_in ={0} ;
	double_queue_node queue_out ={0} ;
    uint8_t p1 = 0;
	uint8_t p2 = 0;
	uint32_t off = 0;
	uint32_t temp_len =0;
	uint32_t le = LE_MAX_LEN;

	//双端队列初始化
	util_queue_init(&queue_in);
	util_queue_init(&queue_out);
	//设置命令头
	tpdu_init_with_id(&command,CMD_PKI_ENCIPHER);
	//设置p1p2
	//判断是否有级联数据设置p1
	if(if_last_block==false)
	{
	    p1=p1|0x80;
	}
	
	//按照非对称算法设置p1
	switch(key->alg)
	{
		case ALG_RSA1024_CRT:
			p1=p1|0x00;
			break;
		case ALG_RSA2048_CRT:
			p1=p1|0x00;
			break;
		case ALG_ECC256_NIST:
			p1=p1|0x20;
			break;
		case ALG_SM2:
			p1=p1|0x10;
			break;	
	}
	//按照RSA填充模式设置p1
	if(key->alg == ALG_RSA1024_CRT || key->alg == ALG_RSA2048_CRT )
	{
	    if(asym_param->padding_type==PADDING_NOPADDING)
		{
			p1=p1|0x00;
		}
		else if(asym_param->padding_type==PADDING_PKCS1)
		{
			p1=p1|0x01;
		}
	}
	
	
	//根据应用密钥KID设置P2
	p2 = key->id;
	tpdu_set_p1p2 (&command,p1,p2);


	//输入数据进入双端队列
	util_queue_rear_push((uint8_t *)in_buf,in_buf_len, &queue_in);
    if(queue_in.q_buf_len>0xff)
	{
		command.isoCase = ISO_CASE_3E;
	}
    else
    {
	    command.isoCase = ISO_CASE_3S;
    }
    if(if_last_block)
	{
		command.isoCase = ISO_CASE_4E;
		tpdu_set_le(&command,le);
	}
	ret = tpdu_execute(&command, (uint8_t *)&queue_in, util_queue_size (&queue_in), (uint8_t *)&queue_out, &temp_len);
	if(ret!=SE_SUCCESS&&ret!=SE_ERR_DATA_REMAINING)
		return ret;

	//最后一包后发出
	if(if_last_block)
	{
		off = queue_out.front_node;
		//从双端队列中拷贝出数据
		memcpy(out_buf,&queue_out.q_buf[off],temp_len);
		*out_buf_len = temp_len;
		//返回61XX，再发命令继续读
		while((ret>>8&0xff)==0x61)
		{
			//切换读取命令
			util_queue_init(&queue_in);
			util_queue_init(&queue_out);
			tpdu_init_with_id(&command,CMD_GET_RESPONSE);
			tpdu_set_le (&command,le);
			ret = tpdu_execute(&command, (uint8_t *)&queue_in, util_queue_size (&queue_in), (uint8_t *)&queue_out, &temp_len);
			if(ret!=SE_SUCCESS&&ret!=SE_ERR_DATA_REMAINING)
				return ret;
			//拼接剩余数据
			off = queue_out.front_node;
			//从双端队列中拷贝出数据
			memcpy(out_buf+(*out_buf_len),&queue_out.q_buf[off],temp_len);
			*out_buf_len += temp_len;
		}

	}


	return SE_SUCCESS;
}


/**
* @brief 非对称解密命令封装
* @param [in] ke->alg  非对称算法类型(ALG_RSA1024_CRT/ALG_RSA2048_CRT/ALG_ECC256/ALG_SM2)
* @param [in] key->id  公钥id值
* @param [in] asym_param->padding_type  补位方式(PADDING_NOPADDING/PADDING_PKCS1)(仅RSA算法时有效)
* @param [in] in_buf  明文数据 
* @param [in] in_buf_len  明文数据长度
* @param [in] if_last_block  是否为最后一包数据
* @param [out] out_buf  明文数据
* @param [out] out_buf_len  明文数据长度
* @return 参见error.h
* @note no
* @see 函数  util_queue_init	 util_queue_rear_push  tpdu_init_with_id  tpdu_set_p1p2  tpdu_execute	util_queue_size
*/
se_error_t apdu_asym_dec (pri_key_t *key, const alg_asym_param_t *asym_param,const uint8_t *in_buf,uint32_t in_buf_len, uint8_t * out_buf, uint32_t * out_buf_len, bool if_last_block)
{
	iso_command_apdu_t command = {0};
	se_error_t ret = 0;
	double_queue_node queue_in ={0} ;
	double_queue_node queue_out ={0} ;
    uint8_t p1 = 0;
	uint8_t p2 = 0;
	uint32_t off = 0;
	uint32_t temp_len =0;
	uint32_t le = LE_MAX_LEN;

	//双端队列初始化
	util_queue_init(&queue_in);
	util_queue_init(&queue_out);
	//设置命令头
	tpdu_init_with_id(&command,CMD_PKI_DECIPHER);
	//设置p1p2
	//判断是否有级联数据设置p1
	if(if_last_block==false)
	{
	    p1=p1|0x80;
	}
	
	//按照非对称算法设置p1
	switch(key->alg)
	{
		case ALG_RSA1024_CRT:
			p1=p1|0x00;
			break;
		case ALG_RSA2048_CRT:
			p1=p1|0x00;
			break;
		case ALG_ECC256_NIST:
			p1=p1|0x20;
			break;
		case ALG_SM2:
			p1=p1|0x10;
			break;	
	}
	//按照RSA填充模式设置p1
	if(key->alg == ALG_RSA1024_CRT || key->alg == ALG_RSA2048_CRT )
	{
	    if(asym_param->padding_type==PADDING_NOPADDING)
		{
			p1=p1|0x00;
		}
		else if(asym_param->padding_type==PADDING_PKCS1)
		{
			p1=p1|0x01;
		}
	}

	//根据应用密钥KID设置P2
	p2 = key->id;
	tpdu_set_p1p2 (&command,p1,p2);


	//输入数据进入双端队列
	util_queue_rear_push((uint8_t *)in_buf,in_buf_len, &queue_in);
    if(queue_in.q_buf_len>0xff)
	{
		command.isoCase = ISO_CASE_3E;
	}
    else
    {
	    command.isoCase = ISO_CASE_3S;
    }
    if(if_last_block)
	{
		command.isoCase = ISO_CASE_4E;
		tpdu_set_le(&command,le);
	}
	ret = tpdu_execute(&command, (uint8_t *)&queue_in, util_queue_size (&queue_in), (uint8_t *)&queue_out, &temp_len);
	if(ret!=SE_SUCCESS&&ret!=SE_ERR_DATA_REMAINING)
		return ret;

	//最后一包后发出
	if(if_last_block)
	{
		off = queue_out.front_node;
		//从双端队列中拷贝出数据
		memcpy(out_buf,&queue_out.q_buf[off],temp_len);
		*out_buf_len = temp_len;
		//返回61XX，再发命令继续读
		while((ret>>8&0xff)==0x61)
		{
			//切换读取命令
			util_queue_init(&queue_in);
			util_queue_init(&queue_out);
		    tpdu_init_with_id(&command,CMD_GET_RESPONSE);
			tpdu_set_le (&command,le);
			ret = tpdu_execute(&command, (uint8_t *)&queue_in, util_queue_size (&queue_in), (uint8_t *)&queue_out, &temp_len);
			if(ret!=SE_SUCCESS&&ret!=SE_ERR_DATA_REMAINING)
				return ret;
			//拼接剩余数据
			off = queue_out.front_node;
			//从双端队列中拷贝出数据
			memcpy(out_buf+(*out_buf_len),&queue_out.q_buf[off],temp_len);
			*out_buf_len += temp_len;
		}

	}


	return SE_SUCCESS;
}




/**
* @brief 非对称签名命令封装
* @param [in] key->alg  非对称算法类型(ALG_RSA1024_CRT/ALG_RSA2048_CRT/ALG_ECC256/ALG_SM2)
* @param [in] key->id  公钥id值
* @param [in] asym_param->hash_type 哈希算法 
* @param [in] in_buf  输入数据
* @param [in] in_buf_len  输入数据长度
* @param [in] if_last_block  是否为最后一包数据
* @param [out] sign_buf     签名数据
* @param [out] sign_buf_len 签名数据长度
* @return 参见error.h
* @note no
* @see 函数  util_queue_init	 util_queue_rear_push  tpdu_init_with_id  tpdu_set_p1p2  tpdu_execute	util_queue_size
*/
se_error_t apdu_asym_sign (pub_key_t *key, const alg_asym_param_t *asym_param,const uint8_t *in_buf,uint32_t in_buf_len, uint8_t *sign_buf, uint32_t * sign_buf_len , bool if_last_block)
{
	iso_command_apdu_t command = {0};
	se_error_t ret = 0;
	double_queue_node queue_in ={0} ;
	double_queue_node queue_out ={0} ;
    uint8_t p1 = 0;
	uint8_t p2 = 0;
	uint32_t off = 0;
	uint32_t temp_len =0;

	//双端队列初始化
	util_queue_init(&queue_in);
	util_queue_init(&queue_out);
	//设置命令头
	tpdu_init_with_id(&command,CMD_COMPUTE_SIGNATURE);
	//设置p1p2
	//判断是否有级联数据设置p1
	if(if_last_block==false)
	{
	    p1=p1|0x80;
	}
	
	//按照非对称算法设置p1
	switch(key->alg)
	{
		case ALG_RSA1024_CRT:
			p1=p1|0x00;
			break;
		case ALG_RSA2048_CRT:
			p1=p1|0x00;
			break;
		case ALG_ECC256_NIST:
			p1=p1|0x20;
			break;
		case ALG_SM2:
			p1=p1|0x10;
			break;	
	}
	//按照哈希类型设置p1
	switch(asym_param->hash_type)
	{
		case ALG_NONE:
			p1=p1|0x08;
			break;
		case ALG_SHA1:
			p1=p1|0x00;
			break;
		case ALG_SHA224:
			p1=p1|0x01;
			break;
		case ALG_SHA256:
			p1=p1|0x02;
			break;
		case ALG_SHA384:
			p1=p1|0x03;
			break;
		case ALG_SHA512:
			p1=p1|0x04;
			break;
		case ALG_SM3:
			p1=p1|0x05;
			break;	
	}
	
	//根据应用密钥KID设置P2
	p2 = key->id;
	tpdu_set_p1p2 (&command,p1,p2);

	//输入数据进入双端队列
	util_queue_rear_push((uint8_t *)in_buf,in_buf_len, &queue_in);
    if(queue_in.q_buf_len>0xff)
	{
		command.isoCase = ISO_CASE_3E;
	}
    else
    {
	    command.isoCase = ISO_CASE_3S;
    }
    

	ret = tpdu_execute(&command, (uint8_t *)&queue_in, util_queue_size (&queue_in), (uint8_t *)&queue_out, &temp_len);
	if(ret!=SE_SUCCESS&&ret!=SE_ERR_DATA_REMAINING)
		return ret;
	
	if(if_last_block)
	{
		off = queue_out.front_node;
		//从双端队列中拷贝出数据
		memcpy(sign_buf,&queue_out.q_buf[off],temp_len);
		*sign_buf_len = temp_len; 
	}


	return SE_SUCCESS;
}


/**
* @brief 非对称签名验签命令封装
* @param [in] key->alg  非对称算法类型(ALG_RSA1024_CRT/ALG_RSA2048_CRT/ALG_ECC256/ALG_SM2)
* @param [in] key->id  公钥id值
* @param [in] asym_param->hash_type 哈希算法 
* @param [in] in_buf  输入数据
* @param [in] in_buf_len  输入数据长度
* @param [in] if_last_block  是否为最后一包数据
* @param [in] sign_buf     签名数据
* @param [in] sign_buf_len 签名数据长度
* @return 参见error.h
* @note no
* @see 函数  util_queue_init	 util_queue_rear_push  tpdu_init_with_id  tpdu_set_p1p2  tpdu_execute	util_queue_size
*/
se_error_t apdu_asym_verify (pub_key_t *key, const alg_asym_param_t *asym_param,const uint8_t *in_buf,uint32_t in_buf_len, uint8_t *sign_buf, uint32_t sign_buf_len , bool if_first_block, bool if_last_block)
{
	iso_command_apdu_t command = {0};
	se_error_t ret = 0;
	double_queue_node queue_in ={0} ;
	double_queue_node queue_out ={0} ;
    uint8_t p1 = 0;
	uint8_t p2 = 0;
	uint32_t temp_len =0;

	//双端队列初始化
	util_queue_init(&queue_in);
	util_queue_init(&queue_out);
	//设置命令头
	tpdu_init_with_id(&command,CMD_VERIFY_SIGNATURE);
	//设置p1p2
	//判断是否有级联数据设置p1
	if(if_last_block==false)
	{
	    p1=p1|0x80;
	}
	
	//按照非对称算法设置p1
	switch(key->alg)
	{
		case ALG_RSA1024_CRT:
			p1=p1|0x00;
			break;
		case ALG_RSA2048_CRT:
			p1=p1|0x00;
			break;
		case ALG_ECC256_NIST:
			p1=p1|0x20;
			break;
		case ALG_SM2:
			p1=p1|0x10;
			break;	
	}
	//按照哈希类型设置p1
	switch(asym_param->hash_type)
	{
		case ALG_NONE:
			p1=p1|0x08;
			break;
		case ALG_SHA1:
			p1=p1|0x00;
			break;
		case ALG_SHA224:
			p1=p1|0x01;
			break;
		case ALG_SHA256:
			p1=p1|0x02;
			break;
		case ALG_SHA384:
			p1=p1|0x03;
			break;
		case ALG_SHA512:
			p1=p1|0x04;
			break;
		case ALG_SM3:
			p1=p1|0x05;
			break;	
	}
	
	//根据应用密钥KID设置P2
	p2 = key->id;
	tpdu_set_p1p2 (&command,p1,p2);
	//判断首包拼接命令头
	if(if_first_block)
	{
		//拼接签名数据
		util_queue_rear_push(sign_buf,sign_buf_len, &queue_in);
	}

	//输入数据进入双端队列
	util_queue_rear_push((uint8_t *)in_buf,in_buf_len, &queue_in);
    if(queue_in.q_buf_len>0xff)
	{
		command.isoCase = ISO_CASE_3E;
	}
    else
    {
	    command.isoCase = ISO_CASE_3S;
    }
    

	ret = tpdu_execute(&command, (uint8_t *)&queue_in, util_queue_size (&queue_in), (uint8_t *)&queue_out, &temp_len);
	if(ret!=SE_SUCCESS&&ret!=SE_ERR_DATA_REMAINING)
		return ret;

	return SE_SUCCESS;
}


/**
* @brief SM2 get za命令封装
* @param [in] uid->val  用户ID
* @param [in] uid->val_len   用户ID长度
* @param [in] pub_key->val  SM2公钥值 X|Y
* @param [in] pub_key->val_len   SM2公钥值长度
* @param [out] za	输出数据
* @return 参见error.h
* @note no
* @see 函数  util_queue_init  util_queue_rear_push  tpdu_init_with_id	 tpdu_execute  util_queue_size
*/
se_error_t apdu_sm2_get_za (user_id_t* uid, pub_key_t *pub_key , uint8_t *za )
{
	iso_command_apdu_t command = {0};
	se_error_t ret = 0;
	uint32_t out_len = 0;
	uint32_t off = 0;
	double_queue_node queue_in ={0} ;
	double_queue_node queue_out ={0} ;
	uint8_t temp_data[1] = {0x00};

	if(uid==NULL||pub_key==NULL||za==NULL)
	{
		LOGE("failed to apdu_sm2_get_za pointer params!\n");
		return SE_ERR_PARAM_INVALID;
	}
	if(uid->val_len==0||uid->val_len>32||pub_key->val_len!=64)
	{
		LOGE("failed to apdu_sm2_get_za input params!\n");
		return SE_ERR_PARAM_INVALID;
	}

	//双端队列初始化
	util_queue_init(&queue_in);
	util_queue_init(&queue_out);
	//输入数据存入双端队列 AID长度+AID+公钥
	off = queue_in.front_node;
	temp_data[0] = uid->val_len&0xFF;
	util_queue_rear_push(temp_data,1, &queue_in);
	util_queue_rear_push((uint8_t *)(uid->val),uid->val_len, &queue_in);
	util_queue_rear_push((uint8_t *)(pub_key->val),pub_key->val_len, &queue_in);
	//设置命令头
	tpdu_init_with_id(&command,CMD_SM2_GET_ZA);

	ret = tpdu_execute(&command, (uint8_t *)&queue_in, util_queue_size (&queue_in), (uint8_t *)&queue_out, &out_len);
	if(ret!=SE_SUCCESS)
		return ret;

	//从双端队列拷贝回输出数据
	off = queue_out.front_node;
	memcpy(za,&queue_out.q_buf[off],queue_out.q_buf_len);
 
	return SE_SUCCESS;
}




/**
* @brief 哈希计算命令封装
* @param [in] alg 算法类型
* @param [in]  in_buf 输入数据
* @param [in]  in_buf_len 输入数据长度
* @param [in] if_last_block  是否为最后一包数据
* @param [out] out_buf 输出数据
* @param [out] out_buf_len 输出数据长度
* @return 参见error.h
* @note no
* @see 函数  util_queue_init	 util_queue_rear_push  tpdu_init_with_id  tpdu_set_p1p2  tpdu_execute	util_queue_size
*/
se_error_t apdu_digest (uint32_t alg, const uint8_t *in_buf, uint32_t in_buf_len, uint8_t *out_buf, uint32_t *out_buf_len, bool if_last_block)
{
	iso_command_apdu_t command = {0};
	se_error_t ret = 0;
	double_queue_node queue_in ={0} ;
	double_queue_node queue_out ={0} ;
	uint8_t p1 = 0;
	uint8_t p2 = 0;
	uint32_t off = 0;
	uint32_t temp_len =0;

	//双端队列初始化
	util_queue_init(&queue_in);
	util_queue_init(&queue_out);
	//设置命令头
	tpdu_init_with_id(&command,CMD_DIGEST);
	//设置p1p2
	//判断是否有级联数据设置p1
	if(if_last_block==false)
	{
	    p1=p1|0x80;
	}
	
	//按照非对称算法设置p1
	switch(alg)
	{
		case ALG_SHA1:
			p1=p1|0x00;
			break;
		case ALG_SHA224:
			p1=p1|0x01;
			break;
		case ALG_SHA256:
			p1=p1|0x02;
			break;
		case ALG_SHA384:
			p1=p1|0x03;
			break;
		case ALG_SHA512:
			p1=p1|0x04;
			break;
		case ALG_SM3:
			p1=p1|0x05;
			break;	
	}
	tpdu_set_p1p2 (&command,p1,p2);

	//拼接数据
	util_queue_rear_push((uint8_t *)in_buf,in_buf_len, &queue_in);
	 if(queue_in.q_buf_len>0xff)
	{
		command.isoCase = ISO_CASE_3E;
	}
    else
    {
	    command.isoCase = ISO_CASE_3S;
    }
    
	ret = tpdu_execute(&command, (uint8_t *)&queue_in, util_queue_size (&queue_in), (uint8_t *)&queue_out, &temp_len);
	if(ret!=SE_SUCCESS)
		return ret;
	off = queue_out.front_node;
	//从双端队列中拷贝出数据
	memcpy(out_buf,&queue_out.q_buf[off],temp_len);
	*out_buf_len = temp_len;

	return SE_SUCCESS;
	
}




/**
* @brief 获取随机数命令封装
* @param [in] expected_len  期望的长度
* @param [out] random  获取的随机数
* @return 参见error.h	
* @note no
* @see 函数  util_queue_init  util_queue_rear_push	tpdu_init_with_id  tpdu_execute_no_response
*/
se_error_t apdu_get_random  (uint32_t expected_len, uint8_t *random)
{
  iso_command_apdu_t command = {0};
	se_error_t ret = 0;
	uint32_t out_len = 0;
	uint32_t off = 0;
	double_queue_node queue_in ={0} ;
	double_queue_node queue_out ={0} ;
	
	//双端队列初始化
	util_queue_init(&queue_in);
	util_queue_init(&queue_out);
	//设置命令头
	tpdu_init_with_id(&command,CMD_GET_RANDOM);
	//设置命令头
	tpdu_set_le(&command, expected_len&0xFF);
	ret = tpdu_execute(&command, (uint8_t *)&queue_in, util_queue_size (&queue_in), (uint8_t *)&queue_out, &out_len);
	if(ret!=SE_SUCCESS)
		return ret;

	//从双端队列拷贝回输出数据
	off = queue_out.front_node;
	memcpy(random,&queue_out.q_buf[off],queue_out.q_buf_len);

	return SE_SUCCESS;
}



/**
* @brief 获取SE信息命令封装
* @param [in] type  信息类型
* @param [out] info->CHIP_ID  芯片8字节唯一序列号
* @param [out] info->PRODUCT_INFO   产品信息8字节
* @return 参见error.h
* @note no
* @see 函数  util_queue_init  tpdu_init_with_id  tpdu_set_p1p2  tpdu_execute  util_queue_size
*/
se_error_t  apdu_get_info (info_type type, se_info_t * info)
{
	iso_command_apdu_t command = {0};
	se_error_t ret = 0;
	uint32_t out_len = 0;
	uint32_t off = 0;
	double_queue_node queue_in ={0} ;
	double_queue_node queue_out ={0} ;
	uint32_t Le = 0;
	//双端队列初始化
	util_queue_init(&queue_in);
	util_queue_init(&queue_out);
	//设置命令头
	tpdu_init_with_id(&command,CMD_GET_INFO);
	tpdu_set_p1p2(&command,type&0xFF,0x00);
	//设置Le
	switch(type)
	{
		case CHIP_ID:
			Le=0x08;
			break;
		case PRODUCT_INFO:
			Le=0x08;
			break;
	}
	tpdu_set_le(&command, Le);
	ret = tpdu_execute(&command, (uint8_t *)&queue_in, util_queue_size (&queue_in), (uint8_t *)&queue_out, &out_len);
	if(ret!=SE_SUCCESS)
		return ret;

	//从双端队列拷贝回输出数据
	off = queue_out.front_node;
	if(type==CHIP_ID)
		memcpy(info->chip_id,&queue_out.q_buf[off],queue_out.q_buf_len);
	else if(type==PRODUCT_INFO)
		memcpy(info->product_info,&queue_out.q_buf[off],queue_out.q_buf_len);

	return SE_SUCCESS;
}



/**
* @brief 读SE ID命令封装
* @param [out] se_id->val  返回的SE的ID
* @param [out] se_id->val_len  返回的SE的ID长度
* @return 参见error.h
* @note no
* @see 函数  util_queue_init  tpdu_init_with_id  tpdu_set_le  tpdu_execute  util_queue_size
*/
se_error_t  apdu_get_id (se_id_t *se_id )
{
	iso_command_apdu_t command = {0};
	se_error_t ret = 0;
	uint32_t out_len = 0;
	uint32_t off = 0;
	double_queue_node queue_in ={0} ;
	double_queue_node queue_out ={0} ;

	//双端队列初始化
	util_queue_init(&queue_in);
	util_queue_init(&queue_out);
	//设置命令头
	tpdu_init_with_id(&command,CMD_GET_ID);
	//设置命令头
	tpdu_set_le(&command, 0x02);
	ret = tpdu_execute(&command, (uint8_t *)&queue_in, util_queue_size (&queue_in), (uint8_t *)&queue_out, &out_len);
	if(ret!=SE_SUCCESS)
		return ret;

	//从双端队列拷贝回输出数据
	off = queue_out.front_node;
	memcpy(se_id->val,&queue_out.q_buf[off],queue_out.q_buf_len);
	se_id->val_len=queue_out.q_buf_len;

	return SE_SUCCESS;
}



/**
* @brief 写SEID
* -# 将数据域数据拷贝到queue_in中
* -# 调用tpdu_init_with_id拼接命令头
* -# 调用tpdu_execute_no_response选择文件
* -# 以上过程中发生错误会返回相应错误码
* @param [in] se_id->val  写入的SE的ID
* @param [in] se_id->val_len  写入的SE的ID长度
* @return 参见error.h
* @note no
* @see 函数  util_queue_init  util_queue_rear_push  tpdu_init_with_id  tpdu_execute_no_response
*/
se_error_t  apdu_write_SEID(se_id_t *se_id) 
{
	iso_command_apdu_t command = {0};
	se_error_t ret = 0;
	double_queue_node queue_in ={0} ;
	uint8_t in_buf[2] = {0x00};
  	uint8_t p1 = 0x00;
	uint8_t p2 = 0x00;
	//双端队列初始化
	util_queue_init(&queue_in);
	//输入数据存入双端队列
    util_queue_rear_push(se_id->val,se_id->val_len, &queue_in);
	
	//设置命令头
	tpdu_init_with_id(&command,CMD_WRITE_SEID);
    //设置P1P2
	tpdu_set_p1p2 (&command,p1,p2);
	ret = tpdu_execute_no_response(&command, (uint8_t *)&queue_in, util_queue_size (&queue_in));
	if(ret!=SE_SUCCESS)
		return ret;

	return SE_SUCCESS;

}


/**
* @brief pin修改和重置命令封装
* @param [in] pin->owner pin属主类型
* @param [in] pin->pin_value pin的值
* @param [in] pin->pin_len  取值为0x06-0x10
* @param [in] if_change_pin 是否是修改PIN
* @param [in] in_buf 输入数据
* @param [in] in_buf_len 输入数据长度
* @return 参见error.h
* @note no
* @see no
*/
se_error_t apdu_change_reload_pin(pin_t *pin, const uint8_t *in_buf, uint32_t in_buf_len, bool if_change_pin)
{

	iso_command_apdu_t command = {0};
	se_error_t ret = 0;
	uint8_t p1 = 0x00;
	uint8_t p2 = 0x00;
	double_queue_node queue_in ={0} ;

	//双端队列初始化
	util_queue_init(&queue_in);
	//设置命令头
	tpdu_init_with_id(&command,CMD_CHANGE_RELOAD_PIN);
	//将新pin值加密后放入双端队列
	apdu_pin_encrypt(pin,in_buf,in_buf_len,&queue_in,true);
	//设置P1P2
	if(if_change_pin)
	{
        p1 = 0x01;
		if(pin->owner == ADMIN_PIN)
			p2 = 0x00;
	    if(pin->owner == USER_PIN)
			p2 = 0x01;
	}
	else
	{
		p1 = 0x02;
	}
	tpdu_set_p1p2 (&command,p1,p2);
	
	ret = tpdu_execute_no_response(&command, (uint8_t *)&queue_in, util_queue_size (&queue_in));
	if(ret!=SE_SUCCESS)
		return ret;

	return SE_SUCCESS;
}


/**
* @brief 删除密钥命令封装
* @param [in] pin->owner pin属主类型
* @param [in] pin->pin_value pin的值
* @param [in] pin->pin_len  取值为0x06-0x10
* @return 参见error.h
* @note no
* @see no
*/
se_error_t apdu_verify_pin(pin_t *pin)
{

	iso_command_apdu_t command = {0};
	se_error_t ret = 0;
	uint8_t p1 = 0x00;
	uint8_t p2 = 0x00;
	double_queue_node queue_in ={0} ;

	//双端队列初始化
	util_queue_init(&queue_in);
	//设置命令头
	tpdu_init_with_id(&command,CMD_VERIFY_PIN);
	//将随机数和pin拼接后计算其sm3的值，并将前16字节放入双端队列
	apdu_pin_encrypt(pin,NULL,NULL,&queue_in,false);
	//设置P1P2
	if(pin->owner == ADMIN_PIN)
		p2 = 0x00;
    else if(pin->owner == USER_PIN)
		p2 = 0x01;
	
	tpdu_set_p1p2 (&command,p1,p2);
	
	ret = tpdu_execute_no_response(&command, (uint8_t *)&queue_in, util_queue_size (&queue_in));
	if(ret!=SE_SUCCESS)
		return ret;

	return SE_SUCCESS;
}


/**
* @brief 选择文件命令封装
* @param [in] fid  文件FID
* @return 参见error.h	
* @note no
* @see 函数  util_queue_init  util_queue_rear_push  tpdu_init_with_id  tpdu_execute_no_response
*/
se_error_t  apdu_select_file(uint32_t  fid)
{
	iso_command_apdu_t command = {0};
	se_error_t ret = 0;
	double_queue_node queue_in ={0} ;
	uint8_t in_buf[2] = {0x00};
  uint8_t p1 = 0x00;
	uint8_t p2 = 0x00;
	in_buf[0] = (fid>>8)&0xFF;
	in_buf[1] = (fid)&0xFF;
	//双端队列初始化
	util_queue_init(&queue_in);
	//输入数据存入双端队列
    util_queue_rear_push((uint8_t *)in_buf,0x02, &queue_in);
	
	//设置命令头
	tpdu_init_with_id(&command,CMD_SELECT_FILE);
//设置P1P2
	tpdu_set_p1p2 (&command,p1,p2);
	ret = tpdu_execute_no_response(&command, (uint8_t *)&queue_in, util_queue_size (&queue_in));
	if(ret!=SE_SUCCESS)
		return ret;

	return SE_SUCCESS;
}



/**
* @brief 写文件命令封装
* @param [in] offset 文件偏移
* @param [in] if_encrypt  输入数据是否采用密文
* @param [in] in_buf      输入数据
* @param [in] in_buf_len  输入数据长度
* @return 参见error.h	
* @note no
* @see 函数  util_queue_init  util_queue_rear_push  tpdu_init_with_id  tpdu_execute_no_response
*/
se_error_t  apdu_write_file(uint32_t offset, bool  if_encrypt, const uint8_t *in_buf, uint32_t in_buf_len)
{
	iso_command_apdu_t command = {0};
	se_error_t ret = 0;
	double_queue_node queue_in ={0} ;
	//uint8_t in_buf[2] = {0x00};
	uint8_t p1 = 0x00;
	uint8_t p2 = 0x00;

	p1 = (offset>>8)&0xff;
	p2 = (offset)&0xff;
	//双端队列初始化
	util_queue_init(&queue_in);
		//设置命令头
	tpdu_init_with_id(&command,CMD_WRITE_FILE);
	//设置P1P2
	tpdu_set_p1p2 (&command,p1,p2);
	//判断是否为密文传输，若为密文传输对数据进行加密
	if(if_encrypt==true)
	{
		ret =  apdu_trans_encrypt((uint8_t *)in_buf, in_buf_len,0,&queue_in);//若需要传输密钥加密保护，则对数据加密后进入双端队列	  
		//设置CLA
		tpdu_set_cla (&command,CMD_CLA_CASE2);

	}
    else
	{
	    //输入数据存入双端队列
	    util_queue_rear_push((uint8_t *)in_buf,in_buf_len, &queue_in);
	    //设置CLA
	    tpdu_set_cla (&command,CMD_CLA_CASE1);
    }

    if((queue_in.q_buf_len)>0xff)
	{
	  command.isoCase = ISO_CASE_3E;
	}
	else
	{
      command.isoCase = ISO_CASE_3S;
	} 
	ret = tpdu_execute_no_response(&command, (uint8_t *)&queue_in, util_queue_size (&queue_in));
	if(ret!=SE_SUCCESS)
		return ret;

	return SE_SUCCESS;
}




/**
* @brief 读文件命令封装
* @param [in] offset 文件偏移
* @param [in] if_encrypt  输出数据是否采用密文
* @param [out] out_buf      输出数据
* @param [out] out_buf_len  输出数据长度
* @return 参见error.h	
* @note no
* @see 函数  util_queue_init  util_queue_rear_push  tpdu_init_with_id  tpdu_execute_no_response
*/
se_error_t  apdu_read_file(uint32_t offset, bool  if_encrypt, uint32_t expect_len ,uint8_t *out_buf, uint32_t *out_buf_len)
{
	iso_command_apdu_t command = {0};
	se_error_t ret = 0;
	double_queue_node queue_in ={0} ;
	double_queue_node queue_out ={0} ;
	uint8_t p1 = 0x00;
	uint8_t p2 = 0x00;
    uint32_t off = 0;
	uint32_t le = 0;
	uint32_t temp_len = 0;
	
	p1 = (offset>>8)&0xff;
	p2 = (offset)&0xff;
	//双端队列初始化
	util_queue_init(&queue_in);
	//设置命令头
	tpdu_init_with_id(&command,CMD_READ_FILE); 
	//判断是否为密文传输，若为密文传输对数据进行加密
	if(if_encrypt)
	{
		//设置CLA
		tpdu_set_cla (&command,CMD_CLA_CASE2);
		
	}
    else
	{
	    //设置CLA
	    tpdu_set_cla (&command,CMD_CLA_CASE1);
    }
	//设置P1P2
	tpdu_set_p1p2 (&command,p1,p2);
	//le
	if(if_encrypt)
		le = ((expect_len+1+16)/16)*16;
	else le = expect_len;
	tpdu_set_le(&command, le);
	
	command.isoCase = ISO_CASE_2E;
	
    ret = tpdu_execute(&command, (uint8_t *)&queue_in, util_queue_size (&queue_in), (uint8_t *)&queue_out, &temp_len);
	if(ret!=SE_SUCCESS)
		return ret;

	//从双端队列拷贝回输出数据
	if(if_encrypt==true)
	{
	  ret =  apdu_trans_decrypt(&queue_out);
	  if(ret!=SE_SUCCESS)
		return ret;  
	  off = queue_out.front_node;
	  memcpy(out_buf,&queue_out.q_buf[off+2],expect_len);
	  *out_buf_len = expect_len;  
	}
	else
	{
	   off = queue_out.front_node;
	   memcpy(out_buf,&queue_out.q_buf[off],temp_len);
	  *out_buf_len = temp_len;    
	}

	
	return SE_SUCCESS;
}


/**
* @brief 读文件信息命令封装
* @param [in] if_df 为true时为获取DF信息，为false时为获取EF信息
* @param [out] out_buf      输出数据
* @param [out] out_buf_len  输出数据长度
* @return 参见error.h	
* @note no
* @see 函数  util_queue_init  util_queue_rear_push  tpdu_init_with_id  tpdu_execute_no_response
*/
se_error_t  apdu_get_file_info(bool  if_df,uint8_t *out_buf, uint32_t *out_buf_len)
{
	iso_command_apdu_t command = {0};
	se_error_t ret = 0;
	double_queue_node queue_in ={0} ;
	double_queue_node queue_out ={0} ;
    uint32_t off = 0;
	uint32_t temp_len = 0;
	
	//双端队列初始化
	util_queue_init(&queue_in);
	//设置命令头
	tpdu_init_with_id(&command,CMD_GET_FILE_INFO); 	
	//设置P1P2
	if(if_df)
	  tpdu_set_p1p2 (&command,0x00,0x00);
    else
	  tpdu_set_p1p2 (&command,0x01,0x00);
	//le
	tpdu_set_le(&command, 0x00);
	
	
    ret = tpdu_execute(&command, (uint8_t *)&queue_in, util_queue_size (&queue_in), (uint8_t *)&queue_out, &temp_len);
	if(ret!=SE_SUCCESS)
		return ret;

	//从双端队列拷贝回输出数据	
	off = queue_out.front_node;
	memcpy(out_buf,&queue_out.q_buf[off],temp_len);
	*out_buf_len = temp_len;    
			
	return SE_SUCCESS;
}



/**
* @brief loader下载初始化
* @param [in] image_addr 升级数据地址
* @return 参见error.h
* @note no
* @see 函数  util_queue_init  tpdu_init_with_id  tpdu_set_p1p2  tpdu_execute_no_response  util_queue_size
*/
se_error_t apdu_loader_init(uint8_t* image_addr)
{
	se_error_t ret = SE_SUCCESS;
	iso_command_apdu_t command = {0};
	double_queue_node queue_in ={0} ;
	uint16_t cmd_len = 0;
	uint16_t off = 0;
  uint8_t mac_buf[4]= {0};
	//检查参数
	if(image_addr==NULL)
	{
		LOGE("failed to apdu_loader_init pointer params!\n");
		return SE_ERR_PARAM_INVALID;
	}

	//获取命令长度
	cmd_len = image_addr[off]<<8 | image_addr[off+1];
	off+=2;
	if(cmd_len!=0x0009)
	{
		LOGE("failed to apdu_loader_init input length params!\n");
		return SE_ERR_PARAM_INVALID;
	}
	
	if(memcmp(image_addr+off,"\xBF\x41\x00",3))
	{
		LOGE("failed to apdu_loader_init input params!\n");
		return SE_ERR_PARAM_INVALID;
	}
	
	//双端队列初始化
	util_queue_init(&queue_in);
	
	//设置命令头
	tpdu_init_with_id(&command,CMD_LOADER_INIT);
	//取出4字节MAC值
	memcpy(mac_buf,image_addr+off+5 ,4); 
	//将4字节mac值放入双端队列
	//输入数据进入双端队列
	util_queue_rear_push(mac_buf,4, &queue_in);
	//设置P2参数
	tpdu_set_p1p2 (&command,0x00,image_addr[off+3]);  
	
	ret = tpdu_execute_no_response(&command, (uint8_t *)&queue_in, util_queue_size(&queue_in));
	
	return ret;
}


/**
* @brief loader下载
* @param [in] image_addr 升级数据地址
* @return 参见error.h
* @note no
* @see 函数  util_queue_init  tpdu_init_with_id  util_queue_rear_push  tpdu_execute_no_response  util_queue_size
*/
se_error_t apdu_loader_program(uint8_t* image_addr)
{
	se_error_t ret = SE_SUCCESS;
	iso_command_apdu_t command = {0};
	double_queue_node queue_in ={0} ;
	uint16_t cmd_len = 0;
	uint16_t off = 0;

	//检查参数
	if(image_addr==NULL)
	{
		LOGE("failed to apdu_loader_download pointer params!\n");
		return SE_ERR_PARAM_INVALID;
	}

	//获取命令长度
	cmd_len = (uint16_t)(image_addr[off]<<8 | image_addr[off+1]);
	off+=2;
	
	if(cmd_len>0x2007)
	{
		LOGE("failed to apdu_loader_download input length params!\n");
		return SE_ERR_PARAM_INVALID;
	}
	
	//双端队列初始化
	util_queue_init(&queue_in);
	
	//设置命令头
	tpdu_init_with_id(&command,CMD_LOADER_PROGRAM);
	memcpy(&(command.classbyte), image_addr+off, 5);
	//tpdu_set_cla (&command,image_addr[off++]);
	//tpdu_set_p1p2 (&command,image_addr[off],image_addr[off+1]);
	
	off+=5;
	
	if(cmd_len > 0x104)//如果大于0x104则Lc为3字节
	{
		off+=2;
	}
	//输入数据存入双端队列
	//util_queue_rear_push(image_addr+off,cmd_len-5, &queue_in);
	
	util_queue_rear_push(image_addr+off,cmd_len-off+2, &queue_in);

	ret = tpdu_execute_no_response(&command, (uint8_t *)&queue_in, util_queue_size(&queue_in));

	return ret;
}



/**
* @brief loader校验数据
* @param [in] image_addr 升级数据地址
* @return 参见error.h
* @note no
* @see 函数  util_queue_init  tpdu_init_with_id  util_queue_rear_push  tpdu_execute_no_response  util_queue_size
*/
se_error_t apdu_loader_checkprogram(uint8_t* image_addr)
{
	se_error_t ret = SE_SUCCESS;
	iso_command_apdu_t command = {0};
	double_queue_node queue_in ={0} ;
	uint16_t cmd_len = 0;
	uint16_t off = 0;

	//检查参数
	if(image_addr==NULL)
	{
		LOGE("failed to apdu_loader_checkdata pointer params!\n");
		return SE_ERR_PARAM_INVALID;
	}

	//获取命令长度
	cmd_len = (uint16_t)(image_addr[off]<<8 | image_addr[off+1]);
	off+=2;
	
	if(cmd_len!=0x0025)
	{
		LOGE("failed to apdu_loader_checkdata input length params!\n");
		return SE_ERR_PARAM_INVALID;
	}
	
	if(memcmp(image_addr+off,"\xBF\x42\x00\x00\x20",5))
	{
		LOGE("failed to apdu_loader_checkdata input params!\n");
		return SE_ERR_PARAM_INVALID;
	}	
	
	//双端队列初始化
	util_queue_init(&queue_in);
	
	//设置命令头
	tpdu_init_with_id(&command,CMD_LOADER_CHECKPROGRAM);

	//输入数据存入双端队列
	util_queue_rear_push(image_addr+off+5,cmd_len-5, &queue_in);

	ret = tpdu_execute_no_response(&command, (uint8_t *)&queue_in, util_queue_size(&queue_in));

	return ret;
}


/**
* @brief V2X GENERATE KEY DERIVE SEED命令封装
* @param [in]  derive_type    衍生类型，预留，默认0x00
* @param [out] out_buf    衍生数据信息，参见derive_seed_t
* @note no
* @see 函数  util_queue_init   tpdu_init_with_id  tpdu_set_le  tpdu_execute  util_queue_size
*/
se_error_t  apdu_v2x_gen_key_derive_seed (uint8_t derive_type,derive_seed_t* out_buf)
{
	iso_command_apdu_t command = {0};
	se_error_t ret = 0;
	uint32_t out_len = 0;
	uint32_t off = 0;
	double_queue_node queue_in ={0} ;
	double_queue_node queue_out ={0} ;
    
	//双端队列初始化
	util_queue_init(&queue_in);
	util_queue_init(&queue_out);
	//设置命令头
	tpdu_init_with_id(&command,CMD_V2X_GENERATE_KEY_DERIVE_SEED);
	//设置le
	tpdu_set_le(&command, 0xA0);
	ret = tpdu_execute(&command, (uint8_t *)&queue_in, util_queue_size (&queue_in), (uint8_t *)&queue_out, &out_len);
	if(ret!=SE_SUCCESS)
		return ret;

	//从双端队列拷贝回输出数据
	off = queue_out.front_node;
	memcpy(out_buf->SM4_KS,&queue_out.q_buf[off],16);
	memcpy(out_buf->SM4_KE,&queue_out.q_buf[off + 16],16);
	memcpy(out_buf->SM2_A,&queue_out.q_buf[off + 32],64);
	memcpy(out_buf->SM2_P,&queue_out.q_buf[off + 96],64);

	return SE_SUCCESS;
}



/**
* @brief V2X RECONSITUTION KEY命令封装
* @param [in]  in_buf      重构数据信息(重构私钥ID-1B|i-4B｜j-4B|CTij)
* @param [in]  in_buf_len  重构数据信息长度
* @param [out] out_buf     假名证书
* @param [out] out_buf_len 假名证书长度
* @return 参见error.h
* @note no
* @see 函数  util_queue_init   tpdu_init_with_id  tpdu_set_le  tpdu_execute  util_queue_size
*/
se_error_t  apdu_v2x_reconsitution_key (uint8_t *in_buf, uint32_t *in_buf_len,uint8_t *out_buf, uint32_t *out_buf_len)
{
	iso_command_apdu_t command = {0};
	se_error_t ret = 0;
	uint32_t out_len = 0;
	uint32_t off = 0;
	uint8_t p1 = 0;
	uint8_t p2 = 0;
	double_queue_node queue_in ={0} ;
	double_queue_node queue_out ={0} ;
    
	//双端队列初始化
	util_queue_init(&queue_in);
	util_queue_init(&queue_out);
	//设置命令头
	tpdu_init_with_id(&command,CMD_V2X_RECONSITUTION_KEY);
	//设置le
	tpdu_set_le(&command, 0x00);
	//根据重构私钥ID设置P2
	memcpy(&p2, in_buf, 1);
	tpdu_set_p1p2 (&command,p1,p2);
	//输入数据进入双向队列
	util_queue_rear_push(in_buf+1,*in_buf_len-1, &queue_in);
	ret = tpdu_execute(&command, (uint8_t *)&queue_in, util_queue_size (&queue_in), (uint8_t *)&queue_out, &out_len);
	if(ret!=SE_SUCCESS)
		return ret;
  
	//从双端队列拷贝回输出数据
	off = queue_out.front_node;
	memcpy(out_buf,&queue_out.q_buf[off],out_len);
	*out_buf_len = out_len;
	
	return SE_SUCCESS;
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


