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
 * @file
 *
 * @brief    This interface provides access to QWF Voice Service.
 *
 *          This interface gives the client the ability to make/receive calls and retrieve the call information.
 *
 *          Clients may do the following operations via this interface.
 *          - start/receive a voice call
 *          - start emergency call
 *          - answer/reject/end the call
 *          - mute/unmute the call
 *          - start a single DTMF tone
 *          - send a DTMF tone string
 *          - stop DTMF tones
 *          - retrieve call information related to current call
 *          - retrieve status information related to current call
 *
 *          Usage:
 *          - Client application calls qwf_Init() to initialize the QWF
 *            context data used as container for QWF and service data (like
 *            voice)
 *          - Client application calls qwf_voice_RegisterCallback() to
 *            register a callback fuction used to process events from the QWF
 *            voice component. @b Note: similar callbacks will be registered for each
 *            service the application is interested in.
 *          - Client application calls the qwf_ProcessEvents() function to
 *            block and wait for events to be sent to each services callback
 *            which was registered during initialization
 *          - The client application processes the events as required.
 *
 * For code examples, refer to @ref QWF_EXAMPLES_VOICE
 *
 * More information regarding the QWF Voice Service may be obtained from
 * the "QWF Architecture and Users Guide".
 *
 * @b NOTE: qwf_Init() must be called once before using QWF services,
 *          qwf_ProcessEvents must be called once to block and
 *          wait for events to be sent to each QWF service including voice service,
 *          and qwf_Cleanup() must be called once after using QWF services.
 */

#ifndef _QWF_VOICE_H_INCLUDED
#define _QWF_VOICE_H_INCLUDED

#include <qwf/qwf_interface.h>
#include <stdbool.h>

__BEGIN_DECLS

/**
 * @defgroup QWF_VOICE QWF Cellular Voice Service
 * @{
 */


/**
* Maximum string length of phone number
*/
#define QWF_VOICE_MAX_PHONE_NUMBER_LEN 80
/**
* Invalid call Id
*/
#define QWF_VOICE_INVALID_CALL_ID 0

/**
 * @brief Voice service state
 *
 * This enumeration type defines the possible states of voice service
 */
typedef enum
{
    VOICE_STATE_IDLE,   ///< The voice service is at idle state
    VOICE_STATE_ALERT,  ///< The voice service has an incoming call and is alerting the user
    VOICE_STATE_ACTIVE, ///< The voice service has an active call
    VOICE_STATE_INVALID ///< The voice service is at invalid state
} qwf_voice_VoiceState_e;

/**
 * @brief Call state
 *
 * This enumeration type defines the possible call states
 */
typedef enum
{
    VOICE_CALL_STATE_IDLE,            ///< The call is in idle state
    VOICE_CALL_STATE_INCOMING,        ///< There is an incoming call
    VOICE_CALL_STATE_CONNECTING,      ///< The call is connecting
    VOICE_CALL_STATE_REMOTE_RINGING,  ///< The remote side is ringing
    VOICE_CALL_STATE_CONNECTED,       ///< The call is connected
    VOICE_CALL_STATE_DISCONNECTED,    ///< The call is disconnected
    VOICE_CALL_STATE_FAILED,          ///< The call failed
    VOICE_CALL_STATE_INVALID          ///< The call is in invalid state
} qwf_voice_CallState_e;

/**
 * @brief Call type
 *
 * This enumeration type defines the possible call types
 */
typedef enum
{
    VOICE_CALL_TYPE_INCOMING,         ///< Incoming call
    VOICE_CALL_TYPE_OUTGOGING,        ///< Outgoing call
    VOICE_CALL_TYPE_EMERGENCY,        ///< Emergency call
    VOICE_CALL_TYPE_EMERGENCY_ECALL,  ///< Emergency ecall
    VOICE_CALL_TYPE_UNKNOWN           ///< Unknown call
} qwf_voice_CallType_e;

/**
 * @brief Call actions
 *
 * This enumeration type defines the possible call actions in bitmap
 *
 * @b Note: A call may have multiple call action items at one time.
 */
