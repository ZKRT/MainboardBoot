/**
  ******************************************************************************
  * @file    led.c
  * @author  ZKRT
  * @version V2.0
  * @date    2018-05-28
  * @brief   led
 ===============================================================================
  * @attention
  *
  * ...
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "led.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
  * @brief  led_init.
  * @param  None
  * @retval None
  */
void led_init(void) {
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOF | RCC_AHB1Periph_GPIOG, ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd   = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_7 | GPIO_Pin_8;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(GPIOF, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8;
	GPIO_Init(GPIOG, &GPIO_InitStructure);

	GPIO_SetBits(GPIOB, GPIO_Pin_14 | GPIO_Pin_15);
	GPIO_SetBits(GPIOC, GPIO_Pin_12);
	GPIO_SetBits(GPIOD, GPIO_Pin_7 | GPIO_Pin_8);
	GPIO_SetBits(GPIOF, GPIO_Pin_11);
	GPIO_SetBits(GPIOG, GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8);
	delay_nms(1000);
}
/**
  * @brief  led_process. ledµÆ¿ØÖÆ
  * @param  None
  * @retval None
  */
void led_process(void) {
	if ((TimingDelay - mobile_recv_time) > 50) {
		GPIO_SetBits(GPIOC, GPIO_Pin_12);
	}
	if ((TimingDelay - mobile_send_time) > 50) {
		GPIO_SetBits(GPIOG, GPIO_Pin_8);
	}
}
void run_led(char state) {
	if (state)
		GPIO_SetBits(GPIOF, GPIO_Pin_11);
	else
		GPIO_ResetBits(GPIOF, GPIO_Pin_11);
}
void init_led(char state) {
	if (state)
		GPIO_SetBits(GPIOB, GPIO_Pin_14);
	else
		GPIO_ResetBits(GPIOB, GPIO_Pin_14);
}

/************************ (C) COPYRIGHT ZKRT *****END OF FILE****/
