#include "ADIO.h"
#include "ads1113.h"
#include "Ds2401CRC.h"
#include "Ds2401.h"
#include "RTC_ISL1208.h"
#include "valve_ctrl.h"
#include "PCA8574.h"
#include "ad.h"
#include "MCP4728.h"
#include "trdp_app.h"
#include "trdp_udp.h"
#include "maintenance.h"

#define TEST_PRINT LOG_PRINT
//#define TEST_PRINT USB_LOG_PRINT

#define TEST_BUF_SIZE 256
static U8 s_test_buf[TEST_BUF_SIZE + TEST_BUF_SIZE + 32];

static U32 rx_mvb_sn_tick = 0;
static U8 rx_mvb_sn_buf[12] = { 0,0,0,0, 0,0,0,0, 0,0,0,0 };

#pragma  pack (push,1)
typedef struct
{
	U32	m_dio_flag : 1;//1：测试    0：不测试
	U32 m_do_all_out_flag : 1;
	U32	m_ai_flag : 1;
	U32	m_flash_all_flag : 1;
	U32	m_flash_fast_flag : 1;
	U32	m_fram_flag : 1;
	U32	m_can_flag : 1;
	U32	m_adc_flag : 1;
	U32	m_rtc_flag : 1;
	U32	m_rs485_flag : 1;//
	U32	m_eth_flag : 1;//
	U32	m_beep_flag : 1;//蜂鸣器
	U32	m_addr_flag : 1;//设备地址测试
	U32	m_led_flag : 1;//灯板
	U32	m_label_flag : 1;//标签贴上
	U32	m_locking_flag : 1;//防松标识
	U32 m_res_flag : 16;

	U16	m_dio_results : 1;//1：OK   0:FAIL
	U16	m_ai_results : 1;
	U16	m_flash_all_results : 1;
	U16	m_flash_fast_results : 1;
	U16	m_fram_results : 1;
	U16	m_rs485_results : 1;
	U16	m_can_results : 1;
	U16	m_adc_results : 1;
	U16	m_rtc_results : 1;
	U16	m_beep_results : 1;//蜂鸣器
	U16	m_addr_results : 1;//设备地址测试
	U16	m_led_results : 1;//灯板
	U16	m_label_results : 1;//标签贴上
	U16	m_locking_results : 1;//防松标识
	U16 m_res_results : 2;

	U8 do_fail_no[48];//fail 路数  48  和下位机软件保持一样
	U8 di_fail_no[76];//fail 路数 64  和下位机软件保持一样

	U8 ai_fail_no[40];//fail 路数
	U16 ai_data[40];//AI数值
	U8 m_fail_no;//错误码

	short m_adc_VBat;

	U8 trdp_info1[18];
	U8 trdp_info2[18];
	U8 trdp_dev_op_info[20];

	Time_t date;
}TestInfo_t;
#pragma  pack (pop)

static BOOL SpiFramTest2()
{
	TEST_PRINT("========SpiFramTest==========\r\n");
	sleep(50);

	//SpiFramInit();
	//TEST_PRINT("SpiFramInit ok\r\n");

	//SPI_FRAM_WRITE_ENABLE();
	//SPI_FRAM_WRITE_PROTECT();
	//sleep(50);
	//TEST_PRINT("SR=%02x\r\n", RDSR());

	U32 flash_addr = 0;
	int k = 0;
	while (flash_addr < SPI_FRAM_SIZE) {
		//TEST_PRINT("flash_addr=%x\r\n", flash_addr);
		//sleep(20);
		//WDEX_FEED();
		for (int i = 0; i < TEST_BUF_SIZE; i++)
			s_test_buf[i] = 0xaa - (U8)i;
		SpiFramWrite(flash_addr, s_test_buf, TEST_BUF_SIZE);
		TEST_PRINT("w");

		mem_set(s_test_buf, 0, sizeof(s_test_buf));
		SpiFramRead(flash_addr, s_test_buf, TEST_BUF_SIZE);
		TEST_PRINT("r");
		//   memdump(s_test_buf, sizeof(s_test_buf));
		for (int i = 0; i < TEST_BUF_SIZE; i++) {
			if (s_test_buf[i] != (U8)(0xaa - (U8)i)) {
				TEST_PRINT("err at [%d] v=%02x\r\n", i, s_test_buf[i]);
				return FALSE;
			}
		}
		flash_addr += TEST_BUF_SIZE;
		k++;
		if ((k & 0x1F) == 0)
			TEST_PRINT("\r\n");
	}

	TEST_PRINT("SpiFramTest success.\r\n");
	return TRUE;
}