typedef enum
{
    VOICE_CALL_ACTIONS_NONE         = 0x00000000,  ///< No call actions
    VOICE_CALL_ACTIONS_MUTE_CALL    = 0x00000001,  ///< Mute call
    VOICE_CALL_ACTIONS_UNMUTE_CALL  = 0x00000002,  ///< Unmute call
    VOICE_CALL_ACTIONS_ANSWER_CALL  = 0x00000004,  ///< Answer call
    VOICE_CALL_ACTIONS_REJECT_CALL  = 0x00000008,  ///< Reject call
    VOICE_CALL_ACTIONS_END_CALL     = 0x00000010   ///< End call
} qwf_voice_CallActions_e;

/**
 * @brief Privacy indicator
 *
 * This enumeration type defines the possible privacy indicators for the incoming call number
 */
typedef enum
{
    VOICE_PRIVACY_INDICATOR_UNKNOWN,  ///< Incoming call number is unknown
    VOICE_PRIVACY_INDICATOR_PRIVATE,  ///< Incoming call number is private
    VOICE_PRIVACY_INDICATOR_NONE      ///< Incoming call number is shown as usual
} qwf_voice_PrivacyIndicator_e;

/**
 * @brief Call failure error code
 *
 * This enumeration type defines the possible error codes of a call failure
 *
 */
typedef enum
{
    VOICE_CALL_GENERAL_ERROR,                       ///< There was a general call failure
    VOICE_CALL_SUBSCRIBER_BUSY,                     ///< The subscriber is busy
    VOICE_CALL_CONGESTION,                          ///< There is network congestion
    VOICE_CALL_CELLULAR_PATH_UNAVAIL,               ///< The cellular path is not available
    VOICE_CALL_NUM_UNOBTAINABLE,                    ///< The caller number can't be obtained
    VOICE_CALL_AUTH_FAILURE,                        ///< There was an authentication failure
    VOICE_CALL_EMERGENCY_CALLS_ONLY,                ///< Only emergency call can be placed
    VOICE_CALL_EMERGENCY_CALL_NOT_ALLOWED,          ///< The use of emergency calls is restricted
    VOICE_CALL_OUTGOING_CALL_BARRED,                ///< The outgoing call was barred
    VOICE_CALL_SERVICE_NOT_AVAILABLE,               ///< The network service is not available
    VOICE_CALL_FDN_MISMATCHED,                      ///< There was an FDN (Fixed Dialing Number) mismatch error
    VOICE_CALL_INCOMING_CALL_BARRED,                ///< The incoming call was barred
    VOICE_CALL_NO_USER_RESPONDING,                  ///< The user didn't respond
    VOICE_CALL_USER_ALERTING_NO_ANSWER,             ///< The user is alerting but there is no answer
    VOICE_CALL_FACILITY_CLIR_NOT_SUBSCRIBED,        ///< The client isn't subscribed to CLIR (Calling Line Identification Restriction)
    VOICE_CALL_IMSI_NOT_IN_VLR_CAUSE_4,             ///< The IMSI (International Mobile Subscriber Identity) isn't in the VLR (Visitor Location Register)
    VOICE_CALL_IMEI_NOT_ACCEPTED_CAUSE_5,           ///< The IMEI (International Mobile Station Equipment Identity) was not accepted
    VOICE_CALL_REDIAL,                              ///< Redial indicator
    VOICE_CALL_MUST_END_CURRENT_SESSION,            ///< The current session must be ended before starting a call
    VOICE_CALL_UNKNOWN_ERROR,                       ///< An unknown error occurred
    VOICE_CALL_FAILURE_NONE                         ///< The call is established successfully
} qwf_voice_CallFailure_e;

/**
 * @brief Return result of voice service
 *
 * This enumeration type defines the possible voice service return results
 */
typedef enum
{
    VOICE_RETURN_SUCCESS,                 ///< The voice service returns successful result
    VOICE_RETURN_GENERAL_FAILURE,         ///< The voice service returns general failure
    VOICE_RETURN_INVALID_CALL,            ///< The voice service can't find the call with the specified call Id
    VOICE_RETURN_NETWORK_UNAVAILABLE,     ///< The network is unavailable
    VOICE_RETURN_NETWORK_REJECTED,        ///< The network has rejected the call
    VOICE_RETURN_INVALID_STATE,           ///< The voice service is in a state that makes this request invalid
    VOICE_RETURN_INVALID_PHONE_NUM,       ///< The phone number is invalid
    VOICE_RETURN_SERVICE_INACTIVE,        ///< The voice service is inactive
    VOICE_RETURN_EXISTS,                  ///< The voice service already exists or has been initialized
    VOICE_RETURN_NOT_SUPPORTED            ///< Function not supported by CRM and/or modem
} qwf_voice_ReturnResult_e;

