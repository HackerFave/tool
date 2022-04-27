/**@file  tpdu.c
* @brief  tpdu interface definition
* @author  liangww
* @date  2021-04-28
* @version	V1.0
* @copyright  Copyright(C),CEC Huada Electronic Design Co.,Ltd.
*/

/***************************************************************************
* Include Header Files
***************************************************************************/

#include <stdio.h>
#include "util.h"
#include "tpdu.h"
#include "comm.h"

#define COMMAND_LIST_NUM 35
#define VAR 0xFF

/** @addtogroup SE_Service
  * @{
  */

/** @addtogroup CMD
  * @brief Command layer.
  * @{
  */

/** @defgroup TPDU TPDU
  * @brief tpdu command pack , unpack, execute.
  * @{
  */



/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/** @defgroup TPDU_Exported_Functions TPDU Exported Functions
  * @{
  */



/**
* @brief 设置命令头子函数
* @param [in] isoCase	APDU协议类型
* @param [in] cla CLA
* @param [in] ins INS
* @param [in] p1 P1
* @param [in] p2 P2
* @param [in] le LE
* @param [out] command  命令结构体
* @return 参见error.h
* @note no
*/
iso_command_apdu_t* tpdu_init(iso_command_apdu_t *command, int32_t isoCase, int32_t cla, int32_t ins,int32_t p1, int32_t p2,int32_t lc, int32_t le)
{
	#ifdef _ASSERT_DEBUG
	ASSERT(command!=NULL); 
	#endif
	
	//赋值APDU各参数
	if ( isoCase >= 0 )
		command->isoCase = isoCase;
	if ( cla >= 0 )
		command->classbyte = cla;
	if ( ins >= 0 )
		command->instruction = ins;
	if ( p1  >= 0 )
		command->p1 = p1;
	if ( p2  >= 0 )
		command->p2 = p2;
	if ( lc  >= 0 )
		command->lc = lc;
	if ( le  >  0 )
		command->le = le;

	return command;
}


/**
* @brief 根据命令头初始表设计命令头
* @param [in] commandID  命令ID号
* @param [out] command  命令结构体
* @return 参见error.h
* @note no
* @see 函数  tpdu_init 
*/
iso_command_apdu_t* tpdu_init_with_id(iso_command_apdu_t *command, uint8_t commandID)
{
	typedef struct 
	{
		uint8_t  isoCase;
		uint8_t  classbyte;
		uint8_t  instruction;
		uint8_t  p1;
		uint8_t  p2;
	}command_info_t;
	command_info_t *pc = NULL;
	//命令初始化表，方便后续更新命令
	command_info_t command_list[COMMAND_LIST_NUM]=
	{
		{ISO_CASE_3S, 0x80, 0xE2,0x00,VAR},		//CMD_ENTER_LOWPOWER
		{ISO_CASE_3S, 0x00, 0x82, 0x00, 0x00},	//CMD_EXTTERN_AUTH
		{ISO_CASE_3S, VAR, 0xD4, VAR,VAR},          	//CMD_WRITE_KEY
        {ISO_CASE_3S, 0x80, 0x46, 0x00, 0x00},    //CMD_GENERATE_KEY
		{ISO_CASE_3S, 0x80, 0x44,VAR,VAR},	  //CMD_GEN_SHARED_KEY
		{ISO_CASE_1, 0x80, 0x48,0x00,VAR},	       	//CMD_DEL_KEY
		{ISO_CASE_3S, 0x80, 0x3A, VAR, VAR},	//CMD_EXPORT_KEY
		{VAR, 0x80, 0x3C,VAR,VAR},		        //CMD_IMPORT_KEY
		{ISO_CASE_2S, 0x80, 0x42, VAR, 0x00},	//CMD_GET_KEY_INFO		
		{ISO_CASE_3S, 0x80, 0x5E, VAR, VAR},	//CMD_CHANGE_RELOAD_PIN 
		{ISO_CASE_3S, 0x00, 0x20, 0x00, VAR},	//CMD_VERIFY_PIN
		{VAR, 0x80, 0x3E, VAR, VAR},	        //CMD_CIPHER_DATA 
		{VAR, 0x80, 0x30, VAR, VAR},		//CMD_PKI_ENCIPHER
		{VAR, 0x80, 0x32, VAR, VAR},		//CMD_PKI_DECIPHER
		{VAR, 0x80, 0x36, VAR, VAR},		//CMD_COMPUTE_SIGNATURE  
		{VAR, 0x80, 0x38, VAR, VAR},		//CMD_VERIFY_SIGNATURE 
		{ISO_CASE_3S, 0x80, 0x4E, 0x00, 0x00},	//CMD_SM2_GET_ZA
		{VAR, 0x80, 0x34, VAR, VAR},           	//CMD_DIGEST
		{ISO_CASE_2S, 0x00, 0x84, 0x00,0x00},	//CMD_GET_RANDOM
		{ISO_CASE_3S, 0x00, 0xA4, VAR,0x00},	//CMD_SELECT_FILE
		{VAR, VAR, 0xD6, VAR, VAR},      //CMD_WRITE_FILE
		{VAR, VAR, 0xB0, VAR, VAR},		//CMD_READ_FILE
		{ISO_CASE_3S, 0x80, 0xD2, 0x00,0x00},	//CMD_WRITE_SEID
		{ISO_CASE_2S, 0x80,0xC8, VAR, 0x00}, 	//CMD_GET_INFO
		{ISO_CASE_2S, 0x80, 0x40,0x00,0x00}, 	//CMD_GET_ID
		{ISO_CASE_2E, 0x00, 0xC0,0x00,0x00}, 	//CMD_GET_RESPONSE	
    	{ISO_CASE_2E, 0x80, 0x4A,VAR,0x00},	//CMD_GET_FILE_INFO
    	{ISO_CASE_3S, 0xBF, 0x41, 0x00, 0x00},   //CMD_LOADER_INIT
		{ISO_CASE_3E, 0x00, 0x00, 0x00, 0x00},   //CMD_LOADER_DOWNLOAD
        {ISO_CASE_3S, 0xBF, 0x42, 0x00, 0x00},   //CMD_LOADER_CHECKDATA	
      	{ISO_CASE_2S, 0x80, 0x4C, 0x00, 0x00},   //CMD_V2X_GENERATE_KEY_DERIVE_SEED
      	{ISO_CASE_4E, 0x80, 0x50, 0x00, VAR},   //CMD_V2X_RECONSITUTION_KEY
	};
	#ifdef _ASSERT_DEBUG
    ASSERT(command!=NULL);
		ASSERT(commandID<sizeof(command_list));
	#endif
	pc = &command_list[commandID];
	//根据command ID指令赋值各个参数
    tpdu_init(command,
        pc->isoCase,
        pc->classbyte,
        pc->instruction,
        pc->p1,
        pc->p2,
        -1,
        -1
    );

    return command;
}

