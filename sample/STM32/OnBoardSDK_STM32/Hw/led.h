#ifndef LED_H
#define LED_H

#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include "sys.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void led_init(void);
void led_process(void);
void run_led(char state);
void init_led(char state);
#ifdef __cplusplus
}
#endif
#endif