/**
 * @brief Emergency category
 *
 * This enumeration type defines the possible emergency categories
 */
typedef enum
{
    VOICE_EMERGENCY_CATEGORY_POLICE,      ///< Emergency category is police
    VOICE_EMERGENCY_CATEGORY_AMBULANCE,   ///< Emergency category is ambulance
    VOICE_EMERGENCY_CATEGORY_FIRE,        ///< Emergency category is fire
    VOICE_EMERGENCY_CATEGORY_MARINE,      ///< Emergency category is marine
    VOICE_EMERGENCY_CATEGORY_MOUNTAIN,    ///< Emergency category is mountain
    VOICE_EMERGENCY_CATEGORY_DEFAULT      ///< Emergency category is default
} qwf_voice_EmergencyCategory_e;

/**
 * @brief Event codes for voice service events
 *
 * This enumeration defines the possible voice service event types.
 */
typedef enum
{
    /**
     * This event indicates that the incoming call has arrived.
     */
    VOICE_EVENT_INCOMING_CALL,
    /**
     * This event indicates that the call state has changed.
     */
    VOICE_EVENT_CALL_STATE_CHANGE,
    /**
     * This event indicates that the call type has changed.
     */
    VOICE_EVENT_CALL_TYPE_CHANGE,
    /**
     * This event indicates that the call actions has changed.
     */
    VOICE_EVENT_CALL_ACTIONS_CHANGE,
    /**
     * This event indicates that the start time has changed.
     */
    VOICE_EVENT_START_TIME_CHANGE,
    /**
     * This event indicates that the end time has changed.
     */
    VOICE_EVENT_END_TIME_CHANGE,
    /**
     * This event indicates that the ended-by-user has changed.
     */
    VOICE_EVENT_ENDED_BY_USER_CHANGE,
    /**
     * This event indicates that the caller number has changed.
     */
    VOICE_EVENT_CALLER_NUMBER_CHANGE,
    /**
     * This event indicates that the privacy indicator has changed.
     */
    VOICE_EVENT_PRIVATE_INDICATOR_CHANGE,
    /**
     * This event indicates that the call failure code has changed.
     */
    VOICE_EVENT_CALL_FAILURE_CODE_CHANGE,
    /**
     * This event indicates that the muted state has changed.
     */
    VOICE_EVENT_MUTED_CHANGE,
    /**
     * This event indicates that the last dialed number has changed.
     */
    VOICE_EVENT_LAST_DIALED_CHANGE,
    /**
     * This event indicates that the voice state has changed.
     */
    VOICE_EVENT_VOICE_STATE_CHANGE,
    /**
     * This event indicates that the voice service is inactive.
     */
    VOICE_EVENT_VOICE_SERVICE_INACTIVE,
    /**
     * This event indicates that the voice service is active.
     */
    VOICE_EVENT_VOICE_SERVICE_ACTIVE

} qwf_voice_EventCode_e;

/**
 * @brief Voice service call info
 *
 * Structure containing call information we monitor when an outgoing call is made
 * or an incoming call has arrived.
 */
typedef struct qwf_voice_CallInfo_s
{
    /**
     * Call state
     */
    qwf_voice_CallState_e callState;
    /**
     * Call type
     */
    qwf_voice_CallType_e callType;
    /**
     * Call actions
     * @see ::qwf_voice_CallActions_e
     */
    uint32_t callActions;
    /**
     * The time of the system clock in seconds when the call is connected
     */
    int startTime;
    /**
     * The time of the system clock in seconds when the call is disconnected
     */
    int endTime;
    /**
     * Indicates if the call was ended by the user. When it is true, it means
     * the call was ended by the user. When it is false, it means that the call
     * was ended remotely. The default is false.
     */
    bool endedByUser;
    /**
     * Caller number including NULL terminator
     */
    char callerNumber[QWF_VOICE_MAX_PHONE_NUMBER_LEN];
    /**
     * Privacy indicator
     */
    qwf_voice_PrivacyIndicator_e privateIndicator;
    /**
     * Call failure code
     */
    qwf_voice_CallFailure_e callFailureCode;
    /**
     * Muted state
     */
    bool muted;
} qwf_voice_CallInfo_t;

