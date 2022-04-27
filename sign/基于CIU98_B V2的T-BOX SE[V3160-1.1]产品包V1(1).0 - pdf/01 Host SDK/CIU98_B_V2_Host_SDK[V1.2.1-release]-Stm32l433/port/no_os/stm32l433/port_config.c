/**@file  port_config.c
* @brief  提供系统初始化、控制IO初始化、串口初始化等底层驱动函数
* @author  zhengwd
* @date  2021-4-28
* @version	V1.0
* @copyright  Copyright(C),CEC Huada Electronic Design Co.,Ltd.
*/


/***************************************************************************
* Include Header Files
***************************************************************************/
#include "port_config.h"


#ifdef _DEBUG


#if PORT_UART_PRINTF_ENABLE

UART_HandleTypeDef hal_uart_handle = {0};


/**
* @brief uart初始化，使用demo板的串口打印输出log信息，波特率115200bps
* @param no
* @return no	
* @note no
*/
void port_printf_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct = {0};

	PORT_USART1_RX_GPIO_CLK_ENABLE();
	PORT_USART1_TX_GPIO_CLK_ENABLE();

	PORT_USART1_CLK_ENABLE();

	/*##-2- Configure peripheral GPIO ##########################################*/
	/* LPUART TX GPIO pin configuration  */
	GPIO_InitStruct.Pin 	  = PORT_USART1_TX_PIN;
	GPIO_InitStruct.Mode  = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull	  = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Alternate = PORT_USART1_TX_AF;

	HAL_GPIO_Init(PORT_USART1_TX_GPIO_PORT, &GPIO_InitStruct);

	/* LPUART RX GPIO pin configuration  */
	GPIO_InitStruct.Pin 	  = PORT_USART1_RX_PIN;
	GPIO_InitStruct.Alternate = PORT_USART1_RX_AF;

	HAL_GPIO_Init(PORT_USART1_RX_GPIO_PORT, &GPIO_InitStruct);

	/* LPUART configured as follow:
	- Word Length = 8 Bits
	- Stop Bit = One Stop bit
	- Parity = None
	- BaudRate = 115200 baud
	- Hardware flow control disabled (RTS and CTS signals)
	- Oversampling at 16
	- One bit sampling feature disabled */
	hal_uart_handle.Instance            = LPUART1;
	hal_uart_handle.Init.BaudRate       = 115200;
	hal_uart_handle.Init.WordLength     = UART_WORDLENGTH_8B;
	hal_uart_handle.Init.StopBits       = UART_STOPBITS_1;
	hal_uart_handle.Init.Parity         = UART_PARITY_NONE;
	hal_uart_handle.Init.HwFlowCtl      = UART_HWCONTROL_NONE;
	hal_uart_handle.Init.Mode           = UART_MODE_TX_RX;
	hal_uart_handle.Init.OverSampling   = UART_OVERSAMPLING_16;
	hal_uart_handle.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;

	if(HAL_UART_Init(&hal_uart_handle) != HAL_OK)
	{

	}
}


int fputc(int ch, FILE *f) 
{
	uint8_t tmp_ch = 0;
	
	tmp_ch = ch;
	HAL_UART_Transmit(&hal_uart_handle,&tmp_ch,1,100);
	return(ch);
}


#else

#define ITM_Port8(n)    (*((volatile uint8_t *)(0xE0000000+4*n)))
#define ITM_Port16(n)   (*((volatile uint16_t *)(0xE0000000+4*n)))
#define ITM_Port32(n)   (*((volatile uint32_t *)(0xE0000000+4*n)))
#define DEMCR           (*((volatile uint32_t *)(0xE000EDFC)))
#define TRCENA          0x01000000

struct __FILE { int handle; /* Add whatever you need here */ };
extern FILE __stdout;
extern FILE __stdin;

    
int fputc(int ch, FILE *f) 
{
	if (DEMCR & TRCENA) 
	{
		while (ITM_Port32(0) == 0);
		ITM_Port8(0) = ch;
	}
	return(ch);
}
#endif   //#if PORT_UART_PRINTF_ENABLE

