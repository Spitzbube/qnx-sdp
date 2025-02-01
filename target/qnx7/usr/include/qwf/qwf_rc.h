/*
 * $QNXLicenseC:
 * Copyright 2015-2019 QNX Software Systems. All Rights Reserved.
 *
 * You must obtain a written license from and pay applicable license fees to QNX
 * Software Systems before you may reproduce, modify or distribute this software,
 * or any work that includes all or part of this software.   Free development
 * licenses are available for evaluation and non-commercial purposes.  For more
 * information visit http://licensing.qnx.com or email licensing@qnx.com.
 *
 * This file may contain contributions from others.  Please review this entire
 * file for other proprietary rights or license notices, as well as the QNX
 * Development Suite License Guide at http://licensing.qnx.com/license-guide/
 * for other information.
 * $
 */

/**
 * @file
 *
 * @brief QNX Wireless Framework (QWF) cellular Radio Control (RC) service public interface
 *
 * The QWF RC service provides an interface that allows a client to configure
 * and monitor the capabilities and state of the cellular radio and the cellular
 * network.
 *
 * Control, such as cellular radio power, is provided by the QWF RC service.
 *
 * Monitoring, such as cellular radio power, signal strength, network
 * availability, access technologies and available network services is
 * provided by the QWF RC service.
 *
 * The functionality is separated into a handful of categories:
 * - Event (QWF RC service event - designated by @b qwf_rc_Event* definitions and interfaces)
 * - Cell (cellular serving cell - designated by @b qwf_rc_Cell* definitions and interfaces)
 * - Radio (cellular radio - designated by @b qwf_rc_Radio* definitions and interfaces)
 * - Signal (cellular radio signal - designated by @b qwf_rc_Signal* definitions and interfaces)
 *
 * To use the QWF RC service interface, use the @c -l @c qwf_interface option
 * to @c qcc to link against @c libqwf_interface.
 *
 * To compile against the QWF RC service interface, include @c <qwf/qwf_rc.h>.
 *
 * For all QWF RC service interfaces, the safety is:
 * - Cancellation point - yes
 * - Interrupt handler - no
 * - Signal handler - no
 * - Thread - yes
 *
 * For code examples, refer to @ref QWF_EXAMPLES_RC
 *
 * More information regarding the QWF RC service may be obtained from
 * the "QWF Architecture and Users Guide".
 */
#ifndef __QWF_RC_H_INCLUDED
#define __QWF_RC_H_INCLUDED

#include <qwf/qwf_interface.h> // core QWF public interface
#include <inttypes.h>          // system primitive types
#include <stdbool.h>           // bool

__BEGIN_DECLS

/**
 * @defgroup QWF_RC QWF Cellular Radio Control Service
 * @{
 */


//
// constants, macros
//

/// Invalid Cell ID (specified by 3GPP)
#define QWF_RC_CELL_ID_INVALID ((uint32_t) 0x0fffffff)

/// Invalid Location Area Code (LAC) (specified by 3GPP)
#define QWF_RC_LAC_INVALID ((uint16_t) 0xfffe)

/// Invalid Tracking Area Code (TAC) (specified by 3GPP)
#define QWF_RC_TAC_INVALID ((uint16_t) 0xfffe)

/// Invalid Routing Area Code (RAC) (specified by 3GPP)
#define QWF_RC_RAC_INVALID ((uint8_t) 0xff)

/// Invalid Mobile Country Code (MCC)
#define QWF_RC_MCC_INVALID ((uint16_t) 0x0fff)

/// Invalid Mobile Network Code (MNC)
#define QWF_RC_MNC_INVALID ((uint16_t) 0x0fff)

/// Invalid received signal strength (RSSI) (GSM/UMTS/LTE)
#define QWF_RC_RSSI_INVALID ((int16_t)-256)

/// Invalid or unsupported antennae load
#define QWF_RC_ANTENNAE_LOAD_INVALID ((int16_t) -255)

/// Temperature sensor measurement not available or unsupported
#define QWF_RC_TEMP_UNKNOWN ((int16_t) -255)

/// Maximum string length of any radio device information string (vendor, model, etc.)
#define QWF_RC_DEVICE_INFO_LENGTH (150)

/// Maximum string length of a radio device firmware configuration name
#define QWF_RC_DEVICE_FW_CONFIG_NAME_LENGTH (32)

/// Maximum number of available firmware configurations
#define QWF_RC_MAX_FW_CONFIGS (5)


//
// typedefs
//

/**
 * Forward declaration for cellular network cell object
 */
typedef struct qwf_rc_Cell_s qwf_rc_Cell_s;

/**
 * Opaque pointer to a QWF RC service cellular network serving cell
 *
 * The data are accessed with the @c qwf_rc_CellGet* interfaces.
 */
typedef qwf_rc_Cell_s *qwf_rc_Cell_t;

/**
 * Forward declaration for cellular radio object
 */
typedef struct qwf_rc_Radio_s qwf_rc_Radio_s;

/**
 * Opaque pointer to a QWF RC service radio object
 *
 * The data are accessed with the @c qwf_rc_RadioGet* interfaces.
 */
typedef qwf_rc_Radio_s *qwf_rc_Radio_t;

/**
 * Forward declaration for cellular radio signal object
 */
typedef struct qwf_rc_Signal_s qwf_rc_Signal_s;

/**
 * Opaque pointer to a QWF RC service radio signal object
 *
 * The data are accessed with the @c qwf_rc_SignalGet* interfaces.
 */
typedef qwf_rc_Signal_s *qwf_rc_Signal_t;

/**
 * Opaque pointer to a QWF RC service event object
 *
 * The event is passed as the @c event argument of the
 * function registered with qwf_rc_EventRegisterHandler().
 *
 * An event carries an opaque pointer, @c payload, whose type is
 * identified by the event type, ::qwf_rc_EventType_e, and is one of:
 * - ::qwf_rc_Cell_t
 * - ::qwf_rc_Radio_t
 * - ::qwf_rc_Signal_t
 *
 * @b Note: The event delivered as the @c event argument of a registered event handler
 *          will be destroyed on return from the registered event handler.  This includes
 *          any data pointed to by the @c payload within @c event.
 *
 * @see ::qwf_rc_EventHandler_t
 * @see qwf_rc_EventGetType()
 * @see qwf_rc_EventGetPayload()
 */
typedef struct qwf_rc_Event_s *qwf_rc_Event_t;

/// QWF RC public interface return code
typedef enum {
    QWF_RC_RETURN_OK,          ///< No error
    QWF_RC_RETURN_ERROR,       ///< General error
    QWF_RC_RETURN_INVALID_ARG, ///< Invalid argument(s) provided
    QWF_RC_RETURN_UNSUPPORTED, ///< Unsupported operation
    QWF_RC_RETURN_INACTIVE     ///< QWF RC service is inactive
} qwf_rc_Return_e;

/**
 * Cellular radio device information string used for all device information and firmware version strings
 */
typedef char qwf_rc_DeviceInfoString_t[QWF_RC_DEVICE_INFO_LENGTH + 1];

/**
 * Cellular radio device firmware configuration name string
 */
typedef char qwf_rc_FwConfigNameString_t[QWF_RC_DEVICE_FW_CONFIG_NAME_LENGTH];