/**
 * @brief Voice service status info.
 *
 * Structure containing status information when an outgoing call is made
 * or an incoming call has arrived.
 */
typedef struct qwf_voice_StatusInfo_s
{
    /**
      * Last dialed phone number including NULL terminator
      */
    char lastDialed[QWF_VOICE_MAX_PHONE_NUMBER_LEN];
    /**
      * Voice state
      */
    qwf_voice_VoiceState_e voiceState;
} qwf_voice_StatusInfo_t;


/**
 * @brief Defines the structure used when passing voice events from voice API to
 * the client
 */
typedef struct qwf_voice_Event_s
{
    /**
      * The actual event to be processed
      */
    qwf_voice_EventCode_e eventCode;
    /**
      * The callback associated with the voice event.
      */
    void *pClientCb;
    /**
      * Voice event data
      */
    union
    {
        /**
          * The call Id returned when ::VOICE_EVENT_INCOMING_CALL is received
          */
         int callId;
        /**
          * The current call information returned when the following event is received
          * ::VOICE_EVENT_CALL_STATE_CHANGE
          * ::VOICE_EVENT_CALL_TYPE_CHANGE
          * ::VOICE_EVENT_CALL_ACTIONS_CHANGE
          * ::VOICE_EVENT_START_TIME_CHANGE
          * ::VOICE_EVENT_END_TIME_CHANGE
          * ::VOICE_EVENT_ENDED_BY_USER_CHANGE
          * ::VOICE_EVENT_CALLER_NUMBER_CHANGE
          * ::VOICE_EVENT_PRIVATE_INDICATOR_CHANGE
          * ::VOICE_EVENT_CALL_FAILURE_CODE_CHANGE
          * ::VOICE_EVENT_MUTED_CHANGE
          *
          * @see ::qwf_voice_EventCode_e
          */
         qwf_voice_CallInfo_t callInfo;
        /**
          * The current status information returned when the following event is received
          * ::VOICE_EVENT_LAST_DIALED_CHANGE
          * ::VOICE_EVENT_VOICE_STATE_CHANGE
          *
          * @see ::qwf_voice_EventCode_e
          */
        qwf_voice_StatusInfo_t statusInfo;
    } voiceEvent;
} qwf_voice_Event_t;

/**
 * @brief Client's function to process generated voice events.
 *
 * Callback registered by the client to notify when an event has been generated.
 * This function is called if there is a voice event.
 *
 * @param[in]   pCtx A pointer to QWF service context.
 * @param[in]   pEvent A pointer to voice event.
 * @param[in]   pClientData A pointer to client's data to be used with this event handler
 *                   @b Note: this is optional to allow client to associate an extra
 *                   data structure with this context. Voice service simply loops this
 *                   pointer back to the client. @c NULL is a valid value indicating not used.
 *
 */
typedef void (*qwf_voice_EventClientCallback)(qwf_Context_t * pCtx, qwf_voice_Event_t *pEvent, void *pClientData);


/**
 * @brief Register a callback for QWF voice service.
 *
 * @param[in]  pCtx  A pointer to QWF service context.
 * @param[in]  eventHandler The callback to handle voice events. If eventHandler is @c NULL, the callback is deregistered.
 * @param[in]  pClientData A pointer to client's data to be used with this event handler
 *                  @b Note: this is optional to allow client to associate an extra
 *                  data structure with this context. Voice service simply loops this
 *                  pointer back to the client. @c NULL is a valid value indicating not used.
 *
 * @return ::VOICE_RETURN_SUCCESS
 * @return ::VOICE_RETURN_GENERAL_FAILURE
 * @return ::VOICE_RETURN_SERVICE_INACTIVE
 *
 */
qwf_voice_ReturnResult_e qwf_voice_RegisterCallback(qwf_Context_t *pCtx, qwf_voice_EventClientCallback eventHandler, void *pClientData);


/**
 * @brief Get voice state
 *
 * @param[in]  pCtx A pointer to QWF context
 * @param[out]  pVoiceState A pointer to store the voice state value
 *
 * @return ::VOICE_RETURN_SUCCESS
 * @return ::VOICE_RETURN_GENERAL_FAILURE
 * @return ::VOICE_RETURN_SERVICE_INACTIVE
 */
qwf_voice_ReturnResult_e qwf_voice_GetVoiceState(const qwf_Context_t * const pCtx, qwf_voice_VoiceState_e * const pVoiceState);


