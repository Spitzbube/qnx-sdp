/*
 * $QNXLicenseC:
 * Copyright 2015-2019, QNX Software Systems. All Rights Reserved.
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
 *
 * @file
 *
 * @brief  This interface provides access to the Short Message Service (SMS) services.
 *
 * @details This file provides the required data types and APIs
 *          to allow the client communicate with SMS services.
 *
 *          Client interacts with SMS services via this
 *          interface in order to
 *          - register the client as the only listener to
 *            receive all incoming short messages,
 *            qwf_sms_RegMsgListener()
 *          - deregister the client as listener to stop
 *            receiving any incoming short messages,
 *            qwf_sms_DeregMsgListener()
 *          - Register the client to receive SMS service events,
 *            qwf_sms_RegisterEventCallback()
 *          - send a short message to specific recipient,
 *            qwf_sms_SendMsg()
 *          - set the read status of the stored message at a
 *            specific index (memory slot) on SIM to
 *            read/unread, qwf_sms_SetReadStatusOnSim()
 *          - get the indices for the stored messages on SIM,
 *            qwf_sms_GetStoredMsgIndexOnSim()
 *          - read the message stored at a specific index on
 *            SIM, qwf_sms_ReadMsgOnSim()
 *          - delete the message stored at a specific index on
 *            SIM, qwf_sms_DelMsgOnSim()
 *          - get the SIM storage capabilities, qwf_sms_GetStorageCapabilityOnSim()
 *          - get the SMS service event value for ::SMS_MSG_ID,
 *            qwf_sms_GetMsgId()
 *          - get the SMS service event value for
 *            ::SMS_CONCATENATION_REF_NUMBER,
 *            qwf_sms_GetConcatRefNumber()
 *          - get the SMS service event value for
 *            ::SMS_STATS_MO_MSG_SUCCESS_COUNT,
 *            qwf_sms_GetMoMsgSuccCnt()
 *          - get the SMS service event value for
 *            ::SMS_STATS_MO_MSG_ERROR_COUNT,
 *            qwf_sms_GetMoMsgErrCnt()
 *          - get the SMS service event value for
 *            ::SMS_STATS_MT_MSG_SUCCESS_COUNT,
 *            qwf_sms_GetMtMsgSuccCnt()
 *          - get the SMS service event value for
 *            ::SMS_STATS_MT_MSG_ERROR_COUNT,
 *            qwf_sms_GetMtMsgErrCnt()
 *          - get the SMS service event value for
 *            ::SMS_SERVICE_BUSY, qwf_sms_GetSmsServiceBusy()
 *          - get the SMS option event value for
 *            ::SMS_SERVICE_CENTER_ADDRESS, qwf_sms_GetServiceCenterAddr()
 *          - set the SMS option event value for
 *            ::SMS_SERVICE_CENTER_ADDRESS, qwf_sms_SetServiceCenterAddr()
 *          - get the SMS option event value for
 *            ::SMS_ROUTING_PREFERENCE, qwf_sms_GetRoutingPref()
 *          - set the SMS option event value for
 *            ::SMS_ROUTING_PREFERENCE, qwf_sms_SetRoutingPref()
 *          - Reset the SMS service stats, qwf_sms_ResetSmsStats()
 *
 *          Usage:
 *          - Call qwf_Init() to initialize the QWF Context data
 *            used as container for QWF and SMS service.
 *          - Call qwf_sms_RegisterEventCallback() to receive
 *            the SMS service events.
 *          - Call qwf_sms_RegMsgListener() to register a
 *            callback function to receive incoming short
 *            messages.
 *
 * @b NOTE: qwf_Init() must be called once before using QWF services,
 *          and qwf_Cleanup() must be called once after using QWF services.
 *
 * For code examples, refer to @ref QWF_EXAMPLES_SMS
 *
 * More information regarding the QWF SMS service may be
 * obtained from the "QWF Architecture and Users Guide".
 */


#ifndef _QWF_SMS_H_INCLUDED
#define _QWF_SMS_H_INCLUDED

#include <qwf/qwf_interface.h>
#include <stdbool.h>

__BEGIN_DECLS

/**
 * @defgroup QWF_SMS QWF Cellular SMS Service
 * @{
 */


#define QWF_SMS_MAX_MSG_LEN                 3100    ///< Max SMS message length = Nr of segments * max user data per segment
#define QWF_SMS_MAX_MSG_OCTET_COUNT         9300    ///< Max SMS message length in UTF-8 octet = max message length * adjustment for UTF-8

#define QWF_SMS_MAX_NUMERIC_ADDR_LEN        20      ///< Maximum SMS address string length.
#define QWF_SMS_MAX_64_BIT_STRING_LEN       20      ///< Number of characters to represent unsigned 64bit int
#define QWF_SMS_MAX_SMS_ADDR_LEN            49      ///< Number of characters for the service center address

