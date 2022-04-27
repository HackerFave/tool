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
* @brief ���ô�����Կ�����ݽ��м��ܺ����˫�˶���
* @param [in] input  Դ����
* @param [in] input_len   Դ���ݳ���
* @param [in] off ˫�˶����в�������������ݵ�ƫ��      
* @param [in] in_queue  �����˫�˶��е�ַ
* @return �μ�error.h
* @note no
* @see ����  util_queue_rear_push 
*/
se_error_t apdu_trans_encrypt	(uint8_t *input, uint32_t input_len,uint32_t off,double_queue in_queue)
{
	
	uint8_t len_high=0x00;
	uint8_t len_low =0x00;
	uint8_t pading[16]={0}; 
	
	sm4_ctxt.mode = SM4_ENCRYPT;
	sm4_setkey_enc(&sm4_ctxt,trans_key);
	//���������ݳ�����˫�˶���
	len_high=input_len>>8;
	util_queue_rear_push(&len_high,1, in_queue);
	len_low=input_len&0xff;
	util_queue_rear_push(&len_low,1, in_queue);
	//�������ݽ���˫�˶���
	util_queue_rear_push((uint8_t *)input,input_len, in_queue);
	//�ж��Ƿ�80
	if((in_queue->q_buf_len-off)% 16!=0)
	{   //��80
		pading[0] = 0x80;
		util_queue_rear_push(pading,1, in_queue);
		//�ж��Ƿ�00
		if((in_queue->q_buf_len-off) % 16!=0)
		{
		  memset(pading,0x00,16);
		  util_queue_rear_push(pading,16-(in_queue->q_buf_len-off)%16, in_queue);
		}			
	}
	//SM4������Կ��������
	sm4_crypt_ecb(&sm4_ctxt,SM4_ENCRYPT,in_queue->q_buf_len-off, (unsigned char *) &(in_queue->q_buf[in_queue->front_node+off]), (unsigned char *) &(in_queue->q_buf[in_queue->front_node+off])); 

	return SE_SUCCESS;
}


/**
* @brief ���ô�����Կ�Լ������ݽ��н��ܺ����˫�˶���     
* @param [in] in_queue  �����˫�˶��е�ַ
* @return �μ�error.h
* @note no
* @see ����  util_queue_rear_push 
*/
se_error_t apdu_trans_decrypt	(double_queue in_queue)
{
	sm4_ctxt.mode = SM4_DECRYPT;
	sm4_setkey_dec(&sm4_ctxt,trans_key);
	//SM4������Կ��������
	sm4_crypt_ecb(&sm4_ctxt,SM4_DECRYPT,in_queue->q_buf_len, (unsigned char *) &(in_queue->q_buf[in_queue->front_node]), (unsigned char *) &(in_queue->q_buf[in_queue->front_node])); 

	return SE_SUCCESS;
}


