/*
 *
 * $QNXLicenseC:
 * Copyright 2017-2019, QNX Software Systems. All Rights Reserved.
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
 * @brief  This interface provides access to the Wi-Fi Manager service.
 *
 * @details This interface gives a client application the ability to control
 *          the Wi-Fi Network connections on the device and retrieve their
 *          associated configuration and state information.
 *
 *          Client applications can interact with Wi-Fi Manager via this
 *          interface in order to perform "Station" mode operations such as:
 *          - Initiate scan of available Wi-Fi Access Points and query the
 *            resulting data for the available Access Points
 *          - Obtain status of Wi-Fi connections and scan results
 *          - Maintain list of Saved Network profiles
 *          - Set authentication parameters needed to connect to Access Point
 *          - Establish connections to an available Access Point
 *
 *          Client applications can also use this interface to have the Wi-Fi
 *          Manager serve as an Access Point in "AP" mode. Operations supported
 *          for "AP" mode include:
 *          - Create an Access Point profile and setup authentication settings
 *            to allow the device to serve as an Access Point for remote
 *            clients.
 *          - Track the number of clients connected to the Access Point.
 *
 *          Usage:
 *          - Client application calls qwf_Init() to initialize the QWF Context
 *            data used as container for QWF and service data (like Wi-Fi
 *            Manager)
 *          - Client application calls qwf_wifi_RegisterCallback() to register a
 *            callback fuction used to process events from the QWF Wi-Fi Manager
 *            component. @b Note: Similar callbacks may be registered for each
 *            service the application is interested in.
 *          - Client application calls the qwf_ProcessEvents() function to
 *            block and wait for events to be sent to each services callback
 *            which was registered during initialization
 *          - The client application processes the events as required.
 *
 *          For code examples, refer to @ref QWF_EXAMPLES_WIFI
 */

#ifndef _QWF_WIFI_H_INCLUDED
#define _QWF_WIFI_H_INCLUDED

#include <qwf/qwf_interface.h>
#include <stdbool.h>

__BEGIN_DECLS

/**
 * @defgroup QWF_WIFI QWF Wi-Fi Manager Service
 * @{
 */


#define QWF_WIFI_MAX_SSID_LEN           (128)  ///< Defines the maximum number of ascii values in SSID array
#define QWF_WIFI_MIN_PASSPHRASE         (8)    ///< Defines the minimum length of the PassPhrase character string
#define QWF_WIFI_MAX_PASSPHRASE         (64)   ///< Defines the maximum length of the PassPhrase character string
#define QWF_WIFI_64_BIT_WEPKEY_LEN      (10)   ///< Defines the length of a 64 BIT WEP Key character string
#define QWF_WIFI_128_BIT_WEPKEY_LEN     (26)   ///< Defines the length of a 128 BIT WEP Key character string
#define QWF_WIFI_MAX_EAP_USERNAME       (64)   ///< Defines the maximum length of EAP Username character string
#define QWF_WIFI_MAX_EAP_PASSWORD       (64)   ///< Defines the maximum length of EAP Password character string
#define QWF_WIFI_MAX_EAP_CA_CERT        (128)  ///< Defines the maximum length of EAP Server Certificate file character string
#define QWF_WIFI_MAX_EAP_CA_CERT_DIR    (128)  ///< Defines the maximum length of EAP Server Certificate directory character string
#define QWF_WIFI_MAX_EAP_CLIENT_CERT    (128)  ///< Defines the maximum length of EAP Client Certificate file character string
#define QWF_WIFI_MAX_EAP_PRIVATE_KEY    (128)  ///< Defines the maximum length of EAP Private Key file character string
#define QWF_WIFI_MAX_EAP_PRIVATE_KEY_PW (64)   ///< Defines the maximum length of EAP Private Key Password character string
#define QWF_WIFI_MAX_SCAN_RESULTS       (100)  ///< Defines the maximum number of scan results in qwf_wifi_GetScanResults array
#define QWF_WIFI_MAX_BG_SCAN_DB_FILE    (128)  ///< Defines the maximum length of BG Scan DB file character string
#define QWF_WIFI_MAX_BSSID_LEN          (18)   ///< Defines the maximum length of a BSSID character string
#define QWF_WIFI_MAX_CAPABILTIES_LEN    (256)  ///< Defines the maximum length of HT or VHT Capabilities character string
#define QWF_WIFI_MAX_COUNTRY_CODE_LEN   (2)    ///< Defines the maximum length of Country Code character string
#define QWF_WIFI_MAX_WPA_SUPL_EVENT_LEN (128)  ///< Defines the maximum length of Wi-Fi Supplicant Event string
#define QWF_WIFI_AUTO_AP_CHANNEL        (-1)   ///< Defines the value used for Wi-Fi Manager assigned channel number
#define QWF_WIFI_MAX_HOSTNAME_LEN       (128)  ///< Defines the maximum length of Wi-Fi client hostname string
#define QWF_WIFI_MAX_MAC_ADDR_LEN       (17)   ///< Defines the maximum length of the MAC address string (without NULL terminator)
#define QWF_WIFI_MAX_IP_ADDR_LEN        (46)   ///< Defines the maximum length of IP Address. Allow for IPv4 or IPv6 address.
#define QWF_WIFI_MAX_AP_VSE_LEN         (514)  ///< Defines the maximum length of an VSE character string for scanned AP
#define QWF_WIFI_MAX_HOSTAPD_EVENT_LEN  (321)  ///< Defines the maximum length of Wi-Fi HostAPD Event string
#define QWF_WIFI_MAX_HOSTAPD_VSE_LEN    (494)  ///< Defines the maximum length of Wi-Fi HostAPD VSE string

typedef char qwf_wifi_IpAddrStr_t[QWF_WIFI_MAX_IP_ADDR_LEN + 1];   ///< Defines the typedef to use with IP address strings.

/**
 * @brief Return values used by QWF Wi-Fi Manager interface
 *
 * @details This enumeration defines the possible Wi-Fi Manager return values.
 */
typedef enum
{
    WIFI_OK                 = 0,          ///< Successful result
    WIFI_ERR_INT            = 1,          ///< Failure - Internal
    WIFI_ERR_PARAM          = 2,          ///< Failure - Invalid parameter
    WIFI_ERR_NOT_EXIST      = 3,          ///< Failure - Does not exist
    WIFI_ERR_EXISTS         = 4,          ///< Failure - Resource busy or already exists
    WIFI_ERR_INACTIVE       = 5           ///< Failure - Wi-Fi service is not active

} qwf_wifi_Results_e;


/**
 * @brief Values used by QWF Wi-Fi Manager interface to indicate the "type" of
 *        Security Protocol used by Access Point. Uses bitmap values as more
 *        than one may be supported by a Scanned Access Point.
 *
 */
typedef enum
{
    WIFI_SEC_PROT_NONE              = 0x00,   ///< No security authentication needed
    WIFI_SEC_PROT_WEP               = 0x01,   ///< Wired Equivalent Privacy (WEP)
    WIFI_SEC_PROT_WPA_PSK           = 0x02,   ///< Wireless Protected Access (WPA) Personal, Pre-shared Key
    WIFI_SEC_PROT_WPA_EAP           = 0x04,   ///< WPA Enterprise, Extensible Authentication Protocol
    WIFI_SEC_PROT_WPA2_PSK          = 0x08,   ///< WPA (v2) Personal, Pre-shared Key
    WIFI_SEC_PROT_WPA2_EAP          = 0x10    ///< WPA (v2) Enterprise, Extensible Authentication Protocol

} qwf_wifi_SecurityProtocol_e;

/**
 * @brief Values used by QWF Wi-Fi Manager interface to indicate the "method"
 *        of Extensible Authentication Protocol (EAP) used by Access Point.
 *
 */
typedef enum
{
    WIFI_EAP_METHOD_NONE      = 0,   ///< No EAP method
    WIFI_EAP_METHOD_TLS       = 1,   ///< EAP Transport Layer Security
    WIFI_EAP_METHOD_TTLS      = 2,   ///< EAP Tunneled Transport Layer Security
    WIFI_EAP_METHOD_PEAP      = 3    ///< Protected EAP

} qwf_wifi_EapMethod_e;

/**
 * @brief Event codes for QWF Wi-Fi Manager events
 *
 * @details This enumeration defines the possible QWF Wi-Fi Manager event
 *          types.
 */
typedef enum
{
    /**
     * This event indicates that scan results are available or updated containing
     * currently available Wi-Fi Access Points.
     */
    WIFI_EVENT_SCAN_RESULTS_AVAILABLE,

    /**
     * This event indicates that the current "Station" mode connection status for
     * Wi-Fi interface has changed. The event contains data related to the Wi-Fi
     * Manager when serving as a Wi-Fi client capable of connecting to an available
     * Wi-Fi Access Point.
     */
    WIFI_EVENT_CLIENT_CONNECTION_STATUS,

    /**
     * This event indicates that the current "Access Point" or AP mode connection
     * status for Wi-Fi interface has changed. The event contains data related to
     * the Wi-Fi Manager when serving as an Access Point.
     */
    WIFI_EVENT_AP_CONNECTION_STATUS,

    /**
     * This event indicates that the Wi-Fi Manager subsystem is no longer actively
     * monitoring status changes for the Wi-Fi interface.
     */
    WIFI_EVENT_WIFI_SERVICE_INACTIVE,

    /**
     * This event indicates that the Wi-Fi Manager subsystem is actively monitoring
     * status changes for the Wi-Fi interface.
     */
    WIFI_EVENT_WIFI_SERVICE_ACTIVE

} qwf_wifi_EventCode_e;

