/*
 * $QNXLicenseC:
 * Copyright 2015-2019 QNX Software Systems. All Rights Reserved.
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
 * @brief  This interface provides access to the Cellular Data Service (CDS).
 *
 * @details This interface gives client the ability to control cellular data
 *          logical connections and retrieve their associated configuration
 *          and state information.
 *
 *          A logical connection is an abstraction above a physical cellular
 *          access point. Carrier can provide multiple access points over
 *          their network that can be used to send data for different
 *          purposes. Clients of CDS interact with CDS using well known
 *          logical connection names. They do not care about how logical
 *          connections are configured with cellular data access parameters
 *          such APN, IP type, etc.
 *
 *          The Carrier Data Configuration XML (/etc/carrier/config/"carrier
 *          id".xml) is used to configure a list of logical connections with
 *          carrier specific cellular data connection parameters.
 *          The actual logical connections names are transparent to CDS except
 *          for "internet". Example well-known logical connection names are:
 *          - "internet" which represents the default connection installed
 *              into the default route
 *          - "mms" which typically represents an access point to the network
 *            for exclusive mms data traffic
 *          - "carrier_apps" which typically represents a zero rated connection
 *            that authorized carrier applications use
 *
 *          A networking interface is associated with connected/activated
 *          logical connection. By default most processes are not concerned
 *          about how their packets are routed and simply let the networking
 *          use the default routing mechanisms. However, if a process is
 *          required to route traffic over a specific connection, it binds its
 *          sockets to the networking interface for that connection. The
 *          networking interface associated with the "internet" connection is
 *          always in the default route.
 *
 *          When data service mode is set to ::CDS_DATA_SERVICE_MODE_OFF,
 *          logical connections are disconnected to stop carrying cellular data
 *          traffic. However, it does not make sense to stop data traffic on a
 *          logical connection which supports voip service in this case. Logical
 *          connection can be configured to ignore data service off and stays up
 *          even when data service mode is set to ::CDS_DATA_SERVICE_MODE_OFF.
 *
 *          Clients interact with CDS via this interface in order to
 *          - activate/deactivate a logical connection
 *          - obtain notification on logical connection status changes
 *          - retrieve dynamic information related to the logical connections
 *          - retrieve configuration information related to the logical
 *            connections
 *          - set data service mode to allow/disallow data connections
 *          - set data roaming mode to allow/disallow data connections while
 *            roaming
 *          - affect modem device internal routing management (@b NOTE: Not supported by all modem devices)
 *
 *          Usage:
 *          - Client application calls qwf_Init() to initialize the QWF
 *            Context data used as container for QWF and service data (like
 *            CDS)
 *          - Client application calls qwf_cds_RegisterCallback() to
 *            register a callback fuction used to process events from the QWF
 *            CDS component. @b Note: simlar callbacks will be registered for each
 *            service the application is interested in.
 *          - Client application calls the qwf_ProcessEvents() function to
 *            block and wait for events to be sent to each services callback
 *            which was registered during initialization
 *          - The client application processes the events as required.
 *
 *          For code examples, refer to @ref QWF_EXAMPLES_CDS
 *
 *          More information regarding the QWF CDS service may be obtained
 *          from the "QWF Architecture and Users Guide".
 */

#ifndef _QWF_CDS_H_INCLUDED
#define _QWF_CDS_H_INCLUDED

#include <qwf/qwf_interface.h>
#include <stdbool.h>
#include <netinet/in.h> // struct in_addr

__BEGIN_DECLS

/**
 * @defgroup QWF_CDS QWF Cellular Data Service
 * @{
 */


/**
 * Max length for Logical Connections name
 */
#define QWF_CDS_MAX_CONNECTION_NAME_LEN (32)
/**
 * Max length of APN for cellular data connection
 */
#define QWF_CDS_MAX_APN_LEN (98)
/**
 * Max length of networking interface name associated with the named logical
 * connection
 */
#define QWF_CDS_MAX_NETIF_NAME_LEN (128)
/**
 * Max length for User Name for Cellular Connection
 */
#define QWF_CDS_MAX_USER_NAME_LEN (63)
/**
 * Max length for Password for Cellular Connection
 */
#define QWF_CDS_MAX_PASSWORD_LEN (63)
/**
 * Max string length for a modem device raw route command
 */
#define QWF_CDS_MAX_MODEM_ROUTE_CMD_LEN (135)
/**
 * Max number of modem device route entries in the modem device's internal routing table
 */
#define QWF_CDS_MAX_MODEM_ROUTE_ENTRIES (30)
/**
 * Defines the typedef to use with IP address strings.
 */
typedef char qwf_cds_IpAddrStr_t[INET6_ADDRSTRLEN + 1];


/**
 * @brief Return values used by CDS service interface
 *
 * @details This enumeration defines the possible CDS return values.
 */
typedef enum
{
    CDS_OK                = 0,         ///< Successful result
    CDS_ERR_INT           = 1,         ///< Failure - Internal
    CDS_ERR_PARAM         = 2,         ///< Failure - Invalid parameter
    CDS_ERR_BUSY          = 3,         ///< Failure - System in use
    CDS_ERR_EXIST         = 4,         ///< Failure - Connection already exisits
    CDS_ERR_NOT_ALLOWED   = 5,         ///< Failure - Connection Not Allowed
    CDS_ERR_ALWAYS_ON     = 6,         ///< Connection can't disconnect as "Always On"
    CDS_ERR_DISCONNECTED  = 7,         ///< Connection can't disconnect as already disconnected
    CDS_ERR_CONN_UNAVAIL  = 8,         ///< Connection Not Available (Interim state if Data Service not established)
    CDS_ERR_INACTIVE      = 9,         ///< Failure - CDS service is inactive
    CDS_ERR_NOT_SUPPORTED = 10         ///< Function not supported by modem and/or CRM
} qwf_cds_Results_e;

/**
 * @brief Cellular Data Service logical connection names.
 *
 * @details A logical connection comes to existence after its configuration is
 *          loaded into CDS. This enumeration type defines the different
 *          logical connections that can be made.
 */
typedef enum
{
    /**
     * Logical connection name for default internet service. This typically is
     * an always-on connection. This service's name is "internet".
     */
    QWF_CDS_CONNECTION_INTERNET,
    /**
     * Logical connection name for MMS service. This service's name is "mms".
     */
    QWF_CDS_CONNECTION_MMS,
    /**
     * Logical connection name for Emergancy service. This service's name is "emergency".
     */
    QWF_CDS_CONNECTION_EMERGENCY,
    /**
     * Logical connection name for Tethering service. This service's name is "tethering".
     */
    QWF_CDS_CONNECTION_TETHERING,
    /**
     * Logical connection for a first custom service, which has no pre-designated
     * purpose and, instead, is intended for use for an application's custom service.
     * This service's name is "custom1".
     */
    QWF_CDS_CONNECTION_CUSTOM1,
    /**
     * Logical connection for a second custom service, which has no pre-designated
     * purpose and, instead, is intended for use for an application's custom service.
     * This service's name is "custom2".
     */
    QWF_CDS_CONNECTION_CUSTOM2

} qwf_cds_ConnectionName_e;

