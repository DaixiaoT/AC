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
		m_isInit = FALSE;
		m_port = 0;
		m_remote_addr.sin_addr.s_addr = 0;
	}

	BOOL dg_udp_info_reg(U16 port)
	{
		struct ip_sockaddr_in addr;

		IP_S32 status;
		IP_S32 dg_errno;
		if (!dg_udp_stat())
		{
			PRINT_ERR_INT(dg_udp_stat());
			return FALSE;
		}

		m_sockd = ip_socket(IP_AF_INET, IP_SOCK_DGRAM, 0, &dg_errno, dg_udp_handle());
		if (m_sockd < 0)
		{
			osl_printf("ip_socket() failed %d\n", dg_errno);
			return FALSE;
		}

		m_port = port;
		osl_printf("open socket %d\n", m_sockd);

		addr.sin_family = IP_AF_INET;
		addr.sin_addr.s_addr = IP_INADDR_ANY;
		addr.sin_port = ip_htons(m_port);

		status = ip_bind(m_sockd, (struct ip_sockaddr*)&addr, sizeof(struct ip_sockaddr), &dg_errno, dg_udp_handle());
		if (status < 0)
		{
			osl_printf("ip_bind() failed %d\n", dg_errno);
			ip_close(m_sockd, &dg_errno, dg_udp_handle());
			return FALSE;
		}

		m_isInit = TRUE;
		return m_isInit;
	}


	int check_udp_data(U8* buf, int len)
	{
		IP_S32 status;
		IP_S32 dg_errno;
		if (!dg_udp_stat() || (!m_isInit))
			return 0;

		struct ip_sockaddr_in remote_addr;
		status = ip_recvfrom(m_sockd, buf, len, 0, (struct ip_sockaddr*)&remote_addr, &ADDR_LEN, &dg_errno, dg_udp_handle());
		if (status > 0)
		{
			//recvdata[status] = 0;

			mem_copy((U8*)&m_remote_addr, (U8*)&remote_addr, sizeof(ip_sockaddr_in));

			return status;
		}

		return 0;
	}

	int send_udp_data(U8* buf, int len)
	{
		if (!dg_udp_stat() || (!m_isInit))
			return 0;
		struct ip_sockaddr_in remote_addr;
		IP_S32 dg_errno;
		mem_copy((U8*)&remote_addr, (U8*)&m_remote_addr, sizeof(struct ip_sockaddr_in));

		if (remote_addr.sin_addr.s_addr == 0)
			return 1;

		return ip_sendto(m_sockd, buf, len, 0, (struct ip_sockaddr*)&remote_addr, ADDR_LEN, &dg_errno, dg_udp_handle());
	}

};

#define UDP_PTU_PORT 6957
#define UDP_IAP_PORT 6956

#define UDP_DEBUG_PORT 16969

extern DG_UDP g_iap_udp;
extern DG_UDP g_ptu_udp;
extern DG_UDP g_debug_udp;
void dg_udp_init();

void LOG_PRINT_NET(const char* fmt, ...);

//BOOL dg_udp_info_reg(DG_UDP_INFO *udp_info);
//int check_udp_data(DG_UDP_INFO *udp_info,U8* buf, int len);
//int send_udp_data(DG_UDP_INFO *udp_info,U8* buf, int len);

#endif//_TRDP_UDP_H__