/**
 * @brief Wi-Fi interface setting for Wi-Fi Manager service
 *
 * @details This enumeration defines the status values for WLAN interface.
 */
typedef enum
{
    /**
     * This mode indicates that the WLAN interface is Down.
     */
    WIFI_INTERFACE_DOWN,
    /**
     * This mode indicates that the WLAN interface is Up.
     */
    WIFI_INTERFACE_UP

} qwf_wifi_InterfaceSetting_e;

/**
 * @brief Wi-Fi connection status for Wi-Fi Manager service
 *
 * @details This enumeration defines the possible connections states for the
 *          Wi-Fi Manager.
 */
typedef enum
{
    /**
     * This mode indicates the Wi-Fi Manager is not connected to Access Point.
     */
    WIFI_STATUS_DISCONNECTED,
    /**
     * This mode indicates the Wi-Fi Manager is connected to Access Point.
     */
    WIFI_STATUS_CONNECTED


} qwf_wifi_ConnectionStatus_e;

/**
 * @brief Wi-Fi frequency bands for Wi-Fi Manager service
 *
 * @details This enumeration defines the possible frequency bands used by the
 *          Wi-Fi interface for both client Network Profiles and Access Points.
 */
typedef enum
{
    /**
     * Band is unknown or not set.
     */
    WIFI_BAND_UNKNOWN,
    /**
     * This band is for 2.4 GHz only.
     */
    WIFI_BAND_2_4_GHZ,
    /**
     * This band is for 5 GHz only.
     */
    WIFI_BAND_5_GHZ,
    /**
     * This band is only for client Network Profiles and indicates the device will
     * connect to an available Access Points on either 2.4 GHz or 5 GHz as
     * specified by the Access Point.
     */
    WIFI_BAND_DUAL

} qwf_wifi_FrequencyBand_e;

/**
 * @brief Wi-Fi standards for Wi-Fi Manager service
 *
 * @details This enumeration defines the possible Wi-Fi standards that may be
 *          used by the Wi-Fi interface. Bit mask values can be combined when
 *          multiple standards are supported, for example 802.11 b/g/n.
 */
typedef enum
{
    WIFI_STANDARD_UNKNOWN      = 0x00,   ///< Standard is not known
    WIFI_STANDARD_A            = 0x01,   ///< Standard is 802.11a
    WIFI_STANDARD_B            = 0x02,   ///< Standard is 802.11b
    WIFI_STANDARD_G            = 0x04,   ///< Standard is 802.11g
    WIFI_STANDARD_N            = 0x08,   ///< Standard is 802.11n
    WIFI_STANDARD_AC           = 0x10    ///< Standard is 802.11ac

} qwf_wifi_Standard_e;

/**
 * @brief Wi-Fi role for Wi-Fi Manager service
 *
 * @details This enumeration defines the possible roles for the Wi-Fi Manager.
 *          "Station" and "AP" modes are supported.
 */
typedef enum
{
    /**
     * This role indicates the Wi-Fi Manager role is unknown.
     */
    WIFI_ROLE_NONE,
    /**
     * This role indicates the Wi-Fi Manager is acting as in Station mode (aka
     * Client mode) and allows the user application the ability to connect to
     * avaiable Access Points.
     */
    WIFI_ROLE_STATION,
    /**
     * This role indicates the Wi-Fi Manager is acting as an Access Point or server
     * and is configured to accept incoming Wi-Fi connection requests.
     */
    WIFI_ROLE_ACCESS_POINT

} qwf_wifi_WifiRole_e;

/**
 * @brief Wi-Fi Manager's state for Access Point mode
 *
 * @details This enumeration defines the possible states the Wi-Fi Manager can
 *          be in while in Access Point (AP) mode.
 *
 */
typedef enum
{
    /**
     * This state indicates the State of AP mode is unknown.
     */
    WIFI_AP_MODE_STATE_NONE,
    /**
     * This state indicates the Access Point is active.
     */
    WIFI_AP_MODE_STATE_ACTIVE,
    /**
     * This state indicates the Access Point is not active.
     */
    WIFI_AP_MODE_STATE_INACTIVE,
    /**
     * This state indicates the Access Point LAN interface is starting.
     * Intermediate state prior to becoming active.
     */
    WIFI_AP_MODE_STATE_LAN_STARTING,
    /**
     * This state indicates the Access Point WAN interface is starting.
     * Intermediate state prior to becoming active.
     */
    WIFI_AP_MODE_STATE_WAN_STARTING,
    /**
     * This state indicates the Access Point is in a suspended state.
     */
    WIFI_AP_MODE_STATE_SUSPEND,
    /**
     * This state indicates the Access Point is shutting down.
     * Intermediate state prior to becoming inactive.
     */
    WIFI_AP_MODE_STATE_SHUTDOWN,
    /**
     * This state indicates the Access Point LAN interface is stopping.
     * Intermediate state prior to becoming inactive.
     */
    WIFI_AP_MODE_STATE_SHUTDOWN_WAIT_LAN,
    /**
     * This state indicates the Access Point WAN interface is stopping.
     * Intermediate state prior to becoming inactive.
     */
    WIFI_AP_MODE_STATE_SHUTDOWN_WAIT_WAN

} qwf_wifi_AccessPointModeState_e;

/**
 * @brief Defines the possible values for VHT Operating Channel Width
 *        functionality within hostapd.
 *
 */
typedef enum
{
    /**
     * Indicates that the VHT Operating Channel Width is not set.
     */
    WIFI_VHT_OPER_CH_WIDTH_UNUSED,
    /**
     * Indicates that the VHT Operating Channel Width is 20 or 40 MHz.
     */
    WIFI_VHT_OPER_CH_WIDTH_20_40,
    /**
     * Indicates that the VHT Operating Channel Width is 80 MHz.
     */
    WIFI_VHT_OPER_CH_WIDTH_80,
    /**
     * Indicates that the VHT Operating Channel Width is 160 MHz.
     */
    WIFI_VHT_OPER_CH_WIDTH_160,
    /**
     * Indicates that the VHT Operating Channel Width is 80+80 MHz.
     */
    WIFI_VHT_OPER_CH_WIDTH_80_80

} qwf_wifi_VhtOperChanWidth_e;

/**
 * @brief Defines the possible values for background scanning functionality.
 *
 * @details This enumeration defines the possible modes that can be provided
 *          to the WPA supplicant layer to allow it to provide background
 *          scanning while a Saved Network profile is connected to an access
 *          point.
 *
 */
typedef enum
{
    /**
     * This mode indicates that background scanning is disabled.
     */
    WIFI_BG_SCAN_MODE_DISABLED  = 1,
    /**
     * This mode indicates that background scanning is "simple".
     */
    WIFI_BG_SCAN_MODE_SIMPLE    = 2,
    /**
    * This mode indicates that background scanning is "learned".
     */
    WIFI_BG_SCAN_MODE_LEARN     = 3

} qwf_wifi_BgScanMode_e;

/**
 * @brief Defines the structure to hold the background scanning information,
 *        if used, for a Saved Network profile.
 */
typedef struct qwf_wifi_BgScanInfo_s
{
    /**
     *  Background scan mode.
     */
    qwf_wifi_BgScanMode_e bgScanMode;
    /**
     *  Short interval value in seconds for the background scan mode or rule.
     */
    int shortInterval;
    /**
     *  Long interval value in seconds for the background scan mode or rule.
     */
    int longInterval;
    /**
     *  Signal strength threshold value for the background scan mode or rule. e.g.
     *  "-75".
     */
    int sigThreshold;

} qwf_wifi_BgScanInfo_t;

/**
 * @brief Defines the structure used to hold the Service Set Identifier (SSID)
 *        entry. The SSID represents the name of the Access Point.
 */
typedef struct qwf_wifi_Ssid_s
{
    /**
     *  Char string that identifies the SSID.
     */
    char name[QWF_WIFI_MAX_SSID_LEN+1];

} qwf_wifi_Ssid_t;

/**
 * @brief Defines the structure used to access Wi-Fi status data for Station mode.
 */
typedef struct qwf_wifi_StationModeStatusData_s
{
    /**
    *   Current Wi-Fi Manager role (Station or Access Point).
    */
    qwf_wifi_WifiRole_e role;
    /**
     *  WLAN interface mode - Up or Down
     *  @b Note: Only applies when interface is in Station Mode
     */
    qwf_wifi_InterfaceSetting_e    wlanSetting;
    /**
     *  Wi-Fi Connected to an Access Point - True or False.
     */
    bool                    connected;
    /**
    *   Currently connected Saved Network profile Id. Valid if Wi-Fi "connected" is
    *   True.
    */
    uint16_t                connectedSavedNetworkId;
    /**
     * Currently connected SSID name as string. Valid if Wi-Fi "connected" is TRUE.
     */
    qwf_wifi_Ssid_t         connectedSsid;
    /**
     * Number of Saved Network profiles
     */
    uint16_t                profileCount;
    /**
     * Last event generated by WPA Supplicant (applies to both AP and Station modes)
     */
    char                lastEvent[QWF_WIFI_MAX_WPA_SUPL_EVENT_LEN+1];

} qwf_wifi_StationModeStatusData_t;

