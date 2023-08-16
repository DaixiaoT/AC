/************************************************************************//**
 *  \file       dg_ip.h
 *
 *  \brief      Function declarations and type definitions of
 *              Socket module host interface.
 *
 *  \defgroup   SOCKET_HOST Socket Host Driver
 *  @{
 ****************************************************************************
 *  Licence : duagon Software Licence (see file 'licence.txt')
 ****************************************************************************
 *  (C) COPYRIGHT, duagon AG, CH-8953 Dietikon, Switzerland
 *  All Rights Reserved.
 ***************************************************************************/

#ifndef DG_IP_H
#define DG_IP_H


/****************************************************************************
    Documentation
*****************************************************************************

Introduction
============

Sockets provide a simple interface to access the TCP/IP protocol stack running 
on a duagon Ethernet interface. On the host system, the application can use
the sockets through the host driver.

This document describes the features and the API of the TCP and UDP sockets on 
duagon Ethernet interfaces. The API provides a subset of the BSD TCP/IP stack
functions for accessing sockets.

Real time handle
================

For most functions of this API, there exist two ways to call the function:

1) Run to completion: The function returns after the result is completely
evaluated. This can take some time, because first the parameters are 
transferred to the device, then the function is executed on the device and 
finally the result is transferred back to the host.
-> To run functions to completion, set rt_handle NULL

2) Split transaction: The function returns immediately after the parameters
are transferred to the device. The first call to the function provides a 
handle (real time handle). The function must be polled with this handle to 
detect if the result of the function is available. The host can do some other 
work between polling the function. 

-> To run functions with real time handle:
   -initialise a DG_EXT_HANDLE variable with rtm_init_handle()
   -repeat calling the function with this handle, until rtm_isFinished()
    returns TRUE
   -evaluate the return parameters

   Example for API XYZ:

   #include "dg_rtm.h"
   DG_EXT_HANDLE rt_handle;
   XYZ_S32 xyz_errno;
   rtm_init_handle(&rt_handle);
   while (1)
   {
       xyz_function(...,...,&xyz_errno,&rt_handle);
       if ( rtm_isFinished(&rt_handle) )
       {
           //TODO: check xyz_errno
           //TODO: process result
           rtm_init_handle(&rt_handle);
       }
       else
       {
           //TODO: do other activities
       }
   }

****************************************************************************/


/* ==========================================================================
 *
 *  Include Files
 *
 * ==========================================================================
 */

#include "os_def.h"
#include "dg_ip_g.h"
#include "dg_iperr.h"
#include "dg_rtm.h"

#ifndef IP_DEBUG
#define IP_DEBUG 0
#endif

/*---------------------------------------------------------------------------
 * some defines are in dg_ip_g.h
 */
#define SOCKET_PROTOCOL_ID 0x02  /* new protocol id */

/*---------------------------------------------------------------------------
 * datatypes and defines used with ip_sockaddr
 */

/** IP v4 address size is 16 Byte. */
#define IP_INET_ADDRSTRLEN 16

/** Address to accept any incoming messages. */
#define IP_INADDR_ANY       ((IP_U32) 0x00000000)

#undef s_addr /* needed for windows */

/* Internet address. */
struct ip_in_addr
{
    IP_U32    s_addr;
};

 /*
  * Argument structure for IP_ADD_MEMBERSHIP and IP_DROP_MEMBERSHIP.
  */
struct ip_mreq_dg
{
    struct    ip_in_addr imr_multiaddr;    /* IP multicast address of group */
    struct    ip_in_addr imr_interface;    /* local IP address of interface */
};

struct ip_sockaddr 
{
  IP_U16        sa_family;      /* address family, AF_xxx   */
  IP_U8         sa_data[30];    /* 30 bytes of protocol address */
};

struct ip_sockaddr_in
{
  IP_U16     sin_family;      /* Address family   */
  IP_U16     sin_port;        /* Port number      */
  struct ip_in_addr sin_addr; /* Internet address */

  /* Pad to size of `struct sockaddr'. */
  IP_U8 __pad[sizeof(struct ip_sockaddr) - sizeof(IP_S16) -
                      sizeof(IP_S16) - sizeof(struct ip_in_addr)];
};


/*
 * used by ip_gethostbyname and ip_gethostbyaddr
 */
struct ip_hostent
{
    IP_CHAR8 *hname;
    IP_CHAR8 **h_aliases;
    IP_S32   h_addrtype;
    IP_S32   h_length;
    IP_CHAR8 **h_addr_list;
};

/** Base value of h_addr_list */
#define ip_h_addr h_addr_list[0]

/*---------------------------------------------------------------------------
 * Defines and datatypes used for select
 */