static BOOL SpiFramTest()
{
	int n = 2;
	while (n--) {
		if (!SpiFramTest2())
			return FALSE;
	}

	return TRUE;
}

static BOOL check_abort()
{
	char c;
	int ret = Uart0_TryRead(&c, 1);
	if (ret == 1 && (c == '\r' || c == '\n')) {
		LOG_PRINT("\r\nabort testing...\r\n");
		while (Uart0_TryRead(&c, 1) == 1);
		return TRUE;
	}
	return FALSE;
}

static BOOL SpiFlashTest(int test_len)
{
	TEST_PRINT("========SpiFlashTest==========\r\n");
	//	sleep(50);

	SpiFlashInit();

	SpiFlashEraseFull();

	U32 flash_addr = 0;
	int k = 0;
	while (flash_addr < (test_len)) {
		//TEST_PRINT("flash_addr=%x\r\n", flash_addr);
		//sleep(20);
		//WDEX_FEED();
		for (int i = 0; i < TEST_BUF_SIZE; i++)
			s_test_buf[i] = 0xaa - (U8)i;
		SpiFlashWrite(flash_addr, s_test_buf, TEST_BUF_SIZE);
		//TEST_PRINT("w");

		mem_set(s_test_buf, 0, sizeof(s_test_buf));
		SpiFlashRead(flash_addr, s_test_buf, TEST_BUF_SIZE);
		//TEST_PRINT("r");
		//   memdump(s_test_buf, sizeof(s_test_buf));
		for (int i = 0; i < TEST_BUF_SIZE; i++) {
			if (s_test_buf[i] != (U8)(0xaa - (U8)i)) {
				TEST_PRINT("err at [%d] v=%02x\r\n", i, s_test_buf[i]);
				return FALSE;
			}
		}
		flash_addr += TEST_BUF_SIZE; //*16;
									 //TEST_PRINT("\r progress %d%%, addr=%d", (flash_addr*100)/test_len, flash_addr);
		if ((flash_addr & 0x7FFF) == 0) {
			TEST_PRINT("%d", (flash_addr * 100) / test_len);
			k++;
			if ((k & 0x1F) == 0)
				TEST_PRINT("\r\n");
		}
		if (check_abort())
			break;
	}

	TEST_PRINT("\r\nSpiFlashTest success.\r\n");
	return TRUE;
}

/*
DI: 4个端子，每个19路，最后一个端子7路。DI每个端子20个口，最后一口是公共端。
DO: 4个端子，每个10路，最右边的是公共端，是本端子所有DO的电源端。DO=1，则该路DO与公共端短接，输出1

以下下标都从1开始：
DO 1~19, 对应DI 1~19, 20~38
DO 20,   对DO第2/3端子供电
DO 21~39，对应DI 39~57, 58~64
DO 40,   对DO第4端子供电
*/
#define DIO_DELAY 200