#define QWF_SMS_MIN_SEGMENTS_COUNT          1       ///< Min number of segments in an SMS message.
#define QWF_SMS_MAX_SEGMENTS_COUNT          20      ///< Maximum number of segments in a concatenated SMS message.
#define QWF_SMS_MIN_SIM_INDEX               1       ///< Min SIM index
#define QWF_SMS_MAX_SIM_INDEX               255     ///< Maximum SIM index
#define QWF_SMS_INVALID_MSG_LISTENER        -1      ///< SMS services returns -1 if client's registration attempt as listener fails


/**
 * @brief Enumeration of the outgoing SMS character encoding
 *
 */
typedef enum
{
    /**
     * Indicates no encoding is used.
     */
    SMS_NO_ENCODING,

    /**
     * GSM0338 is the 7-bit encoder, 7 bits per character.
     */
    SMS_GSM0338,

    /**
     * Universal character set 2-byte, 16 bits per character.
     */
    SMS_UCS2
} qwf_sms_OutgoingMsgEncoding_e;


/**
 * @brief Enumeration of the incoming SMS message character
 *        encoding.
 *
 */
typedef enum
{
    /**
     * 8-bit transformation format of Unicode.
     */
    SMS_UTF_8
} qwf_sms_IncomingMsgEncoding_e;



/**
 * @brief Describes the class of an incoming short message.
 *
 *  Values 0, 1, 2, 3 all represent their specific class and a
 *  value of 4 represents no class given.
 */
typedef enum
{
    /**
     * The SMS message is displayed on the screen without being
     * saved in the message store or on the SIM card
     */
    SMS_CLASS0,
    /**
     * The message is to be stored in the SIM card
     */
    SMS_CLASS1,
    /**
     * The message class carries SIM card data. The SIM card data
     * must be successfully transferred prior to sending
     * acknowledgment to the service center. An error message is
     * sent to the service center if this transmission is not
     * possible.
     */
    SMS_CLASS2,
    /**
     * This message is forwarded from the receiving entity to an
     * external device. The delivery acknowledgment is sent to the
     * service center regardless of whether or not the message was
     * forwarded to the external device.
     */
    SMS_CLASS3,
    /**
     * The class was not given by the SMS sender.
     */
    SMS_CLASS_NOT_GIVEN
} qwf_sms_MsgClass_e;


/**
 * @brief Defines the message status stored on SIM.
 */
typedef enum
{
    SMS_SIM_MSG_UNREAD = 0,                ///< The unread messages on SIM
    SMS_SIM_MSG_READ,                      ///< The read messages on SIM
    SMS_SIM_MSG_UNSENT,                    ///< The unsent messages on SIM
    SMS_SIM_MSG_SENT,                      ///< The sent messages on SIM
    SMS_SIM_MSG_ALL                        ///< Includes all, i. e. UNREAD, READ, UNSENT and SENT
} qwf_sms_SimMsgStatus_e;


/**
 * @brief Defines the SMS service event code.
 *
 * The service event value is reset after restart of SMS
 * Services or a reset of sms stats, ::qwf_sms_ResetSmsStats().
 *
 */
typedef enum
{
    SMS_MSG_ID = 1,                              ///< Service event:ID of the next uplink or downlink message.
    SMS_CONCATENATION_REF_NUMBER,                ///< Service event:The next 16-bit concatenation reference number to use in segmentation.
    SMS_STATS_MO_MSG_SUCCESS_COUNT,              ///< Service event:SMS message successfully sent.
    SMS_STATS_MO_MSG_ERROR_COUNT,                ///< Service event:Failed to send the SMS message.
    SMS_STATS_MT_MSG_SUCCESS_COUNT,              ///< Service event:Successfully received a short message.
    SMS_STATS_MT_MSG_ERROR_COUNT,                ///< Service event:There is no storage capacity for more downlink short messages.
    SMS_SERVICE_BUSY,                            ///< Service event:Indicates whether SMS service is currently busy processing an SMS request.
    SMS_SERVICE_CENTER_ADDRESS,                  ///< Option event: Address of the service center to which the device submits its uplink short messages.
    SMS_ROUTING_PREFERENCE,                      ///< Option event: Cellular network routing preference for uplink short messages.
    SMS_SERVICE_UNAVAILABLE,                     ///< Unable to receive any events, i. e. the SMS service is not available.
    SMS_SERVICE_AVAILABLE,                       ///< The SMS service is available and monitoring SMS activity
    SMS_EVENT_END
} qwf_sms_EventCode_e;




/**
 * @brief The structure is used to report the SMS service
 *        events.
 *
 */
