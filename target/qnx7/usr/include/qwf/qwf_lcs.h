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
 * @brief  This interface provides access to the LoCation Service (LCS).
 *         This interface currently only supports autonomous GPS devices.
 *
 * @details This interface gives the client the ability to control the basic GPS
 *          location functionality from the device and retrieve its associated
 *          configuration and state information.
 *
 *          Usage:
 *          - Client application calls qwf_Init() to initialize the QWF Context
 *            data used as container for QWF and service data (like LCS)
 *          - Client application calls qwf_lcs_RegisterCallback() to register a
 *            callback function used to process events from the QWF LCS
 *            component. Note: similar callbacks will be registered for each
 *            service the application is interested in.
 *          - Client application calls the qwf_ProcessEvents() function to
 *            block and wait for events to be sent to each services callback
 *            which was registered during initialization
 *          - The client application processes the events as required.
 *
 *          For code examples, refer to @ref QWF_EXAMPLES_LCS
 */

#ifndef _QWF_LCS_H_INCLUDED
#define _QWF_LCS_H_INCLUDED

#include <qwf/qwf_interface.h>
#include <stdbool.h>
#include <sys/types.h>

__BEGIN_DECLS

/**
 * @defgroup QWF_LCS QWF Location Services
 * @{
 */

/**
 * @brief Return values used by QWF Location Services interface
 *
 * @details This enumeration defines the possible LCS return values.
 */
typedef enum
{
    LCS_OK                 = 0,          ///< Successful result
    LCS_ERR_INT            = 1,          ///< Failure - Internal
    LCS_ERR_PARAM          = 2,          ///< Failure - Invalid parameter
    LCS_ERR_INACTIVE       = 3,          ///< Failure - Inactive service
    LCS_ERR_EXISTS         = 4           ///< LCS service already exists or has been initialized
} qwf_lcs_Results_e;

/**
 * @brief Event codes for Location services events.
 *
 * @details This enumeration defines the possible LCS event types.
 */
typedef enum
{
    LCS_EVENT_CMD_STATUS = 0,            ///< Command status response event
    LCS_EVENT_COMM_TIMEOUT,              ///< Communications timeout
    LCS_EVENT_HDW_STATUS,                ///< Hardware status update event
    LCS_EVENT_LOCATION_FIX,              ///< Real time location fix event
    LCS_EVENT_JAM_DETECT,                ///< Jamming detection response event
    LCS_EVENT_PSMODE,                    ///< Power saving mode response event
    LCS_EVENT_SENSOR_UPDATE,             ///< DR Sensor status response event
    LCS_EVENT_ANTENNA_STATUS,            ///< Antenna status response event
    LCS_EVENT_SATSYS_STATUS,             ///< Satellite system response event
    LCS_EVENT_SATINFO_STATUS,            ///< Satellite information response event
    LCS_EVENT_VEHATT_STATUS,             ///< Vehicle attitude response event
    LCS_EVENT_TPCFG_STATUS,              ///< Time pulse configuration response event
    LCS_EVENT_LCS_SERVICE_INACTIVE,      ///< LCS service is inactive event
    LCS_EVENT_LCS_SERVICE_ACTIVE,        ///< LCS service is active event
    // Insert new event codes before this line.
    LCS_EVENT_NONE
} qwf_lcs_EventCode_e;

/**
 * @brief Command codes for Location services.
 *
 * @details This enumeration defines the possible LCS command types.
 */
typedef enum {
    LCS_CMD_POWER = 0,                   ///< Power state change command
    LCS_CMD_ASSIST,                      ///< Assistance file load command
    LCS_CMD_LOGGING,                     ///< Location logging state change command
    LCS_CMD_PSMODE,                      ///< Power saving mode state change command
    LCS_CMD_JAMMING,                     ///< Jamming detection state change command
    LCS_CMD_SATSYS,                      ///< Satellite system change command
    LCS_CMD_INFOMSG,                     ///< Information message change command
    LCS_CMD_EXTSENSMEAS,                 ///< External sensor measurement input command
    LCS_CMD_TPCFG,                       ///< Time pulse configuration change command
    LCS_CMD_MEASRATE,                    ///< Navigation measurement rate change command
    // Place new LCS commands before this line.
    LCS_CMD_MAX_CMDS
} qwf_lcs_Cmd_e;

/**
 * @brief Power command modes for Location services.
 *
 * @details This enumeration defines the possible LCS power command types.
 */
typedef enum {
    LCS_POWER_ON = 0,                    ///< Power ON or wake device from sleep
    LCS_POWER_OFF,                       ///< Power OFF device
    LCS_POWER_SLEEP,                     ///< Device sleep
    LCS_POWER_INVALID                    ///< Power invalid
} qwf_lcs_PowerState_e;

/**
 * @brief Logging command modes for Location services.
 *
 * @details This enumeration defines the possible LCS logging command types.
 */