/**
 * @brief Defines the structure used to access Wi-Fi Access Point (AP) Mode
 *        status data
 */
typedef struct qwf_wifi_ApModeStatusData_s
{
    /**
    *   Current Wi-Fi Manager role (Station or Access Point).
    */
    qwf_wifi_WifiRole_e role;
    /**
     * State of Wi-Fi Manager when role is Access Point.
     */
    qwf_wifi_AccessPointModeState_e    apState;
    /**
     * Number of clients connected to this Access Point.
     */
    uint16_t    apNumClients;
    /**
     * Maximum number of clients allowed on this Access Point.
     */
    uint16_t    apMaxClients;
    /**
     * Last event generated by WPA Supplicant (applies to both AP and Station modes)
     */
    char        lastEvent[QWF_WIFI_MAX_WPA_SUPL_EVENT_LEN+1];
    /**
     * Last event generated by hostapd (for AP mode)
     */
    char                apEvent[QWF_WIFI_MAX_HOSTAPD_EVENT_LEN+1];

} qwf_wifi_ApModeStatusData_t;

/**
 * @brief Defines the structure used when passing Wi-Fi Manager events from
 *        Wi-Fi Manager API to the client
 */
typedef struct qwf_wifi_Event_s
{
    /**
     *  The actual event to be processed.
     */
    qwf_wifi_EventCode_e eventCode;
    /**
     *  The client callback to process the event.
     */
    void *pClientCB;
    /**
     *  Union of all the possible Event data members.
     */
    union
    {
        /**
         *  Connection status for event ::WIFI_EVENT_CLIENT_CONNECTION_STATUS
         */
        qwf_wifi_StationModeStatusData_t clientData;
        /**
         *  Connection status for event ::WIFI_EVENT_AP_CONNECTION_STATUS
         */
        qwf_wifi_ApModeStatusData_t apData;
        /**
         *  Scan result data for event ::WIFI_EVENT_SCAN_RESULTS_AVAILABLE
         *  Only sends the number of results available. Use qwf_wifi_GetScanResults()
         *  to obtain the available access point data.
         */
        uint8_t numberResults;

    } wifiEvent;

} qwf_wifi_Event_t;

/**
 * @brief Defines the structure used to represent a scan result entry
 * @b Note: Values in this data structure remain valid until the next request
 * for scan results using interface qwf_wifi_GetScanResults().
 */
typedef struct qwf_wifi_ScanResult_s
{
    /**
     * ID or index of the scanned result
     */
    uint8_t     scanResultId;
    /**
     * SSID name as null terminated String.
     */
    qwf_wifi_Ssid_t    ssid;
    /**
     * Signal level value.
     */
    int    signalLevel;
    /**
     * Supported security protocols.
     * Bitmask of security protocols supported by Access Point.
     */
    qwf_wifi_SecurityProtocol_e    supportedProtocols;
    /**
     * Frequency band used by Saved Network.
     */
    qwf_wifi_FrequencyBand_e         frequencyBand;
    /**
     * BSSID address as null terminated String.
     */
    char    bssid[QWF_WIFI_MAX_BSSID_LEN+1];
    /**
     * Vendor Specified Element (VSE) information as null terminated String.
     */
    char    vseStr[QWF_WIFI_MAX_AP_VSE_LEN+1];

} qwf_wifi_ScanResult_t;

/**
 * @brief Defines the structure used to represent a Saved Network entry
 */
typedef struct qwf_wifi_SavedNetwork_s
{
    /**
     * Saved Network ID.
     */
    uint16_t     networkId;
    /**
     * SSID name as null terminated String.
     */
    qwf_wifi_Ssid_t    ssid;
    /**
     * Most recent signal level for "available" Saved Network.
     */
    int         signalLevel;
    /**
     * Frequency band used by Saved Network.
     */
    qwf_wifi_FrequencyBand_e         frequencyBand;
    /**
     * Network is "available" or found in latest scan results.
     */
    bool        available;
    /**
     * Network is "enabled" or ready to be connected or auto connected.
     */
    bool        enabled;
    /**
     * Network is the currently "connected" Wi-Fi network.
     */
    bool        connected;
    /**
     * Supported security protocols.
     * Bitmask of security protocols supported by Access Point.
     */
    qwf_wifi_SecurityProtocol_e    supportedProtocols;
    /**
     * Priority of this Saved Network if within ESS (where a lower value has
     * higher priority, and 0 indicates not used).
     */
    uint16_t    priority;
    /**
     * BSSID address to select as null terminated string. Use to select specific
     * network within ESS.
     */
    char    bssid[QWF_WIFI_MAX_BSSID_LEN+1];
    /**
     * Background scanning information, if used.
     */
    qwf_wifi_BgScanInfo_t   bgScanData;

} qwf_wifi_SavedNetwork_t;

/**
 * @brief Defines the structure used to represent the Access Point profile
 *        (when in AP mode)
 */
typedef struct qwf_wifi_ApProfileData_s
{
    /**
     * Structure containing SSID name as null terminated String.
     */
    qwf_wifi_Ssid_t    ssid;
    /**
     * Supported security protocol.
     */
    qwf_wifi_SecurityProtocol_e    authProtocol;
    /**
     * Passphrase as null terminated String.
     */
    char passphrase[QWF_WIFI_MAX_PASSPHRASE+1];
    /**
     * WEP key as null terminated String.
     */
    char wepKey[QWF_WIFI_128_BIT_WEPKEY_LEN+1];
    /**
     * Maximum number of clients allowed on Access Point.
     */
    uint16_t maxClients;
    /**
     * Wi-Fi standard used by the Access Point.
     */
    qwf_wifi_Standard_e standard;
    /**
     * Frequency band for the Access Point.
     */
    qwf_wifi_FrequencyBand_e band;
    /**
     * Privacy flag for the Access Point defining whether connected clients can
     * "see" each other.
     */
    bool privacy;
    /**
     * Short country code used by hostapd. String has a maximum length of 2
     * characters. Refer to the sample hostapd.conf configuration file for
     * additional information.
     */
    char countryCode[QWF_WIFI_MAX_COUNTRY_CODE_LEN+1];
    /**
     * Channel number to use for this access point.
     * A value of QWF_WIFI_AUTO_AP_CHANNEL indicates the Wi-Fi Manager determines
     * the channel number. Refer to the sample hostapd.conf configuration file for
     * additional information.
     */
    int channelNum;
    /**
     * VHT operating channel width setting used by hostapd for the Access Point
     * being started or updated. Refer to the sample hostapd.conf configuration file for
     * additional information.
     */
    qwf_wifi_VhtOperChanWidth_e vhtOperChWidth;
    /**
     * VHT center frequency index (1st segment) used by hostapd for the Access Point
     * being started or updated. Refer to the sample hostapd.conf configuration file for
     * additional information.
     */
    uint8_t vhtOperCenterFreqSeg0;
    /**
     * VHT center frequency index (2nd segment) used by hostapd for the Access Point
     * being started or updated. Refer to the sample hostapd.conf configuration file for
     * additional information.
     */
    uint8_t vhtOperCenterFreqSeg1;
    /**
     * HT capability flags used by hostapd. String is formatted as specified in the
     * hostapd.conf file (e.g. "[HT40-][SHORT-GI-20][SHORT-GI-40]". Refer to the
     * sample hostapd.conf configuration file for additional information and flag
     * descriptions.
     */
    char htCapabilities[QWF_WIFI_MAX_CAPABILTIES_LEN+1];
    /**
     * VHT capability flags used by hostapd. String is formatted as specified in the
     * hostapd.conf file (e.g. "[SHORT-GI-80][HTC-VHT]". Refer to the sample
     * hostapd.conf configuration file for additional information and flag
     * descriptions.
     */
    char vhtCapabilities[QWF_WIFI_MAX_CAPABILTIES_LEN+1];
    /**
     * Character array containing a VSE (Vendor Specific Element) to be
     * used by the hostapd as a filter against the VSE of an incoming Association
     * Request (or Re-Association Request). The hostapd only allows connection to
     * clients whose VSE contains this "associationVSE".
     */
    char associationVSE[QWF_WIFI_MAX_HOSTAPD_VSE_LEN+1];
    /**
     * Character array containing one or more VSEs (Vendor Specific
     * Element) to be used by the hostapd when broadcasting a Beacon or Probe.
     * Refer to the "vendor_elements" parameter in the sample hostapd.conf file for
     * additional information on usage.
     */
    char vendorElements[QWF_WIFI_MAX_HOSTAPD_VSE_LEN+1];

} qwf_wifi_ApProfileData_t;

/**
 * @brief Defines the structure used to represent a client currently connected
 *        to the enabled Access Point.
 */
