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
static BOOL s_udp_init=FALSE;

BOOL task_udp_init()
{
	

	return TRUE;
}

#ifdef _DEBUG
BOOL dg_udp_info_reg(DG_UDP_INFO *udp_info)
{
	
	return TRUE;
}
#endif // _DEBUG


void dg_udp_init()
{

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
int check_udp_data(DG_UDP_INFO *udp_info,U8* buf,int len)
{
	
	return 0;
}

int send_udp_data(DG_UDP_INFO *udp_info,U8* buf,int len)
{
	
}




#include "trdp_udp.h"
#define NET_DEBUG_BUF_LEN   2048
static U8 net_debug_buf[NET_DEBUG_BUF_LEN];
static BOOL s_debug_net = FALSE;
void reboot();
void net_debug_check(){
	
}

static inline int debug_udp_send(U8 *buf, int len)
{
	
}

#include <stdarg.h>
#include <stddef.h>
extern "C" int vsnprintf(char *buffer, size_t n, const char *format, va_list ap);
#ifdef _TOS_
static sema_t s_print_lock;
#endif
void LOG_PRINT_NET(const char *fmt, ...)
{
	
}








#endif // _DEBUG