/**
 * QWF RC cellular radio power state
 *
 * @b Note: Some values are valid for use as the @c power argument to qwf_rc_RadioSetPower() and
 *          as status values returned from qwf_rc_RadioGetPower(), while other values are valid
 *          only as status values.
 */
typedef enum {
    QWF_RC_POWER_UNKNOWN,     ///< Power unknown or invalid (QWF RC service inactive) (output status only)
    QWF_RC_POWER_ON,          ///< Power on (input command and output status)
    QWF_RC_POWER_OFF,         ///< Power off (input command and output status)
    QWF_RC_POWER_REBOOT,      ///< Power reboot (input command and output status) (software reboot not supported by all modems)
    QWF_RC_POWER_SAVE,        ///< Power save (input command and output status)
    QWF_RC_POWER_SHUTTING_OFF ///< Power shutting off (transient state) (output status only)
} qwf_rc_Power_e;

/// QWF RC cellular radio access technology (RAT) (Currently, only 3GPP access technologies are supported)
typedef enum {
    QWF_RC_RAT_UNKNOWN, ///< Unknown (QWF RC service inactive)
    QWF_RC_RAT_NONE,    ///< None
    QWF_RC_RAT_GSM,     ///< GSM
    QWF_RC_RAT_UMTS,    ///< UMTS
    QWF_RC_RAT_LTE      ///< LTE
} qwf_rc_Rat_e;

/// QWF RC cellular network Circuit Switched (CS) and Packet Switched (PS) services (@b BITMAP)
typedef enum {
    QWF_RC_SERVICES_BITMAP_UNKNOWN    = 0,        ///< Unknown (QWF service inactive or no RAT)
    QWF_RC_SERVICES_BITMAP_NONE       = (1 << 0), ///< No network service
    QWF_RC_SERVICES_BITMAP_EMERGENCY  = (1 << 1), ///< Emergency voice service (CS)
    QWF_RC_SERVICES_BITMAP_VOICE      = (1 << 2), ///< Voice service (CS)
    QWF_RC_SERVICES_BITMAP_DATA_GPRS  = (1 << 3), ///< GPRS data service (PS)
    QWF_RC_SERVICES_BITMAP_DATA_EDGE  = (1 << 4), ///< EDGE data service (PS)
    QWF_RC_SERVICES_BITMAP_DATA_UMTS  = (1 << 5), ///< UMTS data service (PS)
    QWF_RC_SERVICES_BITMAP_DATA_HSPA  = (1 << 6), ///< HSPA data service (PS)
    QWF_RC_SERVICES_BITMAP_DATA_HSPAP = (1 << 7), ///< HSPA+ data service (PS)
    QWF_RC_SERVICES_BITMAP_DATA_LTE   = (1 << 8)  ///< LTE data service (PS)
} qwf_rc_Services_e;

/// QWF RC cellular roaming state
typedef enum {
    QWF_RC_ROAMING_STATE_UNKNOWN,    ///< Unknown roaming state (QWF RC service inactive)
    QWF_RC_ROAMING_STATE_ROAMING,    ///< Roaming
    QWF_RC_ROAMING_STATE_NOT_ROAMING ///< Not roaming
} qwf_rc_RoamingState_e;

/// QWF RC cellular antenna state
typedef enum {
    QWF_RC_ANTENNA_STATE_UNKNOWN,    ///< Unknown antenna state
    QWF_RC_ANTENNA_STATE_GOOD,       ///< Good (connected) antenna state
    QWF_RC_ANTENNA_STATE_OPEN,       ///< Open circuit (undetected) antenna state
    QWF_RC_ANTENNA_STATE_SHORT,      ///< Short circuit (to power or ground) antenna state
    QWF_RC_ANTENNA_STATE_UNSUPPORTED ///< Antenna state not supported
} qwf_rc_AntennaState_e;

/// QWF RC cellular antennae status (state and load) (not all modems support antennae status)
typedef struct {
    int16_t               load;           ///< DC component of antennae load in kOhm
    qwf_rc_AntennaState_e primaryState;   ///< Primary antenna state
    qwf_rc_AntennaState_e secondaryState; ///< Secondary (diversity) antenna state
} qwf_rc_AntennaeStatus_t;

/// QWF RC cellular temperature sensors measurements (not all modems support temperature sensors measurements)
typedef struct {
    int16_t xo;  ///< oscillator (XO) sensor (-40 to 140 Celsius or ::QWF_RC_TEMP_UNKNOWN)
    int16_t rf;  ///< Transceiver (RF) sensor (-40 to 140 Celsius or ::QWF_RC_TEMP_UNKNOWN)
    int16_t pm;  ///< Power Management IC (PMIC) sensor (-40 to 140 Celsius or ::QWF_RC_TEMP_UNKNOWN)
    int16_t bb0; ///< Baseband (BB) sensor 0 (-40 to 140 Celsius or ::QWF_RC_TEMP_UNKNOWN)
    int16_t bb1; ///< Baseband (BB) sensor 1 (-40 to 140 Celsius or ::QWF_RC_TEMP_UNKNOWN)
    int16_t bb2; ///< Baseband (BB) sensor 2 (-40 to 140 Celsius or ::QWF_RC_TEMP_UNKNOWN)
    int16_t bb3; ///< Baseband (BB) sensor 3 (-40 to 140 Celsius or ::QWF_RC_TEMP_UNKNOWN)
    int16_t bb4; ///< Baseband (BB) sensor 4 (-40 to 140 Celsius or ::QWF_RC_TEMP_UNKNOWN)
    int16_t bb5; ///< Baseband (BB) sensor 5 (-40 to 140 Celsius or ::QWF_RC_TEMP_UNKNOWN)
} qwf_rc_Temps_t;

/// QWF RC available firmware configurations list (not all modems support multiple firmware configurations)
typedef struct {
    uint8_t                     num;                            ///< Number of available firmware configurations in @c config[]
    qwf_rc_FwConfigNameString_t configs[QWF_RC_MAX_FW_CONFIGS]; ///< Available firmware configuration names
} qwf_rc_FwConfigList_t;

/**
 * QWF RC service event payload type
 *
 * Specifies the type of the event payload delivered in an event.
 *
 * @see qwf_rc_EventGetType()
 * @see qwf_rc_EventGetPayload()
 */
typedef enum {
    /// Indicates the event payload is a cell, so cast it to a ::qwf_rc_Cell_t
    /// and use the @c qwf_rc_CellGet* interfaces to access its values.
    QWF_RC_EVENT_TYPE_CELL,

    /// Indicates the event payload is a cellular radio, so cast it to a ::qwf_rc_Radio_t
    /// and use the @c qwf_rc_RadioGet* interfaces to access its values.
    QWF_RC_EVENT_TYPE_RADIO,

    /// Indicates the event payload is a cellular radio signal, so cast to a ::qwf_rc_Signal_t
    /// and use the @c qwf_rc_SignalGet* interfaces to access its values.
    QWF_RC_EVENT_TYPE_SIGNAL,

    /// Indicates the event is a QWF RC service state inactive event (no event payload)
    QWF_RC_EVENT_TYPE_STATE_INACTIVE,

    /// Indicates the event is a QWF RC service state is active event (no event payload)
    QWF_RC_EVENT_TYPE_STATE_ACTIVE,

    /// Invalid or unknown event (no event payload)
    QWF_RC_EVENT_TYPE_INVALID
} qwf_rc_EventType_e;