typedef struct qwf_wifi_ApClientData_s
{
    /**
     * Hostname of connected client as null terminated String.
     */
    char hostname[QWF_WIFI_MAX_HOSTNAME_LEN+1];
    /**
     * IP address of connected client as null terminated String.
     */
    qwf_wifi_IpAddrStr_t ipAddr;
    /**
     * The MAC address for the network interface as string
     */
    char mac[QWF_WIFI_MAX_MAC_ADDR_LEN + 1];

} qwf_wifi_ApClientData_t;

/**
 * @brief Start a scan of available Wi-Fi Access Points
 *
 * Interface to request the Wi-Fi Manager to perform a scan of available Wi-Fi
 * access points. The interface returns immediately with a result indicating
 * whether the scan has started.
 * The ::WIFI_EVENT_SCAN_RESULTS_AVAILABLE event is generated once the scan is
 * complete, typically after several seconds. Once event is received the
 * results can be queried using the qwf_wifi_GetScanResults() interface.
 *
 * @param [in]  pCtx  Pointer to QWF Service context.
 *
 * @return ::WIFI_OK               Successful result
 * @return ::WIFI_ERR_INACTIVE     Service inactive or not ready
 * @return ::WIFI_ERR_INT          Internal Error
 * @return ::WIFI_ERR_PARAM        Invalid parameter
 *
 */
qwf_wifi_Results_e qwf_wifi_StartScan(qwf_Context_t *pCtx);

/**
 * @brief Query the available Wi-Fi Access Points from recent scan
 *
 * Interface to request the Wi-Fi Manager to return an array of available Wi-Fi
 * access points. The interface returns an array of ::qwf_wifi_ScanResult_t
 * entries. The array must be allocated by the calling application and have
 * enough memory to store the number of entries requested by variable
 * pNumEntries. Upon return, the pNumEntries variable will be updated to
 * contain the actual number of entries in the array (in case it is less than
 * requested).
 *
 * @param [in]  pCtx  Pointer to QWF Service context.
 * @param [in,out]  pNumEntries  Reference pointer to the number of Wi-Fi
 *        access points requested. Upon return, this variable is updated to
 *        show the actual number of access points in the array.
 * @param [out]  pResultArray  Reference pointer to an array of data to hold
 *        the list of scanned access points.
 *
 * @return ::WIFI_OK               Successful result
 * @return ::WIFI_ERR_INACTIVE     Service inactive or not ready
 * @return ::WIFI_ERR_INT          Internal Error
 * @return ::WIFI_ERR_PARAM        Invalid parameter
 *
 */
qwf_wifi_Results_e qwf_wifi_GetScanResults(qwf_Context_t *pCtx,
                                           uint8_t * const pNumEntries,
                                           qwf_wifi_ScanResult_t * const pResultArray);

/**
 * @brief Enable the WLAN interface to allow access to Wi-Fi Access Points
 *
 * Interface to request the Wi-Fi Manager to allow connections to available
 * Wi-Fi access points. When WLAN Interface is enabled, the Wi-Fi Manager will
 * attempt connections to enabled saved networks.
 * The status can be queried as part of the qwf_wifi_GetClientStatus()
 * interface.
 *
 * @param [in]  pCtx  Pointer to QWF Service context.
 *
 * @return ::WIFI_OK               Successful result
 * @return ::WIFI_ERR_INACTIVE     Service inactive or not ready
 * @return ::WIFI_ERR_INT          Internal Error
 * @return ::WIFI_ERR_PARAM        Invalid parameter
 *
 */
qwf_wifi_Results_e qwf_wifi_EnableWlanInterface(qwf_Context_t *pCtx);

/**
 * @brief Disable the WLAN interface access to Wi-Fi Access Points
 *
 * Interface to request the Wi-Fi Manager to stop connections to available Wi-Fi
 * access points. When WLAN Interface is disabled, the Wi-Fi Manager will not
 * attempt connections to enabled saved networks. Disabling will cause the
 * current connection to an access point to be disconnected.
 * The status can be queried as part of the qwf_wifi_GetClientStatus()
 * interface.
 *
 * @param [in]  pCtx  Pointer to QWF Service context.
 *
 * @return ::WIFI_OK               Successful result
 * @return ::WIFI_ERR_INACTIVE     Service inactive or not ready
 * @return ::WIFI_ERR_INT          Internal Error
 * @return ::WIFI_ERR_PARAM        Invalid parameter
 *
 */
qwf_wifi_Results_e qwf_wifi_DisableWlanInterface(qwf_Context_t *pCtx);

/**
 * @brief Query Wi-Fi status information for Client Mode
 *
 * Interface to request the Wi-Fi Manager to return  for "station" or "client"
 * mode information related to the Wi-Fi Manager. The interface returns a data
 * structure of type ::qwf_wifi_StationModeStatusData_t.
 * The data must be allocated by the calling application and is accessed via
 * the provided reference pointer.
 *
 * @param [in]  pCtx  Pointer to QWF Service context.
 * @param [out]  pWifiStatus  Reference pointer to a data structure of type
 *        ::qwf_wifi_StationModeStatusData_t to hold the Wi-Fi Manager
 *        Status information for Station mode.
 *
 * @return ::WIFI_OK               Successful result
 * @return ::WIFI_ERR_INACTIVE     Service inactive or not ready
 * @return ::WIFI_ERR_INT          Internal Error
 * @return ::WIFI_ERR_PARAM        Invalid parameter
 *
 */
qwf_wifi_Results_e qwf_wifi_GetClientStatus(qwf_Context_t *pCtx,
                                            qwf_wifi_StationModeStatusData_t * const pWifiStatus);

/**
 * @brief Query Wi-Fi status information for Access Point (AP) mode
 *
 * Interface to request the Wi-Fi Manager to return Access Point mode
 * information related to the Wi-Fi Manager. The interface returns a data
 * structure of type ::qwf_wifi_ApModeStatusData_t.
 * The data must be allocated by the calling application and is accessed via
 * the provided reference pointer.
 *
 * @param [in]  pCtx  Pointer to QWF Service context.
 * @param [out]  pWifiStatus  Reference pointer to a data structure of type
 *        ::qwf_wifi_ApModeStatusData_t to hold the Wi-Fi Manager
 *        Status information for AP mode.
 *
 * @return ::WIFI_OK               Successful result
 * @return ::WIFI_ERR_INACTIVE     Service inactive or not ready
 * @return ::WIFI_ERR_INT          Internal Error
 * @return ::WIFI_ERR_PARAM        Invalid parameter
 *
 */
qwf_wifi_Results_e qwf_wifi_GetApStatus(qwf_Context_t *pCtx,
                                        qwf_wifi_ApModeStatusData_t * const pWifiStatus);

/**
 * @brief Query the SSID for the currently connected Wi-Fi Access Point
 *
 * Interface to request the Wi-Fi Manager to return the SSID for currently
 * connected Wi-Fi Access Point. Interface returns a data structure of type
 * ::qwf_wifi_Ssid_t containing the SSID name.
 * The data must be allocated by the calling application and is accessed via
 * the provided reference pointer. If not connected to any access point, the
 * SSID name will be an empty string.
 * @b Note: This data is also available from the qwf_wifi_GetClientStatus()
 * interface.
 *
 * @param [in]  pCtx  Pointer to QWF Service context.
 * @param [out]  pSsid  Reference pointer to the SSID name in data structure
 *        of type ::qwf_wifi_Ssid_t.
 *
 * @return ::WIFI_OK               Successful result
 * @return ::WIFI_ERR_INACTIVE     Service inactive or not ready
 * @return ::WIFI_ERR_INT          Internal Error
 * @return ::WIFI_ERR_PARAM        Invalid parameter
 *
 */
qwf_wifi_Results_e qwf_wifi_GetConnectedSSID(qwf_Context_t *pCtx,
                                             qwf_wifi_Ssid_t * const pSsid);

/**
 * @brief Set a passphrase prior to creating or updating a Saved Network
 *        profile for PSK based access points
 *
 * Interface to temporarily save a passphrase to be used for WPA-PSK or
 * WPA2-PSK authentication. This interface must be called prior to using the
 * qwf_wifi_CreateSavedNetwork() or qwf_wifi_UpdateSavedNetwork() interfaces
 * to create or update a Saved Network profiles that use WPA-PSK or WPA2-PSK
 * authentication.
 * This temporary passphrase will be stored in the persisted Saved Network
 * profile specified by the qwf_wifi_CreateSavedNetwork() or
 * qwf_wifi_UpdateSavedNetwork() interface. Note that after saving, the
 * temporary passphrase is reset to an empty string.
 * Once part of the persisted Saved Network profile, the passphrase will be
 * used whenever making a connection to that Access Point.
 * The Passphrase must be at least 8 and no more than 64 characters.
 *
 * @param [in]  pCtx  Pointer to QWF Service context.
 * @param [in]  pPassphrase  Pointer to a null terminated character string
 *        representing a Passphrase for PSK authentication
 *
 * @return ::WIFI_OK               Successful result
 * @return ::WIFI_ERR_INACTIVE     Service inactive or not ready
 * @return ::WIFI_ERR_INT          Internal Error
 * @return ::WIFI_ERR_PARAM        Invalid parameter
 *
 */
qwf_wifi_Results_e qwf_wifi_SetPassphrase(qwf_Context_t *pCtx,
                                          char * pPassphrase);

