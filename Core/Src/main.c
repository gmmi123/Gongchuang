/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
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
#include "adc.h"
#include "dma.h"
#include "fdcan.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "PowerWarning.h"
#include "Hardware_Task.h"
#include "Task_Management.h"
#include "bsp_fdcan.h"
#include "dm4310_drv.h"
#include "Motor_can.h"
#include "BMI088driver.h"
#include "BMI088reg.h"
#include "BMI088Middleware.h"
#include "MahonyAHRS.h"
#include "MyClaw.h"
#include "MyCarControl.h"
#include "MyLift.h"
#include "MyTurnPlate.h"
#include "MyArm.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
// 电源检测
uint16_t adc_val[2];
float vbus;
// 串口DMA空闲中断配置

uint8_t UART10_RX_BUF[UART10_RX_LEN];        // 缓存一
uint8_t UART10_RX_Second_BUF[UART10_RX_LEN]; // 缓存二
__IO uint16_t UART10_RX_STA = 0;             // 字节数


uint8_t UART7_RX_BUF[UART7_RX_LEN];        // 缓存一
uint8_t UART7_RX_Second_BUF[UART7_RX_LEN]; // 缓存二
__IO uint16_t UART7_RX_STA = 0;             // 字节数

uint8_t UART9_RX_BUF[UART9_RX_LEN];        // 缓存一
uint8_t UART9_RX_Second_BUF[UART9_RX_LEN]; // 缓存二
__IO uint16_t UART9_RX_STA = 0;             // 字节数

char UART9_flag=0;

uint8_t UART8_RX_BUF[UART8_RX_LEN];//缓存一
uint8_t UART8_RX_Second_BUF[UART8_RX_LEN];//缓存二
__IO uint16_t UART8_RX_STA = 0;	 //字节数
// 陀螺仪
float gyro[3], accel[3], temp;
// 电机
extern motor_t motor[num];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

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
  MX_DMA_Init();
  MX_TIM12_Init();
  MX_TIM6_Init();
  MX_TIM7_Init();
  MX_ADC1_Init();
  MX_FDCAN1_Init();
  MX_FDCAN2_Init();
  MX_FDCAN3_Init();
  MX_USART10_UART_Init();
  MX_UART9_Init();
  MX_SPI2_Init();
  MX_TIM2_Init();
  MX_UART7_Init();
  MX_UART8_Init();
  /* USER CODE BEGIN 2 */
  // 陀螺仪初始化
  Mahony_Init(66.67);
  while (BMI088_init())
  {
    ;
  }

  // 电源开启
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, 1);
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, 1);
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, 1);
  // 启用串口空闲中断
  //  HAL_UART_Receive_DMA(&huart1, (uint8_t *)UART1_RX_BUF, UART1_RX_LEN);
//    __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
      HAL_UART_Receive_DMA(&huart10, (uint8_t *)UART10_RX_BUF, UART10_RX_LEN);
    __HAL_UART_ENABLE_IT(&huart10, UART_IT_IDLE);
      HAL_UART_Receive_DMA(&huart7, (uint8_t *)UART7_RX_BUF, UART7_RX_LEN);
    __HAL_UART_ENABLE_IT(&huart7, UART_IT_IDLE);
      HAL_UART_Receive_DMA(&huart9, (uint8_t *)UART9_RX_BUF, UART9_RX_LEN);
   __HAL_UART_ENABLE_IT(&huart9, UART_IT_IDLE);
  	HAL_UART_Receive_DMA(&huart8, UART8_RX_BUF, UART8_RX_LEN);
	__HAL_UART_ENABLE_IT(&huart8, UART_IT_IDLE);//开启空闲中断
  // 电源ADC
  HAL_ADCEx_Calibration_Start(&hadc1, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED);
  HAL_ADC_Start_DMA(&hadc1, (uint32_t *)adc_val, 2);
  // 开启蜂鸣器
  HAL_TIM_PWM_Start(&htim12, TIM_CHANNEL_2);
  // 陀螺仪置零
  //  ResetHWT101();
  // Can初始化
  HAL_Delay(3000);
  bsp_can_init();
  // 电机初始化

  dm4310_motor_init();
  dm4310_clear_err(&hfdcan1, &motor[Motor1]);
  HAL_Delay(5);
  dm4310_clear_err(&hfdcan1, &motor[Motor2]);
  HAL_Delay(5);
  dm4310_clear_err(&hfdcan2, &motor[Motor3]);
  HAL_Delay(5);
  dm4310_clear_err(&hfdcan2, &motor[Motor4]);
  HAL_Delay(5);
  dm4310_clear_err(&hfdcan1, &motor[Motor5]);
  HAL_Delay(5);
  dm4310_clear_err(&hfdcan2, &motor[Motor6]);

  dm4310_enable(&hfdcan1, &motor[Motor1]);
  HAL_Delay(5);
  dm4310_enable(&hfdcan1, &motor[Motor2]);
  HAL_Delay(5);
  dm4310_enable(&hfdcan2, &motor[Motor3]);
  HAL_Delay(5);
  dm4310_enable(&hfdcan2, &motor[Motor4]);
  HAL_Delay(5);
  dm4310_enable(&hfdcan1, &motor[Motor5]); 
  HAL_Delay(5);
  dm4310_enable(&hfdcan2, &motor[Motor6]); 
//  HAL_Delay(100);
  //机械爪初始化
  MyArm_Init(&Arm);
	MyTurnPlate_Init(&TurnPlate);
	MyClaw_Init(&Claw);
//	MyCarControl_Init(&Car,0.02);
	MyLift_Init(&Lift);

  // 打开定时器中断
  HAL_TIM_Base_Start_IT(&htim6);
  HAL_TIM_Base_Start_IT(&htim7);
    // 开启PWM
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);
  

//  uint8_t temp[] = {'p', 'a', 'g', 'e', '0', '.', 't', '1', '.', 't', 'x', 't', '=', '\"', '1', '2', '3', '+', '3', '2', '1', '\"', 0xff, 0xff, 0xff};
//  HAL_UART_Transmit(&huart8, temp, 25, 0xffff);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    //	  uint8_t tx_data[8] = {0,1,2,3,4,5,6,7};
    //
    //	  		fdcanx_send_data(&hfdcan1, 0x520, tx_data, 8);
    //		HAL_Delay(100);
    //
    //		fdcanx_send_data(&hfdcan2, 0x520, tx_data, 8);
    //		HAL_Delay(100);
    //
    //		fdcanx_send_data(&hfdcan3, 0x520, tx_data, 8);
    //		HAL_Delay(100);

    Low_Task();
    //		//设置零点
    //	save_pos_zero(&hfdcan1,motor[Motor5].id,motor[Motor5].cmd.mode);
    HAL_Delay(1);
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Supply configuration update enable
  */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 2;
  RCC_OscInitStruct.PLL.PLLN = 40;
  RCC_OscInitStruct.PLL.PLLP = 1;
  RCC_OscInitStruct.PLL.PLLQ = 6;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_3;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

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
