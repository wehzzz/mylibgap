/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
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
#include <gapcom/gapcom.h>
#include <gapcom/gapcom_server.h>
#include <communicator.h>
#include <communicator_impl_server.h>
#include <gap.pb.h>
#include <gapcom_core.h>

#include "utils.h"
#include "callbacks.h"
#include "logger.h"
#include "mpu6050.h"
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

UART_HandleTypeDef huart7;
UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */
log_level global_level = LOG_INFO;

static gapcom_handle_t *handle;
static communicator_t *communicator;

uint8_t RX_Buffer[GAPCOM_RECVBUF_SIZE];
static int protobuf_len = -1; // -1 means no header received, else it's the protocol buffer length
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_UART7_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_I2C1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
static void UART_RXComplete(UART_HandleTypeDef *huart) {
	if (huart != &huart7) {
		return;
	}

	// TinyFrame header received, decode it to extract protocol buffer length
	if (protobuf_len == -1) {
		// Extract len
		// TODO: not sure if it's working
		protobuf_len = get_protobuf_len((uint8_t*) RX_Buffer);
		if (protobuf_len == 0) // No body means no data checksum
			goto accept;

		communicator->recv(communicator,
				(uint8_t*) (RX_Buffer + GAPCOM_TF_HEADER_SIZE_BYTES),
				protobuf_len + GAPCOM_TF_FOOTER_SIZE_BYTES);
	}
	// Protocol buffer and data checksum received
	else {
		accept:
		// Body + Data checksum received, we can call accept
		if (protobuf_len == 0) {
			gapcom_accept(handle, (uint8_t*) RX_Buffer,
			GAPCOM_TF_HEADER_SIZE_BYTES);
		} else {
			gapcom_accept(handle, (uint8_t*) RX_Buffer,
					GAPCOM_TF_HEADER_SIZE_BYTES + protobuf_len
							+ GAPCOM_TF_FOOTER_SIZE_BYTES);
		}
		protobuf_len = -1;

		// Receive next header
		communicator->recv(communicator, (uint8_t*) RX_Buffer,
				GAPCOM_TF_HEADER_SIZE_BYTES);
	}
}

static int register_HAL_UART_Callback() {
	HAL_StatusTypeDef status;

	// Flash a green LED when a UART7 transmission is complete
	status = HAL_UART_RegisterCallback(&huart7, HAL_UART_TX_COMPLETE_CB_ID,
			flash_led_tx_complete);
	if (status != HAL_OK) {
		flash_red_led();
		return 1;
	}

	// Flash a red LED when a UART7 communication got an error
	status = HAL_UART_RegisterCallback(&huart7, HAL_UART_ERROR_CB_ID,
			flash_red_led);
	if (status != HAL_OK) {
		flash_red_led();
		return 1;
	}
	return 0;
}
/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {

	/* USER CODE BEGIN 1 */
	GyroData gyro_data;
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
	MX_UART7_Init();
	MX_USART1_UART_Init();
	MX_I2C1_Init();
	/* USER CODE BEGIN 2 */

	// Register basic UART7 callback to flash LED on message transmission
	register_HAL_UART_Callback();
	// Register UART7 RX complete callback
	HAL_UART_RegisterCallback(&huart7, HAL_UART_RX_COMPLETE_CB_ID,
			UART_RXComplete);

	// Init Gapcom
	handle = gapcom_create();
	communicator = communicator_impl_server_get();
	gapcom_set_sender_impl(handle, (gapcom_sender_t*) communicator);

	// Register Gapcom callback
	gapcom_install_callback(handle, &ping_callback, GAPCOM_MSG_PING_REQ);
	gapcom_install_callback(handle, &set_log_verbosity_callback,
			GAPCOM_MSG_SET_LOG_VERBOSITY_REQ);
	gapcom_install_callback(handle, &set_gyroscope_callback,
			GAPCOM_MSG_SET_GYROSCOPE_REQ);

	// Receive first header
	communicator->recv(communicator, (uint8_t*) RX_Buffer,
			GAPCOM_TF_HEADER_SIZE_BYTES);

	gap_log(LOG_INFO, "GAP system booted");
	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {
		if (MPU6050_GetData(&gyro_data) == HAL_OK) {
			gap_log_gyro(&gyro_data);
		}
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
	}
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

	/** Configure the main internal regulator output voltage
	 */
	__HAL_RCC_PWR_CLK_ENABLE();
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK) {
		Error_Handler();
	}
}

/**
 * @brief I2C1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_I2C1_Init(void) {

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
	if (HAL_I2C_Init(&hi2c1) != HAL_OK) {
		Error_Handler();
	}

	/** Configure Analogue filter
	 */
	if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE)
			!= HAL_OK) {
		Error_Handler();
	}

	/** Configure Digital filter
	 */
	if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN I2C1_Init 2 */

	/* USER CODE END I2C1_Init 2 */

}

/**
 * @brief UART7 Initialization Function
 * @param None
 * @retval None
 */
static void MX_UART7_Init(void) {

	/* USER CODE BEGIN UART7_Init 0 */

	/* USER CODE END UART7_Init 0 */

	/* USER CODE BEGIN UART7_Init 1 */

	/* USER CODE END UART7_Init 1 */
	huart7.Instance = UART7;
	huart7.Init.BaudRate = 9600;
	huart7.Init.WordLength = UART_WORDLENGTH_9B;
	huart7.Init.StopBits = UART_STOPBITS_1;
	huart7.Init.Parity = UART_PARITY_EVEN;
	huart7.Init.Mode = UART_MODE_TX_RX;
	huart7.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart7.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&huart7) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN UART7_Init 2 */

	/* USER CODE END UART7_Init 2 */

}

/**
 * @brief USART1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART1_UART_Init(void) {

	/* USER CODE BEGIN USART1_Init 0 */

	/* USER CODE END USART1_Init 0 */

	/* USER CODE BEGIN USART1_Init 1 */

	/* USER CODE END USART1_Init 1 */
	huart1.Instance = USART1;
	huart1.Init.BaudRate = 9600;
	huart1.Init.WordLength = UART_WORDLENGTH_9B;
	huart1.Init.StopBits = UART_STOPBITS_1;
	huart1.Init.Parity = UART_PARITY_EVEN;
	huart1.Init.Mode = UART_MODE_TX_RX;
	huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart1.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&huart1) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN USART1_Init 2 */

	/* USER CODE END USART1_Init 2 */

}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void) {
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	/* USER CODE BEGIN MX_GPIO_Init_1 */
	/* USER CODE END MX_GPIO_Init_1 */

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOF_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOG_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13 | GPIO_PIN_14, GPIO_PIN_RESET);

	/*Configure GPIO pins : PG13 PG14 */
	GPIO_InitStruct.Pin = GPIO_PIN_13 | GPIO_PIN_14;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

	/* USER CODE BEGIN MX_GPIO_Init_2 */
	/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
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