#endif  //#ifdef _DEBUG




/**
* @brief System Clock Configuration
*			HSI48 used as USB clock source (USE_USB_CLKSOURCE_CRSHSI48 defined in main.h)
*			  - System Clock source 		   = HSI
*			  - HSI Frequency(Hz)			   = 48000000
*			  - SYSCLK(Hz)					   = 48000000
*			  - HCLK(Hz)					   = 48000000
*			  - AHB Prescaler				   = 1
*			  - APB1 Prescaler				   = 1
*			  - APB2 Prescaler				   = 2
*			  - Flash Latency(WS)			   = 1
*			  - Main regulator output voltage  = Scale1 mode
*
*			PLL(HSE) used as USB clock source (USE_USB_CLKSOURCE_PLL defined in main.h)
*			  - System Clock source 		   = PLL (HSE)
*			  - HSE Frequency(Hz)			   = 8000000
*			  - SYSCLK(Hz)					   = 48000000
*			  - HCLK(Hz)					   = 48000000
*			  - AHB Prescaler				   = 1
*			  - APB1 Prescaler				   = 1
*			  - APB2 Prescaler				   = 1
*			  - PLL_M						   = 1
*			  - PLL_N						   = 24
*			  - PLL_P						   = 7
*			  - PLL_Q						   = 4
*			  - PLL_R						   = 4
*			  - Flash Latency(WS)			   = 4
*
* @return no	
* @note no
*/
void port_system_clock_config(void)
{
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct = {0};

#if defined (USE_USB_CLKSOURCE_CRSHSI48)
	static RCC_CRSInitTypeDef RCC_CRSInitStruct = {0};
#endif

#if defined (USE_USB_CLKSOURCE_CRSHSI48)

	/* Enable HSI48 Oscillator to be used as system clock source */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_HSI48; 
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;  
	RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_OFF;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;

	HAL_RCC_OscConfig(&RCC_OscInitStruct); 

	/* Select HSI48 as USB clock source */
	PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USB;
	PeriphClkInitStruct.UsbClockSelection = RCC_USBCLKSOURCE_HSI48;
	HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);


	
	/* HSE is enabled after System reset, activate PLL with HSE as source */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	
	/* MSI is enabled after System reset, activate PLL with MSI as source */
	//RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
	//RCC_OscInitStruct.MSIState = RCC_MSI_ON;
	//RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
	//RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
	//RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	//RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
	
	RCC_OscInitStruct.PLL.PLLM = 1;
	RCC_OscInitStruct.PLL.PLLN = 20;//40;
	RCC_OscInitStruct.PLL.PLLR = 2;
	RCC_OscInitStruct.PLL.PLLP = 7;
	RCC_OscInitStruct.PLL.PLLQ = 4;
	if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		/* Initialization Error */
		while(1);
	}

	/* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
	 clocks dividers */
	RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;  
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;  
	if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
	{
		/* Initialization Error */
		while(1);
	}


	/*Configure the clock recovery system (CRS)**********************************/

	/* Enable CRS Clock */
	__HAL_RCC_CRS_CLK_ENABLE();

	/* Default Synchro Signal division factor (not divided) */
	RCC_CRSInitStruct.Prescaler = RCC_CRS_SYNC_DIV1;

	/* Set the SYNCSRC[1:0] bits according to CRS_Source value */
	RCC_CRSInitStruct.Source = RCC_CRS_SYNC_SOURCE_USB;

	/* HSI48 is synchronized with USB SOF at 1KHz rate */
	RCC_CRSInitStruct.ReloadValue =  RCC_CRS_RELOADVALUE_DEFAULT;
	RCC_CRSInitStruct.ErrorLimitValue = RCC_CRS_ERRORLIMIT_DEFAULT;

	RCC_CRSInitStruct.Polarity = RCC_CRS_SYNC_POLARITY_RISING;

	/* Set the TRIM[5:0] to the default value*/
	RCC_CRSInitStruct.HSI48CalibrationValue = RCC_CRS_HSI48CALIBRATION_DEFAULT; 

	/* Start automatic synchronization */ 
	HAL_RCCEx_CRSConfig (&RCC_CRSInitStruct);

