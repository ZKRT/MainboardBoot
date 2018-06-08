#ifndef __SYS_H
#define __SYS_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

extern volatile uint32_t TimingDelay;
extern volatile uint32_t mobile_recv_time;
extern volatile uint32_t mobile_send_time;

void delay_nms(uint16_t time);
#ifdef __cplusplus
}
#endif

#endif
