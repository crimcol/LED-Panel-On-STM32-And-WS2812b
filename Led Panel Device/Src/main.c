/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  *
  * COPYRIGHT(c) 2016 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f1xx_hal.h"

/* USER CODE BEGIN Includes */
#include "ws2812b.h"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim3;
DMA_HandleTypeDef hdma_tim3_ch4_up;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void Error_Handler(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_TIM3_Init(void);
static void MX_USART1_UART_Init(void);                                    
void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);
                                

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

const uint8_t ledsNumber = 90;
const uint8_t preambleSize = 2;
const uint8_t preambleShift = preambleSize - 1;
//LedColor leds[ledsNumber];
PwmColor pwmColor[preambleSize + ledsNumber];

uint16_t bufferSize = (preambleSize + ledsNumber) * 3 * 8;
volatile uint8_t setLeds = 0;

const uint16_t colorPacketSize = ledsNumber * 3 + 1;
uint8_t aRxBuffer[colorPacketSize];
uint8_t currentLedNumber = 0;
uint8_t colorPosition = 0;
uint8_t receivedByte = 0;
    
void SetLedColor(/*LedColor * leds,*/ PwmColor *pwmColor, uint16_t index, LedColor *color)
{
    //leds[index].Red = color->Red;
    //leds[index].Green = color->Green;
    //leds[index].Blue = color->Blue;
    
    ConvertColorLedToPwm(color, &pwmColor[preambleShift + index]);
}

void SetAllLedColor(LedColor * leds, PwmColor *pwmColor, LedColor *color)
{
    for(int i = 0; i < ledsNumber; i++)
    {
        SetLedColor(/*leds, */pwmColor, i, color);
    }
}

void ClearAllLeds(/*LedColor * leds, */PwmColor *pwmColor)
{
    //for(int i = 0; i < ledsNumber; i++)
    //{
    //    ClearLedColor(&leds[i]);
    //}
    
    for(int i = 0; i < preambleSize + ledsNumber; i++)
    {
        ClearPwmColor(&pwmColor[i]);
    }
}

void ConvertAllLedsToPwm(LedColor * leds, PwmColor *pwmColor)
{
    for(int i = 0; i < ledsNumber; i++)
    {
        ConvertColorLedToPwm(&leds[i], &pwmColor[preambleShift + i]);
    }
}