/**
 * @brief Cellular data service logical connection states.
 *
 * @details A logical connection comes to existence after its configuration is
 *          loaded into CDS. The state of logical connection is represented by
 *          the linear states defined below.
 */
typedef enum
{
    /**
     * Logical connection is configured but is disabled. It currently does not
     * provide any service.
     */
    CDS_CONNECTION_DISABLED,

    /**
     * Logical connection has not been requested to connect by a client. The
     * connection may not be "connectable" if cellular connectivity is not currently
     * allowed for variety of reasons such as data service off, data roaming rule,
     * etc.
     */
    CDS_CONNECTION_DISCONNECTED,

    /**
     * Logical connection activation has been requested. Activation is in process.
     * If activation is rejected by the network for some non critical cause, the
     * radio layer actively retries activation with its back-off algorithm.
     */
    CDS_CONNECTION_CONNECTING,

    /**
     * Logical connection activation has been requested, but the request cannot be
     * satisfied at the moment. When the condition allows activation again, retry
     * activation is initiated by the radio layer automatically.
     */
    CDS_CONNECTION_ON_HOLD,

    /**
     * Logical connection is activated and available for passing data traffic. A
     * networking interface is associated with this logical connection now. This
     * networking interface is bound to the @c SOCK_SO_BINDTODEVICE environment
     * variable in the current process if the autobind parameter is set to true
     * when qwf_cds_Disconnect() is called.
     */
    CDS_CONNECTION_CONNECTED,

    /**
     * Logical connection is activated but data temporarily cannot be routed. This
     * normally happens when the device goes out of cellular coverage with an
     * active cellular data connection.
     */
    CDS_CONNECTION_CONNECTED_FLOW_CONTROLLED,

    /**
     * Logical connection is about to be disconnected with a grace period.
     * Full cellular data connectivity is still available during this grace
     * period. This allows session based client (e.g. vvpn) to properly close its
     * session with remote server over the cellular data connection.
     */
    CDS_CONNECTION_PENDING_DISCONNECT,

    /**
     * Logical connection does not exist.
     */
    CDS_CONNECTION_INVALID_STATE

} qwf_cds_ConnectionState_e;
/**
 * @brief Cellular data service logical connection disconnect reasons.
 *
 * @details The reason the logical connection become disconnected.
 */
typedef enum
{
    /**
     * Disconnect reason is unknown or connection is active
     */
    CDS_CONNECTION_DISCONNECT_NONE,

    /**
     * Normal disconnect of the logical connection
     */
    CDS_CONNECTION_DISCONNECT_NORMAL,

    /**
     * Logical connection was disconnected or rejected by network due to a invalid
     * APN or service not supported for the specified APN
     */
    CDS_CONNECTION_DISCONNECT_APN_REJECTED,

} qwf_cds_DisconnectReason_e;

/**
 * @brief Cellular data service logical connection status.
 *
 * @details Data structure used to track the status of a given cellular data
 *          logical connection.
 */
typedef struct qwf_cds_ConnectionStatus_s
{
    /**
     * Name of the logical connection that has changed status.
     */
    char connectionName[QWF_CDS_MAX_CONNECTION_NAME_LEN+1];

    /**
     * Indicates if the connection is access controlled, i.e., if the connection
     * is not shared with a non-access controlled connection, the network
     * interface route will only allow processes that successfully called connect
     * to pass traffic.
     */
    bool accessControlled;

    /**
     * Indicates if the logical connection is always on, i.e., is automatically requested by CDS if the connection
     * is allowed.
     */
    bool alwaysOn;

    /**
     * Current state of the named logical connection.
     */
    qwf_cds_ConnectionState_e state;

    /**
     * Null terminated APN configured for the named logical connection.
     * This is only valid in CDS_CONNECTION_CONNECTED state.
     */
    char apn[QWF_CDS_MAX_APN_LEN+1];

    /**
     * Null terminated networking interface name associated with the named logical connection
     * This is only valid in CDS_CONNECTION_CONNECTED state.
     */
    char networkingInterfaceName[QWF_CDS_MAX_NETIF_NAME_LEN+1];

    /**
     * Reason the logical connection became disconnected.
     * This is only valid in CDS_CONNECTION_DISCONNECTED state.
     */
    qwf_cds_DisconnectReason_e disconnectReason;

} qwf_cds_ConnectionStatus_t;

/**
 * @brief Event codes for cellular data events.
 *
 * @details This enumeration defines the possible cellular data event types.
 */
typedef enum
{
    /**
     * This event indicates that the CDS subsystem is no longer actively
     * monitoring status changes for the CDS.
     */
    CDS_EVENT_DATA_SERVICE_INACTIVE,

    /**
     * This event indicates that the CDS subsystem is actively monitoring status
     * changes for the CDS.
     */
    CDS_EVENT_DATA_SERVICE_ACTIVE,

    /**
     * This event indicates that the logical connection state has changed.
     */
    CDS_EVENT_CONNECTION_STATE_CHANGE,

    /**
     * This event indicates that a new logical connection has been defined. This
     * can happen after a new SIM has been inserted into the device.
     */
    CDS_EVENT_CONNECTION_NEW,

    /**
     * This event indicates one of the defined logical connection no
     * longer exists. This can happen after a SIM has been removed from
     * the device.
     */
    CDS_EVENT_CONNECTION_ENDED,

    /**
     * The cellular data service status on the device has changed. Refers to
     * ::qwf_cds_DataServiceStatus_e.
     * Client can use qwf_cds_GetDataServiceStatus() to query the new data
     * status.
     */
    CDS_EVENT_DATA_SERVICE_STATUS_CHANGE,

    /**
     * The device data roaming state has changed.
     * Refers to ::qwf_cds_DataRoamingStatus_e.
     * Client can use qwf_cds_GetDataRoamingStatus() to query the new
     * roaming status.
     */
    CDS_EVENT_DATA_ROAMING_STATUS_CHANGE,

    /**
     * The cellular data service mode on the device has changed.
     * Refers to ::qwf_cds_DataServiceMode_e.
     * Client can use qwf_cds_GetDataServiceMode() to query the new data
     * status.
     */
    CDS_EVENT_DATA_SERVICE_MODE_CHANGE,

    /**
     * The device data roaming mode has changed.
     * Refers to ::qwf_cds_DataRoamingMode_e.
     * Client can use qwf_cds_GetDataRoamingMode() to query the new roaming
     * mode.
     */
    CDS_EVENT_DATA_ROAMING_MODE_CHANGE,

    /**
     * The device user enabled data roaming status has changed.
     * Refers to ::qwf_cds_UserEnabledDataRoamingState_e.
     */
    CDS_EVENT_DATA_USER_ENABLED_DATA_ROAMING_CHANGE

} qwf_cds_EventCode_e;

/**
 * @brief Enumeration of possible cellular data service mode.
 *
 * Data service mode is considered as the "master" switch for data services
 * being enabled or not. If OFF then no data connections will be allowed.  If
 * ON, data connections are allowed subject to data roaming rules.
 *
 */