/**
 * QWF RC service event payload changes (@b BITMAP)
 *
 * Specifies one or more changes within an event's payload.
 */
typedef enum {
    /// Unknown event changes (QWF RC service inactive)
    QWF_RC_EVENT_CHANGES_BITMAP_UNKNOWN        = 0,

    /// No event changes
    QWF_RC_EVENT_CHANGES_BITMAP_NONE           = (1 << 0),

    /// Indicates the cellular network serving cell information has changed. The event
    /// payload is a ::qwf_rc_Cell_t, so use @c qwf_rc_CellGet* to get the its values.
    QWF_RC_EVENT_CHANGES_BITMAP_SERVING_CELL   = (1 << 1),

    /// Indicates the cellular network radio access technology (RAT) has changed. The event
    /// payload is a ::qwf_rc_Cell_t, so use qwf_rc_CellGetRat() to get the RAT value.
    QWF_RC_EVENT_CHANGES_BITMAP_RAT            = (1 << 2),

    /// Indicates the available network services has changed. The event payload is
    /// a ::qwf_rc_Cell_t, so use qwf_rc_CellGetServices() to get the services value.
    QWF_RC_EVENT_CHANGES_BITMAP_SERVICES       = (1 << 3),

    /// Indicates the roaming status has changed. The event payload is a ::qwf_rc_Cell_t,
    /// so use qwf_rc_CellGetRoamingStatus() to get the roaming status value.
    QWF_RC_EVENT_CHANGES_BITMAP_ROAMING        = (1 << 4),

    /// Indicates the cellular radio power state has changed. The event payload is a
    /// ::qwf_rc_Radio_t, so use qwf_rc_RadioGetPower() to get the power value.
    QWF_RC_EVENT_CHANGES_BITMAP_POWER          = (1 << 5),

    /// Indicates the radio device information has changed. The event payload
    /// is a ::qwf_rc_Radio_t, so use @c qwf_rc_RadioGetVendor(), qwf_rc_RadioGetModel(),
    /// qwf_rc_RadioGetImei(), qwf_rc_RadioGetFirmwareConfig() and qwf_rc_RadioGetFirmwareConfigList()
    /// to get the device information.
    QWF_RC_EVENT_CHANGES_BITMAP_DEVICE_INFO    = (1 << 6),

    /// Indicates the temperature sensors measurements have changed. The event payload is a ::qwf_rc_Radio_t,
    /// so use @c qwf_rc_RadioGetTemps() to get the temperature sensors measurements values.
    QWF_RC_EVENT_CHANGES_BITMAP_TEMPS          = (1 << 7),

    /// Indicates the received signal strength (RSSI) has changed. The event payload
    /// is a ::qwf_rc_Signal_t, so use @c qwf_rc_SignalGet* to get the signal values.
    QWF_RC_EVENT_CHANGES_BITMAP_SIGNAL         = (1 << 8),

    /// Indicates the antennae status has changed. The event payload is a ::qwf_rc_Signal_t,
    /// so use @c qwf_rc_SignalGetAntennaeStatus() to get the antennae status values.
    QWF_RC_EVENT_CHANGES_BITMAP_ANTENNAE       = (1 << 9),

    /// Indicates the QWF service state has changed to inactive.
    QWF_RC_EVENT_CHANGES_BITMAP_STATE_INACTIVE = (1 << 10),

    /// Indicates the QWF service state has changed to active.
    QWF_RC_EVENT_CHANGES_BITMAP_STATE_ACTIVE   = (1 << 11)

} qwf_rc_EventChanges_e;

/**
 * QWF RC service client event handler prototype
 *
 * @param[in]   pQwfCtx       A QWF service context handle created by qwf_Init()
 * @param[in]   event         Event type and any relevant event payload
 * @param[in]   pClientData   An optional pointer to extra data to be passed back to the client as the
 *                            @c pClientData argument to @c eventHandler() when the handler is executed
 *
 * @see qwf_rc_EventRegisterHandler()
 * @see ::qwf_rc_Event_t
 *
 * @b Safety
 *    - Cancellation point - yes
 *    - Interrupt handler - no
 *    - Signal handler - no
 *    - Thread - It is left to the client to ensure the event handler is thread safe.  The event
 *               handler will be executed within the main QWF core thread that is created with
 *               the call to qwf_Init().
 *               @b Note: @c event will be destroyed by the QWF RC service on return from the event handler!
 */
typedef void (*qwf_rc_EventHandler_t)(const qwf_Context_t  * const pQwfCtx,
                                      const qwf_rc_Event_t         event,
                                            void           * const pClientData);


//
// public interface function declarations
//

//
// QWF RC service event APIs
//

/**
 * Register an event handler with the QWF RC service
 *
 * This function registers an event handler that will be called each time a QWF RC
 * service event occurs.  @c eventHandler must match the ::qwf_rc_EventHandler_t prototype.
 *
 * @b Note: Only one handler may be registered at a time.  Subsequent calls to this function
 *          will fail if a handler already is registered unless qwf_rc_EventRegisterHandler()
 *          is called first with a @c NULL @c eventHandler argument.
 *
 * @param[in]   pQwfCtx        A QWF service context handle created by qwf_Init()
 * @param[in]   eventHandler   An event handler to be executed when a QWF RC service event occurs,
 *                             or @c NULL to deregister any previously registered handler
 * @param[in]   pClientData    An optional pointer to extra data to be passed back to the client,
 *                             unchanged, as the @c pClientData argument to @c eventHandler when
 *                             the handler is executed. The client is responsible for managing
 *                             any memory associated with @c pClientData.  @c pClientData is
 *                             ignored if @c eventHandler is @c NULL.
 *
 * @return ::QWF_RC_RETURN_OK if successful
 * @return ::QWF_RC_RETURN_UNSUPPORTED if a handler already is registered - you first must
 *         deregister as described above (see @c eventHandler parameter description)
 * @return ::QWF_RC_RETURN_INVALID_ARG if a @c NULL pointer or otherwise invalid argument is provided
 * @return ::QWF_RC_RETURN_INACTIVE if the QWF RC service is inactive
 *
 * @see qwf_rc_EventHandler_t
 */
qwf_rc_Return_e qwf_rc_EventRegisterHandler(const qwf_Context_t         * const pQwfCtx,
                                                  qwf_rc_EventHandler_t         eventHandler,
                                                  void                  *       pClientData);

/**
 * Print an event
 *
 * This function prints to the system log the string representation
 * of the input event object @c event.
 *
 * @param[in]   event   An event, delivered as the @c event argument to a
 *                      registered event handler, that you want to print
 *
 * @return ::QWF_RC_RETURN_OK if successful
 * @return ::QWF_RC_RETURN_INVALID_ARG if an invalid argument is provided
 * @return ::QWF_RC_RETURN_ERROR otherwise
 *
 * @see qwf_rc_EventGetPayload() for a representative code example
 */
qwf_rc_Return_e qwf_rc_EventPrint(const qwf_rc_Event_t event);