typedef enum {
    LCS_LOGGING_ON = 0,                  ///< Location logging enable
    LCS_LOGGING_OFF,                     ///< Location logging disable
    LCS_LOGGING_READ,                    ///< Location log read
    LCS_LOGGING_INVALID                  ///< Location log invalid
} qwf_lcs_LoggingState_e;

/**
 * @brief Power saving command modes for Location services.
 *
 * @details This enumeration defines the possible LCS power saving command types.
 */
typedef enum {
    LCS_PSMODE_ON = 0,                   ///< Power saving mode enable
    LCS_PSMODE_OFF,                      ///< Power saving mode disable
    LCS_PSMODE_INVALID                   ///< Power saving invalid
} qwf_lcs_PowerSaveMode_e;

/**
 * @brief Command status codes for Location services.
 *
 * @details This enumeration defines the possible LCS command status types that
 *          relate to the LCS_EVENT_CMD_STATUS.
 */
typedef enum {
    LCS_CMD_STATUS_SUCCESS = 0,          ///< Status success
    LCS_CMD_STATUS_ERROR,                ///< Status error
    LCS_CMD_STATUS_BUSY,                 ///< Status busy, command running
    LCS_CMD_STATUS_SLEEP,                ///< Status sleep
    LCS_CMD_STATUS_PSMODE,               ///< Status power saving mode enabled
} qwf_lcs_CmdStatus_e;

/**
 * @brief Antenna power states for Location services.
 *
 * @details This enumeration defines the possible LCS antenna power state types.
 */
typedef enum {
    LCS_ANTENNA_POWER_OFF = 0,
    LCS_ANTENNA_POWER_ON,
    LCS_ANTENNA_POWER_UNKNOWN
} qwf_lcs_AntPwr_e;

/**
 * @brief Reset types within the GNSS device.
 *
 * @details This enumeration defines the possible reset types supported within the device.
 *
 * @note: The hardware and software resets go through the same sequences as a power off and on.
 *        The hdwRunning flag will switch to off, then back on again.  The status will then show
 *        status busy until the configuration of the chip has completed again.
 */
typedef enum {
    LCS_RESET_TYPE_EXT_HARDWARE = 0,    ///< This type requires support in qwfgps_hw library.
    LCS_RESET_TYPE_INT_HARDWARE,        ///< GNSS device hardware internal (watchdog) reset.
    LCS_RESET_TYPE_INT_SOFTWARE,        ///< GNSS device full software reset.
    LCS_RESET_TYPE_INT_GNSS_SOFTWARE,   ///< GNSS internal satellite processing reset.
    LCS_RESET_TYPE_INT_GNSS_STOP,       ///< GNSS stop internal satellite processing.
    LCS_RESET_TYPE_INT_GNSS_START,      ///< GNSS start internal satellite processing.
    LCS_RESET_TYPE_INT_UNKNOWN          ///< Unknown value.
} qwf_lcs_ResetType_e;

/**
 * @brief Features to clear on a reset.
 *
 * @details This enumeration defines the possible parameters to clear on resetting device.
 *
 * @note These cannot be cleared during a LCS_RESET_TYPE_EXT_HARDWARE reset.
 */
typedef enum {
    LCS_RESET_CLEAR_NONE       = 0x0000,    ///< Clear no GNSS software parameters.
    LCS_RESET_CLEAR_ALMANAC    = 0x0001,    ///< Clear satellite almanac related parameters.
    LCS_RESET_CLEAR_EPHEMERIS  = 0x0002,    ///< Clear satellite ephemeris related parameters.
    LCS_RESET_CLEAR_HEALTH     = 0x0004,    ///< Clear satellite health related parameters.
    LCS_RESET_CLEAR_IONOSPHERE = 0x0008,    ///< Clear ionospheric noise related parameters.
    LCS_RESET_CLEAR_POSITION   = 0x0010,    ///< Clear position related parameters.
    LCS_RESET_CLEAR_CLKDRIFT   = 0x0020,    ///< Clear clock drift related parameters.
    LCS_RESET_CLEAR_OSCILLATOR = 0x0040,    ///< Clear clock oscillator related parameters.
    LCS_RESET_CLEAR_UTCTIME    = 0x0080,    ///< Clear UTC time related parameters.
    LCS_RESET_CLEAR_RTCTIME    = 0x0100,    ///< Clear real time clock hardware related parameters.
    LCS_RESET_CLEAR_AUTOORBIT  = 0x0200,    ///< Clear satellite orbit estimation related parameters.
    LCS_RESET_CLEAR_ALL        = 0xFFFF     ///< Clear all above GNSS software parameters.
} qwf_lcs_ResetClr_e;

/**
 * @brief Antenna status codes for Location services.
 *
 * @details This enumeration defines the possible LCS antenna status types.
 */
