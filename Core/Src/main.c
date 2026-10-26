/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ssd1306.h"
#include "ssd1306_fonts.h"
#include <stdio.h>
#include <math.h>
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
I2C_HandleTypeDef hi2c1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#define MPU6050_ADDR    (0x68 << 1)

#define LED_PORT        GPIOA
#define LED_PIN         GPIO_PIN_8

/* Doc gia tri anh sang LUX */

/* Khoi tao MPU6050 */
HAL_StatusTypeDef MPU6050_Init(void)
{
    uint8_t value;

    /* Thoat trang thai ngu */
    value = 0x00;
    if (HAL_I2C_Mem_Write(
            &hi2c1, MPU6050_ADDR, 0x6B,
            I2C_MEMADD_SIZE_8BIT,
            &value, 1, 100) != HAL_OK)
        return HAL_ERROR;

    /* Gyroscope: +/- 250 do/giay */
    value = 0x00;
    if (HAL_I2C_Mem_Write(
            &hi2c1, MPU6050_ADDR, 0x1B,
            I2C_MEMADD_SIZE_8BIT,
            &value, 1, 100) != HAL_OK)
        return HAL_ERROR;

    /* Accelerometer: +/- 2g */
    value = 0x00;
    if (HAL_I2C_Mem_Write(
            &hi2c1, MPU6050_ADDR, 0x1C,
            I2C_MEMADD_SIZE_8BIT,
            &value, 1, 100) != HAL_OK)
        return HAL_ERROR;

    return HAL_OK;
}

/* Doc gia toc va tinh goc nghieng */
HAL_StatusTypeDef MPU6050_ReadAngle(float *roll, float *pitch)
{
    uint8_t data[14];
    int16_t ax_raw, ay_raw, az_raw;
    int16_t gx_raw, gy_raw, gz_raw;

    static uint8_t first_run = 1;
    static float roll_filtered = 0.0f;
    static float pitch_filtered = 0.0f;
    static uint32_t previous_tick = 0;

    if (HAL_I2C_Mem_Read(
            &hi2c1,
            MPU6050_ADDR,
            0x3B,
            I2C_MEMADD_SIZE_8BIT,
            data,
            14,
            100) != HAL_OK)
    {
        return HAL_ERROR;
    }

    /* Ghep 2 byte thanh gia tri signed 16-bit */
    ax_raw = (int16_t)((data[0] << 8) | data[1]);
    ay_raw = (int16_t)((data[2] << 8) | data[3]);
    az_raw = (int16_t)((data[4] << 8) | data[5]);

    gx_raw = (int16_t)((data[8] << 8) | data[9]);
    gy_raw = (int16_t)((data[10] << 8) | data[11]);
    gz_raw = (int16_t)((data[12] << 8) | data[13]);

    /* Chuyen doi sang don vi g va do/giay */
    float ax = ax_raw / 16384.0f;
    float ay = ay_raw / 16384.0f;
    float az = az_raw / 16384.0f;

    float gx = gx_raw / 131.0f;
    float gy = gy_raw / 131.0f;
    (void)gz_raw;

    /* Tinh goc tu gia toc */
    float accel_roll =
        atan2f(ay, az) * 57.29578f;

    float accel_pitch =
        atan2f(-ax, sqrtf(ay * ay + az * az))
        * 57.29578f;

    uint32_t current_tick = HAL_GetTick();

    if (first_run)
    {
        roll_filtered = accel_roll;
        pitch_filtered = accel_pitch;
        previous_tick = current_tick;
        first_run = 0;
    }
    else
    {
        float dt = (current_tick - previous_tick) / 1000.0f;
				previous_tick = current_tick;

				if (dt <= 0.0f || dt > 0.2f)
				{
						dt = 0.0f;
				}

        const float alpha = 0.7f;

        roll_filtered =
            alpha * (roll_filtered + gx * dt)
            + (1.0f - alpha) * accel_roll;

        pitch_filtered =
            alpha * (pitch_filtered + gy * dt)
            + (1.0f - alpha) * accel_pitch;
    }

    *roll = roll_filtered;
    *pitch = pitch_filtered;

    return HAL_OK;
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
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
	ssd1306_Init();
	ssd1306_Fill(Black);
	ssd1306_UpdateScreen();

	HAL_Delay(100);

	HAL_StatusTypeDef mpu_init_status = MPU6050_Init();

if (mpu_init_status != HAL_OK)
{
    ssd1306_Fill(Black);
    ssd1306_SetCursor(0, 0);
    ssd1306_WriteString("MPU INIT ERROR",
                        Font_7x10, White);
    ssd1306_UpdateScreen();

    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8,
                      GPIO_PIN_RESET);

    while (1)
    {
        // Dung tai day neu MPU6050 khoi tao that bai
    }
}

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
		float roll = 0.0f;
    float pitch = 0.0f;

    HAL_StatusTypeDef angle_ok =
        MPU6050_ReadAngle(&roll, &pitch);

    /* Bat LED neu goc nghieng vuot 20 do */
    if (angle_ok == HAL_OK &&
        (fabsf(roll) > 20.0f || fabsf(pitch) > 20.0f))
    {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
    }
    else
    {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
    }

    /* Cap nhat OLED */
    ssd1306_Fill(Black);

    char line[24];

    ssd1306_SetCursor(0, 0);
    ssd1306_WriteString("MPU6050", Font_7x10, White);

    ssd1306_SetCursor(0, 14);

    if (angle_ok == HAL_OK)
    {
        sprintf(line, "Roll: %d deg", (int)roll);
        ssd1306_WriteString(line, Font_7x10, White);
    }
    else
    {
        ssd1306_WriteString("MPU ERROR", Font_7x10, White);
    }

    ssd1306_SetCursor(0, 28);

    if (angle_ok == HAL_OK)
    {
        sprintf(line, "Pitch: %d deg", (int)pitch);
        ssd1306_WriteString(line, Font_7x10, White);
    }

    ssd1306_SetCursor(0, 42);

    if (angle_ok == HAL_OK &&
        (fabsf(roll) > 20.0f || fabsf(pitch) > 20.0f))
    {
        ssd1306_WriteString("TILT WARNING", Font_7x10, White);
    }
    else if (angle_ok == HAL_OK)
    {
        ssd1306_WriteString("NORMAL", Font_7x10, White);
    }
    else
    {
        ssd1306_WriteString("CHECK MPU", Font_7x10, White);
    }

    ssd1306_UpdateScreen();

    HAL_Delay(50);
    /* USER CODE BEGIN 3 */
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */
	
  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */
	
  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);

  /*Configure GPIO pin : PA8 */
  GPIO_InitStruct.Pin = GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
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
#ifdef USE_FULL_ASSERT
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