typedef enum
{
    /**
     * Cellular data services are enabled.
     * Data connections are allowed subject to data roaming rules.
     */
    CDS_DATA_SERVICE_MODE_ON,

    /**
     * Cellular data services are disabled.
     * Data connections are not allowed.
     */
    CDS_DATA_SERVICE_MODE_OFF

} qwf_cds_DataServiceMode_e;

/**
 * @brief Enumeration of possible cellular data service states.
 *
 * This enumeration defines the data service states. Client can call
 * qwf_cds_GetDataServiceStatus() to retrieve the current state.
 * States can be one of the following:
 * - ::CDS_DATA_ON: data services are enabled and always on connections have
 *   been activated by CDSC. On demand connects can be requested.
 * - ::CDS_DATA_OFF: data services are disabled and deactivated.  On demand
 *   connections will be refused.
 * - ::CDS_OFF_ROAMING: same as OFF except date services were turned off because
 *   device entered a data roaming state. Data services maybe re-enabled via
 *   the API qwf_cds_UserEnableDataRoaming().
 * .
 */
typedef enum
{
    /**
     * Cellular data services are enabled.
     * Always on connections have been activated by CDS.
     * On demand connects can be requested.
     */
    CDS_DATA_ON,

    /**
     * Cellular data services is off. Based on the Carrier, certain services may
     * still be available to connect for data.
     *
     * Cellular data services are disabled. Connections are deactivated.  On
     * demand connections will be refused.
     * Exception: Based on the roaming connectivity policy specified in the
     * carrier configuration against individual logical connections, certain
     * logical connections still allow client to connect even when the overall
     * data services are turned off.
     */
    CDS_DATA_OFF,

    /**
     * Same as OFF except date services were turned off because device entered a
     * data roaming state. Data services maybe re-enabled via the
     * qwf_cds_UserEnableDataRoaming() interface function.
     */
    CDS_OFF_ROAMING,

    /**
     * Unknown cellular data service status.
     */
    CDS_DATA_SERVICE_STATUS_UNKNOWN

} qwf_cds_DataServiceStatus_e;

/**
 * @brief Enumeration of possible cellular data roaming statuses.
 *
 * This enumeration defines cellular data roaming status. Client can call
 * qwf_cds_GetDataRoamingStatus() interface function to retrieve the current
 * status.
 *
 */
typedef enum
{
    /**
     * The device is not data roaming.
     */
    CDS_NOT_DATA_ROAMING,

    /**
     * The device is data roaming.
     */
    CDS_DATA_ROAMING,

    /**
     * Unknown data roaming status.
     */
    CDS_DATA_ROAMING_STATUS_UNKOWN

} qwf_cds_DataRoamingStatus_e;

/**
 * @brief Enumeration of data roaming mode
 *
 * Data roaming mode controls if data connections are allowed when in
 * roaming networks. Client can call qwf_cds_GetDataRoamingMode()
 * to retrieve the current mode.
 */
typedef enum
{
    /**
     * Data roaming is enabled.
     */
    CDS_DATA_ROAMING_MODE_ON,

    /**
     * Data roaming is disabled.
     */
    CDS_DATA_ROAMING_MODE_OFF,

    /**
     * Data roaming is disabled. If in roaming networks, current data connections
     * are dropped right away. The client can decide to enable data while roaming
     * or not. The selection can be sent back via CDS API
     * qwf_cds_UserEnableDataRoaming().
     */
    CDS_DATA_ROAMING_MODE_PROMPT

} qwf_cds_DataRoamingMode_e;

/**
 * @brief Enumeration of user enabled data roaming state
 *
 * User enabled data roaming state indicates whether or not data
 * roaming has been explicitly enabled or disabled via
 * qwf_cds_UserEnableDataRoaming().
 * Data roaming mode controls whether data connections are allowed when in
 * roaming networks. When the mode is set to "prompt" the client can get input
 * from user to determine if data connections should be allowed while roaming.
 * The Data roaming state then indicates whether or not data roaming has been
 * explicitly enabled or disabled by user and is set via the API function
 * qwf_cds_UserEnableDataRoaming().
 *
 * Client can call qwf_cds_GetDataServiceStatus() to retrieve the current data
 * roaming state.
 */
typedef enum
{
    /**
     * Data roaming is disabled explicitly user.
     */
    CDS_USER_DISABLED_DATA_ROAMING,

    /**
     * Data roaming is enabled explicitly by user.
     */
    CDS_USER_ENABLED_DATA_ROAMING,

    /**
     * There is no user response to the UI prompt for data roaming decision.
     */
    CDS_NO_USER_SELECTION_FOR_DATA_ROAMING

} qwf_cds_UserEnabledDataRoamingState_e;

/**
 * @brief Enumeration of Data Unit used for statistic reporting
 *
 * Client specifies data unit used when calling qwf_cds_GetServiceStats()
 * to retrieve the current data sent and received on logical connection.
 *
 */
typedef enum
{
    /**
     * Data statistics reported in bytes
     */
    CDS_DATA_UNIT_BYTES,
    /**
     * Data statistics reported in Kilobytes
     */
    CDS_DATA_UNIT_KBYTES,
    /**
     * Data statistics reported in Megabytes
     */
    CDS_DATA_UNIT_MBYTES,
    /**
     * Data statistics reported in Gigabytes
     */
    CDS_DATA_UNIT_GBYTES

} qwf_cds_DataUnit_e;

/**
 * @brief Enumeration of possible cellular data authentication types.
 *
 * This enumeration defines cellular data authentication types.
 *
 */
typedef enum
{
    /**
     * No authentication used
     */
    QWF_CDS_AUTH_NONE,
    /**
     * PAP authentication used
     */
    QWF_CDS_AUTH_PAP,
    /**
     * CHAP authentication used
     */
    QWF_CDS_AUTH_CHAP,
    /**
     * Authentication (PAP, CHAP or NONE) is determined by remote host
     */
    QWF_CDS_AUTH_AUTO

} qwf_cds_AuthType_e;

/**
 * @brief Enumeration of possible APN Storage types.
 *
 * This enumeration defines cellular data storage location for the APN
 * information used by a PDP Context.
 *
 */
typedef enum
{
    /**
     * APN information is stored in NVM on connected modem
     */
    QWF_CDS_APN_MODEM_NVM,
    /**
     * APN information is stored in NVM on host
     */
    QWF_CDS_APN_HOST_NVM
} qwf_cds_ApnStorageType_e;

/**
 * @brief Enumeration of possible cellular data IP Address types.
 *
 * This enumeration defines cellular data IP Address types.
 *
 */
typedef enum
{
    QWF_CDS_IPTYPE_IPV4,
    QWF_CDS_IPTYPE_IPV4V6,
    QWF_CDS_IPTYPE_IPV6
} qwf_cds_IPType_e;