/**
 * @brief Get last dialed phone number
 *
 * @param[in]  pCtx A pointer to QWF context
 * @param[out]  pLastDialed A pointer to store the last dialed phone number with NULL terminator
 *
 * @return ::VOICE_RETURN_SUCCESS
 * @return ::VOICE_RETURN_GENERAL_FAILURE
 * @return ::VOICE_RETURN_SERVICE_INACTIVE
 */
qwf_voice_ReturnResult_e qwf_voice_GetLastDialedNum(const qwf_Context_t * const pCtx, char * const pLastDialed);


/**
 * @brief Get call info
 *
 * @param[in]  pCtx A pointer to QWF context
 * @param[in]  callId Call id
 * @param[out]  pCallInfo A pointer to store call info structure
 *
 * @return ::VOICE_RETURN_SUCCESS
 * @return ::VOICE_RETURN_GENERAL_FAILURE
 * @return ::VOICE_RETURN_SERVICE_INACTIVE
 */
qwf_voice_ReturnResult_e qwf_voice_GetCallInfo(const qwf_Context_t * const pCtx,
                               int callId,
                               qwf_voice_CallInfo_t * const pCallInfo);


/**
 * @brief Request to start a regular voice call
 *
 * @param [in]  pCtx  A pointer to QWF context
 * @param [in]  pNum  A pointer to a dialed phone number with NULL terminator
 * @param [out]  pCallId  A pointer to store call Id returned (only valid when this API returns ::VOICE_RETURN_SUCCESS)
 *
 * @return ::VOICE_RETURN_SUCCESS
 * @return ::VOICE_RETURN_GENERAL_FAILURE
 * @return ::VOICE_RETURN_INVALID_CALL
 * @return ::VOICE_RETURN_NETWORK_UNAVAILABLE
 * @return ::VOICE_RETURN_NETWORK_REJECTED
 * @return ::VOICE_RETURN_INVALID_STATE
 * @return ::VOICE_RETURN_INVALID_PHONE_NUM
 * @return ::VOICE_RETURN_SERVICE_INACTIVE
 */
qwf_voice_ReturnResult_e qwf_voice_StartCall(const qwf_Context_t * const pCtx, const char *pNum, int * const pCallId);


/**
 * @brief Request to start an emergency call
 *
 * @param [in]  pCtx  A pointer to QWF context
 * @param [in]  category  Emergency category
 * @param [out]  pCallId  A pointer to store call Id returned (only valid when this API returns ::VOICE_RETURN_SUCCESS)
 *
 * @return ::VOICE_RETURN_SUCCESS
 * @return ::VOICE_RETURN_GENERAL_FAILURE
 * @return ::VOICE_RETURN_INVALID_CALL
 * @return ::VOICE_RETURN_NETWORK_UNAVAILABLE
 * @return ::VOICE_RETURN_NETWORK_REJECTED
 * @return ::VOICE_RETURN_INVALID_STATE
 * @return ::VOICE_RETURN_SERVICE_INACTIVE
 */
qwf_voice_ReturnResult_e qwf_voice_StartEmergencyCall(const qwf_Context_t * const pCtx, qwf_voice_EmergencyCategory_e category, int * const pCallId);


/**
 * @brief Request to answer the call
 *
 * @param [in]  pCtx  A pointer to QWF context
 * @param [in]  callId  call Id returned
 *
 * @return ::VOICE_RETURN_SUCCESS
 * @return ::VOICE_RETURN_GENERAL_FAILURE
 * @return ::VOICE_RETURN_INVALID_CALL
 * @return ::VOICE_RETURN_SERVICE_INACTIVE
 */
qwf_voice_ReturnResult_e qwf_voice_AnswerCall(const qwf_Context_t * const pCtx, int callId);


/**
 * @brief Request to reject the call
 *
 * @param [in]  pCtx  A pointer to QWF context
 * @param [in]  callId  call Id returned
 *
 * @return ::VOICE_RETURN_SUCCESS
 * @return ::VOICE_RETURN_GENERAL_FAILURE
 * @return ::VOICE_RETURN_INVALID_CALL
 * @return ::VOICE_RETURN_SERVICE_INACTIVE
 */
qwf_voice_ReturnResult_e qwf_voice_RejectCall(const qwf_Context_t * const pCtx, int callId);