typedef enum {
    LCS_ANTENNA_STATE_INIT = 0,
    LCS_ANTENNA_STATE_OK,
    LCS_ANTENNA_STATE_SHORTCIRCUIT,
    LCS_ANTENNA_STATE_OPENCIRCUIT,
    LCS_ANTENNA_STATE_UNKNOWN
} qwf_lcs_AntState_e;

/**
 * @brief Jamming status codes for Location services.
 *
 * @details This enumeration defines the possible LCS jamming detection status types.
 */
typedef enum {
    LCS_CMD_JAMDET_OFF = 0,              ///< Jamming detection is OFF or in unknown state
    LCS_CMD_JAMDET_NONE,                 ///< Jamming detection is ON and no jamming detected
    LCS_CMD_JAMDET_MINOR_INT,            ///< Jamming detection is ON and minor jamming detected
    LCS_CMD_JAMDET_CRITICAL_INT,         ///< Jamming detection is ON and critical jamming detected
} qwf_lcs_JamDetect_e;

/**
 * @brief Combined GNSS and Dead Reckoning sensor status codes for Location services.
 *
 * @details This enumeration defines the possible Combined GNSS and Dead Reckoning sensor information
 */
typedef enum {
    LCS_COMBSENS_INIT = 0,    ///< sensor initialization mode
    LCS_COMBSENS_GNSSANDDR,   ///< sensor and GNSS data combined
    LCS_COMBSENS_SUSPENDED,   ///< sensor combination mode temporarily disabled
    LCS_COMBSENS_DISABLED,    ///< sensor combination mode permanently disabled
    LCS_COMBSENS_UNKNOWN
} qwf_lcs_CombSensors_e;

/**
 * @brief Dead Reckoning sensor types for Location services.
 *
 * @details This enumeration defines the possible Dead Reckoning sensor types information
 */
typedef enum {
    LCS_DRSENSOR_NONE = 0,
    LCS_DRSENSOR_FLWHEEL,     ///< front-left wheel ticks
    LCS_DRSENSOR_FRWHEEL,     ///< front-right wheel ticks
    LCS_DRSENSOR_RLWHEEL,     ///< rear-left wheel ticks
    LCS_DRSENSOR_RRWHEEL,     ///< rear-right wheel ticks
    LCS_DRSENSOR_SINGTICK,    ///< single tick (speed tick)
    LCS_DRSENSOR_SPEED,       ///< speed
    LCS_DRSENSOR_GYROTEMP,    ///< gyroscope temperature
    LCS_DRSENSOR_XGYROANGL,   ///< x-axis gyroscope angular rate
    LCS_DRSENSOR_YGYROANGL,   ///< y-axis gyroscope angular rate
    LCS_DRSENSOR_ZGYROANGL,   ///< z-axis gyroscope angular rate
    LCS_DRSENSOR_XACCFORCE,   ///< x-axis accelerometer specific force
    LCS_DRSENSOR_YACCFORCE,   ///< y-axis accelerometer specific force
    LCS_DRSENSOR_ZACCFORCE,   ///< z-axis accelerometer specific force
    LCS_DRSENSOR_MAX          // Please place new sensor defines before this line.
} qwf_lcs_DRSensorType_e;

/**
 * @brief Dead Reckoning sensor status for Location services.
 *
 * @details This enumeration defines the possible Dead Reckoning sensor status information
 */
typedef enum {
    LCS_DRSENSOR_STATUS_NONE        = 0x00,   ///< no update
    LCS_DRSENSOR_STATUS_READY       = 0x01,   ///< sensor ready
    LCS_DRSENSOR_STATUS_CALIBRATING = 0x02,   ///< sensor calibrating
    LCS_DRSENSOR_STATUS_CALIBRATED  = 0x04,   ///< sensor calibrated
    LCS_DRSENSOR_STATUS_USED        = 0x08,   ///< sensor used
    LCS_DRSENSOR_STATUS_BADMEAS     = 0x10,   ///< sensor bad measurement detected
    LCS_DRSENSOR_STATUS_BADTIME     = 0x20,   ///< sensor bad time detected
    LCS_DRSENSOR_STATUS_MISSMEAS    = 0x40,   ///< sensor missing or time misaligned measurement
    LCS_DRSENSOR_STATUS_NOISYMEAS   = 0x80,   ///< sensor noisy measurement
    LCS_DRSENSOR_STATUS_MAX         = 0xFF
} qwf_lcs_DRSensorStatus_e;

/**
 * @brief Satellite system selection.
 *
 * @details This enumeration defines the possible satellite system types to enable.
 *          Not all systems may be supported together.
 */
