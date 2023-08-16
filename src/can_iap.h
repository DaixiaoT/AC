#ifndef _CAN_IAP_H_
#define _CAN_IAP_H_

#include "iap_base.h"

/******************�����APP IAP��Ϣ����Ҫ�޸�*******************/
//id=10010500 version=20200819 mac=[00 ]
#ifdef _AC_BASE_

#define IAP_ID (IAP_CHIP_STM32F407 | IAP_SYSTEM_AC | IAP_HVAC_UAE_CAB | IAP_HVAC_DEV_CPU)
#define IAP_VERSION 20211103     



#endif

#define APP_ADDR     0x08010000
#define APP_OK_MARK  0x66778899  

/************************************************************/

// �յ�������ά����
#define HVAC_PACKET_START_CODE		0xF5
#define HVAC_PACKET_END_CODE		0xF6
// HVAC���ڴ����з�װ�ĸ�ʽ��0xF5 + len(2B) + data + CRC(2B) + 0xF6
// CRC = len+data��CRC
// len = data_len + 6
// MSB��ʽ

#define HVAC_MAX_PACKET_LEN 1024

/************************************************************/

#ifdef __cplusplus
void iap_set_buf(U8 *buf);
void iap_set_mac(int bus, U8 mac);

//int iap_uart0_send(U32 id, U8 *buf, int len);

#define CHECK_IAP_USB 1
#define CHECK_IAP_CAN 2
#define CHECK_IAP_UDP 4

void CheckIAPBus(U32 flag);

static inline void LoopCheckIAPBus(U32 flag)
{
    while(1) {
        CheckIAPBus(flag);
        sleep(2);
    }
}

int hvac_ptu_send(int bus, U8 *buf, int len);

// ���ش��� ��CAN ���ߴ�����֡
void iap_process_up_bus(int bus, U32 iap_id, U8 *buf, int len);

// ���ش��� �����ϵ�IAP֡, bufǰ�������4�ֽڡ�
void iap_process_sub_bus(int bus, U32 iap_id, U8 *buf, int len);

// �豸������IAP֡
void dev_iap_process(int bus, U32 iap_id, U8 *buf, int len);

#endif // __cplusplus

#endif //_CAN_IAP_H_

