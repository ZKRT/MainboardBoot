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

void delay_nms(uint16_t time);
#ifdef __cplusplus
}
#endif

#endif
