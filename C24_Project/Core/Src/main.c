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
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include <stdio.h>

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

// MPU6050_SCL PB10
// MPU6050_SDA PB11
// Pot         PA1
// Mosfet 3713 PA0

#define MPU6050_I2C_Address 0xD0
// KNOWLEGDE: I2C SLAVE ADDRESSING
// In common I2C devices, almost use 7 bits addressing.
// Means that first seven bits is the address of the I2C device, last bit is Write/Read flag (0 is write and 1 is read)
// specifically, MPU6050 default address is 0x68 according to the datasheet. 0x68 convert to binary is 1101000 (7 bits). So in order to write to MPU6050, we have to put 0 to the LSB bit result 1101000 which is 0xD0 in hex.

// https://invensense.tdk.com/wp-content/uploads/2015/02/MPU-6000-Register-Map1.pdf
#define PWR_MGMT_1 0x6B        // Register 107: This register configure the power mode and clock source. It also provides a bit for resetting the entire device, and a bit for disabling the temperature sensor.
#define SMPLRT_DIV 0x19        // Register 25 : This register specifies the divider from the gyroscope output rate used to generate the Sample Rate for the MPU-60X0.
#define GYRO_CONFIG 0x1B       // Register 27 : This register configure the Gyroscope full scale range.
#define ACCEL_CONFIG 0x1c      // Register 28 : This register configure the Accelerometer full scale range.
#define RAW_OUT_START 0x3B     // Register 59 -> 64 store the most recent accelerometer measurements. Register 67 -> 72 store the most recent gyroscope measurements.

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

double AccX, AccY, AccZ;    /* Accelerometer of 3 axis */
double GyroXRate;    /* rate of change of angle (angular velocity) */
double GyroXAngle = 180;    /* Start position */
uint32_t timer;
long lastMillis = 0;
int lightDelay = 200;
int darkDelay;
int minDarkDelay = 1;
int maxDarkDelay = 50;
int angle;
int mode = 1;

void HAL_DelayMicroseconds (uint16_t us)
{
	__HAL_TIM_SET_COUNTER(&htim1,0);
	while (__HAL_TIM_GET_COUNTER(&htim1) < us);
}

double map(double value, int fromLow, int fromHigh, int toLow, int toHigh) 
{
    return (value - fromLow) * (toHigh - toLow) / (fromHigh - fromLow) + toLow;
}

void MPU6050Init()
{
	uint8_t pData;

	pData = 0x08;

	HAL_I2C_Mem_Write(&hi2c2, MPU6050_I2C_Address, PWR_MGMT_1, 1, &pData, 1, 1000); // pData = 0x08 will set the MPU6050 to normal mode (no sleep mode or cycle mode), disable internal temperature sensor (no need) and set the clock source to internal 8MHz oscillator.
	
  pData = 0x07;

	HAL_I2C_Mem_Write(&hi2c2, MPU6050_I2C_Address, SMPLRT_DIV, 1, &pData, 1, 1000);     // pData = 0x07 will set the Sample rate of MPU6050 to 1kHz according to this formula: Sample Rate = Gyroscope Output Rate / (1 + SMPLRT_DIV). The Gyroscope Output Rate = 8kHz (We will not use the Digital Low Pass Filter).
	
  pData = 0x00;
	
  HAL_I2C_Mem_Write(&hi2c2, MPU6050_I2C_Address, GYRO_CONFIG, 1, &pData, 1, 1000);    // pData = 0x00 will set the Full scale range of Gyroscope to +-250 degree/s and disable self-test
	
  pData = 0x00;
	
  HAL_I2C_Mem_Write(&hi2c2, MPU6050_I2C_Address, ACCEL_CONFIG, 1, &pData, 1, 1000);   // pData = 0x00 will set the Full scale range of Accelerometer to +-2g and disable self-test
}

void MPU6050Read()
{
	uint8_t data[10];
	
  uint16_t Accel_X_RAW, Accel_Y_RAW, Accel_Z_RAW, Gyro_X_RAW;
	
  HAL_I2C_Mem_Read (&hi2c2, MPU6050_I2C_Address, RAW_OUT_START, 1, data, 10, 1000);

/*
 * KNOWLEGDE: Get and store 16-bit values
 * Each register return 1 byte (8 bits) so we will read 6 bytes together start from ACCEL_OUT_START for Accelerometer and GYRO_OUT_START for Gyroscope
 *
 * Accelerometer for example, 6 Bytes from 6 Register in order:
 *     X higher 8 bits
 *     X lower 8 bits
 *     Y higher 8 bits
 *     Y lower 8 bits
 *     Z higher 8 bits
 *     Z lower 8 bits
 * Gyroscope is the same.
 * In order to have 16 bits value, we shift the higher 8 bits to the left and then ‘OR’ it with the lower 8 bits.
 *	Accel_X_RAW = data[0] << 8 | data[1];
 *	Accel_Y_RAW = data[2] << 8 | data[3];
 *	Accel_Z_RAW = data[4] << 8 | data[5];
 *	Gyro_X_RAW = data[8] << 8 | data[9];
 *
 * To get the g value of Accelerometer, we divide the raw value from 16384.0 (according to the datasheet about the LSB Sensitivity)
 * Similar to Accelerometer, to get the dps (degree per second) value of Gyroscope, we divide the raw value from 131.0 (according to the datasheet about the LSB Sensitivity).
 *	AccX = Accel_X_RAW/16384.0;
 *	AccY = Accel_Y_RAW/16384.0;
 *	AccZ = Accel_Z_RAW/16384.0;
 *	GyroXRate = Gyro_X_RAW/131.0;
 */
}

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
  
  MX_I2C2_Init();
  
  MX_ADC1_Init();
  
  MX_TIM1_Init();
  
  MX_TIM2_Init();
  
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

  __HAL_TIM_SET_COUNTER(&htim2,0);
  
  timer = __HAL_TIM_GET_COUNTER(&htim2);
  
  MPU6050Init();
  
  HAL_ADC_Start(&hadc1);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    MPU6050Read();

    GyroXAngle += GyroXRate*((__HAL_TIM_GET_COUNTER(&htim2) - timer)/1000000);    /* Calculate gyro angle using the unbiased rate */
    
    timer = __HAL_TIM_GET_COUNTER(&htim2);

    if(AccZ > 1.5 && (HAL_GetTick() - lastMillis > 300))
    {
      if(mode == 1)
      {
    	  mode = 0;
      }
      else 
      {
    	  mode = 1;
      }
      lastMillis = HAL_GetTick();
    }
    if(mode == 1)
    {
      angle = 250 - GyroXAngle;    /* use this angle to calculate to make object moving backward or forward */

      darkDelay = map(HAL_ADC_GetValue(&hadc1), 0, 4095, minDarkDelay, maxDarkDelay);
      
      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, 1);    /* make the LED blink with suitable frequency */
      
      HAL_DelayMicroseconds(lightDelay);
      
      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, 0);
      
      HAL_Delay(darkDelay);
      
      HAL_DelayMicroseconds(2000 + angle*10);    /* extra delay base on the angle to make object moving backward or forward */
    }
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
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
