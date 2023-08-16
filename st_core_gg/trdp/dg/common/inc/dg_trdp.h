/**
 * \file dg_trdp.h
 * \brief
 * \date 10.10.2014
 *
 * \details
 *
 * ==========================================================================
 *  Licence   : Duagon Software Licence (see file 'licence.txt')
 *
 * --------------------------------------------------------------------------
 *
 *  (C) COPYRIGHT, Duagon AG, CH-8953 Dietikon, Switzerland
 *  All Rights Reserved.
 *
 * ==========================================================================
 */

/**
 * \defgroup trdp_host TRDP Host API
 * @{
 */

#ifndef DG_TRDP_H_
#define DG_TRDP_H_

/* ==================================================================================
 * Includes
 * ================================================================================*/
#include "os_def.h"
#include "dg_conf.h"
#include "dg_rtm.h"

#include "dg_trdp_types.h"
/* ==================================================================================
 * Definitions (typedef, constants, macros)
 * ================================================================================*/
#define TRDP_PD_PROTOCOL_ID   0x16
#define TRDP_MD_PROTOCOL_ID   0x17

#define TRDP_MAX_SIZE_PD 1432
#define TRDP_MAX_SIZE_MD 65388
#define TRDP_MAX_SIZE_RPC 256000

/* General structs */
typedef struct TRDP_STATUS
{
        DG_S32 nb_pub_pd;
        DG_S32 nb_sub_pd;
        DG_S32 nb_list_md;
} TRDP_STATUS;

/* PD structs */
typedef struct TRDP_PUB_INFO
{
        DG_U32 com_id;
} TRDP_PUB_INFO;

typedef struct TRDP_PD_INFO
{
        DG_TRDP_IP_ADDR_T src_ip;
        DG_TRDP_IP_ADDR_T dest_ip;
        DG_U32 seq_count;
        DG_S32 msg_type;
        DG_U32 com_id;
} TRDP_PD_INFO;

/* MD structs */
typedef struct TRDP_MSG_INFO
{
        DG_U32 user_ref;
        DG_U32 com_id;
        DG_TRDP_IP_ADDR_T src_ip;
        DG_TRDP_IP_ADDR_T dest_ip;
        DG_TRDP_URI_T src_uri;
        DG_TRDP_URI_T dest_uri;
        DG_TRDP_UUID_T session_id;
        DG_S32 msg_type;
        DG_S32 flags;
} TRDP_MSG_INFO;

typedef struct TRDP_RQST_INFO
{
        DG_TRDP_UUID_T session_id;
} TRDP_RQST_INFO;

typedef struct TRDP_REPLY_INFO
{
        DG_TRDP_UUID_T session_id;
        DG_U16 user_status;
        DG_U32 com_id;
} TRDP_REPLY_INFO;

/* --------------------------------------------------------------------------
 * Commands used to transfer trdp functions on host to duagon device
 */
/**
 * \enum
 * \brief containing the identifier used by the PD HDC protocol to identify the used function
 */
enum
{
    TRDP_PD_PUBLISH,
    TRDP_PD_UNPUBLISH,
    TRDP_PD_SUBSCRIBE,
    TRDP_PD_UNSUBSCRIBE,
    TRDP_PD_PUT,
    TRDP_PD_GET,
    TRDP_PD_REQUEST,
    TRDP_PD_SET_REDUNDANT,
    TRDP_PD_GET_REDUNDANT,
    TRDP_PD_GET_ITF,
    TRDP_PD_POLL_SUBSCRIBERS_ITF,
    TRDP_PD_POLL_SUBSCRIBERS,
    TRDP_PD_LAST_CMD
};

/**
 * \enum
 * \brief containing the identifier used by the MD HDC protocol to identify the used function
 */
