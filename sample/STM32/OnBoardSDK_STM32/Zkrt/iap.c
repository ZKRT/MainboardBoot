/**
  ******************************************************************************
  * @file    iap.c
  * @author  ZKRT
  * @version V1.0
  * @date    2018-05-29
  * @brief
  ******************************************************************************
  * @attention
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "iap.h"
#include "flash_if.h"
#include "common.h"
#include "appprotocol.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
//#define MANU_TEST_IAP
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
//iap relevant
typedef  void (*pFunction)(void);
pFunction Jump_To_Application;
uint32_t JumpAddress;
__IO uint32_t FlashProtection = 0;

iap_handle_st iaphandle;
/* Private function prototypes -----------------------------------------------*/
void manu_iap_test(void);
/**
  * @brief
  * @param  None
  * @retval None
  */
void iap_init(void) {
    //read flash
    userflash_read(USER_CONFIG_ADDRESS);
    if ((user_flash_buffer._start_cod == 0XFEDCBA98) && (user_flash_buffer._end_cod == 0X76543210)) {
        iaphandle.api_enabled = user_flash_buffer.api_enabled;
        iaphandle.program_mode = user_flash_buffer.program_mode;
    } else {
        //用户数据未初始化，默认进入API模式，让API程序执行用户数据初始化
        printf("************************[Mboot]User FLash Uninited.\r\n");
        iaphandle.api_enabled = API_VALID;
        iaphandle.program_mode = API_MODE;
    }
    //chech jump api mode
    if ((iaphandle.api_enabled == API_VALID) && (iaphandle.program_mode == API_MODE)) {
        iap_jump2appliacation();
    }
}
/**
  * @brief
  * @param  None
  * @retval None
  */
void iap_process(void) {

#ifdef MANU_TEST_IAP
    manu_iap_test();
#endif
    //chech reset
    if (iaphandle.need_reset_flag) {
        printf("************************[Mboot]Reboot.\r\n");
        __set_FAULTMASK(1);
        NVIC_SystemReset();
    }
    //chech jump api mode
    if ((iaphandle.api_enabled == API_VALID) && (iaphandle.program_mode == API_MODE)) {
        iap_jump2appliacation();
    }
}
/**
  * @brief  跳转程序
  * @param  None
  * @retval None
  */
uint8_t iap_jump2appliacation(void) {
    printf("************************[Mboot]iap_jump2appliacation.\r\n");
//  __disable_irq(); //无效果，屏蔽。
    USART_ITConfig(USART1, USART_IT_RXNE, DISABLE); //关闭串口1中断，因为串口1一直在接收数据，试过关闭全局总中断，没有效果。

    JumpAddress = *(__IO uint32_t*) (APPLICATION_ADDRESS + 4);
    /* Jump to user application */
    Jump_To_Application = (pFunction) JumpAddress;
    /* Initialize user application's Stack Pointer */
    __set_MSP(*(__IO uint32_t*) APPLICATION_ADDRESS);
    Jump_To_Application();
    return 1;
}
/**
  * @brief
  * @param  None
  * @retval None
  */
void manu_iap_test(void) {
    uint8_t key = 0;
    /* Test if any sector of Flash memory where user application will be loaded is write protected */
    if (FLASH_If_GetWriteProtectionStatus() == 0) {
        FlashProtection = 1;
    } else {
        FlashProtection = 0;
    }

    while (1) {
        printf("\r\n================== Main Menu ============================\r\n\n");
        printf("  Download Image To the STM32F4xx Internal Flash ------- 1\r\n\n");
        printf("  Upload Image From the STM32F4xx Internal Flash ------- 2\r\n\n");
        printf("  Execute The New Program ------------------------------ 3\r\n\n");

        if (FlashProtection != 0) {
            printf("  Disable the write protection ------------------------- 4\r\n\n");
        }

        printf("==========================================================\r\n\n");

        /* Receive key */
        key = GetKeyV2();

        if (key == 0x31) {
            /* Download user application in the Flash */
//      SerialDownload();
        } else if (key == 0x32) {
            /* Upload user application from the Flash */
//      SerialUpload();
        } else if (key == 0x33) { /* execute the new program */
            printf("jump to application.\n");
            iap_jump2appliacation();
        } else if ((key == 0x34) && (FlashProtection == 1)) {
            /* Disable the write protection */
            switch (FLASH_If_DisableWriteProtection()) {
            case 1: {
                printf("Write Protection disabled...\r\n");
                FlashProtection = 0;
                break;
            }
            case 2: {
                printf("Error: Flash write unprotection failed...\r\n");
                break;
            }
            default: {
            }
            }
        } else {
            if (FlashProtection == 0) {
                printf("Invalid Number ! ==> The number should be either 1, 2 or 3\r");
            } else {
                printf("Invalid Number ! ==> The number should be either 1, 2, 3 or 4\r");
            }
        }
    }
}
/**
  * @brief
  * @param  None
  * @retval None
  */