void MoveLeds(LedColor * leds, PwmColor *pwmColor)
{
    LedColor temp = leds[ledsNumber - 1];
    
    for(int i=ledsNumber - 1; i > 0; i--)
    {
        leds[i] = leds[i - 1];
    }
    leds[0] = temp;
}
/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_TIM3_Init();
  MX_USART1_UART_Init();

  /* USER CODE BEGIN 2 */
	//HAL_TIM_Base_Start_IT(&htim6);
	//HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);
	//TIM3->CCR4 = 20;
    ClearAllLeds(/*leds, */pwmColor);
    uint8_t light = 2;
    
    LedColor mixColor = { .Red = light, .Green = light, .Blue = light};    
    LedColor greenColor = { .Red = 0, .Green = light, .Blue = 0};
    LedColor redColor = { .Red = light, .Green = 0, .Blue = 0};
    LedColor blueColor = { .Red = 0, .Green = 0, .Blue = light};
    LedColor yellowColor = { .Red = light, .Green = light, .Blue = 0};
    
    //for(int i=0; i<ledsNumber; i++)
    //{
    //    if (i%9 == 0)
    //    {
    //        SetLedColor(leds, pwmColor, i, &greenColor);
    //    }
    //    else if (i%6 == 0)
    //    {
    //        SetLedColor(leds, pwmColor, i, &blueColor);
    //    }
    //    else if (i%3 == 0)
    //    {
    //        SetLedColor(leds, pwmColor, i, &redColor);
    //    }
    //    else
    //    {
    //        SetLedColor(leds, pwmColor, i, &yellowColor);
    //    }
    //}
    
    SetLedColor(/*leds, */pwmColor, 0, &greenColor);
    SetLedColor(/*leds, */pwmColor, 1, &redColor);
    SetLedColor(/*leds, */pwmColor, 2, &blueColor);
    SetLedColor(/*leds, */pwmColor, 3, &yellowColor);
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
    HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_4, (uint32_t *)pwmColor, bufferSize);
    
    //HAL_UART_Transmit(&huart1, (uint8_t *)"hello\r\n", 7, 5000);
    //HAL_Delay(5000);
    //HAL_UART_Transmit_IT(&huart1, (uint8_t *)"hello\r\n", 7);
    HAL_UART_Receive_IT(&huart1, aRxBuffer, colorPacketSize);
    
	//HAL_Delay(1000);
	//HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_4, (uint32_t *)pwmColor, bufferSize);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
    uint16_t i = 0;
    LedColor tempColor = { .Red = 0, .Green = 0, .Blue = 0};
    while (1)
    {
        if (setLeds == 1)
        {
            
            for(i = 0; i < ledsNumber; i++)
            {
                
                tempColor.Red = aRxBuffer[1 + i * 3 + 0];
                tempColor.Green = aRxBuffer[1 + i * 3 + 1];
                tempColor.Blue = aRxBuffer[1 + i * 3 + 2];
                SetLedColor(pwmColor, i, &tempColor);
            }
            //ConvertAllLedsToPwm(leds, pwmColor);
            
            HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_4, (uint32_t *)pwmColor, bufferSize);
            //HAL_UART_Receive_IT(&huart1, aRxBuffer, colorPacketSize);
            HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
            
            setLeds = 0;
        }
        
        //redColor.Red = light + 1;
        //SetLedColor(leds, pwmColor, 0, &redColor);
        //HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_4, (uint32_t *)pwmColor, bufferSize);
        //light++;
        //
        //if (light == 255)
        //{
        //    light = 0;
        //}
        //HAL_Delay(125);
        //MoveLeds(leds, pwmColor);
        //ConvertAllLedsToPwm(leds, pwmColor);
        //HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_4, (uint32_t *)pwmColor, bufferSize);
        //HAL_Delay(250);
        
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */

    }
  /* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* TIM3 init function */
static void MX_TIM3_Init(void)
{

  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;
  TIM_OC_InitTypeDef sConfigOC;

  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 0;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 30;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }

  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }

  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
  {
    Error_Handler();
  }

  HAL_TIM_MspPostInit(&htim3);

}

/* USART1 init function */
static void MX_USART1_UART_Init(void)
{

  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }

}

/** 
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void) 
{
  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel3_IRQn);

}

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
static void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);

  /*Configure GPIO pin : PB0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
//{
//	HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
//}

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
	HAL_TIM_PWM_Stop_DMA(&htim3, TIM_CHANNEL_4);
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    
    setLeds = 1;
    HAL_UART_Receive_IT(&huart1, aRxBuffer, colorPacketSize);
    /*
    receivedByte = aRxBuffer[0];
    
    if (receivedByte == 1)
    {
        currentLedNumber = 0;
        colorPosition = 0;
        return;
    }
    else if (currentLedNumber != ledsNumber)
    {
        if (colorPosition == 0)
        {
            leds[currentLedNumber].Red = receivedByte;
            colorPosition++;
        }
        else if (colorPosition == 1)
        {
            leds[currentLedNumber].Green = receivedByte;
            colorPosition++;
        }
        else if (colorPosition == 2)
        {
            leds[currentLedNumber].Blue = receivedByte;
            colorPosition = 0;
            currentLedNumber++;
        }
    }
    
    if (currentLedNumber == ledsNumber - 1)
    {
        //HAL_UART_Transmit_IT(&huart1, (uint8_t*)"OK\n\r", 4);
        ConvertAllLedsToPwm(leds, pwmColor);
        setLeds = 1;
    }
    HAL_UART_Receive_IT(&huart1, aRxBuffer, 1);
    */
    //LedColor receivedColor = { .Red = aRxBuffer[1], .Green = aRxBuffer[2], .Blue = aRxBuffer[3]};
    //SetAllLedColor(leds, pwmColor, &receivedColor);
    //HAL_UART_Transmit_IT(&huart1, (uint8_t*)"OK\n\r", 4);
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
  }
  /* USER CODE END Error_Handler */ 
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