/**
 * Get the event type
 *
 * This function gets the event type from the @c event.  The event type is used
 * to determine the type of event payload that is delivered in @c event.
 *
 * @param[in]    pQwfCtx      A QWF service context handle created by qwf_Init()
 * @param[in]    event        An event, delivered as the @c event argument to a
 *                            registered event handler, from which you want event type
 * @param[out]   pEventType   A pointer to a ::qwf_rc_EventType_e to store the event type
 *
 * @return ::QWF_RC_RETURN_OK if successful
 * @return ::QWF_RC_RETURN_ERROR if an error occurred
 * @return ::QWF_RC_RETURN_INVALID_ARG if @c pQwfCtx is @c NULL or an otherwise invalid argument is provided
 *
 * @see qwf_rc_EventGetPayload() for a representative code example
 * @see ::qwf_rc_EventType_e
 * @see qwf_rc_EventRegisterHandler()
 */
qwf_rc_Return_e qwf_rc_EventGetType(const qwf_Context_t      * const pQwfCtx,
                                    const qwf_rc_Event_t             event,
                                          qwf_rc_EventType_e * const pEventType);

/**
 * Get the QWF RC service changes bitmap from an event
 *
 * This function gets the event changes bitmap from @c event used to
 * determine which values within the event payload have changed.
 *
 * @param[in]    pQwfCtx         A QWF service context handle created by qwf_Init()
 * @param[in]    event           An event from which you the event changes
 * @param[out]   pEventChanges   A pointer to a ::qwf_rc_EventChanges_e to store the event changes
 *
 * @return ::QWF_RC_RETURN_OK if successful
 * @return ::QWF_RC_RETURN_INVALID_ARG if @c pQwfCtx is @c NULL or an otherwise invalid argument is provided
 *
 * @see qwf_rc_EventGetPayload() for a representative code example
 * @see ::qwf_rc_EventChanges_e
 * @see qwf_rc_EventRegisterHandler()
 */
qwf_rc_Return_e qwf_rc_EventGetChanges(const qwf_Context_t         * const pQwfCtx,
                                       const qwf_rc_Event_t                event,
                                             qwf_rc_EventChanges_e * const pEventChanges);

/**
 * Get the payload from an event
 *
 * Get the event payload object from @c event. @c ppPayload should be cast to the appropriate event type.
 *
 * @param[in]    pQwfCtx     A QWF service context handle created by qwf_Init()
 * @param[in]    event       An event from which you want the payload
 * @param[out]   ppPayload   A double void pointer to store the event payload
 *                           that should be cast based on its event type
 *
 * @return ::QWF_RC_RETURN_OK if successful, and if @c event carries no payload, @c *ppPayload will
 *         be set to @c NULL. (@see ::QWF_RC_EVENT_TYPE_STATE_INACTIVE)
 * @return ::QWF_RC_RETURN_INVALID_ARG if @c pQwfCtx is @c NULL or an otherwise invalid argument is provided
 * @return ::QWF_RC_RETURN_ERROR otherwise
 *
 * @see ::qwf_rc_EventType_e for information regarding the various payloads delivered by an event
 */
qwf_rc_Return_e qwf_rc_EventGetPayload(const qwf_Context_t  * const pQwfCtx,
                                       const qwf_rc_Event_t         event,
                                             void           **      ppPayload);


//
// Cellular network cell APIs
//

/**
 * Create and/or initialize a QWF RC service network cell object
 *
 * This function creates and/or initializes with default values a network cell object.
 *
 * If @c pCell is @c NULL, a new network cell object is allocated, initialized and
 * returned with the current values from the underlying QWF RC service.
 *
 * After initialization, you can use the @c qwf_rc_CellGet*
 * interfaces to get the object's values.
 *
 * @b Note: If you allowed the function to allocate the network cell object for you,
 *          then you should destroy it with qwf_rc_CellDestroy() when you are finished with it.
 *
 * @param[in]    pQwfCtx   A QWF service context handle created by qwf_Init()
 * @param[out]   pCell     A pointer to a network cell object you want to initialize,
 *                         or @c NULL to create and initialize a new object
 *
 * @return ::QWF_RC_RETURN_OK if successful
 * @return ::QWF_RC_RETURN_INACTIVE if QWF RC service is inactive
 * @return ::QWF_RC_RETURN_INVALID_ARG if a @c NULL pointer or otherwise invalid argument is provided
 * @return ::QWF_RC_RETURN_ERROR otherwise
 */
qwf_rc_Return_e qwf_rc_CellInit(const qwf_Context_t * const pQwfCtx, qwf_rc_Cell_t *pCell);

/**
 * Destroy a QWF RC service network cell object
 *
 * This function destroys the network cell object pointed to by @c pCell. After you
 * have destroyed a cell object, @c pCell will be set to @c NULL, and you should not reuse it.
 *
 * @param[in]   pQwfCtx   A QWF service context handle created by qwf_Init()
 * @param[in]   pCell     A pointer to a network cell object that you want to destroy
 *
 * @return ::QWF_RC_RETURN_OK if successful
 * @return ::QWF_RC_RETURN_INVALID_ARG if a @c NULL pointer or otherwise invalid argument is provided
 * @return ::QWF_RC_RETURN_ERROR otherwise
 */
qwf_rc_Return_e qwf_rc_CellDestroy(const qwf_Context_t * const pQwfCtx, qwf_rc_Cell_t *pCell);

/**
 * Print a QWF RC service network cell object
 *
 * This function prints to the system log the string representation
 * of the input network cell object @c cell or from the underlying
 * QWF RC service if @c cell is @c NULL.
 *
 * @param[in]   cell   A network cell object you want to print, or @c NULL to print
 *                     the current cell values from the underlying QWF RC service
 *
 * @return ::QWF_RC_RETURN_OK if successful
 * @return ::QWF_RC_RETURN_INACTIVE if @c cell is @c NULL and the QWF RC service is inactive
 * @return ::QWF_RC_RETURN_ERROR otherwise
 */
qwf_rc_Return_e qwf_rc_CellPrint(const qwf_rc_Cell_s * const cell);

/**
 * Get the QWF RC service cellular network services
 *
 * This function gets the cellular network services from @c cell
 * or from the underlying QWF RC service if @c cell is @c NULL.
 *
 * @param[in]    pQwfCtx     A QWF service context handle created by qwf_Init()
 * @param[in]    cell        A cell object from which you want
 *                           the cellular network services value, or @c NULL to retrieve
 *                           the current value from the underlying QWF RC service
 * @param[out]   pServices   A pointer to a ::qwf_rc_Services_e to store the cellular
 *                           network services value
 *
 * @return ::QWF_RC_RETURN_OK if successful
 * @return ::QWF_RC_RETURN_INVALID_ARG if @c pQwfCtx is @c NULL or an otherwise invalid argument is provided
 * @return ::QWF_RC_RETURN_INACTIVE if @c cell is @c NULL and the QWF RC service is inactive
 * @return ::QWF_RC_RETURN_ERROR otherwise
 */
qwf_rc_Return_e qwf_rc_CellGetServices(const qwf_Context_t     * const pQwfCtx,
                                             qwf_rc_Cell_t             cell,
                                             qwf_rc_Services_e * const pServices);

/**
 * Get the QWF RC service cellular radio access technology
 *
 * This function gets the cellular radio access technology from the input @c cell or
 * from the underlying QWF RC service if @c cell is @c NULL.
 *
 * @param[in]    pQwfCtx   A QWF service context handle created by qwf_Init()
 * @param[in]    cell      A QWF RC service cell object from which you want
 *                         the cellular network RAT value, or @c NULL to retrieve the
 *                         current value from the underlying QWF RC service
 * @param[out]   pRat      A pointer to a ::qwf_rc_Rat_e to store the cellular radio
 *                         access technology value
 *
 * @return ::QWF_RC_RETURN_OK if successful
 * @return ::QWF_RC_RETURN_INVALID_ARG if @c pQwfCtx is @c NULL or an otherwise invalid argument is provided
 * @return ::QWF_RC_RETURN_INACTIVE if @c cell is @c NULL and the QWF RC service is inactive
 * @return ::QWF_RC_RETURN_ERROR otherwise
 */
qwf_rc_Return_e qwf_rc_CellGetRat(const qwf_Context_t  * const pQwfCtx,
                                  const qwf_rc_Cell_t          cell,
                                        qwf_rc_Rat_e   * const pRat);

/**
 * Get the QWF RC service cellular network serving cell MCC/MNC
 *
 * This function gets the cellular network serving cell Mobile Country Code (MCC)
 * and Mobile Network Code (MNC) from the input @c cell or from the underlying
 * QWF RC service if @c cell is @c NULL.
 *
 * @b Note: Two-digit MNCs are appended with an 'f'.  For example, an MNC of
 *          01 will be reported as 01f.
 *
 * @param[in]    pQwfCtx   A QWF service context handle created by qwf_Init()
 * @param[in]    cell      A QWF RC service cell object from which you want
 *                         the MCC/MNC values, or @c NULL to retrieve the current values
 *                         from the underlying QWF RC service
 * @param[out]   pMcc      A pointer to a @c uint16_t to store the MCC value
 * @param[out]   pMnc      A pointer to a @c uint16_t to store the MNC value
 *
 * @return ::QWF_RC_RETURN_OK if successful
 * @return ::QWF_RC_RETURN_INVALID_ARG if @c pQwfCtx is @c NULL or an otherwise invalid argument is provided
 * @return ::QWF_RC_RETURN_INACTIVE if @c cell is @c NULL and the QWF RC service is inactive
 * @return ::QWF_RC_RETURN_ERROR otherwise
 */
qwf_rc_Return_e qwf_rc_CellGetMccMnc(const qwf_Context_t * const pQwfCtx,
                                     const qwf_rc_Cell_t         cell,
                                     uint16_t            * const pMcc,
                                     uint16_t            * const pMnc);

/**
 * Get the QWF RC service cellular network serving cell information
 *
 * This function gets the cellular network serving cell's cell ID, Location Area
 * Code (LAC), Tracking Area Code (TAC) and Routing Area Code (RAC) from the
 * input @c cell or from the underlying QWF RC service if @c cell is @c NULL.
 *
 * @param[in]    pQwfCtx   A QWF service context handle created by qwf_Init()
 * @param[in]    cell      A QWF RC service cell object from which you want
 *                         the MCC/MNC values, or @c NULL to retrieve the current values
 *                         from the underlying QWF RC service
 * @param[out]   pCellId   A pointer to a @c uint32_t to store the cell ID value
 * @param[out]   pLac      A pointer to a @c uint16_t to store the LAC value
 * @param[out]   pTac      A pointer to a @c uint16_t to store the TAC value
 * @param[out]   pRac      A pointer to a @c uint8_t to store the RAC value
 *
 * @return ::QWF_RC_RETURN_OK if successful
 * @return ::QWF_RC_RETURN_INVALID_ARG if @c pQwfCtx is @c NULL or an otherwise invalid argument is provided
 * @return ::QWF_RC_RETURN_INACTIVE if @c cell is @c NULL and the QWF RC service is inactive
 * @return ::QWF_RC_RETURN_ERROR otherwise
 */
qwf_rc_Return_e qwf_rc_CellGetInfo(const qwf_Context_t * const pQwfCtx,
                                   const qwf_rc_Cell_t         cell,
                                   uint32_t            * const pCellId,
                                   uint16_t            * const pLac,
                                   uint16_t            * const pTac,
                                   uint8_t             * const pRac);

/**
 * Get the QWF RC service cellular network roaming state
 *
 * This function gets the cellular network roaming state from the input @c cell or from
 * the underlying QWF RC service if @c cell is @c NULL.
 *
 * @param[in]    pQwfCtx         A QWF service context handle created by qwf_Init()
 * @param[in]    cell            A QWF RC service cell object from which you want
 *                               the roaming state, or @c NULL to retrieve the current value(s)
 *                               from the underlying QWF RC service
 * @param[out]   pRoamingState   A pointer to a ::qwf_rc_RoamingState_e to store the roaming state
 *
 * @return ::QWF_RC_RETURN_OK if successful
 * @return ::QWF_RC_RETURN_INVALID_ARG if @c pQwfCtx is @c NULL or an otherwise invalid argument is provided
 * @return ::QWF_RC_RETURN_INACTIVE if @c cell is @c NULL and the QWF RC service is inactive
 * @return ::QWF_RC_RETURN_ERROR otherwise
 */
qwf_rc_Return_e qwf_rc_CellGetRoamingState(const qwf_Context_t          * const pQwfCtx,
                                            const qwf_rc_Cell_t                  cell,
                                                  qwf_rc_RoamingState_e * const pRoamingState);


//
// QWF RC service radio object APIs
//

/**
 * Create and/or initialize a QWF RC service radio object
 *
 * This function creates and/or initializes with default values a radio object.
 *
 * If @c pRadio is @c NULL, a new radio object is allocated, initialized and
 * returned with the current values from the underlying QWF RC service.
 *
 * After initialization, you can use the @c qwf_rc_RadioGet*
 * interfaces to get the object's values.
 *
 * @b Note: If you allowed the function to allocate the radio object for you,
 *          then you should destroy it with qwf_rc_RadioDestroy() when you are finished with it.
 *
 * @param[in]    pQwfCtx   A QWF service context handle created by qwf_Init()
 * @param[out]   pRadio    A pointer to a radio object you want to initialize,
 *                         or @c NULL to create and initialize a new object
 *
 * @return ::QWF_RC_RETURN_OK if successful
 * @return ::QWF_RC_RETURN_INACTIVE if QWF RC service is inactive
 * @return ::QWF_RC_RETURN_INVALID_ARG if a @c NULL pointer or otherwise invalid argument is provided
 * @return ::QWF_RC_RETURN_ERROR otherwise
 */
qwf_rc_Return_e qwf_rc_RadioInit(const qwf_Context_t * const pQwfCtx, qwf_rc_Radio_t *pRadio);

/**
 * Destroy a QWF RC service cellular radio object
 *
 * This function destroys the radio object pointed to by @c pRadio. After you have destroyed
 * a radio object, @c pRadio will be set to @c NULL, and you should not reuse it.
 *
 * @param[in]   pQwfCtx   A QWF service context handle created by qwf_Init()
 * @param[in]   pRadio    A pointer to a cellular radio object that you want to destroy
 *
 * @return ::QWF_RC_RETURN_OK if successful
 * @return ::QWF_RC_RETURN_INVALID_ARG if a @c NULL pointer or otherwise invalid argument is provided
 * @return ::QWF_RC_RETURN_ERROR otherwise
 */