static BOOL DIO_test_1(int do_pos, int di_off, TestInfo_t* info)
{
	BOOL ret = TRUE;
	int i, j;
	for (i = do_pos; i < do_pos + 19; i++) {
		DO_SET(i);
		DO_test_update();

		sleep(DIO_DELAY);
		DI_test_update();

		for (j = 0; j < DI_NUM; j++) {
			if (j == (i + di_off) || j == (i + di_off + 19)) {
				if (DI_STAT(j) == 0) {
					TEST_PRINT("\r\nstep 2. DI(%d) err. should be 1, DO(%d)\r\n", j + 1, i + 1);
					TEST_PRINT("\r\n [DI%d] %d,[DI%d] %d\r\n", i + di_off + 1, DI_STAT(i + di_off), i + di_off + 19 + 1, DI_STAT(i + di_off + 19));

					if ((DI_STAT(i + di_off) == 0) && (DI_STAT(i + di_off + 19) == 0)) {
						//DO出错概率大
						info->do_fail_no[i] = 1;
					}
					else if (DI_STAT(i + di_off) == 0) {
						info->di_fail_no[i + di_off] = 1;
					}
					else if (DI_STAT(i + di_off + 19) == 0) {
						info->di_fail_no[i + di_off + 19] = 1;
					}

					ret = FALSE;
				}
			}
			else {
				if (DI_STAT(j)) {
					TEST_PRINT("\r\nstep 2. DI(%d) err. should be 0 DO(%d)\r\n", j + 1, i + 1);
					info->di_fail_no[j] = 1;
					ret = FALSE;
				}
			}
		}

		DO_CLR(i);
		TEST_PRINT(".");

		if (check_abort())
			break;
	}

	return ret;
}

#define CLOSE 0
#define OPEN 1
void out_di_110V(BOOL s)//DI加上24V或110V
{
	gpioSetValue(GPIOE, 11, s);
	gpioSetValue(GPIOE, 12, s);
	gpioSetValue(GPIOE, 3, s);
	gpioSetValue(GPIOE, 4, s);
}


void out_di_dtc(BOOL s)//分时采集
{
	gpioSetValue(GPIOE, 13, s);
	gpioSetValue(GPIOE, 15, s);
	gpioSetValue(GPIOG, 6, s);
	gpioSetValue(GPIOG, 7, s);
}

void di_test(TestInfo_t* info);
static BOOL DIO_test2(TestInfo_t* info)
{
	BOOL ret = TRUE;
	TEST_PRINT("========DIO test==========\r\n");
	sleep(50);

	int i, j;
	//out_di_dtc(CLOSE);
	out_di_110V(OPEN);
	// 1. clear all DO
	for (i = 0; i < DO_NUM; i++) {
		DO_CLR(i);
		info->do_fail_no[i] = 0xff;
	}
	DO_test_update();

	sleep(DIO_DELAY);
	DI_test_update();
	// check all DI, must all 0
	for (i = 0; i < DI_NUM; i++) {
		info->di_fail_no[i] = 0xff;
		if (DI_STAT(i)) {
			TEST_PRINT("step 1. DI(%d) err. should be 0\r\n", i + 1);
			info->di_fail_no[i] = 1;
			ret = FALSE;
		}
	}

	sleep(DIO_DELAY);

	int di1, di2, di3;

	// check all DI, must all 0
	for (i = 0; i < DO_NUM; i++) {
		info->do_fail_no[i] = 0xff;
		DO_SET(i);
		DO_test_update();

		sleep(DIO_DELAY);
		DI_test_update();

		// 		di1= di1_map[i];
		// 		di2= di2_map[i];
		// 		di3= di3_map[i];

		di1 = i * 2;
		if (i >= 10)
			di1 -= 1;

		di2 = di1 + 1;

		if (i == 9 || i == 19)
			di2 = di1;


		if (di2 >= DI_NUM)
			di2 = di1;


		if ((DI_STAT(di1) == 0) || (DI_STAT(di2) == 0))
		{//出现错误
			LOG_PRINT("ERROR:DO(%d)->1  DI(%d)->%d DI(%d)->%d\n", i + 1, di1 + 1, DI_STAT(di1), di2 + 1, DI_STAT(di2));

			if (di1 == di2)
			{
				info->do_fail_no[i] = 1;
				info->di_fail_no[di1] = 1;
				ret = FALSE;
			}
			else if ((DI_STAT(di1) == 0) && (DI_STAT(di2) == 0)) {
				//DO错误概率大
				info->do_fail_no[i] = 1;
				ret = FALSE;
			}
			else if (DI_STAT(di1) == 0) {
				info->di_fail_no[di1] = 1;
				ret = FALSE;
			}
			else {
				info->di_fail_no[di2] = 1;
				ret = FALSE;
			}
		}

		for (j = 0; j < DI_NUM; j++) {

			if ((j != di1) && (j != di2))
			{
				if (DI_STAT(j))
				{
					TEST_PRINT("DI(%d) err. should be 0 DO%d\r\n", j + 1, i + 1);
					info->di_fail_no[j] = 1;
					ret = FALSE;
				}
			}
		}

		DO_CLR(i);
	}
	for (i = 0; i < DO_NUM; i++) {
		DO_CLR(i);
		//info->do_fail_no[i] = 0xff;
	}

	DO_test_update();

	for (i = 0; i < DO_NUM; i++)
	{
		LOG_PRINT("di_fail_no[%d]:%d,%d\n", i, info->di_fail_no[i], info->do_fail_no[i]);
	}

	return ret;
}