/**
* @brief 修改CLA
* @param [in] cla  CLA
* @param [out] command  命令结构体
* @return 参见error.h
* @note no
* @see 函数  tpdu_init 
*/
iso_command_apdu_t* tpdu_set_cla  (iso_command_apdu_t *command, uint32_t cla)
{
	#ifdef _ASSERT_DEBUG
	ASSERT(command!=NULL);
	#endif
	//更新CLA参数
	tpdu_init(command, -1, cla, -1, -1, -1,  -1, -1);
    return command;

}

/**
* @brief 修改P1P2  
* @param [in] p1  P1
* @param [in] p2  P2
* @param [out] command  命令结构体
* @return 参见error.h
* @note no
* @see 函数  tpdu_init 
*/
iso_command_apdu_t* tpdu_set_p1p2 (iso_command_apdu_t *command, uint8_t p1, uint8_t p2)
{
	#ifdef _ASSERT_DEBUG
	ASSERT(command!=NULL);
	#endif
	//更新P1P2
	tpdu_init(command, -1, -1, -1, p1, p2, -1, -1);
    return command;
}

/**
* @brief 修改Le
* @param [in] le  LE
* @param [out] command  命令结构体
* @return 参见error.h
* @note no
* @see 函数  tpdu_init 
*/
iso_command_apdu_t* tpdu_set_le(iso_command_apdu_t *command, uint32_t le)
{
	#ifdef _ASSERT_DEBUG   
	ASSERT(command!=NULL);
	#endif
	//更新Le
	tpdu_init(command, -1, -1, -1, -1, -1, -1, le);
    return command;
}