/**
 * @brief Set enterprise credentials prior to creating or updating a Saved
 *        Network profile for EAP based access points
 *
 * Interface to temporarily save enterprise credentials to be used for WPA-EAP
 * or WPA2-EAP authentication. This interface must be called prior to using
 * the qwf_wifi_CreateSavedNetwork() or qwf_wifi_UpdateSavedNetwork()
 * interfaces to create or update a Saved Network profiles that use WPA-EAP or
 * WPA2-EAP authentication.
 * These credentials will be stored in the persisted Saved Network profile
 * specified by the qwf_wifi_CreateSavedNetwork() or
 * qwf_wifi_UpdateSavedNetwork() interface. Note that after saving, the
 * temporary enterprise credentials are reset to empty strings.
 * Once part of the persisted Saved Network profile, the enterprise
 * credentials will be used whenever making a connection to that Access Point.
 * The Context and EAP Method are mandatory input parameters, however the
 * credential variables may be set to NULL if not used by specified EAP
 * method.
 *
 * @param [in]  pCtx  Pointer to QWF Service context.
 * @param [in]  eapMethod  EAP Method as defined by ::qwf_wifi_EapMethod_e
 * @param [in]  pUsername  Pointer to a null terminated character string
 *        representing the Username for EAP authentication when method is TLS,
 *        TTLS or PEAP
 * @param [in]  pPassword  Pointer to a null terminated character string
 *        representing the password for EAP authentication when method is TTLS
 *        or PEAP
 * @param [in]  pCaCertificate  Pointer to a null terminated character string
 *        representing the Server Certificate file for EAP authentication when
 *        method is TLS, TTLS or PEAP
 * @param [in]  pCaCertificateDir  Pointer to a null terminated character
 *        string representing the Server Certificate Directory Path for EAP
 *        authentication when method is TLS, TTLS or PEAP
 * @param [in]  pClientCertificate  Pointer to a null terminated character
 *        string representing the Client Certificate file for EAP
 *        authentication when method is TLS or PEAP
 * @param [in]  pPrivateKey  Pointer to a null terminated character string
 *        representing the Private Key file for EAP authentication when method
 *        is TLS
 * @param [in]  pPrivateKeyPassword  Pointer to a null terminated character
 *        string representing the Private Key Password for EAP authentication
 *        when method is TLS
 *
 * @return ::WIFI_OK               Successful result
 * @return ::WIFI_ERR_INACTIVE     Service inactive or not ready
 * @return ::WIFI_ERR_INT          Internal Error
 * @return ::WIFI_ERR_PARAM        Invalid parameter
 *
 */
qwf_wifi_Results_e qwf_wifi_SetEnterpriseCredentials(qwf_Context_t *pCtx,
                                                     qwf_wifi_EapMethod_e eapMethod,
                                                     char * pUsername,
                                                     char * pPassword,
                                                     char * pCaCertificate,
                                                     char * pCaCertificateDir,
                                                     char * pClientCertificate,
                                                     char * pPrivateKey,
                                                     char * pPrivateKeyPassword);


/**
 * @brief Set WEP Key prior to creating or updating a Saved Network profile
 *        for WEP based access points
 *
 * Interface to temporarily save a key to be used for WEP authentication. This
 * interface must be called prior to using the qwf_wifi_CreateSavedNetwork()
 * or qwf_wifi_UpdateSavedNetwork() interfaces to create or update a Saved
 * Network profiles that use WEP authentication.
 * This temporary key will be stored in the persisted Saved Network profile
 * specified by the qwf_wifi_CreateSavedNetwork() or
 * qwf_wifi_UpdateSavedNetwork() interface. Note that after saving, the
 * temporary key is reset to an empty string.
 * Once part of the persisted Saved Network profile, the key will be used
 * whenever making a connection to that Access Point. The key must be
 * at either 10 or 26 characters.
 *
 * @param [in]  pCtx  Pointer to QWF Service context.
 * @param [in]  pKey  Pointer to a null terminated character string
 *        representing a Key for WEP authentication
 *
 * @return ::WIFI_OK               Successful result
 * @return ::WIFI_ERR_INACTIVE     Service inactive or not ready
 * @return ::WIFI_ERR_INT          Internal Error
 * @return ::WIFI_ERR_PARAM        Invalid parameter
 *
 */
qwf_wifi_Results_e qwf_wifi_SetWepKey(qwf_Context_t *pCtx,
                                      char * pKey);

/**
 * @brief Set the background scan parameters prior to creating or updating a
 *        Saved Network profile
 *
 * Interface to temporarily save background (BG) scan settings prior to
 * creating or updating a Saved Network profile. The settings are used by the
 * WPA Supplicant as "rules" specifying background scan behaviour for the
 * purpose of roaming within an ESS (i.e., within a single network block with
 * all the APs using the same SSID).
 * This interface must be called prior to using the
 * qwf_wifi_CreateSavedNetwork() or qwf_wifi_UpdateSavedNetwork() interfaces
 * to apply background scan parameters to a Saved Network profile, if not
 * called the background scan parameters will be set to a default setting of
 * "disabled".
 *
 * @param [in]  pCtx  Pointer to QWF Service context.
 * @param [in]  pBgScanInfo  Pointer to qwf_wifi_BgScanInfo_t structure
 *                    containing background scanning information.
 * @param [in]  pDbFileName  Filename of background scan database, as a
 *                    NULL-terminated string. This is required only for the
 *                    WIFI_BG_SCAN_MODE_LEARN mode.
 *
 * @return ::WIFI_OK               Successful result
 * @return ::WIFI_ERR_INACTIVE     Service inactive or not ready
 * @return ::WIFI_ERR_PARAM        Invalid parameter
 *
 */
qwf_wifi_Results_e qwf_wifi_SetBgScanMode(qwf_Context_t *pCtx,
                                          const qwf_wifi_BgScanInfo_t * const pBgScanInfo,
                                          const char * const pDbFileName);

/**
 * @brief Query of currently Saved Network profiles
 *
 * Interface to request the Wi-Fi Manager to return an array of currently Saved
 * Network profiles. The interface returns an array of
 * ::qwf_wifi_SavedNetwork_t entries. The array must be allocated by the
 * calling application and have enough memory to store the number of entries
 * requested by variable pNumEntries.
 * Upon return, the pNumEntries variable will be updated to contain the actual
 * number of entries in the array (in case it is less than requested).
 * Note that the NetworkId values returned by this interface are to be used
 * for all other interfaces involving saved networks (except for Create) and
 * that this ID will remain valid even if other Saved Networks are deleted or
 * created. However, after a power cycle, the networks should be queried again
 * as the persisted data is "collapsed" to remove any holes in the data
 * structure caused by deleting Saved Network profiles.
 *
 * @param [in]  pCtx  Pointer to QWF Service context.
 * @param [in,out]  pNumEntries  Reference pointer to the number of Saved
 *        Network profiles requested. Upon return, this variable is updated to
 *        show the actual number of Saved Networks in the array.
 * @param [out]  pNetworks  Reference pointer to an array of
 *        ::qwf_wifi_SavedNetwork_t data to hold the list of Saved Network
 *        profiles.
 *
 * @return ::WIFI_OK               Successful result
 * @return ::WIFI_ERR_INACTIVE     Service inactive or not ready
 * @return ::WIFI_ERR_INT          Internal Error
 * @return ::WIFI_ERR_PARAM        Invalid parameter
 *
 */
qwf_wifi_Results_e qwf_wifi_GetSavedNetworks(qwf_Context_t *pCtx,
                                             uint16_t * const pNumEntries,
                                             qwf_wifi_SavedNetwork_t * const pNetworks);

/**
 * @brief Create a Wi-Fi Network profile.
 *
 * Interface to create and save a Wi-Fi network profile. The "Saved Network"
 * profiles are used to store required authentication settings and can be
 * setup to "auto connect" to an access point when access point available. The
 * Saved Networks are persisted over power cycle.
 * The Saved Network is not "enabled" by default, so it will not connect to
 * specified access point if available, but connection can be enabled using
 * qwf_wifi_EnableSavedNetwork() with the value returned in the reference
 * variable pCreatedNetworkId.
 * Prior to using qwf_wifi_CreateSavedNetwork() interface, additional
 * interfaces must be called to setup authentication parameters, if required
 * by the Saved Network's Access Point, as follows:
 *   - qwf_wifi_SetPassphrase() must be called for WPA-PSK and WPA2-PSK
 *     authentication protocols
 *   - qwf_wifi_SetWepKey() must be called for WEP authentication protocol
 *   - qwf_wifi_SetEnterpriseCredentials() must be called for WPA-EAP and
 *     WPA2-EAP authentication protocols
 * Also prior to using qwf_wifi_CreateSavedNetwork() interface, the
 * qwf_wifi_SetBgScanMode() interface may be called to setup parameters for
 * background scanning if the optional background scanning feature is to be
 * used when the profile is connected to an access point.
 *
 * @param [in]  pCtx  Pointer to QWF Service context.
 * @param [in]  pSsid  Pointer to SSID name in data structure of type
 *        ::qwf_wifi_Ssid_t.
 * @param [in]  protocol  Authentication protocol as defined by type
 *        ::qwf_wifi_SecurityProtocol_e.
 * @param [in]  band  Frequency band used by the Access Point as defined by
 *        type ::qwf_wifi_FrequencyBand_e.
 * @param [in]  priority  Network selection priority given to the Saved
 *        Network profile within an ESS (same SSID). Value of 1 is highest
 *        priority. Value of 0 indicates that network selection will be based
 *        on strongest signal of available access points, and not base on
 *        priority.
 * @param [in]  pBssid  Pointer to string containing the specific BSSID (HW
 *        MAC address) to connect to when an access point has more than one
 *        within the ESS (same SSID). A value of NULL indicates the profile
 *        can connect to the access point within the SSID with the strongest
 *        signal.
 * @param [out]  pCreatedNetworkId  Reference pointer to the variable to store
 *        the ID for the newly created Saved Network.
 *
 * @return ::WIFI_OK               Successful result
 * @return ::WIFI_ERR_INACTIVE     Service inactive or not ready
 * @return ::WIFI_ERR_INT          Internal Error
 * @return ::WIFI_ERR_PARAM        Invalid parameter
 *
 */
