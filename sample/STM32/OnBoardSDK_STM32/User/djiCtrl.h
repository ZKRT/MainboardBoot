/**
  ******************************************************************************
  * @file    djiCtrl.h
  * @author  ZKRT
  * @version V0.0.1
  * @date    08-March-2017
  * @brief   Header for djiCtrl.cpp module
  *          + (1) init
  ******************************************************************************
  * @attention
  *
  * ...
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DJICTRL_H
#define __DJICTRL_H

/* Includes ------------------------------------------------------------------*/
#include "dji_vehicle.hpp"
#include "main.h"
/* Exported macro ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

//dji ����״̬
enum dji_run_state {
  init_none_djirs = 0,      //û������
  set_broadcastFreq_djirs,  //���͹㲥����
  set_broadcastFreq_ok,     //���ĳɹ�
  set_avtivate_djirs,       //����
  avtivated_ok_djirs        //����ɹ�
};

/* Exported typedef ------------------------------------------------------------*/

//dji ����״̬�ṹ��
typedef struct {
  dji_run_state run_status;
  unsigned int cmdres_timeout;
} dji_sdk_status;

/* Exported functions ------------------------------------------------------- */
int dji_init(void);
void dji_process(void);
extern dji_sdk_status djisdk_state;
#endif /* __DJICTRL_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT ZKRT *****END OF FILE****/