/**
 * @brief Modem device routing table commands
 *
 * This enumeration defines the possible route commands that affect the modem
 * device's routing table.  Support for route commands is device-specific, and
 * not all modem devices support this functionality.
 *
 * @b NOTE: The arguments to a ::CDS_MODEM_ROUTE_CMD_DELETE command must match the
 *          equivalent arguments for the ::CDS_MODEM_ROUTE_CMD_ADD command that added
 *          the route to the modem device's routing table.
 */
typedef enum {
    CDS_MODEM_ROUTE_CMD_ADD,    ///< Add route entries to the modem device routing table
    CDS_MODEM_ROUTE_CMD_DELETE, ///< Delete route entries from the modem device routing table
    CDS_MODEM_ROUTE_CMD_PRINT   ///< Print to the system log the current modem device routing table
} qwf_cds_ModemRouteCmd_e;

/**
 * @brief Modem device internal routing table data
 *
 * This structure defines the modem device routing data for a given logical connection name
 * specified by @c ifName (one of ::qwf_cds_ConnectionName_e).
 *
 * When sent as an argument to qwf_cds_ModemRouteCmd() to add a modem device route entry, a network
 * route will be set with the destination IP address @c ipv4dest (masked) and subnet mask @c ipv4mask
 * on the modem WAN interface corresponding to the logical connection @c ifName.
 *
 * @b NOTE: Not all modem devices support this functionality
 */
typedef struct qwf_cds_ModemRouteData_s {
    qwf_cds_ConnectionName_e ifName;   ///< CDS logical connection (service) name
    struct in_addr           ipv4dest; ///< IPv4 destination address
    struct in_addr           ipv4mask; ///< IPv4 subnet mask
} qwf_cds_ModemRouteData_t;

/**
 * @brief Modem device routing table
 *
 * This structure defines a modem device's internal routing
 * table as retrieved from the modem device.
 *
 * @c routes is an array of @c numRoutes NUL-terminated strings, each constituting
 * a modem device routing table entry as reported by the modem device.
 *
 * The Iface value is the CDS logical connection (service) name with a (wan) designation,
 * or the modem internal interface name with a (lan) designation.
 *
 * All routing table entries are returned, unedited, in this form (the header line is not included):
 *
 *   Destination     Gateway         Genmask         Flags Metric Ref    Use Iface
 *   0.0.0.0         33.125.85.21    0.0.0.0         UG    0      0        0 internet (wan)
 *   33.0.0.0        33.121.226.12   255.0.0.0       UG    0      0        0 mms (wan)
 *   33.0.0.0        0.0.0.0         255.0.0.0       U     0      0        0 internet (wan)
 *   192.168.2.0     0.0.0.0         255.255.255.0   U     0      0        0 modem (lan)
 *   192.168.90.0    0.0.0.0         255.255.255.0   U     0      0        0 eth0 (lan)
 *
 * @b NOTE: The last two entries are managed by the modem and should not be deleted.  They are
 *          included in the @c routes array for completeness. The entry
 *
 * @b NOTE: Not all modem devices support this functionality
 */
typedef struct qwf_cds_ModemRouteTable_s {
    char    routes[QWF_CDS_MAX_MODEM_ROUTE_ENTRIES]; ///< Array of modem device internal routing table entries
    uint8_t numRoutes;                               ///< Number of routing table entries in @c routes
} qwf_cds_ModemRouteTable_t;

/**
 * @brief Structure containing Cellular Data Service (CDS) Service Statistics
 *        data for one logical connection. Data sent and received is
 *        determined by ::qwf_cds_DataUnit_e specified when calling
 *        qwf_cds_GetServiceStats()
 */
typedef struct qwf_cds_ServiceStatistics_s {
    /**
     * Time idle for this connection type
     */
    int idleTime;
    /**
     * Data Sent over this connection type
     */
    int dataSent;
    /**
     * Data Received over this connection type.
     */
    int dataReceived;

} qwf_cds_ServiceStatistics_t;

/**
 * @brief Structure containing Cellular Data APN information for a logical connection.
 *
 */
typedef struct qwf_cds_ApnInformation_s {
    /**
     * APN
     */
    char apn[QWF_CDS_MAX_APN_LEN+1];
    /**
     * IP Address Type
     */
    qwf_cds_IPType_e ipType;
    /**
     * Authentication Type
     */
    qwf_cds_AuthType_e authType;
    /**
     * Authentication username (if required), set to an empty string if not used.
     */
    char username[QWF_CDS_MAX_USER_NAME_LEN+1];
    /**
     * Authentication password (if required), set to an empty string if not used.
     */
    char password[QWF_CDS_MAX_PASSWORD_LEN+1];
} qwf_cds_ApnInformation_t;

/**
 * @brief Defines the structure used for cellular network interface information
 */
typedef struct qwf_cds_InterfaceData_s
{
    /**
     * IP Address Type (IPv4, IPv6, or IPv4v6)
     */
    qwf_cds_IPType_e ipType;
    /**
     * The IPv4 address for the cellular network interface
     */
    struct in_addr   ip4Address;
    /**
     * The IPv4 netmask (if IPv4 provided) for the cellular network interface
     */
    struct in_addr   ip4Netmask;
    /**
     * The IPv4 gateway for the cellular network interface
     */
    struct in_addr   ip4Gateway;
    /**
     * The IPv6 address (if provided) for the cellular network interface
     */
    struct in_addr   ip6Address;
    /**
     * The IPv6 network prefix length (if IPv6 provided) for the cellular network
     * interface as a length (number of bits used for mask).
     * Maximum prefix length is 128.
     */
    uint8_t          ip6NetPrefixLen;
    /**
     * The IPv6 gateway for the cellular network interface
     */
    struct in_addr   ip6Gateway;
    /**
     * The primary DNS name server used for the cellular network interface
     */
    struct in_addr   nameserver1;
    /**
     * The secondary DNS name server used for the cellular network interface
     */
    struct in_addr   nameserver2;

} qwf_cds_InterfaceData_t;

/**
 * @brief Defines the structure used when passing CDS events from CDS API to
 * the Client
 */
typedef struct qwf_cds_Event_s
{
    qwf_cds_EventCode_e eventCode;  ///< The actual event to be processed

    void *pClientCB;                 ///< The client Callback to process the event

    union
    {
        /**
         *   Logical connection status information for event
         *   ::CDS_EVENT_CONNECTION_STATE_CHANGE,
         *   ::CDS_EVENT_CONNECTION_NEW or ::CDS_EVENT_CONNECTION_ENDED.
         */
        qwf_cds_ConnectionStatus_t connectionStatus;

        /**
         *  Current data service status.
         */
        qwf_cds_DataServiceStatus_e dataServiceStatus;

        /**
         *  Current data service mode.
         */
        qwf_cds_DataServiceMode_e dataServiceMode;

        /**
         *  Current data roaming status.
         */
        qwf_cds_DataRoamingStatus_e dataRoamingStatus;

        /**
         *  Current data roaming mode.
         */
        qwf_cds_DataRoamingMode_e dataRoamingMode;

        /**
         *  Current user enabled data roaming state.
         */
        qwf_cds_UserEnabledDataRoamingState_e userEnabledDataRoaming;

    } cdsEvent; ///< event payload relevant to @c eventCode

} qwf_cds_Event_t;

