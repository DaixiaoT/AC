#include "sys_api.h"
#include "trdp_app.h"
#include "dg_trdp.h"
#include "os_def.h"
#include "dg_error.h"
#include "dg_svc.h"
#include <string.h>
#include "trdp_data.h"
#include "vos_crc32.h"
#include "trdp_udp.h"

// BOOL g_trdp_timeout = FALSE;
static char res[255];
static sema_t s_lock;
#define TRDP_LOCK() p_sema(&s_lock, -1)
#define TRDP_UNLOCK() v_sema(&s_lock)

#define CCUTOHAVC_IP_A "239.255.0.50"
#define CCUTOHAVC_IP_B "239.255.0.50"

#define CCUTOALL_IP_A "239.255.0.1"
#define CCUTOALL_IP_B "239.255.0.1"

#define HAVCTOCCU_IP_A "239.255.0.50"
#define HAVCTOCCU_IP_B "239.255.0.50"

#define CCUTOHAVC_COMIDA 1050
#define CCUTOHAVC_COMIDB 1050

#define CCUTOALL_COMIDA 1001
#define CCUTOALL_COMIDB 1001

// #define HAVCTOCCU_COMIDA 1140
// #define HAVCTOCCU_COMIDB 1240
#define HAVCTOCCU_COMIDA (s_hvac_config_a[s_car_idx].commid)
#define HAVCTOCCU_COMIDB (s_hvac_config_b[s_car_idx].commid)

#define CCUTOHAVC_DATASETA 1022
#define CCUTOHAVC_DATASETB 1322

#define CCUTOALL_DATASETA 1010
#define CCUTOALL_DATASETB 1310

// #define HAVCTOCCU_DATASETA 1140
// #define HAVCTOCCU_DATASETB 1240
#define HAVCTOCCU_DATASETA (s_hvac_config_a[s_car_idx].dataset)
#define HAVCTOCCU_DATASETB (s_hvac_config_b[s_car_idx].dataset)

// #define TRDP_IF_IP_A    "10.1.1.11"
// #define TRDP_IF_IP_B    "10.2.1.11"
#define TRDP_IF_IP_A (s_hvac_config_a[s_car_idx].local_ip)
#define TRDP_IF_IP_B (s_hvac_config_b[s_car_idx].local_ip)

static U32 HVACXs_Lifesign_U32 = 0;
static U8 s_car_idx = 1;
typedef struct
{
	const char* local_ip;
	DG_U32 commid;
	DG_U32 dataset;
} TRDP_CONFIG_T;

static const TRDP_CONFIG_T s_hvac_config_a[8] = {
	{"10.0.1.50", 2150, 2150},
	{"10.0.2.50", 2250, 2250},
	{"10.0.3.50", 2350, 2350},
	{"10.0.4.50", 2450, 2450},
	{"10.0.5.50", 2550, 2550},
	{"10.0.6.50", 2650, 2650},
	{"10.0.7.50", 2750, 2750},
	{"10.0.8.50", 2850, 2850} };

static const TRDP_CONFIG_T s_hvac_config_b[8] = {
	{"10.0.1.51", 2151, 2151},
	{"10.0.2.51", 2251, 2251},
	{"10.0.3.51", 2351, 2351},
	{"10.0.4.51", 2451, 2451},
	{"10.0.5.51", 2551, 2551},
	{"10.0.6.51", 2651, 2651},
	{"10.0.7.51", 2751, 2751},
	{"10.0.8.51", 2851, 2851} };

static U32 s_tick_ccutohvac_a = 0;
static U32 s_tick_ccutoall_a = 0;
static U32 s_tick_ccutohvac_b = 0;
static U32 s_tick_ccutoall_b = 0;

static TRDP_CCU2HVAC s_ccutohavcdata_a, s_ccutohavcdata_a_app;
static TRDP_CCU2HVAC s_ccutohavcdata_b, s_ccutohavcdata_b_app;

static TRDP_COMM_DATA s_ccutoalldata_a, s_ccutoalldata_a_app;
static TRDP_COMM_DATA s_ccutoalldata_b, s_ccutoalldata_b_app;

static TRDP_HVAC2CCU s_havctoccudata_a, s_havctoccudata_a_app;
static TRDP_HVAC2CCU s_havctoccudata_b, s_havctoccudata_b_app;

TRDP_CCU2HVAC g_ccutohavcdata;
TRDP_COMM_DATA g_ccutoalldata;
TRDP_HVAC2CCU g_havctoccudata;

// static DG_U8 ccutohavcdataA[100];
// static DG_U8 ccutohavcdataB[100];
//
// static DG_U8 ccutoalldataA[101];
// static DG_U8 ccutoalldataB[101];
//
// static DG_U8 havctoccudataA[88];
// static DG_U8 havctoccudataB[88];

static DG_U32 ccutohavcdatasize = sizeof(TRDP_CCU2HVAC);
static DG_U32 ccutoalldatasize = sizeof(TRDP_COMM_DATA);
static DG_U32 havctoccudatasize = sizeof(TRDP_HVAC2CCU);

static DG_U32 nb_exchg_par = 6;
static DG_U32 nb_data_set = 6;

static DG_U32 pd_cycle = 500000; // 发送周期500ms
static DG_U32 pd_redundant = 0;
static DG_U32 pd_timeout = 1000000; // 接收超时500ms*2

DG_U16 pd_offset = 0;

static TRDP_STAT s_stat = TRDP_NULL; // 网卡初始化状态

/* ==================================================================================
 * Local function declarations
 * ================================================================================*/
static SVC_S32 set_key(const char* key, const char* val, EXT_HANDLE_P hdl);
static SVC_S32 get_key(const char* key, char* res, EXT_HANDLE_P hdl);
static SVC_S32 if_is_up(const char* key, EXT_HANDLE_P hdl);
static void wait_until_up(const char* key, SVC_S32 timeout, EXT_HANDLE_P hdl);
static SVC_S32 svc_initialize(EXT_HANDLE_P hdl);
static SVC_S32 svc_check_parameter(const char* key, const char* val, SVC_S32* status, EXT_HANDLE_P hdl);
static SVC_S32 svc_check_config(EXT_HANDLE_P hdl);
static DG_S32 trdp_pd_demo(DG_S32* trdp_err_no);