typedef enum {
    LCS_SATELLITE_SYSTEM_NONE        = 0x00,   ///< no satellite systems
    LCS_SATELLITE_SYSTEM_GPS         = 0x01,   ///< main, US based
    LCS_SATELLITE_SYSTEM_GLONASS     = 0x02,   ///< main, Russian based
    LCS_SATELLITE_SYSTEM_BEIDOU      = 0x04,   ///< main, Chinese based
    LCS_SATELLITE_SYSTEM_GALILEO     = 0x08,   ///< main, EU based
    LCS_SATELLITE_SYSTEM_SBAS        = 0x10,   ///< Space Based Augmentation Systems
    LCS_SATELLITE_SYSTEM_IMES        = 0x20,   ///< International MEssaging System
    LCS_SATELLITE_SYSTEM_QZSS        = 0x40,   ///< Quasi Zenith Satellite System
    LCS_SATELLITE_SYSTEM_ALL         = 0xFF
} qwf_lcs_SatSystems_e;

/**
 * @brief Satellite response types.
 *
 * @details This enumeration defines the possible satellite types to enable in information response.
 */
typedef enum {
    // Main satellite system types
    LCS_GNSS_TYPE_GPS = 0,
    LCS_GNSS_TYPE_GLONASS,
    LCS_GNSS_TYPE_GALILEO,
    LCS_GNSS_TYPE_BEIDOU,
    // Augmentation satellite system types
    LCS_GNSS_TYPE_SBAS,
    LCS_GNSS_TYPE_IMES,
    LCS_GNSS_TYPE_QZSS,
    LCS_GNSS_TYPE_UNKNOWN
} qwf_lcs_GnssType_e;

/**
 * @brief Information message selection.
 *
 * @details This enumeration defines the possible information message types to enable.
 */
typedef enum {
    LCS_INFO_MSG_SAT_INFO = 0,      ///< Satellite vehicle information
    LCS_INFO_MSG_VEH_ATT            ///< Automotive vehicle attitude information
} qwf_lcs_InfoMsg_e;

/**
 * @brief Satellite time grid synchronization types.
 *
 * @details This enumeration defines the possible satellite time grid synchronization types.
 */
typedef enum {
    LCS_TIMEGRID_UTC = 0,
    LCS_TIMEGRID_GPS,
    LCS_TIMEGRID_GLONASS,
    LCS_TIMEGRID_BEIDOU,
    LCS_TIMEGRID_GALILEO,
    LCS_TIMEGRID_MAX
} qwf_lcs_TimeGrid_e;

/**
 * @brief Defines the structure for LCS power ON /OFF response information from the device
 */
#define LCS_MAN_STR   25
#define LCS_SWV_STR   50

typedef struct {
    bool hdwRunning;                ///< Flag to show hardware is running or not
    double hdwStartTime;            ///< System time when device first turned on
    char manuNameStr[LCS_MAN_STR];  ///< Device manufacturer name
    char manuModelStr[LCS_MAN_STR]; ///< Device model
    char manuSoftVerStr[LCS_SWV_STR];///< Device firmware version
} qwf_lcs_PowerResp_t;

/**
 * @brief Defines the structure for LCS location fix information from the device.
 *        The same structure is used for real time and logged location fixes.
 */
typedef struct {
    int fixDate;               ///< Date fix was recorded (DDMMYY)
    int fixTime;               ///< Time fix was recorded (hhmmss)
    double latitude;           ///< Absolute location latitude (DDmm.mm)
    bool north;                ///< North latitude if true (T=+, F=-)
    double longitude;          ///< Absolute location longitude (DDmm.mm)
    bool east;                 ///< East longitude if true (T=+, F=-)
    double altitude;           ///< Location altitude (m)
    double heading;            ///< Direction heading
    double velocity;           ///< Direction speed (m/s)
    double hdop;               ///< Horizontal dilution of precision
    double horAcc;             ///< Horizontal accuracy (m)
    int numSVs;                ///< Number of satellite vehicles used in fix
    int fixType;               ///< Location fix 2D /3D
    double pdop;               ///< Position dilution of precision
    /**
     *  Spoofing indication
     *     0 - unknown or disabled
     *     1 - no spoofing detected
     *     2 - spoofing is detected
     *     3 - multiple spoofing indications
     */
    int spoof;
} qwf_lcs_LocationFix_t;

/**
 * @brief Defines the structure for LCS logged fix information from the device.
 *        The locArray parameter points to an allocated array of logged fixes.
 */
#define LCS_LOGGED_FIXES_MAX   100

typedef struct {
    double logReqTime;         ///< System time when request sent
    int numLocations;          ///< Number of location fixes in array
    qwf_lcs_LocationFix_t *locArray;   ///< Array of location fixes
} qwf_lcs_LoggedFixes_t;

/**
 * @brief Defines the structure for antenna status information.
 */
typedef struct {
    qwf_lcs_AntPwr_e   antennaPower;
    qwf_lcs_AntState_e antennaState;
} qwf_lcs_AntStatus_t;

/**
 * @brief Defines the structure for individual DR sensor information.
 */