/**
 * @brief Client's function to process generated cellular data events.
 *
 * Callback registered by the client to notify when an event has been
 * generated. This function is called if there is a CDS event in the event
 * queue to be passed to client.
 *
 * @param [in]  pCtx Pointer to Service context.
 * @param [in]  pEvent Pointer to CDS event data.
 * @param [in]  pClientData  Pointer to client's data to be used with this
 *             event handler. @b Note: this is optional to allow client to
 *             associate an extra data structure with this context. CDS
 *             service simply returns this pointer back to the client. @c NULL is
 *             a valid value indicating not used.
 *
 */
typedef void (*qwf_cds_EventClientCallback_t)(qwf_Context_t *pCtx,
                                              qwf_cds_Event_t *pEvent,
                                              void *pClientData);

/**
 * @brief Register a callback for the cellular service for data.
 *
 * @param [in]  pCtx  Pointer to Service context.
 * @param [in]  eventHandler   The callback to handle cellular data events.
 * @param [in]  pClientData  Pointer to client's data to be used with this
 *             event handler @b Note: this is optional to allow client to
 *             associate an extra data structure with this context. CDS
 *             service simply returns this pointer back to the client. @c NULL is
 *             a valid value indicating not used.
 *
 * @return ::CDS_OK             Successful result
 * @return ::CDS_ERR_INACTIVE   Service inactive
 * @return ::CDS_ERR_INT        Internal Error
 * @return ::CDS_ERR_PARAM      Invalid parameter
 *
 */
qwf_cds_Results_e qwf_cds_RegisterCallback(const qwf_Context_t * const pCtx,
                                           qwf_cds_EventClientCallback_t eventHandler,
                                           void *pClientData);

/**
 * @brief Determine the status of a logical connection.
 *
 * This function retrieves the last known connection status of a logical
 * connection. Status returned as type ::qwf_cds_ConnectionStatus_t
 *
 * @param [in]   pCtx  Pointer to Service context.
 * @param [in]   logicalConnectionName  Logical connection to query.
 * @param [out]  pLogicalConnectionStatus  Returned status information of the
 *        logical connection.
 *
 * @return ::CDS_OK             Successful result
 * @return ::CDS_ERR_INACTIVE   Service inactive or not ready
 * @return ::CDS_ERR_PARAM      Invalid parameter
 * @return ::CDS_ERR_CONN_UNAVAIL  Connection not found (Interim state if Data
 *         Service not established)
 *
 */
qwf_cds_Results_e qwf_cds_GetConnectionStatus(const qwf_Context_t * const pCtx,
                                              qwf_cds_ConnectionName_e logicalConnectionName,
                                              qwf_cds_ConnectionStatus_t *pLogicalConnectionStatus);


/**
 * @brief Request to create a logical connection to the network.
 *
 * This function attempts to connect a cellular logical connection.
 * The connect sequence is asynchronous in that a successful result of this
 * API call only indicates the activation process has started. The actual
 * result of the connect attempt can be determined by checking the @c state
 * setting within the ::qwf_cds_ConnectionStatus_t obtained from API function
 * qwf_cds_GetConnectionStatus() and/or waiting for the corresponding
 * ::CDS_EVENT_CONNECTION_STATE_CHANGE event.
 *
 * IMPORTANT: When this function returns successfully the client is
 * expected to call qwf_cds_Disconnect() when done using the connection.
 *
 * @param [in]   pCtx  Pointer to Service context.
 * @param [in]   logicalConnectionName  Name of the logical connection.
 *
 * @return ::CDS_OK             Successful result
 * @return ::CDS_ERR_INACTIVE   Service inactive or not ready
 * @return ::CDS_ERR_INT        Internal Error
 * @return ::CDS_ERR_PARAM      Invalid parameter
 * @return ::CDS_ERR_CONN_UNAVAIL  Connection not found (Interim state if Data
 *         Service not established)
 * @return ::CDS_ERR_EXIST      Logical connection is already activated
 * @return ::CDS_ERR_NOT_ALLOWED  Logical connection not allowed (possible
 *         reasons: the board isn't registered on the carrier network or data
 *         service mode is turned off for the board).
 *
 */
qwf_cds_Results_e qwf_cds_Connect(const qwf_Context_t * const pCtx,
                                  qwf_cds_ConnectionName_e logicalConnectionName);

/**
 * @brief Request to disconnect a logical connection.
 *
 * This function attempts to deactivate a cellular logical connection.
 * A successful return with value of @c EOK means the request for disconnect
 * has been submitted successfully and is being processed; it does not mean
 * the logical connection has been disconnected successfully. A logical
 * connection may be required to remain connected for various reasons such as
 * being defined as "always on" or when it is referenced by other existing
 * connections. To determine if the logical connection has actually been
 * disconnected, you can confirm by using qwf_cds_GetConnectionStatus() and/or
 * waiting for the corresponding ::CDS_EVENT_CONNECTION_STATE_CHANGE event. The
 * "Force" parameter can be set to TRUE to ensure the connection is
 * disconnected regardless of configuration or remaining referenced
 * connections.
 *
 * The physical cellular data connection may be disconnected immediately when
 * the logical connection goes to ::CDS_CONNECTION_DISCONNECTED
 * state. This is influenced by the bearer keep alive timeout value in the
 * logical connection configuration. The intention is to reduce frequent hits
 * on network when clients activate/deactivate logical connection repeated in
 * a short time spam. Bearer keep alive timeout should be set to zero in order
 * to ensure immediate deactivation of physical connection. By default, bearer
 * keep alive timeout is set to 300 seconds (5 minutes).
 *
 *
 * @param [in]   pCtx  Pointer to Service context.
 * @param [in]   logicalConnectionName  Name of the logical connection.
 * @param [in]   force  Force the disconnect even if other references exist on
 *        connection
 *
 * @return ::CDS_OK             Successful result
 * @return ::CDS_ERR_INACTIVE   Service inactive or not ready
 * @return ::CDS_ERR_INT        Internal Error
 * @return ::CDS_ERR_PARAM      Invalid parameter
 * @return ::CDS_ERR_CONN_UNAVAIL  Connection not found (Interim state if Data
 *                              Service not established)
 * @return ::CDS_ERR_BUSY       Operation not allowed as connection has
 *                              other references remaining
 * @return ::CDS_ERR_ALWAYS_ON  Connection Can't Disconnect as Connection
 *                              defined as "Always On"
 * @return ::CDS_ERR_DISCONNECTED  Connection Can't Disconnect as Connection
 *                              has already been disconnected
 *
 */
qwf_cds_Results_e qwf_cds_Disconnect(const qwf_Context_t * const pCtx,
                                     qwf_cds_ConnectionName_e logicalConnectionName,
                                     bool force);

