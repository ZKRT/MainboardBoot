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

//dji 运行状态
enum dji_run_state {
  init_none_djirs = 0,      //没有连接
  set_broadcastFreq_djirs,  //发送广播订阅
  set_broadcastFreq_ok,     //订阅成功
  set_avtivate_djirs,       //激活
  avtivated_ok_djirs        //激活成功
};

/* Exported typedef ------------------------------------------------------------*/

//dji 运行状态结构体
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
