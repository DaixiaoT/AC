#ifndef _MIIP_UDP_H__
#define _MIIP_UDP_H__
#ifdef _MIIP_
#include "sys_api.h"
#include "miIP.h"

BOOL miip_udp_stat();


class MIIP_UDP
{
public:
	BOOL m_isInit;
	U16 m_port;
	U32 m_udp_handle;
	ip_addr_t from_ip;
	U16 from_port;
	netif_t *m_nif;
public:
	MIIP_UDP()
	{
		m_isInit=FALSE;
		m_udp_handle = 0;
		m_port = 0;
		from_port = 0;
		from_ip.addr = 0;
		m_nif = NULL;
	}

	BOOL miip_udp_info_reg(U16 port,netif_t *nif=0)
	{
		if(m_isInit)
		{
			if(m_udp_handle)
				udp_close(m_udp_handle);
			m_udp_handle = 0;
			m_isInit=FALSE;
		}
		int ret = udp_open(&m_udp_handle);
		if(ret < 0)
		{
			LOG_PRINT("udp_open err=%d\r\n", ret);
			return FALSE;
		}
		
		LOG_PRINT("udp handle: %d ,port:%d\r\n", m_udp_handle,port);
		
		ret = udp_bind_if(m_udp_handle, 0, port,nif);
		if(ret)
		{
			LOG_PRINT("udp_bind err=%d\r\n", ret);
			return FALSE;
		}

		m_nif = nif;
		m_isInit = TRUE;
		return m_isInit;
	}


	int check_udp_data(U8* buf, int len)
	{
		if(!miip_udp_stat() || (!m_isInit))
		{
//			PRINT_ERR_STR("!m_isInit");
			return -1;
		}

		int rx_len = udp_recvfrom(m_udp_handle, buf, len, &from_ip, &from_port);

		if(rx_len <= 0){
			if(rx_len != NE_EMPTY_QUEUE)
				PRINT_ERR_INT(rx_len);
			return 0;
		}

		return rx_len;
	}

	int send_udp_data(U8* buf, int len)
	{
		if(!miip_udp_stat()|| (!m_isInit))
			return -2;
		
		if(from_ip.addr == 0)
			return -1;

		return udp_sendto_if(m_udp_handle, buf, len, &from_ip, from_port,m_nif);
	}


	int send_udp_data_to_host(U8* buf, int len, U32 addr, U16 port)
	{
		if(!miip_udp_stat() || (!m_isInit))
			return 0;

		if(addr == 0)
		{
			PRINT_ERR_INT(addr);
			return -1;
		}

		if (addr)
			from_ip.addr = addr;

		if (port)
			from_port = port;

		return udp_sendto_if(m_udp_handle, buf, len, &from_ip, from_port,m_nif);
	}
};

#define UDP_PTU_PORT 6957
#define UDP_IAP_PORT 6956
#define UDP_DEBUG_PORT 16969

extern MIIP_UDP g_ptu_udp;

void miip_udp_init();
#endif // _MIIP_


#endif//_MIIP_UDP_H__