/* ==================================================================================
 * Local function implementations
 * ================================================================================*/
static SVC_S32 set_key(const char* key, const char* val, EXT_HANDLE_P hdl)
{
	SVC_S32 svc_errno = 0;
	SVC_S32 status;

	status = svc_set(key, val, &svc_errno, hdl);

	if (status == SVC_OK)
	{
		osl_printf("%-20s <- %s\n", key, val);
	}
	else
	{
		osl_printf("%-20s <- %s svc_errno = %d (SVC_FAILED)\n", key, val, svc_errno);
	}

	return status;
}

static SVC_S32 get_key(const char* key, char* res, EXT_HANDLE_P hdl)
{
	SVC_S32 status;
	// char res[255];
	SVC_S32 svc_errno = 0;

	status = svc_get(key, res, 255, &svc_errno, hdl);

	if (status == SVC_OK)
	{
		osl_printf("%-20s -> %s\n", key, res);
	}
	else
	{
		osl_printf("%-20s -> svc_errno = %d (SVC_FAILED)\n", key, svc_errno);
	}

	return status;
}

static SVC_S32 if_is_up(const char* key, EXT_HANDLE_P hdl)
{
	SVC_S32 status;
	// char res[255];
	SVC_S32 svc_errno = 0;
	SVC_S32 ret = 0;

	status = svc_get(key, res, sizeof(res), &svc_errno, hdl);
	if (status != SVC_OK)
	{
		osl_printf("%-20s -> (SVC_FAILED) \n", key);
	}
	else
	{
		if (strcmp(res, "up") == 0)
		{
			osl_printf("\n\n%s interface is up\n", key);
			ret = 1;
		}
	}
	return ret;
}

static void wait_until_up(const char* key, SVC_S32 timeout, EXT_HANDLE_P hdl)
{
	SVC_S32 status;
	// char res[255];
	SVC_S32 svc_errno = 0;
	SVC_S32 wait_time = 0;

	osl_printf("wait until ETH interface is up (key=%s)\n", key);

	if (strcmp(key, "if_eth0_up") == 0)
	{
		status = set_key("if_eth0_interface", "start", hdl);
	}
	else if (strcmp(key, "if_eth1_up") == 0)
	{
		status = set_key("if_eth1_interface", "start", hdl);
	}
	else
	{
		osl_printf("\n\n %s input parameter error!!!\n", key);
	}

	while (1)
	{
		// wait 100ms per circle, this delay should be implemented by customer
		sleep(1);

		wait_time++;

		status = svc_get(key, res, sizeof(res), &svc_errno, hdl);

		if (status != SVC_OK)
		{
			osl_printf("%-20s -> %s\n", key, "SVC_FAILED");
			return;
		}

		if (strcmp(res, "up") == 0)
		{
			osl_printf("\n\n%s interface is up\n", key);
			return;
		}
		else
		{
			osl_printf("<%s>", res);

			// if wait time exceeds the specified time, break
			if (wait_time > (timeout * 10))
			{
				osl_printf("\n\n wait %s is timeout!!!\n", key);
				return;
			}
		}
	}
}

static SVC_S32 svc_initialize(EXT_HANDLE_P hdl)
{
	SVC_S32 svc_errno = 0; // error code from the firmware
	int cnt = 100;

	while ((svc_init(&svc_errno, hdl) == -1) && (cnt--))
	{
		switch (svc_errno)
		{
		case DG_ERROR_OSL_NOT_READY:
			LOG_PRINT("cnt:%d\t", cnt);
			osl_printf("svc_initialize REGISTERS not ready\n");

			sleep(100);
			break;

		case DG_ERROR_EMPTY_BLOCK:
			LOG_PRINT("cnt:%d\t", cnt);
			osl_printf("PROTOCOL not ready\n");
			sleep(100);
			break;

		case DG_ERROR_OSL_INIT:
			LOG_PRINT("cnt:%d\t", cnt);
			osl_printf("I/O access initialization failed\n");
#ifdef _WIN32
			osl_printf("is SYS device running? (GiveIO or OLS)\n");
#endif
			sleep(100);
			return -1;

		default:
			LOG_PRINT("cnt:%d\t", cnt);
			osl_printf("svc_init failed with error: %d\n", svc_errno);
			sleep(100);
			return -1;
		}
	}
	if (cnt <= 0)
		return -1;
	return 0;
}

static SVC_S32 svc_check_parameter(const SVC_CHAR8* key, const SVC_CHAR8* val, SVC_S32* status, EXT_HANDLE_P hdl)
{
	SVC_S32 ret_val = 0;
	// char res[255] = {0};
	ret_val = get_key(key, res, hdl);
	if (ret_val == 0)
	{
		if (strcmp(val, res) != 0)
		{
			ret_val = set_key(key, val, hdl);
			(*status)++;
		}
	}
	return ret_val;
}

char g_mac1[18];
char g_mac2[18];
char g_device_op[20];
static char s_res[255] = { 0 };