/*
 * defines IP_FD_SET manipulation
 * this is used with ip_select
 * we support 64 Bits (2 * 32)
 */

/** Mark socket active macro. */
#define IP_FD_SET(n, p)     ((p)->fdsbits[(n)/32] |= (1L << ((n) % 32)))
/** Clear socket active flag macro. */
#define IP_FD_CLR(n, p)     ((p)->fdsbits[(n)/32] &= ~(1L << ((n) % 32)))
/** Check whether socket is active macro. */
#define IP_FD_ISSET(n, p)   ((p)->fdsbits[(n)/32] & (1L << ((n) % 32)))
/** Initialize fd_set macro. */
#define IP_FD_ZERO(p)       (p)->fdsbits[0]=0;(p)->fdsbits[1]=0;

/** FD Bits size in memory */
#define IP_FD_SETSIZE (2 * sizeof(IP_S32))

/** The fd_set structure is used by select() function, to place sockets into a "set" for various purposes,
 *  such as testing a given socket for readability using the readfds parameter of the select() function.
 *
 *  Do not access fdsbits directly! Use given macros.
 */
typedef struct _ip_fd_set
{
    IP_S32   fdsbits[2]; /* 64 bits */
} ip_fd_set;

/*
 * used with ip_select
 */
struct ip_timeval
{
    IP_S32 tv_sec;
    IP_S32 tv_usec;
};

/*---------------------------------------------------------------------------
 * different defines
 */

/** Maximum hostname length. */
#define IP_HOST_NAME_MAX    255

/** Maximum ip data length. */
#define IP_DATA_SIZE_MAX    65536

/* ==========================================================================
 *
 *  Macros
 *
 * ==========================================================================
 */

/* --------------------------------------------------------------------------
 *  Macros for declaration of variables and procedures.
 *  NOTE:
 *  extern "C" is used in mixed C/C++ headers to force C linkage on an
 *  external definition.
 * --------------------------------------------------------------------------
 */
#define IP_DECL_PUBLIC DG_DECL_PUBLIC
#define IP_DECL_LOCAL DG_DECL_LOCAL

// #ifndef NULL
//  #define NULL (void*)0
// #endif

/* ==========================================================================
 *
 *  Public Global Variables
 *
 * ==========================================================================
 */
IP_DECL_PUBLIC OSL_MUTEX  ip_protocol_mutex; 
IP_DECL_PUBLIC IP_U8  ip_mutex_init_done;

/* ==========================================================================
 *
 *  Public Procedure Interface
 *
 * ==========================================================================
 */


/**
* \brief    Initialization of the communication channel between host and duagon interface.
*
* @param[out]       ip_errno        Error code.
* @param[in,out]    rt_handle       Realtime and card info Handle.
*
* @retval    DG_ENOERR(0)       If successful.
* @retval    DG_EGENERAL(-1)    If error occurred (see ip_errno for details).
*
* \remark    This function must be called once at start-up.
*
*/
IP_DECL_PUBLIC
IP_S32
ip_init
(
    IP_S32 *ip_errno,
    RTM_HANDLE_P rtHandle
);


/**
* \brief    Create an endpoint for communication and returns a descriptor.
*
* @param[in]        domain       Defines protocol family: must be #IP_AF_INET (ARPA Internet Protocols).
* @param[in]        type         Defines semantics of communication: must be #IP_SOCK_STREAM (TCP) or #IP_SOCK_DGRAM (UDP).
* @param[in]        protocol     Specifies particular protocol: must be 0(automatic selection).
* @param[out]       ip_errno     Error code.
* @param[in,out]    rt_handle    Realtime and card info Handle.
*
* @return    Upon successful completion, ip_socket() returns a non-negative
*            integer, the socket file descriptor. Otherwise, a value of -1
*            be returned and *ip_errno set to indicate the error.
*
*/
IP_DECL_PUBLIC
IP_S32
ip_socket
(
    IP_S32 domain,
    IP_S32 type,
    IP_S32 protocol,
    IP_S32 *ip_errno,
    RTM_HANDLE_P rtHandle
);



