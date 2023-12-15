#include "ptu_error_run_log.h"


#define min(a, b) (((a) < (b)) ? (a) : (b))
#define AC_ERR_RECORD_FLASH_SIZE (0x40000 + 4096)
#define AC_RUN_RECORD_FLASH_SIZE (6 * 1024 * 1024) // 6M
#define AC_RUN_RECORD_FLASH_ADDR (0x100000)		   // ���м�¼�����ַ1Mλ��
#define FLASH_BLOCK_SIZE 0x1000
#define FLASH_BLOCK_MASK (FLASH_BLOCK_SIZE - 1)
#define MAX_RUN_LOG_NUM ((AC_RUN_RECORD_FLASH_SIZE - FLASH_BLOCK_SIZE) / sizeof(AC_RUN_Record))
#define MAX_ERR_LOG_NUM ((AC_ERR_RECORD_FLASH_SIZE - FLASH_BLOCK_SIZE) / sizeof(AC_ERR_Record)) // 1024����¼


U32 g_down_log_tick = 0;
static U32 s_run_log_time = 0;
static U32 s_err_log_time = 0;
static U8 s_run_log_buf[2048];
int ac_err_record_num()
{
	return g_runInfo.err_log_num;
}
int ac_run_record_num()
{
	return g_runInfo.run_log_num;
}
// return 0, ok. -1, bad logIdx
int ac_err_read_record(int logIdx, AC_ERR_Record* f) //
{
	if (logIdx <= 0 || logIdx > g_runInfo.err_log_num)
		return -1; // E_SYS_LOG_IDX;

	U32 addr = g_runInfo.err_log_cur_addr + AC_ERR_RECORD_FLASH_SIZE - logIdx * sizeof(AC_ERR_Record);
	// addr &= SPI_FLASH_MASK;
	if (addr >= AC_ERR_RECORD_FLASH_SIZE)
		addr -= AC_ERR_RECORD_FLASH_SIZE;

	SpiFlashRead(addr, (U8*)f, sizeof(AC_ERR_Record));
	return 0;
}

int ac_run_read_record(int logIdx, AC_RUN_Record* f)
{
	if (logIdx <= 0 || logIdx > g_runInfo.run_log_num)
		return -1; // E_SYS_LOG_IDX;

	U32 addr = g_runInfo.run_log_cur_addr + AC_RUN_RECORD_FLASH_SIZE - logIdx * sizeof(AC_RUN_Record);
	// addr &= SPI_FLASH_MASK;
	if (addr >= AC_RUN_RECORD_FLASH_SIZE)
		addr -= AC_RUN_RECORD_FLASH_SIZE;

	addr += AC_RUN_RECORD_FLASH_ADDR;

	SpiFlashRead(addr, (U8*)f, sizeof(AC_RUN_Record));

	s_run_log_time = sys_time(); // ��ȡ���ϼ�¼ʱ�����¼�ʱ1���ӣ��Է�ֹ����ʱ��¼�����仯
	return 0;
}

void Reply_ErrLogPacket(MAINTENANCE_BUS bus, U8* p, int idx) //���ع��ϼ�¼
{
	unsigned char* buf = (unsigned char*)pMainTaincePacket;
	static AC_ERR_Record s_log;
	const int Tlen = 64;
	int i;

	//LOG_MAINTENANCEW_DEBUG("@@@@@@@@@@ Reply_ErrLogPacket\r\n");
	mem_set(buf, 0, Tlen);

	buf[0] = 0xf5;
	Set16(buf + 1, Tlen);
	buf[3] = 0x51; // 0x51

	Set32(buf + 5, ac_err_record_num()); // ��������
	idx = min(ac_err_record_num(), idx);

	Set32(buf + 9, idx);

	g_down_log_tick = sys_time();

	if (ac_err_read_record(idx, &s_log) == 0)
	{
		mem_copy(buf + 13, s_log.rtc, 6);
		mem_copy(buf + 21, (U8*)&s_log.u1_err, sizeof(ERR_Record_DATA)); //
	}

	buf[Tlen - 1] = GetParity(buf, Tlen - 1);

	maintenance_send(bus, buf, Tlen);
}
static inline void get_rtc(U8* buf)
{
	Time_t t;
	RTC_GetTime(&t);
	buf[0] = t.year;
	buf[1] = t.month;
	buf[2] = t.day;
	buf[3] = t.hour;
	buf[4] = t.minute;
	buf[5] = t.second;
}

static BOOL StoreRunInfo(AC_RUN_INFO* p)
{
	get_rtc(p->rtc);
	p->crc = CRC16((U8*)p, sizeof(AC_RUN_INFO) - 2);

#ifdef _SPI_FRAM_
	SpiFramWrite(AC_RUN_INFO_ADDR, (U8*)p, sizeof(AC_RUN_INFO));
#endif // _SPI_FRAM_
	return TRUE;
}



AC_RUN_INFO g_runInfo;