static SVC_S32 svc_check_config(EXT_HANDLE_P hdl)
{
	SVC_S32 ret_val = 0;
	// char res[255] = {0};
	SVC_S32 cfg_flag = 0;

	do
	{
		cfg_flag = 0;

		// Sleep 10s, wait TRDP card prepare registers
		// sleep(10 * 1000);

		ret_val = svc_initialize(hdl);
		if (ret_val != 0)
		{
			osl_printf("\n svc_initialize error! \n");
			return ret_val;
		}

		get_key("if_eth0_mac", &s_res[0], hdl);
		// osl_printf("if_eth0_mac:%s\r\n",s_res);
		mem_copy((U8*)g_mac1, (U8*)s_res, 18);

		get_key("if_eth1_mac", &s_res[0], hdl);
		osl_printf("if_eth1_mac:%s\r\n", s_res);
		mem_copy((U8*)g_mac2, (U8*)s_res, 18);

		get_key("device_op", &s_res[0], hdl);
		osl_printf("device_op:%s\r\n", s_res);
		mem_copy((U8*)g_device_op, (U8*)s_res, 20);

		// Check ETH0 is connected  timeout = 10s
		wait_until_up("if_eth0_up", 5, hdl);

		// Check ETH1 is connected  timeout = 10s
		wait_until_up("if_eth1_up", 5, hdl);

#ifndef _BOOT_
		// Use upper case whenever possible. if you can't find it, use lower case
		// check ETH0 config does not match the code,if not we will call set_key()
		ret_val = svc_check_parameter("ETH0_DHCP", "0", &cfg_flag, hdl);
		ret_val = svc_check_parameter("ETH0_IP", TRDP_IF_IP_A, &cfg_flag, hdl);
		ret_val = svc_check_parameter("ETH0_NETMASK", "255.255.0.0", &cfg_flag, hdl);
		ret_val = svc_check_parameter("ETH0_GATEWAY", "0.0.0.0", &cfg_flag, hdl);

		// check ETH1 config does not match the code,if not we will call set_key()
		ret_val = svc_check_parameter("ETH1_DHCP", "0", &cfg_flag, hdl);
		ret_val = svc_check_parameter("ETH1_IP", TRDP_IF_IP_B, &cfg_flag, hdl);
		ret_val = svc_check_parameter("ETH1_NETMASK", "255.255.0.0", &cfg_flag, hdl);
		ret_val = svc_check_parameter("ETH1_GATEWAY", "0.0.0.0", &cfg_flag, hdl);
#endif //_BOOT_

		// if the device.cfg file does not match the code, software restart
		if (cfg_flag != 0)
		{
			osl_printf("\n device.cfg has been modified in (%d) places!\n", cfg_flag);
#ifdef CARD_D215E
			ret_val = set_key("device_softreset", "", hdl);
#else
			ret_val = set_key("device_reset", "", hdl);
#endif
			// Sleep 15s, wait TRDP card prepare for reset
			sleep(15 * 1000);
		}
	} while (cfg_flag);

#if 1
	set_key("ETH0_START_AT_POWERUP", "1", hdl);
	set_key("ETH1_START_AT_POWERUP", "1", hdl);
	// Check if ETH0 is connected  timeout = 60s
	wait_until_up("if_eth0_up", 60, hdl);

	// Check if ETH1 is connected  timeout = 60s
	wait_until_up("if_eth1_up", 5, hdl);
#else
	set_key("ETH0_START_AT_POWERUP", "1", hdl);
	set_key("ETH1_START_AT_POWERUP", "1", hdl);
	// set_key("ETH1_START_AT_POWERUP", "0", hdl);
#endif

	return ret_val;
}

void trdp_set_stat(TRDP_STAT s)
{
	TRDP_LOCK();
	s_stat = s;
	TRDP_UNLOCK();
}

TRDP_STAT trdp_get_stat()
{
	if (s_stat == TRDP_NULL)
		return s_stat;
	TRDP_STAT s;
	TRDP_LOCK();
	s = s_stat;
	TRDP_UNLOCK();
	return s;
}

static void trdp_hw_reset()
{
	TRDP_RST_LOW();
	sleep(200);
	
	TRDP_RST_HIGH();

}