/**
 * @brief Request to focus a logical connection.
 *
 * This function attempts to set an on-demand logical connection in focus in
 * order to give it higher priority for cellular resources when the requested
 * connections exceeds the network limit. This could be needed as some legacy
 * cellular networks support a limited number of concurrent active logical
 * connections. Even modern networks may become temporarily resource
 * constrained. If an application can't afford to wait for the requested
 * logical connection to be activated, it can use this command to prioritize
 * its connection request.
 * @b NOTE: Prioritizing a connection may result in disconnecting one or more
 * other active connections. This command should be used sparingly. The
 * defocus command must be called on a focused connection as soon as the
 * critical transmission has completed.
 *
 * @param [in]  pCtx  Pointer to Service context.
 * @param [in]  logicalConnectionName  Name of the logical connection.
 * @param [in]  immediate Indicates whether this focus request is to be
 *              applied immediately.
 *
 * @return ::CDS_OK             Successful result
 * @return ::CDS_ERR_INACTIVE   Service inactive or not ready
 * @return ::CDS_ERR_INT        Internal Error
 * @return ::CDS_ERR_PARAM      Invalid parameter
 * @return ::CDS_ERR_CONN_UNAVAIL  Connection not found (Interim state if Data
 *         Service not established)
 *
 */
qwf_cds_Results_e qwf_cds_ConnectionFocus(const qwf_Context_t * const pCtx,
                                          qwf_cds_ConnectionName_e logicalConnectionName,
                                          bool immediate);


/**
 * @brief Request to defocus a logical connection.
 *
 * This function attempts to defocus a logical connection to indicate that the
 * specified connection no longer requires higher priority for limited network
 * resources which was obtained using qwf_cds_ConnectionFocus().
 *
 * @param [in]   pCtx  Pointer to Service context.
 * @param [in]   logicalConnectionName Name of the logical connection.
 *
 * @return ::CDS_OK             Successful result
 * @return ::CDS_ERR_INACTIVE   Service inactive or not ready
 * @return ::CDS_ERR_INT        Internal Error
 * @return ::CDS_ERR_PARAM      Invalid parameter
 * @return ::CDS_ERR_CONN_UNAVAIL  Connection not found (Interim state if Data
 *         Service not established)
 *
 */
qwf_cds_Results_e qwf_cds_ConnectionDefocus(const qwf_Context_t * const pCtx,
                                            qwf_cds_ConnectionName_e logicalConnectionName);

/**
 * @brief Get the cellular data service status.
 *
 * This function queries the last known status of the cellular data service
 * for the device. The states and their meanings are defined in
 * ::qwf_cds_DataServiceStatus_e. This function also returns the current
 * setting of Data Roaming Mode as defined in
 * ::qwf_cds_UserEnabledDataRoamingState_e.
 *
 * @param [in]  pCtx  Pointer to Service context.
 * @param [out] pDataServiceStatus Returned value for last known status of
 *              cellular data services.
 * @param [out]  pUserEnabledDataRoaming Returned value for current setting
 *              of data roaming mode.
 *
 * @return ::CDS_OK             Successful result
 * @return ::CDS_ERR_INACTIVE   Service inactive or not ready
 * @return ::CDS_ERR_PARAM      Invalid parameter
 *
 */
qwf_cds_Results_e qwf_cds_GetDataServiceStatus(const qwf_Context_t * const pCtx,
                                               qwf_cds_DataServiceStatus_e * const pDataServiceStatus,
                                               qwf_cds_UserEnabledDataRoamingState_e * const pUserEnabledDataRoaming);

/**
 * @brief Get the cellular data service mode.
 *
 * This function queries the last known cellular data service mode for the
 * device. The mode is defined by ::qwf_cds_DataServiceMode_e
 *
 * @param [in]  pCtx  Service context.
 * @param [out] pDataServiceMode   Return value for current setting
 *              of data service mode, ::CDS_DATA_SERVICE_MODE_ON or
 *              ::CDS_DATA_SERVICE_MODE_OFF.
 *
 * @return ::CDS_OK             Successful result
 * @return ::CDS_ERR_INACTIVE   Service inactive or not ready
 * @return ::CDS_ERR_PARAM      Invalid parameter
 *
 */
qwf_cds_Results_e qwf_cds_GetDataServiceMode(const qwf_Context_t * const pCtx,
                                             qwf_cds_DataServiceMode_e * const pDataServiceMode);


/**
 * @brief Set the cellular data service mode.
 *
 * This function sets the cellular data service mode for the device. The mode
 * is defined by ::qwf_cds_DataServiceMode_e. This value is persisted across
 * device restart.
 *
 * @param [in]  pCtx  Pointer to Service context.
 * @param [in]  dataServiceMode   New cellular data service mode for device.
 * @param [in]  rememberUserEnabledDataRoaming Indicates whether the current
 *              setting for User Enabled Data Roaming is maintained after the
 *              Data Service Mode setting changes to either ::CDS_DATA_SERVICE_MODE_ON
 *              or ::CDS_DATA_SERVICE_MODE_OFF.
 *
 * @return ::CDS_OK             Successful result
 * @return ::CDS_ERR_INACTIVE   Service inactive or not ready
 * @return ::CDS_ERR_INT        Internal Error
 * @return ::CDS_ERR_PARAM      Invalid parameter
 *
 */
qwf_cds_Results_e qwf_cds_SetDataServiceMode(const qwf_Context_t * const pCtx,
                                             qwf_cds_DataServiceMode_e dataServiceMode,
                                             bool rememberUserEnabledDataRoaming);

/**
 * @brief Get the cellular radio data roaming status.
 *
 * This function queries the last known cellular radio data roaming state.
 * The states and their meanings are defined by ::qwf_cds_DataRoamingStatus_e.
 *
 * @param [in]  pCtx  Pointer to Service context.
 * @param [out] pDataRoamingStatus   Return value for current or last know
 *              data roaming state of the cellular radio.
 *
 * @return ::CDS_OK             Successful result
 * @return ::CDS_ERR_INACTIVE   Service inactive or not ready
 * @return ::CDS_ERR_PARAM      Invalid parameter
 */
qwf_cds_Results_e qwf_cds_GetDataRoamingStatus(const qwf_Context_t * const pCtx,
                                               qwf_cds_DataRoamingStatus_e * const pDataRoamingStatus);

/**
 * @brief Get the cellular data roaming mode.
 *
 * This function queries the last known cellular data roaming mode for the
 * the device. The modes and their meanings are defined by
 * ::qwf_cds_DataRoamingMode_e.
 *
 * @param [in]  pCtx  Pointer to Service context.
 * @param [out] pDataRoamingMode   Return value for current cellular data
 *              roaming mode.
 *
 * @return ::CDS_OK             Successful result
 * @return ::CDS_ERR_INACTIVE   Service inactive or not ready
 * @return ::CDS_ERR_PARAM      Invalid parameter
 */
qwf_cds_Results_e qwf_cds_GetDataRoamingMode(const qwf_Context_t * const pCtx,
                                             qwf_cds_DataRoamingMode_e * const pDataRoamingMode);

