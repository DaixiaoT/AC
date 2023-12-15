#include "ptu_error_run_log.h"


#define min(a, b) (((a) < (b)) ? (a) : (b))
#define AC_ERR_RECORD_FLASH_SIZE (0x40000 + 4096)
#define AC_RUN_RECORD_FLASH_SIZE (6 * 1024 * 1024) // 6M
#define AC_RUN_RECORD_FLASH_ADDR (0x100000)		   // 运行记录保存地址1M位置
#define FLASH_BLOCK_SIZE 0x1000
#define FLASH_BLOCK_MASK (FLASH_BLOCK_SIZE - 1)
#define MAX_RUN_LOG_NUM ((AC_RUN_RECORD_FLASH_SIZE - FLASH_BLOCK_SIZE) / sizeof(AC_RUN_Record))
#define MAX_ERR_LOG_NUM ((AC_ERR_RECORD_FLASH_SIZE - FLASH_BLOCK_SIZE) / sizeof(AC_ERR_Record)) // 1024条记录


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

	s_run_log_time = sys_time(); // 读取故障记录时，重新计时1分钟，以防止下载时记录数量变化
	return 0;
}

void Reply_ErrLogPacket(MAINTENANCE_BUS bus, U8* p, int idx) //下载故障记录
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

	Set32(buf + 5, ac_err_record_num()); // 故障数量
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

// 往FLASH中写入一条记录
int ac_run_log_record(AC_RUN_Record* f)
{
	f->mark = AC_RUN_RECORD_MARK;//设置记录中的标记

	get_rtc(f->rtc);//获取时钟信息
	f->crc = CRC16((U8*)f, sizeof(AC_RUN_Record) - 2);

	LOG_PRINT("run log record: num=%d addr=0x%x\r\n",
		g_runInfo.run_log_num, g_runInfo.run_log_cur_addr);
	LOG_PRINT("year=%x mon=%x day=%x hour=%x min=%x second=%x\r\n",
		f->rtc[0], f->rtc[1], f->rtc[2],
		f->rtc[3], f->rtc[4], f->rtc[5]);

	//计算Flash存储地址
	U32 addr = g_runInfo.run_log_cur_addr + AC_RUN_RECORD_FLASH_ADDR;

	//检查是否需要进行Flash存储器块擦除
	if ((addr & FLASH_BLOCK_MASK) == 0)
	{
		SpiFlashErase4K(addr);
	}

	//将记录写入Flash存储器
	SpiFlashWrite(addr, (U8*)f, sizeof(AC_RUN_Record));

	//更新运行日志地址
	addr = g_runInfo.run_log_cur_addr + sizeof(AC_RUN_Record);
	if (addr >= AC_RUN_RECORD_FLASH_SIZE)
		addr = 0;

	//更新运行日志号
	if (g_runInfo.run_log_num < MAX_RUN_LOG_NUM)
		g_runInfo.run_log_num++;
	g_runInfo.run_log_cur_addr = addr;

	//存储运行信息到非易失性存储器
	StoreRunInfo(&g_runInfo); // 铁电，参数

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

	if (TimeGap(s_run_log_time) < (5 * 1000))//确保日志至少每隔5秒保存一次
		return;
	s_run_log_time = sys_time();

	//获取设备IO信息包并将其存入 s_run_log_buf，并记录数据的长度在变量 len 中。
	int len = GetDeviceIOInfoPacket(s_run_log_buf, 2048);

	if (len <= 0)
		return;

	//从 s_run_log_buf + 5 处将数据复制到 s_log.data 中
	mem_copy(s_log.data, s_run_log_buf + 5, RUN_DATA_LEN);

	s_log.data_type = 0; // 运行记录

	ac_run_log_record(&s_log);
}

void Reply_RunLogPacket(MAINTENANCE_BUS bus, U8* p, int idx) //下载运行记录
{
	unsigned char* buf = (unsigned char*)pMainTaincePacket;

	int i;
	const int Tlen = RUN_DATA_LEN + 1 + 21 + 1;

	//LOG_MAINTENANCEW_DEBUG("@@@@@@@@@@ Reply_RunLogPacket\r\n");
	mem_set(buf, 0, Tlen);
	WDEX_OFF(); // 关掉看门狗
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

//记录错误日志的函数
void ac_err_log_record(AC_ERR_Record* f)
{
	//检查时间间隔是否小于10s
	if (TimeGap(s_err_log_time) < (10 * 1000))
		return ;

	s_err_log_time = sys_time();//更新时间为当前系统函数

	//获取实时时钟信息
	get_rtc(f->rtc);

	LOG_PRINT("log record: num=%d addr=0x%x\r\n",
		g_runInfo.err_log_num, g_runInfo.err_log_cur_addr);
	LOG_PRINT("year=%x mon=%x day=%x hour=%x min=%x second=%x\r\n",
		f->rtc[0], f->rtc[1], f->rtc[2],
		f->rtc[3], f->rtc[4], f->rtc[5]);

	//获取当前的Flash存储地址
	U32 addr = g_runInfo.err_log_cur_addr;

	//如果当前地址是Flash块的起始地址，则擦除该块
	if ((addr & FLASH_BLOCK_MASK) == 0)
	{
		SpiFlashErase4K(addr);
	}
	//将错误写入Flash存储器
	SpiFlashWrite(addr, (U8*)f, sizeof(AC_ERR_Record));

	//更新地址，如果超出范围，则从头开始
	addr += sizeof(AC_ERR_Record);
	if (addr >= AC_ERR_RECORD_FLASH_SIZE)
		addr = 0;

	//如果故障日志数量小于最大限制，则增加数量
	if (g_runInfo.err_log_num < MAX_ERR_LOG_NUM)
		g_runInfo.err_log_num++;

	//更新错误日志当前地址
	g_runInfo.err_log_cur_addr = addr;

	StoreRunInfo(&g_runInfo);

#if 0
	static AC_Record log;
	ac_read_record(1, &log);
	memdump((U8*)&log, sizeof(AC_Record));
#endif

	return;
}