static int trdp_config()
{
	DG_S32 trdp_err_no = 0;
	DG_S32 ret_val = 0;
	DG_U32 iface = 0;
	DG_S32 nb_pub_pd = 0;
	DG_S32 nb_sub_pd = 0;
	DG_S32 nb_list_md = 0;
	DG_TRDP_URI_T dest_ip;
	DG_TRDP_URI_T src_ip;
	DG_TRDP_URI_T src_iptemp;

	int retry_cnt = 0;
	// Check TRDP card configuration
	ret_val = svc_check_config(NULL);
	osl_printf("svc_check_config ret_val = %d\n", ret_val);
	if (ret_val)
		return -1;

#ifdef _BOOT_
	return 0;
#endif //_BOOT_

	osl_printf("Initializing TRDP\n");
	retry_cnt = 10;
	while ((trdp_init(&trdp_err_no, NULL) == -1) && (retry_cnt-- >= 0))
	{
		switch (trdp_err_no)
		{
		case DG_ERROR_OSL_NOT_READY:
			LOG_PRINT("retry_cnt:%d\t", retry_cnt);
			osl_printf("trdp_init REGISTERS not ready\n");
			sleep(100);
			break;

		case DG_ERROR_EMPTY_BLOCK:
			LOG_PRINT("retry_cnt:%d\t", retry_cnt);
			osl_printf("PROTOCOL not ready\n");
			sleep(100);
			break;

		case DG_ERROR_OSL_INIT:
			LOG_PRINT("retry_cnt:%d\t", retry_cnt);
			osl_printf("I/O access initialization failed\n");
#ifdef _WIN32
			osl_printf("is SYS device running? (GiveIO or OLS)\n");
#endif
			sleep(100);
			return -2;

		default:
			LOG_PRINT("retry_cnt:%d\t", retry_cnt);
			osl_printf("trdp_init failed with error: %d\n", trdp_err_no);
			sleep(100);
			return -3;
		}
	}
	if (retry_cnt <= 0)
		return -4;

	/*内存分配及过程数据端口周期初始化*/
	ret_val = trdp_initialize_memory(0, nb_exchg_par, nb_data_set, &trdp_err_no, NULL);
	ret_val = trdp_configure_interface(0, &trdp_err_no, NULL);
	ret_val = trdp_configure_process_data(0, 17224, DG_TRDP_FLAGS_NONE, 1000000, DG_TRDP_TO_SET_TO_ZERO, 0, 0, &trdp_err_no, NULL);

	ret_val = trdp_initialize_memory(1, nb_exchg_par, nb_data_set, &trdp_err_no, NULL);
	ret_val = trdp_configure_interface(1, &trdp_err_no, NULL);
	ret_val = trdp_configure_process_data(1, 17224, DG_TRDP_FLAGS_NONE, 1000000, DG_TRDP_TO_SET_TO_ZERO, 0, 0, &trdp_err_no, NULL);

	/* Configure dataset 数据类型U8*/
	DG_U32 type[1] = { 8 }; //?????????????????????????????  这里编译出错，这个修改的，原来DG发来的是下行的
				  // ret_val = trdp_configure_data_set(CCUTOALL_DATASETA, 1, 8, &ccutoalldatasize, &trdp_err_no, NULL);
	ret_val = trdp_configure_data_set(CCUTOALL_DATASETA, 1, type, &ccutoalldatasize, &trdp_err_no, NULL);
	ret_val = trdp_configure_data_set(CCUTOALL_DATASETB, 1, type, &ccutoalldatasize, &trdp_err_no, NULL);
	ret_val = trdp_configure_data_set(CCUTOHAVC_DATASETA, 1, type, &ccutohavcdatasize, &trdp_err_no, NULL);
	ret_val = trdp_configure_data_set(CCUTOHAVC_DATASETB, 1, type, &ccutohavcdatasize, &trdp_err_no, NULL);
	ret_val = trdp_configure_data_set(HAVCTOCCU_DATASETA, 1, type, &havctoccudatasize, &trdp_err_no, NULL);
	ret_val = trdp_configure_data_set(HAVCTOCCU_DATASETB, 1, type, &havctoccudatasize, &trdp_err_no, NULL);

	/* Configure ComID on ETH0*/
	iface = 0;
	// ccutoall a
	ret_val = trdp_configure_com_id(iface, CCUTOALL_COMIDA, CCUTOALL_DATASETA, 1, 1, 0, DG_TRDP_EXCHG_UNSET, &trdp_err_no, NULL);
	ret_val = trdp_com_id_configure_pd_par(iface, CCUTOALL_COMIDA, pd_cycle, pd_redundant, pd_timeout, DG_TRDP_TO_SET_TO_ZERO,
		DG_TRDP_FLAGS_NONE, pd_offset, &trdp_err_no, NULL);
	strcpy(dest_ip, CCUTOALL_IP_A);
	strcpy(src_ip, CCUTOALL_IP_A);
	strcpy(src_iptemp, "0.0.0.0");
	ret_val = trdp_com_id_add_dest(iface, CCUTOALL_COMIDA, 1, dest_ip, &trdp_err_no, NULL);
	ret_val = trdp_com_id_add_src(iface, CCUTOALL_COMIDA, 1, src_iptemp, &trdp_err_no, NULL);
	// ccutohavc a
	ret_val = trdp_configure_com_id(iface, CCUTOHAVC_COMIDA, CCUTOHAVC_DATASETA, 1, 1, 0, DG_TRDP_EXCHG_UNSET, &trdp_err_no, NULL);
	ret_val = trdp_com_id_configure_pd_par(iface, CCUTOHAVC_COMIDA, pd_cycle, pd_redundant, pd_timeout, DG_TRDP_TO_SET_TO_ZERO,
		DG_TRDP_FLAGS_NONE, pd_offset, &trdp_err_no, NULL);
	strcpy(dest_ip, CCUTOHAVC_IP_A);
	strcpy(src_ip, CCUTOHAVC_IP_A);
	strcpy(src_iptemp, "0.0.0.0");
	ret_val = trdp_com_id_add_dest(iface, CCUTOHAVC_COMIDA, 1, dest_ip, &trdp_err_no, NULL);
	ret_val = trdp_com_id_add_src(iface, CCUTOHAVC_COMIDA, 1, src_iptemp, &trdp_err_no, NULL);
	// havctoccu a
	ret_val = trdp_configure_com_id(iface, HAVCTOCCU_COMIDA, HAVCTOCCU_DATASETA, 0, 1, 0, DG_TRDP_EXCHG_UNSET, &trdp_err_no, NULL);
	ret_val = trdp_com_id_configure_pd_par(iface, HAVCTOCCU_COMIDA, pd_cycle, pd_redundant, pd_timeout, DG_TRDP_TO_SET_TO_ZERO,
		DG_TRDP_FLAGS_NONE, pd_offset, &trdp_err_no, NULL);
	strcpy(dest_ip, HAVCTOCCU_IP_A);
	// strcpy(src_ip, HAVCTOCCU_IP);
	ret_val = trdp_com_id_add_dest(iface, HAVCTOCCU_COMIDA, 1, dest_ip, &trdp_err_no, NULL);
	// ret_val = trdp_com_id_add_src(iface, HAVCTOCCU_COMIDA, 1, src_ip, &trdp_err_no, NULL);

	/* Configure ComID on ETH1*/
	iface = 1;
	// ccutoall a
	ret_val = trdp_configure_com_id(iface, CCUTOALL_COMIDB, CCUTOALL_DATASETB, 1, 1, 0, DG_TRDP_EXCHG_UNSET, &trdp_err_no, NULL);
	ret_val = trdp_com_id_configure_pd_par(iface, CCUTOALL_COMIDB, pd_cycle, pd_redundant, pd_timeout, DG_TRDP_TO_SET_TO_ZERO,
		DG_TRDP_FLAGS_NONE, pd_offset, &trdp_err_no, NULL);
	strcpy(dest_ip, CCUTOALL_IP_B);
	strcpy(src_ip, CCUTOALL_IP_B);
	strcpy(src_iptemp, "0.0.0.0");
	ret_val = trdp_com_id_add_dest(iface, CCUTOALL_COMIDB, 1, dest_ip, &trdp_err_no, NULL);
	ret_val = trdp_com_id_add_src(iface, CCUTOALL_COMIDB, 1, src_iptemp, &trdp_err_no, NULL);
	// ccutohavc a
	ret_val = trdp_configure_com_id(iface, CCUTOHAVC_COMIDB, CCUTOHAVC_DATASETB, 1, 1, 0, DG_TRDP_EXCHG_UNSET, &trdp_err_no, NULL);
	ret_val = trdp_com_id_configure_pd_par(iface, CCUTOHAVC_COMIDB, pd_cycle, pd_redundant, pd_timeout, DG_TRDP_TO_SET_TO_ZERO,
		DG_TRDP_FLAGS_NONE, pd_offset, &trdp_err_no, NULL);
	strcpy(dest_ip, CCUTOHAVC_IP_B);
	strcpy(src_ip, CCUTOHAVC_IP_B);
	strcpy(src_iptemp, "0.0.0.0");
	ret_val = trdp_com_id_add_dest(iface, CCUTOHAVC_COMIDB, 1, dest_ip, &trdp_err_no, NULL);
	ret_val = trdp_com_id_add_src(iface, CCUTOHAVC_COMIDB, 1, src_iptemp, &trdp_err_no, NULL);

	// havctoccu b
	ret_val = trdp_configure_com_id(iface, HAVCTOCCU_COMIDB, HAVCTOCCU_DATASETB, 0, 1, 0, DG_TRDP_EXCHG_UNSET, &trdp_err_no, NULL);
	ret_val = trdp_com_id_configure_pd_par(iface, HAVCTOCCU_COMIDB, pd_cycle, pd_redundant, pd_timeout, DG_TRDP_TO_SET_TO_ZERO,
		DG_TRDP_FLAGS_NONE, pd_offset, &trdp_err_no, NULL);
	strcpy(dest_ip, HAVCTOCCU_IP_B);
	// strcpy(src_ip, HAVCTOCCU_IP);
	ret_val = trdp_com_id_add_dest(iface, HAVCTOCCU_COMIDB, 1, dest_ip, &trdp_err_no, NULL);
	// ret_val = trdp_com_id_add_src(iface, HAVCTOCCU_COMIDB, 1, src_ip, &trdp_err_no, NULL);

	return 0;

	// 	ret_val = -1;
	//     retry_cnt=200;
	// 	while((ret_val != 0) && (retry_cnt-->0))
	// 	{
	// 		osl_printf("trdp_initialize_stack %d\n",retry_cnt);
	// 		ret_val = trdp_initialize_stack(&trdp_err_no, NULL);
	//         sleep(100);
	// 	}

	//   //  ret_val = trdp_get_status(&nb_pub_pd, &nb_sub_pd, &nb_list_md, &trdp_err_no, NULL);
	// 	if(ret_val == DG_TRDP_NO_ERR)
	// 	{
	// 		//ret_val = trdp_pd_demo(&trdp_err_no);
	//         return 0;
	// 	}

	// 	osl_printf("Result: %d\n", trdp_err_no);
	//     return -5;
}

