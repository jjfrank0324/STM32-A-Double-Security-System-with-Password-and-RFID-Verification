/*
RPI MPS Final Project

Member: Jiahong Ji, Lizheng Liu

Project name: RF double security door Lock
 */
#include "rfid.h"
#include "rfid_functions.h"
#include "init.h"

SPI_HandleTypeDef SpiHandle;

void SystemClock_Config(void);
static void Error_Handler(void);
void SPI_Init(void);
void LCD_LOG_Setup(void);




void SPI_Init() {
	/*##-1- Configure the SPI peripheral #######################################*/
    /* Set the SPI parameters */
	SPIx_FORCE_RESET();
	SPIx_RELEASE_RESET();
	SPIx_CLK_ENABLE();

  SpiHandle.Instance               = SPIx;
  SpiHandle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
  SpiHandle.Init.Direction         = SPI_DIRECTION_2LINES;
  SpiHandle.Init.CLKPhase          = SPI_PHASE_1EDGE; // data stable on first clock edge, if clock is idle low, that is low->high transition
  SpiHandle.Init.CLKPolarity       = SPI_POLARITY_LOW; // clock idle state low
  SpiHandle.Init.DataSize          = SPI_DATASIZE_8BIT;
  SpiHandle.Init.FirstBit          = SPI_FIRSTBIT_MSB;
  SpiHandle.Init.TIMode            = SPI_TIMODE_DISABLE;
  SpiHandle.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
  //SpiHandle.Init.CRCPolynomial     = 7;
  SpiHandle.Init.NSS               = SPI_NSS_SOFT;
  SpiHandle.Init.Mode 			       = SPI_MODE_MASTER;
  /* Slave board must wait until Master Board is ready. This to guarantee the
     correctness of transmitted/received data */
  //HAL_Delay(5);

  if(HAL_SPI_Init(&SpiHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
}

void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi) {
  GPIO_InitTypeDef  GPIO_InitStruct;
  GPIO_InitTypeDef  GPIO_InitStruct1;

  //SPI_InitTypeDef SPI_InitStruct;

    /*##-1- Enable peripherals and GPIO Clocks #################################*/
    /* Enable GPIO TX/RX clock */
  SPIx_SCK_GPIO_CLK_ENABLE();
  SPIx_MISO_GPIO_CLK_ENABLE();
  SPIx_MOSI_GPIO_CLK_ENABLE();
  SPIx_CS_GPIO_CLK_ENABLE();

  /*##-2- Configure peripheral GPIO ##########################################*/
  /* SPI SCK GPIO pin configuration  */
  GPIO_InitStruct.Pin       = SPIx_SCK_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed     = GPIO_SPEED_HIGH;
  GPIO_InitStruct.Alternate = SPIx_SCK_AF;
  HAL_GPIO_Init(SPIx_SCK_GPIO_PORT, &GPIO_InitStruct);

  /* SPI MISO GPIO pin configuration; MISO line should be floating */
  GPIO_InitStruct.Pull      = GPIO_NOPULL;
  GPIO_InitStruct.Pin = SPIx_MISO_PIN;
  GPIO_InitStruct.Alternate = SPIx_MISO_AF;
  HAL_GPIO_Init(SPIx_MISO_GPIO_PORT, &GPIO_InitStruct);

  /* SPI MOSI GPIO pin configuration  */
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Pin = SPIx_MOSI_PIN;
  GPIO_InitStruct.Alternate = SPIx_MOSI_AF;
  HAL_GPIO_Init(SPIx_MOSI_GPIO_PORT, &GPIO_InitStruct);

  /* SPI CS GPIO pin configuration  */
  GPIO_InitStruct1.Pin = SPIx_CS_PIN;
  GPIO_InitStruct1.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct1.Speed = GPIO_SPEED_HIGH;
  GPIO_InitStruct1.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(SPIx_CS_GPIO_PORT, &GPIO_InitStruct1);
}

void SPIx_IRQHandler(void)
{
  HAL_SPI_IRQHandler(&SpiHandle);
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 216000000
  *            HCLK(Hz)                       = 216000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 25000000
  *            PLL_M                          = 25
  *            PLL_N                          = 432
  *            PLL_P                          = 2
  *            PLL_Q                          = 9
  *            PLL_R                          = 7
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 7
  * @param  None
  * @retval None
  */


/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
//  /* Configure LED1 which is shared with SPI2_SCK signal */
//  BSP_LED_Init(LED1);
//  BSP_LED_Off(LED1);
//  while(1)
//  {
//    /* Toogle LED1 for error */
//    BSP_LED_Toggle(LED1);
//    HAL_Delay(1000);
//  }
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif


