/*
 * $QNXLicenseC:
 * Copyright 2015-2019, QNX Software Systems. All Rights Reserved.
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
 *
 * @file
 *
 * @brief   This interface provides access to QWF ECall service.
 *
 * @details This file provides the required data types and APIs
 *          to allow the client start an eCall, Update Minimum
 *          Set of Data (MSD) and running an eCall self
 *          test.
 *
 *          Client may accomplish the following
 *          operations via this interface in order to:
 *          - start an eCall that includes establishing an
 *            outgoing emergency voice call and also uploading
 *            the MSD attributes to PSAP (Public Safety Answering
 *            Point) operator, qwf_ecall_StartEcall().
 *          - update MSD attributes during and after an eCall,
 *            qwf_ecall_UpdateMsd().
 *          - run a self test to verify whether the eCall
 *            service is supported, qwf_ecall_EcallSelfTest().
 *          - retrieve MSD uploading status information, see
 *            qwf_ecall_RegisterCallback()
 *
 *          Usage:
 *          - Client application calls qwf_Init() to
 *            initialize the QWF context data used as container
 *            for QWF voice.
 *          - Client application calls
 *            qwf_voice_RegisterCallback() to register a
 *            callback fuction to receive events from the QWF
 *            voice component.
 *          - Client application calls
 *            qwf_ecall_RegisterCallback() to register a
 *            callback fuction to receive MSD (Minimum Set of
 *            Data) uploading events from the QWF ecall
 *            component.
 *          - Client application calls the qwf_ProcessEvents() function to
 *            block and wait for events to be sent to each services callback
 *            which was registered during initialization
 *          - The client application processes the events as required.
 *
 *
 * More information regarding the QWF ECall service may be
 * obtained from the "QWF Architecture and Users Guide".
 *
 * For code examples, refer to @ref QWF_EXAMPLES_ECALL
 *
 * @b NOTE: qwf_Init() must be called once before using QWF services,
 *          and qwf_Cleanup() must be called once after using QWF services.
 */


#ifndef _QWF_ECALL_H_INCLUDED
#define _QWF_ECALL_H_INCLUDED

#include <qwf/qwf_interface.h>

__BEGIN_DECLS

/**
 * @defgroup QWF_ECALL QWF Cellular eCall Service
 * @{
 */
;

/**
 * ECall attributes value range, see the European standard specification CEN 15722:2011.
 */
#define QWF_ECALL_MAX_VIN_SIZE_LEN 20                    ///< Max size of the Vehicle Identification Number
#define QWF_ECALL_MIN_VIN_SIZE_LEN 17                    ///< Min size of the Vehicle Identification Number
#define QWF_ECALL_TIME_STAMP_MIN   0LL                   ///< Time stamp min value in seconds
#define QWF_ECALL_TIME_STAMP_MAX   4294967295LL          ///< Time stamp max value in seconds
#define QWF_ECALL_LATITUDE_MIN    -324000000L            ///< Latitude min value in miliarcseconds
#define QWF_ECALL_LATITUDE_MAX     324000000L            ///< Latitude max value in miliarcseconds
#define QWF_ECALL_LONGITUDE_MIN   -648000000L            ///< Longitude min value in miliarcseconds
#define QWF_ECALL_LONGITUDE_MAX    648000000L            ///< Longitude max value in miliarcseconds
#define QWF_ECALL_DELTA_LATITUDE_LONGITUDE_MIN  -512     ///< Delta latitude/longitude min value in seconds
#define QWF_ECALL_DELTA_LATITUDE_LONGITUDE_MAX   511     ///< Delta latitude/longitude max value in seconds
#define QWF_ECALL_VEHICLE_DIRECTION_MIN   0              ///< Vehicle direction min value
#define QWF_ECALL_VEHICLE_DIRECTION_MAX   179            ///< Vehicle direction max value
#define QWF_ECALL_MAX_ADDL_DATA_LEN       94             ///< Size of the Additional Data
#define QWF_ECALL_MAX_NUM_PASSENGERS      254            ///< Maximum number of passengers
#define QWF_ECALL_UNKNOWN_NUM_PASSENGERS  255            ///< Unknown number of passengers