qwf_wifi_Results_e qwf_wifi_CreateSavedNetwork(qwf_Context_t *pCtx,
                                               const qwf_wifi_Ssid_t * const pSsid,
                                               qwf_wifi_SecurityProtocol_e protocol,
                                               qwf_wifi_FrequencyBand_e band,
                                               uint16_t priority,
                                               const char * const pBssid,
                                               uint16_t * const pCreatedNetworkId);

/**
 * @brief Update a Saved Network profile.
 *
 * Interface to update an existing Saved Network profile. The Saved Networks
 * are persisted over power cycle. The Saved Network is accessed via it's
 * NetworkId which can be obtained from the query interface
 * qwf_wifi_GetSavedNetworks(). This interface does not affect the "Enabled"
 * setting of the Saved Network.
 * Prior to using qwf_wifi_UpdateSavedNetwork() interface, additional
 * interfaces must be called to setup authentication parameters, if required
 * by the Saved Network's Access Point, as follows:
 *   - qwf_wifi_SetPassphrase() must be called for WPA-PSK and WPA2-PSK
 *     authentication protocols
 *   - qwf_wifi_SetWepKey() must be called for WEP authentication protocol
 *   - qwf_wifi_SetEnterpriseCredentials() must be called for WPA-EAP and
 *     WPA2-EAP authentication protocols
 * Also prior to using qwf_wifi_UpdateSavedNetwork() interface, the
 * qwf_wifi_SetBgScanMode() interface may be called to setup parameters for
 * background scanning if the optional background scanning feature is to be
 * used when the profile is connected to an access point.
 *
 * All input variables must be set even when same as existing, except for the
 * pNewSsid variable which can be set to NULL if not changing SSID name.
 *
 * @param [in]  pCtx  Pointer to QWF Service context.
 * @param [in]  networkId  Saved Network ID to update.
 * @param [in]  newProtocol  Authentication protocol as defined by type
 *        ::qwf_wifi_SecurityProtocol_e.
 * @param [in]  newBand  Frequency band used by the Access Point as defined
 *        by type ::qwf_wifi_FrequencyBand_e.
 * @param [in]  newPriority  Network selection priority given to the Saved
 *        Network profile within ESS (same SSID). Value of 1 is highest
 *        priority. Value of 0 indicates that network selection will be based
 *        on strongest signal of available access points within ESS, and not
 *        base on priority.
 * @param [in]  pNewBssid  Pointer to string containing the specific BSSID (HW
 *        MAC address) to connect to when an access point has more than one
 *        within an ESS. A value of NULL indicates the profile can connect to
 *        the access point within the SSID with the strongest signal.
 * @param [in]  pNewSsid Pointer to SSID name in data structure of type
 *        ::qwf_wifi_Ssid_t. Can be NULL if keeping same SSID name.
 *
 * @return ::WIFI_OK               Successful result
 * @return ::WIFI_ERR_INACTIVE     Service inactive or not ready
 * @return ::WIFI_ERR_INT          Internal Error
 * @return ::WIFI_ERR_PARAM        Invalid parameter
 *
 */
qwf_wifi_Results_e qwf_wifi_UpdateSavedNetwork(qwf_Context_t *pCtx,
                                               uint16_t networkId,
                                               qwf_wifi_SecurityProtocol_e newProtocol,
                                               qwf_wifi_FrequencyBand_e newBand,
                                               uint16_t newPriority,
                                               const char * const pNewBssid,
                                               const qwf_wifi_Ssid_t * const pNewSsid);

/**
 * @brief Allow a Saved Network profile to connect to access point
 *
 * Interface to allow a saved Wi-Fi network profile to attempt a connection to
 * it's Access Point when no Wi-Fi connection currently established and Saved
 * Network's access point is available. The Saved Network is accessed via it's
 * NetworkId which can be obtained from the query interface
 * qwf_wifi_GetSavedNetworks().
 *
 * @param [in]  pCtx  Pointer to QWF Service context.
 * @param [in]  networkId  ID of Saved Network to enable.
 *
 * @return ::WIFI_OK               Successful result
 * @return ::WIFI_ERR_INACTIVE     Service inactive or not ready
 * @return ::WIFI_ERR_INT          Internal Error
 * @return ::WIFI_ERR_PARAM        Invalid parameter
 *
 */
qwf_wifi_Results_e qwf_wifi_EnableSavedNetwork(qwf_Context_t *pCtx,
                                               uint16_t networkId);

/**
 * @brief Disable "auto connect" for a Saved Network profile.
 *
 * Interface to disable auto connecting to the Saved Network's Access Point
 * even when the Saved Network's access point is available.
 * Saved Network must be "Enabled" to allow it to attempt connecting to it's
 * Access Point. The Saved Network is accessed via it's NetworkId which can be
 * obtained from the query interface qwf_wifi_GetSavedNetworks().
 *
 * @param [in]  pCtx  Pointer to QWF Service context.
 * @param [in]  networkId  ID of Saved Network to disable.
 *
 * @return ::WIFI_OK               Successful result
 * @return ::WIFI_ERR_INACTIVE     Service inactive or not ready
 * @return ::WIFI_ERR_INT          Internal Error
 * @return ::WIFI_ERR_PARAM        Invalid parameter
 *
 */
qwf_wifi_Results_e qwf_wifi_DisableSavedNetwork(qwf_Context_t *pCtx,
                                                uint16_t networkId);

/**
 * @brief Force a Saved Network profile to connect to access point
 *
 * Interface to force a saved Wi-Fi network profile to attempt a connection to
 * it's Access Point if available. Any other Wi-Fi connection currently
 * established will be disconnected.
 * The Saved Network is accessed via it's NetworkId which can be obtained from
 * the query interface qwf_wifi_GetSavedNetworks().
 *
 * @param [in]  pCtx  Pointer to QWF Service context.
 * @param [in]  networkId  ID of Saved Network to enable.
 *
 * @return ::WIFI_OK               Successful result
 * @return ::WIFI_ERR_INACTIVE     Service inactive or not ready
 * @return ::WIFI_ERR_INT          Internal Error
 * @return ::WIFI_ERR_PARAM        Invalid parameter
 *
 */
qwf_wifi_Results_e qwf_wifi_ConnectSavedNetwork(qwf_Context_t *pCtx,
                                                uint16_t networkId);

/**
 * @brief Deletes a Saved Network profile.
 *
 * Interface to delete a Saved Network profile from persistant memory.
 * The Saved Network is accessed via it's NetworkId which can be obtained from
 * the query interface qwf_wifi_GetSavedNetworks().
 *
 * @param [in]  pCtx  Pointer to QWF Service context.
 * @param [in]  networkId  ID of Saved Network to delete.
 *
 * @return ::WIFI_OK               Successful result
 * @return ::WIFI_ERR_INACTIVE     Service inactive or not ready
 * @return ::WIFI_ERR_INT          Internal Error
 * @return ::WIFI_ERR_PARAM        Invalid parameter
 *
 */
qwf_wifi_Results_e qwf_wifi_DeleteSavedNetwork(qwf_Context_t *pCtx,
                                               uint16_t networkId);

/**
 * @brief Query the ID of the Saved Network profile that is currently
 *        connected to an Access Point.
 *
 * Interface to request the Wi-Fi Manager to return the Saved Network ID for
 * currently connected Wi-Fi Access Point. Interface returns a value containing
 * an ID of the connected Saved Network or 0 if not connected. This value is
 * to be used by the interfaces that manage Saved Networks.
 * The data must be allocated by the calling application and is accessed via
 * the provided reference pointer. If not connected to any access point, the
 * Network ID will be 0.
 *
 * @param [in]  pCtx  Pointer to QWF Service context.
 * @param [out]  pNetworkId  Reference pointer to a variable to store the
 *        connected Saved Network ID.
 *
 * @return ::WIFI_OK               Successful result
 * @return ::WIFI_ERR_INACTIVE     Service inactive or not ready
 * @return ::WIFI_ERR_INT          Internal Error
 * @return ::WIFI_ERR_PARAM        Invalid parameter
 *
 */
qwf_wifi_Results_e qwf_wifi_GetConnectedSavedNetworkId(qwf_Context_t *pCtx,
                                                       uint16_t * const pNetworkId);


