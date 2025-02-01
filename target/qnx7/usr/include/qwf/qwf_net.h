/*
 *
 * $QNXLicenseC:
 * Copyright 2016-2019, QNX Software Systems. All Rights Reserved.
 *
 * You must obtain a written license from and pay applicable
 * license fees to QNX Software Systems before you may reproduce,
 * modify or distribute this software, or any work that includes
 * all or part of this software.   Free development licenses are
 * available for evaluation and non-commercial purposes.  For more
 * information visit http://licensing.qnx.com or email
 * licensing@qnx.com.
 *
 * This file may contain contributions from others.  Please review
 * this entire file for other proprietary rights or license notices,
 * as well as the QNX Development Suite License Guide at
 * http://licensing.qnx.com/license-guide/ for other information.
 * $
 */

 /**
 * @file
 *
 * @brief  This interface provides access to the Network Manager (NET MGR)
 *         Service.
 *
 * @details This interface gives client the ability to control the Network
 *          Interface connections on the device and retrieve their associated
 *          configuration and state information. The interface is provided by
 *          the "ifconfig" utility.
 *
 *          Clients interact with NET MGR via this interface in order to
 *          - Inform the NET MGR that an interface is available or connected,
 *            providing the connected interface and networking parameters
 *          - Inform the NET MGR that an interface is disconnected
 *          - Obtain notification of changes to network interface data
 *
 *          Usage:
 *          - Client application calls qwf_Init() to initialize the QWF Context
 *            data used as container for QWF and service data (like NET MGR)
 *          - Client application calls qwf_net_RegisterCallback() to register a
 *            callback fuction used to process events from the QWF NET MGR
 *            component. Note: similar callbacks will be registered for each
 *            service the application is interested in.
 *          - Client application calls the qwf_ProcessEvents() function to
 *            block and wait for events to be sent to each services callback
 *            which was registered during initialization
 *          - The client application processes the events as required.
 *
 *          For code examples, refer to @ref QWF_EXAMPLES_NET
 */

#ifndef _QWF_NET_H_INCLUDED
#define _QWF_NET_H_INCLUDED

#include <qwf/qwf_interface.h>
#include <stdbool.h>
#include <netinet/in.h>

__BEGIN_DECLS

/**
 * @defgroup QWF_NET QWF Network Manager Service
 * @{
 */


#define QWF_NET_MAX_IF_NAME_LEN     (15)    ///< Defines the maximun length of the interface name (without NULL terminator)
#define QWF_NET_MAX_NUM_IFACES      (50)    ///< Defines the maximun number of interfaces shown in an array of interfaces
#define QWF_NET_MAX_MAC_ADDR_LEN    (17)    ///< Defines the maximun length of the MAC address string (without NULL terminator)
#define QWF_NET_MAX_DNS_NAME_LEN    (255)   ///< Defines the maximun length of the DNS name string (without NULL terminator)
#define QWF_NET_NUM_DNS_ADDRS       (2)     ///< Defines the number of DNS name servers

typedef char qwf_net_IfName_t[QWF_NET_MAX_IF_NAME_LEN + 1];   ///< Defines the typedef to use with interface name strings.
typedef char qwf_net_IpAddrStr_t[INET6_ADDRSTRLEN + 1];   ///< Defines the typedef to use with IP address strings.

/**
 * @brief Return values used by QWF Network Manager Service interface
 *
 * @details This enumeration defines the possible NET return values.
 */
typedef enum
{
    NET_OK                 = 0,          ///< Successful result
    NET_ERR_INT            = 1,          ///< Failure - Internal
    NET_ERR_PARAM          = 2,          ///< Failure - Invalid parameter
    NET_ERR_INACTIVE       = 3,          ///< Failure - Inactive service
    NET_ERR_EXISTS         = 4           ///< Already Exists
} qwf_net_Results_e;

/**
 * @brief Values used by QWF Network Manager Service interface to indicate the
 *        "type" of network interface
 *
 */
typedef enum
{
    NET_TYPE_NONE               = 0,   ///< Not initialized or Unknown
    NET_TYPE_CELLULAR           = 1,   ///< Any cellular network interface
    NET_TYPE_WIRED              = 2,   ///< Any wired Ethernet interface
    NET_TYPE_WIFI               = 3,   ///< Any wireless network interface
    NET_TYPE_VPN                = 4,   ///< Any VPN tunnel
    NET_TYPE_USB                = 5,   ///< Any direct USB connection
    NET_TYPE_BT                 = 6    ///< Any bluetooth connection

} qwf_net_IfType_e;

/**
 * @brief Values used by QWF Network Manager Service interface to indicate the
 *        "type" of IP Configuration used for network interface
 *
 */
typedef enum
{
    NET_IP_CONFIG_OFF          = 0,   ///< Not considered for IP selection
    NET_IP_CONFIG_MANUAL       = 1,   ///< IP Configured manually by user
    NET_IP_CONFIG_DHCP_ON      = 2    ///< IP Configured by DHCP service

} qwf_net_IpConfigMethod_e;

/**
 * @brief Event codes for Network Manager service events.
 *
 * @details This enumeration defines the possible NET MGR event types.
 */
typedef enum
{
    /**
     * This event indicates that the current network interface being used
     * (default) has changed to a different network interface.
     * The event contains the new default interface as a string.
     */
    NET_EVENT_DEFAULT_IFACE_CHANGED,
    /**
     * This event indicates that one of the network interfaces has had it's status
     * or data modified. The event contains a structure containing the network
     * interface information ::qwf_net_InterfaceData_t. At startup, an initial
     * ::NET_EVENT_IFACE_UPDATED event is sent for each network interface
     * discovered.
     */
    NET_EVENT_IFACE_UPDATED,
    /**
     * This event indicates that one of the network interfaces has been removed or
     * destroyed. The event contains the removed interface name as a string.
     */
    NET_EVENT_IFACE_REMOVED,
    /**
     * This event indicates that the current network interface priority list has
     * been modified. The priority is used to determine which interface is active
     * when more than one are available. The event contains the new priority list
     * as an array of interface strings.
     */
    NET_EVENT_INTERFACE_PRIORITY,
    /**
     * This event indicates that the Network Manager subsystem is no longer
     * actively monitoring status changes for the network interfaces.
     */
    NET_EVENT_NET_SERVICE_INACTIVE,
    /**
     * This event indicates that the Network Manager subsystem is actively
     * monitoring status for the Network Manager service.
     */
    NET_EVENT_NET_SERVICE_ACTIVE

} qwf_net_EventCode_e;

/**
 * @brief Defines the structure used to pass network interface information to
 *        client
 */