qwf_rc_Return_e qwf_rc_RadioDestroy(const qwf_Context_t * const pQwfCtx, qwf_rc_Radio_t *pRadio);

/**
 * Print a QWF RC service cellular radio object
 *
 * This function prints to the system log the string representation
 * of the input cellular radio object @c radio or from the underlying
 * QWF RC service if @c radio is @c NULL.
 *
 * @param[in]   radio   A cellular radio object that you want to print, or @c NULL to
 *                      print the current radio values from the underlying QWF service
 *
 * @return ::QWF_RC_RETURN_OK if successful
 * @return ::QWF_RC_RETURN_INACTIVE if @c radio is @c NULL and the QWF RC service is inactive
 * @return ::QWF_RC_RETURN_ERROR otherwise
 */
qwf_rc_Return_e qwf_rc_RadioPrint(const qwf_rc_Radio_s * const radio);

/**
 * Set the QWF RC service cellular radio power
 *
 * This function sets the cellular radio power value
 *
 * @param[in]   pQwfCtx   A QWF service context handle created by qwf_Init()
 * @param[in]   power         The cellular radio power to set
 *
 * @return ::QWF_RC_RETURN_OK if successful
 * @return ::QWF_RC_RETURN_INACTIVE if the QWF RC service is inactive
 * @return ::QWF_RC_RETURN_INVALID_ARG if a @c NULL pointer or otherwise invalid argument is provided
 * @return ::QWF_RC_RETURN_ERROR otherwise
 */
qwf_rc_Return_e qwf_rc_RadioSetPower(const qwf_Context_t  * const pQwfCtx,
                                     const qwf_rc_Power_e         power);

/**
 * Get the QWF RC service cellular radio power value
 *
 * This function gets the cellular radio power value from the input @c radio or from
 * the underlying QWF RC service if @c radio is @c NULL.
 *
 * @param[in]    pQwfCtx   A QWF service context handle created by qwf_Init()
 * @param[in]    radio     A cellular radio object from which you want the power value, or
 *                         @c NULL to retrieve the current value from the underlying QWF RC service
 * @param[out]   pPower    A pointer to a ::qwf_rc_Power_e to store the cellular radio power value
 *
 * @return ::QWF_RC_RETURN_OK if successful
 * @return ::QWF_RC_RETURN_INVALID_ARG if @c pQwfCtx is @c NULL or an otherwise invalid argument is provided
 * @return ::QWF_RC_RETURN_INACTIVE if @c radio is @c NULL and the QWF RC service is inactive
 * @return ::QWF_RC_RETURN_ERROR otherwise
 *
 * @see qwf_rc_RadioSetPower()
 */
qwf_rc_Return_e qwf_rc_RadioGetPower(const qwf_Context_t  * const pQwfCtx,
                                     const qwf_rc_Radio_t         radio,
                                           qwf_rc_Power_e * const pPower);

/**
 * Get the QWF RC service cellular radio temperature sensors measurements
 *
 * This function gets the cellular radio temperature sensors measurements from the input @c radio or from
 * the underlying QWF RC service if @c radio is @c NULL.
 *
 * @param[in]    pQwfCtx   A QWF service context handle created by qwf_Init()
 * @param[in]    radio     A cellular radio object from which you want the temperature sensors measurements,
 *                         or @c NULL to retrieve the current value from the underlying QWF RC service
 * @param[out]   pTemps    A pointer to a ::qwf_rc_Temps_t to store the cellular radio temperature sensors measurements
 *
 * @return ::QWF_RC_RETURN_OK if successful
 * @return ::QWF_RC_RETURN_INVALID_ARG if @c pQwfCtx is @c NULL or an otherwise invalid argument is provided
 * @return ::QWF_RC_RETURN_INACTIVE if @c radio is @c NULL and the QWF RC service is inactive
 * @return ::QWF_RC_RETURN_ERROR otherwise
 */
qwf_rc_Return_e qwf_rc_RadioGetTemps(const qwf_Context_t  * const pQwfCtx,
                                     const qwf_rc_Radio_t         radio,
                                           qwf_rc_Temps_t * const pTemps);

/**
 * Get the QWF RC service cellular radio device vendor
 *
 * This function gets the cellular radio device vendor string from the input @c radio or from
 * the underlying QWF RC service if @c radio is @c NULL.
 *
 * @param[in]    pQwfCtx   A QWF service context handle created by qwf_Init()
 * @param[in]    radio     A cellular radio object from which you want the device vendor, or
 *                         @c NULL to retrieve the current information from the underlying QWF RC service
 * @param[out]   vendor    A ::qwf_rc_DeviceInfoString_t to store the device vendor
 *
 * @return ::QWF_RC_RETURN_OK if successful
 * @return ::QWF_RC_RETURN_INVALID_ARG if @c pQwfCtx is @c NULL or an otherwise invalid argument is provided
 * @return ::QWF_RC_RETURN_INACTIVE if @c radio is @c NULL and the QWF RC service is inactive
 * @return ::QWF_RC_RETURN_ERROR otherwise
 */
qwf_rc_Return_e qwf_rc_RadioGetVendor(const qwf_Context_t             * const pQwfCtx,
                                      const qwf_rc_Radio_t                    radio,
                                            qwf_rc_DeviceInfoString_t         vendor);

/**
 * Get the QWF RC service cellular radio device model
 *
 * This function gets the cellular radio device model string from the input @c radio or from
 * the underlying QWF RC service if @c radio is @c NULL.
 *
 * @param[in]    pQwfCtx   A QWF service context handle created by qwf_Init()
 * @param[in]    radio     A cellular radio object from which you want the device model, or
 *                         @c NULL to retrieve the current information from the underlying QWF RC service
 * @param[out]   model     A ::qwf_rc_DeviceInfoString_t to store the device model
 *
 * @return ::QWF_RC_RETURN_OK if successful
 * @return ::QWF_RC_RETURN_INVALID_ARG if @c pQwfCtx is @c NULL or an otherwise invalid argument is provided
 * @return ::QWF_RC_RETURN_INACTIVE if @c radio is @c NULL and the QWF RC service is inactive
 * @return ::QWF_RC_RETURN_ERROR otherwise
 */
qwf_rc_Return_e qwf_rc_RadioGetModel(const qwf_Context_t             * const pQwfCtx,
                                     const qwf_rc_Radio_t                    radio,
                                           qwf_rc_DeviceInfoString_t         model);

/**
 * Get the QWF RC service cellular radio device International Mobile Station Equipment Identity (IMEI)
 * and IMEI Software Version (IMEISV)
 *
 * This function gets the cellular radio device IMEI and IMEISV strings from the input @c radio or from
 * the underlying QWF RC service if @c radio is @c NULL.
 *
 * @param[in]    pQwfCtx   A QWF service context handle created by qwf_Init()
 * @param[in]    radio     A cellular radio object from which you want the device IMEI, or
 *                         @c NULL to retrieve the current information from the underlying QWF RC service
 * @param[out]   imei      A ::qwf_rc_DeviceInfoString_t to store the device IMEI
 * @param[out]   imeisv    A ::qwf_rc_DeviceInfoString_t to store the device IMEISV
 *
 * @return ::QWF_RC_RETURN_OK if successful
 * @return ::QWF_RC_RETURN_INVALID_ARG if @c pQwfCtx is @c NULL or an otherwise invalid argument is provided
 * @return ::QWF_RC_RETURN_INACTIVE if @c radio is @c NULL and the QWF RC service is inactive
 * @return ::QWF_RC_RETURN_ERROR otherwise
 *
 * @b Note: Not all modem's support IMEISV
 */
