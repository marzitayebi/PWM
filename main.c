/*
 * timer_lib.c
 *
 * @author:          Marziyeh Geramitayebi

 */

  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "system_lib.h"
#include "gpio_lib.h"
#include "adc_lib.h"
#include "timer1_lib.h"
#include "timer2_lib.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */




uint8_t LedSelection[1];
int percentage;


/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
uint8_t selectLed[] = "Please Enter 1 for onboard LED or 2 for external LED\r\n";

uint8_t onbrdLed[] = "You have selected on board LED â€“ please enter % value:";

uint8_t exbrdLed[] = "You have selected extension LED â€“ please enter % value\r\n";


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MX_GPIO_Init(void);
void MX_ADC1_Init(ADC_HandleTypeDef* hadc1);
void MX_USART2_UART_Init2(UART_HandleTypeDef * huart);
void INIT_TIMER1(
    TIM_HandleTypeDef *htim,
    TIM_TypeDef *instance,
    int counterMode,
    int clockDivision, int repCounter,
    int autoReload);
//static void MX_TIM1_Init(void);
static void MX_TIM2_Init(void);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */


void uart_TxRx_data(void){

   HAL_UART_Transmit(&huart2,selectLed, sizeof(selectLed), 1000 );
   HAL_UART_Receive(&huart2, (uint8_t *) LedSelection , 1, 100);

   while (LedSelection[0]!=1 || LedSelection[0]!=2) {
   HAL_UART_Receive(&huart2, (uint8_t *) LedSelection , 1, 100);
   }
   if(LedSelection[0] == 1){

    HAL_UART_Transmit(&huart2, onbrdLed,sizeof(onbrdLed), 1000 );
    HAL_UART_Receive(&huart2, (uint8_t *) percentage , 2, 100);
   HAL_UART_Transmit(&huart2, (uint8_t *) "Setting on board LED to:", 24, 1000 );
   HAL_UART_Transmit(&huart2, (uint8_t *) percentage, 24, 1000 );
   HAL_UART_Transmit(&huart2, (uint8_t *) "% Power", 7, 1000 );
   __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,(percentage*32000)/100);

   }

   if(LedSelection[0] == 2){

      HAL_UART_Transmit(&huart2, exbrdLed,sizeof(exbrdLed), 1000 );
      HAL_UART_Receive(&huart2, (uint8_t *) percentage , 2, 100);
     HAL_UART_Transmit(&huart2, (uint8_t *) "Setting on board LED to:", 24, 1 );
     HAL_UART_Transmit(&huart2, (uint8_t *) percentage, 24, 1 );
     HAL_UART_Transmit(&huart2, (uint8_t *) "% Power", 7, 1 );
     __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,(percentage*32000)/100);

     }


}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

for(int i=0;i<1;i++){
  LedSelection[i]=0;
}
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init2(&huart2);
  MX_ADC1_Init(&hadc1);
 // MX_TIM1_Init();
  MX_TIM2_Init();
  INIT_TIMER1(&htim1,TIM1,TIM_COUNTERMODE_UP,
        TIM_CLOCKDIVISION_DIV1, 0,TIM_AUTORELOAD_PRELOAD_ENABLE);
  Set_Prescalar(&htim1,0);
    Set_Period(&htim1,65535);
      INIT_TIMER2(&htim2,TIM2,TIM_COUNTERMODE_UP,
          TIM_CLOCKDIVISION_DIV1,TIM_AUTORELOAD_PRELOAD_ENABLE);
      Set_Prescalar(&htim2,0);
           Set_Period(&htim2,65535);
  /* USER CODE BEGIN 2 */
  HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2);
  HAL_UART_Transmit(&huart2, (uint8_t*)"Assignment 3 - <Marzieh> <Gerami>\r\n", 35, 1000);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
   uart_TxRx_data(); // Read Data From Uart2

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */


  }
  /* USER CODE END 3 */
}




/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */


/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */


/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = Clock_Period;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */
  HAL_TIM_MspPostInit(&htim2);

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */



/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */


#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