typedef struct qwf_sms_EventData_s
{
    qwf_sms_EventCode_e        eventCode;      ///< Event name
    void       *  pClientCB;                    ///< Client callback

    union{
        /**
         * ID of the next uplink or downlink message. All possible
         * 32-bit integer values are valid.
         */
        int32_t    messageId;

        /**
         *  The next 16-bit concatenation reference number to use in
         *  segmentation. All possible 16-bit integer values are valid.
         */
        int16_t    concatenationReferenceNumber;

        /**
         * SMS message successfully sent. This value is reset after a
         * restart of SMS Services or ::qwf_sms_ResetSmsStats() request.
         * All non-negative integers are possible values.
         */
        int32_t    statsMoMessageSuccessCount;

        /**
         * Failed to send the SMS message. This value is reset after a
         * restart of SMS Services or ::qwf_sms_ResetSmsStats() request.
         * All non-negative integers are possible values.
         */
        int32_t    statsMoMessageErrorCount;

        /**
         * Successfully received a short message. This
         * value is reset after a restart of SMS Services or
         * ::qwf_sms_ResetSmsStats() request. All non-negative integers
         * are possible values.
         */
        int32_t    statsMtMessageSuccessCount;

        /**
         * Failed processing a number of messages during reception (at
         * SMS services level). This does not count any message rejected
         * by modem. The value is reset after a restart of SMS Services
         * or ::qwf_sms_ResetSmsStats() request. All non-negative
         * integers are possible values.
         */
        int32_t    statsMtMessageErrorCount;

        /**
         * If false, SMS service is ready to handle sending SMS.
         * If true, any attempts to send an SMS will result in a busy
         * failure.
         */
        bool       smsServiceBusy;

        /**
         * Null terminated string configured for the address of the
         * service center to which the device submits its uplink short
         * messages. All alpha-numeric strings are possible.
         */
        char    serviceCenterAddress[QWF_SMS_MAX_SMS_ADDR_LEN + 1];

        /**
         * Cellular network routing preference for uplink short
         * messages. 0 to 3 inclusive are possible, specifically
         * 0 = circuit-switched,
         * 1 = circuit-switched preferred,
         * 2 = packet-switched preferred,
         * 3 = packet-switched.
         */
        uint8_t routingPreference;
    } smsEvent; ///< event payload relevant to @c eventCode
}qwf_sms_EventData_t;



/**
 * @brief Defines the SMS return code
 *
 */
typedef enum
{
    SMS_ERR_SUCCESS = 0,

    /**
     * Unknown request message from SMS Client.
     */
    SMS_ERR_INVALID_MSG,

    /**
     * Value is unexpected.
     */
    SMS_ERR_INVALID_VALUE,

    /**
     * Payload has too many characters or invalid recipient address
     * length or too many SIM indices specified.
     */
    SMS_ERR_INVALID_SIZE,

    /**
     * Argument is missing.
     */
    SMS_ERR_MISSING_ARGUMENT,

    /**
     * There is an uplink message being processed.
     */
    SMS_ERR_SERVICE_IS_BUSY,

    /**
     * Attempt to register on TEXT_MESSAGING_PORT when a listener
     * has already been allocated.
     */
    SMS_ERR_FILTER_IS_BUSY,

    /**
     * Client is not the default text message listener.
     */
    SMS_ERR_NO_PERMISSION,

    /**
     * UICC is not ready, impossible to access the SIM.
     */
    SMS_ERR_SIM_NOT_READY,

    /**
     * Modem is not responsive or unable to perform the requested
     * action due to an invalid message index.
     */
    SMS_ERR_RADIO_FAILURE,

    /**
     * Service failure due to invalid client Id or unable to
     * allocate flash memory for the payload.
     */
    SMS_ERR_SERVICE_FAILURE,

    /**
     * Invalid message listener Id.
     */
    SMS_ERR_INVALID_LISTENER_ID,

    /**
     * The error is unrelated to the SMS service layer.
     */
    SMS_ERR_GENERIC_ERROR,

    /**
     * Already registered.
     */
    SMS_ERR_ALREADY_REGISTERED,

    /**
     * Client is not registered
     */
    SMS_ERR_NOT_REGISTERED,

    /**
     * The SMS service is not available
     */
    SMS_ERR_SERVICE_UNAVAILABLE,

    /**
     * Null parameter
     */
    SMS_ERR_PARAMETER_ERROR,

    /**
     * Already Exists or Initialized
     */
    SMS_ERR_EXISTS,
    /**
     * Function not supported by CRM and/or modem
     */
    SMS_ERR_NOT_SUPPORTED
} qwf_sms_ReturnCode_e;



/**
 * @brief Defines the supported service type.
 *
 */
typedef enum
{
    SMS_MSG_TYPE = 1        ///< SMS message type is supported.
} qwf_sms_MsgType_e;