enum
{
    TRDP_INIT,
    TRDP_GET_STATUS,
    TRDP_WHO_IS,
    TRDP_INITIALIZE_STACK,
    TRDP_INITIALIZE_MEMORY,
    TRDP_CONFIG_IF,
    TRDP_CONFIG_PD,
	TRDP_CONFIG_MD,
    TRDP_CONFIG_COM_ID,
	TRDP_COM_ID_ADD_DEST,
	TRDP_COM_ID_CONFIGURE_MD_PAR,
    TRDP_COM_ID_CONFIGURE_PD_PAR,
    TRDP_COM_ID_ADD_SRC,
    TRDP_CONFIG_DATA_SET,
    TRDP_MD_ADD_LISTENER,
    TRDP_MD_DEL_LISTENER,
    TRDP_MD_NOTIFY,
    TRDP_MD_REQUEST,
    TRDP_MD_REPLY,
    TRDP_MD_REPLY_QUERY,
    TRDP_MD_CONFIRM,
    TRDP_MD_POLL_MSGS,
    TRDP_MD_GET,
    TRDP_GET_STATUS_ITF,
    TRDP_CONFIG_MARSHALLING,
    TRDP_MD_LAST_CMD
};

/* ==================================================================================
 * Global variables declarations
 * ================================================================================*/

/* ==================================================================================
 * Global function prototypes
 * ================================================================================*/
/* General functions */
/**
 * @brief init function for the TRDP host <-> device communication. Compares the version on the device to the host version.
 * @param[out] trdp_err_no error number
 * @param[in] rt_handle Real time handle
 * @return 0 if ok, else -1 with trdp_err_no set
 */
extern DG_S32 trdp_init(
    DG_S32 * trdp_err_no,
    RTM_HANDLE_P rt_handle);

/**
 * @brief function to get status information about the TRDP stack running on the device
 * @param[out] nb_pub_pd number of process data publishers
 * @param[out] nb_sub_pd number of process data subscribers
 * @param[out] nb_list_md number of message data listeners
 * @param[out] trdp_err_no error number
 * @param[in] rt_handle Real time handle
 * @return 0 if ok, else -1 with trdp_err_no set
 */
extern DG_S32 trdp_get_status(
    DG_S32 * nb_pub_pd,
    DG_S32 * nb_sub_pd,
    DG_S32 * nb_list_md,
    DG_S32 * trdp_err_no,
    RTM_HANDLE_P rt_handle);

/**
 * @brief function to get status information about the TRDP stack of the specified interface
 * @param[in] itf specifies interface
 * @param[out] nb_pub_pd number of process data publishers
 * @param[out] nb_sub_pd number of process data subscribers
 * @param[out] nb_list_md number of message data listeners
 * @param[out] trdp_err_no error number
 * @param[in] rt_handle Real time handle
 * @return 0 if ok, else -1 with trdp_err_no set
 */
extern DG_S32 trdp_get_status_itf(
    DG_U32 itf,
    DG_S32 * nb_pub_pd,
    DG_S32 * nb_sub_pd,
    DG_S32 * nb_list_md,
    DG_S32 * trdp_err_no,
    RTM_HANDLE_P rt_handle);

/**
 * @brief function to resolve host uri to ip address
 * @param[in] iface ethernet interface the uri has been configured on
 * @param[in] uri uri that needs to be resolved
 * @param[out] ip_addr ip address corresponding to the URI
 * @param[out] trdp_err_no error number
 * @param[in] rt_handle Real time handle
 * @return 0 if ok, else -1 with trdp_err_no set
 */
extern DG_S32 trdp_who_is(
    DG_U8 iface,
    DG_TRDP_URI_T uri,
    DG_U32 * ip_addr,
    DG_S32 * trdp_err_no,
    RTM_HANDLE_P rt_handle);

/**
 * @brief Initializes the trdp stack after all the configurations. From now on the stack is running and can't be reconfigured.
 * @param[out] trdp_err_no error number
 * @param[out] rt_handle Real time handle
 * @return 0 if ok, else -1 with trdp_err_no set
 */
extern DG_S32 trdp_initialize_stack(
    DG_S32 * trdp_err_no,
    RTM_HANDLE_P rt_handle);

/* Configuration functions*/
/**
 * @brief Initializes the internal memory of the TRDP stack
 * @param[in] interface interface number
 * @param[in] nb_exchg_param number of exchange parameters
 * @param[in] nb_data_set numer of datasets
 * @param[out] trdp_err_no error number
 * @param[out] rt_handle Real time handle
 * @return 0 if ok, else -1 with trdp_err_no set
 */