u8 iap_start(u32 newfwsize, u8* newfwname) {
    /* Test the size of the image to be sent */
    /* Image size is greater than Flash size */
    if (newfwsize > USER_FLASH_SIZE) {
        return TNS_FAIL_FWSIZE;
    }
    /* erase user application area */
    FLASH_If_Erase(APPLICATION_ADDRESS);

    iaphandle.fw_total_size = newfwsize;
    memcpy(iaphandle.fw_name, newfwname, sizeof(iaphandle.fw_name));
    iaphandle.api_enabled = API_INVALID;
    user_flash_buffer.api_enabled = API_INVALID; //write flash
    userflash_write(USER_CONFIG_ADDRESS);

    iaphandle.packet_seq = 0xFF;  //mean standby pack seq
    iaphandle.fw_recved_size = 0;
    iaphandle.flashdestination = APPLICATION_ADDRESS;
    return TNS_OK;
}
/**
  * @brief
  * @param  None
  * @retval None
  */
u8 iap_updating(u8 seq, u8* packet) {
    uint32_t ramsource;
    /**
    原则：
        ①处理下一个序列包；
        ②处理重复的上个序列包，避免ACK丢失问题；
    **/
    //check update valid
    if (iaphandle.fw_total_size == 0)
        return TNS_FAIL_INVALID_FW;

    //check seq
    if (seq != (iaphandle.packet_seq + 1)) { //packet seq continuous check
        if (seq == iaphandle.packet_seq) { //上个序列包，返回成功, 不做任何操作
            return TNS_OK;
        } else
            return TNS_FAIL_SEQ;
    }
    //update
    ramsource = (uint32_t)packet;

    /* Write received data in Flash */
    if (FLASH_If_Write(&iaphandle.flashdestination, (uint32_t*) ramsource, (uint16_t) IAP_PACKET_LEN / 4)  == 0) {
        iaphandle.packet_seq++;
        iaphandle.fw_recved_size += IAP_PACKET_LEN;
        return TNS_OK;
    } else { /* An error occurred while writing to Flash memory */
        //关闭升级流程
        iap_cancel();
        /* End session */
        return TNS_FAIL_FWWRITE;
    }
}
/**
  * @brief
  * @param  None
  * @retval None
  */
u8 iap_updated_ok(u8 last_seq) {
    if ((last_seq == iaphandle.packet_seq) &&
            (iaphandle.fw_recved_size >= iaphandle.fw_total_size) &&
            ((iaphandle.fw_recved_size - IAP_PACKET_LEN) < iaphandle.fw_total_size)
       ) {
        iaphandle.api_enabled = API_VALID;
        iaphandle.program_mode = API_MODE;
        //write flash
        user_flash_buffer.api_enabled = API_VALID;
        user_flash_buffer.program_mode = API_MODE;
        userflash_write(USER_CONFIG_ADDRESS);
        return TNS_OK;
    }
    return TNS_FAIL_UPDATE;
}
/**
  * @brief
  * @param  None
  * @retval None
  */
u8 iap_cancel(void) {
    iaphandle.fw_total_size = 0;
    iaphandle.packet_seq = 0xFF;
    iaphandle.fw_recved_size = 0;
    iaphandle.flashdestination = APPLICATION_ADDRESS;
    return TNS_OK;
}