/**
 * @brief Connect to the specified scanned Wi-Fi Access Point
 *
 * Interface to request the Wi-Fi Manager to connect to a recently scanned Wi-Fi
 * Access Point. After getting scan results via the qwf_wifi_GetScanResults()
 * api, this interface can be used to create a Saved Network using a scan
 * result index (scanResultId) and enable the Saved Network so that it will
 * attampt to connect to the Access Point.
 * A Saved Network is created using scan result data such as SSID and
 * authentication protocol.
 * Prior to using qwf_wifi_ConnectToAccessPoint() interface, additional
 * interfaces must be called to setup authentication parameters, if required
 * by the scanned Access Point, as follows:
 *   - qwf_wifi_SetPassphrase() must be called for WPA-PSK and WPA2-PSK
 *     authentication protocols
 *   - qwf_wifi_SetWepKey() must be called for WEP authentication protocol
 *   - qwf_wifi_SetEnterpriseCredentials() must be called for WPA-EAP and
 *     WPA2-EAP authentication protocols
 *
 * @param [in]  pCtx  Pointer to QWF Service context.
 * @param [in]  scanResultId  ID or index from the most recent array of
 *        scanned result data.
 *
 * @return ::WIFI_OK               Successful result
 * @return ::WIFI_ERR_INACTIVE     Service inactive or not ready
 * @return ::WIFI_ERR_INT          Internal Error
 * @return ::WIFI_ERR_PARAM        Invalid parameter
 *
 */
qwf_wifi_Results_e qwf_wifi_ConnectToAccessPoint(qwf_Context_t *pCtx,
                                                 uint8_t scanResultId);


/**
 * @brief Interface to request the Wi-Fi Manager to disconnect Wi-Fi
 *
 * Interface to "disconnect" the active Wi-Fi connection by "Disabling" the
 * Saved Network profile that is the current active Wi-Fi connection.
 * @b Note: If another Saved Network profile is enabled, the Wi-Fi Manager will
 * attempt to make a connection to the Access Point if available.
 *
 * @param [in]  pCtx  Pointer to QWF Service context.
 *
 * @return ::WIFI_OK               Successful result
 * @return ::WIFI_ERR_INACTIVE     Service inactive or not ready
 * @return ::WIFI_ERR_INT          Internal Error
 * @return ::WIFI_ERR_PARAM        Invalid parameter
 *
 */
qwf_wifi_Results_e qwf_wifi_Disconnect(qwf_Context_t *pCtx);

/**
 * @brief Set optional hostapd parameters for a Wi-Fi Access Point
 *
 * Interface to set up optional parameters used by hostapd when the device is
 * to act as a Wi-Fi Access Point. This interface may be called prior to
 * starting or updating a Wi-Fi Access Point using interfaces
 * qwf_wifi_StartAccessPoint() or qwf_wifi_UpdateAccessPoint().
 * @b Note: The hostapd parameters used in this interface are to be set as
 * defined by the sample hostapd.conf configuration file provided in the
 * wireless package. Please refer to the sample hostapd.conf configuration
 * file for additional descriptions of these optional parameters.
 *
 * @param [in]  pCtx  Pointer to QWF Service context.
 * @param [in]  channelNum  Channel number to use for this access point. Set
 *        to QWF_WIFI_AUTO_AP_CHANNEL to let the Wi-Fi Manager choose channel
 *        number.
 * @param [in]  vhtOperChWidth   VHT operating channel width setting used by
 *        hostapd for the Access Point being started or updated.
 * @param [in]  vhtOperCenterFreqSeg0  VHT center frequency index (1st
 *        segment) used by hostapd for the Access Point being started or
 *        updated. Maximum value of 173. Value of 0 indicates the Wi-Fi
 *        Supplicant will use it's default value.
 * @param [in]  vhtOperCenterFreqSeg1  VHT center frequency index (2nd
 *        segment) used by hostapd for the Access Point being started or
 *        updated. Maximum value of 173. Value of 0 indicates the Wi-Fi
 *        Supplicant will use it's default value.
 * @param [in]  pCountryCode Pointer to char string specifying a 2 character
 *        short country code used by hostapd. String has a maximum length of 2
 *        characters. Note that this parameter can be NULL if Country Code is
 *        not specified.
 * @param [in]  pHtCapabilities  Pointer to char string specifying HT
 *        capability flags used by hostapd. String has a maximum length of
 *        QWF_WIFI_MAX_CAPABILTIES_LEN characters and is formatted as specified
 *        in the hostapd.conf file (e.g. "[HT40-][SHORT-GI-20][SHORT-GI-40]".
 *        Note that this parameter can be NULL if HT capabilities not used.
 * @param [in]  pVhtCapabilities  Pointer to char string specifying VHT
 *        capability flags used by hostapd. String has a maximum length of
 *        QWF_WIFI_MAX_CAPABILTIES_LEN characters and is formatted as specified
 *        in the hostapd.conf file (e.g. "[SHORT-GI-80][HTC-VHT]". Note that
 *        this parameter can be NULL if VHT capabilities not used.
 * @param [in]  pAssocVse  Pointer to char string representing the Association
 *        VSE (Vendor Specific Element) of the Access Point to be used by the
 *        hostapd as a filter against the VSE of an incoming Association Request
 *        (or Re-Association Request). String has a maximum length defined by
 *        QWF_WIFI_MAX_HOSTAPD_VSE_LEN. When set, the hostapd only allows
 *        connection to clients whose VSE contains this "associationVSE". This
 *        option is Wi-Fi driver specific, refer to driver documentation for
 *        more detail. Note that this parameter can be NULL if Association VSE
 *        not used.
 * @param [in] pVendorElements  Pointer to char string containing one or more
 *        VSEs (Vendor Specific Element) to be used by the hostapd when
 *        broadcasting a Beacon or Probe. String has a maximum length defined by
 *        QWF_WIFI_MAX_HOSTAPD_VSE_LEN. See the "vendor_elements" parameter in
 *        the "hostapd.conf" file for additional information on usage. Note that
 *        this parameter can be NULL if VSEs are not used.
 *
 * @return ::WIFI_OK               Successful result
 * @return ::WIFI_ERR_INACTIVE     Service inactive or not ready
 * @return ::WIFI_ERR_INT          Internal Error
 * @return ::WIFI_ERR_PARAM        Invalid parameter
 *
 */
qwf_wifi_Results_e qwf_wifi_SetHostapdOptions(qwf_Context_t *pCtx,
                                              int channelNum,
                                              qwf_wifi_VhtOperChanWidth_e vhtOperChWidth,
                                              uint8_t vhtOperCenterFreqSeg0,
                                              uint8_t vhtOperCenterFreqSeg1,
                                              char *pCountryCode,
                                              char *pHtCapabilities,
                                              char *pVhtCapabilities,
                                              char *pAssocVse,
                                              char *pVendorElements);


/**
 * @brief Make the Wi-Fi Manager serve as a Wi-Fi Access Point
 *
 * Interface to request the Wi-Fi Manager to act as a Wi-Fi Access Point to
 * allow remote clients to connect this device. This interface creates a
 * persisted profile to be used as an Access Point. The Wi-Fi manager enters
 * into "AP" mode using this configured Access Point profile.
 *
 * Prior to using qwf_wifi_StartAccessPoint() interface, additional interfaces
 * must be called to setup authentication parameters, if required by the
 * scanned Access Point, as follows:
 *   - qwf_wifi_SetPassphrase() must be called for WPA-PSK and WPA2-PSK
 *     authentication protocols
 *   - qwf_wifi_SetWepKey() must be called for WEP authentication protocol
 *   - qwf_wifi_SetEnterpriseCredentials() must be called for WPA-EAP and
 *     WPA2-EAP authentication protocols
 *
 * Also additional optional hostapd related parameters may be set if needed
 * using the interface qwf_wifi_SetHostApdOptions(). See that interface
 * description for details.
 *
 * @param [in]  pCtx  Pointer to QWF Service context.
 * @param [in]  pSsid  Pointer to SSID name in data structure of type
 *        ::qwf_wifi_Ssid_t.
 * @param [in]  authProtocol  Authentication protocol as defined by type
 *        ::qwf_wifi_SecurityProtocol_e.
 * @param [in]  maxClients  Maximum number of clients allowed on
 *        this access point. Currently 8 is the upper limit.
 * @param [in]  standard  802.11 standard to use on this Access Point as
 *        defined by type ::qwf_wifi_Standard_e.
 * @param [in]  band  Frequency band to use on this Access Point as defined by
 *        type ::qwf_wifi_FrequencyBand_e.
 * @param [in]  privacy  Boolean value set to "true" if connected clients are
 *        not allowed to "see" other connected clients on network
 * @param [in]  dhcpLeaseTime  DHCP Lease Time in minutes defining the time
 *        before the DHCP IP address needs to be renewed.
 * @param [in]  pGatewayIpAddress  Pointer to char string representing the
 *        Gateway's IP Address
 * @param [in]  pGatewaySubnet  Pointer to char string representing the
 *        Gateway's Subnet mask
 *
 * @return ::WIFI_OK               Successful result
 * @return ::WIFI_ERR_INACTIVE     Service inactive or not ready
 * @return ::WIFI_ERR_INT          Internal Error
 * @return ::WIFI_ERR_PARAM        Invalid parameter
 * @return ::WIFI_ERR_EXISTS       Access Point already started
 *
 */