/**
 * @brief Defines the SMS service notifications.
 *
 */
typedef enum
{
    /**
     * Notify the incoming SMS message
     */
    SMS_NOTIFY_SMS_INCOMING_MSG,
} qwf_sms_NotifyMtMsg_e;



/**
 * @brief Defines the structure to receive data about the
 *        receipt of an incoming short message.
 *
 */
typedef struct qwf_sms_Incoming_s
{
    /**
     * A 32-bit integer used to identify messages received via SMS
     * service. The client should keep this ID to match up the
     * message with future updates.
     */
    int32_t             msgId;

    /**
     * Null terminated string configured for SMS message sender's
     * phone number.
     */
    char                senderAddress[QWF_SMS_MAX_NUMERIC_ADDR_LEN + 1];

    /**
     * Null terminated string configured for the SMS message
     * payload. The payload is passed as a UTF-8 string. If the
     * message consists of multiple segments, the SUB character
     * (0x1A) is used to represent segments that have not been
     * received.
     */
    char               payload[QWF_SMS_MAX_MSG_OCTET_COUNT + 1];

    /**
     * Character encoding of the message payload. ::SMS_UTF_8 is the
     * possible value.
     */
    qwf_sms_IncomingMsgEncoding_e encoding;

    /**
     * Value range 0..4, see ::qwf_sms_MsgClass_e for more details.
     */
    qwf_sms_MsgClass_e  smsClass;

    /**
     * Record indices at which the concatenated SMS message segments
     * (each segment is a short message) are stored on the SIM card.
     * A concatenated incoming message could be up to
     * ::QWF_SMS_MAX_SEGMENTS_COUNT segments.
     * Available only if ::SMS_CLASS2.
     */
    uint8_t             simIndices[QWF_SMS_MAX_SEGMENTS_COUNT];

    /**
     * Null terminated string configured for serviceCenterTimeStamp.
     * The UTC (Coordinated Universal Time) time when the
     * serviceCenter received the SM. Value is a decimal number.
     * Optional attribute
     */
    char                serviceCenterTimeStamp[QWF_SMS_MAX_64_BIT_STRING_LEN + 1];

    /**
     * An indication that this message shall replace another message
     * from the same sender address and has the same replacement
     * type value.
     * Optional attribute
     */
    uint8_t             replacementType;

    /**
     * Total number of segments expected to be delivered under this
     * message id. Value range: 1..20. Otherwise is set to 0 if
     * unavailable.
     * Optional attribute
     */
    uint8_t             totalSegments;

    /**
     * Number of segments that have been received under this message
     * id. Once this value reaches @c totalSegments, message is
     * complete. Value range: 1..20. Otherwise is set to 0 if
     * unavailable.
     * Optional attribute.
     */
    uint8_t             receivedSegments;
} qwf_sms_IncomingMsg_t;



/**
 * @brief Defines the structure containing reply for the request
 *        to return the stored message at a specific index on
 *        SIM to client.
 *
 * On error, there will not be any data present except for the
 * error return code, see ::qwf_sms_ReturnCode_e
 */
typedef struct qwf_sms_ReadMsgOnSimRsp_s
{
    /**
     * True if sender address data is available otherwise
     * false.
     */
    bool               isSenderAddress;

    /**
     * Null terminated string configured for SMS message sender's
     * phone number.
     * Optional attribute
     */
    char                senderAddress[QWF_SMS_MAX_NUMERIC_ADDR_LEN + 1];

    /**
     * Null terminated string configured for SMS payload. The
     * payload is passed as a UTF-8 string. If the message consists
     * of multiple segments, the SUB character (0x1A) is used to
     * represent segments that have not been received.
     */
    char                payload[QWF_SMS_MAX_MSG_OCTET_COUNT + 1];

    /**
     * Character encoding of the message payload.
     * ::SMS_UTF_8 is the possible value.
     * Optional attribute
     */
    qwf_sms_IncomingMsgEncoding_e encoding;

    /**
     * Value range 0..4
     * Optional attribute.
     *
     * @see ::qwf_sms_MsgClass_e for more details.
     */
    qwf_sms_MsgClass_e  smsClass;

    /**
     * True if service center time stamp available otherwise
     * false.
     */
    bool                isSCTimeStamp;

    /**
     * Null terminated string configured for serviceCenterTimeStamp.
     * The UTC (Coordinated Universal Time) time when the
     * serviceCenter received the SM.
     * Optional attribute.
     */
    char                serviceCenterTimeStamp[QWF_SMS_MAX_64_BIT_STRING_LEN + 1];

    /**
     * An indication that this message shall replace another message
     * from the same sender address and has the same replacement
     * type value.
     * Optional attribute
     */
    uint8_t             replacementType;
} qwf_sms_ReadMsgOnSimRsp_t;