/**
 * @brief Request to end the call
 *
 * @param [in]  pCtx  A pointer to QWF context
 * @param [in]  callId  call Id returned from @c qwf_voice_StartCall() or @c qwf_voice_StartEmergencyCall()
 *
 * @return ::VOICE_RETURN_SUCCESS
 * @return ::VOICE_RETURN_GENERAL_FAILURE
 * @return ::VOICE_RETURN_INVALID_CALL
 * @return ::VOICE_RETURN_SERVICE_INACTIVE
 *
 * @see @c qwf_voice_StartCall() and @see @c qwf_voice_StartEmergencyCall()
 */
qwf_voice_ReturnResult_e qwf_voice_EndCall(const qwf_Context_t * const pCtx, int callId);


/**
 * @brief Request to mute the call
 *
 * @param [in]  pCtx  A pointer to QWF context
 * @param [in]  callId  call Id returned from @c qwf_voice_StartCall() or @c qwf_voice_StartEmergencyCall()
 *
 * @return ::VOICE_RETURN_SUCCESS
 * @return ::VOICE_RETURN_GENERAL_FAILURE
 * @return ::VOICE_RETURN_INVALID_CALL
 * @return ::VOICE_RETURN_SERVICE_INACTIVE
 *
 * @see @c qwf_voice_StartCall() and @see @c qwf_voice_StartEmergencyCall()
 */
qwf_voice_ReturnResult_e qwf_voice_MuteCall(const qwf_Context_t * const pCtx, int callId);


/**
 * @brief Request to unmute the call
 *
 * @param [in]  pCtx  A pointer to QWF context
 * @param [in]  callId  call Id returned from @c qwf_voice_StartCall() or @c qwf_voice_StartEmergencyCall()
 *
 * @return ::VOICE_RETURN_SUCCESS
 * @return ::VOICE_RETURN_GENERAL_FAILURE
 * @return ::VOICE_RETURN_INVALID_CALL
 * @return ::VOICE_RETURN_SERVICE_INACTIVE
 *
 * @see @c qwf_voice_StartCall() and @see @c qwf_voice_StartEmergencyCall()
 */
qwf_voice_ReturnResult_e qwf_voice_UnmuteCall(const qwf_Context_t * const pCtx, int callId);


/**
 * @brief Request to start a DTMF tone
 *
 * Request to play a single DTMF tone when there is an active voice call ongoing.
 * The tone is played for 100 ms and then stopped.
 *
 * @param [in]  pCtx  A pointer to QWF context
 * @param [in]  pSingleTone  A pointer to a single tone which can be A-Z, 0-9, #, *, a, or b.
 *
 * @return ::VOICE_RETURN_SUCCESS
 * @return ::VOICE_RETURN_GENERAL_FAILURE
 * @return ::VOICE_RETURN_SERVICE_INACTIVE
 *
 * @b Note: if the tone is invalid, it returns ::VOICE_RETURN_GENERAL_FAILURE.
 */
qwf_voice_ReturnResult_e qwf_voice_StartDtmfTone(const qwf_Context_t * const pCtx, const char *pSingleTone);


/**
 * @brief Request to send a DTMF tone string
 *
 * Request to play a string of DTMF tones when there is an active voice call ongoing.
 * Each tone is played for 100ms. It will be stopped when the whole tone string has been played.
 *
 * @param [in]  pCtx  A pointer to QWF context
 * @param [in]  pTones  A pointer to a tone string (A-Z, 0-9, #, *, a, or b) with NULL terminator.
 *
 * @return ::VOICE_RETURN_SUCCESS
 * @return ::VOICE_RETURN_GENERAL_FAILURE
 * @return ::VOICE_RETURN_SERVICE_INACTIVE
 *
 * @b Note: if no voice call is active, it returns ::VOICE_RETURN_GENERAL_FAILURE.
 * If some tones are invalid, it skips the invalid tones and continues playing the remaining valid tones.
 */
qwf_voice_ReturnResult_e qwf_voice_SendDtmfTones(const qwf_Context_t * const pCtx, const char *pTones);

/** @}*/ // end group QWF_VOICE

__END_DECLS

#endif /* _QWF_VOICE_H_INCLUDED */

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/wireless_framework/branches/7.1/trunk/common/lib/qwf_interface/public/qwf/qwf_voice.h $ $Rev: 2922 $")
#endif
