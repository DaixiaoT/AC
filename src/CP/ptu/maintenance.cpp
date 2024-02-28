
#include "maintenance.h"

static unsigned long buf0[512];
char *pMainTaincePacket = (char *)buf0 + 1;
static MAINTENANCE_BUS s_bus = MAINTENANCE_UART; //ͨѶ��������
static void ProcessMaintainPacket(MAINTENANCE_BUS bus, char *p, int len)
{
    char type = p[3];
    switch (type)
    {
    case 0x10:      // ����
        Reply_DeviceIdentifyInfoPacket(bus);
        break;
    case 0x32:      // ����
        StoreDeviceIOInfo(p, len);
        BEEP(100);
        break;
    case 0x30:      //���ݳ��淢��
        BEEP(50);
        sleep(100);
        BEEP(50);
        s_DeviceStatusAutoRefresh = (p[4] >> 7) & 1;
        Reply_DeviceIOInfoPacket(bus);
        BEEP(50);
        break;
    case 0x40:      //����
        LOG_AC("Device Para Require\r\n");
        ReplyDeviceParaInfo(bus);
        BEEP(50);
        break;
    case 0x42:      //���� -> ����
        LOG_AC("Device Para Set\r\n");
        StoreDeviceParaInfo(p, len);
        BEEP(500);
        break;

    case 0x50:      // ����
        BEEP(50);
        if ((p[4] & 1) == 0)
        {           // ���ϼ�¼
            int idx = (int)Get32((unsigned char*)p + 5);
            Reply_ErrLogPacket(bus, (U8*)p, idx); // ���ݴ������
        }
        else
        {           // ���м�¼
            int idx = (int)Get32((unsigned char*)p + 5);
            U32 t = sys_time();
            Reply_RunLogPacket(bus, (U8*)p, idx);
            

        }
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

int maintenance_send(MAINTENANCE_BUS bus, U8 *buf, int len) // ��PTU��������
{
    int ret = -1;
    //LOG_PRINT("\n��������Ϊ:%d\n",bus);
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

void maintenance_uart_recv(int bus, U8 *buf, int len) // ����PTU����
{
    LOG_PRINT("PTU buf:%d,len:%d,s_bus:%d\n",buf,len,bus);
    s_bus = (MAINTENANCE_BUS)bus;
    ProcessMaintainPacket((MAINTENANCE_BUS)bus, (char *)buf, len);

}
//PTU 500ms�Զ�ˢ��
void maintenance_update()
{
	int len;
	static U32 CheckTime = 0;

	if (sys_time() - CheckTime < 1000)
		return;
	CheckTime = sys_time();

	if (s_DeviceStatusAutoRefresh)
	{
		Reply_DeviceIOInfoPacket(s_bus);
		
	}

}

void Init_Can_sema()
{
}

void Can_Pick_Data()
{
}