/**
 * @brief Defines the structure containing elements required to
 *        send an SMS message.
 *
 */
typedef struct qwf_sms_SendMsg_s
{
    /**
     * Null terminated string configured for SMS message recipient's
     * phone number.
     */
    char recipientAddress[QWF_SMS_MAX_NUMERIC_ADDR_LEN + 1];

    /**
     * Null terminated string configured for the payload.
     * The payload is passed as 7-bit or 16-bit per character string based on the
     * specified encoding types of SMS_GSM0338 and SMS_UCS2 respectively. When
     * SMS_NO_ENCODING is specified, the payload must contain a string of valid
     * hexidecimal digits.
     */
    char payload[QWF_SMS_MAX_MSG_OCTET_COUNT + 1];

    /**
     * Supported message encoding.
     */
    qwf_sms_OutgoingMsgEncoding_e encoding;
} qwf_sms_SendMsg_t;



/**
 * @brief Defines the structure used when passing the received
 *        incoming SMS messages to the client.
 *
 */
typedef struct qwf_sms_MsgListener_s
{
    void *                       pClientCB;            ///< Client callback
    /**
     * Mobil terminated messsage name ::SMS_NOTIFY_SMS_INCOMING_MSG
     */
    qwf_sms_NotifyMtMsg_e         msgName;
    qwf_sms_IncomingMsg_t         incomingMsg;         ///< Received an incoming SMS message
} qwf_sms_MsgListener_t;



/**
 * @brief Callback registered by the client to receive SMS
 *        service events.
 *
 * @param [in]  pCtx Pointer to service context.
 * @param [in]  pEvent Pointer to SMS service event data.
 * @param [in]  pClientData Pointer to client's data to be used
 *              with this event handler Note: this is optional
 *              to allow client to associate an extra data
 *              structure with this context. SMS service simply
 *              returns this pointer back to the client. NULL is
 *              a valid value indicating not used.
 *
 */
typedef void (*qwf_sms_EventClientCB_t)(qwf_Context_t *pCtx, qwf_sms_EventData_t *pEvent, void *pClientData);



/**
 * @brief Callback registered by the client to receive incoming
 *        short message.
 *
 * The callback is used when the client requests to register as
 * the only listener to the incoming messages, see
 * qwf_sms_RegMsgListener().
 *
 * @param [in]  pCtx Pointer to service context.
 * @param [in]  pMsgListener Pointer to message listener data.
 * @param [in]  pClientData Pointer to client's data to be used
 *              with this event handler Note: this is optional
 *              to allow client to associate an extra data
 *              structure with this context. SMS service simply
 *              returns this pointer back to the client. NULL is
 *              a valid value indicating not used.
 *
 */
typedef void (*qwf_sms_MsgListenerClientCB_t)(qwf_Context_t *pCtx, qwf_sms_MsgListener_t *pMsgListener, void *pClientData);



/**
 * @brief Register a callback to receive the SMS service events.
 *
 * @param[in]  pCtx Pointer to service context.
 * @param[in]  eventHandler The callback to handle SMS
 *             service events. NULL is a valid value and used
 *             when the client decides to de-register the
 *             callback function.
 * @param[in]  pClientData client's identifier to be used with
 *             this @c notifyHandler Note: this is optional to
 *             allow client to associate extra identification
 *             with this context. SMS service simply loops this
 *             pointer back to the client. NULL is a valid
 *             value.
 *
 * @return     One of ::qwf_sms_ReturnCode_e
 */
qwf_sms_ReturnCode_e qwf_sms_RegisterEventCallback(const qwf_Context_t * const pCtx,
                                                   qwf_sms_EventClientCB_t  eventHandler,
                                                   void *pClientData);


/**
 * @brief Request to send a short message to the recipient
 *        address.
 *
 * Immediately after sending the request the client receives a
 * delivery status event that explains whether the message
 * submission to the network was successful or not,
 * ::qwf_sms_EventCode_e. In case network detects any problem
 * returns an SMS message in which explains the message delivery
 * issue.
 *
 *
 * @param[in]  pCtx Pointer to service context.
 * @param[in]  pSendMsgData Pointer to the structure that
 *             contains the mandatory data to send the short
 *             message.
 * @param[out] pMsgId Reference pointer variable to return a
 *             32-bit integer used to identify messages sent
 *             using SMS service. The unique message ID is
 *             useful in case the recipient address is invalid
 *             which triggers the network to return an SMS
 *             message containing this message ID and an error
 *             string to explain the problem with the message
 *             delivery.
 *
 * @return     One of ::qwf_sms_ReturnCode_e
 */
qwf_sms_ReturnCode_e qwf_sms_SendMsg(const qwf_Context_t * const pCtx,
                                     const qwf_sms_SendMsg_t * const pSendMsgData,
                                     int32_t * pMsgId);