static BOOL DO_ALL_OUT(int v)
{
	int i;
	for (i = 0; i < DO_NUM; i++) {
		if (v)
			DO_SET(i);
		else
			DO_CLR(i);
	}
	DO_test_update();
}

static BOOL DIO_test(TestInfo_t* info)
{
	int n = 2;
	while (n--) {
		if (!DIO_test2(info))
			return FALSE;
	}

	return TRUE;
}


//#define curr(ad) ((float)((float)4096.0*(float)ad)/ (0x7fff)/120)
//#define VOL(ad) ((float)((float)4096.0*(float)ad)/ (0x7fff))

//#define PULLUP_RESIS    4750
//#define CalcNTCR(volt,base)  ((PULLUP_RESIS*(volt))/(base - (volt)))

#define NTC_NUM		6
#define CURR_NUM	6
#define VOL_NUM		2

#define CURR_IDX(ch)	(ch+NTC_NUM)
#define VOL_IDX(ch)		(CURR_IDX(ch) + CURR_NUM)


#define FILTER_CNT 5
static BOOL ai_test2(TestInfo_t* info)
{
	TEST_PRINT("========AI test==========\r\n");
	U32 r_arr[FILTER_CNT];
	int ch, ret, R;
	U16 ad;
	int cnt;
	ret = TRUE;
	int ad_ret = 0;

	//测试NTC
	for (ch = 0; ch < NTC_NUM; ch++)
	{
		for (cnt = 0; cnt < FILTER_CNT; cnt++)
		{
			ad_ret = get_ad_ntc(ch, &ad, 10);
			if (ad_ret) PRINT_ERR_INT(ad_ret);
			R = 4750 * ad / (0x7fff - ad);//CalcNTCR(vol,4096);
			r_arr[cnt] = R;
			sleep(50);
		}

		for (int k = 0; k < FILTER_CNT - 1; k++) {
			for (int j = k + 1; j < FILTER_CNT; j++) {
				if (r_arr[j] > r_arr[k]) {
					U32 t = r_arr[k];
					r_arr[k] = r_arr[j];
					r_arr[j] = t;
				}
			}
		}
		R = r_arr[FILTER_CNT / 2];

		U32 base = 10000;
		U32 range = 150;
		info->ai_data[ch] = (U16)R;

		if (R<(base - range) || R>(base + range)) {
			TEST_PRINT("bad R. %d,R:%d\r\n", ch + 1, R);

			info->ai_fail_no[ch] = 2 + (R < (base - range));
			ret = FALSE;
		}
		if (check_abort())
			break;
	}

	//测试4-20ma
	for (ch = 0; ch < CURR_NUM; ch++)
	{
		for (cnt = 0; cnt < FILTER_CNT; cnt++)
		{
			ad_ret = get_ad_current4_20ma(ch, &ad, 10);
			if (ad_ret) PRINT_ERR_INT(ad_ret);
			R = (int)curr(ad);
			r_arr[cnt] = R;
			sleep(50);
		}

		for (int k = 0; k < FILTER_CNT - 1; k++) {
			for (int j = k + 1; j < FILTER_CNT; j++) {
				if (r_arr[j] > r_arr[k]) {
					U32 t = r_arr[k];
					r_arr[k] = r_arr[j];
					r_arr[j] = t;
				}
			}
		}
		R = r_arr[FILTER_CNT / 2];

		U32 base = 50;//5ma
		U32 range = 5;
		info->ai_data[CURR_IDX(ch)] = (U16)R;

		if (R<(base - range) || R>(base + range)) {
			TEST_PRINT("bad Curr. %d (%d), c:%d\r\n", ch + 1, CURR_IDX(ch), R);
			info->ai_fail_no[CURR_IDX(ch)] = 2 + (R < (base - range));
			ret = FALSE;
		}
		if (check_abort())
			break;
	}


	//测试VIN和VO
	for (ch = 0; ch < VOL_NUM; ch++)
	{
		for (cnt = 0; cnt < FILTER_CNT; cnt++)
		{
			ad_ret = get_ad_vol_0_5V(ch, &ad, 10);
			if (ad_ret) PRINT_ERR_INT(ad_ret);
			R = (int)VOL(ad, 6114) * 2;//输入做了1/2分压
			r_arr[cnt] = R;
			sleep(50);
		}

		for (int k = 0; k < FILTER_CNT - 1; k++) {
			for (int j = k + 1; j < FILTER_CNT; j++) {
				if (r_arr[j] > r_arr[k]) {
					U32 t = r_arr[k];
					r_arr[k] = r_arr[j];
					r_arr[j] = t;
				}
			}
		}
		R = r_arr[FILTER_CNT / 2];

		U32 base = 5000;//5v
		U32 range = 200;//200mv
		info->ai_data[VOL_IDX(ch)] = (U16)R;

		if (R<(base - range) || R>(base + range)) {
			TEST_PRINT("bad vol. %d (%d), c:%d\r\n", ch + 1, VOL_IDX(ch), R);
			info->ai_fail_no[VOL_IDX(ch)] = 2 + (R < (base - range));
			ret = FALSE;
		}
		if (check_abort())
			break;
	}

	for (ch = 0; ch < 30; ch++)
	{
		LOG_PRINT("ai_fail_no[%d] %d\r\n", ch, info->ai_fail_no[ch]);
	}


	if (ad_ret)
	{
		info->m_fail_no = 1;
		return FALSE;
	}

	info->m_fail_no = 0;
	TEST_PRINT("AI test done.\r\n");
	return ret;
}

