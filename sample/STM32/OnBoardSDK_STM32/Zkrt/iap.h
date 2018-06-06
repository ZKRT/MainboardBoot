#ifndef __IAP_H
#define __IAP_H

#ifdef __cplusplus
extern "C" {
#endif
#include "zkrt.h"

//api_enabled
#define API_INVALID        0
#define API_VALID          1
//program_mode
#define BOOT_MODE          1
#define API_MODE           2

//升级流程处理结构体
typedef struct {
	u32 flashdestination;  //api存储地址，随着固件写入不断递增地址值
	u8 packet_seq; //升级过程中的数据包序列号
	u32 fw_total_size; //本次升级的固件大小，在准备升级时发送端提供
	u32 fw_recved_size; //升级过程中接收到固件数据累计大小
	u8 fw_name[30];
	u8 api_enabled; //api程序是否有效的标记---flash存储 //0-invalid, 1-valid
	u8 program_mode; //重启之后运行boot还是api的标记---flash存储 //1-boot,2-api
////else
	u8 need_reset_flag;
} iap_handle_st;

void iap_init(void);
void iap_process(void);
uint8_t iap_jump2appliacation(void);
u8 iap_start(u32 newfwsize, u8* newfwname);
u8 iap_updating(u8 seq, u8* packet);
u8 iap_updated_ok(u8 last_seq);
u8 iap_cancel(void);
extern iap_handle_st iaphandle;
#ifdef __cplusplus
}
#endif
#endif