extern DG_S32 trdp_initialize_memory(
    DG_U8 iface,
    DG_U32 nb_exchg_param,
    DG_U32 nb_data_set,
    DG_S32 * trdp_err_no,
    RTM_HANDLE_P rt_handle);

/**
 * @brief Configures the interface
 * @param[in] interface interface number
 * @param[out] trdp_err_no error number
 * @param[out] rt_handle Real time handle
 * @return 0 if ok, else -1 with trdp_err_no set
 */
extern DG_S32 trdp_configure_interface(
    DG_U8 iface,
    DG_S32 * trdp_err_no,
    RTM_HANDLE_P rt_handle);

/**
 * @brief Configures the global process data parameters
 * @param[in] interface interface number
 * @param[in] port port number
 * @param[in] flags TRDP flags for marshalling
 * @param[in] timeout timeout in us
 * @param[in] timeout_behaviour timeout behaviour flag
 * @param[in] qos Quality of service
 * @param[in] ttl time to live
 * @param[out] trdp_err_no error number
 * @param[out] rt_handle Real time handle
 * @return 0 if ok, else -1 with trdp_err_no set
 */
extern DG_S32 trdp_configure_process_data(
    DG_U8 iface,
    DG_U16 port,
    DG_S32 flags,
    DG_U32 timeout,
    DG_S32 timeout_behaviour,
    DG_U8 qos,
    DG_U8 ttl,
    DG_S32 * trdp_err_no,
    RTM_HANDLE_P rt_handle);

/**
 * @brief Configures the global message data parameters
 * @param[in] iface interface number
 * @param[in] udp_port UDP port number
 * @param[in] tcp_port TCP port number
 * @param[in] confirm_timeout confirm timeout in us
 * @param[in] connect_timeout connect timeout in us
 * @param[in] reply_timeout reply timeout in us
 * @param[in] flags TRDP flags
 * @param[in] qos Quality of service
 * @param[in] ttl time to live
 * @param[out] trdp_err_no error number
 * @param[out] rt_handle Real time handle
 * @return 0 if ok, else -1 with trdp_err_no set
 */
extern DG_S32 trdp_configure_message_data(
    DG_U8 iface,
    DG_U16 udp_port,
    DG_U16 tcp_port,
    DG_U32 confirm_timeout,
    DG_U32 connect_timeout,
    DG_U32 reply_timeout,
    DG_S32 flags,
    DG_U8 qos,
    DG_U8 ttl,
    DG_S32 *trdp_err_no,
	RTM_HANDLE_P rt_handle);

/**
 * @brief Configure ComID
 * @param[in] iface interface number
 * @param[in] com_id ComID
 * @param[in] data_set_id dataset ID associated with ComId
 * @param[in] num_src number of sources added to this ComID
 * @param[in] num_dest Number of destinations added to this ComID
 * @param[in] create Create flag, 1 for true, 0 for false. If true: associated publisher/listener/subscriber shall be generated automatically
 * @param[in] type Exchange type
 * @param[out] trdp_err_no error number
 * @param[out] rt_handle Real time handle
 * @return 0 if ok, else -1 with trdp_err_no set
 */
extern DG_S32 trdp_configure_com_id(
    DG_U8 iface,
    DG_U32 com_id,
    DG_U32 data_set_id,
    DG_U32 num_src,
    DG_U32 num_dest,
    DG_U8 create,
    DG_S32 type,
    DG_S32 * trdp_err_no,
    RTM_HANDLE_P rt_handle);

/**
 * @brief Add destination to ComID. Several destinations with different IDs can be added to a Com ID.
 * @param[in] iface interface number
 * @param[in] com_id ComID to which the destination is added
 * @param[in] dest_id Destination ID of this destination
 * @param[in] uri URI for this destination / Com ID pair
 * @param[out] trdp_err_no error number
 * @param[out] rt_handle Real time handle
 * @return 0 if ok, else -1 with trdp_err_no set
 */