qwf_rc_Return_e qwf_rc_RadioGetImei(const qwf_Context_t             * const pQwfCtx,
                                    const qwf_rc_Radio_s            * const radio,
                                          qwf_rc_DeviceInfoString_t         imei,
                                          qwf_rc_DeviceInfoString_t         imeisv);

/**
 * Get the QWF RC service cellular radio device firmware version string
 *
 * This function gets the cellular radio device firmware version string from the input @c radio
 * or from the underlying QWF RC service if @c radio is @c NULL.  The output @c is the firmware
 * version string that currently is enabled in the device.
 *
 * @b Note: The firmware version is not the same as the firmware configuration.  The firmware version often
 *          includes multiple version strings that, for example, correspond to the actual firmware versions
 *          of application and communication cores in the modem device.
 *
 * @param[in]    pQwfCtx    A QWF service context handle created by qwf_Init()
 * @param[in]    radio      A cellular radio object from which you want the device firmware version, or
 *                          @c NULL to retrieve the current information from the underlying QWF RC service
 * @param[out]   fwVersion  A ::qwf_rc_DeviceInfoString_t to store the firmware version string of the currently
 *                          enabled firmware
 *
 * @return ::QWF_RC_RETURN_OK if successful
 * @return ::QWF_RC_RETURN_INVALID_ARG if @c pQwfCtx is @c NULL or an otherwise invalid argument is provided
 * @return ::QWF_RC_RETURN_INACTIVE if @c radio is @c NULL and the QWF RC service is inactive
 * @return ::QWF_RC_RETURN_ERROR otherwise
 */
qwf_rc_Return_e qwf_rc_RadioGetFirmwareVersion(const qwf_Context_t             * const pQwfCtx,
                                               const qwf_rc_Radio_t                    radio,
                                                     qwf_rc_DeviceInfoString_t         fwVersion);

/**
 * Get the QWF RC service cellular radio device firmware configuration enabled in the device
 *
 * The output index is the index into the ::qwf_rc_FwConfigList_t.configs[], returned from a call to
 * qwf_rc_RadioGetFirmwareList(), that corresponds to the firmware configuration currently enabled in
 * the device.
 *
 * @b Note: Multiple firmware configurations is not supported by all cellular modem modules.
 *
 * @param[in]    pQwfCtx    A QWF service context handle created by qwf_Init()
 * @param[in]    radio      A cellular radio object from which you want the device firmware configuration, or
 *                          @c NULL to retrieve the current information from the underlying QWF RC service
 * @param[out]   pConfig    A pointer to store the currently enabled firmware configuration
 *
 * @return ::QWF_RC_RETURN_OK if successful
 * @return ::QWF_RC_RETURN_INVALID_ARG if either @c pQwfCtx or @c pConfig is @c NULL
 * @return ::QWF_RC_RETURN_INACTIVE if @c radio is @c NULL and the QWF RC service is inactive
 * @return ::QWF_RC_RETURN_UNSUPPORTED if multiple firmware configurations is not supported
 * @return ::QWF_RC_RETURN_ERROR otherwise
 */
qwf_rc_Return_e qwf_rc_RadioGetFirmwareConfig(const qwf_Context_t * const pQwfCtx,
                                              const qwf_rc_Radio_t        radio,
                                                    uint8_t       * const pConfig);

/**
 * Set the QWF RC service cellular radio device firmware configuration enabled in the device
 *
 * This function sets the cellular radio device firmware configuration identified by @c index in the device.
 * The input index is the index into the ::qwf_rc_FwConfigList_t.configs[], returned from a call to
 * qwf_rc_RadioGetFirmwareList(), that corresponds to the firmware configuration to be enabled in the device.
 *
 * @b Example: qwf_rc_RadioGetFirmwareList() returns a ::qwf_rc_FwConfigList_t with
 *             ::qwf_rc_FwConfigList_t.num of 3. Those three firmware configurations
 *             have indexes 0, 1 and 2. Thus, to set the firmware configuration to the
 *             second configuration in the list, ::qwf_rc_FwConfigList_t.configs[1], the
 *             user would call @c qwf_rc_RadioSetFirmwareConfig(pQwfCtx, 1).
 *
 * @b Note: The cellular radio device may reboot in order to complete any firmware switch.
 *
 * @b Note: Multiple firmware configurations is not supported by all cellular modem modules.
 *
 * @param[in]    pQwfCtx    A QWF service context handle created by qwf_Init()
 * @param[in]    config     The firmware configuration to be set. This is the index into the firmware
 *                          configurations array, ::qwf_rc_FwConfigList_t.configs[], returned from a call
 *                          to qwf_rc_RadioGetFirmwareConfigList().
 *
 * @return ::QWF_RC_RETURN_OK if successful
 * @return ::QWF_RC_RETURN_INVALID_ARG if @c pQwfCtx is @c NULL or an otherwise invalid argument is provided
 * @return ::QWF_RC_RETURN_INACTIVE if the QWF RC service is inactive
 * @return ::QWF_RC_RETURN_UNSUPPORTED if multiple firmware configurations is not supported
 * @return ::QWF_RC_RETURN_ERROR otherwise
 */
qwf_rc_Return_e qwf_rc_RadioSetFirmwareConfig(const qwf_Context_t * const pQwfCtx,
                                                    uint8_t               config);

/**
 * Get the QWF RC service cellular radio firmware list of firmware configurations that may be enabled
 * in the device.
 *
 * This function gets the list of available cellular radio firmware configurations from @c radio.
 *
 * @b Note: Multiple firmware configurations is not supported by all cellular modem modules.
 *
 * @param[in]    pQwfCtx    A QWF service context handle created by qwf_Init()
 * @param[in]    radio      A cellular radio object from which you want the list of available firmware
 *                          configurations, or @c NULL to retrieve the current values from the
 *                          underlying QWF RC service
 * @param[out]   pConfigs   A pointer to store a ::qwf_rc_FwConfigList_t for the available
 *                          cellular radio firmware configurations that can be selected
 *
 * @return ::QWF_RC_RETURN_OK if successful
 * @return ::QWF_RC_RETURN_INVALID_ARG if @c pQwfCtx is @c NULL or an otherwise invalid argument is provided
 * @return ::QWF_RC_RETURN_INACTIVE if @c radio is @c NULL and the QWF RC service is inactive
 * @return ::QWF_RC_RETURN_UNSUPPORTED if multiple firmware configurations is not supported
 * @return ::QWF_RC_RETURN_ERROR otherwise
 */
qwf_rc_Return_e qwf_rc_RadioGetFirmwareConfigList(const qwf_Context_t         * const pQwfCtx,
                                                  const qwf_rc_Radio_t                radio,
                                                        qwf_rc_FwConfigList_t * const pConfigs);



//
// QWF RC service radio signal object APIs
//