typedef struct qwf_net_InterfaceData_s
{
    /**
     * Network interface name as string
     */
    qwf_net_IfName_t  name;
    /**
     * Network interface type as string
     */
    qwf_net_IfType_e  ifType;
    /**
     * Indicates if the network interface is connected
     */
    bool connected;
    /**
     * Indicates if the physical interface is up
     */
    bool physicalUp;
    /**
     * Indicates if the network interface has an IP address
     */
    bool hasIpAddr;
    /**
     * Indicates if the network interface has IPv4 address
     */
    bool ip4;
    /**
     * Indicates if the network interface has IPv6 address
     */
    bool ip6;
    /**
     * The FIB (Forwarding Information Base) for the network interface
     */
    uint8_t fib;
    /**
     * The MTU (Maximum Transmission Unit) for the network interface
     */
    uint16_t mtu;
    /**
     * The IPv4 address (if available) for the network interface as string
     */
    qwf_net_IpAddrStr_t ip4Address;
    /**
     * The IPv4 netmask (if IPv4 available) for the network interface as string
     */
    qwf_net_IpAddrStr_t ip4Netmask;
    /**
     * The IPv4 gateway for the network interface as string
     */
    qwf_net_IpAddrStr_t ip4Gateway;
    /**
     * The IPv6 address (if available) for the network interface as string
     */
    qwf_net_IpAddrStr_t ip6Address;
    /**
     * The IPv6 network prefix length (if IPv6 available) for the network
     * interface as a length (number of bits used for mask).
     * Maximum prefix length is 128.
     */
    uint8_t             ip6NetPrefixLen;
    /**
     * The IPv6 gateway for the network interface as string
     */
    qwf_net_IpAddrStr_t ip6Gateway;
    /**
     * The broadcast IP address for the network interface as string
     */
    qwf_net_IpAddrStr_t broadcast;
    /**
     * The MAC address for the network interface as string
     */
    char mac[QWF_NET_MAX_MAC_ADDR_LEN + 1];
    /**
     * The IPv4 primary and secondary name servers for the network
     * interface as an array of strings
     */
    qwf_net_IpAddrStr_t ip4NameServers[QWF_NET_NUM_DNS_ADDRS];
    /**
     * The IPv6 primary and secondary name servers for the network
     * interface as an array of strings
     */
    qwf_net_IpAddrStr_t ip6NameServers[QWF_NET_NUM_DNS_ADDRS];
    /**
     * The search domain name(s) for the network interface as string.
     * Informational only. Multiple Search Domain names will be represented as
     * comma delimeted in the string
     */
    char searchDomain[QWF_NET_MAX_DNS_NAME_LEN + 1];
    /**
     * Indicates if the IPv4 interface settings configured manually or if DHCP
     * settings applied. If interface is not to be considered for IPv4 selection,
     * value is set to "off".
     */
    qwf_net_IpConfigMethod_e ip4ConfigMethod;
    /**
     * Indicates if the IPv6 interface settings configured manually or if DHCP
     * settings applied. If interface is not to be considered for IPv6 selection,
     * value is set to "off".
     */
    qwf_net_IpConfigMethod_e ip6ConfigMethod;
    /**
     * Indicates if the interface is being managed internally within QWF Framework
     */
    bool unmanaged;

} qwf_net_InterfaceData_t;

/**
 * @brief Defines the structure used to hold an array of inteface names
 */
typedef struct qwf_net_IfaceList_s
{
    /**
     *  Number of interfaces contained in the priority list
     */
    uint8_t numIfaces;
    /**
     *  Array of network interface Names
     */
    qwf_net_IfName_t ifName[QWF_NET_MAX_NUM_IFACES];

} qwf_net_IfaceList_t;

/**
 * @brief Defines the structure used when passing NET MGR events from NET MGR
 * API to the client
 */
typedef struct qwf_net_Event_s
{
    /**
     *  The actual event to be processed
     */
    qwf_net_EventCode_e eventCode;
    /**
     *  The client callback to process the event
     */
    void *pClientCB;
    /**
     *  Union of all the possible Event data members
     */
    union
    {
        /**
         *  Default interface for event ::NET_EVENT_DEFAULT_IFACE_CHANGED
         */
        qwf_net_IfName_t defaultIface;
        /**
         *  Interface priority list for event ::NET_EVENT_INTERFACE_PRIORITY
         */
        qwf_net_IfaceList_t priorityList;
        /**
         *  Interface priority list for event ::NET_EVENT_IFACE_UPDATED
         */
        qwf_net_InterfaceData_t ifData;
        /**
         *  Network interface for event ::NET_EVENT_IFACE_REMOVED
         */
        qwf_net_IfName_t  removedIface;

    } netEvent;

} qwf_net_Event_t;

/**
 * @brief Defines the structure used to pass data to the NET MGR to create the
 * Network Connection via interface qwf_net_Connect()
 * Note: Currently only supporting IPv4
 */