/**
* Invalid call Id
*/
#define QWF_ECALL_INVALID_CALL_ID 0


/**
 * @brief MSD uploading status
 */
typedef enum
{
    ECALL_MSD_SUCCESS,                ///< MSD buffer successfully encoded and uploaded.
    ECALL_MSD_FAILED,                 ///< MSD buffer was not uploaded to PSAP.
    ECALL_MSD_INVALID_PARAMETERS,     ///< Encoder failed due to invalid parameters.
    ECALL_MSD_UNKNOWN                 ///< Only used for the event initialization
} qwf_ecall_MsdStatus_e;


/**
 * @brief Return result of eCall service containing the possible
 *        eCall service return results.
 */
typedef enum
{
    ECALL_RETURN_SUCCESS,                 ///< The voice service returns successful result
    ECALL_RETURN_GENERAL_FAILURE,         ///< The voice service returns general failure
    ECALL_RETURN_INVALID_CALL,            ///< The voice service can't find the call with the specified call Id
    ECALL_RETURN_NETWORK_UNAVAILABLE,     ///< The network is unavailable
    ECALL_RETURN_NETWORK_REJECTED,        ///< The network has rejected the call
    ECALL_RETURN_INVALID_STATE,           ///< The voice service is in a state that makes this request invalid
    ECALL_RETURN_INVALID_MSD_PARAMETERS,  ///< The eCall MSD (Minimum Set of Data) encoding failed
    ECALL_RETURN_ECALL_NOT_SUPPORTED,     ///< The eCall function is not supported
    ECALL_RETURN_CB_ALREADY_REGISTERED,   ///< The callback function already registered
    ECALL_RETURN_SERVICE_UNAVAILABLE,     ///< The ecall service is not available
    ECALL_RETURN_PARAMETER_ERROR,         ///< NULL parameter
    ECALL_RETURN_NOT_SUPPORTED            ///< Function not supported by CRM and/or modem
} qwf_ecall_ReturnResult_e;


/**
 * @brief eCall type, see the European standard specification
 *        CEN 15722:2011.
 */
typedef enum
{
    ECALL_AUTO_ECALL_TYPE = 1,            ///< automatic
    ECALL_MAN_ECALL_TYPE  = 2,            ///< manual (default type if the attribute is unavailable).
    ECALL_TEST_ECALL_TYPE = 3             ///< test
} qwf_ecall_EcallType_e;



/**
 * @brief Vehicle type, see standard specification CEN
 *        15722:2011
 */
typedef enum
{
    ECALL_UNKNOWN_VEHICLE_TYPE              = 0,
    ECALL_PASSENGER_VEHICLE_CLASS_M1        = 1,
    ECALL_BUSES_AND_COACHES_CLASS_M2        = 2,
    ECALL_BUSES_AND_COACHES_CLASS_M3        = 3,
    ECALL_LIGHT_COMMERCIAL_VEHICLE_CLASS_N1 = 4,
    ECALL_HEAVY_DUTY_VEHICLE_CLASS_N2       = 5,
    ECALL_HEAVY_DUTY_VEHICLE_CLASS_N3       = 6,
    ECALL_MOTORCYCLES_CLASS_L1E             = 7,
    ECALL_MOTORCYCLES_CLASS_L2E             = 8,
    ECALL_MOTORCYCLES_CLASS_L3E             = 9,
    ECALL_MOTORCYCLES_CLASS_L4E             = 10,
    ECALL_MOTORCYCLES_CLASS_L5E             = 11,
    ECALL_MOTORCYCLES_CLASS_L6E             = 12,
    ECALL_MOTORCYCLES_CLASS_L7E             = 13
} qwf_ecall_VehicleType_e;