typedef struct {
    qwf_lcs_DRSensorType_e   sensType;
    qwf_lcs_DRSensorStatus_e sensStat;
} qwf_lcs_SensorStat_t;

/**
 * @brief Defines the structure for reported DR sensor information.
 */
typedef struct {
    qwf_lcs_CombSensors_e combMode;
    int numSensors;
    qwf_lcs_SensorStat_t sensArr[LCS_DRSENSOR_MAX];
} qwf_lcs_NotifySensors_t;

/**
 * @brief Defines the structure for reported specific satellite information.
 */
typedef struct {
    qwf_lcs_GnssType_e gnssId;   ///< Satellite system type being reported
    int  svId;                   ///< Satellite vehicle ID
    int  cno;                    ///< Satellite signal to noise value
    int  elev;                   ///< Elevation of satellite to horizon
    int  azim;                   ///< Azimuth of satellite
    int  prRes;                  ///< Pseudorange residual
} qwf_lcs_SatInfoData_t;

/**
 * @brief Defines the structure for reported overall satellite information.
 * @note: An internal buffer is allocated on first report reception and the data can be retrieved
 *        through ::qwf_lcs_readSatInfoData() on receiving event.
 */
#define LCS_SATINFO_MAX_SATELLITES   256

typedef struct {
    int iTOW;                            ///< Integer time of week (ms)
    int numSvs;                          ///< Number of satellite vehicles being reported
    qwf_lcs_SatInfoData_t arrSatInfo[LCS_SATINFO_MAX_SATELLITES];   ///< Buffer holding data array
} qwf_lcs_SatData_t;

/**
 * @brief Defines the structure for reported vehicle attitude information.
 */
typedef struct {
    int    iTOW;                         ///< Integer time of week (ms)
    double roll;                         ///< Roll of vehicle reference frame (degrees)
    double pitch;                        ///< Pitch of vehicle reference frame (degrees)
    double heading;                      ///< Heading of vehicle reference frame (degrees)
    double accRoll;                      ///< Roll accuracy (degrees)
    double accPitch;                     ///< Pitch accuracy (degrees)
    double accHeading;                   ///< Heading accuracy (degrees)
} qwf_lcs_VehAttData_t;

/**
 * @brief Defines the structure to input external sensor data.
 */
#define LCS_SENSMEAS_DATA_SIZE   32

typedef struct {
    qwf_lcs_DRSensorType_e sensType;     ///< Sensor type defined in qwf_lcs_DRSensorType_e
    uint32_t               sensData;     ///< Sensor data value to write
} qwf_lcs_ExtDRSensData_t;

/**
 * @brief Defines the structure to input time pulse settings.
 */
typedef struct {
    int16_t  antCableDelay;        ///< Antenna cable delay parameter (ns)
    int16_t  rfGroupDelay;         ///< RF group delay parameter (ns)
    uint32_t pulsePeriod;          ///< Pulse period time parameter (us)
    uint32_t pulsePeriodLock;      ///< Pulse period time when locked to GNSS time parameter (us)
    uint32_t pulseLenRatio;        ///< Pulse length parameter (us)
    uint32_t pulseLenRatioLock;    ///< Pulse length time when locked to GNSS time parameter (us)
    int32_t  userConfigDelay;      ///< User configurable time pulse delay parameter (ns)
    qwf_lcs_TimeGrid_e selUtcGnss; ///< Time grid type defined in ::qwf_lcs_TimeGrid_e
    uint8_t  index;                ///< Index of selected time pulse configuration
    bool     active;               ///< Set to enable the selected time pulse configuration
    bool     lockGnssFreq;         ///< Synchronize time pulse to GNSS time when valid
    bool     lockOtherSet;         ///< Switch between pulse period and len to the locked parameters
    bool     alignToTow;           ///< Align pulse to top of second
    bool     risingEdge;           ///< Pulse polarity, rising edge at top of second if true
} qwf_lcs_TimePulseConfig_t;

/**
 * @brief Defines the structure for LCS command status information from the device
 */
typedef struct {
    qwf_lcs_Cmd_e lcsCmd;            ///< LCS command that this response relates to
    qwf_lcs_CmdStatus_e cmdStatus;   ///< Overall command status
    int errCode;                     ///< If status ERROR, errno value returned
    union {
        /**
         * Other power response information valid for LCS_CMD_POWER
         */
        qwf_lcs_PowerResp_t powerResp;
    } a;
} qwf_lcs_CmdStatus_t;

/**
 * @brief Defines the structure for LCS events from the API to the client
 */