/**
 * @brief Request SMS service to register the client as the only
 *        listener to receive all incoming short messages.
 *
 * This API is the only way for the client to register the
 * message listener callback function to receive the incoming
 * short messages. Upon successful registration the SMS service
 * returns a unique ID that is used if client decides to
 * deregister as message listener, qwf_sms_DeregMsgListener().
 *
 * @param[in]  pCtx Pointer to service context.
 * @param[in]  MsgHandler Callback function is provided by the
 *             client. NULL is not a valid value. For
 *             de-registration see qwf_sms_DeregMsgListener().
 * @param[in]  msgType Indicates the type of message
 *             that is supported.
 * @param[out] pMsgListenerId Reference pointer variable to
 *             return a unique ID given by the SMS service
 *             identifies a single filter in the service and
 *             shall be used if client decides to deregister as
 *             listener. Valid identifier value is equal or
 *             greater than zero which means successfull
 *             registration otherwise is set to
 *             ::QWF_SMS_INVALID_MSG_LISTENER.
 * @param[in]  pClientData An optional pointer to extra data
 *             to be passed back to the client as the @c pClientData
 *             argument to @c MsgHandler() when the handler is executed.
 *
 * @return     One of ::qwf_sms_ReturnCode_e
 */
qwf_sms_ReturnCode_e qwf_sms_RegMsgListener(const qwf_Context_t * const pCtx,
                                            qwf_sms_MsgListenerClientCB_t MsgHandler,
                                            const qwf_sms_MsgType_e msgType,
                                            int32_t * pMsgListenerId,
                                            void *pClientData);


/**
 * @brief Request SMS service to deregister the client as the
 *        listener of the incoming messages.
 *
 * The request shall use the msgListenerId returned by
 * qwf_sms_RegMsgListener().
 *
 * @param[in]  pCtx Pointer to service context.
 * @param[in]  msgListenerId The unique id provided
 *             when the client registered as listener.
 *
 * @return     One of ::qwf_sms_ReturnCode_e
 */
qwf_sms_ReturnCode_e qwf_sms_DeregMsgListener(const qwf_Context_t * const pCtx,
                                              const int32_t msgListenerId);



/**
 * @brief Request SMS service to set the read/unread status for
 *        the stored short messages at specific indices on SIM.
 *
 * @param[in]  pCtx Pointer to service context.
 * @param[in]  pSimIndices Pointer to recorded indices of the
 *             short messages whose statuses are to be set.
 *             These indices should be the same ones as those in
 *             the incoming message notification.
 * @param[in]  simIndexCnt SIM indices array size.
 * @param[in]  isRead True means the short messages status
 *             are to be marked as read otherwise unread.
 *
 * @return     One of ::qwf_sms_ReturnCode_e
 */
qwf_sms_ReturnCode_e qwf_sms_SetReadStatusOnSim(const qwf_Context_t * const pCtx,
                                                const uint8_t * const pSimIndices,
                                                const uint8_t simIndexCnt,
                                                const bool isRead);


/**
 * @brief Request SMS service to return the indices for the
 *        stored messages with a specific message status on SIM.
 *
 * Client can use the same indices to read the stored messages
 * on SIM. (see ::qwf_sms_SimMsgStatus_e)
 *
 * @param[in]  pCtx Pointer to service context.
 * @param[in]  msgStatus Indicates the type of short messages
 *             eventually stored on SIM.
 * @param[out] pSimIndices Reference pointer to an array of size
 *             ::QWF_SMS_MAX_SIM_INDEX to return SIM indices for
 *             the stored short messages whose status match the
 *             requested type. The pSimIndexCount returns zero
 *             if no messages are found.
 * @param[out] pSimIndexCount Reference pointer variable to
 *             return the number of returned SIM indices.
 *
 * @return     One of ::qwf_sms_ReturnCode_e
 */
qwf_sms_ReturnCode_e qwf_sms_GetStoredMsgIndexOnSim(const qwf_Context_t * const pCtx,
                                                    const qwf_sms_SimMsgStatus_e msgStatus,
                                                    uint8_t * pSimIndices, uint16_t * pSimIndexCount);



/**
 * @brief Request SMS service to read messages stored at a
 *        specific index on SIM and return it to the client.
 *
 * Also the read message status on the SIM is to be set to read
 * if unread.
 *
 * @param[in]  pCtx Pointer to service context.
 * @param[in]  simIndex Index for the stored short message to be
 *             read and returned to client.
 * @param[out] pMsgOnSim Reference structure pointer to return
 *             the message payload stored at the specified index
 *             on SIM.
 *
 * @return     On error, there will not be any data present
 *             except for the error return code, see
 *             ::qwf_sms_ReturnCode_e
 */