/**
* \brief    Sets options on a socket.
*
* @param[in]        sockDesc     Socket descriptor returned by ip_socket().
* @param[in]        level        Level at which the option resides. Must be: #IP_SOL_SOCKET (socket level) or #IP_SOL_IP (IP level).
* @param[in]        optName      Name of the option to adjust. The following options are supported:
*                                - #IP_SO_KEEPALIVE (enables keep connections alive)
*                                - #IP_SO_BROADCAST (enables permission to transmit broadcast messages)
*                                - #IP_SO_DIFFSERV (set the differentiated service field used by IP headers on this socket, requires level #IP_SOL_IP)
* @param[in]        pOptVal      Pointer to the parameter value.
* @param[in]        optLen       Length of the parameter value.
* @param[out]       ip_errno     Error code.
* @param[in,out]    rtHandle     Realtime and card info Handle.
*
* @retval    DG_ENOERR(0)       If successful.
* @retval    DG_EGENERAL(-1)    If error occurred (see ip_errno for details).
*
*/
IP_DECL_PUBLIC
IP_S32
ip_setsockopt
(
    IP_S32 sockDesc,
    IP_S32 level,
    IP_S32 optName,
    const void *pOptVal,
    IP_S32 optLen,
    IP_S32 *ip_errno,
    RTM_HANDLE_P rtHandle
);


/**
 * \brief    Gets options on a socket.
 *
 * @param[in]        sockDesc     Socket descriptor returned by ip_socket().
 * @param[in]        level        Level at which the option resides. Must be: #IP_SOL_SOCKET (socket level) or #IP_SOL_IP (IP level).
 * @param[in]        optName      Name of the option. The following options are supported:
 *                                - #IP_SO_KEEPALIVE (enables keep connections alive)
 *                                - #IP_SO_BROADCAST (enables permission to transmit broadcast messages)
 *                                - #IP_SO_SNDBUF (get buffer size for output)
 *                                - #IP_SO_RCVBUF (get buffer size for input)
 *                                - #IP_SO_ERROR (get and clear error on the socket)
 *                                - #IP_SO_DIFFSERV (get the differentiated service field used by IP headers on this socket, requires level #IP_SOL_IP)
 * @param[out]       optVal       Pointer to the parameter value.
 * @param[out]       optLen       Pointer to the length of the parameter value.
 * @param[out]       ip_errno     Error code.
 * @param[in,out]    rtHandle     Realtime and card info Handle.
 *
 * @retval    DG_ENOERR(0)       If successful.
 * @retval    DG_EGENERAL(-1)    If error occurred (see ip_errno for details).
 *
 */
IP_DECL_PUBLIC
IP_S32
ip_getsockopt
(
    IP_S32 sockDesc,
    IP_S32 level,
    IP_S32 optName,
    void   *optVal,
    IP_S32 *optLen,
    IP_S32 *ip_errno,
    RTM_HANDLE_P rtHandle
);


/**
 * \brief    Binds an address and a port number to a socket.
 *
 * @param[in]        sockDesc         Socket descriptor returned by ip_socket().
 * @param[in]        pSockAddr        Pointer to a socket address structure containing address and port number.
 * @param[out]       sockAddrLen      Size of the ip_sockaddr structure.
 * @param[out]       ip_errno         Error code.
 * @param[in,out]    rtHandle         Realtime and card info Handle.
 *
 * @retval    DG_ENOERR(0)       If successful.
 * @retval    DG_EGENERAL(-1)    If error occurred (see ip_errno for details).
 *
 */
IP_DECL_PUBLIC
IP_S32
ip_bind
(
    IP_S32 sockDesc,
    struct ip_sockaddr *pSockAddr,
    IP_S32 sockAddrLen,
    IP_S32 *ip_errno,
    RTM_HANDLE_P rtHandle
);


/**
* \brief    Closes a socket descriptor.
*
* @param[in]        sockDesc     Socket descriptor returned by ip_socket().
* @param[out]       ip_errno     Error code.
* @param[in,out]    rtHandle    Realtime and card info Handle.
*
* @retval    DG_ENOERR(0)       If successful.
* @retval    DG_EGENERAL(-1)    If error occurred (see ip_errno for details).
*
*/
IP_DECL_PUBLIC
IP_S32
ip_close
(
    IP_S32 sockDesc,
    IP_S32 *ip_errno,
    RTM_HANDLE_P rtHandle
);


/**
* \brief    Initiates a connection on a socket.
*
*           For IP_SOCK_DGRAM sockets, the peer address identifies where all datagrams are sent on subsequent
*           send() functions, and limits the remote sender for subsequent ip_recv() functions.
*
*           If the initiating socket is connection-mode, then connect()attempts to establish
*           a connection to the address specified by the address argument.
*
*           If the connection cannot be established immediately and file descriptor is not set nonblocking, ip_connect()
*           blocks for up to an unspecified timeout interval until the connection is established.
*
* @param[in]        sockDesc         Socket descriptor returned by ip_socket().
* @param[in]        pSockAddr        Points to a ip_sockaddr structure containing the peer address. The length and format
*                                    of the address depend on the address family of the socket.
* @param[in]        sockAddrLen      Size of the ip_sockaddr structure.
* @param[out]       ip_errno         Error code.
* @param[in,out]    rtHandle         Realtime and card info Handle.
*
* @retval    DG_ENOERR(0)       If successful.
* @retval    DG_EGENERAL(-1)    If error occurred (see ip_errno for details).
*
*/
IP_DECL_PUBLIC
IP_S32
ip_connect
(
    IP_S32 sockDesc,
    struct ip_sockaddr *pSockAddr,
    IP_S32 sockAddrLen,
    IP_S32 *ip_errno,
    RTM_HANDLE_P rtHandle
);