static void drive_trdp_app_data()
{
	TRDP_LOCK();
	mem_copy((U8*)&s_havctoccudata_a, (U8*)&s_havctoccudata_a_app, havctoccudatasize);

	if (s_ccutoalldata_a_app.CCU_Lifesign_U32 != s_ccutoalldata_a.CCU_Lifesign_U32)
		s_tick_ccutoall_a = sys_time();
	mem_copy((U8*)&s_ccutoalldata_a_app, (U8*)&s_ccutoalldata_a, ccutoalldatasize);

	if (s_ccutohavcdata_a_app.CCU_Lifesign_U32 != s_ccutohavcdata_a.CCU_Lifesign_U32)
		s_tick_ccutohvac_a = sys_time();
	mem_copy((U8*)&s_ccutohavcdata_a_app, (U8*)&s_ccutohavcdata_a, ccutohavcdatasize);
	//-------------------------------------------------------------------------------------
	mem_copy((U8*)&s_havctoccudata_b, (U8*)&s_havctoccudata_b_app, havctoccudatasize);

	if (s_ccutoalldata_b_app.CCU_Lifesign_U32 != s_ccutoalldata_b.CCU_Lifesign_U32)
		s_tick_ccutoall_b = sys_time();
	mem_copy((U8*)&s_ccutoalldata_b_app, (U8*)&s_ccutoalldata_b, ccutoalldatasize);

	if (s_ccutohavcdata_b_app.CCU_Lifesign_U32 != s_ccutohavcdata_b.CCU_Lifesign_U32)
		s_tick_ccutohvac_b = sys_time();
	mem_copy((U8*)&s_ccutohavcdata_b_app, (U8*)&s_ccutohavcdata_b, ccutohavcdatasize);

	TRDP_UNLOCK();
}

DG_DECL_PUBLIC DG_RESULT hdc_reset(DG_U8 card_index);
static DG_U8 s_rx_buf[1024];
DG_S32 s_ret_val_a = DG_TRDP_NO_ERR;
DG_S32 s_ret_val_b = DG_TRDP_NO_ERR;

static void update_crc(U8* buf, int len)
{
	U32 crc32_u32 = vos_crc32(INITFCS, buf, len - SIZE_OF_FCS);
	Set32(buf + len - SIZE_OF_FCS, crc32_u32);
}
int eth0_link = 0;
int eth1_link = 0;

void check_eth_link(EXT_HANDLE_P pCard_handle)
{
	if (if_is_up("if_eth0_up", pCard_handle))
		eth0_link = 1;
	else
		eth0_link = 0;

	if (if_is_up("if_eth1_up", pCard_handle))
		eth1_link = 1;
	else
		eth1_link = 0;
}

