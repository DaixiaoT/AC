#ifndef _TRDP_UDP_H__
#define _TRDP_UDP_H__
#include "dg_ip.h"
#include "sys_api.h"

BOOL dg_udp_stat();
EXT_HANDLE_P dg_udp_handle();
static IP_S32 ADDR_LEN = sizeof(struct ip_sockaddr_in);


class DG_UDP
{
public:
	BOOL m_isInit;
	U16 m_port;
	IP_S32 m_sockd;
	struct ip_sockaddr_in m_remote_addr;

public:
	DG_UDP()
	{
		m_isInit=FALSE;
		m_port = 0;
		m_remote_addr.sin_addr.s_addr =0;
	}

	BOOL dg_udp_info_reg(U16 port)
	{
		
	}


	int check_udp_data(U8* buf, int len)
	{
		
		return 0;
	}

	int send_udp_data(U8* buf, int len)
	{
		
	}

};

#define UDP_PTU_PORT 6957
#define UDP_IAP_PORT 6956

#define UDP_DEBUG_PORT 16969

extern DG_UDP g_iap_udp;
extern DG_UDP g_ptu_udp;
extern DG_UDP g_debug_udp;
void dg_udp_init();

void LOG_PRINT_NET(const char *fmt, ...);

//BOOL dg_udp_info_reg(DG_UDP_INFO *udp_info);
//int check_udp_data(DG_UDP_INFO *udp_info,U8* buf, int len);
//int send_udp_data(DG_UDP_INFO *udp_info,U8* buf, int len);

#endif//_TRDP_UDP_H__