/**
 * @brief Set the cellular data roaming mode.
 *
 * This function sets the cellular data roaming mode for the device.
 * The modes and their meanings are defined in ::qwf_cds_DataRoamingMode_e.
 * This value is persisted across device restart.
 *
 * @param [in]  pCtx   Pointer to Service context.
 * @param [in]  dataRoamingMode   New cellular data roaming mode for device.
 *
 * @return ::CDS_OK             Successful result
 * @return ::CDS_ERR_INACTIVE   Service inactive or not ready
 * @return ::CDS_ERR_INT        Internal Error
 * @return ::CDS_ERR_PARAM      Invalid parameter
 */
qwf_cds_Results_e qwf_cds_SetDataRoamingMode(const qwf_Context_t * const pCtx,
                                             qwf_cds_DataRoamingMode_e dataRoamingMode);

/**
 * @brief Enable Data Services While Data Roaming.
 *
 * When the data roaming mode is set to ::CDS_DATA_ROAMING_MODE_PROMPT and device enters a roaming
 * network, the user is prompted to either enable or disable data while
 * roaming. Data service status goes to ::CDS_OFF_ROAMING by default prior to the
 * user selection.
 *
 * This function turns data services back on. The choice specified though this
 * function is subsequently published in the
 * ::CDS_EVENT_DATA_USER_ENABLED_DATA_ROAMING_CHANGE event.
 * This choice is maintained until data services are turned off or until the
 * device ceases to be in a data roaming state. The modes and their meanings
 * are defined in ::qwf_cds_UserEnabledDataRoamingState_e.
 *
 * @param [in]   pCtx   Pointer to Service context.
 * @param [in]   dataRoamingEnabled   Indicates whether to enable or disable
 *               data while roaming (true=Enabled, false=Disabled).
 *
 * @return ::CDS_OK             Successful result
 * @return ::CDS_ERR_INACTIVE   Service inactive or not ready
 * @return ::CDS_ERR_INT        Internal Error
 * @return ::CDS_ERR_PARAM      Invalid parameter
 */
qwf_cds_Results_e qwf_cds_UserEnableDataRoaming(const qwf_Context_t * const pCtx,
                                                bool dataRoamingEnabled);

/**
 * @brief Get data related statistics for connection.
 *
 * This function returns the current data related statistics for the logical
 * connection specified within the service context. The following data can be
 * retrieved: Idle time for connection, Bytes sent via the connection, and
 * bytes received via the connection.
 *
 * @param [in]   pCtx    Pointer to Service context.
 * @param [in]   logicalConnectionName   Name of the logical connection.
 * @param [in]   dataUnit    Size Unit of reported transmitted data
 * @param [out]  pServiceStats   Reference pointer returned with the
 *               statistics for the specified logical connection. Data
 *               returned is defined by typedef ::qwf_cds_ServiceStatistics_t
 *
 * @return ::CDS_OK             Successful result
 * @return ::CDS_ERR_INACTIVE   Service inactive or not ready
 * @return ::CDS_ERR_INT        Internal Error
 * @return ::CDS_ERR_PARAM      Invalid parameter
 * @return ::CDS_ERR_CONN_UNAVAIL  Connection not found (Interim state if Data
 *         Service not established)
 *
 */
qwf_cds_Results_e qwf_cds_GetServiceStats(const qwf_Context_t * const pCtx,
                                          qwf_cds_ConnectionName_e logicalConnectionName,
                                          qwf_cds_DataUnit_e dataUnit,
                                          qwf_cds_ServiceStatistics_t * pServiceStats);

/**
 * @brief Set the IP address, network mask and gateway for the internet
 *        connection.
 *
 * This function sets a local or static IP address, network mask and gateway to
 * be used by the modem as the device IP address.
 *
 * @param [in]   pCtx       pointer to service context
 * @param [in]   logicalConnectionName   Name of the logical connection (???? Needed?)
 * @param [in]   pIpInfo   Pointer to a structure containing the Cellular Data
 *                         IP information.
 *
 * @return ::CDS_OK                Command was executed successfully
 * @return ::CDS_ERR_INACTIVE      Service inactive or not ready
 * @return ::CDS_ERR_PARAM         Invalid parameter was input
 * @return ::CDS_ERR_INT           Internal Error
 * @return ::CDS_ERR_NOT_SUPPORTED Function not supported by modem
 */
qwf_cds_Results_e qwf_cds_SetLocalIpAddress(const qwf_Context_t           * const pCtx,
                                            qwf_cds_ConnectionName_e logicalConnectionName,
                                            const qwf_cds_InterfaceData_t * const pIpInfo);
/**
 * @brief Query the IP address, network mask, gateway and name servers for the
 *        internet connection.
 *
 * This function returns the a static IP address, network mask, gateway and name
 * servers to be used by the modem as the local or static IP address associated
 * with the @c logicalConnectionName "Internet".
 *
 * @param [in]   pCtx       pointer to service context
 * @param [out]  pIpInfo    Reference pointer to a structure used to store the
 *                          queried Cellular Data IP information.
 *
 * @return ::CDS_OK             Command was executed successfully
 * @return ::CDS_ERR_INACTIVE   Service inactive or not ready
 * @return ::CDS_ERR_PARAM      Invalid parameter was input
 * @return ::CDS_ERR_INT        Internal Error
 * @return ::CDS_ERR_NOT_SUPPORTED Function not supported by modem
 */
qwf_cds_Results_e qwf_cds_GetLocalIpAddress(const qwf_Context_t     * const pCtx,
                                            qwf_cds_InterfaceData_t * const pIpInfo);

/**
 * @brief Set the APN name and authentication on the internet connection.
 *
 * This function sets the APN name and authentication for the PDP Context
 * associated with the @c logicalConnectionName "Internet". The authentication
 * method or type (PAP, CHAP or none) and, if needed, the username and password
 * are provided for authentication.
 *
 *
 * @param [in]   pCtx       pointer to service context
 * @param [in]   location   Location to save the APN information.
 *                   Note: currently only supports ::QWF_CDS_APN_MODEM_NVM
 * @param [in]   pApnInfo   Pointer to a structure containing the APN
 *                          information
 *
 * @return ::CDS_OK             Command was executed successfully
 * @return ::CDS_ERR_INACTIVE   Service inactive or not ready
 * @return ::CDS_ERR_PARAM      Invalid parameter was input
 * @return ::CDS_ERR_INT        Internal Error
 * @return ::CDS_ERR_NOT_SUPPORTED Function not supported by modem
 */
qwf_cds_Results_e qwf_cds_SetApn(const qwf_Context_t            * const pCtx,
                                 qwf_cds_ApnStorageType_e       location,
                                 const qwf_cds_ApnInformation_t * const pApnInfo);

/**
 * @brief Query the APN name and authentication on the internet connection.
 *
 * This function returns the APN name and IP type for the PDP Context associated
 * with the @c logicalConnectionName "Internet".
 *
 * @param [in]   pCtx           Pointer to service context
 * @param [in]   preConfigApn   Indicates to get the modem's pre-configured APN
 *                              or currently active APN (Not sure this is
 *                              possible).
 * @param [out]  pApnInfo       Reference pointer to a structure used to store
 *                              the queried APN information.
 *
 * @return ::CDS_OK             Command was executed successfully
 * @return ::CDS_ERR_INACTIVE   Service inactive or not ready
 * @return ::CDS_ERR_PARAM      Invalid parameter was input
 * @return ::CDS_ERR_INT        Internal Error
 * @return ::CDS_ERR_NOT_SUPPORTED Function not supported by modem
 */