/**
* \brief    Listens for socket connections and limit the queue of incoming connections.
*
*           The listen() function marks a connection-mode socket, specified by the socket argument, as accepting connections.
*
* @param[in]        sockDesc     Socket descriptor returned by ip_socket().
* @param[in]        backlog      Maximum number of connection requests that may be pending on the socket. Must be <= 16.
* @param[out]       ip_errno     Error code.
* @param[in,out]    rtHandle     Realtime and card info Handle.
*
* @retval    DG_ENOERR(0)       If successful.
* @retval    DG_EGENERAL(-1)    If error occurred (see ip_errno for details).
*
*
*/
IP_DECL_PUBLIC
IP_S32
ip_listen
(
    IP_S32 sockDesc,
    IP_S32 backlog,
    IP_S32 *ip_errno,
    RTM_HANDLE_P rtHandle
);


/**
* \brief    Accept a connection on a socket.
*
*            The accepted socket cannot itself accept more connections. The original socket remains open and can accept more
*            connections.
*
*            If the listen queue is empty of connection requests and file descriptor is not set nonblocking for the socket,
*            ip_accept() blocks until a connection is present.
*
*            If the ip_listen() queue is empty of connection requests and  the file descriptor is nonblocking for the socket,
*            ip_accept() fails and set *ip_errno to [IP_EAGAIN].
*
* @param[in]        sockDesc         Socket descriptor returned by ip_socket().
* @param[out]       pSockAddr        A pointer to a sockaddr structure where the address of the connecting socket be returned
*                                    of the address depend on the address family of the socket.
* @param[out]       sockAddrLen      Pointer to the size of the ip_sockaddr structure.
* @param[out]       ip_errno         Error code.
* @param[in,out]    rtHandle         Realtime and card info Handle.
*
* @return    Upon successful completion, ip_accept() returns a non-negative file descriptor of the accepted socket.
*            Otherwise, -1 be returned and *ip_errno set to indicate the error.
*
*/
IP_DECL_PUBLIC
IP_S32
ip_accept
(
    IP_S32 sockDesc,
    struct ip_sockaddr *pSockAddr,
    IP_S32 *sockAddrLen,
    IP_S32 *ip_errno,
    RTM_HANDLE_P rtHandle
);



/**
* \brief    Send data on a socket.
*
*           The ip_send() function initiates transmission of a message from the specified socket to its peer.
*           The ip_send() function only sends a message when the socket is connected (or pre-specified) using ip_connect().
*           If the socket is a connectionless-mode socket, the message shall be sent to the pre-specified peer address.
*
*           If space is not available at the sending socket to hold the  data to be transmitted, and the socket associated with sock_desc
*           has not been set to be nonblocking, ip_send() blocks until space is available.
*
*           If space is not available at the sending socket to hold the data to be transmitted, and the socket associated with sock_desc
*           has been set to be nonblocking, ip_send() fails.
*
*           The ip_select() function can be used to determine when it is possible to send more data.
*
* @param[in]        sockDesc     Socket descriptor returned by ip_socket().
* @param[in]        pMsg         Points to the buffer containing the data to send.
* @param[in]        msgLen       Length of the data, max is 64kBytes.
* @param[in]        flags        Must be 0 (no other flags supported).
* @param[out]       ip_errno     Error code.
* @param[in,out]    rtHandle     Realtime and card info Handle.
*
* @return     Upon successful completion, ip_send() returns the number of bytes sent.
*             Otherwise -1 returned and *ip_errno set to indicate the error.
*
*/
IP_DECL_PUBLIC
IP_S32
ip_send
(
    IP_S32 sockDesc,
    const void *pMsg,
    IP_S32 msgLen,
    IP_S32 flags,
    IP_S32 *ip_errno,
    RTM_HANDLE_P rtHandle
);


