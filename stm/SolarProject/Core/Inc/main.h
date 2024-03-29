/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
extern void mainloop();
extern void mainsetup();
extern void customUART(char c);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define AzEna_Pin GPIO_PIN_0
#define AzEna_GPIO_Port GPIOC
#define AzDir_Pin GPIO_PIN_1
#define AzDir_GPIO_Port GPIOC
#define ElPul_Pin GPIO_PIN_0
#define ElPul_GPIO_Port GPIOA
#define ElDir_Pin GPIO_PIN_1
#define ElDir_GPIO_Port GPIOA
#define ElEna_Pin GPIO_PIN_4
#define ElEna_GPIO_Port GPIOA
#define ElYellow_Pin GPIO_PIN_5
#define ElYellow_GPIO_Port GPIOA
#define ElGreen_Pin GPIO_PIN_6
#define ElGreen_GPIO_Port GPIOA
#define ElPurple_Pin GPIO_PIN_7
#define ElPurple_GPIO_Port GPIOA
#define AzOrange_Pin GPIO_PIN_5
#define AzOrange_GPIO_Port GPIOC
#define AzPul_Pin GPIO_PIN_0
#define AzPul_GPIO_Port GPIOB
#define AzWhite_Pin GPIO_PIN_1
#define AzWhite_GPIO_Port GPIOB
#define AzGrey_Pin GPIO_PIN_2
#define AzGrey_GPIO_Port GPIOB
#define I2C2_SCL_D6_Pin GPIO_PIN_10
#define I2C2_SCL_D6_GPIO_Port GPIOB
#define AzPurple_Pin GPIO_PIN_12
#define AzPurple_GPIO_Port GPIOB
#define AzRedWhite_Pin GPIO_PIN_13
#define AzRedWhite_GPIO_Port GPIOB
#define AzBlackWhite_Pin GPIO_PIN_14
#define AzBlackWhite_GPIO_Port GPIOB
#define AzRed_Pin GPIO_PIN_6
#define AzRed_GPIO_Port GPIOC
#define AzBlack_Pin GPIO_PIN_8
#define AzBlack_GPIO_Port GPIOC
#define ElBlack_Pin GPIO_PIN_9
#define ElBlack_GPIO_Port GPIOC
#define ElWhite_Pin GPIO_PIN_8
#define ElWhite_GPIO_Port GPIOA
#define ElGrey_Pin GPIO_PIN_9
#define ElGrey_GPIO_Port GPIOA
#define AzGreen_Pin GPIO_PIN_11
#define AzGreen_GPIO_Port GPIOA
#define AzYellow_Pin GPIO_PIN_12
#define AzYellow_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define I2C2_SDA_D3_Pin GPIO_PIN_3
#define I2C2_SDA_D3_GPIO_Port GPIOB
#define ElBlackWhite_Pin GPIO_PIN_4
#define ElBlackWhite_GPIO_Port GPIOB
#define ElRedWhite_Pin GPIO_PIN_5
#define ElRedWhite_GPIO_Port GPIOB
#define ElRed_Pin GPIO_PIN_8
#define ElRed_GPIO_Port GPIOB
#define ElOrange_Pin GPIO_PIN_9
#define ElOrange_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */


/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