/**
 * @brief Vehicle energy Storage type, see European standard
 *          specification CEN 15722:2011 & 2015
 */
typedef enum
{
    ECALL_UNKNOWN_STORAGE          = 0x00,
    ECALL_GASOLINE_TANK            = 0x01,
    ECALL_DIESEL_TANK              = 0x02,
    ECALL_COMPRESS_NATURAL_GAS     = 0x04,
    ECALL_LIQUID_PROPANE_GAS       = 0x08,
    ECALL_ELECTRIC_ENERGY_STORAGE  = 0x10,
    ECALL_HYDROGEN_STORAGE         = 0x20,
    ECALL_OTHER_STORAGE            = 0x40,
    ECALL_VALID_STORAGE            = 0x7f       // just a sum of valid maps
} qwf_ecall_VehicleStorageType_e;



/**
 * @brief Minimum Set of Data (MSD)
 *
 * The Minimum Set of Data is used to start an ecall. According
 * to the European standard specification CEN 15722:2011 & 2015
 * the majority of the attributes are required. Due to
 * importance of the emergency call if the client does not
 * provide the required MSD attributes the QWF implementation of
 * eCall will upload the MSD buffer (contains either encoded
 * default values or zeros) and also establish the emergency
 * voice call to PSAP (Public Safety Answering Point).
 *
 */
typedef struct qwf_ecall_Msd_s
{
    /**
     * eCall type
     * @see ::qwf_ecall_EcallType_e
     * Mandatory attribute
     */
    qwf_ecall_EcallType_e  ecalltype;

    /**
     * The position can be trusted.
     * True: position can be trusted.
     * False if the position is not within the limits of +/-150 m
     * with 95% confidence.
     * Mandatory attribute
     */
     bool                  confidence;

    /**
     * Vehicle type, ::qwf_ecall_VehicleType_e
     * Will be set to ECALL_PASSENGER_VEHICLE_CLASS_M1 if unknown to
     * prevent ASN encoder to fail. Note: The European standard
     * EN15722:2015 for eCall does NOT define any default value for
     * the vehicle type.
     * Mandatory attribute
                          */
    qwf_ecall_VehicleType_e vehicle;

     /**
     *  Vehicle Identification Number.
     *  Will be set to "FFFFFFFFFFFFFFFFF" if unknown to prevent ASN
     *  encoder to fail. Note: The European standard EN15722:2015 for
     *  eCall does NOT define any default value for VIN.
     *  Mandatory attribute
     */
     char vin[QWF_ECALL_MAX_VIN_SIZE_LEN + 1];

    /**
     * The type of vehicle energy storage. More than one bit may be
     * set if there is more than one type of energy storage present.
     * See ::qwf_ecall_VehicleStorageType_e
     * Mandatory attribute
     */
    uint16_t     storageType;

    /**
     * The time stamp in sec.
     * Value range: 0 .. 4294967295.
     * Set to 0 if unknown.
     * Mandatory attribute
     */
    uint64_t    timeStamp;

    /**
     * The position latitude is the current location of the vehicle
     * and in milliarcsec.
     * Value range: -324000000 .. 324000000.
     * Set to 0x7FFFFFFF, If invalid or unknown.
     * Mandatory attribute
     */
    int32_t    latitude;

    /**
     * The position longitude is the current location of the vehicle
     * and in milliarcsec.
     * Value range: -648000000 .. 648000000.
     * Set to 0x7FFFFFFF, If invalid or unknown.
     * Mandatory attribute
     */
    int32_t    longitude;

    /**
     * The vehicle direction.
     * Value range: 0 .. 179, (clockwise).
     * Set to 0xFF, If invalid or unknown.
     * Mandatory attribute
     */
    uint16_t   direction;

    /**
     * The location delta with respect to the recent vehicle
     * locatation and is in milliarcsec.
     * Value range: -512 .. 511.
     * Set to 0 if invalid or unknown.
     * Optional attribute
     */
    int16_t     latDeltaN1;

    /**
     * The location delta with respect to the recent vehicle
     * locatation and is in milliarcsec.
     * Value range: -512 .. 511.
     * Set to 0 if invalid or unknown.
     * Optional attribute
     */
    int16_t     longDeltaN1;

    /**
     * The location delta with respect to N1 vehicle locatation and
     * is in milliarcsec.
     * Value range: -512 .. 511.
     * Set to 0 if invalid or unknown.
     * Optional attribute
     */
    int16_t     latDeltaN2;

    /**
     * The location delta with respect to N1 vehicle locatation and
     * is in milliarcsec.
     * Value range: -512 .. 511.
     * Set to 0 if invalid or unknown.
     * Optional attribute
     */
    int16_t     longDeltaN2;

    /**
     * The number of passengers.
     * Value range: 0 .. 254.
     * Set to 0xFF if invalid or unknown.
     * Optional attribute
     */
    uint8_t     passengers;

     /**
     *  Additional Data.
     *  The length of the additional Data string cannoth be greater than
     *  QWF_ECALL_MAX_ADDL_DATA_LEN. This data is optional, set to an empty string,
     *  '\0', to not use. Optional attribute
     */
     char additionalData[QWF_ECALL_MAX_ADDL_DATA_LEN + 1];

} qwf_ecall_Msd_t;


