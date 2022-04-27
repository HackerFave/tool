/**@file  update.c
* @brief  update interface definition
* @author  ade
* @date  2020-06-02
* @version	V1.0
* @copyright  Copyright(C),CEC Huada Electronic Design Co.,Ltd.
*/

/***************************************************************************
* Include Header Files
***************************************************************************/
#include "comm.h"
#include "update.h"
#include "info.h"
#include "info.h"


/** @addtogroup SE_Service
  * @{
  */

/** @addtogroup API 
  * @brief API layer.
  * @{
  */

/** @defgroup UPDATE UPDATE
  * @brief update interface api.
  * @{
  */



/* Exported functions --------------------------------------------------------*/

/** @defgroup UPDATE_Exported_Functions UPDATE Exported Functions
  * @{
  */



/**
* @brief 程序升级
* @param [in] image_addr 升级数据地址
* @param [in] image_len 升级数据长度
* @return 参见error.h
* @note no
* @see 函数  apdu_get_info  apdu_enter_loader  api_reset  apdu_loader_init  apdu_loader_init  apdu_loader_checkdata
*/
se_error_t api_loader_download(uint8_t *image_addr, uint32_t image_len)
{
	se_error_t ret = SE_SUCCESS;
	uint8_t atr[32] = {0};
	uint32_t atr_len = 0;
	uint32_t download_len = 0;
	uint32_t addr_off = 0;
	uint8_t cos_info_version[4] = {0};
	//info_type type = PRODUCT_INFO;//PRODUCT_INFO  CHIP_ID
	//se_info_t info = {0};
  //  uint8_t in_len = 5;
	se_info_t info;
	//参数检查
	if(image_addr==NULL||image_len==0)
	{  
		LOGE("failed to api_loader_download input params!\n");
		return SE_ERR_PARAM_INVALID;
	}

	
	//从image中，获取COS信息和初始化规范版本号
	memcpy(cos_info_version, image_addr+image_len-4, 4);  //*********保存COS信息和初始化规范版本号**********************************
   do
   {

	//检查是否需要升级
	//1、获取ATR，判断其产品状态bit6 cos有效标识位，如果为1，则执行升级过程
		ret = api_reset(atr, &atr_len);
		if(ret != SE_SUCCESS)
		{
			LOGE("failed to api_reset!\n");
			return ret;
		}
		//检查atr其状态信息bit6 cos有效标识位
		if(atr[9]&0x20)
		{
			break;
		}
		
	//2、判断ATR其产品状态bit5 补丁有效标识位，如果为0，则执行升级过程
		//检查atr其状态信息bit5 补丁有效标识位
		if(!atr[9]&0x10)
		{
			break;
		}
			
	//3、如果ATR产品状态bit6为0、bit5为1，则执行api_get_info获取其OS信息和初始化规范版本号
	    ret =  api_get_info (PRODUCT_INFO, &info);
		if(ret != SE_SUCCESS)
		{
			LOGE("failed to api_transceive!\n");
			return ret;
		}
	//4、如果不等于image中的COS信息和初始化规范版本，则执行升级过程
	    if(memcmp(cos_info_version, info.product_info, 4)!=0 )
	    {
	    	break;
	    }
		else
		{
		    LOGE("need not update!\n");
			return ret;
		}
		

	}while(1);
		
	//检查数据有效性
	download_len = (uint16_t)(image_addr[addr_off]<<8 | image_addr[addr_off+1]) + 2;
	if(download_len > image_len)
	{
		LOGE("failed to api_loader_download image length!\n");
		return SE_ERR_PARAM_INVALID;
	}
	//下载初始化
	ret = apdu_loader_init(image_addr);
	if(ret != SE_SUCCESS)
	{
		port_printf("failed to apdu_loader_init!\n");
		return ret;
	}
	addr_off = download_len;

	do
	{
		//检查数据有效性
		download_len = download_len + (uint16_t)(image_addr[addr_off]<<8 | image_addr[addr_off+1]) + 2;
		if(download_len > image_len)
		{
			LOGE("failed to api_loader_download image length!\n");
			return SE_ERR_PARAM_INVALID;
		}

		//长度为0x0025，当前从image 获取的命令为数据校验命令
		if((uint16_t)(image_addr[addr_off]<<8 | image_addr[addr_off+1])==0x0025 )
		{
			//发送数据校验命令
			ret = apdu_loader_checkprogram(image_addr+addr_off);
			if(ret != SE_SUCCESS)
			{
				port_printf("failed to apdu_loader_download!\n");
				return ret;
			}

			//从image中，获取COS版本信息
			addr_off = download_len;
			download_len = download_len + (uint16_t)(image_addr[addr_off]<<8 | image_addr[addr_off+1]) + 2;
			if(download_len > image_len)
			{
				LOGE("failed to api_loader_download image length!\n");
				return SE_ERR_PARAM_INVALID;
			}

			memcpy(cos_info_version, image_addr+addr_off+2, 4);  //保存image文件中COS信息和初始化规范版本号
			
			break;  //下载结束，退出
		}
		
		//下载image数据
		ret = apdu_loader_program(image_addr+addr_off);
		if(ret != SE_SUCCESS)
		{
			port_printf("failed to apdu_loader_download!\n");
			return ret;
		}
		addr_off = download_len;

	}while(1);
    //至少延迟1秒确保升级成功
    ret =  api_delay (1000000);//延迟1秒
	if(ret!=SE_SUCCESS)
	 {		 
		  LOGE("failed to api_connect\n");
		  return ret;
	 } 	
	//检查是否升级成功
	//1、获取ATR，判断其产品状态bit6 cos有效标识位，如果为1，则返回错误码
		ret = api_reset(atr, &atr_len);
		if(ret != SE_SUCCESS)
		{
			LOGE("failed to api_reset!\n");
			return ret;
		}
		//检查atr其状态信息bit6 cos有效标识位
		if(atr[9]&0x20)
		{
			LOGE("failed to cos valid!\n");
			return SE_ERR_UPDATE;
		}
		
	//2、判断ATR其产品状态bit5 补丁有效标识位，如果为0，则返回错误码
		//检查atr其状态信息bit5 补丁有效标识位
		if(!atr[9]&0x10)
		{
			LOGE("failed to patch valid!\n");
			return SE_ERR_UPDATE;
		}
			
	//3、如果ATR产品状态bit6为0、bit5为1，则执行api_get_info获取其COS信息和初始化规范版本号
	   ret =  api_get_info (PRODUCT_INFO, &info);
		if(ret != SE_SUCCESS)
		{
			LOGE("failed to api_transceive!\n");
			return ret;
		}
		
	//4、如果不等于image中的COS版本号和初始化规范版本号，则返回错误码
	   if(memcmp(cos_info_version, info.product_info, 4)!=0 )
	    {
	    	LOGE("failed to patch version!\n");
			return SE_ERR_UPDATE;
	    }
		

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

/**
  * @}
  */


