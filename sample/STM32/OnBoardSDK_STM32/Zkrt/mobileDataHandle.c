/*
 *  File name   : mobile_data_handle.c
 *  Created on  :
 *  Author      :
 *  Description :
 *  Version     :
 *  History     : <author>    <time>    <version>   <desc>
 */
#include "zkrt.h"
#include "mobileDataHandle.h"
#include "appprotocol.h"
#include "iap.h"
#include "flash_if.h"
/////////////////////////////////////////////////////////////////////valible define
msg_handle_st msg_handlest; //消息处理
zkrt_packet_t *main_dji_rev; //point in msg_handle_init

void sendToMobile(uint8_t *data, uint8_t len);
////////////////////////////////////////////////////////////////////static function define
static void mobile_data_handle(void);
static void copydataformmobile(const u8* sdata, u8 sdatalen);
/**
 * @brief msg_handle_init
 * @param
 * @param
 * @return
 **/
void msg_handle_init(void) {
	main_dji_rev = &msg_handlest.recvpacket_app;
	memset(&msg_handlest, 0x00, sizeof(msg_handlest));
}
/*
* @brief 将接收到的mobile透传数据进行解析处理
  mobile数据来源是DJI串口透传数据，经过UART1中断接收解析处理过，拷贝到数组
*/
void mobile_data_process(void) {
	if (msg_handlest.datalen_recvapp != 0) {
		copydataformmobile(msg_handlest.data_recv_app, msg_handlest.datalen_recvapp);
		mobile_data_handle();
		msg_handlest.datalen_recvapp = 0;
	}
}
/**
 * @brief mobile data handle
 * @param
 * @param
 * @return
 **/