typedef struct qwf_lcs_Event_s
{
    /**
     *  The actual event to be processed
     */
    qwf_lcs_EventCode_e eventCode;
    /**
     *  The client callback to process the event
     */
    void *pClientCB;
    /**
     *  Union of all the possible Event data members
     */
    union {
        qwf_lcs_CmdStatus_t       cmdResp;
        qwf_lcs_LocationFix_t     currLocFix;
        qwf_lcs_LoggedFixes_t     loggedFixes;
        qwf_lcs_NotifySensors_t   sensorStat;
        qwf_lcs_AntStatus_t       antennaStatus;
        qwf_lcs_VehAttData_t      vehAttitude;
        qwf_lcs_SatSystems_e      satSysStatus;
        qwf_lcs_TimePulseConfig_t tpConfig;
        int                       jamDetect;
        bool                      psMode;
    } lcsEvent;

} qwf_lcs_Event_t;

/**
 * @brief Client's function prototype used when registering callback function
 *        to process generated LCS related events.
 *
 * Callback registered by the client to notify when an event has been
 * generated. This function is called if there is a LCS event in the event
 * queue to be passed to client.
 *
 * @param [in]  pCtx Pointer to Service context.
 * @param [in]  pEvent Pointer to LCS event data.
 * @param [in]  pClientData  Pointer to client's data to be used with this
 *             event handler Note: this is optional to allow client to
 *             associate an extra data structure with this context. LCS
 *             service simply returns this pointer back to the client. NULL is
 *             a valid value indicating not used.
 *
 */
typedef void (*qwf_lcs_EventClientCallback_t)(qwf_Context_t *pCtx,
                                              qwf_lcs_Event_t *pEvent,
                                              void *pClientData);

/**
 * @brief Register a callback for the LCS service.
 *
 * @param [in]  pCtx  Pointer to Service context.
 * @param [in]  eventHandler   The callback to handle cellular data events.
 * @param [in]  pClientData  Pointer to client's data to be used with this
 *             event handler Note: this is optional to allow client to
 *             associate an extra data structure with this context. LCS
 *             service simply returns this pointer back to the client. NULL is
 *             a valid value indicating not used.
 *
 * @return ::LCS_OK               Successful result
 * @return ::LCS_ERR_INACTIVE     Service inactive
 * @return ::LCS_ERR_INT          Internal Error
 * @return ::LCS_ERR_PARAM        Invalid parameter
 *
 */
qwf_lcs_Results_e qwf_lcs_RegisterCallback(const qwf_Context_t * const pCtx,
                                           qwf_lcs_EventClientCallback_t eventHandler,
                                           void *pClientData);

/**
 * @brief LCS: Send command to power ON /OFF GPS device
 *
 * Interface to enable /disable power to a GPS device.
 *
 * @param [in]  pCtx  Pointer to Service context.
 * @param [in]  mode  ON, OFF, or SLEEP.
 * @param [in]  length Seconds of sleep.
 *
 * @return ::LCS_OK               Successful result
 * @return ::LCS_ERR_INACTIVE     Service inactive or not ready
 * @return ::LCS_ERR_INT          Internal Error
 * @return ::LCS_ERR_PARAM        Invalid parameter
 *
 */
qwf_lcs_Results_e qwf_lcs_setPower(qwf_Context_t *pCtx, qwf_lcs_PowerState_e mode, int length);

/**
 * @brief LCS: Send command to reset GPS device
 *
 * Interface to reset the GPS device through hardware or software mechanism.
 *
 * @param [in]  pCtx    Pointer to Service context.
 * @param [in]  type    qwf_lcs_ResetType_e.
 * @param [in]  swClr   qwf_lcs_ResetClr_e.
 *
 * @return ::LCS_OK               Successful result
 * @return ::LCS_ERR_INACTIVE     Service inactive or not ready
 * @return ::LCS_ERR_INT          Internal Error
 * @return ::LCS_ERR_PARAM        Invalid parameter
 *
 */
qwf_lcs_Results_e qwf_lcs_setReset(qwf_Context_t *pCtx, qwf_lcs_ResetType_e type,
                                   qwf_lcs_ResetClr_e swClr);

/**
 * @brief LCS: Send command to load assistance file GPS device
 *
 * Interface to load assistance file to a GPS device.
 *
 * @param [in]  pCtx  Pointer to Service context.
 * @param [in]  path  Path and file name to load.
 *
 * @return ::LCS_OK               Successful result
 * @return ::LCS_ERR_INACTIVE     Service inactive or not ready
 * @return ::LCS_ERR_INT          Internal Error
 * @return ::LCS_ERR_PARAM        Invalid parameter
 *
 */
qwf_lcs_Results_e qwf_lcs_setAssist(qwf_Context_t *pCtx, char *path);