extern DG_S32 trdp_com_id_add_dest(
    DG_U8 iface,
    DG_U32 com_id,
    DG_U32 dest_id,
    DG_TRDP_URI_T uri,
    DG_S32 *trdp_err_no,
    RTM_HANDLE_P rt_handle);

/**
 * @brief Configure MD parameters of existing ComID
 * @param[in] iface interface number
 * @param[in] com_id ComID
 * @param[in] confirm_timeout confirm timeout in us
 * @param[in] reply_timeout reply timeout in us
 * @param[in] flags TRDP flags
 * @param[out] trdp_err_no error number
 * @param[out] rt_handle Real time handle
 * @return 0 if ok, else -1 with trdp_err_no set
 */
extern DG_S32 trdp_com_id_configure_md_par(
    DG_U8 iface,
    DG_U32 com_id,
    DG_U32 confirm_timeout,
    DG_U32 reply_timeout,
    DG_S32 flags,
    DG_S32 *trdp_err_no,
    RTM_HANDLE_P rt_handle);

/**
 * @brief Configure PD parameters of existing ComID
 * @param[in] iface interface number
 * @param[in] com_id ComID
 * @param[in] cycle Cycle time (interval) in us
 * @param[in] redundant 0 = not redundant, != 0 redundancy group
 * @param[in] timeout timeout in us
 * @param[in] to_behav Behaviour on timeout (DG_TRDP_TO_BEHAVIOUR_T)
 * @param[in] flags TRDP flags
 * @param[in] offset Offset-address for PD in traffic stored for ladder topology
 * @param[out] trdp_err_no error number
 * @param[out] rt_handle Real time handle
 * @return 0 if ok, else -1 with trdp_err_no set
 */
extern DG_S32 trdp_com_id_configure_pd_par(
    DG_U8 iface,
    DG_U32 com_id,
    DG_U32 cycle,
    DG_U32 redundant,
    DG_U32 timeout,
    DG_S32 to_behav,
    DG_S32 flags,
    DG_U16 offset,
    DG_S32 *trdp_err_no,
    RTM_HANDLE_P rt_handle);

/**
 * @brief Add a source to a Com ID used for source filtering
 * @param[in] iface interface number
 * @param[in] com_id Com ID to which the source is added
 * @param[in] src_id Source ID of this destination
 * @param[in] uri1 URI for this source ID / Com ID pair
 * @param[out] trdp_err_no error number
 * @param[out] rt_handle Real time handle
 * @return 0 if ok, else -1 with trdp_err_no set
 */
extern DG_S32 trdp_com_id_add_src(
    DG_U8 iface,
    DG_U32 com_id,
    DG_U32 src_id,
    DG_TRDP_URI_T uri1,
    DG_S32 *trdp_err_no,
    RTM_HANDLE_P rt_handle);

/**
 * @brief Configure a dataset
 * @param[in] data_set_id Dataset ID
 * @param[in] elements Number of elements
 * @param[in] type_array Pointer to type array, must be the size of elements
 * @param[in] size_array Pointer to size array, must be the size of elements
 * @param[out] trdp_err_no error number
 * @param[out] rt_handle Real time handle
 * @return 0 if ok, else -1 with trdp_err_no set
 */
extern DG_S32 trdp_configure_data_set(
    DG_U32 data_set_id,
    DG_U32 elements,
    DG_U32 *type_array,
    DG_U32 *size_array,
    DG_S32 *trdp_err_no,
    RTM_HANDLE_P rt_handle);

/* PD functions */
/**
 * @brief publishes the data to the telegram specified by the com id
 * @param[in] com_id specifies the telegram
 * @param[in] dest_id specifies for which destination to publish the telegram (0 for all)
 * @param[in] data data to be published
 * @param[in] size size of the data
 * @param[out] trdp_err_no error number
 * @param[in] rt_handle Real time handle
 * @return 0 if ok, else -1 with trdp_err_no set
 */
extern DG_S32 trdp_pd_publish(
    DG_U32 com_id,
    DG_U32 dest_id,
    DG_U8 * data,
    DG_U32 size,
    DG_S32 * trdp_err_no,
    RTM_HANDLE_P rt_handle);

