/**
  ******************************************************************************
  * @file    appprotocol.h
  * @author  ZKRT
  * @version V1.0
  * @date    9-May-2017
  * @brief
    *                    + (1) init
  ******************************************************************************
  * @attention
  *
  * ...
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __APPPROTOCOL_H
#define __APPPROTOCOL_H
/* Includes ------------------------------------------------------------------*/
#include "zkrt.h"
/* Exported macro ------------------------------------------------------------*/
//respond flag
#define NEED_RETRUN                   1
#define NOTNEED_RETRUN                0

//version and model string //zkrt_notice
#define DEV_MODEL_MB              "OMU-BOOT"
#define DEV_HW_MB                 "000400"
#define DEV_SW_MB                 "000100"

//iap rel
#define IAP_PACKET_LEN                72

/* Exported constants --------------------------------------------------------*/
//////////////////////////////////////////////////////special define

//////////////////////////////////////////////////////
//#define TN_HEARTBEAT                  1
//type num enum
typedef enum {
    TN_None = 0,
    TN_HEARTBEAT,
    TN_GETDEVINFO,
    TN_JUMPBOOTMODE,
    TN_GETBOOTINFO,
    TN_FWUPDATESTART,
    TN_FWUPDATEING,
    TN_FWUPDATEOK,
    TN_FWUPDATECANCEL,
    TN_MAX
} TypeNumEnum;
//common respond status
typedef enum {
    TNS_OK = 0,
    TNS_FAIL,
    TNS_FAIL_UPDATE,
    TNS_FAIL_FWSIZE, //固件超过允许的大小
    TNS_FAIL_SEQ, //序列号出错，可能丢包
    TNS_FAIL_INVALID_FW,//无效的固件包,可能没有发送准备升级命令
    TNS_FAIL_FWWRITE //写失败
} CommonRespondState;
/* Exported types ------------------------------------------------------------*/
#pragma pack(1)
///////////////////////////////////////////////////////////common command
#define RES_HEADER_LEN 2 //type num + status
typedef struct {
    u8 type_num;
    u8 type_data[ZK_DATA_MAX_LEN - 1];
} common_data_plst;
typedef struct {
    u8 type_num;
    u8 status;
    u8 other_data[ZK_DATA_MAX_LEN - 2];
} rcommon_data_plst;
/******send******/
//jump_boot_mode_comnf
typedef struct {
    u8 mode;
} common_jumpbootmode_plst;
//get_boot_info_comnf -> none
//fw_update_standby_comnf
typedef struct {
    u32 fw_size;
    u8 fw_name[30];
} common_fwupdatestart_plst;
//fw_updating_comnf
typedef struct {
    u8 seq;
    u8 packet[IAP_PACKET_LEN];
} common_fwupdating_plst;
//fw_update_ok_comnf
typedef struct {
    u8 last_seq;
} common_fwupdateok_plst;
//fw_update_cancel_comnf -> none
/******respond******/
//get_boot_info_comnf
typedef struct {
    u8 status;
    u8 apienable;
    u8 version[30];
} common_rgetbootinfo_plst;
///////////////////////////////////////////////////////////specify command
///////////////////////////////
#pragma pack()
/* Exported functions ------------------------------------------------------- */

#endif /* __APPCAN_H */
/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT ZKRT *****END OF FILE****/