static BOOL ai_test(TestInfo_t* info)
{
	int n = 2;
	//while(n--) 
	{
		if (!ai_test2(info))
			return FALSE;
	}

	return TRUE;
}


BOOL rtc_test(TestInfo_t* p)
{
	Time_t t;
	if (!RTC_ISL1208_SetTime(&p->date))
		return FALSE;
	sleep(1200);
	if (!RTC_ISL1208_GetTime(&t))
		return FALSE;


	int s1 = (int)BCD2Char(p->date.hour) * 3600 + (int)BCD2Char(p->date.minute) * 60 + (int)(int)BCD2Char(p->date.second + 1);
	int s2 = (int)BCD2Char(t.hour) * 3600 + (int)BCD2Char(t.minute) * 60 + (int)(int)BCD2Char(t.second);

	int diff = s2 - s1;
	LOG_PRINT("diff time=%d\r\n", diff);
	if (diff < -2 || diff>2)
		return FALSE;

	if (p->date.second == t.second)
		return FALSE;

	return TRUE;
}

BOOL ADC_hw_test2(TestInfo_t* p)
{

	return TRUE;
}

#define get_addr()	gpioGetInValue(GPIOF, 10) //地址GPIO
BOOL addr_test(TestInfo_t* p)
{
	//CAB没有地址
// 	BEEP(50);
// 	int cnt = 1500;
// 	int sw_cnt=0;//变化次数
// 	BOOL stat=get_addr();//地址GPIO;
// 	while(cnt > 0)
// 	{
// 		cnt -=1;
// 		sleep(10);//15秒
// 		if(stat != get_addr()){
// 			if((++sw_cnt) >= 4)
// 				return TRUE;
// 			stat = get_addr();
// 		}
// 	}
	return FALSE;
}