/**
 * @brief unpublishes the telegram specified by the com id
 * @param[in] com_id specifies the telegram
 * @param[in] dest_id specifies for which destination to unpublish the telegram (0 for all)
 * @param[out] trdp_err_no error number
 * @param[in] rt_handle Real time handle
 * @return 0 if ok, else -1 with trdp_err_no set
 */
extern DG_S32 trdp_pd_unpublish(
    DG_U32 com_id,
    DG_U32 dest_id,
    DG_S32 * trdp_err_no,
    RTM_HANDLE_P rt_handle);

/**
 * @brief subscribes to the telegram specified by the com_id
 * @param[in] com_id specifies the telegram
 * @param[in] src_id specifies for which source to subscribe the telegram (0 for all)
 * @param[out] trdp_err_no error number
 * @param[in] rt_handle Real time handle
 * @return 0 if ok, else -1 with trdp_err_no set
 */
extern DG_S32 trdp_pd_subscribe(
    DG_U32 com_id,
    DG_U32 src_id,
    DG_S32 * trdp_err_no,
    RTM_HANDLE_P rt_handle);

/**
 * @brief unsubscribes from the telegram specified by the com_id
 * @param[in] com_id specifies the telegram
 * @param[in] src_id specifies for which source to unsubscribe the telegram (0 for all)
 * @param[out] trdp_err_no error number
 * @param[in] rt_handle Real time handle
 * @return 0 if ok, else -1 with trdp_err_no set
 */
extern DG_S32 trdp_pd_unsubscribe(
    DG_U32 com_id,
    DG_U32 src_id,
    DG_S32 * trdp_err_no,
    RTM_HANDLE_P rt_handle);

/**
 * @brief puts the data on the traffic memory belonging to the telegram specified by the com_id
 * @param[in] com_id specifies the telegram
 * @param[in] data data to be published
 * @param[in] size size of the data
 * @param[out] trdp_err_no error number
 * @param[in] rt_handle Real time handle
 * @return 0 if ok, else -1 with trdp_err_no set
 */
extern DG_S32 trdp_pd_put(
    DG_U32 com_id,
    DG_U8 * data,
    DG_U32 size,
    DG_S32 * trdp_err_no,
    RTM_HANDLE_P rt_handle);
    
/**
 * @brief sends out a process data request to the specified IP.
 * @param[in] com_id specifies the telegram
 * @param[in] dst_ip address to send the request to
 * @param[in] reply_com_id specifies the com id which is requested
 * @param[in] reply_ip address to send the response to
 * @param[in] data data to be sent
 * @param[in] size size of the data
 * @param[out] trdp_err_no error number
 * @param[in] rt_handle Real time handle
 * @return 0 if ok, else -1 with trdp_err_no set
 */
extern DG_S32 trdp_pd_request(
    DG_U32 com_id,
    DG_U32 dst_ip,
    DG_U32 reply_com_id,
    DG_TRDP_IP_ADDR_T reply_ip,
    DG_U8 * data,
    DG_U32 size,
    DG_S32 * trdp_err_no,
    RTM_HANDLE_P rt_handle);

/**
 * @brief gets data belonging to the com id from the traffic memory
 * @param[in] com_id specifies which data to get
 * @param[out] src_ip source ip of the message
 * @param[out] dest_ip destination ip of the message
 * @param[out] seq_count sequence count
 * @param[out] msg_type type of the message
 * @param[out] data received data
 * @param[in,out] size size of the received data
 * @param[out] trdp_err_no error number
 * @param[in] rt_handle Real time handle
 * @return 0 if ok, else -1 with trdp_err_no set
 */
extern DG_S32 trdp_pd_get(
    DG_U32 com_id,
    DG_TRDP_IP_ADDR_T * src_ip,
    DG_TRDP_IP_ADDR_T * dest_ip,
    DG_U32 * seq_count,
    DG_S32 * msg_type,
    DG_U8 * data,
    DG_U32 * size,
    DG_S32 * trdp_err_no,
    RTM_HANDLE_P rt_handle);
       
