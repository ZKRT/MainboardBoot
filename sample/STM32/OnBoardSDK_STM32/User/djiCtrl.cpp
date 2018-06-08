/**
  ******************************************************************************
  * @file    djiCtrl.cpp
  * @author  ZKRT
  * @version V0.0.1
  * @date    15-March-2017
  * @brief   dji_ctrl communication with the a3 flight controller
  *          + (1) init
  ******************************************************************************
  * @attention
  *
  * ...
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "djiCtrl.h"
#include "led.h"
#include "zkrt.h"
#include "iap.h"
#include "mobileDataHandle.h"

extern Vehicle  vehicle;
extern Vehicle* v;
using namespace DJI::OSDK;
using namespace DJI::OSDK::Telemetry;

/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
dji_sdk_status djisdk_state = {init_none_djirs, 0xffffffff};  //dji sdk 运行状态
/* Private functions ---------------------------------------------------------*/
/**
  * @brief  DJI init. 大疆SDK初始化
  * @param  None
  * @retval None
  */
int dji_init(void) {
	init_led(0);
	delay_nms(5000);
	/*(1): dji requirement	*/
	// Check USART communication
	if (!v->protocolLayer->getDriver()->getDeviceStatus()) {
		printf("USART communication is not working.\r\n");
		__set_FAULTMASK(1);
		NVIC_SystemReset();
		delete (v);
		return -1;
	}
	printf("\nPrerequisites:\r\n");
	printf("1. Vehicle connected to the Assistant and simulation is ON\r\n");
	printf("2. Battery fully chanrged\r\n");
	printf("3. DJIGO App connected (for the first time run)\r\n");
	printf("4. Gimbal mounted if needed\r\n");

	delay_nms(30);

	//! Initialize functional Vehicle components like
	//! Subscription, Broabcast, Control, Camera, etc
	v->functionalSetUp();
	delay_nms(500);

	if (v->getFwVersion() == 0) {
		printf("version unmath, Upgrade firmware using Assistant software!\n");
		__set_FAULTMASK(1);
		NVIC_SystemReset();
		delete (v);
		return -1;
	}

	// Check if the firmware version is compatible with this OSDK version
	if (v->getFwVersion() > 0 &&
	        v->getFwVersion() < extendedVersionBase &&
	        v->getFwVersion() != Version::M100_31) {
		printf("version unmath, Upgrade firmware using Assistant software!\n");
		__set_FAULTMASK(1);
		NVIC_SystemReset();
		delete (v);
		return -1;
	}
	delay_nms(1000);
	return 1;
}
/**
  * @brief  dji_process.  DJI 流程
  * @param  None
  * @retval None
  */
void dji_process(void) {
	switch (djisdk_state.run_status) {
	case init_none_djirs:
		djisdk_state.run_status = set_avtivate_djirs;
		djisdk_state.cmdres_timeout = TimingDelay;
		userActivate(); 																	                        //(1)
		// Verify subscription
		if (v->getFwVersion() != Version::M100_31) {                              //(2)
			v->subscribe->verify();
			delay_nms(500);
		}
		// Re-set Broadcast frequencies to their default values
//		ACK::ErrorCode ack = v->broadcast->setBroadcastFreqDefaults(20);	    //(3)
		v->broadcast->setBroadcastFreqToZero(); //zkrt_notice: Bootloader不接收广播
		delay_nms(50);
		// Mobile Callback set                                                     //(4)
		v->moc->setFromMSDKCallback(parseFromMobileCallback_v2, (UserData)&msg_handlest);
		delay_nms(50);
		break;
	case set_avtivate_djirs:
		if (TimingDelay - djisdk_state.cmdres_timeout >= 5000) { //激活不成功时，每5秒重新激活一次
			djisdk_state.run_status = init_none_djirs;
		}
		break;
	case avtivated_ok_djirs:
		break;
	default:
		break;
	}
}
/************************ (C) COPYRIGHT ZKRT *****END OF FILE****/

