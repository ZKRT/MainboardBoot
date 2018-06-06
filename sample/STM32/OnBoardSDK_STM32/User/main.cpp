/*! @file main.cpp
 *  @version 3.3
 *  @date 2018.06
 * */

#include "stm32f4xx.h"
#include "main.h"
#include "djiCtrl.h"
#include "iap.h"
#include "flash_if.h"

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
	printf("Mainborad Boot Start.\r\n");
	iap_init();
	FLASH_If_Init(); /* Unlock the Flash Program Erase controller */
	if (dji_init() < 0)
		return -1;
	printf("Mainborad Boot Enter Loop.\r\n");
	while (1) {
		dji_process();
		iap_process();
	}
}
