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

//�������̴���ṹ��
typedef struct {
	u32 flashdestination;  //api�洢��ַ�����Ź̼�д�벻�ϵ�����ֵַ
	u8 packet_seq; //���������е����ݰ����к�
	u32 fw_total_size; //���������Ĺ̼���С����׼������ʱ���Ͷ��ṩ
	u32 fw_recved_size; //���������н��յ��̼������ۼƴ�С
	u8 fw_name[30];
	u8 api_enabled; //api�����Ƿ���Ч�ı��---flash�洢 //0-invalid, 1-valid
	u8 program_mode; //����֮������boot����api�ı��---flash�洢 //1-boot,2-api
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