#elif defined (USE_USB_CLKSOURCE_PLL) 

	/* Enable HSE Oscillator and activate PLL with HSE as source
	PLLCLK = (8 * 24) / 4) = 48 MHz */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 1;
	RCC_OscInitStruct.PLL.PLLN = 24;
	RCC_OscInitStruct.PLL.PLLR = 2;
	RCC_OscInitStruct.PLL.PLLP = 7;
	RCC_OscInitStruct.PLL.PLLQ = 4;

	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		port_error_handler();
	}

	/*Select PLL 48 MHz output as USB clock source */
	PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USB;
	PeriphClkInitStruct.UsbClockSelection = RCC_USBCLKSOURCE_PLLCLK;
	HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);

	/* Select PLL as system clock source and configure the HCLK and PCLK1 
	clock dividers */
	RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
	{
		port_error_handler();
	}

#endif /*USE_USB_CLKSOURCE_CRSHSI48*/

	/* Enable Power Controller clock */
	__HAL_RCC_PWR_CLK_ENABLE();

	SystemCoreClockUpdate();

	// g_TIM_clk_time = SystemCoreClock/(1000000*1);   //当前apb1为1分频，记录time一个时钟周期的时间的倒数
}


/** @addtogroup SE_Driver
  * @{
  */

/** @addtogroup PORT 
  * @brief hardware  portable layer .
  * @{
  */


/** @defgroup PORT_CONFIG PORT_CONFIG
  * @brief hardware portable layer common interface driver.
  * @{
  */


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function  -----------------------------------------------*/


/* Exported functions --------------------------------------------------------*/

/** @defgroup Port_Config_Exported_Functions Port_Config Exported Functions
  * @{
  */


/**
* @brief LED、调试 IO 等功能的GPIO 定义
* @param no
* @return no
* @note no
*/
void port_gpio_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct = {0};
	
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

	/* Configure  SPI LED Ctrl   */
	PORT_SPI_LED_CTRL_IO_CLK_ENABLE();
	GPIO_InitStruct.Pin = PORT_SPI_LED_CTRL_IO_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD; 
	HAL_GPIO_Init(PORT_SPI_LED_CTRL_PORT, &GPIO_InitStruct);
	PORT_SPI_LED_CTRL_OFF();   // LED等 默认灭

	/* Configure  I2C LED Ctrl   */
	PORT_I2C_LED_CTRL_IO_CLK_ENABLE();
	GPIO_InitStruct.Pin = PORT_I2C_LED_CTRL_IO_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD; 
	HAL_GPIO_Init(PORT_I2C_LED_CTRL_PORT, &GPIO_InitStruct);
	PORT_I2C_LED_CTRL_OFF();   

	/* Configure POWER Ctrl  IO*/
	PORT_POWER_CTRL_IO_CLK_ENABLE();
	GPIO_InitStruct.Pin = PORT_POWER_CTRL_IO_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD; 
	HAL_GPIO_Init(PORT_POWER_CTRL_PORT, &GPIO_InitStruct);
	PORT_POWER_CTRL_ON();                    //输入低， 电源导通

	/* Configure  debug  Ctrl IO*/	
	PORT_DEBUG_IO_CLK_ENABLE();
	GPIO_InitStruct.Pin = PORT_DEBUG_IO_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; 
	HAL_GPIO_Init(PORT_DEBUG_IO_PORT, &GPIO_InitStruct);
	PORT_DEBUG_IO_LOW() ;    //默认输出低电平
}


/**
* @brief mcu上电初始化
* @param no
* @return no
* @note no
*/
void port_mcu_init(void)
{
	HAL_Init();

	/* Configure the system clock to get correspondent USB clock source */
	port_system_clock_config();

	/* Enable Power Clock*/
	__HAL_RCC_PWR_CLK_ENABLE();

	port_gpio_init();

#if PORT_UART_PRINTF_ENABLE
	port_printf_init();
#endif
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