void mobile_data_handle(void) {
	int k;
	u8 localmsg_type = 0;
	char respondflag   = 0;
	u8 datalen       = main_dji_rev->length;
//check length
	if (msg_handlest.datalen_recvapp != main_dji_rev->length + ZK_FIXED_LEN)
		return;
//check packet format
	if (zkrt_check_packet(main_dji_rev) == false)
		return;
	for (k = 0; k < msg_handlest.datalen_recvapp; k++) {
		printf("%x ", msg_handlest.data_recv_app[k]);
	}
	printf("\n");

	if (main_dji_rev->UAVID[3] != DEVICE_TYPE_SELF)
		return;

	switch (main_dji_rev->command) {
	case ZK_COMMAND_NORMAL:
		break;
	case ZK_COMMAND_SPECIFIED:
		break;
	case ZK_COMMAND_COMMON:
		if (main_dji_rev->data[0] >= TN_MAX)
			return;
		respondflag = ptcol_fun[main_dji_rev->data[0]](main_dji_rev->data, msg_handlest.sendpacket_app.data, main_dji_rev->length, &msg_handlest.sendpacket_app.length);
		break;
	default: break;
	}

	if (respondflag) {
		memcpy(&msg_handlest.sendpacket_app, main_dji_rev, ZK_HEADER_LEN);
		msg_handlest.sendpacket_app.cmd = UAV_TO_APP;
		msg_handlest.sendpacket_app.end_code = _END_CODE;
		msg_handlest.datalen_sendapp = zkrt_final_encode(msg_handlest.data_send_app, &msg_handlest.sendpacket_app);
		sendToMobile(msg_handlest.data_send_app, msg_handlest.datalen_sendapp);
	}
}
/*
* @brief: mobile数组数据 转存到 zkrt_packet_t格式的全局结构体里
*/
void copydataformmobile(const u8* sdata, u8 sdatalen) {
//copy data to packet
	zkrt_data2_packet(sdata, sdatalen, main_dji_rev);
}
////////////////////////////////////////////////////////////////////////////common function
static char none_comnf(void *sdata, void *rdata, u8 slen, u8* rlen);
static char get_dev_info_comnf(void *sdata, void *rdata, u8 slen, u8* rlen);
static char jump_boot_mode_comnf(void *sdata, void *rdata, u8 slen, u8* rlen);
static char get_boot_info_comnf(void *sdata, void *rdata, u8 slen, u8* rlen);
static char fw_update_standby_comnf(void *sdata, void *rdata, u8 slen, u8* rlen);
static char fw_updating_comnf(void *sdata, void *rdata, u8 slen, u8* rlen);
static char fw_update_ok_comnf(void *sdata, void *rdata, u8 slen, u8* rlen);
static char fw_update_cancel_comnf(void *sdata, void *rdata, u8 slen, u8* rlen);
const PTCOL_FUN ptcol_fun[] = {
	none_comnf,
	none_comnf,
	get_dev_info_comnf,
	jump_boot_mode_comnf,
	get_boot_info_comnf,
	fw_update_standby_comnf,
	fw_updating_comnf,
	fw_update_ok_comnf,
	fw_update_cancel_comnf
};
//
static char none_comnf(void *sdata, void *rdata, u8 slen, u8* rlen) {
	return NOTNEED_RETRUN;
}
//
static char get_dev_info_comnf(void *sdata, void *rdata, u8 slen, u8* rlen) {
	return NOTNEED_RETRUN;
}
//跳转Bootloader
static char jump_boot_mode_comnf(void *sdata, void *rdata, u8 slen, u8* rlen) {
	u8 ret = TNS_OK;
	common_data_plst *common             = (common_data_plst*)sdata;
	rcommon_data_plst *rcommon             = (rcommon_data_plst*)rdata;
	common_jumpbootmode_plst *sother = (common_jumpbootmode_plst*)common->type_data;

	if (sother->mode == API_MODE) {
		if (iaphandle.api_enabled == API_VALID) {
			iaphandle.program_mode = API_MODE;
			user_flash_buffer.program_mode = API_MODE; //write flash
			userflash_write(USER_CONFIG_ADDRESS);
		} else {
			ret = TNS_FAIL;
		}
	}
	//respond packet
	rcommon->type_num = common->type_num;
	rcommon->status = ret;
	*rlen =  RES_HEADER_LEN;
	return NEED_RETRUN;
}
//获取Bootloader信息
static char get_boot_info_comnf(void *sdata, void *rdata, u8 slen, u8* rlen) {
	common_data_plst *common             = (common_data_plst*)sdata;
	rcommon_data_plst *rcommon             = (rcommon_data_plst*)rdata;
	common_rgetbootinfo_plst *rother = (common_rgetbootinfo_plst*)rcommon->other_data;

	rother->apienable = iaphandle.api_enabled;
	strcpy((char*)rother->version, DEV_SW_MB);
	//respond packet
	rcommon->type_num = common->type_num;
	rcommon->status = TNS_OK;
	*rlen =  RES_HEADER_LEN + sizeof(common_rgetbootinfo_plst);
	return NEED_RETRUN;
}
//准备固件升级
static char fw_update_standby_comnf(void *sdata, void *rdata, u8 slen, u8* rlen) {
	u8 ret;
	common_data_plst *common             = (common_data_plst*)sdata;
	rcommon_data_plst *rcommon             = (rcommon_data_plst*)rdata;
	common_fwupdatestart_plst *rother = (common_fwupdatestart_plst*)rcommon->other_data;
	ret = iap_start(rother->fw_size, rother->fw_name);
	//respond packet
	rcommon->type_num = common->type_num;
	rcommon->status = ret;
	*rlen =  RES_HEADER_LEN;
	return NEED_RETRUN;
}
//接收固件升级包
static char fw_updating_comnf(void *sdata, void *rdata, u8 slen, u8* rlen) {
	u8 ret;
	common_data_plst *common             = (common_data_plst*)sdata;
	rcommon_data_plst *rcommon             = (rcommon_data_plst*)rdata;
	common_fwupdating_plst *rother = (common_fwupdating_plst*)rcommon->other_data;
	ret = iap_updating(rother->seq, rother->packet);
	//respond packet
	rcommon->type_num = common->type_num;
	rcommon->status = ret;
	*rlen =  RES_HEADER_LEN;
	return NEED_RETRUN;
}
//固件升级包发送完成通知
static char fw_update_ok_comnf(void *sdata, void *rdata, u8 slen, u8* rlen) {
	u8 ret;
	common_data_plst *common             = (common_data_plst*)sdata;
	rcommon_data_plst *rcommon             = (rcommon_data_plst*)rdata;
	common_fwupdateok_plst *rother = (common_fwupdateok_plst*)rcommon->other_data;
	ret = iap_updated_ok(rother->last_seq);
	//respond packet
	rcommon->type_num = common->type_num;
	rcommon->status = ret;
	*rlen =  RES_HEADER_LEN;
	return NEED_RETRUN;
}
//取消固件升级
static char fw_update_cancel_comnf(void *sdata, void *rdata, u8 slen, u8* rlen) {
	u8 ret;
	common_data_plst *common             = (common_data_plst*)sdata;
	rcommon_data_plst *rcommon             = (rcommon_data_plst*)rdata;
	ret = iap_cancel();
	//respond packet
	rcommon->type_num = common->type_num;
	rcommon->status = ret;
	*rlen =  RES_HEADER_LEN;
	return NEED_RETRUN;
}