qwf_sms_ReturnCode_e qwf_sms_ReadMsgOnSim(const qwf_Context_t * const pCtx,
                                          const uint8_t simIndex,
                                          qwf_sms_ReadMsgOnSimRsp_t * const pMsgOnSim);



/**
 * @brief Request SMS service to delete short messages atored at
 *        the specified indices on the SIM.
 *
 * @param[in] pCtx Pointer to service context.
 * @param[in] pSimIndices Pointer to the indices to be
 *            deleted.
 * @param[in] simIndexCnt SIM indices array size.
 *
 * @return    One of ::qwf_sms_ReturnCode_e
 */
qwf_sms_ReturnCode_e qwf_sms_DelMsgOnSim(const qwf_Context_t * const pCtx,
                                         const uint8_t * const pSimIndices,
                                         const uint8_t simIndexCnt);



/**
 * @brief Request SMS service to return the SIM storage
 *        capabilities.
 *
 * The returned storage capability data include the total amount
 * of records on SIM and the amount of free slots.
 *
 * @param[in]  pCtx Pointer to service context.
 * @param[out] pTotalRecords Reference pointer variable to
 *             return the total amount of records on the SIM for
 *             storage of short messages.
 * @param[out] pFreeRecords Reference pointer variable to return
 *             the total number of records on the SIM that
 *             currently do not have a short message stored.
 *
 * @return     One of ::qwf_sms_ReturnCode_e
 */
qwf_sms_ReturnCode_e qwf_sms_GetStorageCapabilityOnSim(const qwf_Context_t * const pCtx,
                                                       uint16_t * const pTotalRecords,
                                                       uint16_t * const pFreeRecords);



/**
 * @brief Request SMS service to return message Id of the next
 *        uplink or downlink message.
 *
 * @param[in]  pCtx Pointer to service context.
 * @param[out] pId Reference pointer variable to return
 *             the message Id. All possible 32-bit integer
 *             values are valid.
 *
 * @return     One of ::qwf_sms_ReturnCode_e
 */
qwf_sms_ReturnCode_e qwf_sms_GetMsgId(const qwf_Context_t * const pCtx, int32_t * pId);



/**
 * @brief Request SMS service to return the concatenation
 *        reference number to use in segmentation.
 *
 * @param[in]  pCtx Pointer to service context.
 * @param[out] pRefNum Reference pointer variable to return the
 *             concatenation reference number. All possible
 *             16-bit integer values are valid.
 *
 * @return     One of ::qwf_sms_ReturnCode_e
 */
qwf_sms_ReturnCode_e qwf_sms_GetConcatRefNumber(const qwf_Context_t * const pCtx, int16_t * pRefNum);



/**
 * @brief Request SMS service to return number of messages
 *        successfully sent.
 *
 * This value is not persisted and will be reset after a restart
 * of SMS Services or a reset of sms stats,
 * qwf_sms_ResetSmsStats().
 *
 * @param[in]  pCtx Pointer to service context.
 * @param[out] pCount Reference pointer variable to return
 *             the number of successfully sent messages. All
 *             non-negative integers are possible values.
 *
 * @return     One of ::qwf_sms_ReturnCode_e
 */
qwf_sms_ReturnCode_e qwf_sms_GetMoMsgSuccCnt(const qwf_Context_t * const pCtx, int32_t * pCount);



/**
 * @brief Request SMS service to return number of messages that
 *        failed to be sent.
 *
 * This value is not persisted and will be reset after a restart
 * of SMS Services or a reset of sms stats,
 * qwf_sms_ResetSmsStats().
 *
 * @param[in]  pCtx Pointer to service context.
 * @param[out] pCount Reference pointer variable to return
 *             the number of messages failed to send. All
 *             non-negative integers are possible values.
 *
 * @return     One of ::qwf_sms_ReturnCode_e
 */
qwf_sms_ReturnCode_e qwf_sms_GetMoMsgErrCnt(const qwf_Context_t * const pCtx, int32_t * pCount);



/**
 * @brief Request SMS service to return number of messages
 *        successfully received.
 *
 * This value is not persisted and will be reset after a restart
 * of SMS Services or a reset of sms stats,
 * qwf_sms_ResetSmsStats().
 *
 * @param[in]  pCtx Pointer to service context.
 * @param[out] pCount Reference pointer variable to return
 *             the number of successfully received messages. All
 *             non-negative integers are possible values.
 *
 * @return     One of ::qwf_sms_ReturnCode_e
 */
qwf_sms_ReturnCode_e qwf_sms_GetMtMsgSuccCnt(const qwf_Context_t * const pCtx, int32_t * pCount);