qwf_cds_Results_e qwf_cds_GetApn(const qwf_Context_t      * const pCtx,
                                 bool  preConfigApn,
                                 qwf_cds_ApnInformation_t * const pApnInfo);

/**
 * @brief Reset the APN name and authentication on the internet connection to
 *        the factory default settings for the current network provider.
 *
 * This function sets the APN name and authentication for the PDP Context
 * associated with the @c logicalConnectionName "Internet" back to the original
 * factory configured APN settings.
 *
 *
 * @param [in]   pCtx       pointer to service context
 *
 * @return ::CDS_OK             Command was executed successfully
 * @return ::CDS_ERR_INACTIVE   Service inactive or not ready
 * @return ::CDS_ERR_PARAM      Invalid parameter was input
 * @return ::CDS_ERR_INT        Internal Error
 * @return ::CDS_ERR_NOT_SUPPORTED Function not supported by modem
 */
qwf_cds_Results_e qwf_cds_SetDefaultApn(const qwf_Context_t * const pCtx);

/**
 * @brief Get one or more static routes in the modem device's IPv4 routing table
 *
 * This function gets the modem routing table entries from the modem and returns them in the
 * @c qwf_cds_ModemRouteTable_t pointed to by @c pRouteTable.
 *
 * @param [in]    pCtx          Pointer to service context
 * @param [out]   pRouteTable   Pointer to a buffer containing the modem routing table entries
 *                              in a @c ::qwf_cds_ModemRouteTable_t
 *
 * @b NOTE: No analysis nor parsing is done on the retrieved routing table
 *          data, and it is returned as retrieved from the modem device.
 *
 * @return ::CDS_OK                Command was executed successfully
 * @return ::CDS_ERR_INACTIVE      Service inactive or not ready
 * @return ::CDS_ERR_PARAM         Invalid parameter was input
 * @return ::CDS_ERR_INT           Internal Error
 * @return ::CDS_ERR_NOT_SUPPORTED Function not supported by modem
 */
qwf_cds_Results_e qwf_cds_ModemRouteTableGet(const qwf_Context_t             * const pCtx,
                                                   qwf_cds_ModemRouteTable_t * const pRouteTable);

/**
 * @brief Add or delete a static route in the modem device's IPv4 routing table, or print the
 * modem device's internal routing table to the system log.
 *
 * @param [in]   pCtx         Pointer to service context
 * @param [in]   cmd          Modem device route command to be executed
 * @param [in]   pRouteData   Pointer to a buffer containing the ::qwf_cds_ModemRouteData_t data that
 *                            is to be added or deleted from the modem device internal routing table
 *
 * @b NOTE: For a ::CDS_MODEM_ROUTE_CMD_PRINT command, the modem device's internal routing table is
 *          printed, and @c pRouteData is ignored.
 *
 * @b NOTE: No analysis is done on the input data with respect to the validity of
 *          route command nor its effect on the modem device's routing behavior.  An
 *          inappropriate command may corrupt the modem device's internal routing table.
 *
 * @b NOTE: The arguments to a ::CDS_MODEM_ROUTE_CMD_DELETE command must match the equivalent arguments
 *          for the ::CDS_MODEM_ROUTE_CMD_ADD command that added the route to the modem device's
 *          routing table.
 *
 * @return ::CDS_OK                Command was executed successfully
 * @return ::CDS_ERR_INACTIVE      Service inactive or not ready
 * @return ::CDS_ERR_PARAM         Invalid parameter was input
 * @return ::CDS_ERR_INT           Internal Error
 * @return ::CDS_ERR_NOT_SUPPORTED Function not supported by modem
 */
qwf_cds_Results_e qwf_cds_ModemRouteCmd(const qwf_Context_t            * const pCtx,
                                              qwf_cds_ModemRouteCmd_e          cmd,
                                        const qwf_cds_ModemRouteData_t * const pRouteData);

/**
 * @brief Send a "raw" route command to the modem device
 *
 * This function tunnels a "raw" route command directly to the modem device. Presumably, syntax
 * follows the Linux route command syntax, although full support for the command should not be
 * expected, as each modem device's implementation will vary.
 *
 * If the command includes a trailing "dev name" option, the "name" argument is ignored.  Instead,
 * QWF will derive the appropriate modem interface name from the input @c logicalConnectionName and
 * pass the derived device name as the argument to the "dev" option.
 *
 * If the command has no "dev name" option, @c logicalConnectionName is ignored.
 *
 * Examples of @c pRawCmd:
 *    "route -n add -net aaa.aaa.aaa.aaa netmask mmm.mmm.mmm.mmm gw ggg.ggg.ggg.ggg"
 *    "route -n add default gw ggg.ggg.ggg.ggg"
 *    "route -n add default dev internet"
 *    "route -n add default dev"
 *
 * For example, the command @c pRawCmd:
 *    "route -n add -net aaa.aaa.aaa.aaa netmask mmm.mmm.mmm.mmm dev internet"
 * Will result in this equivalent route command:
 *    "route -n add -net aaa.aaa.aaa.aaa netmask mmm.mmm.mmm.mmm dev iface"
 * where "iface" is the modem device WAN interface name that is mapped to @c logicalConnectionName
 * specified by "name".
 *
 * @param [in]   pCtx                    Pointer to service context
 * @param [in]   logicalConnectionName   Name of the logical connection (service) name
 * @param [in]   pRawCmd                 Pointer to a buffer containing a NUL-terminated "raw" route command
 *                                       string that is to be tunneled directly to the modem device.  The
 *                                       string length is limited to ::QWF_CDS_MAX_MODEM_ROUTE_CMD_LEN.
 *
 * @b NOTE: No analysis is done on the input command with respect to the validity of
 *          route command nor its effect on the modem device's routing behavior.  An
 *          inappropriate command may corrupt the modem device's internal routing table.
 *
 * @b NOTE: The route command to delete a route must match the command arguments for
 *          the route command that added the route to the modem device's routing table.
 *
 * @return ::CDS_OK                Command was executed successfully
 * @return ::CDS_ERR_INACTIVE      Service inactive or not ready
 * @return ::CDS_ERR_PARAM         Invalid parameter was input
 * @return ::CDS_ERR_INT           Internal Error
 * @return ::CDS_ERR_NOT_SUPPORTED Function not supported by modem
 */
qwf_cds_Results_e qwf_cds_ModemRouteRawCmd(const qwf_Context_t            * const pCtx,
                                           const qwf_cds_ConnectionName_e         logicalConnectionName,
                                           const char                     * const pRawCmd);

/** @}*/ // end group QWF_CDS

__END_DECLS

#endif /* _QWF_CDS_H_INCLUDED */

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/wireless_framework/branches/7.1/trunk/common/lib/qwf_interface/public/qwf/qwf_cds.h $ $Rev: 3025 $")
#endif