/**
 * @brief gets data belonging to the com id from the traffic memory of a specific interface
 * @param[in] itf specifies from which interface to get the data
 * @param[in] com_id specifies which data to get
 * @param[out] src_ip source ip of the message
 * @param[out] dest_ip destination ip of the message
 * @param[out] seq_count sequence count
 * @param[out] msg_type type of the message
 * @param[out] data received data
 * @param[in,out] size size of the received data
 * @param[out] trdp_err_no error number
 * @param[in] rt_handle Real time handle
 * @return 0 if ok, else -1 with trdp_err_no set
 */   
extern DG_S32 trdp_pd_get_itf(
    DG_U8 itf,
    DG_U32 com_id,
    DG_TRDP_IP_ADDR_T * src_ip,
    DG_TRDP_IP_ADDR_T * dest_ip,
    DG_U32 * seq_count,
    DG_S32 * msg_type,
    DG_U8 * data,
    DG_U32 * size,
    DG_S32 * trdp_err_no,
    RTM_HANDLE_P rt_handle);

/**
 * @brief get status of all subscribers of an interface
 * @param[in] interface specifies from which interface to get the status
 * @param[in] nb_of_subs_itf number of subscribers
 * @param[out] com_id_array reply array with com ids of subscribed telegrams
 * @param[out] src_ip_array reply array of source IPs corresponding to the com ids
 * @param[out] result_code_array reply array with the result codes of subscribed telegrams
 * @param[out] trdp_err_no error number
 * @return 0 if ok, else -1 with trdp_err_no set
 */
extern DG_S32 trdp_pd_poll_subscribers_itf(
    DG_U8 itf,
    DG_U32 nb_of_subs_itf,
    DG_U32 * com_id_array_u32,
    DG_U32 * src_ip_array_u32,
    DG_S32 * result_code_array_u32,
    DG_S32 * trdp_err_no,
    RTM_HANDLE_P rt_handle);

/**
 * @brief get status of all subscribers of an interface
 * @param[in] nb_of_subs number of subscribers
 * @param[out] itf_array reply array with interfaces of subscribed telegrams
 * @param[out] com_id_array reply array with com ids of subscribed telegrams
 * @param[out] src_ip_array reply array of source IPs corresponding to the com ids
 * @param[out] result_code_array reply array with the result codes of subscribed telegrams
 * @param[out] trdp_err_no error number
 * @return 0 if ok, else -1 with trdp_err_no set
 */
extern DG_S32 trdp_pd_poll_subscribers(
    DG_U32 nb_of_subs,
    DG_U8 * itf_array,
    DG_U32 * com_id_array_u32,
    DG_U32 * src_ip_array_u32,
    DG_S32 * result_code_array_u32,
    DG_S32 * trdp_err_no,
    RTM_HANDLE_P rt_handle);

/**
 * @brief Change leader state for a redundancy group
 * @param[in] red_id Redundancy Group ID
 * @param[in] leader Leader state
 * @param[out] trdp_err_no error number
 * @param[in] rt_handle Real time handle
 * @return 0 if ok, else -1 with trdp_err_no set
 */
extern DG_S32 trdp_pd_set_redundant(
    DG_U32 red_id, 
    DG_U8 leader,
    DG_S32 * trdp_err_no,
    RTM_HANDLE_P rt_handle);

    /**
 * @brief Get leader state for a redundancy group
 * @param[in] red_id Redundancy Group ID
 * @param[out] leader Leader state
 * @param[out] trdp_err_no error number
 * @param[in] rt_handle Real time handle
 * @return 0 if ok, else -1 with trdp_err_no set
 */
extern DG_S32 trdp_pd_get_redundant(
    DG_U32 red_id, 
    DG_U8 * leader,
    DG_S32 * trdp_err_no,
    RTM_HANDLE_P rt_handle);
    
    
