
/* ==========================================================================
 *
 *  File      : DG_IP_G.H
 *
 *  Purpose   : Common part for Socket API for UART emulation 2G (Host/Device)
 *
 *
 *  Licence   : Duagon Software Licence (see file 'licence.txt')
 *
 * --------------------------------------------------------------------------
 *
 *  (C) COPYRIGHT, Duagon AG, CH-8953 Dietikon, Switzerland
 *  All Rights Reserved.
 *
 * ==========================================================================
 */
#ifndef DG_IP_G_H
#define DG_IP_G_H


/* ==========================================================================
 *
 *  Include Files
 *
 * ==========================================================================
 */

#include "os_def.h"

/* --------------------------------------------------------------------------
 *  8-bit data types
 * --------------------------------------------------------------------------
 */
typedef DG_S8 IP_S8;
typedef DG_U8 IP_U8;
typedef DG_CHAR8 IP_CHAR8;


/* --------------------------------------------------------------------------
 *  16-bit data types
 * --------------------------------------------------------------------------
 */
typedef DG_U16 IP_U16;
typedef DG_S16 IP_S16;

/* --------------------------------------------------------------------------
 *  32-bit data types
 * --------------------------------------------------------------------------
 */
typedef DG_S32 IP_S32;
typedef DG_U32 IP_U32;



/* --------------------------------------------------------------------------
 *  special data types
 * --------------------------------------------------------------------------
 */

/* --------------------------------------------------------------------------
 * Commands used to transfer socket functions on host to duagon device
 */
enum {
    CMD_IP_SOCKET_CHECK = 0,        /* rpc internal usage: must be 0 */
    CMD_IP_SOCKET_PING = 1,         /* ip test request to device */
    CMD_IP_INIT = 2,                /* ip_init() */
    CMD_IP_SOCKET = 3,              /* ip_socket() create a new socket */
    CMD_IP_SETSOCKOPT = 4,          /* ip_setsockopt() */
    CMD_IP_GETSOCKOPT = 5,          /* ip_getsockopt() */
    CMD_IP_BIND = 6,                /* ip_bind() binds socket to a address */
    CMD_IP_CLOSE = 7,               /* ip_close(); closes a socket */
    CMD_IP_CONNECT = 8,             /* ip_connect(); connects a socket */
    CMD_IP_LISTEN = 9,              /* ip_listen(); set socket passive */
    CMD_IP_ACCEPT = 10,             /* ip_accept(); accepts new request */
    CMD_IP_SEND = 11,               /* ip_send() send data */
    CMD_IP_SENDTO = 12,             /* ip_sendto() (udp) */
    CMD_IP_RECV = 13,               /* ip_recv() data */
    CMD_IP_RECVFROM = 14,           /* ip_recvto() (udp) */
    CMD_IP_GETHOSTBYNAME = 15,      /* ip_gethostbyname() */
    CMD_IP_GETHOSTBYADDR = 16,      /* ip_gethostbyaddr() */
    CMD_IP_IOCTL = 17,              /* ip_fcntl(); set io option such as nonblocking */
    CMD_IP_SELECT = 18,             /* ip_select(); selects active file disciptors */
    CMD_IP_GETHOSTNAME = 19,        /* ip_gethostbyname(); gets the hostname from the device */
    CMD_IP_CONV_TPC_PASSIVE = 20,   /* ip_conv_tcp_passive(); tcp socket, bind and listen */
    CMD_IP_CONV_UDP_PASSIVE = 21,   /* ip_conv_tcp_passive(); udp socket, bind */
    CMD_IP_RECVFROM_GET_LENGTH = 22,/* ip_recvfrom() get packet length */
    CMD_IP_RECVFROM_GET_DATA = 23,  /* ip_recvfrom() get packet data */
    CMD_IP_LAST                 /* mark last element */
};


/*---------------------------------------------------------------------------
 * Common socket defines
 */
#define IP_AF_INET         2        /* internetwork: UDP, TCP, etc. */

/* Socket types. */
#define IP_SOCK_STREAM  1           /* stream (connection) socket   */
#define IP_SOCK_DGRAM   2           /* datagram (conn.less) socket  */


/*---------------------------------------------------------------------------
 * defines used by ip_ioctl
 */

/* non blocking I/O */
#define IP_FIONBIO 1

/*---------------------------------------------------------------------------
 * defines used with ip_setsockopt(), ip_getsockopt
 */

#define IP_SOL_IP          0x0000      /* options for socket level IP */
#define IP_SOL_SOCKET      0xffff      /* options for socket level */

/* IP level options */
#define IP_SO_DIFFSERV          0x0003   /* aka. IP_TOS*/
#define    IP_SO_TTL               0x0004   /* IP time to live */
#define    IP_SO_MULTICAST_IF      0x0009   /* set/get IP multicast i/f  */
#define    IP_SO_MULTICAST_TTL     0x000A   /* set/get IP multicast ttl */
#define    IP_SO_MULTICAST_LOOP    0x000B   /* set/get IP multicast loopback */
#define    IP_SO_ADD_MEMBERSHIP    0x000C   /* struct ip_mreq; add an IP group membership */
#define    IP_SO_DROP_MEMBERSHIP    0x000D   /* struct ip_mreq; drop an IP group membership */

/* socket level options */
#define    IP_SO_REUSEADDR       0x0004      /* allow local address reuse */
#define IP_SO_KEEPALIVE    0x0008      /* keep connections alive */
#define IP_SO_BROADCAST    0x0020      /* permit sending of broadcast msgs */

/* additional options */
#define IP_SO_SNDBUF    0x1001        /* send buffer size */
#define IP_SO_RCVBUF    0x1002        /* receive buffer size */
#define IP_SO_SNDLOWAT    0x1003        /* send low-water mark */
#define IP_SO_RCVLOWAT    0x1004        /* receive low-water mark */
#define IP_SO_SNDTIMEO    0x1005        /* send timeout */
#define IP_SO_RCVTIMEO    0x1006        /* receive timeout */
#define    IP_SO_ERROR        0x1007        /* get error status and clear */
#define    IP_SO_TYPE        0x1008        /* get socket type */


/*---------------------------------------------------------------------------
 * Defines used with ip_gethostbyname() and ip_gethostbyaddr()
 *
 * Error return codes from ip_gethostbyname() and ip_gethostbyaddr()
 */

/* Authoritative Answer Host not found */
#define IP_HOST_NOT_FOUND 1
/* Non-Authoritive Host not found, or SERVERFAIL */
#define IP_TRY_AGAIN      2
/* Non recoverable errors, FORMERR, REFUSED, NOTIMP */
#define IP_NO_RECOVERY    3
/* Valid name, no data record of requested type */
#define IP_NO_DATA        4

/*
 * Error return codes from gethostbyname() and gethostbyaddr()
 * on device this error is in h_error.
 * on host this error is in ip_h_error.
*/

#define IP_HOSTENT_FAILED 100       /* call failed because so data did not fit into memory */
#define IP_HOSTENT_WARNING 101      /* call ok but some addresses or alias did not fit and is not in result */


/*---------------------------------------------------------------------------
 * different defines
 */
/* Flags we can use with send/ and recv. */
#define IP_MSG_PEEK         0x2


#endif /* DG_IP_G_H */
