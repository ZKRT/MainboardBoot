#include "sys.h"
#include "core_cm4.h"
#include "stm32f4xx_pwr.h"
volatile uint32_t TimingDelay = 0;
volatile uint32_t mobile_recv_time = 0;
volatile uint32_t mobile_send_time = 0;

void
delay_nms(uint16_t time) {
	u32 i = 0;
	while (time--) {
		i = 30000;
		while (i--)
			;
	}
}
//void delay_init(void) {
//	while (SysTick_Config(168000000 / 1000) != 0);
//}

//void delay_us(uint16_t nus) {
//	uint32_t start_num;
//	uint32_t temp;
//	uint32_t nus_pai;

//	nus_pai = 168 * nus;
//	start_num = SysTick->VAL;
//	do {
//		temp = SysTick->VAL;
//		if (temp < start_num) {
//			temp = start_num - temp;
//		} else {
//			temp = start_num + 168000 - temp;
//		}
//	} while (temp < nus_pai);
//}
//void delay_ms(uint16_t nms) {							//��ഫ��65535ms
//	uint32_t start_num;

//	start_num = TimingDelay;
//	while ((start_num - TimingDelay) < nms);
//}
//#ifdef __cplusplus  //zkrt_test
//extern "C" {
//#endif //__cplusplus
////���������¼�����ÿ�ζ�ʱ������Ϊ0��ʱ�򣬻ᴥ��һ���жϺ�����Ҫ1msʱ��
//void SysTick_Handler(void) {
//	TimingDelay--;
//}
//#ifdef __cplusplus
//}
//#endif //__cplusplus