void set_valve_pos(U16 pos1, U16 pos2);
BOOL value_test(TestInfo_t* p, int n)
{
#ifdef _PCA9629_
	DEV_STAT_DATA_2 stat;
	valve_get_stat_data(&stat);

	if (n == 1)
	{//阀1顺时针
		set_valve_pos(stat.dest_step[0] + 200, stat.dest_step[1]);
	}
	else if (n == 2)
	{//阀1逆时针
		set_valve_pos(stat.dest_step[0] - 200, stat.dest_step[1]);
	}
	else if (n == 3)
	{//阀1顺时针
		set_valve_pos(stat.dest_step[0], stat.dest_step[1] + 200);
	}
	else if (n == 4)
	{//阀1逆时针
		set_valve_pos(stat.dest_step[0], stat.dest_step[1] - 200);
	}
#endif//_PCA9629_
}

static BOOL RS485_test(TestInfo_t* p)
{
	TEST_PRINT("========RS485 test==========\r\n");
	p->m_fail_no = 0;
	for (int i = 0; i < TEST_BUF_SIZE; i++)
		s_test_buf[i] = (U8)i;
	//RS485_TX_ON();
	comm_uart4.Write((char*)s_test_buf, TEST_BUF_SIZE);

	sleep(1500);
	mem_set(s_test_buf, 0, 7 + TEST_BUF_SIZE);
	int ret = comm_uart4.TryRead((char*)s_test_buf, 7 + TEST_BUF_SIZE);
	TEST_PRINT("recv %d\r\n", ret);
	if (ret != 7 + TEST_BUF_SIZE) {
		TEST_PRINT("lost data. failed.\r\n");
		p->m_fail_no = 1;
		return FALSE;
	}
	for (int i = 0; i < TEST_BUF_SIZE; i++)
		if (s_test_buf[7 + i] != i) {
			TEST_PRINT("i=%d recv=%02x\n", i, s_test_buf[i]);
			p->m_fail_no = 2;
			return FALSE;
		}

	TEST_PRINT("RS485 test success.\r\n");
	return TRUE;
}

extern char g_mac1[18];
extern char g_mac2[18];
extern char g_device_op[20];

static BOOL eth_test(TestInfo_t* p)
{

	if (trdp_get_stat() == TRDP_OK)
	{//正常
		mem_copy(p->trdp_info1, (U8*)g_mac1, 18);
		mem_copy(p->trdp_info2, (U8*)g_mac2, 18);
		mem_copy(p->trdp_dev_op_info, (U8*)g_device_op, 20);
		p->trdp_info1[17] = 0;
		p->trdp_info2[17] = 0;
		p->trdp_dev_op_info[19] = 0;
		return TRUE;
	}

	mem_set((U8*)p->trdp_info1, 0xff, 18);
	mem_set((U8*)p->trdp_info2, 0xff, 18);
	return FALSE;
}

static BOOL s_beep_test = FALSE;
static void ai_test_loop();
int trdp_udp_stat();
int trdp_udp_test_task();
int Equ_Test(U8* buf, int len) {
	TestInfo_t* p = (TestInfo_t*)buf;
	s_beep_test = p->m_beep_flag;

	DO_ALL_OUT(p->m_do_all_out_flag);

	if (p->m_dio_flag)
		p->m_dio_results = DIO_test(p);
	else if (p->m_ai_flag)
		p->m_ai_results = ai_test(p);
	else if (p->m_flash_all_flag)
		p->m_flash_all_results = SpiFlashTest(SPI_FLASH_SIZE);
	else if (p->m_flash_fast_flag)
		p->m_flash_fast_results = SpiFlashTest(SPI_FLASH_TEST_SIZE);
	else if (p->m_fram_flag)
		p->m_fram_results = SpiFramTest();
	else if (p->m_adc_flag) {
		p->m_adc_results = ADC_hw_test2(p);
	}
	else if (p->m_rtc_flag)
		p->m_rtc_results = rtc_test(p);
	else if (p->m_addr_flag)
		p->m_addr_results = addr_test(p);
	else if (p->m_rs485_flag)
		p->m_rs485_results = RS485_test(p);
	else if (p->m_eth_flag)
		p->m_fail_no = eth_test(p);

	return sizeof(TestInfo_t);
}


