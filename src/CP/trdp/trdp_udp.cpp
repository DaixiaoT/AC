#include "sys_api.h"
#include "trdp_udp.h"
#include "dg_trdp.h"
#include "os_def.h"
#include "dg_error.h"
#include "dg_svc.h"
#include <string.h>
#include "dg_ip.h"
#include "dg_iperr.h"



// #define TASK_UDP_STACK_SIZE (1024*4)
// static U32 task1_stack[TASK_UDP_STACK_SIZE >> 2];

#define osl_printf LOG_PRINT



DG_UDP g_ptu_udp;
DG_UDP g_iap_udp;
DG_UDP g_debug_udp;

// #define UDP_PORT 6957
// static struct ip_sockaddr_in s_remote_addr;
// static IP_S32 sockd;



static DG_EXT_HANDLE card_handle = INIT_EXT_HANDLE;
static EXT_HANDLE_P pCard_handle = &card_handle;
static BOOL s_udp_init = FALSE;

BOOL task_udp_init()
{
	struct ip_sockaddr_in addr;
	IP_S32 status;
	IP_S32 dg_errno;
	//s_remote_addr.sin_addr.s_addr = 0;
	DG_U8 card_number = 0;

	osl_printf("card number %d\n", card_number);
	card_init_handle(card_number, pCard_handle);

	/* IP Socket API init */
	while (ip_init(&dg_errno, pCard_handle) == -1)
	{
		switch (dg_errno)
		{
		case DG_ERROR_OSL_NOT_READY:
			osl_printf("REGISTERS not ready\n");
			break;

		case DG_ERROR_EMPTY_BLOCK:
			osl_printf("PROTOCOL not ready\n");
			break;

		case DG_ERROR_OSL_INIT:
			osl_printf("I/O access initialization failed\n");
			osl_printf("is SYS device running? (GiveIO or OLS)\n");
			return FALSE;

		default:
			osl_printf("ip_init failed with error: %d\n", dg_errno);
			return FALSE;
		}
	}
	osl_printf(" Starting nonblocking UDP Server\n");



	return TRUE;
}

#ifdef _DEBUG
BOOL dg_udp_info_reg(DG_UDP_INFO* udp_info)
{
	if (!dg_udp_stat())
	{
		PRINT_ERR_INT(dg_udp_stat());
		return FALSE;
	}

	udp_info->m_sockd = ip_socket(IP_AF_INET, IP_SOCK_DGRAM, 0, &dg_errno, pCard_handle);
	if (udp_info->m_sockd < 0)
	{
		osl_printf("ip_socket() failed %d\n", dg_errno);
		return FALSE;
	}

	osl_printf("open socket %d\n", udp_info->m_sockd);

	addr.sin_family = IP_AF_INET;
	addr.sin_addr.s_addr = IP_INADDR_ANY;
	addr.sin_port = ip_htons(udp_info->port);

	status = ip_bind(udp_info->m_sockd, (struct ip_sockaddr*)&addr, sizeof(struct ip_sockaddr), &dg_errno, pCard_handle);
	if (status < 0)
	{
		osl_printf("ip_bind() failed %d\n", dg_errno);
		ip_close(udp_info->m_sockd, &dg_errno, pCard_handle);
		return FALSE;
	}

	return TRUE;
}
#endif // _DEBUG


void dg_udp_init()
{
	// 	int ret = create_thread(task_udp, 0, (U32)task1_stack, TASK_UDP_STACK_SIZE, ROOT_THREAD_PRIO);
	// 	if(ret < 0)
	// 		LOG_PRINT("create task_udp thread ret=%d\r\n", ret);
	s_udp_init = task_udp_init();

	LOG_PRINT("dg_udp_init() %d\r\n", s_udp_init);
}

BOOL dg_udp_stat()
{
	return s_udp_init;
}

EXT_HANDLE_P dg_udp_handle()
{
	return pCard_handle;
}