/**
* @brief 子函数，将APDU命令结构体设置到双端队列中
* @param [in] command  命令结构体
* @param [out] output  输出数据
* @param [out] output_len  输出数据长度
* @return 参见error.h
* @note no
* @see 函数 util_queue_front_push  util_queue_size
*/
se_error_t tpdu_pack(iso_command_apdu_t *command, uint8_t *output, uint32_t *output_len)
{
	//output 为双端队列
	uint8_t front_buffer[7] = {0};
	uint8_t rear_buffer[2] = {0};
	double_queue queue_out = (double_queue)output;
	
	//拷贝到缓存区，方便放入双向队列
	front_buffer[0] = command->classbyte;
	front_buffer[1] = command->instruction;
	front_buffer[2] = command->p1;
	front_buffer[3] = command->p2;
	//根据APDU协议类型拼装8种数据
	//ISO_CASE_1 = 0x01,  // CLA INS P1 P2 00
	//ISO_CASE_2S = 0x02,  // CLA INS P1 P2 Le
	//ISO_CASE_3S = 0x03,  // CLA INS P1 P2 Lc Data
	//ISO_CASE_4S = 0x04   // CLA INS P1 P2 Lc Data Le（Le 3字节）
	//ISO_CASE_2E = 0x12,  // CLA INS P1 P2 Le
	//ISO_CASE_3E = 0x13,  // CLA INS P1 P2 Lc（Lc 3字节） Data
	//ISO_CASE_4E = 0x14   // CLA INS P1 P2 Lc（Lc 2字节 ）Data Le（Le 2字节）

	switch ( command->isoCase ) {
		case ISO_CASE_1:
			front_buffer[4] = 0x00;
			util_queue_front_push(front_buffer,5, queue_out);
			*output_len = 5;
			break;

		case ISO_CASE_2S:
			front_buffer[4] = (command->le)&0xFF;
			*output_len = 5;
			util_queue_front_push(front_buffer,5, queue_out);
			break;

		case ISO_CASE_3S:
			front_buffer[4] = util_queue_size(queue_out)&0xFF;
			util_queue_front_push(front_buffer,5, queue_out);
			*output_len = util_queue_size(queue_out);
			break;

		case ISO_CASE_4S:
			front_buffer[4] = util_queue_size(queue_out)&0xFF;
			*output_len = 5+util_queue_size(queue_out)+1;
			util_queue_front_push(front_buffer,5, queue_out);
			rear_buffer[1]=(command->le)&0xFF;
			util_queue_rear_push(rear_buffer,1, queue_out);
			break;
	
		case ISO_CASE_2E:
			front_buffer[4] = 0x00;
			front_buffer[5] = (command->le>>8)&0xFF;
			front_buffer[6] = command->le&0xFF;
			*output_len = 7;
			util_queue_front_push(front_buffer,7, queue_out);
			break;

		
		case ISO_CASE_3E:
			front_buffer[4] = 0x00;
			front_buffer[5] = (util_queue_size(queue_out)>>8)&0xFF;
			front_buffer[6] = util_queue_size(queue_out)&0xFF;
			util_queue_front_push(front_buffer,7, queue_out);
			*output_len = util_queue_size(queue_out);
			break;

		case ISO_CASE_4E:
			front_buffer[4] = 0x00;
			front_buffer[5] = (util_queue_size(queue_out)>>8)&0xFF;
			front_buffer[6] = util_queue_size(queue_out)&0xFF;
			*output_len = 6+util_queue_size(queue_out);
			util_queue_front_push(front_buffer,7, queue_out);
			rear_buffer[0]=(command->le>>8)&0xFF;
			rear_buffer[1]=(command->le)&0xFF;
			util_queue_rear_push(rear_buffer,2, queue_out);
			break;

			
	}
	output = (uint8_t*)queue_out;

	return SE_SUCCESS;

}

/**
* @brief 子函数，拆分状态字和数据
* @param [out] output  输出双端队列
* @param [out] output_len  输出双端队列数据长度
* @return 参见error.h
* @note no
* @see 函数  util_queue_rear_pop
*/
se_error_t tpdu_unpack(uint8_t *output, uint32_t *output_len)
{
	double_queue queue_out=(double_queue)output;
	//参数判断
	if(output==NULL||output_len==NULL)
	{
		LOGE("failed to tpdu_unpack output params!");
		return SE_ERR_PARAM_INVALID;
	}
	//去除状态字
	util_queue_rear_pop(2 ,queue_out);
	output = (uint8_t *)queue_out;
	*output_len = queue_out->q_buf_len;

	return SE_SUCCESS;
}


/**
* @brief apdu命令发送接收
* @param [in] in_buf  输入数据
* @param [in] in_buf_len   输入数据长度 
* @param [out] output  输出数据
* @param [out] output_len  输出数据长度
* @return 参见error.h
* @note no
* @see 函数  api_transceive_queue   util_queue_rear_push    util_queue_init
*/
se_error_t tpdu_send(uint8_t *input, uint32_t input_len, uint8_t *output, uint32_t *output_len)
{

	double_queue queue_in=(double_queue)input;
	double_queue queue_out=(double_queue)output;
	//uint8_t temp_buffer[5]={0x00};
	se_error_t ret = 0;
	uint32_t temp_len = 0;
		
	
	ret=api_transceive_queue((uint8_t*)queue_in,queue_in->q_buf_len, (uint8_t*)queue_out,&temp_len);
	
	output = (uint8_t *)queue_out;
	*output_len = temp_len;
	if(ret!=SE_SUCCESS)
		return ret;


	return SE_SUCCESS;
}