/**
 * @brief LCS: Allocate memory to hold logged fixes.
 *
 * Interface to allocate memory to hold logged fixes.  This function returns a pointer to
 * the memory where the logged fixes will be stored.  The application will need to copy
 * the data before calling qwf_lcs_delLogMem().
 *
 * @b NOTE: This must be called before qwf_lcs_setLogging(READ) to allocate
 *          required memory.  The pLog pointer returned should only be freed using
 *          qwf_lcs_delLogMem() function.
 *
 * @param [in]  pCtx  Pointer to Service context.
 * @param [in]  pLog  Pointer to allocated memory pointer.
 *
 * @return ::LCS_OK               Successful result
 * @return ::LCS_ERR_INACTIVE     Service inactive or not ready
 * @return ::LCS_ERR_INT          Internal Error
 * @return ::LCS_ERR_PARAM        Invalid parameter
 *
 */
qwf_lcs_Results_e qwf_lcs_allocLogMem(qwf_Context_t *pCtx, const qwf_lcs_LoggedFixes_t **pLog);

/**
 * @brief LCS: Send command for logging ON /OFF /READ GPS device
 *
 * Interface to enable, disable, and read logging info on a GPS device.
 *
 * @b NOTE: The qwf_lcs_setLogging(READ) copies the logged fixes response from the
 *          device into allocated memory from qwf_lcs_allocLogMem().
 *
 * @param [in]  pCtx  Pointer to Service context.
 * @param [in]  mode  ON, OFF, or READ.
 * @see         qwf_lcs_allocLogMem(), qwf_lcs_delLogMem()
 *
 * @return ::LCS_OK               Successful result
 * @return ::LCS_ERR_INACTIVE     Service inactive or not ready
 * @return ::LCS_ERR_INT          Internal Error
 * @return ::LCS_ERR_PARAM        Invalid parameter
 *
 */
qwf_lcs_Results_e qwf_lcs_setLogging(qwf_Context_t *pCtx, qwf_lcs_LoggingState_e mode);

/**
 * @brief LCS: Free memory for logged fixes.
 *
 * Interface to free memory for logged fixes.
 *
 * @b NOTE: This must be called after qwf_lcs_setLogging(READ) to free read
 *          log memory after application use.
 *
 * @param [in]  pCtx  Pointer to Service context.
 *
 * @return ::LCS_OK               Successful result
 * @return ::LCS_ERR_INACTIVE     Service inactive or not ready
 * @return ::LCS_ERR_INT          Internal Error
 * @return ::LCS_ERR_PARAM        Invalid parameter
 *
 */
qwf_lcs_Results_e qwf_lcs_delLogMem(qwf_Context_t *pCtx);

/**
 * @brief LCS: Send command to power save mode ON /OFF GPS device
 *
 * Interface to enable /disable power save mode to a GPS device.
 *
 * @param [in]  pCtx  Pointer to Service context.
 * @param [in]  mode  ON, OFF.
 * @param [in]  cycle Seconds from on to on cycle (may not be exact).
 * @param [in]  onTime Seconds of on time (num of fixes).
 * @param [in]  ephUpdate  Boolean of whether to wake to update sat. ephemeris info.
 * @param [in]  rtcUpdate  Boolean of whether to wake to update GPS real time clock info.
 *
 * @return ::LCS_OK               Successful result
 * @return ::LCS_ERR_INACTIVE     Service inactive or not ready
 * @return ::LCS_ERR_INT          Internal Error
 * @return ::LCS_ERR_PARAM        Invalid parameter
 *
 */
qwf_lcs_Results_e qwf_lcs_setPowerSave(qwf_Context_t *pCtx, qwf_lcs_PowerSaveMode_e mode, int cycle,
                                       int onTime, bool ephUpdate, bool rtcUpdate);

/**
 * @brief LCS: Send command to set jamming ON /OFF GPS device
 *
 * Interface to enable /disable jamming detection on a GPS device.
 *
 * @param [in]  pCtx  Pointer to Service context.
 * @param [in]  enable  Boolean to enable /disable jamming detection.
 *
 * @return ::LCS_OK               Successful result
 * @return ::LCS_ERR_INACTIVE     Service inactive or not ready
 * @return ::LCS_ERR_INT          Internal Error
 * @return ::LCS_ERR_PARAM        Invalid parameter
 *
 */
qwf_lcs_Results_e qwf_lcs_setJamming(qwf_Context_t *pCtx, bool enable);

/**
 * @brief LCS: Send command to change satellite systems on GPS device
 *
 * Interface to change satellite systems or read current selected on a GPS device.
 *
 * @param [in]  pCtx    Pointer to Service context.
 * @param [in]  write   Boolean of whether to write (@c true) new settings or read (@c false) current settings.
 * @param [in]  satSys  ::qwf_lcs_SatSystems_e.
 *
 * @return ::LCS_OK               Successful result
 * @return ::LCS_ERR_INACTIVE     Service inactive or not ready
 * @return ::LCS_ERR_INT          Internal Error
 * @return ::LCS_ERR_PARAM        Invalid parameter
 *
 */
qwf_lcs_Results_e qwf_lcs_setSatSystems(qwf_Context_t *pCtx, bool write, qwf_lcs_SatSystems_e satSys);