// ��FLASH��д��һ����¼
int ac_run_log_record(AC_RUN_Record* f)
{
	f->mark = AC_RUN_RECORD_MARK;//���ü�¼�еı��

	get_rtc(f->rtc);//��ȡʱ����Ϣ
	f->crc = CRC16((U8*)f, sizeof(AC_RUN_Record) - 2);

	LOG_PRINT("run log record: num=%d addr=0x%x\r\n",
		g_runInfo.run_log_num, g_runInfo.run_log_cur_addr);
	LOG_PRINT("year=%x mon=%x day=%x hour=%x min=%x second=%x\r\n",
		f->rtc[0], f->rtc[1], f->rtc[2],
		f->rtc[3], f->rtc[4], f->rtc[5]);

	//����Flash�洢��ַ
	U32 addr = g_runInfo.run_log_cur_addr + AC_RUN_RECORD_FLASH_ADDR;

	//����Ƿ���Ҫ����Flash�洢�������
	if ((addr & FLASH_BLOCK_MASK) == 0)
	{
		SpiFlashErase4K(addr);
	}

	//����¼д��Flash�洢��
	SpiFlashWrite(addr, (U8*)f, sizeof(AC_RUN_Record));

	//����������־��ַ
	addr = g_runInfo.run_log_cur_addr + sizeof(AC_RUN_Record);
	if (addr >= AC_RUN_RECORD_FLASH_SIZE)
		addr = 0;

	//����������־��
	if (g_runInfo.run_log_num < MAX_RUN_LOG_NUM)
		g_runInfo.run_log_num++;
	g_runInfo.run_log_cur_addr = addr;

	//�洢������Ϣ������ʧ�Դ洢��
	StoreRunInfo(&g_runInfo); // ���磬����

#if 0
	static AC_Record log;
	ac_read_record(1, &log);
	memdump((U8*)&log, sizeof(AC_Record));
#endif

	// BEEP(50);
	return 0;
}





void save_run_log()
{
	static AC_RUN_Record s_log;

	if (TimeGap(s_run_log_time) < (5 * 1000))//ȷ����־����ÿ��5�뱣��һ��
		return;
	s_run_log_time = sys_time();

	//��ȡ�豸IO��Ϣ����������� s_run_log_buf������¼���ݵĳ����ڱ��� len �С�
	int len = GetDeviceIOInfoPacket(s_run_log_buf, 2048);

	if (len <= 0)
		return;

	//�� s_run_log_buf + 5 �������ݸ��Ƶ� s_log.data ��
	mem_copy(s_log.data, s_run_log_buf + 5, RUN_DATA_LEN);

	s_log.data_type = 0; // ���м�¼

	ac_run_log_record(&s_log);
}

void Reply_RunLogPacket(MAINTENANCE_BUS bus, U8* p, int idx) //�������м�¼
{
	unsigned char* buf = (unsigned char*)pMainTaincePacket;

	int i;
	const int Tlen = RUN_DATA_LEN + 1 + 21 + 1;

	//LOG_MAINTENANCEW_DEBUG("@@@@@@@@@@ Reply_RunLogPacket\r\n");
	mem_set(buf, 0, Tlen);
	WDEX_OFF(); // �ص����Ź�
	buf[0] = 0xf5;
	Set16(buf + 1, Tlen);
	buf[3] = 0x51;

	g_down_log_tick = sys_time();
#if 1
	static AC_RUN_Record s_log;

	Set32(buf + 5, ac_run_record_num());
	idx = min(ac_run_record_num(), idx);
	Set32(buf + 9, idx);

	if (ac_run_read_record(idx, &s_log) == 0)
	{
		mem_copy(buf + 13, s_log.rtc, 6);

		mem_copy(buf + 21, s_log.data, RUN_DATA_LEN);
		buf[21 + RUN_DATA_LEN] = s_log.data_type;
	}
#endif // _DEBUG

	buf[Tlen - 1] = GetParity(buf, Tlen - 1);
	maintenance_send(bus, buf, Tlen);
}

//��¼������־�ĺ���
void ac_err_log_record(AC_ERR_Record* f)
{
	//���ʱ�����Ƿ�С��10s
	if (TimeGap(s_err_log_time) < (10 * 1000))
		return ;

	s_err_log_time = sys_time();//����ʱ��Ϊ��ǰϵͳ����

	//��ȡʵʱʱ����Ϣ
	get_rtc(f->rtc);

	LOG_PRINT("log record: num=%d addr=0x%x\r\n",
		g_runInfo.err_log_num, g_runInfo.err_log_cur_addr);
	LOG_PRINT("year=%x mon=%x day=%x hour=%x min=%x second=%x\r\n",
		f->rtc[0], f->rtc[1], f->rtc[2],
		f->rtc[3], f->rtc[4], f->rtc[5]);

	//��ȡ��ǰ��Flash�洢��ַ
	U32 addr = g_runInfo.err_log_cur_addr;

	//�����ǰ��ַ��Flash�����ʼ��ַ��������ÿ�
	if ((addr & FLASH_BLOCK_MASK) == 0)
	{
		SpiFlashErase4K(addr);
	}
	//������д��Flash�洢��
	SpiFlashWrite(addr, (U8*)f, sizeof(AC_ERR_Record));

	//���µ�ַ�����������Χ�����ͷ��ʼ
	addr += sizeof(AC_ERR_Record);
	if (addr >= AC_ERR_RECORD_FLASH_SIZE)
		addr = 0;

	//���������־����С��������ƣ�����������
	if (g_runInfo.err_log_num < MAX_ERR_LOG_NUM)
		g_runInfo.err_log_num++;

	//���´�����־��ǰ��ַ
	g_runInfo.err_log_cur_addr = addr;

	StoreRunInfo(&g_runInfo);

#if 0
	static AC_Record log;
	ac_read_record(1, &log);
	memdump((U8*)&log, sizeof(AC_Record));
#endif

	return;
}