typedef struct qwf_net_ConnectData_s
{
    /**
     * Network interface name as string
     */
    qwf_net_IfName_t    ifName;
    /**
     * Network interface type as string
     */
    qwf_net_IfType_e    ifType;
    /**
     * Indicates if the IPv4 interface settings configured manually or if DHCP
     * settings applied. If interface is not to be considered for IPv4 selection
     * value is set to "off".
     */
    qwf_net_IpConfigMethod_e ip4ConfigMethod;
    /**
     * Indicates if the IPv6 interface settings configured manually or if DHCP
     * settings applied. If interface is not to be considered for IPv6 selection
     * value is set to "off".
     */
    qwf_net_IpConfigMethod_e ip6ConfigMethod;
    /**
     * The IPv4 address (if available) for the network interface as string
     */
    qwf_net_IpAddrStr_t ip4Address;
    /**
     * The IPv6 address (if available) for the network interface as string
     */
    qwf_net_IpAddrStr_t ip6Address;
    /**
     * The IPv4 netmask (if IPv4 available) for the network interface as string
     */
    qwf_net_IpAddrStr_t ip4Netmask;
    /**
     * The IPv6 network prefix length (if IPv6 available) for the network
     * interface as a length (number of bits used for mask).
     * Maximum prefix length is 128.
     */
    uint8_t             ip6NetPrefixLen;
    /**
     * The IPv4 gateway for the network interface as string
     */
    qwf_net_IpAddrStr_t ip4Gateway;
    /**
     * The IPv6 gateway for the network interface as string
     */
    qwf_net_IpAddrStr_t ip6Gateway;
    /**
     * The IPv4 primary and secondary name servers for the network
     * interface as an array of strings.
     */
    qwf_net_IpAddrStr_t ip4NameServers[QWF_NET_NUM_DNS_ADDRS];
    /**
     * The IPv6 primary and secondary name servers for the network
     * interface as an array of strings.
     */
    qwf_net_IpAddrStr_t ip6NameServers[QWF_NET_NUM_DNS_ADDRS];

} qwf_net_ConnectData_t;

/**
 * @brief Client's function prototype used when registering callback function
 *        to process generated NET MGR related events.
 *
 * Callback registered by the client to notify when an event has been
 * generated. This function is called if there is a NET MGR event in the event
 * queue to be passed to client.
 *
 * @param [in]  pCtx Pointer to Service context.
 * @param [in]  pEvent Pointer to NET MGR event data.
 * @param [in]  pClientData  Pointer to client's data to be used with this
 *             event handler Note: this is optional to allow client to
 *             associate an extra data structure with this context. NET MGR
 *             service simply returns this pointer back to the client. NULL is
 *             a valid value indicating not used.
 *
 */
typedef void (*qwf_net_EventClientCallback_t)(qwf_Context_t *pCtx,
                                              qwf_net_Event_t *pEvent,
                                              void *pClientData);

/**
 * @brief Connect the specified Network Interface
 *
 * Interface to notify the NET MGR that a network interface is connected and
 * available. Contains the connected interface and networking parameters as
 * provided by the "ifconfig" utility.
 *
 * @param [in]  pCtx  Pointer to Service context.
 * @param [in]  pData  Pointer to ::qwf_net_ConnectData_t structure with the
 *        name of the Network Interface and networking parameters
 *
 * @return ::NET_OK               Successful result
 * @return ::NET_ERR_INACTIVE     Service inactive or not ready
 * @return ::NET_ERR_INT          Internal Error
 * @return ::NET_ERR_PARAM        Invalid parameter
 *
 */
qwf_net_Results_e qwf_net_Connect(qwf_Context_t *pCtx,
                                  qwf_net_ConnectData_t *pData);

/**
 * @brief Interface to notify the NET MGR that a network interface is
 *        disconnected
 *
 * @param [in]  pCtx  Pointer to Service context.
 * @param [in]  pIfName   Pointer to ::qwf_net_IfName_t structure with the
 *        name of the Network Interface
 *
 * @return ::NET_OK               Successful result
 * @return ::NET_ERR_INACTIVE     Service inactive or not ready
 * @return ::NET_ERR_INT          Internal Error
 * @return ::NET_ERR_PARAM        Invalid parameter
 *
 */
qwf_net_Results_e qwf_net_Disconnect(qwf_Context_t *pCtx, qwf_net_IfName_t *pIfName);