/**
* \brief    Send data on a socket.
*
*            If space is not available at the sending socket to hold the  data to be transmitted, and the socket file descriptor
*            does not be nonblocking, ip_sendto() blocks until space is available.
*
*            If space is not available at the sending socket to hold the data to be transmitted, and the socket file descriptor is
*            nonblocking, ip_sendto() fails.
*
* @param[in]        sockDesc         Socket descriptor returned by ip_socket().
* @param[in]        pMsg             Points to the buffer containing the data to send.
* @param[in]        msgLen           Length of the data, max is 64kBytes.
* @param[in]        flags            Must be 0 (no other flags supported).
* @param[in]        pSockAddr        Points to a ip_sockaddr structure containing the destination address.
*                                    The length and format of the address depend on the address family of the socket.
* @param[in]        sockAddrLen      Size of the ip_sockaddr structure.
* @param[out]       ip_errno         Error code.
* @param[in,out]    rtHandle         Realtime and card info Handle.
*
* @return     Upon successful completion, ip_sendto() returns the number of bytes sent.
*             Otherwise -1 returned and *ip_errno set to indicate the error.
*/
IP_DECL_PUBLIC
IP_S32
ip_sendto
(
    IP_S32 sockDesc,
    void *pMsg,
    IP_S32 msgLen,
    IP_S32 flags,
    struct ip_sockaddr *pSockAddr,
    IP_S32 sockAddrLen,
    IP_S32 *ip_errno,
    RTM_HANDLE_P rtHandle
);


/**
* \brief    Receive data from a connected socket.
*
*            The ip_recv() function returns the length of the message written to the buffer pointed to by the buffer argument.
*
*            For message-based sockets, such as #IP_SOCK_DGRAM the entire message will be read in a single operation.
*            If a message is too long to fit in the supplied buffer, and #IP_MSG_PEEK is not set in the flags argument, the excess
*            bytes are be discarded. For stream-based sockets, such as #IP_SOCK_STREAM, message boundaries are ignored.
*
*            In this case, data will be returned to the user as soon as it becomes available, and no data will be discarded.
*            If the #IP_MSG_WAITALL flag is not set, data is returned only up to the end of the first message.
*
*            If no messages are available at the socket and file descriptor is nonblocking, ip_recv() blocks until a message arrives.
*            If no messages are available at the socket is nonblocking ip_recv() fails and set *ip_errno to socket [#IP_EAGAIN].
*
* @param[in]        sockDesc     Socket descriptor returned by ip_socket().
* @param[out]       msg          Points to a buffer where the data should be stored.
* @param[in]        len          Specifies the length in bytes of the buffer pointed to by the msg argument.
* @param[in]        flags        Specifies the type of message reception. Values of this  argument are formed by logically
*                                OR'ing zero or more of the following values:
*                                - #IP_MSG_PEEK  Peeks at an incoming message. The data is treated as unread and the next
*                                  ip_recv() still return this data.
*                                - IP_MSG_WAITALL  On SOCK_STREAM sockets this requests that the function  block until the
*                                  full amount of data can be returned. The function may return a smaller amount of data if
*                                  the socket is a message-based socket, if a signal is caught, if the connection is terminated,
*                                  if #IP_MSG_PEEK was specified, or if an error is pending for the socket.
* @param[out]       ip_errno     Error code.
* @param[in,out]    rtHandle     Realtime and card info Handle.
*
* @return    Upon successful completion, ip_recv() returns the length of the message in bytes. If no messages are available to be
*            received and the peer has performed an orderly shutdown, ip_recv() returns 0. Otherwise, -1 returned and *ip_errno set
*            to indicate the error.
*
*/
IP_DECL_PUBLIC
IP_S32
ip_recv
(
    IP_S32 sockDesc,
    void *msg,
    IP_S32 len,
    IP_S32 flags,
    IP_S32 *ip_errno,
    RTM_HANDLE_P rtHandle
);