/* MD functions */
/**
 * @brief adds a listener specified by com id, dest ip, dest uri, and flags.
 * @param[out] user_ref reference to the listener
 * @param[in] com_id com id to listen to
 * @param[in] dest_ip destination ip to listen to
 * @param[in] dest_uri destination uri to listen to
 * @param[in] flags specifier flags
 * @param[out] trdp_err_no error number
 * @param[in] rt_handle Real time handle
 * @return 0 if ok, else -1 with trdp_err_no set
 *
 * Set unused parameters to 0. User ref specifies the listener and is used to retrieve the message.
 */
extern DG_S32 trdp_md_add_listener(
    DG_U32 * user_ref,
    DG_U32 com_id,
    DG_TRDP_IP_ADDR_T dest_ip,
    DG_TRDP_URI_T dest_uri,
    DG_S32 flags,
    DG_S32 * trdp_err_no,
    RTM_HANDLE_P rt_handle);

/**
 * @brief removes the listener specified by the user ref
 * @param[in] user_ref reference to the message
 * @param[out] trdp_err_no error number
 * @param[in] rt_handle Real time handle
 * @return 0 if ok, else -1 with trdp_err_no set
 */
extern DG_S32 trdp_md_del_listener(
    DG_U32 user_ref,
    DG_S32 * trdp_err_no,
    RTM_HANDLE_P rt_handle);

/**
 * @brief sends a notify message
 * @param[in] user_ref reference to the message.
 * @param[in] com_id com id of the message
 * @param[in] src_ip source ip
 * @param[in] dest_ip destination ip
 * @param[in] src_uri source uri
 * @param[in] dest_uri destination uri
 * @param[in] flags flags
 * @param[in] data data to be sent
 * @param[in] size size of the data
 * @param[out] trdp_err_no error number
 * @param[in] rt_handle Real time handle
 * @return 0 if ok, else -1 with trdp_err_no set
 */
extern DG_S32 trdp_md_notify(
    DG_U32 user_ref,
    DG_U32 com_id,
    DG_TRDP_IP_ADDR_T src_ip,
    DG_TRDP_IP_ADDR_T dest_ip,
    DG_TRDP_URI_T src_uri,
    DG_TRDP_URI_T dest_uri,
    DG_S32 flags,
    DG_U8 * data,
    DG_U32 size,
    DG_S32 * trdp_err_no,
    RTM_HANDLE_P rt_handle);

/**
 * @brief sends a request message
 * @param[in] user_ref reference to the message.
 * @param[in] com_id com id of the message
 * @param[in] src_ip source ip
 * @param[in] dest_ip destination ip
 * @param[in] src_uri source uri
 * @param[in] dest_uri destination uri
 * @param[in] flags flags
 * @param[in] num_expected_replies number of expected replies
 * @param[in] max_nr_retries maximum retries
 * @param[out] session_id id of the session.
 * @param[in] data data to be sent
 * @param[in] size size of the data
 * @param[out] trdp_err_no error number
 * @param[in] rt_handle Real time handle
 * @return 0 if ok, else -1 with trdp_err_no set
 */
extern DG_S32 trdp_md_request(
    DG_U32 user_ref,
    DG_U32 com_id,
    DG_TRDP_IP_ADDR_T src_ip,
    DG_TRDP_IP_ADDR_T dest_ip,
    DG_TRDP_URI_T src_uri,
    DG_TRDP_URI_T dest_uri,
    DG_S32 flags,
    DG_U32 num_expected_replies,
    DG_U32 max_nr_retries,
    DG_TRDP_UUID_T session_id,
    DG_U8 * data,
    DG_U32 size,
    DG_S32 * trdp_err_no,
    RTM_HANDLE_P rt_handle);

/**
 * @brief sends a reply to a received request
 * @param[in] session_id id of the session. (from request)
 * @param[in] user_status status
 * @param[in] com_id com id
 * @param[in] data data to be sent
 * @param[in] size size of the data
 * @param[out] trdp_err_no error number
 * @param[in] rt_handle Real time handle
 * @return 0 if ok, else -1 with trdp_err_no set
 */
extern DG_S32 trdp_md_reply(
    DG_TRDP_UUID_T session_id,
    DG_U16 user_status,
    DG_U32 com_id,
    DG_U8 * data,
    DG_U32 size,
    DG_S32 * trdp_err_no,
    RTM_HANDLE_P rt_handle);