/**
* @brief ����pin���ܺ����˫�˶���
* @param [in] pin->owner pin��������
* @param [in] pin->pin_value pin��ֵ
* @param [in] pin->pin_len  ȡֵΪ0x06-0x10
* @param [in] in_buf ��������
* @param [in] in_buf_len �������ݳ���
* @param [in] if_xor �Ƿ���������㣨veryfypin��ʱ��ֻ����sm3ժҪ��ȡǰ16�ֽڣ�
* @param [out] queue_out  �����˫�˶��е�ַ
* @return �μ�error.h
* @note no
* @see ����  util_queue_rear_push    sm3  tpdu_execute 
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

	//����ȡ�����ָ�����˫�˶���queue_out
	//��������ͷ
	tpdu_init_with_id(&command,CMD_GET_RANDOM);
	//����le
	tpdu_set_le(&command, 0x10);
	ret = tpdu_execute(&command, (uint8_t *)&queue_in, util_queue_size (&queue_in), (uint8_t *)queue_out, &out_len);
	if(ret!=SE_SUCCESS)
		return ret;
	
	//16�ֽ������+pinֵƴ�ӷ���˫�˶���queue_out
	util_queue_rear_push(pin->pin_value,pin->pin_len, queue_out);
	
	//sm3ժҪ����
	sm3(queue_out->q_buf+queue_out->front_node, queue_out->q_buf_len, sm3_buf);
    
	if(if_xor)
	{
		//����pinǰ������ֽ�Ld��pin_buf[0]=0��
		pin_buf[1]=in_buf_len&0xff;
		//����pin��ֵ����pin_buf
		memcpy(pin_buf+2,in_buf,in_buf_len);
		in_buf_len = in_buf_len + 2;	

		//�ж��Ƿ�80
		if((in_buf_len)% 16!=0)
		{   //��80
			pin_buf[in_buf_len] = 0x80;
			in_buf_len = in_buf_len + 1;
			//�ж��Ƿ�00
			if(in_buf_len % 16!=0)
			{
			  if(in_buf_len<16)
			  	in_buf_len = 16;//����pinֵ�ĳ���С�ڵ���16�ֽڣ�����80��С��16�ֽڣ��򳤶�ȡ16�ֽ�
			  else if(16<in_buf_len)
			  	in_buf_len = 32;//����pinֵ�ĳ��ȴ��ڵ���16�ֽڣ�����80�Դ���16�ֽڣ��򳤶�ȥȡ32�ֽ�
			}			
		}
		
		//����ժҪ�����ǰ16�ֽ���Ϊ��Կ����pinֵ�������
		for( j = 0;j < in_buf_len/16; j++)
		{
			for( i = 0; i < 16; i++)
				pin_buf[i+j*16] = sm3_buf[i] ^ pin_buf[i+j*16];
		}

		//������������˫�˶��� 
		util_queue_init(queue_out);
	    util_queue_rear_push(pin_buf,in_buf_len, queue_out);
	}
    else
    {
    	//������������˫�˶��� 
		util_queue_init(queue_out);
	    util_queue_rear_push(sm3_buf,16, queue_out);
	    
	}
	return SE_SUCCESS;
}



/**
* @brief �Գ��㷨��λ
* @param [in] padding  ��λ��ʽ
* @param [in] input  Դ����
* @param [in] input_len   Դ���ݳ���
* @param [out] output  ��λ�������
* @param [out] output_len  ��λ������ݳ���
* @return �μ�error.h
* @note no
* @see ����  util_queue_rear_push 
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
* @brief �Գ��㷨ȥ��λ
* @param [in] padding  ��λ��ʽ
* @param [in] input  Դ����
* @param [in] input_len   Դ���ݳ���
* @param [out] output  ȥ��λ����
* @param [out] output_len  ȥ��λ���ݳ���
* @return �μ�error.h
* @note no
* @see ����  util_queue_rear_pop 
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
* @brief ��ȡ��λ����
* @param [in] alg  �㷨����
* @param [in] padding  ��λ��ʽ
* @param [in] input_len   ԭʼ����
* @return ��λ��ĳ���	
* @note no
* @see no
*/
uint32_t apdu_sym_padding_length(uint32_t alg, uint32_t padding, uint32_t input_len)
{
	uint32_t buffer_len = 0;
	uint32_t align_size = 0;
	//���÷��鳤��
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
* @brief �л�����ģʽ�����װ
* @param [in] type  ����ģʽ
* @return �μ�error.h	
* @note no
* @see ����  util_queue_init  tpdu_init_with_id  tpdu_set_p1p2  tpdu_execute_no_response
*/
se_error_t apdu_switch_mode (work_mode  type)
{
	iso_command_apdu_t command = {0};
	se_error_t ret = 0;
	double_queue_node queue_in ={0} ;
	uint32_t p2 = 0;
	
	//˫�˶��г�ʼ��
	util_queue_init(&queue_in);
	//��������ͷ
	tpdu_init_with_id(&command,CMD_ENTER_LOWPOWER);
	//����P1P2
	p2 = 0x00;
	tpdu_set_p1p2 (&command,0x00,p2);

	ret = tpdu_execute_no_response(&command, (uint8_t *)&queue_in, util_queue_size (&queue_in));
	if(ret!=SE_SUCCESS)
		return ret;	
	
	
	return SE_SUCCESS;
}


/**
* @brief ������������װ
* @param [in] ctrlcode  ��������
* @param [in] in_buf  ��������
* @param [in] in_buf_len   �������ݳ��� 
* @param [out] out_buf  �������
* @param [out] out_buf_len  ������ݳ���
* @return �μ�error.h
* @note no
* @see no
*/
se_error_t apdu_control(ctrl_type ctrlcode, const uint8_t *in_buf, uint32_t in_buf_len, uint8_t *out_buf, uint32_t *out_buf_len)
{
	return SE_SUCCESS;
}


/**
* @brief �豸��֤�����װ
* @param [in] in_buf  ��������
* @param [in] in_buf_len   �������ݳ��� 
* @return �μ�error.h
* @note no
* @see no
*/
se_error_t apdu_ext_auth(const uint8_t *in_buf, uint32_t in_buf_len)
{

	iso_command_apdu_t command = {0};
	se_error_t ret = 0;
	double_queue_node queue_in ={0} ;
	//������
	if(in_buf==NULL||in_buf_len==0)
	{  
		LOGE("failed to apdu_ext_auth input params!\n");
		return SE_ERR_PARAM_INVALID;
	}

	//˫�˶��г�ʼ��
	util_queue_init(&queue_in);
	//�������ݴ���˫�˶���
    util_queue_rear_push((uint8_t *)in_buf,in_buf_len, &queue_in);
	//��������ͷ
	tpdu_init_with_id(&command,CMD_EXTTERN_AUTH);
	ret = tpdu_execute_no_response(&command, (uint8_t *)&queue_in, util_queue_size (&queue_in));
	if(ret!=SE_SUCCESS)
		return ret;

	return SE_SUCCESS;
}


/**
* @brief д����Կ����PIN�����װ
* @param [in] in_buf  ������Կֵ/��Կ��Ϣ
* @param [in] in_buf_len   �������ݳ���
* @param [in] if_encrypt  �Ƿ����
* @param [in] if_write  �Ƿ�д����Կ 
* @param [in] if_update_mkey  �Ƿ����������Կ
* @return �μ�error.h
* @note no
* @see ����  util_queue_init	 util_queue_rear_push  tpdu_init_with_id  tpdu_set_cla  tpdu_set_p1p2  tpdu_execute_no_response  util_queue_size
*/
se_error_t apdu_write_key(const uint8_t *in_buf, uint32_t in_buf_len, bool if_encrypt, bool if_write, bool if_update_mkey )
{
  iso_command_apdu_t command = {0};
	se_error_t ret = 0;
	double_queue_node queue_in ={0} ;
	//������
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
	//˫�˶��г�ʼ��
	util_queue_init(&queue_in);
	//��������ͷ
	tpdu_init_with_id(&command,CMD_WRITE_KEY);
	//����P1P2
	//�������ɺ͸��£�����P1=0x00������P1=0x01������������ԿP2=0x00������������ԿP2=0x01
	if(if_write==true)
		tpdu_set_p1p2 (&command,0x00,0x00);
	else if(if_update_mkey==true)
		tpdu_set_p1p2 (&command,0x01,0x00);
	else if(if_update_mkey==false)
	    tpdu_set_p1p2 (&command,0x01,0x01);
	//�ж��Ƿ�Ϊ���Ĵ��䣬��Ϊ���Ĵ�������ݽ��м���
	if(if_encrypt)
	{
		ret =  apdu_trans_encrypt((uint8_t *)in_buf, in_buf_len,0,&queue_in);//����Ҫ������Կ���ܱ�����������ݼ��ܺ����˫�˶���	  
		//����CLA
		tpdu_set_cla (&command,CMD_CLA_CASE4);

	}
    else
	{
	    //�������ݴ���˫�˶���
	    util_queue_rear_push((uint8_t *)in_buf,in_buf_len, &queue_in);
	    //����CLA
	    tpdu_set_cla (&command,CMD_CLA_CASE3);
    }
	ret = tpdu_execute_no_response(&command, (uint8_t *)&queue_in, util_queue_size (&queue_in));
	if(ret!=SE_SUCCESS)
		return ret;

	return SE_SUCCESS;

}




/**
* @brief ����/���·ǶԳƹ�˽Կ�������װ
* @param [in] pub_key->alg �㷨���� 
* @param [in] pub_key->id ��Կid
* @param [in] pri_key->id ˽Կid
* @param [in] symkey->alg �Գ��㷨����
* @param [in] symkey->id �Գ���Կid
* @param [out] pub_key->val ��Կֵ
* @param [out] pub_key->val_len ��Կֵ����
* @return �μ�error.h
* @note no
* @see ����  util_queue_init  util_queue_rear_push  tpdu_init_with_id  tpdu_set_p1p2  tpdu_execute  util_queue_size
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

	//˫�˶��г�ʼ��
	util_queue_init(&queue_in);
	util_queue_init(&queue_out);
    //��������ͷ
	tpdu_init_with_id(&command,CMD_GENERATE_KEY);
	//ת��Ϊ�ڲ���Կ����
	temp_buf[KEYUSAGE] = 0x02;//��Կ��Գ���Կ��Կ������;ΪӦ����Կ
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
	if(pub_key->id != pri_key->id)//��ͬ��kid���ɷǶԳ���Կ
	{
        //��Կkidȡֵ
        temp_buf[KID] = pub_key->id;
        //˽Կkidȡֵ
        temp_buf[KID+pri_offset] = pri_key->id;
		//˽Կ������;ΪӦ����Կ
    	temp_buf[KEYUSAGE+pri_offset] = 0x02;
        //�㷨���ͺ�ģ��ȡֵ����
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
	else if(pub_key->id == pri_key->id)//��ͬ��kid���ɷǶԳ���Կ�Զ�
	{
		  //��˽Կ�Զ�kidȡֵ
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

	//��˫�˶��п������������
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
* @brief ����shared key�����װ
* @param [in] shared_key->alg  �㷨����  
* @param [in] input  ��������
* @param [in] input_len   �������ݳ���
* @param [in] if_return_key  �Ƿ񷵻�key
* @param [in] if_return_s  �Ƿ񷵻�s
* @param [out] shared_key  shared keyֵ
* @param [out] sm2_s  sֵ
* @return �μ�error.h
* @note no
* @see ����  util_queue_init  tpdu_init_with_id  tpdu_set_p1p2  util_queue_rear_push  tpdu_execute
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


	//˫�˶��г�ʼ��
	util_queue_init(&queue_in);
	util_queue_init(&queue_out);

	//��������ͷ
	tpdu_init_with_id(&command,CMD_GEN_SHARED_KEY);
	
	//key�Ƿ񵼳� 0x00-���� 0x80������
	return_key_flag = (if_return_key==true)?0x10:0x00;
	//s�Ƿ񵼳� 0x01-���� 0x00������
	return_s_flag =  (if_return_s==true)?0x20:0x00;
	//����P1P2
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

	//�������ݴ���˫�˶���
	util_queue_rear_push(in_buf,in_buf_len,&queue_in);

	ret = tpdu_execute(&command, (uint8_t *)&queue_in, util_queue_size (&queue_in), (uint8_t *)&queue_out, &out_len);
	if(ret!=SE_SUCCESS)
		return ret;

	//��˫�˶��п������������
	off = queue_out.front_node;

	if(shared_key->alg==ALG_ECDH_ECC256||shared_key->alg==ALG_ECDH_SM2)
	{
		//ECC ����32�ֽ�key
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
* @brief ɾ����Կ�����װ
* @param [in] id ��ɾ����Կid
* @return �μ�error.h
* @note no
* @see no
*/
se_error_t apdu_delete_key(uint8_t id)
{

	iso_command_apdu_t command = {0};
	se_error_t ret = 0;
	double_queue_node queue_in ={0} ;

	//˫�˶��г�ʼ��
	util_queue_init(&queue_in);
	//��������ͷ
	tpdu_init_with_id(&command,CMD_DEL_KEY);
	//����P1P2
	tpdu_set_p1p2 (&command,0x00,id);
	
	ret = tpdu_execute_no_response(&command, (uint8_t *)&queue_in, util_queue_size (&queue_in));
	if(ret!=SE_SUCCESS)
		return ret;

	return SE_SUCCESS;
}


/**
* @brief ������Կ�����װ
* @param [in] ukey->alg �㷨����(ALG_RSA1024_CRT/ALG_RSA2048_CRT/ALG_ECC256_NIST/ALG_SM2/ALG_AES128/ALG_DES128/ALG_SM4)
* @param [in] ukey->id  Ӧ����Կid��������˽ԿKID������öԳ���Կ��kid��
* @param [in] inkey->alg �㷨����(ALG_RSA1024_CRT/ALG_RSA2048_CRT/ALG_ECC256_NIST/ALG_SM2/ALG_AES128/ALG_DES128/ALG_SM4)
* @param [in] inkey->id   ��Կ��ʶ������ΧΪ0x01-0xFF
* @param [in] inkey->type ��Կ����
* @param [in] inkey->val  ��Կֵ
* @param [in] inkey->val_len  �Ự��Կֵ����
* @param [in] if_cipher  true-���� false-����
* @param [in] if_trasns_key  �Ƿ���ô�����Կ���ܵ�����Կ
* @return �μ�error.h
* @note no
* @see ����  util_queue_init	 util_queue_rear_push  tpdu_init_with_id  tpdu_set_p1p2	 tpdu_execute	util_queue_size
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
	

	//˫�˶��г�ʼ��
	util_queue_init(&queue_in);
	util_queue_init(&queue_out);
	
	//p1��p2ȡֵ��ʼ��
	if(if_cipher)
	{
        //�������ĵ��룬p1 bit5��1
		p1=p1|0x10;
		//���ô�����Կ���ܴ��䣬p1 bit6��1
		if (if_trasns_key)
		{
			p1=p1|0x20;
			//������Կ�㷨����SM4�㷨��p1 bit4~bit1ȡֵΪ'0100'
			p1=p1|0x04;
			//p2ȡֵΪ������Կid '0x02'
			p2=0x02;
		}
		else
		{
	        //����Ӧ�ü�����Կ�㷨����p1
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
            //�������ڼ��ܵ�Ӧ����Կ�㷨��������p1����
			p1=p1|cipher_alg;
		    //p2ȡֵΪӦ����Կid
		    p2=ukey->id;

		}
	    
	}

	//��֯���������ݣ���Կ����+��Կֵ����/���ģ���������˫�˶���
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
	
	//��Կ���Դ���˫�˶���
	util_queue_rear_push((uint8_t *)temp_buf,8, &queue_in);

	//�������ݽ���˫�˶���
	if(if_trasns_key)
	{
	  ret =  apdu_trans_encrypt((uint8_t *)inkey->val, inkey->val_len,8,&queue_in);//����Ҫ������Կ���ܱ�����������ݼ��ܺ����˫�˶���		
	  if(ret!=SE_SUCCESS)
		return ret;	 
	}
    else
    {
	   util_queue_rear_push((uint8_t *)inkey->val,inkey->val_len, &queue_in);
	}
	
	
	//��������ͷ
	tpdu_init_with_id(&command,CMD_IMPORT_KEY);
	if((queue_in.q_buf_len+8)>0xff)
	{
	  command.isoCase = ISO_CASE_3E;
	}
	else
	{
      command.isoCase = ISO_CASE_3S;
	}
	//����P1P2
	tpdu_set_p1p2 (&command,p1,p2);
	ret = tpdu_execute_no_response(&command, (uint8_t *)&queue_in, util_queue_size(&queue_in));
    if(ret!=SE_SUCCESS)
   	 return ret;

	return SE_SUCCESS;

}


/**
* @brief ������Կ�����װ
* @param [in] ukey->alg �㷨����(ALG_RSA1024_CRT/ALG_RSA2048_CRT/ALG_ECC256_NIST/ALG_SM2/ALG_AES128/ALG_DES128/ALG_SM4)
* @param [in] ukey->id  Ӧ����Կid�������ù�ԿKID������öԳ���Կ��kid��
* @param [in] exkey->alg �㷨����(ALG_RSA1024_CRT/ALG_RSA2048_CRT/ALG_ECC256_NIST/ALG_SM2/ALG_AES128/ALG_DES128/ALG_SM4)
* @param [in] exkey->id   ��Կ��ʶ������ΧΪ0x01-0xFF
* @param [in] exkey->type ��Կ����
* @param [in] if_cipher  true-���� false-����
* @param [out] exkey->val  ��Կֵ
* @param [out] exkey->val_len  �Ự��Կֵ����
* @param [in] if_trasns_key  �Ƿ���ô�����Կ���ܵ�����Կ
* @return �μ�error.h
* @note no
* @see ����  util_queue_init	 util_queue_rear_push  tpdu_init_with_id  tpdu_set_p1p2  tpdu_execute  util_queue_size
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
	//p1��p2ȡֵ��ʼ��

	if(if_cipher==true)
	{
        //�������ĵ�����p1 bit5��1
		p1=p1|0x10;
		//���ô�����Կ���ܴ��䣬p1 bit6��1
		if (if_trasns_key)
		{
			p1=p1|0x20;
			//������Կ�㷨����SM4�㷨��p1 bit4~bit1ȡֵΪ'0100'
			p1=p1|0x04;
			//p2ȡֵΪ������Կid '0x02'
			p2=0x02;
		}
		else
		{
	        //����Ӧ�ü�����Կ�㷨����p1
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
            //�������ڼ��ܵ�Ӧ����Կ�㷨��������p1����
			p1=p1|cipher_alg;
		    //p2ȡֵΪӦ����Կid
		    p2=ukey->id;

		}
	    
	}
	//��֯���������ݣ���Կ����+��Կֵ����/���ģ���������˫�˶���
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


	//˫�˶��г�ʼ��
	util_queue_init(&queue_in);
	//��������ͷ
	tpdu_init_with_id(&command,CMD_EXPORT_KEY);
	//����P1P2
	tpdu_set_p1p2 (&command,p1,p2);
	//�������ݽ���˫�˶���	
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
* @brief ��ȡ��Կ��Ϣָ���װ
* @param [in] if_app_key  �Ƿ��ȡӦ����Կ��Ϣ
* @param [out] out_buf 
* @param [out] out_buf_len  ������ݳ���
* @return �μ�error.h
* @note no
* @see ����  util_queue_init   tpdu_init_with_id  tpdu_set_p1p2  tpdu_execute  util_queue_size
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
	
	//˫�˶��г�ʼ��
	util_queue_init(&queue_in);
	util_queue_init(&queue_out);
	//��������ͷ
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
	//����Le
	tpdu_set_le(&command, Le);
	ret = tpdu_execute(&command, (uint8_t *)&queue_in, util_queue_size (&queue_in), (uint8_t *)&queue_out, &out_len);
	if(ret!=SE_SUCCESS)
		return ret;

	//��˫�˶��п������������
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
* @brief �ԳƼӽ��������װ
* @param [in]  key->alg �㷨����  
* @param [in]  key->id   ��Կid
* @param [in]  sym_param->mode �ԳƼ��ܲ���
* @param [in]  sym_param->iv   ��ʼ����
* @param [in]  sym_param->iv_len  ��ʼ��������
* @param [in]  sym_param->padding_type ��λ��ʽ
* @param [in]  in_buf ��������
* @param [in]  in_buf_len �������ݳ���
* @param [in]  if_first_block �Ƿ�Ϊ��һ������
* @param [in]  if_last_block �Ƿ�Ϊ���һ������
* @param [in] if_enc ����or����
* @param [out] out_buf �������
* @param [out] out_buf_len ������ݳ���
* @return �μ�error.h
* @note no
* @see ����  util_queue_init  util_queue_rear_push  tpdu_init_with_id  tpdu_set_p1p2  tpdu_execute  util_queue_size
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
	
	//˫�˶��г�ʼ��
	util_queue_init(&queue_in);
	util_queue_init(&queue_out);
	
	
	//����p1p2
	//�ж��Ƿ��м�����������p1
	if(if_last_block==false)
	{
	    p1=p1|0x80;
	}
	
	//���նԳ��㷨����p1
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
	//���ռ�����������p1
	if(if_enc)
	{
		p1=p1|0x00;
	}
	else
	{
		p1=p1|0x04;
	}
	//���ռӽ���ģʽ����p1
	switch(sym_param->mode)
	{
		case SYM_MODE_CBC:
			p1=p1|0x01;
			break;
		case SYM_MODE_ECB:
			p1=p1|0x00;
			break;
	}

	//����Ӧ����ԿKID����P2
	p2 = key->id;
	
	//�ж��װ�ƴ������ͷ
	if(if_first_block)
	{
		//ƴ��ivֵ
		if(sym_param->mode==SYM_MODE_CBC )
			util_queue_rear_push(sym_param->iv,sym_param->iv_len, &queue_in);
	}

	//ƴ������
	util_queue_rear_push((uint8_t *)in_buf,in_buf_len, &queue_in);
	//����
	if(if_enc)
	{
		//���һ����λ
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
    //��������ͷ
	tpdu_init_with_id(&command,CMD_CIPHER_DATA);
	//����p1 p2
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


	//����ȥ��λ
	if(!if_enc)
	{
		if(if_last_block)
		{
			if(sym_param->padding_type==PADDING_PKCS7)
			{
				off = queue_out.rear_node;						
				//ȥ��λ
				ret = apdu_sym_unpadding(sym_param->padding_type, (uint8_t*)&queue_out,queue_out.q_buf_len ,(uint8_t*)&queue_out, &temp_len);
				if(ret!=SE_SUCCESS)
					return ret;
			}

		}

	}

	off = queue_out.front_node;
	//��˫�˶����п���������
	memcpy(out_buf,&queue_out.q_buf[off],temp_len);
	*out_buf_len = temp_len;

	return SE_SUCCESS;
}




/**
* @brief MAC�����װ
* @param [in]  key->alg �㷨����  
* @param [in]  key->id   ��Կid
* @param [in]  sym_param->mode �ԳƼ��ܲ���
* @param [in]  sym_param->iv   ��ʼ����
* @param [in]  sym_param->iv_len  ��ʼ��������
* @param [in]  sym_param->padding_type ��λ��ʽ
* @param [in]  in_buf ��������
* @param [in]  in_buf_len �������ݳ���
* @param [in]  mac MAC
* @param [in]  mac_len MAC���ȣ�verifyʱ��Ч��
* @param [in]  if_first_block �Ƿ�Ϊ��һ������
* @param [in]  if_last_block �Ƿ�Ϊ���һ������
* @param [in] if_mac �Ƿ����mac
* @param [out] mac MAC
* @param [out] mac_len MAC���ȣ�MACʱΪ��Ч��
* @return �μ�error.h
* @note no
* @see ����  util_queue_init	 util_queue_rear_push  tpdu_init_with_id  tpdu_set_p1p2  tpdu_execute	util_queue_size
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
	
	//˫�˶��г�ʼ��
	util_queue_init(&queue_in);
	util_queue_init(&queue_out);
	//��������ͷ
	tpdu_init_with_id(&command,CMD_CIPHER_DATA);
	//����p1p2
	
	//�ж��Ƿ��м�����������p1
	if(if_last_block==false)
	{
	    p1=p1|0x80;
	}
	
	//���նԳ��㷨����p1
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
	//���ռ�����������p1
	if(if_mac)
	{
		p1=p1|0x08;
	}
	else
	{
		p1=p1|0x0C;
	}
	//����MAC��䷽ʽ����p1
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
	//����Ӧ����ԿKID����P2
	p2 = key->id;
	tpdu_set_p1p2 (&command,p1,p2);
	//�ж��װ�ƴ������ͷ
	if(if_first_block)
	{
		//ƴ��ivֵ
		if(sym_param->iv_len!=0)
			util_queue_rear_push(sym_param->iv,sym_param->iv_len, &queue_in);
	}

	//ƴ������
	util_queue_rear_push((uint8_t *)in_buf,in_buf_len, &queue_in);
	if(!if_mac)
	{
		if(if_last_block)
		{

     		//��֤MAC����macƴ�ӵ����
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


	//����MAC
	if(if_mac)
	{
		if(if_last_block)
		{
			off = queue_out.front_node;
			//��˫�˶����п���������
			memcpy(mac,&queue_out.q_buf[off],temp_len);
			*mac_len = temp_len;						

		}

	}

	return SE_SUCCESS;

}





/**
* @brief �ǶԳƼ��������װ
* @param [in] key->alg  �ǶԳ��㷨����(ALG_RSA1024_CRT/ALG_RSA2048_CRT/ALG_ECC256/ALG_SM2)
* @param [in] key->id  ��Կidֵ
* @param [in] asym_param->padding_type  ��λ��ʽ(PADDING_NOPADDING/PADDING_PKCS1)(��RSA�㷨ʱ��Ч)
* @param [in] in_buf  �������� 
* @param [in] in_buf_len  �������ݳ���
* @param [in] if_last_block  �Ƿ�Ϊ���һ������
* @param [out] out_buf  ��������
* @param [out] out_buf_len  �������ݳ���
* @return �μ�error.h
* @note no
* @see ����  util_queue_init	 util_queue_rear_push  tpdu_init_with_id  tpdu_set_p1p2  tpdu_execute	util_queue_size
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

	//˫�˶��г�ʼ��
	util_queue_init(&queue_in);
	util_queue_init(&queue_out);
	//��������ͷ
	tpdu_init_with_id(&command,CMD_PKI_ENCIPHER);
	//����p1p2
	//�ж��Ƿ��м�����������p1
	if(if_last_block==false)
	{
	    p1=p1|0x80;
	}
	
	//���շǶԳ��㷨����p1
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
	//����RSA���ģʽ����p1
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
	
	
	//����Ӧ����ԿKID����P2
	p2 = key->id;
	tpdu_set_p1p2 (&command,p1,p2);


	//�������ݽ���˫�˶���
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

	//���һ���󷢳�
	if(if_last_block)
	{
		off = queue_out.front_node;
		//��˫�˶����п���������
		memcpy(out_buf,&queue_out.q_buf[off],temp_len);
		*out_buf_len = temp_len;
		//����61XX���ٷ����������
		while((ret>>8&0xff)==0x61)
		{
			//�л���ȡ����
			util_queue_init(&queue_in);
			util_queue_init(&queue_out);
			tpdu_init_with_id(&command,CMD_GET_RESPONSE);
			tpdu_set_le (&command,le);
			ret = tpdu_execute(&command, (uint8_t *)&queue_in, util_queue_size (&queue_in), (uint8_t *)&queue_out, &temp_len);
			if(ret!=SE_SUCCESS&&ret!=SE_ERR_DATA_REMAINING)
				return ret;
			//ƴ��ʣ������
			off = queue_out.front_node;
			//��˫�˶����п���������
			memcpy(out_buf+(*out_buf_len),&queue_out.q_buf[off],temp_len);
			*out_buf_len += temp_len;
		}

	}


	return SE_SUCCESS;
}


/**
* @brief �ǶԳƽ��������װ
* @param [in] ke->alg  �ǶԳ��㷨����(ALG_RSA1024_CRT/ALG_RSA2048_CRT/ALG_ECC256/ALG_SM2)
* @param [in] key->id  ��Կidֵ
* @param [in] asym_param->padding_type  ��λ��ʽ(PADDING_NOPADDING/PADDING_PKCS1)(��RSA�㷨ʱ��Ч)
* @param [in] in_buf  �������� 
* @param [in] in_buf_len  �������ݳ���
* @param [in] if_last_block  �Ƿ�Ϊ���һ������
* @param [out] out_buf  ��������
* @param [out] out_buf_len  �������ݳ���
* @return �μ�error.h
* @note no
* @see ����  util_queue_init	 util_queue_rear_push  tpdu_init_with_id  tpdu_set_p1p2  tpdu_execute	util_queue_size
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

	//˫�˶��г�ʼ��
	util_queue_init(&queue_in);
	util_queue_init(&queue_out);
	//��������ͷ
	tpdu_init_with_id(&command,CMD_PKI_DECIPHER);
	//����p1p2
	//�ж��Ƿ��м�����������p1
	if(if_last_block==false)
	{
	    p1=p1|0x80;
	}
	
	//���շǶԳ��㷨����p1
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
	//����RSA���ģʽ����p1
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

	//����Ӧ����ԿKID����P2
	p2 = key->id;
	tpdu_set_p1p2 (&command,p1,p2);


	//�������ݽ���˫�˶���
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

	//���һ���󷢳�
	if(if_last_block)
	{
		off = queue_out.front_node;
		//��˫�˶����п���������
		memcpy(out_buf,&queue_out.q_buf[off],temp_len);
		*out_buf_len = temp_len;
		//����61XX���ٷ����������
		while((ret>>8&0xff)==0x61)
		{
			//�л���ȡ����
			util_queue_init(&queue_in);
			util_queue_init(&queue_out);
		    tpdu_init_with_id(&command,CMD_GET_RESPONSE);
			tpdu_set_le (&command,le);
			ret = tpdu_execute(&command, (uint8_t *)&queue_in, util_queue_size (&queue_in), (uint8_t *)&queue_out, &temp_len);
			if(ret!=SE_SUCCESS&&ret!=SE_ERR_DATA_REMAINING)
				return ret;
			//ƴ��ʣ������
			off = queue_out.front_node;
			//��˫�˶����п���������
			memcpy(out_buf+(*out_buf_len),&queue_out.q_buf[off],temp_len);
			*out_buf_len += temp_len;
		}

	}


	return SE_SUCCESS;
}




/**
* @brief �ǶԳ�ǩ�������װ
* @param [in] key->alg  �ǶԳ��㷨����(ALG_RSA1024_CRT/ALG_RSA2048_CRT/ALG_ECC256/ALG_SM2)
* @param [in] key->id  ��Կidֵ
* @param [in] asym_param->hash_type ��ϣ�㷨 
* @param [in] in_buf  ��������
* @param [in] in_buf_len  �������ݳ���
* @param [in] if_last_block  �Ƿ�Ϊ���һ������
* @param [out] sign_buf     ǩ������
* @param [out] sign_buf_len ǩ�����ݳ���
* @return �μ�error.h
* @note no
* @see ����  util_queue_init	 util_queue_rear_push  tpdu_init_with_id  tpdu_set_p1p2  tpdu_execute	util_queue_size
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

	//˫�˶��г�ʼ��
	util_queue_init(&queue_in);
	util_queue_init(&queue_out);
	//��������ͷ
	tpdu_init_with_id(&command,CMD_COMPUTE_SIGNATURE);
	//����p1p2
	//�ж��Ƿ��м�����������p1
	if(if_last_block==false)
	{
	    p1=p1|0x80;
	}
	
	//���շǶԳ��㷨����p1
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
	//���չ�ϣ��������p1
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
	
	//����Ӧ����ԿKID����P2
	p2 = key->id;
	tpdu_set_p1p2 (&command,p1,p2);

	//�������ݽ���˫�˶���
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
		//��˫�˶����п���������
		memcpy(sign_buf,&queue_out.q_buf[off],temp_len);
		*sign_buf_len = temp_len; 
	}


	return SE_SUCCESS;
}


/**
* @brief �ǶԳ�ǩ����ǩ�����װ
* @param [in] key->alg  �ǶԳ��㷨����(ALG_RSA1024_CRT/ALG_RSA2048_CRT/ALG_ECC256/ALG_SM2)
* @param [in] key->id  ��Կidֵ
* @param [in] asym_param->hash_type ��ϣ�㷨 
* @param [in] in_buf  ��������
* @param [in] in_buf_len  �������ݳ���
* @param [in] if_last_block  �Ƿ�Ϊ���һ������
* @param [in] sign_buf     ǩ������
* @param [in] sign_buf_len ǩ�����ݳ���
* @return �μ�error.h
* @note no
* @see ����  util_queue_init	 util_queue_rear_push  tpdu_init_with_id  tpdu_set_p1p2  tpdu_execute	util_queue_size
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

	//˫�˶��г�ʼ��
	util_queue_init(&queue_in);
	util_queue_init(&queue_out);
	//��������ͷ
	tpdu_init_with_id(&command,CMD_VERIFY_SIGNATURE);
	//����p1p2
	//�ж��Ƿ��м�����������p1
	if(if_last_block==false)
	{
	    p1=p1|0x80;
	}
	
	//���շǶԳ��㷨����p1
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
	//���չ�ϣ��������p1
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
	
	//����Ӧ����ԿKID����P2
	p2 = key->id;
	tpdu_set_p1p2 (&command,p1,p2);
	//�ж��װ�ƴ������ͷ
	if(if_first_block)
	{
		//ƴ��ǩ������
		util_queue_rear_push(sign_buf,sign_buf_len, &queue_in);
	}

	//�������ݽ���˫�˶���
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
* @brief SM2 get za�����װ
* @param [in] uid->val  �û�ID
* @param [in] uid->val_len   �û�ID����
* @param [in] pub_key->val  SM2��Կֵ X|Y
* @param [in] pub_key->val_len   SM2��Կֵ����
* @param [out] za	�������
* @return �μ�error.h
* @note no
* @see ����  util_queue_init  util_queue_rear_push  tpdu_init_with_id	 tpdu_execute  util_queue_size
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

	//˫�˶��г�ʼ��
	util_queue_init(&queue_in);
	util_queue_init(&queue_out);
	//�������ݴ���˫�˶��� AID����+AID+��Կ
	off = queue_in.front_node;
	temp_data[0] = uid->val_len&0xFF;
	util_queue_rear_push(temp_data,1, &queue_in);
	util_queue_rear_push((uint8_t *)(uid->val),uid->val_len, &queue_in);
	util_queue_rear_push((uint8_t *)(pub_key->val),pub_key->val_len, &queue_in);
	//��������ͷ
	tpdu_init_with_id(&command,CMD_SM2_GET_ZA);

	ret = tpdu_execute(&command, (uint8_t *)&queue_in, util_queue_size (&queue_in), (uint8_t *)&queue_out, &out_len);
	if(ret!=SE_SUCCESS)
		return ret;

	//��˫�˶��п������������
	off = queue_out.front_node;
	memcpy(za,&queue_out.q_buf[off],queue_out.q_buf_len);
 
	return SE_SUCCESS;
}




/**
* @brief ��ϣ���������װ
* @param [in] alg �㷨����
* @param [in]  in_buf ��������
* @param [in]  in_buf_len �������ݳ���
* @param [in] if_last_block  �Ƿ�Ϊ���һ������
* @param [out] out_buf �������
* @param [out] out_buf_len ������ݳ���
* @return �μ�error.h
* @note no
* @see ����  util_queue_init	 util_queue_rear_push  tpdu_init_with_id  tpdu_set_p1p2  tpdu_execute	util_queue_size
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

	//˫�˶��г�ʼ��
	util_queue_init(&queue_in);
	util_queue_init(&queue_out);
	//��������ͷ
	tpdu_init_with_id(&command,CMD_DIGEST);
	//����p1p2
	//�ж��Ƿ��м�����������p1
	if(if_last_block==false)
	{
	    p1=p1|0x80;
	}
	
	//���շǶԳ��㷨����p1
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

	//ƴ������
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
	//��˫�˶����п���������
	memcpy(out_buf,&queue_out.q_buf[off],temp_len);
	*out_buf_len = temp_len;

	return SE_SUCCESS;
	
}




/**
* @brief ��ȡ����������װ
* @param [in] expected_len  �����ĳ���
* @param [out] random  ��ȡ�������
* @return �μ�error.h	
* @note no
* @see ����  util_queue_init  util_queue_rear_push	tpdu_init_with_id  tpdu_execute_no_response
*/
se_error_t apdu_get_random  (uint32_t expected_len, uint8_t *random)
{
  iso_command_apdu_t command = {0};
	se_error_t ret = 0;
	uint32_t out_len = 0;
	uint32_t off = 0;
	double_queue_node queue_in ={0} ;
	double_queue_node queue_out ={0} ;
	
	//˫�˶��г�ʼ��
	util_queue_init(&queue_in);
	util_queue_init(&queue_out);
	//��������ͷ
	tpdu_init_with_id(&command,CMD_GET_RANDOM);
	//��������ͷ
	tpdu_set_le(&command, expected_len&0xFF);
	ret = tpdu_execute(&command, (uint8_t *)&queue_in, util_queue_size (&queue_in), (uint8_t *)&queue_out, &out_len);
	if(ret!=SE_SUCCESS)
		return ret;

	//��˫�˶��п������������
	off = queue_out.front_node;
	memcpy(random,&queue_out.q_buf[off],queue_out.q_buf_len);

	return SE_SUCCESS;
}



/**
* @brief ��ȡSE��Ϣ�����װ
* @param [in] type  ��Ϣ����
* @param [out] info->CHIP_ID  оƬ8�ֽ�Ψһ���к�
* @param [out] info->PRODUCT_INFO   ��Ʒ��Ϣ8�ֽ�
* @return �μ�error.h
* @note no
* @see ����  util_queue_init  tpdu_init_with_id  tpdu_set_p1p2  tpdu_execute  util_queue_size
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
	//˫�˶��г�ʼ��
	util_queue_init(&queue_in);
	util_queue_init(&queue_out);
	//��������ͷ
	tpdu_init_with_id(&command,CMD_GET_INFO);
	tpdu_set_p1p2(&command,type&0xFF,0x00);
	//����Le
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

	//��˫�˶��п������������
	off = queue_out.front_node;
	if(type==CHIP_ID)
		memcpy(info->chip_id,&queue_out.q_buf[off],queue_out.q_buf_len);
	else if(type==PRODUCT_INFO)
		memcpy(info->product_info,&queue_out.q_buf[off],queue_out.q_buf_len);

	return SE_SUCCESS;
}



/**
* @brief ��SE ID�����װ
* @param [out] se_id->val  ���ص�SE��ID
* @param [out] se_id->val_len  ���ص�SE��ID����
* @return �μ�error.h
* @note no
* @see ����  util_queue_init  tpdu_init_with_id  tpdu_set_le  tpdu_execute  util_queue_size
*/
se_error_t  apdu_get_id (se_id_t *se_id )
{
	iso_command_apdu_t command = {0};
	se_error_t ret = 0;
	uint32_t out_len = 0;
	uint32_t off = 0;
	double_queue_node queue_in ={0} ;
	double_queue_node queue_out ={0} ;

	//˫�˶��г�ʼ��
	util_queue_init(&queue_in);
	util_queue_init(&queue_out);
	//��������ͷ
	tpdu_init_with_id(&command,CMD_GET_ID);
	//��������ͷ
	tpdu_set_le(&command, 0x02);
	ret = tpdu_execute(&command, (uint8_t *)&queue_in, util_queue_size (&queue_in), (uint8_t *)&queue_out, &out_len);
	if(ret!=SE_SUCCESS)
		return ret;

	//��˫�˶��п������������
	off = queue_out.front_node;
	memcpy(se_id->val,&queue_out.q_buf[off],queue_out.q_buf_len);
	se_id->val_len=queue_out.q_buf_len;

	return SE_SUCCESS;
}



/**
* @brief дSEID
* -# �����������ݿ�����queue_in��
* -# ����tpdu_init_with_idƴ������ͷ
* -# ����tpdu_execute_no_responseѡ���ļ�
* -# ���Ϲ����з�������᷵����Ӧ������
* @param [in] se_id->val  д���SE��ID
* @param [in] se_id->val_len  д���SE��ID����
* @return �μ�error.h
* @note no
* @see ����  util_queue_init  util_queue_rear_push  tpdu_init_with_id  tpdu_execute_no_response
*/
se_error_t  apdu_write_SEID(se_id_t *se_id) 
{
	iso_command_apdu_t command = {0};
	se_error_t ret = 0;
	double_queue_node queue_in ={0} ;
	uint8_t in_buf[2] = {0x00};
  	uint8_t p1 = 0x00;
	uint8_t p2 = 0x00;
	//˫�˶��г�ʼ��
	util_queue_init(&queue_in);
	//�������ݴ���˫�˶���
    util_queue_rear_push(se_id->val,se_id->val_len, &queue_in);
	
	//��������ͷ
	tpdu_init_with_id(&command,CMD_WRITE_SEID);
    //����P1P2
	tpdu_set_p1p2 (&command,p1,p2);
	ret = tpdu_execute_no_response(&command, (uint8_t *)&queue_in, util_queue_size (&queue_in));
	if(ret!=SE_SUCCESS)
		return ret;

	return SE_SUCCESS;

}


/**
* @brief pin�޸ĺ����������װ
* @param [in] pin->owner pin��������
* @param [in] pin->pin_value pin��ֵ
* @param [in] pin->pin_len  ȡֵΪ0x06-0x10
* @param [in] if_change_pin �Ƿ����޸�PIN
* @param [in] in_buf ��������
* @param [in] in_buf_len �������ݳ���
* @return �μ�error.h
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

	//˫�˶��г�ʼ��
	util_queue_init(&queue_in);
	//��������ͷ
	tpdu_init_with_id(&command,CMD_CHANGE_RELOAD_PIN);
	//����pinֵ���ܺ����˫�˶���
	apdu_pin_encrypt(pin,in_buf,in_buf_len,&queue_in,true);
	//����P1P2
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
* @brief ɾ����Կ�����װ
* @param [in] pin->owner pin��������
* @param [in] pin->pin_value pin��ֵ
* @param [in] pin->pin_len  ȡֵΪ0x06-0x10
* @return �μ�error.h
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

	//˫�˶��г�ʼ��
	util_queue_init(&queue_in);
	//��������ͷ
	tpdu_init_with_id(&command,CMD_VERIFY_PIN);
	//���������pinƴ�Ӻ������sm3��ֵ������ǰ16�ֽڷ���˫�˶���
	apdu_pin_encrypt(pin,NULL,NULL,&queue_in,false);
	//����P1P2
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
* @brief ѡ���ļ������װ
* @param [in] fid  �ļ�FID
* @return �μ�error.h	
* @note no
* @see ����  util_queue_init  util_queue_rear_push  tpdu_init_with_id  tpdu_execute_no_response
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
	//˫�˶��г�ʼ��
	util_queue_init(&queue_in);
	//�������ݴ���˫�˶���
    util_queue_rear_push((uint8_t *)in_buf,0x02, &queue_in);
	
	//��������ͷ
	tpdu_init_with_id(&command,CMD_SELECT_FILE);
//����P1P2
	tpdu_set_p1p2 (&command,p1,p2);
	ret = tpdu_execute_no_response(&command, (uint8_t *)&queue_in, util_queue_size (&queue_in));
	if(ret!=SE_SUCCESS)
		return ret;

	return SE_SUCCESS;
}



/**
* @brief д�ļ������װ
* @param [in] offset �ļ�ƫ��
* @param [in] if_encrypt  ���������Ƿ��������
* @param [in] in_buf      ��������
* @param [in] in_buf_len  �������ݳ���
* @return �μ�error.h	
* @note no
* @see ����  util_queue_init  util_queue_rear_push  tpdu_init_with_id  tpdu_execute_no_response
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
	//˫�˶��г�ʼ��
	util_queue_init(&queue_in);
		//��������ͷ
	tpdu_init_with_id(&command,CMD_WRITE_FILE);
	//����P1P2
	tpdu_set_p1p2 (&command,p1,p2);
	//�ж��Ƿ�Ϊ���Ĵ��䣬��Ϊ���Ĵ�������ݽ��м���
	if(if_encrypt==true)
	{
		ret =  apdu_trans_encrypt((uint8_t *)in_buf, in_buf_len,0,&queue_in);//����Ҫ������Կ���ܱ�����������ݼ��ܺ����˫�˶���	  
		//����CLA
		tpdu_set_cla (&command,CMD_CLA_CASE2);

	}
    else
	{
	    //�������ݴ���˫�˶���
	    util_queue_rear_push((uint8_t *)in_buf,in_buf_len, &queue_in);
	    //����CLA
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
* @brief ���ļ������װ
* @param [in] offset �ļ�ƫ��
* @param [in] if_encrypt  ��������Ƿ��������
* @param [out] out_buf      �������
* @param [out] out_buf_len  ������ݳ���
* @return �μ�error.h	
* @note no
* @see ����  util_queue_init  util_queue_rear_push  tpdu_init_with_id  tpdu_execute_no_response
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
	//˫�˶��г�ʼ��
	util_queue_init(&queue_in);
	//��������ͷ
	tpdu_init_with_id(&command,CMD_READ_FILE); 
	//�ж��Ƿ�Ϊ���Ĵ��䣬��Ϊ���Ĵ�������ݽ��м���
	if(if_encrypt)
	{
		//����CLA
		tpdu_set_cla (&command,CMD_CLA_CASE2);
		
	}
    else
	{
	    //����CLA
	    tpdu_set_cla (&command,CMD_CLA_CASE1);
    }
	//����P1P2
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

	//��˫�˶��п������������
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
* @brief ���ļ���Ϣ�����װ
* @param [in] if_df ΪtrueʱΪ��ȡDF��Ϣ��ΪfalseʱΪ��ȡEF��Ϣ
* @param [out] out_buf      �������
* @param [out] out_buf_len  ������ݳ���
* @return �μ�error.h	
* @note no
* @see ����  util_queue_init  util_queue_rear_push  tpdu_init_with_id  tpdu_execute_no_response
*/
se_error_t  apdu_get_file_info(bool  if_df,uint8_t *out_buf, uint32_t *out_buf_len)
{
	iso_command_apdu_t command = {0};
	se_error_t ret = 0;
	double_queue_node queue_in ={0} ;
	double_queue_node queue_out ={0} ;
    uint32_t off = 0;
	uint32_t temp_len = 0;
	
	//˫�˶��г�ʼ��
	util_queue_init(&queue_in);
	//��������ͷ
	tpdu_init_with_id(&command,CMD_GET_FILE_INFO); 	
	//����P1P2
	if(if_df)
	  tpdu_set_p1p2 (&command,0x00,0x00);
    else
	  tpdu_set_p1p2 (&command,0x01,0x00);
	//le
	tpdu_set_le(&command, 0x00);
	
	
    ret = tpdu_execute(&command, (uint8_t *)&queue_in, util_queue_size (&queue_in), (uint8_t *)&queue_out, &temp_len);
	if(ret!=SE_SUCCESS)
		return ret;

	//��˫�˶��п������������	
	off = queue_out.front_node;
	memcpy(out_buf,&queue_out.q_buf[off],temp_len);
	*out_buf_len = temp_len;    
			
	return SE_SUCCESS;
}



/**
* @brief loader���س�ʼ��
* @param [in] image_addr �������ݵ�ַ
* @return �μ�error.h
* @note no
* @see ����  util_queue_init  tpdu_init_with_id  tpdu_set_p1p2  tpdu_execute_no_response  util_queue_size
*/
se_error_t apdu_loader_init(uint8_t* image_addr)
{
	se_error_t ret = SE_SUCCESS;
	iso_command_apdu_t command = {0};
	double_queue_node queue_in ={0} ;
	uint16_t cmd_len = 0;
	uint16_t off = 0;
  uint8_t mac_buf[4]= {0};
	//������
	if(image_addr==NULL)
	{
		LOGE("failed to apdu_loader_init pointer params!\n");
		return SE_ERR_PARAM_INVALID;
	}

	//��ȡ�����
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
	
	//˫�˶��г�ʼ��
	util_queue_init(&queue_in);
	
	//��������ͷ
	tpdu_init_with_id(&command,CMD_LOADER_INIT);
	//ȡ��4�ֽ�MACֵ
	memcpy(mac_buf,image_addr+off+5 ,4); 
	//��4�ֽ�macֵ����˫�˶���
	//�������ݽ���˫�˶���
	util_queue_rear_push(mac_buf,4, &queue_in);
	//����P2����
	tpdu_set_p1p2 (&command,0x00,image_addr[off+3]);  
	
	ret = tpdu_execute_no_response(&command, (uint8_t *)&queue_in, util_queue_size(&queue_in));
	
	return ret;
}


/**
* @brief loader����
* @param [in] image_addr �������ݵ�ַ
* @return �μ�error.h
* @note no
* @see ����  util_queue_init  tpdu_init_with_id  util_queue_rear_push  tpdu_execute_no_response  util_queue_size
*/
se_error_t apdu_loader_program(uint8_t* image_addr)
{
	se_error_t ret = SE_SUCCESS;
	iso_command_apdu_t command = {0};
	double_queue_node queue_in ={0} ;
	uint16_t cmd_len = 0;
	uint16_t off = 0;

	//������
	if(image_addr==NULL)
	{
		LOGE("failed to apdu_loader_download pointer params!\n");
		return SE_ERR_PARAM_INVALID;
	}

	//��ȡ�����
	cmd_len = (uint16_t)(image_addr[off]<<8 | image_addr[off+1]);
	off+=2;
	
	if(cmd_len>0x2007)
	{
		LOGE("failed to apdu_loader_download input length params!\n");
		return SE_ERR_PARAM_INVALID;
	}
	
	//˫�˶��г�ʼ��
	util_queue_init(&queue_in);
	
	//��������ͷ
	tpdu_init_with_id(&command,CMD_LOADER_PROGRAM);
	memcpy(&(command.classbyte), image_addr+off, 5);
	//tpdu_set_cla (&command,image_addr[off++]);
	//tpdu_set_p1p2 (&command,image_addr[off],image_addr[off+1]);
	
	off+=5;
	
	if(cmd_len > 0x104)//�������0x104��LcΪ3�ֽ�
	{
		off+=2;
	}
	//�������ݴ���˫�˶���
	//util_queue_rear_push(image_addr+off,cmd_len-5, &queue_in);
	
	util_queue_rear_push(image_addr+off,cmd_len-off+2, &queue_in);

	ret = tpdu_execute_no_response(&command, (uint8_t *)&queue_in, util_queue_size(&queue_in));

	return ret;
}



/**
* @brief loaderУ������
* @param [in] image_addr �������ݵ�ַ
* @return �μ�error.h
* @note no
* @see ����  util_queue_init  tpdu_init_with_id  util_queue_rear_push  tpdu_execute_no_response  util_queue_size
*/
se_error_t apdu_loader_checkprogram(uint8_t* image_addr)
{
	se_error_t ret = SE_SUCCESS;
	iso_command_apdu_t command = {0};
	double_queue_node queue_in ={0} ;
	uint16_t cmd_len = 0;
	uint16_t off = 0;

	//������
	if(image_addr==NULL)
	{
		LOGE("failed to apdu_loader_checkdata pointer params!\n");
		return SE_ERR_PARAM_INVALID;
	}

	//��ȡ�����
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
	
	//˫�˶��г�ʼ��
	util_queue_init(&queue_in);
	
	//��������ͷ
	tpdu_init_with_id(&command,CMD_LOADER_CHECKPROGRAM);

	//�������ݴ���˫�˶���
	util_queue_rear_push(image_addr+off+5,cmd_len-5, &queue_in);

	ret = tpdu_execute_no_response(&command, (uint8_t *)&queue_in, util_queue_size(&queue_in));

	return ret;
}


/**
* @brief V2X GENERATE KEY DERIVE SEED�����װ
* @param [in]  derive_type    �������ͣ�Ԥ����Ĭ��0x00
* @param [out] out_buf    ����������Ϣ���μ�derive_seed_t
* @note no
* @see ����  util_queue_init   tpdu_init_with_id  tpdu_set_le  tpdu_execute  util_queue_size
*/
se_error_t  apdu_v2x_gen_key_derive_seed (uint8_t derive_type,derive_seed_t* out_buf)
{
	iso_command_apdu_t command = {0};
	se_error_t ret = 0;
	uint32_t out_len = 0;
	uint32_t off = 0;
	double_queue_node queue_in ={0} ;
	double_queue_node queue_out ={0} ;
    
	//˫�˶��г�ʼ��
	util_queue_init(&queue_in);
	util_queue_init(&queue_out);
	//��������ͷ
	tpdu_init_with_id(&command,CMD_V2X_GENERATE_KEY_DERIVE_SEED);
	//����le
	tpdu_set_le(&command, 0xA0);
	ret = tpdu_execute(&command, (uint8_t *)&queue_in, util_queue_size (&queue_in), (uint8_t *)&queue_out, &out_len);
	if(ret!=SE_SUCCESS)
		return ret;

	//��˫�˶��п������������
	off = queue_out.front_node;
	memcpy(out_buf->SM4_KS,&queue_out.q_buf[off],16);
	memcpy(out_buf->SM4_KE,&queue_out.q_buf[off + 16],16);
	memcpy(out_buf->SM2_A,&queue_out.q_buf[off + 32],64);
	memcpy(out_buf->SM2_P,&queue_out.q_buf[off + 96],64);

	return SE_SUCCESS;
}



/**
* @brief V2X RECONSITUTION KEY�����װ
* @param [in]  in_buf      �ع�������Ϣ(�ع�˽ԿID-1B|i-4B��j-4B|CTij)
* @param [in]  in_buf_len  �ع�������Ϣ����
* @param [out] out_buf     ����֤��
* @param [out] out_buf_len ����֤�鳤��
* @return �μ�error.h
* @note no
* @see ����  util_queue_init   tpdu_init_with_id  tpdu_set_le  tpdu_execute  util_queue_size
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
    
	//˫�˶��г�ʼ��
	util_queue_init(&queue_in);
	util_queue_init(&queue_out);
	//��������ͷ
	tpdu_init_with_id(&command,CMD_V2X_RECONSITUTION_KEY);
	//����le
	tpdu_set_le(&command, 0x00);
	//�����ع�˽ԿID����P2
	memcpy(&p2, in_buf, 1);
	tpdu_set_p1p2 (&command,p1,p2);
	//�������ݽ���˫�����
	util_queue_rear_push(in_buf+1,*in_buf_len-1, &queue_in);
	ret = tpdu_execute(&command, (uint8_t *)&queue_in, util_queue_size (&queue_in), (uint8_t *)&queue_out, &out_len);
	if(ret!=SE_SUCCESS)
		return ret;
  
	//��˫�˶��п������������
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