/**
* \brief    Receive data from a socket.
*
*           The ip_recvfrom() function receives a message from a socket and captures the address from which the data was sent. Unlike
*           the ip_recv() call, which can only be used on a connected stream socket or bound datagram socket, ip_recvfrom() can be
*           used to receive data on a socket whether or not it is connected.
*
*           If no messages are available at the socket, the
*           ip_recvfrom() call waits for a message to arrive unless the socket is nonblocking.
*           If a socket is nonblocking, -1 is
*           returned and the external variable errno is set to #IP_EAGAIN. ip_select() can be used to wait for incoming messages.
*
*           The ip_recvfrom() function returns the length of the message written to the buffer pointed to by the p_mesg argument.
*           For message-based sockets, such as #IP_SOCK_DGRAM the entire message will be read in a single operation.
*           If a message is too long to fit in the supplied buffer, and #IP_MSG_PEEK is not set in the flags argument, the excess
*           bytes will be discarded. For stream-based sockets, such as #IP_SOCK_STREAM, message boundaries are ignored. In this
*           case, data will be returned to the user as soon as it becomes available, and no data will be discarded.
*           If the #IP_MSG_WAITALL flag is not set, data is returned only up to the end of the first message.
*
*           If no messages are available at the socket associated with "sock_desc" is blocking, ip_recvfrom() blocks until a message arrives.
*           If no messages are available at the socket is nonblocking ip_recvfrom() fails and set *ip_errno to socket [#IP_EAGAIN].
*
* @param[in]        sockDesc           Socket descriptor returned by ip_socket().
* @param[out]       pMsg               Points to a buffer where the data should be stored.
* @param[in]        msgLen             Specifies the length in bytes of the buffer pointed to by the msg argument.
* @param[in]        flags              Specifies the type of message reception. Values of this  argument are formed by logically
*                                      OR'ing zero or more of the following values:
*                                      - #IP_MSG_PEEK  Peeks at an incoming message. The data is treated as unread and the next
*                                        ip_recv() still return this data.
*                                      - IP_MSG_WAITALL  On SOCK_STREAM sockets this requests that the function  block until the
*                                        full amount of data can be returned. The function may return the smaller amount of data if
*                                        the socket is a message-based socket, if a signal is caught, if the connection is terminated,
*                                        if #IP_MSG_PEEK was specified, or if an error is pending for the socket.
* @param[in]        pSockAddr          A null pointer, or points to a ip_sockaddr structure in  which the sending address is to be stored.
*                                      The length and format of the address depend on the address family of the socket.
* @param[out]       pSockAddrLen       Pointer to the size of the ip_sockaddr structure
* @param[out]       ip_errno           Error code.
* @param[in,out]    rtHandle           Realtime and card info Handle.
*
* @return    Upon successful completion, ip_recvfrom() returns the length  of the message in bytes. If no messages are available to be
*            received and the peer has performed an orderly shutdown, ip_recvfrom() returns 0. Otherwise, the function returns -1
*            and set *ip_errno to indicate the error.
*/
IP_DECL_PUBLIC
IP_S32
ip_recvfrom
(
    IP_S32 sockDesc,
    void *pMsg,
    IP_S32 msgLen,
    IP_S32 flags,
    struct ip_sockaddr *pSockAddr,
    IP_S32 *pSockAddrLen,
    IP_S32 *ip_errno,
    RTM_HANDLE_P rtHandle
);


/**
* \brief    receive a message from a socket and return its length
*
* @param[in]        sock_desc          Socket descriptor returned by ip_socket().
* @param[in]        flags              Specifies the type of message reception. Values of this  argument are formed by logically
*                                      OR'ing zero or more of the following values:
*                                      - #IP_MSG_PEEK  Peeks at an incoming message. The data is treated as unread and the next
*                                        ip_recv() still return this data.
*                                      - IP_MSG_WAITALL  On SOCK_STREAM sockets this requests that the function  block until the
*                                        full amount of data can be returned. The function may return the smaller amount of data if
*                                        the socket is a message-based socket, if a signal is caught, if the connection is terminated,
*                                        if #IP_MSG_PEEK was specified, or if an error is pending for the socket.
* @param[out]       msg_id             pointer to an ID of the received message.
*                                      The returned ID is used to retrieve the message with ip_recvfrom_get_data()
* @param[out]       ip_errno           Error code.
* @param[in,out]    rtHandle           Realtime and card info Handle.
*
* @return    Upon successful completion, ip_recvfrom_get_length() returns the length  of the message in bytes. If no messages are available to be
*            received and the peer has performed an orderly shutdown, ip_recvfrom_get_length() returns 0. Otherwise, the function returns -1
*            and set *ip_errno to indicate the error.
*
* \remark    After successful completion, the message can be retrieved with ip_recvfrom_get_data()
*
*/
IP_DECL_PUBLIC
DG_S32
ip_recvfrom_get_length
(
    DG_S32 sock_desc,
    DG_S32 flags,
    DG_U32 *msg_id,
    DG_S32 *ip_errno,
    RTM_HANDLE_P rtHandle
);


/**
* \brief    Retrieve a message from a socket
*
* @param[in]        msg_id             ID of the received message as returned by ip_recvfrom_get_length()
* @param[out]       pMsg               Points to a buffer where the data should be stored.
* @param[in]        recv_len           Specifies the length in bytes of the buffer pointed to by the msg argument.
* @param[in]        pSockAddr          A null pointer, or points to a ip_sockaddr structure in  which the sending address is to be stored.
*                                      The length and format of the address depend on the address family of the socket.
* @param[out]       pSockAddrLen       Pointer to the size of the ip_sockaddr structure
* @param[out]       ip_errno           Error code.
* @param[in,out]    rtHandle           Realtime and card info Handle.
*
* @return       Upon successful completion, ip_recvfrom_get_data() returns the length
 *              of the message in bytes. If no messages are available to be
 *              received and the peer has performed an orderly shutdown,
 *              ip_recvfrom_get_data() returns -1 and set *ip_errno to indicate the error.
 *
 * \remark      ip_recvfrom_get_data() should only be called after a successful call of ip_recvfrom_get_length()
 *
*/
IP_DECL_PUBLIC
DG_S32
ip_recvfrom_get_data
(
    DG_U32 msg_id,
    void *pMsg,
    DG_S32 recv_len,
    struct ip_sockaddr *pSockAddr,
    IP_S32 *pSockAddrLen,
    DG_S32 *ip_errno,
    RTM_HANDLE_P rtHandle
);