/**
 * @brief Return the default Network Interface name being used by device
 *
 * @param [in]  pCtx  Pointer to Service context.
 * @param [out]  pIfName   Reference pointer to ::qwf_net_IfName_t structure
 *        to store the name of the default Network Interface as string
 *
 * @return ::NET_OK               Successful result
 * @return ::NET_ERR_INACTIVE     Service inactive or not ready
 * @return ::NET_ERR_INT          Internal Error
 * @return ::NET_ERR_PARAM        Invalid parameter
 *
 */
qwf_net_Results_e qwf_net_GetDefaultInterface(qwf_Context_t *pCtx,
                                              qwf_net_IfName_t *pIfName);
/**
 * @brief Return the Interface Data for specified Network Interface Name
 *
 * @param [in]  pCtx  Pointer to Service context.
 * @param [in]  pIface   Pointer to ::qwf_net_IfName_t interface name
 * @param [out] pData   Reference pointer to ::qwf_net_InterfaceData_t
 *        structure to store the networking parameters for specified Network
 *        Interface
 *
 * @return ::NET_OK               Successful result
 * @return ::NET_ERR_INACTIVE     Service inactive or not ready
 * @return ::NET_ERR_INT          Internal Error
 * @return ::NET_ERR_PARAM        Invalid parameter
 *
 */
qwf_net_Results_e qwf_net_GetInterfaceData(qwf_Context_t *pCtx,
                                           qwf_net_IfName_t *pIface,
                                           qwf_net_InterfaceData_t *pData);

/**
 * @brief Return the Network Priority List used by Network Manager
 *
 * @param [in]  pCtx  Pointer to Service context.
 * @param [out] pList   Reference pointer to ::qwf_net_IfaceList_t structure
 *        to store the Priority List data
 *
 * @return ::NET_OK               Successful result
 * @return ::NET_ERR_INACTIVE     Service inactive or not ready
 * @return ::NET_ERR_PARAM        Invalid parameter
 *
 */
qwf_net_Results_e qwf_net_GetPriorityList(qwf_Context_t *pCtx,
                                          qwf_net_IfaceList_t * const pList);

/**
 * @brief Return a list of interfaces known by Network Manager
 *
 * @param [in]  pCtx  Pointer to Service context.
 * @param [out] pList   Reference pointer to ::qwf_net_IfaceList_t structure
 *        to store the array of interface names
 *
 * @return ::NET_OK               Successful result
 * @return ::NET_ERR_INACTIVE     Service inactive or not ready
 * @return ::NET_ERR_PARAM        Invalid parameter
 *
 */
qwf_net_Results_e qwf_net_GetAvailableInterfaces(qwf_Context_t *pCtx,
                                                 qwf_net_IfaceList_t * const pList);
/**
 * @brief Register a callback for the NET MGR service.
 *
 * @param [in]  pCtx  Pointer to Service context.
 * @param [in]  eventHandler   The callback to handle cellular data events.
 * @param [in]  pClientData  Pointer to client's data to be used with this
 *             event handler Note: this is optional to allow client to
 *             associate an extra data structure with this context. NET MGR
 *             service simply returns this pointer back to the client. NULL is
 *             a valid value indicating not used.
 *
 * @return ::NET_OK               Successful result
 * @return ::NET_ERR_INACTIVE     Service inactive
 * @return ::NET_ERR_INT          Internal Error
 * @return ::NET_ERR_PARAM        Invalid parameter
 *
 */
qwf_net_Results_e qwf_net_RegisterCallback(const qwf_Context_t * const pCtx,
                                           qwf_net_EventClientCallback_t eventHandler,
                                           void *pClientData);


/** @}*/ // end group QWF_NET

__END_DECLS

#endif /* _QWF_NET_H_INCLUDED */

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/wireless_framework/branches/7.1/trunk/common/lib/qwf_interface/public/qwf/qwf_net.h $ $Rev: 2797 $")
#endif