/**
* @brief apdu命令处理及发送接收
* @param [in] command  命令结构体
* @param [in] in_buf  输入数据
* @param [in] in_buf_len   输入数据长度 
* @param [out] output  输出数据
* @param [out] output_len  输出数据长度
* @return 参见error.h
* @note no
* @see 函数  tpdu_pack  tpdu_send  tpdu_unpack  tpdu_change_error_code
*/
se_error_t tpdu_execute	(iso_command_apdu_t *command, uint8_t *input, uint32_t input_len, uint8_t *output, uint32_t *output_len)
{

	se_error_t ret = 0;
	uint16_t status_word = 0;
	double_queue queue_in=(double_queue)input;
	double_queue queue_out=(double_queue)output;
	uint32_t in_len = 0;
	uint32_t out_len = 0;
	uint32_t off = 0;
	uint32_t change_status = 0;

	//参数判断
	if(command==NULL||input==NULL)
	{
		LOGE("failed to tpdu_execute input params!");
		return SE_ERR_PARAM_INVALID;		
	}	
	if(output==NULL||output_len==NULL)
	{
		LOGE("failed to tpdu_execute output params!");
		return SE_ERR_PARAM_INVALID;		
	}			
	//将APDU命令放入双向队列
	ret = tpdu_pack(command,(uint8_t *) queue_in,&in_len);
	if(ret!=SE_SUCCESS)
		return ret;
	//发送接收APDU命令
	ret = tpdu_send((uint8_t *) queue_in,queue_in->q_buf_len,(uint8_t *)queue_out,&out_len);
	//先判断返回是否正确
	if(ret!=SE_SUCCESS)
	{	
		status_word=((queue_out->q_buf[off-2]<< 8) & 0xFF00) | (queue_out->q_buf[off-1] & 0xFF);
		change_status = tpdu_change_error_code(status_word);
		output = (uint8_t *)queue_out;
	  *output_len = out_len;
		return ret;	
	}
	//将状态字取出
	off = queue_out->rear_node;
	status_word=((queue_out->q_buf[off-2]<< 8) & 0xFF00) | (queue_out->q_buf[off-1] & 0xFF);
	//6100需要处理，先将数据返回
	if((status_word&0xFF00)==0x6100)
	{
		change_status = tpdu_change_error_code(status_word);
		ret = tpdu_unpack((uint8_t *)queue_out,&out_len);
	    output = (uint8_t *)queue_out;
	    *output_len = out_len;
		return change_status;
	}
	//其他返回，直接返回错误码，正确继续处理去状态字
	if(status_word!=0x9000)
		return tpdu_change_error_code(status_word);
	
	//通信且返回都正常，将状态字从数据中去除，并返回有效数据
	ret = tpdu_unpack((uint8_t *)queue_out,&out_len);
	if(ret!=SE_SUCCESS)
		return ret;
	output = (uint8_t *)queue_out;
	*output_len = out_len;

	return SE_SUCCESS;

}

/**
* @brief apdu命令处理及发送接收，无返回数据
* @param [in] command  命令结构体
* @param [in] in_buf  输入数据
* @param [in] in_buf_len   输入数据长度 
* @return 参见error.h
* @note no
* @see 函数  util_queue_init  tpdu_pack  tpdu_pack  tpdu_change_error_code
*/
se_error_t tpdu_execute_no_response	(iso_command_apdu_t *command, uint8_t *input, uint32_t input_len)
{

	se_error_t ret = 0;
	uint16_t status_word = 0;
	double_queue queue_in=(double_queue)input;
	double_queue_node queue_out={0};
	uint32_t in_len = 0;
	uint32_t out_len = 0;
	uint32_t off = 0;

	//参数判断
	if(command==NULL||input==NULL)
	{
		LOGE("failed to tpdu_execute_no_response input params!");
		return SE_ERR_PARAM_INVALID;		
	}		
	//将APDU命令放入双向队列
	util_queue_init(&queue_out);
	ret = tpdu_pack(command,(uint8_t *) queue_in,&in_len);
	if(ret!=SE_SUCCESS)
		return ret;
	//发送APDU命令，无返回
	ret = tpdu_send((uint8_t *) queue_in,queue_in->q_buf_len,(uint8_t *)&queue_out,&out_len);
	off = queue_out.front_node;
	status_word=((queue_out.q_buf[off]<< 8) & 0xFF00) | (queue_out.q_buf[off+1] & 0xFF);
	//判断返回是否正确
	if(ret!=SE_SUCCESS)
		return ret;
	//判断状态字是否正确
	if(status_word!=0x9000)
		return tpdu_change_error_code(status_word);

	return SE_SUCCESS;

}



/**
* @brief 状态码转换
* @param [in] status_word  源状态码
* @return 参见error.h
* @note no
*/
se_error_t tpdu_change_error_code(uint16_t status_word)
{
	se_error_t error = (se_error_t)(status_word&(0x0000FFFF));
	//状态码装换，参见error.h
	if(status_word==0x9000)
		return SE_SUCCESS;
	else
		return (error|(0x10000000));

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