qwf_wifi_Results_e qwf_wifi_StartAccessPoint(qwf_Context_t *pCtx,
                                             qwf_wifi_Ssid_t *pSsid,
                                             qwf_wifi_SecurityProtocol_e authProtocol,
                                             uint16_t maxClients,
                                             qwf_wifi_Standard_e standard,
                                             qwf_wifi_FrequencyBand_e band,
                                             bool privacy,
                                             uint16_t dhcpLeaseTime,
                                             char *pGatewayIpAddress,
                                             char *pGatewaySubnet);

/**
 * @brief Disable the Wi-Fi Access Point
 *
 * Interface to request the Wi-Fi Manager to stop acting as a Wi-Fi Access
 * Point.
 *
 * @param [in]  pCtx  Pointer to QWF Service context.
 *
 * @return ::WIFI_OK               Successful result
 * @return ::WIFI_ERR_INACTIVE     Service inactive or not ready
 * @return ::WIFI_ERR_INT          Internal Error
 * @return ::WIFI_ERR_PARAM        Invalid parameter
 *
 */
qwf_wifi_Results_e qwf_wifi_StopAccessPoint(qwf_Context_t *pCtx);

/**
 * @brief Resume the Wi-Fi Access Point
 *
 * Interface to request the Wi-Fi Manager to resume acting as a Wi-Fi Access
 * Point if the Access Point becomes suspended.
 *
 * @param [in]  pCtx  Pointer to QWF Service context.
 *
 * @return ::WIFI_OK               Successful result
 * @return ::WIFI_ERR_INACTIVE     Service inactive or not ready
 * @return ::WIFI_ERR_INT          Internal Error
 * @return ::WIFI_ERR_PARAM        Invalid parameter
 *
 */
qwf_wifi_Results_e qwf_wifi_ResumeAccessPoint(qwf_Context_t *pCtx);

/**
 * @brief Update the Access Point being used by Wi-Fi Manager serving as a Wi-Fi
 *        Access Point
 *
 * Interface to request the Wi-Fi Manager to update the persisted profile of
 * the Access Point being used by Wi-Fi Manager. This Access Point allows the
 * device to act as a Wi-Fi Access Point which allows remote clients to connect
 * to this device.
 *
 * Prior to using qwf_wifi_UpdateAccessPoint() interface, additional
 * interfaces must be called to setup authentication parameters, if required
 * by the scanned Access Point, as follows:
 *   - qwf_wifi_SetPassphrase() must be called for WPA-PSK and WPA2-PSK
 *     authentication protocols
 *   - qwf_wifi_SetWepKey() must be called for WEP authentication protocol
 *   - qwf_wifi_SetEnterpriseCredentials() must be called for WPA-EAP and
 *     WPA2-EAP authentication protocols
 *
 * Also additional optional hostapd related parameters may be set if needed
 * using the interface qwf_wifi_SetHostApdOptions(). See that interface
 * description for details.
 *
 * @param [in]  pCtx  Pointer to QWF Service context.
 * @param [in]  pSsid  Pointer to SSID name in data structure of type
 *        ::qwf_wifi_Ssid_t.
 * @param [in]  authProtocol  Authentication protocol as defined by type
 *        ::qwf_wifi_SecurityProtocol_e.
 * @param [in]  maxClients  Maximum number of clients allowed on this access
 *        point.  Currently 8 is the upper limit.
 * @param [in]  standard  802.11 standard to used on this Access Point as
 *        defined by type ::qwf_wifi_Standard_e.
 * @param [in]  band  Frequency band to used on this Access Point as defined
 *        by type ::qwf_wifi_FrequencyBand_e.
 * @param [in]  privacy  Boolean value set to "true" if connected clients are
 *        not allowed to "see" other connected clients on network
 * @param [in]  dhcpLeaseTime  DHCP Lease Time in minutes defining the time
 *        before the DHCP IP address needs to be renewed.
 * @param [in]  pGatewayIpAddress  Pointer to char string representing the
 *        Gateway's IP Address
 * @param [in]  pGatewaySubnet  Pointer to char string representing the
 *        Gateway's Subnet mask
 *
 * @return ::WIFI_OK               Successful result
 * @return ::WIFI_ERR_INACTIVE     Service inactive or not ready
 * @return ::WIFI_ERR_INT          Internal Error
 * @return ::WIFI_ERR_PARAM        Invalid parameter
 *
 */
qwf_wifi_Results_e qwf_wifi_UpdateAccessPoint(qwf_Context_t *pCtx,
                                              qwf_wifi_Ssid_t *pSsid,
                                              qwf_wifi_SecurityProtocol_e authProtocol,
                                              uint16_t maxClients,
                                              qwf_wifi_Standard_e standard,
                                              qwf_wifi_FrequencyBand_e band,
                                              bool privacy,
                                              uint16_t dhcpLeaseTime,
                                              char *pGatewayIpAddress,
                                              char *pGatewaySubnet);

/**
 * @brief Query the Access Point profile configuration.
 *
 * Interface to request the Wi-Fi Manager to return the configuration settings
 * for the Wi-Fi Access Point.
 *
 * @param [in]  pCtx  Pointer to QWF Service context.
 * @param [out]  pApProfile  Reference pointer to ::qwf_wifi_ApProfileData_t
 *        data to hold the Access Point profile data.
 *
 * @return ::WIFI_OK               Successful result
 * @return ::WIFI_ERR_INACTIVE     Service inactive or not ready
 * @return ::WIFI_ERR_INT          Internal Error
 * @return ::WIFI_ERR_PARAM        Invalid parameter
 * @return ::WIFI_ERR_NOT_EXIST    Profile does not exist
 *
 */
qwf_wifi_Results_e qwf_wifi_GetAccessPointProfile(qwf_Context_t *pCtx,
                                                  qwf_wifi_ApProfileData_t * const pApProfile);


/**
 * @brief Request list of clients connected to Access Point.
 *
 * Interface to request the Wi-Fi Manager to return the list of currently
 * connected clients for the active Wi-Fi Access Point.
 *
 * @param [in]  pCtx  Pointer to QWF Service context.
 * @param [in,out]  pNumEntries  Reference pointer to the number of clients
 *        requested. The pResultsArray structure should be sized using this
 *        variable. Upon return, this variable is updated to show the actual
 *        number of access points in the array if less than requested.
 * @param [out]  pApClientArray  Reference pointer to an array of
 *        ::qwf_wifi_ApClientData_t data to hold the list of connected clients
 *        to this Access Point.
 *
 * @return ::WIFI_OK               Successful result
 * @return ::WIFI_ERR_INACTIVE     Service inactive or not ready
 * @return ::WIFI_ERR_INT          Internal Error
 * @return ::WIFI_ERR_PARAM        Invalid parameter
 * @return ::WIFI_ERR_NOT_EXIST    Access Point is not active
 *
 */
qwf_wifi_Results_e qwf_wifi_GetApClients(qwf_Context_t *pCtx,
                                         uint8_t *pNumEntries,
                                         qwf_wifi_ApClientData_t * const pApClientArray);


/**
 * @brief Client's function prototype used when registering callback function
 *        to process generated Wi-Fi Manager related events.
 *
 * Callback registered by the client to notify when an event has been
 * generated. This function is called if there is a Wi-Fi Manager event in the
 * event queue to be passed to client.
 *
 * @param [in]  pCtx Pointer to QWF Service context.
 * @param [in]  pEvent Pointer to Wi-Fi Manager event data.
 * @param [in]  pClientData  Pointer to client's data to be used with this
 *             event handler set up with interface
 *             qwf_wifi_RegisterCallback(). Wi-Fi Manager simply returns this
 *             pointer back to the client. NULL is a valid value indicating
 *             not used.
 *
 */
typedef void (*qwf_wifi_EventClientCallback_t)(qwf_Context_t *pCtx,
                                               qwf_wifi_Event_t *pEvent,
                                               void *pClientData);

/**
 * @brief Register a callback for the Wi-Fi Manager service.
 *
 * @param [in]  pCtx  Pointer to QWF Service context.
 * @param [in]  eventHandler   The callback to handle Wi-Fi Manager events.
 * @param [in]  pClientData  Pointer to client's data to be used with this
 *             event handler. Note this is optional to allow client to
 *             associate an extra data structure with this context. Wi-Fi
 *             Manager simply returns this pointer back to the client when
 *             eventHandler callback is invoked. NULL is a valid value
 *             indicating not used.
 *
 * @return ::WIFI_OK               Successful result
 * @return ::WIFI_ERR_INACTIVE     Service inactive
 * @return ::WIFI_ERR_INT          Internal Error
 * @return ::WIFI_ERR_PARAM        Invalid parameter
 *
 */
qwf_wifi_Results_e qwf_wifi_RegisterCallback(const qwf_Context_t * const pCtx,
                                             qwf_wifi_EventClientCallback_t eventHandler,
                                             void *pClientData);

/** @}*/ // end group QWF_WIFI

__END_DECLS

#endif /* _QWF_WIFI_H_INCLUDED */

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/wireless_framework/branches/7.1/trunk/common/lib/qwf_interface/public/qwf/qwf_wifi.h $ $Rev: 2797 $")
#endif