/**
 * @brief Defines the eCall service event code.
 *
 * This enumeration defines the possible eCall service event
 * types.
 */
typedef enum
{
    MSD_STATUS,             ///< MSD uploading status
    ECALL_SUPPORT,          ///< eCall self-test operation verifies whether eCall is supported
    ECALL_SERVICE_INACTIVE, ///< eCall service is inactive
    ECALL_SERVICE_ACTIVE    ///< eCall service is active
} qwf_ecall_EventCode_e;

/**
 * @brief Defines the structure used when passing eCall event
 * from eCall API to the client
 */
typedef struct qwf_ecall_EventData_s
{
    qwf_ecall_EventCode_e   eventCode;  ///< Event name to be processed
    void *  pClientCB;                  ///< Client callback function
    union{
        /**
         * MSD (Minimum Set of Data) uploading status
         */
        qwf_ecall_MsdStatus_e   msdStatus;

        /**
         * During eCall self-test operation the status is triggered at:
         *      - power up
         *      - SIM removal or insertion
         *      - modem reset
         *      - USB connection between host and module
         */
        bool ecallSupport;
    }u;
} qwf_ecall_EventData_t;


/**
 * @brief Client's function to process the generated eCall MSD
 *        (Minimum Set of Data) uploading events.
 *
 * The callback registered by the client to notify when an event
 * has been generated. This function is called if there is an
 * eCall MSD uploading event.
 *
 * @param[in]   pCtx Pointer to QWF service context.
 * @param[in]   pEvent Pointer to eCall MSD event.
 * @param[in]   pClientData Pointer to client's data to be used
 *              with this event handler. This is optional to
 *              allow client to associate an extra data
 *              structure with this context. Ecall service
 *              simply loops this pointer back to the client.
 *              @c NULL is a valid value indicating not used.
 *
 */
typedef void (*qwf_ecall_EventClientCallback)(qwf_Context_t * pCtx, qwf_ecall_EventData_t *pEvent, void *pClientData);