static int trdp_run()
{
	DG_S32 trdp_err_no;
	DG_S32 ret_val = DG_TRDP_NO_ERR;
	// DG_S32 prev_ret_val = DG_TRDP_NO_ERR;
	DG_U32 i = 0;
	TRDP_PD_INFO pd_info;
	pd_info.src_ip = 0; /* Don't care from which source to take data */
	ret_val = -1;

	while (1)
	{
		check_eth_link(0);
		if (eth0_link || eth1_link)
		{
			ret_val = trdp_initialize_stack(&trdp_err_no, NULL);
			if (ret_val == DG_TRDP_NO_ERR)
				break;
		}
		sleep(1000);
	}

	// if(DG_TRDP_NO_ERR == ret_val)
	{
		// prev_ret_val = ret_val;
		//   printf("publish ComID %d on ETH0\n", com_id);

		update_crc((U8*)&s_havctoccudata_a, havctoccudatasize);
		update_crc((U8*)&s_havctoccudata_b, havctoccudatasize);

		s_ret_val_a = trdp_pd_publish(HAVCTOCCU_COMIDA, 1, (DG_U8*)&s_havctoccudata_a, havctoccudatasize, &trdp_err_no, NULL);
		osl_printf("ret_val_a: %d, err_no: %d\n", s_ret_val_a, trdp_err_no);
		s_ret_val_b = trdp_pd_publish(HAVCTOCCU_COMIDB, 1, (DG_U8*)&s_havctoccudata_b, havctoccudatasize, &trdp_err_no, NULL);
		osl_printf("ret_val_b: %d, err_no: %d\n", s_ret_val_b, trdp_err_no);

		// if publish succeed on either interface, set ret_val to DG_TRDP_NO_ERR
		//	ret_val &= prev_ret_val;
	}

	// if(DG_TRDP_NO_ERR == ret_val)
	{
		//	prev_ret_val = ret_val;

		s_ret_val_a |= trdp_pd_subscribe(CCUTOALL_COMIDA, 1, &trdp_err_no, NULL);
		osl_printf("ret_val_a: %d, err_no: %d\n", s_ret_val_a, trdp_err_no);
		//	prev_ret_val = ret_val;

		s_ret_val_b |= trdp_pd_subscribe(CCUTOALL_COMIDB, 1, &trdp_err_no, NULL);
		osl_printf("ret_val_b: %d, err_no: %d\n", s_ret_val_b, trdp_err_no);
		s_ret_val_a |= trdp_pd_subscribe(CCUTOHAVC_COMIDA, 1, &trdp_err_no, NULL);
		osl_printf("ret_val_a: %d, err_no: %d\n", s_ret_val_a, trdp_err_no);
		s_ret_val_b |= trdp_pd_subscribe(CCUTOHAVC_COMIDB, 1, &trdp_err_no, NULL);
		osl_printf("ret_val_b: %d, err_no: %d\n", s_ret_val_b, trdp_err_no);

		//	ret_val &= prev_ret_val;
		// if subscribe succeed on either interface, set ret_val to DG_TRDP_NO_ERR
	}

	U32 time = sys_time();
	U32 t;

	while (trdp_get_stat() == TRDP_OK)
		// while(1)
	{
		t = sys_time();
		HVACXs_Lifesign_U32 += 1;
		TRDP_LOCK();
		s_havctoccudata_a.HVACXs_Lifesign_U32 = hton32(HVACXs_Lifesign_U32);
		s_havctoccudata_b.HVACXs_Lifesign_U32 = hton32(HVACXs_Lifesign_U32);
		;
		TRDP_UNLOCK();

		//		update_crc((U8*)&s_havctoccudata_a, havctoccudatasize);
		//		update_crc((U8*)&s_havctoccudata_b, havctoccudatasize);

		ret_val = trdp_pd_put(HAVCTOCCU_COMIDA, (DG_U8*)&s_havctoccudata_a, havctoccudatasize, &trdp_err_no, NULL);
		//	    osl_printf("Put dataset %u, %u, %u, %u, %u - ret_val %d, errno: %d\n", havctoccudataA[0], havctoccudataA[1], havctoccudataA[2], havctoccudataA[3], havctoccudataA[4], ret_val, trdp_err_no);
		ret_val = trdp_pd_put(HAVCTOCCU_COMIDB, (DG_U8*)&s_havctoccudata_b, havctoccudatasize, &trdp_err_no, NULL);
		//	    osl_printf("Put dataset %u, %u, %u, %u, %u - ret_val %d, errno: %d\n", havctoccudataB[0], havctoccudataB[1], havctoccudataB[2], havctoccudataB[3], havctoccudataB[4], ret_val, trdp_err_no);

		sleep(100);

		pd_info.src_ip = 0;
		ret_val = trdp_pd_get(CCUTOALL_COMIDA, &pd_info.src_ip, &pd_info.dest_ip, &pd_info.seq_count, &pd_info.msg_type, (DG_U8*)s_rx_buf, &ccutoalldatasize, &trdp_err_no, NULL);
		if (ret_val == DG_TRDP_NO_ERR)
			mem_copy((U8*)&s_ccutoalldata_a, (U8*)s_rx_buf, ccutoalldatasize);

		//	    osl_printf("Got dataset %u, %u, %u, %u, %u, from 0x%x, - ret_val %d, errno: %d\n", ccutoalldataA[0], ccutoalldataA[1], ccutoalldataA[2], ccutoalldataA[3], ccutoalldataA[4], pd_info.src_ip, ret_val, trdp_err_no);
		pd_info.src_ip = 0;
		ret_val = trdp_pd_get(CCUTOALL_COMIDB, &pd_info.src_ip, &pd_info.dest_ip, &pd_info.seq_count, &pd_info.msg_type, (DG_U8*)s_rx_buf, &ccutoalldatasize, &trdp_err_no, NULL);
		if (ret_val == DG_TRDP_NO_ERR)
			mem_copy((U8*)&s_ccutoalldata_b, (U8*)s_rx_buf, ccutoalldatasize);
		//	    osl_printf("Got dataset %u, %u, %u, %u, %u, from 0x%x, - ret_val %d, errno: %d\n", ccutoalldataB[0], ccutoalldataB[1], ccutoalldataB[2], ccutoalldataB[3], ccutoalldataB[4], pd_info.src_ip, ret_val, trdp_err_no);
		sleep(100);

		pd_info.src_ip = 0;
		ret_val = trdp_pd_get(CCUTOHAVC_COMIDA, &pd_info.src_ip, &pd_info.dest_ip, &pd_info.seq_count, &pd_info.msg_type, (DG_U8*)s_rx_buf, &ccutohavcdatasize, &trdp_err_no, NULL);
		if (ret_val == DG_TRDP_NO_ERR)
			mem_copy((U8*)&s_ccutohavcdata_a, (U8*)s_rx_buf, ccutohavcdatasize);
		//	    osl_printf("Got dataset %u, %u, %u, %u, %u, from 0x%x, - ret_val %d, errno: %d\n", ccutohavcdataA[0], ccutohavcdataA[1], ccutohavcdataA[2], ccutohavcdataA[3], ccutohavcdataA[4], pd_info.src_ip, ret_val, trdp_err_no);
		pd_info.src_ip = 0;
		ret_val = trdp_pd_get(CCUTOHAVC_COMIDB, &pd_info.src_ip, &pd_info.dest_ip, &pd_info.seq_count, &pd_info.msg_type, (DG_U8*)s_rx_buf, &ccutohavcdatasize, &trdp_err_no, NULL);
		if (ret_val == DG_TRDP_NO_ERR)
			mem_copy((U8*)&s_ccutohavcdata_b, (U8*)s_rx_buf, ccutohavcdatasize);
		//	    osl_printf("Got dataset %u, %u, %u, %u, %u, from 0x%x, - ret_val %d, errno: %d\n", ccutohavcdataB[0], ccutohavcdataB[1], ccutohavcdataB[2], ccutohavcdataB[3], ccutohavcdataB[4], pd_info.src_ip, ret_val, trdp_err_no);
		sleep(100);

		//	    osl_printf("\n");
		drive_trdp_app_data();

		if (TimeGap(time) >= 5000)
		{
			time = sys_time();

			if (s_ret_val_a != DG_TRDP_NO_ERR)
			{
				s_ret_val_a = trdp_pd_publish(HAVCTOCCU_COMIDA, 1, (DG_U8*)&s_havctoccudata_a, havctoccudatasize, &trdp_err_no, NULL);
				osl_printf("ret_val_a: %d, err_no: %d\n", ret_val, trdp_err_no);
				s_ret_val_a |= trdp_pd_subscribe(CCUTOALL_COMIDA, 1, &trdp_err_no, NULL);
				osl_printf("ret_val_a: %d, err_no: %d\n", s_ret_val_a, trdp_err_no);
				s_ret_val_a |= trdp_pd_subscribe(CCUTOHAVC_COMIDA, 1, &trdp_err_no, NULL);
				osl_printf("ret_val_a: %d, err_no: %d\n", s_ret_val_a, trdp_err_no);
				// save_other_log(110, (U8*)&ret_val_a,4);//不能在这里保存 因为这个不支持多线程
			}

			if (s_ret_val_b != DG_TRDP_NO_ERR)
			{
				s_ret_val_b = trdp_pd_publish(HAVCTOCCU_COMIDB, 1, (DG_U8*)&s_havctoccudata_b, havctoccudatasize, &trdp_err_no, NULL);
				osl_printf("ret_val_b: %d, err_no: %d\n", s_ret_val_b, trdp_err_no);
				s_ret_val_b |= trdp_pd_subscribe(CCUTOALL_COMIDB, 1, &trdp_err_no, NULL);
				osl_printf("ret_val_b: %d, err_no: %d\n", s_ret_val_b, trdp_err_no);
				s_ret_val_b |= trdp_pd_subscribe(CCUTOHAVC_COMIDB, 1, &trdp_err_no, NULL);
				osl_printf("ret_val_b: %d, err_no: %d\n", s_ret_val_b, trdp_err_no);
			}

			LOG_PRINT("trdp run...\r\n");
		}

		t = TimeGap(t);
		if (t >= 500)
		{
			PRINT_ERR_INT(t);
			continue;
		}
		sleep(500 - t);
	}

#if 0
	ret_val = trdp_pd_unsubscribe(CCUTOALL_COMIDA, 1, &trdp_err_no, NULL);
	osl_printf("RETURN: %d, ERROR: %d\n", ret_val, trdp_err_no);
	ret_val = trdp_pd_unsubscribe(CCUTOALL_COMIDB, 1, &trdp_err_no, NULL);
	osl_printf("RETURN: %d, ERROR: %d\n", ret_val, trdp_err_no);
	ret_val = trdp_pd_unsubscribe(CCUTOHAVC_COMIDA, 1, &trdp_err_no, NULL);
	osl_printf("RETURN: %d, ERROR: %d\n", ret_val, trdp_err_no);
	ret_val = trdp_pd_unsubscribe(CCUTOHAVC_COMIDB, 1, &trdp_err_no, NULL);
	osl_printf("RETURN: %d, ERROR: %d\n", ret_val, trdp_err_no);
	osl_printf("\n");

	ret_val = trdp_pd_unpublish(HAVCTOCCU_COMIDA, 1, &trdp_err_no, NULL);
	osl_printf("RETURN: %d, ERROR: %d\n", ret_val, trdp_err_no);
	ret_val = trdp_pd_unpublish(HAVCTOCCU_COMIDA, 1, &trdp_err_no, NULL);
	osl_printf("RETURN: %d, ERROR: %d\n", ret_val, trdp_err_no);
#endif // 0

	hdc_reset(0);

	return ret_val;
}