#ifdef _DEBUG
int check_udp_data(DG_UDP_INFO* udp_info, U8* buf, int len)
{
	IP_S32 status;
	IP_S32 dg_errno;
	if (!dg_udp_stat())
		return 0;

	struct ip_sockaddr_in remote_addr;
	status = ip_recvfrom(udp_info->m_sockd, buf, len, 0, (struct ip_sockaddr*)&remote_addr, &ADDR_LEN, &dg_errno, pCard_handle);
	if (status > 0)
	{
		//recvdata[status] = 0;

		mem_copy((U8*)&udp_info->remote_addr, (U8*)&remote_addr, sizeof(ip_sockaddr_in));

		return status;
	}


	return 0;
}

int send_udp_data(DG_UDP_INFO* udp_info, U8* buf, int len)
{
	if (!dg_udp_stat())
		return 0;
	struct ip_sockaddr_in remote_addr;
	IP_S32 dg_errno;
	mem_copy((U8*)&remote_addr, (U8*)&udp_info->remote_addr, sizeof(ip_sockaddr_in));

	if (remote_addr.sin_addr.s_addr == 0)
		return 1;

	return ip_sendto(udp_info->m_sockd, buf, len, 0, (struct ip_sockaddr*)&remote_addr, ADDR_LEN, &dg_errno, pCard_handle);

}




#include "trdp_udp.h"
#define NET_DEBUG_BUF_LEN   2048
static U8 net_debug_buf[NET_DEBUG_BUF_LEN];
static BOOL s_debug_net = FALSE;
void reboot();
void net_debug_check() {
	U8* buf = net_debug_buf;

	if (g_debug_udp.check_udp_data(buf, NET_DEBUG_BUF_LEN) > 0) {
		LOG_PRINT("g_debug_udp recv");
		memdump(buf, 16);

		if (buf[0] == '1' &&
			buf[1] == '2' &&
			buf[2] == '3' &&
			buf[3] == '4' &&
			buf[4] == '6')
		{
			LOG_PRINT_NET("net test open\r\n");
			s_debug_net = TRUE;
		}
		else if ((s_debug_net == TRUE) &&
			buf[0] == 'h' &&
			buf[1] == 'c' &&
			buf[2] == '7' &&
			buf[3] == '9' &&
			buf[4] == '8') {
			save_other_log(11, buf, 5);
			LOG_PRINT_NET("hvac reboot net\r\n");
			LOG_PRINT("hvac reboot net\r\n");
			WDEX_FEED();
			sleep(1000);
			reboot();
		}
		else
		{
			s_debug_net = FALSE;
		}
	}
}

static inline int debug_udp_send(U8* buf, int len)
{
	if (s_debug_net)
		return g_debug_udp.send_udp_data(buf, len);
	return -1;
}

#include <stdarg.h>
#include <stddef.h>
extern "C" int vsnprintf(char* buffer, size_t n, const char* format, va_list ap);
#ifdef _TOS_
static sema_t s_print_lock;
#endif
void LOG_PRINT_NET(const char* fmt, ...)
{
	U8* buf = net_debug_buf;
	va_list a;

	if (!s_debug_net) {
		va_start(a, fmt);
		vsnprintf((char*)buf, NET_DEBUG_BUF_LEN, &fmt[0], a);
		va_end(a);

		int len = str_len((char*)buf);
		if (len > 0) {
			Uart0_Write((char*)buf, len);
		}

		return;
	}


#ifdef _TOS_
	p_sema(&s_print_lock, -1);
#endif

	va_start(a, fmt);
	vsnprintf((char*)buf, NET_DEBUG_BUF_LEN, &fmt[0], a);
	va_end(a);

	int len = str_len((char*)buf);
	if (len > 0) {
		debug_udp_send((U8*)buf, len);
	}



#ifdef _TOS_
	v_sema(&s_print_lock);
#endif
}








#endif // _DEBUG