/**
 * @brief Register a callback to receive eCall MSD (Minimum Set
 *        of Data) uploading events.
 *
 * @param[in]  pCtx QWF service context.
 * @param[in]  eventHandler The callback to handle eCall MSD
 *             uploading events. NULL is a valid value and used
 *             when the client decides to de-register the
 *             callback function.
 * @param[in]  pClientData Pointer to client's data to be
 *             used with this event handler this is optional to
 *             allow client to associate an extra data structure
 *             with this context. Ecall service simply loops
 *             this pointer back to the client.
 *             @c NULL is a valid value indicating not used.
 *
 * @return @c ::ECALL_RETURN_SUCCESS
 * @return @c ::ECALL_RETURN_GENERAL_FAILURE
 * @return @c ::ECALL_RETURN_CB_ALREADY_REGISTERED
 * @return @c ::ECALL_RETURN_SERVICE_UNAVAILABLE
 */
qwf_ecall_ReturnResult_e qwf_ecall_RegisterCallback(const qwf_Context_t * const pCtx,
                                                    qwf_ecall_EventClientCallback eventHandler,
                                                    void *pClientData);


/**
 * @brief Request to start an eCall
 *
 * @param [in]  pCtx  Pointer to QWF context
 * @param [in]  pMsd  Pointer to MSD structure
 * @param [out] pCallId  Pointer to call Id returned (only valid
 *              when this API returns ::ECALL_RETURN_SUCCESS)
 *
 * @return @c ::ECALL_RETURN_SUCCESS
 * @return @c ::ECALL_RETURN_GENERAL_FAILURE
 * @return @c ::ECALL_RETURN_INVALID_CALL
 * @return @c ::ECALL_RETURN_NETWORK_UNAVAILABLE
 * @return @c ::ECALL_RETURN_NETWORK_REJECTED
 * @return @c ::ECALL_RETURN_INVALID_STATE
 * @return @c ::ECALL_RETURN_SERVICE_UNAVAILABLE
 */
qwf_ecall_ReturnResult_e qwf_ecall_StartEcall(const qwf_Context_t * const pCtx,
                                              const qwf_ecall_Msd_t * const pMsd,
                                              int * const pCallId);



/**
 * @brief Request to update the Minimum Set of Data.
 *
 * The client may request to send the new MSD:
 * 1. as long as an eCall is ongoing with PSAP.
 * 2. up to 3 minutes after the ongoing eCall was dropped.
 * 3. up to 3 minutes after the ongoing eCall was
 * successfully ended by PSAP or the remote request to
 * end the eCall. The client may decide every x seconds
 * sends an update if there is a new set of data
 * available. The updated MSD will not be uploaded to
 * PSAP until the PSAP operator requests to pull the
 * data.
 *
 *
 * @param [in]  pCtx  Pointer to QWF context
 * @param [in]  pMsd  Pointer to MSD structure
 *
 * @return @c ::ECALL_RETURN_SUCCESS
 * @return @c ::ECALL_RETURN_INVALID_MSD_PARAMETERS
 * @return @c ::ECALL_RETURN_GENERAL_FAILURE
 * @return @c ::ECALL_RETURN_SERVICE_UNAVAILABLE
 */
qwf_ecall_ReturnResult_e qwf_ecall_UpdateMsd(const qwf_Context_t * const pCtx,
                                             const qwf_ecall_Msd_t * const pMsd);



/**
 * @brief Request QWF to check whether eCall service is
 *        supported.
 *
 * @param [in]  pCtx  Pointer to QWF context
 *
 * @return @c ::ECALL_RETURN_SUCCESS
 * @return @c ::ECALL_RETURN_PARAMETER_ERROR
 * @return @c ::ECALL_RETURN_GENERAL_FAILURE
 * @return @c ::ECALL_RETURN_SERVICE_UNAVAILABLE
 */
qwf_ecall_ReturnResult_e qwf_ecall_EcallSelfTest(const qwf_Context_t * const pCtx);

/** @}*/ // end group QWF_ECALL

__END_DECLS

#endif /* _QWF_ECALL_H_INCLUDED */

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/wireless_framework/branches/7.1/trunk/common/lib/qwf_interface/public/qwf/qwf_ecall.h $ $Rev: 2922 $")
#endif