/**
* \brief    Get network host entry referenced by a host name.
*
* @param[in]        pName         Pointer to string with the name of the host.
* @param[out]       ip_h_errno    Error code.
* @param[in,out]    rtHandle      Realtime and card info Handle.
*
* @return    Upon successful completion, ip_gethostbyname returns a pointer to a IP_HOSTENT structure if the requested entry was found,
*            and a null pointer if the end of the database was reached or the requested entry was not found.
*            Upon unsuccessful ip_gethostbyname() sets *ip_h_errno to indicate the error.
*
* \remark    !!This function is NOT thread safe because the p_name pointer may on return be pointing to a data structure which can
*            be changed by subsequent calls to this or other similar functions.
*            The time this function uses is not defined
*/
IP_DECL_PUBLIC
struct ip_hostent *
ip_gethostbyname
(
    IP_CHAR8 *pName,
    IP_S32 *ip_h_errno,
    RTM_HANDLE_P rtHandle
);


/**
* \brief    Get network host entry referenced by a host name.
*
* @param[out]       pAddr         Pointer to string with the address of the host.
* @param[out]       addrLen       The length of the address in bytes.
* @param[out]       addrFam       The type (address family) of the address.
* @param[out]       ip_h_errno    Error code.
* @param[in,out]    rtHandle      Realtime and card info Handle.
*
* @return    Upon successful completion, ip_gethostbyaddr returns a pointer to a IP_HOSTENT structure if the requested entry was found,
*            and a null pointer if the end of the database was reached or the requested entry was not found.
*            Upon unsuccessful ip_gethostbyaddr() sets *ip_h_errno to indicate the error.
*
* \remark    !!This function is NOT thread safe because the p_name pointer may on return be pointing to a data structure which can
*            be changed by subsequent calls to this or other similar functions.
*            The time this function uses is not defined
*/
IP_DECL_PUBLIC
struct ip_hostent *
ip_gethostbyaddr
(
    const void *pAddr,
    IP_S32 addrLen,
    IP_S32 addrFam,
    IP_S32 *ip_h_errno,
    RTM_HANDLE_P rtHandle
);


/**
* \brief   Synchronous I/O multiplexing.
*
*            The select() function indicates which of the specified file descriptors is ready for reading, ready for writing,
*            or has an error condition pending.
*            If the specified condition is false for all of the specified file descriptors,
*            select() blocks, up to the specified timeout interval, until the specified condition is true for at least one of
*            the specified file descriptors or until a signal arrives that needs to be delivered.
*            File descriptor masks of type IP_FD_SET can be initialized and tested with IP_FD_CLR(), IP_FD_ISSET(), IP_FD_SET(), and IP_FD_ZERO() macros.
*
*            - IP_FD_CLR(fd, &fdset)
*              Clears the bit for the file descriptor fd in the file descriptor set fdset.
*            - IP_FD_ISSET(fd, &fdset)
*               Returns a non-zero value if the bit for the file descriptor.
*               Returns fd is set in the file descriptor set pointed to by
*               Returns fdset, and 0 otherwise
*            - IP_FD_SET(fd, &fdset)
*              Sets the bit for the file descriptor fd in the file descriptor set fdset.
*            - IP_FD_ZERO(&fdset)
*              Initializes the file descriptor set fdset to have zero bits
*              Initializes for all file descriptors.
*              Unexpected errors may occur if fd is less than 0 or greater
*              Unexpected than or equal to IP_FD_SETSIZE in any of these macros.
*
* @param[in]        sockDesc     Specifies how many descriptors should be examined. The descriptors checked are 0 through nfds-1.
* @param[in]        readfds      Points to a bit mask that specifies the file descriptors to check for reading or NULL.
* @param[in]        writefds     Points to a bit mask that specifies the file descriptors to check for writing or NULL.
* @param[in]        exceptfds    Points to a bit mask that specifies the file descriptors to check for exception conditions or NULL
* @param[in]        timeval      The timeval parameter is not supported by this function.  The implementation ignores the timeval input and sets the
*                                timeout value fix to 0.  Notice that the host interface API functions must be non-blocking and must return as soon as possible.
* @param[out]       ip_errno     Error code.
* @param[in,out]    rtHandle     Realtime and card info Handle.
*
* @return     Upon successful completion, the ip_select() function returns the total number of bits set in the bit masks.
*             Otherwise, -1 returned, and errno will be set to indicate the error.
*/
IP_DECL_PUBLIC
IP_S32
ip_select
(
    IP_S32 sockDesc,
    ip_fd_set *readfds,
    ip_fd_set *writefds,
    ip_fd_set *exceptfds,
    const struct ip_timeval *timeval,
    IP_S32 *ip_errno,
    RTM_HANDLE_P rtHandle
);