BOOL trdp_task_init(int wait_s)
{
	U32 time = 0;

	trdp_set_stat(TRDP_WAIT_15S);
	while (time < wait_s)
	{
		LOG_PRINT("trdp wait %d [%d]\r\n", time, s_car_idx);
		time += 1;
		sleep(1000);
	}

	trdp_set_stat(TRDP_INITING);
	int ret = trdp_config();

	return ret == 0;
}

#define TASK2_STACK_SIZE (1024 * 4)
static U32 task2_stack[TASK2_STACK_SIZE >> 2];

BOOL s_trdp_init_ret = FALSE;

void trdp_task(void*)
{
	//return ;
	int cnt = 10;
	int run_ret = 0;
	while (true)
	{
		cnt = 10;
		trdp_hw_reset();
		
		BOOL ret = s_trdp_init_ret =  trdp_task_init(20);
		// BOOL ret = trdp_task_init(15);//测试程序修改了15秒初始化TRDP网卡程序

#if 0
		if (!ret)
			ret = trdp_task_init(5);

		if (!ret) {
			for (int i = 0; i < 5; i++)
			{
				ret = trdp_task_init(1);
				if (ret) break;
			}
		}
#endif // 0

		if (ret)
		{
			BEEP(50);
			sleep(100);
			BEEP(50);
			trdp_set_stat(TRDP_OK);
			dg_udp_init();
			g_ptu_udp.dg_udp_info_reg(UDP_PTU_PORT);
			g_iap_udp.dg_udp_info_reg(UDP_IAP_PORT);
			g_debug_udp.dg_udp_info_reg(UDP_DEBUG_PORT);
			run_ret = trdp_run();
		}
		BEEP(500);
		trdp_set_stat(TRDP_ERROR);
		while (cnt-- > 0)
		{
			sleep(100);
			LOG_PRINT("[%d] TRDP ERROR %d ,run_ret:%d\r\n", cnt, ret, run_ret);
		}
	}
}