/**
 * Create and/or initialize a QWF RC service radio signal object
 *
 * This function creates and/or initializes with default values a radio signal object.
 *
 * If @c pSignal is @c NULL, a new radio signal object is allocated, initialized and
 * returned with the current values from the underlying QWF RC service.
 *
 * After initialization, you can use the @c qwf_rc_SignalGet*
 * interfaces to get the object's values.
 *
 * @b Note: If you allowed the function to allocate the radio signal object for you,
 *          then you should destroy it with qwf_rc_SignalDestroy() when you are finished with it.
 *
 * @param[in]    pQwfCtx   A QWF service context handle created by qwf_Init()
 * @param[out]   pSignal   A pointer to a radio signal object you want  to initialize,
 *                         or @c NULL to create and initialize a new object
 *
 * @return ::QWF_RC_RETURN_OK if successful
 * @return ::QWF_RC_RETURN_INACTIVE if QWF RC service is inactive
 * @return ::QWF_RC_RETURN_INVALID_ARG if a @c NULL pointer or otherwise invalid argument is provided
 * @return ::QWF_RC_RETURN_ERROR otherwise
 */
qwf_rc_Return_e qwf_rc_SignalInit(const qwf_Context_t * const pQwfCtx, qwf_rc_Signal_t *pSignal);

/**
 * Destroy a QWF RC service radio signal object
 *
 * This function destroys the radio signal object pointed to by @c pSignal. After you have
 * destroyed a signal object, @c pSignal will be set to @c NULL, and you should not reuse it.
 *
 * @param[in]   pQwfCtx   A QWF service context handle created by qwf_Init()
 * @param[in]   pSignal   A pointer to a radio signal object that you want to destroy
 *
 * @return ::QWF_RC_RETURN_OK if successful
 * @return ::QWF_RC_RETURN_INVALID_ARG if a @c NULL pointer or otherwise invalid argument is provided
 * @return ::QWF_RC_RETURN_ERROR otherwise
 */
qwf_rc_Return_e qwf_rc_SignalDestroy(const qwf_Context_t * const pQwfCtx, qwf_rc_Signal_t *pSignal);

/**
 * Print a QWF RC service cellular radio signal object
 *
 * This function prints to the system log the string representation
 * of the input cellular radio signal object @c signal or from the underlying
 * QWF RC service if @c signal is @c NULL.
 *
 * @param[in]   signal   A cellular radio signal object that you want to print, or @c NULL
 *                       to print the current radio values from the underlying QWF service
 *
 * @return ::QWF_RC_RETURN_OK if successful
 * @return ::QWF_RC_RETURN_INACTIVE if @c signal is @c NULL and the QWF RC
 *         service is inactive
 * @return ::QWF_RC_RETURN_ERROR otherwise
 */
qwf_rc_Return_e qwf_rc_SignalPrint(const qwf_rc_Signal_s * const signal);

/**
 * Get the current QWF RC service cellular radio received signal strength indication (RSSI)
 *
 * This function gets the cellular radio signal RSSI from @c signal. Generally, the range
 * will be -113 dBm to -51 dBm, with ::QWF_RC_RSSI_INVALID used to indicate no received signal.
 *
 * @param[in]    pQwfCtx   A QWF service context handle created by qwf_Init()
 * @param[in]    signal    A cellular radio signal object from which you want the RSSI  value,
 *                         or @c NULL to retrieve the current value from the underlying QWF RC service
 * @param[out]   pRssi     A pointer to a @c int16_t to store the cellular radio RSSI value
 *
 * @return ::QWF_RC_RETURN_OK if successful
 * @return ::QWF_RC_RETURN_INVALID_ARG if @c pQwfCtx is @c NULL or an otherwise invalid argument is provided
 * @return ::QWF_RC_RETURN_INACTIVE if @c signal is @c NULL and the QWF RC service is inactive
 * @return ::QWF_RC_RETURN_ERROR otherwise
 */
qwf_rc_Return_e qwf_rc_SignalGetRssi(const qwf_Context_t   * const pQwfCtx,
                                     const qwf_rc_Signal_t         signal,
                                           int16_t         * const pRssi);

/**
 * Get the current QWF RC service cellular radio received signal bars information
 *
 * This function gets the cellular radio signal bars information from @c signal.
 *
 * @param[in]    pQwfCtx    A QWF service context handle created by qwf_Init()
 * @param[in]    signal     A cellular radio signal object from which you want the signal bars values,
 *                          or @c NULL to retrieve the current values from the underlying QWF RC service
 * @param[out]   pBars      A pointer to a @c uint8_t to store the cellular radio signal bars value
 * @param[out]   pMaxBars   A pointer to a @c uint8_t to store the cellular radio signal maximum bars value
 *
 * @return ::QWF_RC_RETURN_OK if successful
 * @return ::QWF_RC_RETURN_INVALID_ARG if @c pQwfCtx is @c NULL or an otherwise invalid argument is provided
 * @return ::QWF_RC_RETURN_INACTIVE if @c signal is @c NULL and the QWF RC service is inactive
 * @return ::QWF_RC_RETURN_ERROR otherwise
 */
qwf_rc_Return_e qwf_rc_SignalGetBars(const qwf_Context_t   * const pQwfCtx,
                                     const qwf_rc_Signal_t         signal,
                                           uint8_t         * const pBars,
                                           uint8_t         * const pMaxBars);

/**
 * Get the current QWF RC service cellular radio antennae status
 *
 * This function gets the cellular radio antennae status information from @c signal.  The value returned
 * with @c pAntennaeStatus->load represents the DC component of the cellular antennae load in kOhm.
 *
 * @param[in]    pQwfCtx    A QWF service context handle created by qwf_Init()
 * @param[in]    signal     A cellular radio signal object from which you want the antennae status,
 *                          or @c NULL to retrieve the current values from the underlying QWF RC service
 * @param[out]   pAntennae  A pointer to a ::qwf_rc_AntennaeStatus_t to store the cellular radio signal
 *                          antennae status. If antennae load is unsupported or otherwise invalid,
 *                          ::QWF_RC_ANTENNAE_LOAD_INVALID will be returned for @c pAntennae->load.
 *
 * @return ::QWF_RC_RETURN_OK if successful
 * @return ::QWF_RC_RETURN_INVALID_ARG if @c pQwfCtx is @c NULL or an otherwise invalid argument is provided
 * @return ::QWF_RC_RETURN_INACTIVE if @c signal is @c NULL and the QWF RC service is inactive
 * @return ::QWF_RC_RETURN_ERROR otherwise
 *
 * @b Note: The antennae status functionality is not supported by all cellular modem modules. For those
 *          that do support cellular antennae status, antennae detection circuitry typically is required
 *          as part of both the application/host hardware and each antenna assembly for which antenna
 *          status is desired.
 */
qwf_rc_Return_e qwf_rc_SignalGetAntennaeStatus(const qwf_Context_t           * const pQwfCtx,
                                               const qwf_rc_Signal_t                 signal,
                                                     qwf_rc_AntennaeStatus_t * const pAntennae);

/** @}*/ // end group QWF_RC

__END_DECLS

#endif /* __QWF_RC_H_INCLUDED */

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/wireless_framework/branches/7.1/trunk/common/lib/qwf_interface/public/qwf/qwf_rc.h $ $Rev: 3002 $")
#endif