/**
* \brief    Converts the specified string, in the Internet standard dot notation, network byte order (bytes ordered from left to right).
*
* @param[out]    cp      Points to a string in Internet standard dot notation.
* @param[in]     addr    Buffer where the converted address is to be stored.
*
* @retval    1    If the address is successfully converted.
* @retval    0    If the conversion failed.
*
* \remark    No call to device. Converts the specified string, in the Internet standard dot notation, to a network address,
*            and stores the address in the structure provided.
*/
IP_DECL_PUBLIC
IP_S8
ip_inet_aton
(
    const IP_S8 *cp,
    struct ip_in_addr *addr
);



/**
* \brief   Converts values between host and network byte order.
*
* @param[in]    x    DG_U16 value.
*
* @return    The argument value converted from host to network byte order.
*
* \remark    No call to device.
*/
IP_DECL_PUBLIC
IP_U16
ip_htons
(
    IP_U16 val
);


/**
* \brief   Converts values between host and network byte order.
*
* @param[in]    val    DG_U32 value.
*
* @return    The argument value converted from host to network byte order.
*
* \remark    No call to device.
*/
IP_DECL_PUBLIC
IP_U32
ip_htonl
(
    IP_U32 val
);


/**
* \brief    Converts an address from network format (struct in_addr, or some other binary form, in network byte order)
*           to presentation format.
*
* @param[in]     af          Specifies the address family. Currently, only #IP_AF_INET supported.
* @param[in]     src         Points to the address. The format of the address is interpreted according to af.
* @param[out]    dst         Points to the buffer where the converted address is stored.
* @param[in]     size        Is the size of dst, in bytes.
* @param[out]    ip_errno    Error code.
*
* @return    Returns a pointer to the buffer  containing the text string if the conversion succeeds,
*            and NULL otherwise, and set *ip_errno to indicate the error.
*
* \remark    No call to device.
*/
IP_DECL_PUBLIC
const IP_CHAR8 *
ip_inet_ntop
(
    IP_S32  af,
    const void *src,
    IP_CHAR8 *dst,
    IP_S32 size,
    IP_S32 *ip_errno
);


/**
* \brief    Get name of current device
*
* @param[out]       name         Points to the buffer where the name is stored.
* @param[in]        namelen      Is the size of name buffer, in bytes.
* @param[out]       ip_errno     Error code.
* @param[in,out]    rtHandle     Realtime and card info Handle.
*
* @retval    DG_ENOERR(0)       If successful.
* @retval    DG_EGENERAL(-1)    If error occurred (see ip_errno for details).
*
* \remark    Host names are limited to {IP_HOST_NAME_MAX} bytes.
*/
IP_DECL_PUBLIC
IP_S32
ip_gethostname
(
    IP_CHAR8 *name,
    IP_S32 namelen,
    IP_S32 *ip_errno,
    RTM_HANDLE_P rtHandle
);



/* --------------------------------------------------------------------------
 * Procedure :  ip_socket_ping
 *
 * Purpose :    test socket communication from host to device
 *              (only for duagon tests)
 *
 * Parameters:  u8
 *              retU8  will return u8
 *              s16
 *              rets16 will return s16
 *              u32    will return value of u8
 *              retu32 will return u32
 *              text   0 termination char array
 *              retData data from text
 *              mexSize in buffer size, out len retData
 * Return:      strlen(text)
 * Remark:
 * --------------------------------------------------------------------------
 */
IP_DECL_PUBLIC
IP_S32
ip_socket_ping
(
    const IP_U8 u8,
    IP_U8 *retU8,
    const IP_S16 s16,
    IP_S16 *rets16,
    IP_U32 *u32,
    IP_U32 *retu32,
    const IP_CHAR8 *text,
    const IP_CHAR8 *retData,
    IP_U32 *maxSize,
    IP_S32 *ip_errno,
    RTM_HANDLE_P rtHandle
);


#endif /* DG_IP_H */
