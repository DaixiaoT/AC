
#include "maintenance.h"
#include "ptu_monitor.h"
static unsigned long buf0[512];
char *pMainTaincePacket = (char *)buf0 + 1;

static void ProcessMaintainPacket(MAINTENANCE_BUS bus, char *p, int len)
{
    char type = p[3];
    switch (type)
    {
    case 0x10: // 连接
        Reply_DeviceIdentifyInfoPacket(bus);
        break;
    case 0x32: // 更新
        StoreDeviceIOInfo(p, len);
        BEEP(100);
        sleep(200);
        BEEP(100);
        break;
    case 0x30:
        s_DeviceStatusAutoRefresh = (p[4] >> 7) & 1;
        Reply_DeviceIOInfoPacket(bus);
        break;
    }
}

unsigned char GetParity(unsigned char *p, int len)
{
    unsigned char sum = 0;
    int i;
    for (i = 0; i < len; i++)
        sum += p[i];
    return sum;
}

int maintenance_send(MAINTENANCE_BUS bus, U8 *buf, int len) // 给PTU发送数据
{
    int ret = -1;

    if (bus == MAINTENANCE_NET)
    {
        if ((ret = g_ptu_udp.send_udp_data(buf, len)) < 0)
            PRINT_ERR_INT(ret);

        ret = len;
    }
    else
    {
        Uart0_Write((char *)buf, len);
        //	BEEP(50);

        ret = len;
    }

    return ret;
}

void maintenance_uart_recv(int bus, U8 *buf, int len) // 接收PTU数据
{
    LOG_PRINT("PTU buf:%d,len:%d\n",buf,len);
    ProcessMaintainPacket((MAINTENANCE_BUS)bus, (char *)buf, len);
}

void Init_Can_sema()
{
}

void Can_Pick_Data()
{
}
