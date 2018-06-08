/*! @file main.cpp
 *  @version 3.3
 *  @date 2018.06
 * */

#include "stm32f4xx.h"
#include "main.h"
#include "djiCtrl.h"
#include "iap.h"
#include "flash_if.h"
#include "led.h"

/*-----------------------DJI_LIB VARIABLE-----------------------------*/
using namespace DJI::OSDK;
bool           threadSupport = false;
bool           isFrame       = false;
RecvContainer  receivedFrame;
RecvContainer* rFrame  = &receivedFrame;
Vehicle        vehicle = Vehicle(threadSupport);
Vehicle*       v       = &vehicle;

int main() {
	BSPinit();
	printf("************************[Mboot]Start\r\n");
	printf("************************[Mboot]\r\n");
	FLASH_If_Init(); /* Unlock the Flash Program Erase controller */
	iap_init();
	if (dji_init() < 0)
		return -1;
	printf("************************[Mboot]Enter Loop.\r\n");
	while (1) {
		led_process();
		dji_process();
		iap_process();
	}
}