/**
 * @brief sends a reply query to a received request, expects a confirm
 * @param[in] session_id id of the session. (from request)
 * @param[in] user_status status
 * @param[in] com_id com id
 * @param[in] data data to be sent
 * @param[in] size size of the data
 * @param[out] trdp_err_no error number
 * @param[in] rt_handle Real time handle
 * @return 0 if ok, else -1 with trdp_err_no set
 */
extern DG_S32 trdp_md_reply_query(
    DG_TRDP_UUID_T session_id,
    DG_U16 user_status,
    DG_U32 com_id,
    DG_U8 * data,
    DG_U32 size,
    DG_S32 * trdp_err_no,
    RTM_HANDLE_P rt_handle);

/**
 * @brief sends a confirm to a received reply query
 * @param[in] session_id id of the session. (from request)
 * @param[in] user_status status
 * @param[out] trdp_err_no error number
 * @param[in] rt_handle Real time handle
 * @return 0 if ok, else -1 with trdp_err_no set
 */
extern DG_S32 trdp_md_confirm(
    DG_TRDP_UUID_T session_id,
    DG_U16 user_status,
    DG_S32 * trdp_err_no,
    RTM_HANDLE_P rt_handle);

/**
 * @brief polls the status of the message data queues.
 * @param[out] user_refs array of user references that received messages
 * @param[in,out] size number of elements in the list
 * @param[out] trdp_err_no error number
 * @param[in] rt_handle Real time handle
 * @return 0 if no message was received, 1 if message was receieved, else -1 with trdp_err_no set
 */
extern DG_S32 trdp_md_poll_msgs(
    DG_U32 * user_refs,
    DG_U32 * size,
    DG_S32 * trdp_err_no,
    RTM_HANDLE_P rt_handle);

/**
 * @brief gets the data for the listener specified by user ref
 * @param[in] user_ref speficies the queue to retrieve data from
 * @param[out] com_id com id
 * @param[out] msg_type message type
 * @param[out] src_ip source ip
 * @param[out] dest_ip destination ip
 * @param[out] src_uri source uri
 * @param[out] dest_uri destination uri
 * @param[out] session_id session id
 * @param[out] data received data
 * @param[in,out] size size of the received data
 * @param[out] trdp_err_no error number
 * @param[in] rt_handle Real time handle
 * @return 0 if no message was received, 1 if message was receieved, else -1 with trdp_err_no set
 */
extern DG_S32 trdp_md_get(
    DG_U32 * user_ref,
    DG_U32 * com_id,
    DG_S32 * msg_type,
    DG_U32 * src_ip,
    DG_U32 * dest_ip,
    DG_TRDP_URI_T src_uri,
    DG_TRDP_URI_T dest_uri,
    DG_TRDP_UUID_T session_id,
    DG_U8 * data,
    DG_U32 * size,
    DG_S32 * trdp_err_no,
    RTM_HANDLE_P rt_handle);

/**
 * @brief configures trdp marshalling ip module
 * @param[in] endianness 0 for little-endian, 1 for big-endian
 * @param[in] align_8bit alignement of 8 Bit datatype
 * @param[in] align_16bit alignement of 16 Bit datatype
 * @param[in] align_32bit alignement of 32 Bit datatype
 * @param[in] align_48bit alignement of 48 Bit datatype
 * @param[in] align_64bit alignement of 64 Bit datatype
 * @param[in] align_double alignement of double datatype
 * @param[out] trdp_err_no error number
 * @param[in] rt_handle Real time handle
 * @return 0 if no message was received, 1 if message was receieved, else -1 with trdp_err_no set
 */
extern DG_S32 trdp_marshalling_configure(
    DG_U8 endianness,
    DG_U32 align_8bit,
    DG_U32 align_16bit,
    DG_U32 align_32bit,
    DG_U32 align_48bit,
    DG_U32 align_64bit,
    DG_U32 align_double,
    DG_S32 * trdp_err_no,
    RTM_HANDLE_P rt_handle);

/**
 * @}
 */
#endif /* define DG_TRDP_H_ */