U8 di_test_flag[DI_NUM];
void di_test(TestInfo_t* info)
{
	int i;
	out_di_110V(CLOSE);

	for (int i = 0; i < DI_NUM; i++)
	{
		di_test_flag[i] = 0xff;
	}

	//while(true)
	{
		out_di_110V(CLOSE);
		out_di_dtc(CLOSE);
		//out_di_dtc(OPEN);
		sleep(50);
		DI_test_update();

		for (i = 0; i < DI_NUM; i++)
		{
			LOG_PRINT("di_test() DI(%02d):%d\r\n", i + 1, DI_STAT(i));
			if (DI_STAT(i) != 1)
			{
				di_test_flag[i] = 1;
			}
		}

		out_di_110V(OPEN);
		out_di_dtc(CLOSE);
		sleep(200);
		DI_test_update();

		for (i = 0; i < DI_NUM; i++)
		{
			LOG_PRINT("di_test() DI(%02d):%d\r\n", i + 1, DI_STAT(i));
			for (i = 0; i < DI_NUM; i++)
			{
				LOG_PRINT("di_test() DI(%02d):%d\r\n", i + 1, DI_STAT(i));
				if (DI_STAT(i) != 0)
				{
					di_test_flag[i] = 1;
				}
			}

		}
		//CheckIAPBus(0);
	}

	for (i = 0; i < DI_NUM; i++) {
		if (di_test_flag[i] != 0xff) {
			//自测试DI有问题
			info->di_fail_no[i] = 1;
		}
	}

	int di1, di2;
	for (i = 0; i < DO_NUM; i++) {
		if (i < 38)
		{
			di1 = i;
			//di2 = i + 38;
			di2 = di1;
		}
		else
		{
			di1 = 36 + i - 38;
			di2 = 74 + i - 38;
		}

		if (di2 >= DI_NUM)
		{
			di2 = di1;//DI76没焊接
		}

		if (di_test_flag[di1] != 0xff && di_test_flag[di2] != 0xff)
		{//DO没问题概率大，DI两路自测试有问题
			info->do_fail_no[i] = 0xff;
		}
	}

#if 1
	for (i = 0; i < DI_NUM; i++) {
		if (di_test_flag[i] != 0xff)
		{//DI有问题
			if ((info->do_fail_no[i] != 0xff) && (info->di_fail_no[i] != 0xff))
			{
				info->do_fail_no[i] = 0xff;
			}
		}
		else
		{//DI没问题
			if ((info->do_fail_no[i] != 0xff) && (info->di_fail_no[i] != 0xff))
			{
				info->di_fail_no[i] = 0xff;
			}
		}
	}
#endif

	out_di_110V(OPEN);
	out_di_dtc(OPEN);
}


#define CAN_ADDR_TEST 5
extern U32 can1_rx0_isr, can1_rx1_isr, can2_rx0_isr, can2_rx1_isr;
extern U32 can1_tx_isr, can2_tx_isr;
void dump_can();

//U8 buf[8];
static void can_test()
{
	U8* buf = s_test_buf;
	// 	mem_set(buf, 0x33, 8);
	U32 tx_id = MakeTID(CAN_ADDR_TEST, TID_MAC_MASK, 0, 0);
	// 	int ret = can_send_packet(CAN_BUS_1, tx_id, buf, 8);
	// 	LOG_PRINT("can_test ret:%d\r\n", ret);
	// 
	// 	dump_can();

	int rx_len = can_recv_packet(CAN_BUS_1, &tx_id, buf, 8);
	if (rx_len > 0) {
		LOG_PRINT("can_recv id:0x%x");
		memdump(buf, rx_len);
		tx_id = MakeTID(CAN_ADDR_TEST, TID_MAC_MASK, 0, 0);
		can_send_packet(CAN_BUS_1, tx_id, buf, 8);
		// 	int ret = can_send_packet(CAN_BUS_1, tx_id, buf, 8);
	}
}