/**
 * @brief LCS: Send command to enable /disable information message on GNSS device
 *
 * Interface to enable /disable information message on a GNSS device.
 *
 * @param [in]  pCtx    Pointer to Service context.
 * @param [in]  msg     ::qwf_lcs_InfoMsg_e.
 * @param [in]  enable  Boolean of whether to enable (@c true) selected message type.
 *
 * @return ::LCS_OK               Successful result
 * @return ::LCS_ERR_INACTIVE     Service inactive or not ready
 * @return ::LCS_ERR_INT          Internal Error
 * @return ::LCS_ERR_PARAM        Invalid parameter
 *
 */
qwf_lcs_Results_e qwf_lcs_setInfoMsg(qwf_Context_t *pCtx, qwf_lcs_InfoMsg_e msg, bool enable);

/**
 * @brief LCS: Read the data received in satellite information report.
 *
 * Interface to copy the satellite information data reported from GNSS device.
 *
 * @param [in]  pCtx        Pointer to Service context.
 * @param [in]  outSatData  ::qwf_lcs_SatData_t pointer to write info.
 * @param [in]  bufSize     Size of buffer to write data.
 *
 * @return ::LCS_OK               Successful result
 * @return ::LCS_ERR_INACTIVE     Service inactive or not ready
 * @return ::LCS_ERR_INT          Internal Error
 * @return ::LCS_ERR_PARAM        Invalid parameter
 *
 */
qwf_lcs_Results_e qwf_lcs_readSatInfoData(qwf_Context_t *pCtx, qwf_lcs_SatData_t *outSatData,
                                          size_t bufSize);

/**
 * @brief LCS: Send command to write external sensor data to GNSS device
 *
 * Interface to write external sensor data to GNSS device.
 *
 * @param [in]  pCtx    Pointer to Service context.
 * @param [in]  iTOW    Integer time of the week (ms).
 * @param [in]  id      Data provider identifier.
 * @param [in]  num     Number of data elements included.
 * @param [in]  data    Pointer to array of ::qwf_lcs_ExtDRSensData_t data elements.
 *
 * @return ::LCS_OK               Successful result
 * @return ::LCS_ERR_INACTIVE     Service inactive or not ready
 * @return ::LCS_ERR_INT          Internal Error
 * @return ::LCS_ERR_PARAM        Invalid parameter
 *
 */
qwf_lcs_Results_e qwf_lcs_setExtSensData(qwf_Context_t *pCtx, int iTOW, int id, int num,
                                         qwf_lcs_ExtDRSensData_t *data);

/**
 * @brief LCS: Send command to write GNSS time pulse configuration to GNSS device
 *
 * Interface to write or read GNSS time pulse configuration to GNSS device.
 *
 * @param [in]  pCtx    Pointer to Service context.
 * @param [in]  write   Boolean to write configuration (@c true) or read.
 * @param [in]  data    Pointer to ::qwf_lcs_TimePulseConfig_t data elements.
 *
 * @return ::LCS_OK               Successful result
 * @return ::LCS_ERR_INACTIVE     Service inactive or not ready
 * @return ::LCS_ERR_INT          Internal Error
 * @return ::LCS_ERR_PARAM        Invalid parameter
 *
 */
qwf_lcs_Results_e qwf_lcs_setTPConfig(qwf_Context_t *pCtx, bool write, qwf_lcs_TimePulseConfig_t *data);

/**
 * @brief LCS: Send command to set the GNSS device navigation measurement rate
 *
 * Interface to change the GNSS device navigation measurement rate.  The actual reporting rate
 * becomes (measRate * navRate) milli-seconds.
 *
 * @param [in]  pCtx      Pointer to Service context.
 * @param [in]  measRate  time between measurement report sessions in (ms).
 * @param [in]  navRate   number of measurement sessions before a measurement is output.
 * @param [in]  timeRef   ::qwf_lcs_TimeGrid_e satellite system for time reference.
 *
 * @b NOTE: When changing this from a very slow rate to a quicker rate, the first report may be
 *          delayed up to the previous rate.
 * @b NOTE: navRate parameter currently limited to (1) due to issue with message timeout handling.
 *
 * @return ::LCS_OK               Successful result
 * @return ::LCS_ERR_INACTIVE     Service inactive or not ready
 * @return ::LCS_ERR_INT          Internal Error
 * @return ::LCS_ERR_PARAM        Invalid parameter
 *
 */
qwf_lcs_Results_e qwf_lcs_setMeasRate(qwf_Context_t *pCtx, int measRate, int navRate, qwf_lcs_TimeGrid_e timeRef);


/** @}*/ // end group QWF_LCS

__END_DECLS

#endif /* _QWF_LCS_H_INCLUDED */

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/wireless_framework/branches/7.1/trunk/common/lib/qwf_interface/public/qwf/qwf_lcs.h $ $Rev: 2934 $")
#endif