// car_num车厢号 1~8
void trdp_app_init(U8 car_num)
{
	init_sema(&s_lock, 1);

	if (car_num <= 0 || car_num > 8)
	{
		PRINT_ERR_INT(car_num);
		s_car_idx = 1;
		return;
	}

	s_car_idx = car_num - 1;

	LOG_PRINT("TRDP_COMM_DATA:%d\r\n", sizeof(TRDP_COMM_DATA));
	LOG_PRINT("TRDP_CCU2HVAC:%d\r\n", sizeof(TRDP_CCU2HVAC));
	LOG_PRINT("TRDP_HVAC2CCU:%d\r\n", sizeof(TRDP_HVAC2CCU));

#if 1
	s_stat = TRDP_RESET;
	int ret = create_thread(trdp_task, 0, (U32)task2_stack, TASK2_STACK_SIZE, ROOT_THREAD_PRIO);
	if (ret < 0)
		//BEEP(500);
		LOG_PRINT("create task2 thread ret=%d\r\n", ret);
#endif // 0
}

#define TRDP_DATA_TIME_OUT (500 * 4)
TRDP_TRUST_CHANNEL update_trdp_app_data(TRDP_TRUST_CHANNEL channel)
{
#ifndef _BOOT_
	static U32 s_update_time = 0;
	static TRDP_TRUST_CHANNEL channel_ret = CHANNEL_NULL;

	if (TimeGap(s_update_time) < 200)
		return channel_ret;
	s_update_time = sys_time();

	BOOL ret = TRUE;

	if (trdp_get_stat() != TRDP_OK)
	{
		channel_ret = CHANNEL_NULL;
		return channel_ret;
	}

	// LOG_PRINT_NET("trdp_get_stat ok channel:%d :%d\r\n",channel,havctoccudatasize);

	TRDP_LOCK();
	g_havctoccudata.HVACXs_Lifesign_U32 = hton32(HVACXs_Lifesign_U32);

	// LOG_PRINT("-------------------g_havctoccudata--------------\r\n");
	// memdump((U8*)& g_havctoccudata, sizeof(g_havctoccudata));

	// LOG_PRINT("-------------------g_ccutoalldata--------------\r\n");
	// memdump((U8*)& g_ccutoalldata, sizeof(g_ccutoalldata));

	// LOG_PRINT("-------------------g_ccutohavcdata--------------\r\n");
	// memdump((U8*)& g_ccutohavcdata, sizeof(g_ccutohavcdata));

	mem_copy((U8*)&s_havctoccudata_a_app, (U8*)&g_havctoccudata, havctoccudatasize);
	mem_copy((U8*)&s_havctoccudata_b_app, (U8*)&g_havctoccudata, havctoccudatasize);

	if (ret == FALSE)
	{
		channel_ret = CHANNEL_NULL;
	}

	if ((TimeGap(s_tick_ccutoall_a) <= TRDP_DATA_TIME_OUT || channel == CHANNEL_A) && (channel != CHANNEL_B))
	{
		mem_copy((U8*)&g_ccutoalldata, (U8*)&s_ccutoalldata_a_app, ccutoalldatasize);
		channel_ret = CHANNEL_A;
	}
	else if ((TimeGap(s_tick_ccutoall_b) <= TRDP_DATA_TIME_OUT || channel == CHANNEL_B) && (channel != CHANNEL_A))
	{
		//    PRINT_ERR_STR("ccutoall a timeout");
		mem_copy((U8*)&g_ccutoalldata, (U8*)&s_ccutoalldata_b_app, ccutoalldatasize);
		channel_ret = CHANNEL_B;
	}
	else
	{
		//   PRINT_ERR_STR("ccutoall a&b timeout");
		ret = FALSE;
	}

	if (((TimeGap(s_tick_ccutohvac_a) <= TRDP_DATA_TIME_OUT) || CHANNEL_A == channel) && (channel != CHANNEL_B))
	{
		//	LOG_PRINT_NET("ccutohvac sel A net\r\n");
		mem_copy((U8*)&g_ccutohavcdata, (U8*)&s_ccutohavcdata_a, ccutohavcdatasize);
		channel_ret = CHANNEL_A;
	}
	else if (((TimeGap(s_tick_ccutohvac_b) <= TRDP_DATA_TIME_OUT) || CHANNEL_B == channel) && (channel != CHANNEL_A))
	{
		//		LOG_PRINT_NET("ccutohvac sel B net\r\n");
		mem_copy((U8*)&g_ccutohavcdata, (U8*)&s_ccutohavcdata_b, ccutohavcdatasize);
		channel_ret = CHANNEL_B;
	}
	else
	{
		//	PRINT_ERR_STR("ccutohvac a&b timeout");
		ret = FALSE;
	}

	TRDP_UNLOCK();
	if (ret == FALSE)
	{
		// PRINT_ERR_STR("update_trdp_app_data fault");
		channel_ret = CHANNEL_NULL;
	}

	return channel_ret;
#endif // _BOOT_
}