static void rs485_uart6_test()
{
	//mem_set(s_test_buf, 0x66, 8);
	int i;
	for (i = 0; i < 100; i++)
		s_test_buf[i] = i;

	Uart4_Write((char*)s_test_buf, 10);
	Uart6_Write((char*)s_test_buf, 10);

	int ret = Uart6_TryRead((char*)s_test_buf, 100);
	if (ret > 0)
	{
		LOG_PRINT("Uart6_TryRead len:%d ", ret);
		memdump(s_test_buf, ret);
	}
}

void do_test()
{
	int s = 0;
	while (true)
	{
		for (int i = 0; i < DO_NUM; i++)
		{
			if ((i & 1) == s)
				DO_SET(i);
			else
				DO_CLR(i);
		}
		DO_test_update();

		s = 1 - s;

		sleep(2000);
		CheckIAPBus(CHECK_IAP_USB);
	}
}

TestInfo_t info;

void ftpd_thread_entry(void* parameter);
static void hardware_test()
{
	TEST_PRINT("===========hardware test====================\r\n");

	U8 last_sec;
	Time_t t1;
	U32 beep_time = sys_time();
	int i = 0, ret;
	U32 wdt_time = 0;

	MCP4728_DA_OUT_Sequential(5000, 5000, 5000, 5000);

	while (1)
	{
		//		valve_test();
		update_trdp_app_data(CHANNEL_AUTO);
		can_test();
		if (s_beep_test)
			BEEP(500);
		//mvb_test(&info);
		//check_mvb_sn();
		RTC_ISL1208_GetTime(&t1);
		if (last_sec != t1.second) {
			RTC_ISL1208_print(&t1);
			last_sec = t1.second;

			//			comm_uart4.Read((char*)s_test_buf, 5);
			//			comm_uart4.Write((char*)s_test_buf, 5);

			//			PCA8574_write_led_flicker(((6) & 7) | (0xD0));
			//			PCA8574_write_led_flicker(((1) & 7) | (0xE0));
			//			LOG_PRINT("PCA8574_write_led_flicker\r\n");
			//			PCA8574_write_led_flicker((2) | (0xe0));

			//			vol_0_5V_test();
#if 0
			ntc_test();
			current4_20ma_test();
			MCP4728_DA_OUT_Sequential(10000, 2000, 3000, 10000);
			vol_0_5V_test();

			//MCP4728_DA_OUT_Sequential(8000,8000,8000,8000);
			ad_ntc_vref_test();
			ad_5v_ios_vref_test();
#endif // 0

			//  			DI_test_update();
			//  			for (i=0;i<38;i++)
			//  			{
			//  				LOG_PRINT("DI%02d = %d\r\n",i+1,DI_STAT(i));
			//  			}

			LOG_PRINT("addr:%d\r\n", gpioGetInValue(GPIOF, 10));

#if 1
			if (i == 0)
			{
				LED_COMM_OFF();
				LED_FAULT_OFF();
				LED_RUN_ON();
				i = 1;
			}
			else if (i == 1)
			{
				LED_COMM_OFF();
				LED_FAULT_ON();
				LED_RUN_OFF();
				i = 2;
			}
			else if (i == 2)
			{
				LED_COMM_ON();
				LED_FAULT_OFF();
				LED_RUN_OFF();
				i = 0;
			}
#endif // 0

		}


		if ((ret = g_ptu_udp.check_udp_data(s_buf, 1024)) > 0)
		{
			// BEEP(50);
			BOOL check_ok = TRUE;
			if (s_buf[0] != 0xF5) check_ok = FALSE;
			//LOG_PRINT("g_ptu_udp check 1 %d\r\n",check_ok);
			int len = Get16(s_buf + 1);
			if (len > 2048) check_ok = FALSE;
			//LOG_PRINT("g_ptu_udp check 2 %d\r\n",check_ok);
			U8 sum = 0;
			for (int i = 0; i < len - 1; i++)
				sum += (unsigned char)s_buf[i];
			if (sum != s_buf[len - 1]) check_ok = FALSE;
			//LOG_PRINT("g_ptu_udp check 3 %d\r\n",check_ok);

			if (check_ok)
				maintenance_uart_recv(MAINTENANCE_NET, s_buf, ret);
		}
		maintenance_update();


		CheckIAPBus(CHECK_IAP_UDP);
		sleep(1);
	}

}