/**
 * @brief Request SMS service to return number of messages that
 *        failed processing during reception (at SMS services
 *        level).
 *
 * This does not count any rejected messages by modem. This
 * value is not persisted and will be reset after a restart of
 * SMS Services or a reset of sms stats,
 * qwf_sms_ResetSmsStats().
 *
 * @param[in]  pCtx Pointer to service context.
 * @param[out] pCount Reference pointer variable to return
 *             the number of messages that failed processing
 *             during reception. All non-negative integers are
 *             possible values.
 *
 * @return     One of ::qwf_sms_ReturnCode_e
 */
qwf_sms_ReturnCode_e qwf_sms_GetMtMsgErrCnt(const qwf_Context_t * const pCtx, int32_t * pCount);



/**
 * @brief Request SMS service to return whether SMS service is
 *        currently busy processing an SMS request.
 *
 * If false, SMS service is ready to handle an SMS send request.
 * If the value is true, any attempts to send an SMS will result
 * in a busy failure.
 *
 * @param[in]  pCtx Pointer to service context.
 * @param[out] pIsBusy Reference pointer variable to return the
 *       busy flag.
 *
 * @return     One of ::qwf_sms_ReturnCode_e
 */
qwf_sms_ReturnCode_e qwf_sms_GetSmsServiceBusy(const qwf_Context_t * const pCtx, bool * pIsBusy);



/**
 * @brief Request SMS service to set the service center (SMSC)
 *        address.
 *
 * The address is formatted using the typical ISDN / telephony
 * numbering plan (ITU E.164/E.163). By adding '+' (address
 * type) indicates the SMSC address is an international number.
 * Even if the address type is optional some modules require '+'
 * included when client set the service center address.
 *
 * @param[in]  pCtx Pointer to service context.
 * @param[in]  pScAddress Pointer to a null terminated service
 *       center address.
 *
 * @return     One of ::qwf_sms_ReturnCode_e
 */
qwf_sms_ReturnCode_e qwf_sms_SetServiceCenterAddr(const qwf_Context_t * const pCtx,
                                                     const char * const pScAddress);



/**
 * @brief Request SMS service to return the service center
 *        (SMSC) address to which the device submits its uplink
 *        short messages.
 *
 * All alpha-numeric strings are possible.
 *
 * @param[in]  pCtx Pointer to service context.
 * @param[in]  size Size of the array for service center
 *             address, ::QWF_SMS_MAX_SMS_ADDR_LEN.
 * @param[out] pScAddress Reference pointer to an array to
 *             return the null terminated service center
 *             address. All alpha-numeric strings are possible
 *
 * @return     One of ::qwf_sms_ReturnCode_e
 */
qwf_sms_ReturnCode_e qwf_sms_GetServiceCenterAddr(const qwf_Context_t * const pCtx, uint8_t size,
                                                  char * const pScAddress);



/**
 * @brief Request SMS service to set the routing preference for
 *        uplink short messages.
 *
 * Valid values: 0 to 3 inclusive are possible.
 *
 * @param[in]  pCtx Pointer to service context.
 * @param[in]  routNum Routing preference
 *             value.
 *             Values 0 to 3 are possible,
 *        specifically:
 *            0 = circuit-switched
 *            1 = circuit-switched preferred
 *            2 = packet-switched preferred
 *            3 = packet-switched.
 *
 * @return     One of ::qwf_sms_ReturnCode_e
 */
qwf_sms_ReturnCode_e qwf_sms_SetRoutingPref(const qwf_Context_t * const pCtx,
                                            const uint8_t routNum);



/**
 * @brief Request SMS service to return the routing preference
 *        for uplink short messages.
 *
 * Valid values: 0 to 3 inclusive are possible.
 *
 * @param[in]  pCtx Pointer to service context.
 * @param[out] pRoutNum Reference pointer variable to
 *             return the routing preference value.
 *             Values 0 to 3 are possible,
 *        specifically:
 *            0 = circuit-switched
 *            1 = circuit-switched preferred
 *            2 = packet-switched preferred
 *            3 = packet-switched.
 *
 * @return     One of ::qwf_sms_ReturnCode_e
 */
qwf_sms_ReturnCode_e qwf_sms_GetRoutingPref(const qwf_Context_t * const pCtx,
                                            uint8_t * const pRoutNum);



/**
 * @brief Request SMS service to reset the stats MO/MT
 *        Success/Error message counters.
 *
 * @param[in]  pCtx Pointer to service context.
 *
 * @return     One of ::qwf_sms_ReturnCode_e
 */
qwf_sms_ReturnCode_e qwf_sms_ResetSmsStats(const qwf_Context_t * const pCtx);

/** @}*/ // end group QWF_SMS

__END_DECLS

#endif /* _QWF_SMS_H_INCLUDED */

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/wireless_framework/branches/7.1/trunk/common/lib/qwf_interface/public/qwf/qwf_sms.h $ $Rev: 3015 $")
#endif
