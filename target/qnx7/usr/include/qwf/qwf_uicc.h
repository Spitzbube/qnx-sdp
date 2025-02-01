/*
 *
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
 * @file
 *
 * @brief  This interface provides access to the Cellular Service for
 *         the Universal Integrated Circuit Card (UICC).
 *
 * @details The Cellular UICC Service provides the common interface towards the
 *          UICC and its associated services. The UICC is the smart card used
 *          in the mobile devices that carries the user identity and subscription
 *          information. The Cellular UICC Service interface supports different
 *          types of UICC cards for both 3GPP and 3GPP2 standards, which include:
 *          -  USIM (UICC compliant SIM cards for 3GPP) - 3GPP TS 31.102
 *          -  SIM (Legacy 2G or GSM SIM cards) - 3GPP TS 11.11
 *          -  CSIM (CDMA SIM) - 3GPP2 C.S0065-0
 *          -  R-UIM (Removable User Identity Module - Legacy CDMA SIM) - 3GPP2 C.S0023-C
 *          -  ISIM (IMS Applications on SIM) - 3GPP TS 31.103
 *
 *          Clients interact with UICC via this interface in order to
 *          - Enable/Disable a PIN or Password to lock the device
 *          - Obtain notification on UICC status changes
 *          - Access or read the "Elementary Files" stored on the UICC
 *          - Read, modify or delete entries in the various phonebooks stored
 *            on the UICC
 *          - Access the subscriber information for the device
 *
 *          Usage:
 *          - Client application calls qwf_Init() to initialize the QWF Context
 *            data used as container for QWF and service data (like UICC)
 *          - Client application calls qwf_uicc_RegisterCallback() to register
 *            a callback fuction used to process events from the QWF UICC
 *            component. Note: simlar callbacks will be registered for each
 *            service the application is interested in.
 *          - Client application calls qwf_ProcessEvents() to block and wait
 *            for events to be sent to each services callback which was registered
 *            during initialization
 *          - The client application processes the events as required.
 *
 *          For code examples, refer to @ref QWF_EXAMPLES_UICC
 *
 *          More information regarding the QWF UICC service may be obtained
 *          from the "QWF Architecture and Users Guide".
 */

#ifndef _QWF_UICC_H_INCLUDED
#define _QWF_UICC_H_INCLUDED

#include <qwf/qwf_interface.h>
#include <stdbool.h>

__BEGIN_DECLS

/**
 * @defgroup QWF_UICC QWF Cellular UICC Service
 * @{
 */


/**
 * @brief Define constants used by UICC Phonebook entry structure
 *
 */
/**
 * Phonebook Maximum Number of Group names
 */
#define QWF_UICC_PB_MAX_GROUPS    5
/**
 * Phonebook Maximum Number of Email Addresses
 */
#define QWF_UICC_PB_MAX_EMAILS    1
/**
 * Phonebook Maximum Number of Extended Names and Numbers
 */
#define QWF_UICC_PB_MAX_EXTENDED  5
/**
 * Phonebook Maximum number of characters for Entry Name.
 * Note: Actual length is determined by SIM and can be retrieved using
 * interface qwf_uicc_GetPhonebookInfo()
 */
#define QWF_UICC_PB_MAX_NAME_LEN     241
/**
 * Phonebook Maximum number of characters for Phone Number.
 * Note: Actual length is determined by SIM and can be retrieved using
 * interface qwf_uicc_GetPhonebookInfo()
 */
#define QWF_UICC_PB_MAX_NUMBER_LEN   80
/**
 * Phonebook Maximum EMAIL Address Length.
 * Note: Actual length is determined by SIM and can be retrieved using
 * interface qwf_uicc_GetPhonebookInfo()
 */
#define QWF_UICC_PB_MAX_ADDRESS_LEN  241
/**
 * Phonebook Maximum Group Name Length.
 * Note: Actual length is determined by SIM and can be retrieved using
 * interface qwf_uicc_GetPhonebookInfo()
 */
#define QWF_UICC_PB_MAX_GROUP_LEN    241

/**
 * Network Short Name Maximum Length
 */
#define QWF_UICC_MAX_SHORT_NAME_LEN   48
/**
 * Network Long Name Maximum Length
 */
#define QWF_UICC_MAX_LONG_NAME_LEN    48
/**
 * Group ID Name Maximum Length
 */
#define QWF_UICC_MAX_GROUP_ID_LEN    255

/**
 * MNC and MCC Digit String Maximum Length
 */
#define QWF_UICC_MAX_MCC_DIGITS_LEN  3
#define QWF_UICC_MAX_MNC_DIGITS_LEN  3

/**
 * Home Country Abbreviated MCC Name Maximum Length
 */
#define QWF_UICC_MAX_ABBREV_MCC_LEN  3

/**
 * UICC ID Maximum Length
 */
#define QWF_UICC_MAX_ICCID_LEN 20

/**
 * @brief Return values used by UICC service interface
 *
 * @details This enumeration defines the possible UICC return values.
 */
typedef enum
{
    UICC_OK                 = 0,          ///< Successful result
    UICC_ERR_INT            = 1,          ///< Failure - Internal
    UICC_ERR_PARAM          = 2,          ///< Failure - Invalid parameter
    UICC_ERR_PIN_INVALID    = 3,          ///< Failure - PIN is invalid
    UICC_ERR_CHV_BLOCKED    = 4,          ///< Failure - CHV is Blocked
    UICC_ERR_EF_ID_INVALID  = 5,          ///< Failure - EF ID is invalid
    UICC_ERR_EF_NOT_PRESENT = 6,          ///< Failure - EF File or record Not Present
    UICC_ERR_INACTIVE       = 7,          ///< Failure - Uicc service is inactive
    UICC_ERR_EXIST          = 8,          ///< Failure - Already Exists
    UICC_ERR_NOT_SUPPORTED  = 9           ///< Function not supported by CRM and/or modem
} qwf_uicc_Results_e;


/**
 * @brief Event codes for UICC States.
 *
 * @details This enumeration defines the possible UICC States.
 */
typedef enum
{
    /**
     * SIM is ready to use and valid. Activation has completed
     */
    UICC_STATE_READY,
    /**
     * SIM is not detected (SIM is not accessible and only SOS allowed)
     */
    UICC_STATE_NOT_READY,
    /**
     * SIM is detected but need CHV validation. PIN is required.
     */
    UICC_STATE_CHV_REQUESTED,
    /**
     * SIM is not provisioned (Only SOS allowed)
     */
    UICC_STATE_NOT_PROVISIONED,
    /**
     * SIM is invalid (only SOS allowed) and in MEP locked state. Possible cause
     * could be 1) SIM for other carrier, 2) SIM is not compatible with device
     * technology (e.g. 3G SIM detected instead of 4G SIM)
     */
    UICC_STATE_INVALID_SIM,
    /**
     * SIM encountered general errors (SIM is not accessible). Possible cause
     * could be 1) SIM Not readable, 2) SIM corrupted
     */
    UICC_STATE_CARD_ERROR

} qwf_uicc_State_e;

/**
 * @brief Event codes for UICC Additional information.
 */
typedef enum
{
    /**
     * Default, no additional information for SIM status
     */
    UICC_EXT_INFO_NONE,
    /**
     * Optional only for the devices that supports the SIM detection line. This is
     * an interim state before the SIM initialization starts. Only applicable at
     * NOT_READY state.
     */
    UICC_EXT_INFO_SIM_INSERTED,
    /**
     * System is loading data into cache. Applicable in READY, NOT_READY and
     * CHV_REQUESTED states.
     */
    UICC_EXT_INFO_SIM_LOADING,
    /**
     * SIM is absent. Only applicable at NOT_READY state.
     */
    UICC_EXT_INFO_SIM_ABSENT,
    /**
     * Indicates card invalid due to IMEI lock. Applicable in INVALID_SIM state,
     */
    UICC_EXT_INFO_IMEI_LOCKED_SIM,
    /**
     * Indicates card activation has started when in NOT_PROVISIONED state
     */
    UICC_EXT_INFO_ACTIVATION_STARTED,
    /**
     * Indicates modem resetting when in UICC_NOT_READY state
     */
    UICC_EXT_INFO_MODEM_RESETTING,
    /**
     * UICC Memory error as per 3GPP TS 11.11 9.4.3.  Only applicable at
     * CARD_ERROR state.
     */
    UICC_EXT_INFO_MEMORY_PROBLEM,
    /**
     * UICC Technical problem as per 3GPP TS 11.11 9.4.6.  Only applicable at
     * CARD_ERROR state.
     */
    UICC_EXT_INFO_TECHNICAL_PROBLEM,
    /**
     * Found an unknown or unrecognized value in Extended state setting on UICC
     */
    UICC_EXT_INFO_UNKNOWN

} qwf_uicc_ExtentedStateInfo_e;

/**
 * @brief Bitmap values for possible UICC types.
 */
typedef enum
{
    UICC_TYPE_UNKNOWN = 0,   ///< Type is unknown
    UICC_TYPE_SIM  = 0x01,   ///< Type is SIM
    UICC_TYPE_USIM = 0x02,   ///< Type is USIM
    UICC_TYPE_CSIM = 0x04,   ///< Type is CSIM
    UICC_TYPE_RUIM = 0x08,   ///< Type is RUIM
    UICC_TYPE_ISIM = 0x10    ///< Type is ISIM

} qwf_uicc_Types_e;

/**
 * @brief Values for possible Card Holder Verification (CHV) number
 *
 * @details This enumeration defines the possible UICC return values.
 */
typedef enum
{
    UICC_CHV1       = 1,
    UICC_CHV2       = 2
} qwf_uicc_ChvNumbers_e;

/**
 * @brief Values for possible eCall Support types.
 */
typedef enum
{
    UICC_ECALL_NONE = 0,     ///< No eCall Support
    UICC_ECALL_ONLY,         ///< Support for eCall only
    UICC_ECALL_FULL          ///< eCall and Normal call support

} qwf_uicc_EcallSupport_e;


/**
 * @brief Event codes for UICC service events.
 *
 * @details This enumeration defines the possible UICC event types.
 */
typedef enum
{
    /**
     * This event indicates that the UICC Service subsystem is no longer actively
     * monitoring status for the UICC service. This could occur if connectivity is
     * lost to the UICC.
     */
    UICC_EVENT_UICC_SERVICE_INACTIVE,
    /**
     * This event indicates that the UICC Service subsystem is actively monitoring
     * status for the UICC service.
     */
    UICC_EVENT_UICC_SERVICE_ACTIVE,
    /**
     * This event indicates that the status of UICC has changed. Possible status
     * changes include "State", where values are defined by @c qwf_uicc_State_e
     * and possibly "Additional Information" which further defines the current
     * UICC state and has values defined in @c qwf_uicc_ExtentedStateInfo_e
     */
    UICC_EVENT_UICC_STATUS_CHANGE,
    /**
     * This event indicates that the type of UICC is known or updated. Possible
     * UICC types are provided as a bitmap within the event and defined by
     * qwf_uicc_Types_e
     */
    UICC_EVENT_UICC_TYPE_CHANGE,
    /**
     * This event indicates that the status of Card Holder Verification 1 has
     * changed. Possible status changes include "Enabled", "Blocked", "Unlock
     * Retries Remaining", and "Unblock Retries Remaining"
     */
    UICC_EVENT_CHV1_STATUS_CHANGE,
    /**
     * This event indicates that the status of Card Holder Verification 2 has
     * changed. Possible status changes include "Enabled", "Blocked", "Unlock
     * Retries Remaining", and "Unblock Retries Remaining"
     */
    UICC_EVENT_CHV2_STATUS_CHANGE,
    /**
     * This event indicates that a group ID has changed. Event returns Group ID 1
     * as byte array.
     */
    UICC_EVENT_GROUP_ID1_CHANGE,
    /**
     * This event indicates that a group ID has changed. Event returns Group ID 2
     * as byte array.
     */
    UICC_EVENT_GROUP_ID2_CHANGE,
    /**
     * This event indicates the HPLMN (MCC, MNC) and the Access Technology has
     * changed. The new MCC and MNC is provided in the event. The Access
     * Technology is also returned in the event and is represented as a two byte
     * bit mask as defined in 3GPP specification TS 31.102. Note: For the Access
     * Technology, only 1st mandatory PLMN will be published (bytes 4 and 5 from
     * spec). Our 2 bytes returned will be in following format:
     *        BYTE 5           |        BYTE 4
     * b8 b7 b6 b5 b4 b3 b2 b1 | b8 b7 b6 b5 b4 b3 b2 b1
     *
     */
    UICC_EVENT_HPLMN_ACT_CHANGE,
    /**
     * This event indicates the UICC ID has changed. The UICC ID is returned as a
     * 19 or 20 digit string
     */
    UICC_EVENT_ICCID_CHANGE,
    /**
     * This event eCall Supported setting has changed. The eCall Support Type
     * based on 3GPP TS 31.102 5.3.40. Possible values defined by
     * qwf_uicc_EcallSupport_e
     */
    UICC_EVENT_ECALL_SUPPORT_CHANGE,
    /**
     * This event indicates the HPLMN based on IMSI has changed. The new MCC and
     * MNC is provided in the event.
     * Note: This is same as ::UICC_EVENT_HPLMN_ACT_CHANGE. The IMSI HPLMN is
     * replacing the HPLMN in the future as per 3GPP TS 31.102 4.2.2
     */
    UICC_EVENT_IMSI_HPLMN_CHANGE,
    /**
     * This event indicates the current Operator Name String (ONS) and technology
     * name associated with the registered serving cell has changed. The ONS is
     * returned as a set of strings for "Long Name" and "Short Name" in a @c
     * qwf_uicc_NetworkNames_t structure within the event
     */
    UICC_EVENT_CURRENT_ONS_CHANGE,
    /**
     * This event indicates the home carrier name based on the IMSI Home PLMN has
     * changed. The home carrier name is returned as a set of strings for "Long
     * Name" and "Short Name" in a qwf_uicc_NetworkNames_t structure within the
     * event.
     */
    UICC_EVENT_HOME_CARRIER_CHANGE,
    /**
     * This event indicates the home country name based on the IMSI Home MCC
     * information has changed. The home country name is returned as a string for
     * within the event.
     * Note: The string provided within the event is the MCC abbreviation from the
     * SE13 table, or if not present, the MCC digits are provided.
     */
    UICC_EVENT_HOME_COUNTRY_CHANGE

} qwf_uicc_EventCode_e;


/**
 * @brief UICC Service Elementary File types.
 *
 * @details Elementary Files are files that reside on the SIM and contain
 *          various types of data. This ENUM represents the the different
 *          Elementary Files that can be read.
 */
typedef enum
{
    UICC_EF_ICCID,                ///< Unique identification number for the SIM.
    UICC_EF_LP,                   ///< User's preferred languages in priority order.
    UICC_EF_IMSI,                 ///< International Mobile Subscriber Identity.
    UICC_EF_KC,                   ///< Ciphering key and ciphering key sequence.
    UICC_EF_PLMN_SEL,             ///< User's preferred PLMNs in priority order.
    UICC_EF_HPLMN,                ///< Interval of time beween searches for the HPLMN.
    UICC_EF_ACM_MAX,              ///< Maximum value for the accumulated call meter.
    UICC_EF_SST,                  ///< SIM service table (indicates which services are allocated, and activated).
    UICC_EF_ACM,                  ///< Accumulated call meter.
    UICC_EF_GID1,                 ///< SIM level 1 group identifier.
    UICC_EF_GID2,                 ///< SIM level 2 group identifier.
    UICC_EF_SPN,                  ///< Service provider name.
    UICC_EF_PUCT,                 ///< Price-per-unit and currency table.
    UICC_EF_CBMI,                 ///< Specifies type of content for cell broadcast messages.
    UICC_EF_BCCH,                 ///< Broadcast control channel information.
    UICC_EF_ACC,                  ///< Assigned access control class(es).
    UICC_EF_FPLMN,                ///< Contains coding for four forbidden PLMNs.
    UICC_EF_LOCI,                 ///< Location information.
    UICC_EF_AD,                   ///< Contains administrative data concerning SIM's mode of operation.
    UICC_EF_PHASE,                ///< Information concerning the SIM's phase.
    UICC_EF_ADN,                  ///< Abbreviated dialing numbers and/or supplementary service control strings.
    UICC_EF_FDN,                  ///< Fixed dialing numbers and/or supplementary service control strings.
    UICC_EF_SMS,                  ///< Short messages.
    UICC_EF_CCP,                  ///< Capability configuration parameters.
    UICC_EF_MSISDN,               ///< MSISDN(s) related to the subscriber.
    UICC_EF_SMSP,                 ///< Short message service parameters.
    UICC_EF_SMSS,                 ///< Short message service status information.
    UICC_EF_LND,                  ///< Last number dialed.
    UICC_EF_EXT1,                 ///< Extension data for an ADN/SSC, MSISDN, or an LND.
    UICC_EF_EXT2,                 ///< Extension data for an FDN/SSC.
    UICC_EF_CPS,                  ///< Phase 1 elementary file; not used in phase 2 SIMs.
    UICC_EF_ELP,                  ///< Contains Extended Language Priority data.
    UICC_EF_VGCS,                 ///< List of group ID(s) for user-subscribed voice call group service(s).
    UICC_EF_VGCSS,                ///< Activation status for VGCS group identifiers.
    UICC_EF_VBS,                  ///< List of group ID(s) for user-subscribed voice broadcast service(s).
    UICC_EF_VBSS,                 ///< Activation status for VBS group identifiers.
    /**
     * Information about priority levels and first call set-up conditions for the
     * enhanced Multi-Level Pre-emption and Priority service usable by the
     * subscriber.
     */
    UICC_EF_EMLPP,
    UICC_EF_AAEM,                 ///< Priority levels of eMLPP that mobile station will auto answer.
    UICC_EF_CBMID,                ///< Cell Broadcast Message Identifier for Data Downlad.
    UICC_EF_ECC,                  ///< Contains up to five emergency call codes.
    UICC_EF_CBMIR,                ///< Contains ranges of cell broadcast message identifiers the MS should accept.
    UICC_EF_DCK,                  ///< De-personalization control keys.
    UICC_EF_CNL,                  ///< Co-perative network list.
    UICC_EF_NIA,                  ///< Network's indication of alerting.
    UICC_EF_SDN,                  ///< Special service numbers and/or supplementary service control strings.
    UICC_EF_EXT3,                 ///< Extension data for an SDN.
    UICC_EF_BDN,                  ///< Barred dialing numbers and/or supplementary service control strings.
    UICC_EF_EXT4,                 ///< Extension data for a BDN/SSC.
    UICC_EF_SMSR,                 ///< Short message status reports.
    UICC_EF_CPHS_VMWF,            ///< Voice message waiting flag,
    UICC_EF_CPHS_CFF,             ///< Call forwarding flag.
    UICC_EF_CPHS_ON,              ///< Operator name string.
    UICC_EF_CPHS_CSP,             ///< Customer service profile.
    UICC_EF_CPHS_INFO,            ///< Contains CPHS phase of the SIM and allocated CPHS specified services.
    UICC_EF_CPHS_MN,              ///< Dialing numbers to use for retrieval from mailboxes.
    UICC_EF_CPHS_ONS,             ///< Shortform of the operator name string.
    UICC_EF_CPHS_INFO_NUM,        ///< Information numbers.
    UICC_EF_CPHS_INFO_NUM_OLD,    ///< Information numbers, old format.
    UICC_EF_KC_GPRS,              ///< GPRS ciphering key and ciphering key sequence number.
    UICC_EF_LOCI_GPRS,            ///< GPRS location information.
    UICC_EF_OPL,                  ///< Operator PLMN list.
    UICC_EF_PNN,                  ///< PLMN network name.
    UICC_EF_HZ_PARAMETERS,        ///< Home zone definition parameters.
    UICC_EF_HZ_CACHE1,            ///< Home zone cell cache for zone ID 1.
    UICC_EF_HZ_CACHE2,            ///< Home zone cell cache for zone ID 2.
    UICC_EF_HZ_CACHE3,            ///< Home zone cell cache for zone ID 3.
    UICC_EF_HZ_CACHE4,            ///< Home zone cell cache for zone ID 4.
    UICC_EF_MWIS,                 ///< Message-waiting indication status.
    UICC_EF_PLMNW_ACT,            ///< Preferred PLMNs of the user in priority order.
    UICC_EF_OPLMNW_ACT,           ///< Preferred PLMNs of the operator in priority order.
    UICC_EF_HPLMNW_ACT,           ///< Access Technology for the HPLMN in priority order.
    UICC_EF_CPBCCH,               ///< CPBCCH information.
    UICC_EF_INVSCAN,              ///< Investigation PLMN Scan.
    UICC_EF_APP_SERVICE_TBL,      ///< Enable/disable applications for Cingular.
    UICC_EF_TERMINAL_SUPP_TBL,    ///< Indicates what applications terminal supports.
    UICC_EF_SCAN_CTRL_DATA,       ///< LAI ranges that scans are to be disabled within.
    UICC_EF_NET_SEL_MENU_CTRL_TBL, ///< Network selection menu control table.
    UICC_EF_ACTING_HPLMN,         ///< Contains the acting HPLMN.

    UICC_EF_PL,                   ///< New name for UICC_EF_ELP (Extended Language Priority data)
    UICC_EF_MBDN,                 ///< Mailbox dialling numbers
    UICC_EF_MBI,                  ///< Mailbox identifier
    UICC_EF_CFIS,                 ///< Call forwarding indication status
    UICC_EF_EXT6,                 ///< Extension data for MBDN
    UICC_EF_EXT7,                 ///< Extension data for CFIS
    UICC_EF_SPDI,                 ///< Service provider display information
    UICC_EF_MMSN,                 ///< MMS Notification
    UICC_EF_EXT8,                 ///< Extension data for MMSN
    UICC_EF_MMSICP,               ///< MMS issuer connectivity parameters
    UICC_EF_MMSUP,                ///< MMS user preferences
    UICC_EF_MMSUCP,               ///< MMS user connectivity preferences
    UICC_EF_SAI,                  ///< SoLSA access indicator
    UICC_EF_SLL,                  ///< SoLSA LSA list
    UICC_EF_MEXE_ST,              ///< MExE service table
    UICC_EF_ORPK,                 ///< Operator root public key
    UICC_EF_ARPK,                 ///< Administrator root public key
    UICC_EF_TPRPK,                ///< Third party root public key
    UICC_EF_ECCP,                 ///< Extended capability configuration parameters
    UICC_EF_SUME,                 ///< Setup Menu Elements
    UICC_EF_CMI,                  ///< Comparison method information
    UICC_EF_IMG,                  ///< Image
    UICC_EF_UST,                  ///< USIM service table (indicates which services are allocated, and activated)
    UICC_EF_EST,                  ///< Enabled Services Table (indicates which services are allocated, and activated)

    UICC_EF_LI,                   ///< Language Indication

    UICC_EF_PBR,                  ///< Phone Book Reference
    UICC_EF_IAP1,                 ///< Index Administration Phonebook (ADN1)
    UICC_EF_IAP2,                 ///< Index Administration Phonebook (ADN2)
    UICC_EF_IAP3,                 ///< Index Administration Phonebook (ADN3)
    UICC_EF_IAP4,                 ///< Index Administration Phonebook (ADN4)
    UICC_EF_ADN2,                 ///< 2nd ADN for USIM
    UICC_EF_ADN3,                 ///< 3rd ADN for USIM
    UICC_EF_ADN4,                 ///< 4th ADN for USIM
    UICC_EF_EMAIL1,               ///< Email address (ADN1)
    UICC_EF_EMAIL2,               ///< Email address (ADN2)
    UICC_EF_EMAIL3,               ///< Email address (ADN3)
    UICC_EF_EMAIL4,               ///< Email address (ADN4)
    UICC_EF_PBC1,                 ///< Phone Book Control (ADN1)
    UICC_EF_PBC2,                 ///< Phone Book Control (ADN2)
    UICC_EF_PBC3,                 ///< Phone Book Control (ADN3)
    UICC_EF_PBC4,                 ///< Phone Book Control (ADN4)
    UICC_EF_SNE1,                 ///< Secondary Name Entry (ADN1)
    UICC_EF_SNE2,                 ///< Secondary Name Entry (ADN2)
    UICC_EF_SNE3,                 ///< Secondary Name Entry (ADN3)
    UICC_EF_SNE4,                 ///< Secondary Name Entry (ADN4)
    UICC_EF_ANR1_A,               ///< 1st Additional Number (ADN1)
    UICC_EF_ANR1_B,               ///< 2nd Additional Number (ADN1)
    UICC_EF_ANR1_C,               ///< 3rd Additional Number (ADN1)
    UICC_EF_ANR1_D,               ///< 4th Additional Number (ADN1)
    UICC_EF_ANR2_A,               ///< 1st Additional Number (ADN2)
    UICC_EF_ANR2_B,               ///< 2nd Additional Number (ADN2)
    UICC_EF_ANR2_C,               ///< 3rd Additional Number (ADN2)
    UICC_EF_ANR2_D,               ///< 4rd Additional Number (ADN2)
    UICC_EF_ANR3_A,               ///< 1st Additional Number (ADN3)
    UICC_EF_ANR3_B,               ///< 2nd Additional Number (ADN3)
    UICC_EF_ANR3_C,               ///< 3rd Additional Number (ADN3)
    UICC_EF_ANR3_D,               ///< 4th Additional Number (ADN3)
    UICC_EF_ANR4_A,               ///< 1st Additional Number (ADN4)
    UICC_EF_ANR4_B,               ///< 2nd Additional Number (ADN4)
    UICC_EF_ANR4_C,               ///< 3rd Additional Number (ADN4)
    UICC_EF_ANR4_D,               ///< 4th Additional Number (ADN4)
    UICC_EF_RAT,                  ///< AT&T Radio Access Technology file. (single octet indicating the RAT configuration, 0-DM,1-2G,2-3G)
    UICC_EF_GRP1,                 ///< Grouping File (ADN1)
    UICC_EF_GRP2,                 ///< Grouping File (ADN2)
    UICC_EF_GRP3,                 ///< Grouping File (ADN3)
    UICC_EF_GRP4,                 ///< Grouping File (ADN4)
    UICC_EF_AAS,                  ///< Additional number Alpha String
    UICC_EF_GAS,                  ///< Group information Alpha String
    UICC_EF_HIDDENKEY,            ///< Key for hidden phone book entries
    UICC_EF_PERSO,                ///< O2 Personalisation File
    UICC_EF_RST,                  ///< RUIM Service Table
    UICC_EF_CST,                  ///< CSIM Sertvice Table
    UICC_EF_RUIMID,               ///< RUIM ID

    UICC_EF_DOMAIN,               ///< ISIM: Home network domain name
    UICC_EF_IMPU,                 ///< ISIM: IMS Public User Identity
    UICC_EF_IMPI,                 ///< ISIM: IMS Private User Identity
    UICC_EF_PCSCF,                ///< ISIM: Address of Proxy Call Session Control Function
    UICC_EF_PSISMSC,              ///< ISIM: Public Service Identity of the SM-SC

    UICC_EF_CC,                   ///< CSIM Call Count
    UICC_EF_IMSI_M,               ///< CSIM International Mobile Subscriber Identity.
    UICC_EF_IMSI_T,               ///< CSIM International Mobile Subscriber Identity.
    UICC_EF_TMSI,                 ///< CSIM TMSI (Temporary Mobile Station Identity)
    UICC_EF_ANALOG_HOME_SID,      ///< CSIM Analog home SID
    UICC_EF_ANALOG_OP_PARAMS,     ///< CSIM Analog operational parameters (Extended Address, Local Use Mark, Group ID)
    UICC_EF_ANALOG_LOCN_AND_REGN_IND,  ///< CSIM Analog location and registration indicators (NXTREG, SID, LOCAID, PUREG)
    UICC_EF_HOME_SID_NID,         ///< CSIM CDMA home SID and NID
    UICC_EF_ZONE_BASED_REGN_IND,  ///< CSIM CDMA zone-based registration indicators
    UICC_EF_SYS_REGN_IND,         ///< CSIM CDMA system/network registration indicators
    UICC_EF_DIST_BASED_REGN_IND,  ///< CSIM CDMA distance-based registration indicators
    UICC_EF_ACCOLC,               ///< CSIM Access overload class
    UICC_EF_CALL_TERM_MODE_PREF,  ///< CSIM Call termination mode preferences
    UICC_EF_SCI,                  ///< CSIM Suggested slot cycle index
    UICC_EF_ANALOG_CHAN_PREF,     ///< CSIM Analog channel preferences
    UICC_EF_PRL,                  ///< CSIM Preferred roaming list
    UICC_EF_SPC,                  ///< CSIM SPC (Service Programming Code)
    UICC_EF_OTAPA_SPC_ENABLE,     ///< CSIM Enable/disable OTAPA/SPC modification by network
    UICC_EF_NAM_LOCK,             ///< CSIM Locked/unlocked state of NAM
    UICC_EF_OTASP_OTAPA_FEATURES, ///< CSIM List of OTASP/OTAPA features supported by the RUIM
    UICC_EF_SP,                   ///< CSIM Service preferences
    UICC_EF_ESN_ME,               ///< CSIM ESN of the ME (Electronic Serial Number of the Mobile Equipment)
    UICC_EF_PREF_LANG,            ///< RUIM Language preference
    UICC_EF_CDMA_LI,              ///< CSIM Language indication
    UICC_EF_CDMA_SMS,             ///< CSIM Short messages
    UICC_EF_CDMA_SMS_PARAMS,      ///< CSIM Short message service parameters
    UICC_EF_CDMA_SMS_STATUS,      ///< CSIM Short message service status
    UICC_EF_SUP_SVCS_FEATURE_CODE_TABLE, ///< CSIM service table
    UICC_EF_CDMA_SPN,             ///< CDMA Service Provider Name
    UICC_EF_CDMA_MDN,             ///< CDMA CDMA Mobile Dialling Number
    UICC_EF_CDMA_ECC,             ///< CDMA CDMA emergency call codes
    UICC_EF_CDMA_GID1,            ///< CDMA Group Identifier, level 1
    UICC_EF_CDMA_GID2,            ///< CDMA Group Identifier, level 2
    UICC_EF_LRPLMNSI,             ///< USIM Last RPLMN Selection Indication
    UICC_EF_EPSLOCI,              ///< USIM EPS location information
    UICC_EF_SF_EUIMID,            ///< CSIM Short Form EUIMID (Expanded User Identity Module Identifier)
    UICC_EF_EHPLMN,               ///< USIM Equivalent HPLMN 3GPP TS 31.102 - 4.2.84
    UICC_EF_EHPLMNPI,             ///< USIM Equivalent HPLMN Presentation Indication 3GPP TS 31.102 - 4.2.85
    UICC_EF_PSLOCI,               ///< USIM Packet Switched Location Information 3GPP TS 31.102 - 4.2.23
    UICC_EF_ICEDN,                ///< USIM In Case of Emergency number - Dialing number
    UICC_EF_ICEFF,                ///< USIM In Case of Emergency number - Free Format
    UICC_EF_ICEGRAPHICS,          ///< USIM In Case of Emergency number - Image Format
    UICC_EF_MLPL,                 ///< CSIM Multimode Location Association Priority List (MLPL)
    UICC_EF_MSPL,                 ///< CSIM Multimode System Priority List (MSPL)
    UICC_EF_IST,                  ///< ISIM Service Table
    UICC_EF_GBABP,                ///< ISIM GBA Bootstrapping parameters
    UICC_EF_GBANL,                ///< ISIM GBA NAF list
    UICC_EF_NAFKCA,               ///< ISIM NAF key centre address
    UICC_EF_UICCIARI,             ///< ISIM UICC IMS Application Reference Identifier 3GPP TS 31.103 - 4.2.16
    UICC_EF_ACSGL,                ///< USIM Allowed CSG Lists 3GPP TS 31.102 - 4.4.6.2 - from DF_HNB (id:205)
    UICC_EF_CSGT,                 ///< USIM CSG Type 3GPP TS 31.102 - 4.4.6.3 - from DF_HNB
    UICC_EF_HNBN,                 ///< USIM Home NodeB Name 3GPP TS 31.104 - 4.4.6.4 - from DF_HNB
    UICC_EF_OCSGL,                ///< USIM Operator CSG Lists 3GPP TS 31.102 - 4.4.6.5 - from DF_HNB
    UICC_EF_OCSGT,                ///< USIM Operator CSG Type 3GPP TS 31.102 - 4.4.6.6 - from DF_HNB
    UICC_EF_OHNBN,                ///< USIM Operator Home NodeB Name 3GPP TS 31.102 - 4.4.6.7 - from DF_HNB
    UICC_EF_MTNPREPAY,            ///< USIM carrier "MTN" prepay or postpay card identifier
    UICC_EF_DIR                   ///< ETSI 102.221 mandaory element for UICC for AIDs

} qwf_uicc_ElementaryFile_e;

/**
 * @brief UICC Service Elementary File types.
 *
 * @details Elementary Files are files that reside on the SIM and contain
 *          various types of data. This ENUM represents the type of data
 *          within the elementary file.
 */
typedef enum
{
    /**
     * Transparent Type - binary
     */
    UICC_EF_TRANS_TYPE,
    /**
     * Linear Fixed Record Type
     */
    UICC_EF_LINEAR_TYPE,
    /**
     * Cyclic Record Type
     */
    UICC_EF_CYCLIC_TYPE,
    /**
     * Invalid EF Type
     */
    UICC_EF_INVALID_TYPE

} qwf_uicc_ElementaryFileType_e;

/**
 * @brief UICC Service Phonebook types.
 *
 * @details This ENUM represents the different Phonebook Files that can be
 *          accessed.
 */
typedef enum
{
    /**
     * Use to access the FDN Phonebook on SIM for Fixed Dialing Numbers
     */
    UICC_PB_FDN,
    /**
     * Use to access the ADN Phonebook on SIM for Abbreviated Dialing Numbers
     */
    UICC_PB_ADN,
    /**
     * Use to access the SDN Phonebook on SIM for Service Dialing Numbers
     */
    UICC_PB_SDN

} qwf_uicc_PhonebookType_e;

/**
 * @brief UICC Access technology types.
 *
 * @details This ENUM represents the different System Access technologies that
 *          can be represented.
 */
typedef enum
{
    /**
     * system access technology Unknown
     */
    UICC_ACCESSTECH_UNKNOWN  = 0x0000,
    /**
     * system access technology 2G (GSM)
     */
    UICC_ACCESSTECH_GSM      = 0x0001,
    /**
     * system access technology CDMA (1XRTT)
     */
    UICC_ACCESSTECH_CDMA     = 0x0002,
    /**
     * system access technology 3G (UTRAN)
     */
    UICC_ACCESSTECH_UTRAN    = 0x0004,
    /**
     * system access technology HRPD (1XEVDO)
     */
    UICC_ACCESSTECH_EVDO     = 0x0008,
    /**
     * system access technology LTE (EUTRAN)
     */
    UICC_ACCESSTECH_EUTRAN   = 0x0010
} qwf_uicc_AccessTech_e;

/**
 * @brief Defines the structure used UICC status information for event
 *        UICC_EVENT_UICC_STATUS_CHANGE
 */
typedef struct qwf_uicc_UiccStatus_s
{
    /**
     * UICC current state
     */
    qwf_uicc_State_e  state;
    /**
     * UICC "Additional Information" which further defines the current UICC state
     */
    qwf_uicc_ExtentedStateInfo_e extInfo;
    /**
     * UICC "Present".  Boolean where true indicates SIM is detected
     */
    bool present;

} qwf_uicc_UiccStatus_t;

/**
 * @brief Defines the structure used Card Holder Verification (CHV) status
 *        information for events ::UICC_EVENT_CHV1_STATUS_CHANGE and
 *        ::UICC_EVENT_CHV2_STATUS_CHANGE
 */
typedef struct qwf_uicc_ChvStatus_s
{
    /**
     * CHV state, Enabled is TRUE.
     */
    bool  enabled;
    /**
     * Indicated whether the CHV is blocked.
     */
    bool  blocked;
    /**
     * Number of "Unlock Retries" remain before device becomes blocked. Value is
     * decremented by 1 for each failed PIN code entered. Once no retries remain
     * the CHV is blocked.
     */
    int unlockRetriesRemaining;
    /**
     * Number of "Unblock Retries" remain before device becomes blocked. Value is
     * decremented by 1 for each failed PUK code entered when attempting to
     * unblock the CHV.
     */
    int unblockRetriesRemaining;

} qwf_uicc_ChvStatus_t;

/**
 * @brief Defines the HPLMN (MCC, MNC) and the Access Technology information
 *        for events ::UICC_EVENT_HPLMN_ACT_CHANGE
 */
typedef struct qwf_uicc_HplmnAct_s
{
    /**
     * Mobile Country code
     */
    char  mccStr[QWF_UICC_MAX_MCC_DIGITS_LEN+1];
    /**
     * Mobile Network code
     */
    char  mncStr[QWF_UICC_MAX_MNC_DIGITS_LEN+1];
    /**
     * Two byte bitmask that defines the access technology
     * bit values defined in qwf_uicc_AccessTech_e.
     */
    qwf_uicc_AccessTech_e  act;

} qwf_uicc_HplmnAct_t;

/**
 * @brief Defines the HPLMN (MCC, MNC) for events UICC_EVENT_HPLMN_IMSI_CHANGE
 */
typedef struct qwf_uicc_HplmnImsi_s
{
    /**
     * Mobile Country code
     */
    char  mccStr[QWF_UICC_MAX_MCC_DIGITS_LEN+1];
    /**
     * Mobile Network code
     */
    char  mncStr[QWF_UICC_MAX_MNC_DIGITS_LEN+1];
} qwf_uicc_HplmnImsi_t;

/**
 * @brief Structure containing Short and Long forms of network names
 */
typedef struct qwf_uicc_NetworkNames_s {
    /**
     * Long form of network name (Null terminated string).
     */
    char longName[QWF_UICC_MAX_LONG_NAME_LEN + 1];
    /**
     * Short form of network name (Null terminated string).
     */
    char shortName[QWF_UICC_MAX_SHORT_NAME_LEN + 1];

} qwf_uicc_NetworkNames_t;


/**
 * @brief Defines the structure used when passing CDS events from CDS API to
 * the Client
 */
typedef struct qwf_uicc_Event_s
{
    /**
     *  The actual event to be processed
     */
    qwf_uicc_EventCode_e eventCode;
    /**
     *  The client Callback to process the event
     */
    void *pClientCB;
    /**
     *  Union of all the possible Event data members
     */
    union
    {
        /**
         *  UICC status information for event ::UICC_EVENT_UICC_STATUS_CHANGE
         */
        qwf_uicc_UiccStatus_t uiccStatus;
        /**
         *  UICC status information for event ::UICC_EVENT_UICC_TYPE_CHANGE Value
         *  contains a bitmap of possible types as defined in qwf_uicc_Types_e
         */
         int uiccType;
        /**
         *  CHV status information for events ::UICC_EVENT_CHV1_STATUS_CHANGE and
         *  ::UICC_EVENT_CHV2_STATUS_CHANGE
         */
        qwf_uicc_ChvStatus_t chvStatus;
        /**
         *  Information for event ::UICC_EVENT_ECALL_SUPPORT_CHANGE
         */
        qwf_uicc_EcallSupport_e eCallType;
        /**
         *  Information for event ::UICC_EVENT_GROUP_ID1_CHANGE or
         *  ::UICC_EVENT_GROUP_ID2_CHANGE.
         *  groupId is a null terminated string.
         */
        char groupId[QWF_UICC_MAX_GROUP_ID_LEN + 1];
        /**
         *  Information for event ::UICC_EVENT_HPLMN_ACT_CHANGE
         */
        qwf_uicc_HplmnAct_t hplmnact;
        /**
         *  Information for event ::UICC_EVENT_IMSI_HPLMN_CHANGE
         */
        qwf_uicc_HplmnImsi_t hplmn;
        /**
         *  Information for event ::UICC_EVENT_ICCID_CHANGE. The UICC ID is returned
         *  as a 19 or 20 digit string
         */
        char iccid[QWF_UICC_MAX_ICCID_LEN + 1];
        /**
         *  Short and Long names for events ::UICC_EVENT_CURRENT_ONS_CHANGE and
         *  ::UICC_EVENT_HOME_CARRIER_CHANGE
         */
        qwf_uicc_NetworkNames_t names;
        /**
         *  Abbreviated names for MCC for event ::UICC_EVENT_HOME_COUNTRY_CHANGE Note:
         *  The string provided within the event is the MCC abbreviation from the SE13
         *  table, or if not present, the MCC digits are provided as a null terminated
         *  string.
         */
        char mccName[QWF_UICC_MAX_ABBREV_MCC_LEN + 1];


    } uiccEvent;

} qwf_uicc_Event_t;

/**
 * @brief Defines the structure used when passing Carrier Information to the
 *        Client via the UICC API @c qwf_uicc_GetSubscriberInfo()
 */
typedef struct qwf_uicc_SubscriberInfo_s
{
    /**
     *  HPLMN mcc, mnc and act
     */
    qwf_uicc_HplmnAct_t hplmnact;
    /**
     *  IMSI HPLMN mcc and mnc
     */
    qwf_uicc_HplmnImsi_t hplmn;
    /**
     * Current UICC ONS Names.
     */
    qwf_uicc_NetworkNames_t uiccOns;
    /**
     * Current UICC Home Carrier Names.
     */
    qwf_uicc_NetworkNames_t homeCarrier;
    /**
     *  Home Country NULL terminated string (MCC fro SE13 table if exists, else
     *  the MCC digits)
     */
    char homeCountry[QWF_UICC_MAX_ABBREV_MCC_LEN + 1];
    /**
     * NULL terminated string representing Group ID 1.
     */
    char groupId1[QWF_UICC_MAX_GROUP_ID_LEN + 1];
    /**
     * NULL terminated string representing Group ID 2.
     */
    char groupId2[QWF_UICC_MAX_GROUP_ID_LEN + 1];
    /**
     *  Information for event ::UICC_EVENT_ICCID_CHANGE. The UICC ID is returned
     *  as a 19 or 20 digit string
     */
    char iccid[QWF_UICC_MAX_ICCID_LEN + 1];
    /**
     * Current UICC Ecall support setting.
     */
    qwf_uicc_EcallSupport_e uiccEcall;

} qwf_uicc_SubscriberInfo_t;

/**
 * @brief Structure containing UICC Elementary File (EF) information. Data
 *        type is used when getting EF file information via @c
 *        qwf_uicc_GetFileInfo()
 */
typedef struct qwf_uicc_ElementaryFileInfo_s {
    /**
     * EF File Type
     */
    qwf_uicc_ElementaryFileType_e efType;
    /**
     * EF File Size in bytes
     */
    int efSize;
    /**
     * Contains the length of each record within the EF file
     */
    int efRecordLen;
    /**
     * Contains the number of records within the EF file
     */
    int efRecordCount;

} qwf_uicc_ElementaryFileInfo_t;

/**
 * @brief Structure containing UICC Phonebook File information. Data type is
 *        used when getting EF file information via qwf_uicc_GetFileInfo()
 */
typedef struct qwf_uicc_PhonebookFileInfo_s {
    /**
     * Phonebook File Type
     */
    qwf_uicc_PhonebookType_e pbType;
    /**
     * Indicates whether specified phonebook type is available (true) or not
     * available (false)
     */
    bool available;
    /**
     * Contains the number of records or entries within the Phonebook file
     */
    int recordCount;
    /**
     * Phonebook Mode - Enabled (true) or Disabled (false)
     */
    bool mode;
    /**
     * Maximum number of bytes for phonebook entry Name string
     */
    int nameLen;
    /**
     * Maximum number of digits for Phone Number string
     */
    int numberLen;
    /**
     * Maximum number of bytes for phonebook entry Email string
     */
    int emailLen;

} qwf_uicc_PhonebookFileInfo_t;


/**
 * @brief Structure containing UICC Phonebook "Extended" data
 */
typedef struct qwf_uicc_PhonebookExtendedData_s {
    /**
     * Phonebook entry "extended" Name (NULL terminated string).
     */
    char extName[QWF_UICC_PB_MAX_NAME_LEN + 1];
    /**
     * Phonebook entry "extended" Phone Number (NULL terminated string).
     */
    char extNumber[QWF_UICC_PB_MAX_NUMBER_LEN + 1];

} qwf_uicc_PhonebookExtendedData_t;

/**
 * @brief Structure containing UICC Phonebook Email Address
 */
typedef struct qwf_uicc_PhonebookEmail_s {
    /**
     * Phonebook entry Email Address (NULL terminated string).
     */
    char address[QWF_UICC_PB_MAX_ADDRESS_LEN + 1];
} qwf_uicc_PhonebookEmail_t;

/**
 * @brief Structure containing UICC Phonebook Group name
 */
typedef struct qwf_uicc_PhonebookGroupInfo_s {
    /**
     * Names of any group for a Phonebook entry (NULL terminated string).
     */
    char name[QWF_UICC_PB_MAX_GROUP_LEN + 1];

} qwf_uicc_PhonebookGroupInfo_t;

/**
 * @brief Structure containing UICC Phonebook entry or record data
 */
typedef struct qwf_uicc_PhonebookEntryData_s {
    /**
     * Phonebook type
     */
    qwf_uicc_PhonebookType_e pbType;
    /**
     * Entry or Record index in phonebook
     */
    int pbEntry;
    /**
     * Name of the phonebook entry (NULL terminated string).
     */
    char pbEntryName[QWF_UICC_PB_MAX_NAME_LEN + 1];
    /**
     * Phone Number of the phonebook entry (NULL terminated string).
     */
    char pbEntryNumber[QWF_UICC_PB_MAX_NUMBER_LEN + 1];
    /**
     * An optional Boolean that indicates whether this entry is hidden. Applies
     * only for extended phonebook (ADN) in USIM.
     */
    bool hidden;
    /**
     * An optional second name, represented as a UTF-8 string (NULL terminated)
     * that contains either GSM 7-bit or UCS2 character sets. Applies only for
     * extended phonebook (ADN) in USIM.
     */
    char pbSecondName[QWF_UICC_PB_MAX_NAME_LEN + 1];
    /**
     * The number of groups this entry belongs to.
     */
    int pbNumGroups;
    /**
     * The names of any groups this entry belongs to.
     * Note: the member pbNumGroups indicates the number of group names in this
     * array of strings
     */
    qwf_uicc_PhonebookGroupInfo_t pbGroupName[QWF_UICC_PB_MAX_GROUPS];
    /**
     * The number of mail address(es) for this entry.
     */
    int pbNumEmailAddrs;
    /**
     * The Email address(es) for this entry.
     * Note: the member pbNumEmailAddrs indicates the number of Email address(es)
     * in this array of strings
     */
    qwf_uicc_PhonebookEmail_t pbEmail[QWF_UICC_PB_MAX_EMAILS];
    /**
     * The number of Extended Names and Numbers for this entry.
     */
    int pbNumExtended;
    /**
     * The number of Extended Names and Numbers for this entry.
     * Note: the member pbNumExtended indicates the number of Extended Names and
     * Numbers in this array qwf_uicc_PhonebookExtendedData_t data
     */
    qwf_uicc_PhonebookExtendedData_t pbExtended[QWF_UICC_PB_MAX_EXTENDED];

} qwf_uicc_PhonebookEntryData_t ;


/**
 * @brief Client's function prototype used  when registering callback fucntion
 *        to process generated UICC related events.
 *
 * Callback registered by the client to notify when an event has been
 * generated. This function is called if there is a UICC event in the event
 * queue to be passed to client.
 *
 * @param [in]  pCtx Pointer to Service context.
 * @param [in]  pEvent Pointer to UICC event data.
 * @param [in]  pClientData  Pointer to client's data to be used with this
 *             event handler Note: this is optional to allow client to
 *             associate an extra data structure with this context. UICC
 *             service simply returns this pointer back to the client. NULL is
 *             a valid value indicating not used.
 *
 */
typedef void (*qwf_uicc_EventClientCallback_t)(qwf_Context_t *pCtx,
                                               qwf_uicc_Event_t *pEvent,
                                               void *pClientData);


/**
 * @brief Register a callback for the UICC service.
 *
 * @param [in]  pCtx  Pointer to Service context.
 * @param [in]  eventHandler   The callback to handle cellular data events.
 * @param [in]  pClientData  Pointer to client's data to be used with this
 *             event handler Note: this is optional to allow client to
 *             associate an extra data structure with this context. CDS
 *             service simply returns this pointer back to the client. NULL is
 *             a valid value indicating not used.
 *
 * @return @c UICC_OK               Successful result
 * @return @c UICC_ERR_INACTIVE     Service inactive
 * @return @c UICC_ERR_INT          Internal Error
 * @return @c UICC_ERR_PARAM        Invalid parameter
 *
 */
qwf_uicc_Results_e qwf_uicc_RegisterCallback(const qwf_Context_t * const pCtx,
                                             qwf_uicc_EventClientCallback_t eventHandler,
                                             void *pClientData);

/**
 * @brief Request UICC service to return the Status of the UICC
 *
 * This function returns the various status elements of the UICC. Status data
 * returning include the SIM State, SIM presence which indicates whether the
 * SIM is physically detected, and also additional information related to the
 * UICC State.
 * The UICC Status data is returned to the client via the provided reference
 * pointer to a structure of type @c qwf_uicc_UiccStatus_t
 *
 * @param [in]   pCtx  Pointer to Service context.
 * @param [out]  pUiccStatus  Pointer to structure to store the UICC Status
 *                              data
 *
 * @return @c UICC_OK               Successful result
 * @return @c UICC_ERR_INACTIVE     Service inactive or not ready
 * @return @c UICC_ERR_INT          Internal Error
 * @return @c UICC_ERR_PARAM        Invalid parameter
 *
 */
qwf_uicc_Results_e qwf_uicc_GetUiccStatus(const qwf_Context_t * const pCtx,
                                          qwf_uicc_UiccStatus_t * const pUiccStatus);

/**
 * @brief Request UICC service to return the Type of the UICC
 *
 * This function returns the type of the UICC as defined by the bitmapped
 * fields in @c qwf_uicc_Types_e. More than one type can be set for the UICC.
 *
 * @param [in]   pCtx       Pointer to Service context.
 * @param [out]  pUiccType  Reference pointer provided by client to store UICC
 *                          Type
 *
 * @return @c UICC_OK               Successful result
 * @return @c UICC_ERR_INACTIVE     Service inactive or not ready
 * @return @c UICC_ERR_PARAM        Invalid parameter
 *
 */
qwf_uicc_Results_e qwf_uicc_GetUiccType(const qwf_Context_t * const pCtx,
                                        qwf_uicc_Types_e * const pUiccType);

/**
 * @brief Request UICC service to return Subscriber Information for SIM
 *
 * This function returns the various carrier information elements of the UICC.
 * Subscriber information data returned include
 *
 * The Subscriber Information data is returned to the client via the provided
 * reference pointer to a structure of type @c qwf_uicc_SubscriberInfo_t
 *
 * @param [in]   pCtx  Pointer to Service context.
 * @param [out]  pSubscriberInfo  Reference pointer to structure to store the
 *                              Subscriber Information
 *
 * @return @c UICC_OK               Successful result
 * @return @c UICC_ERR_INACTIVE     Service inactive or not ready
 * @return @c UICC_ERR_INT          Internal Error
 * @return @c UICC_ERR_PARAM        Invalid parameter
 *
 */
qwf_uicc_Results_e qwf_uicc_GetSubscriberInfo(const qwf_Context_t * const pCtx,
                                              qwf_uicc_SubscriberInfo_t * const pSubscriberInfo);

/**
 * @brief Request UICC service to return CHV status information for specified
 *        CHV (1 or 2)
 *
 * This function returns the current status for the Card Holder Verification
 * (CHV) PIN. Information returned include "Enabled" and "Blocked" settings
 * and the number of unlock and unblock retries remaining.
 *
 * The CHV data is returned to the client via the provided reference pointer
 * to a structure of type @c qwf_uicc_ChvStatus_t
 *
 * @param [in]   pCtx  Pointer to Service context.
 * @param [in]   chvNumber  CHV to get status for (1 or 2)
 * @param [out]  pChvStatus  Pointer to structure to store the CHV Status
 *                          information
 *
 * @return @c UICC_OK               Successful result
 * @return @c UICC_ERR_INACTIVE     Service inactive or not ready
 * @return @c UICC_ERR_INT          Internal Error
 * @return @c UICC_ERR_PARAM        Invalid parameter
 *
 */
qwf_uicc_Results_e qwf_uicc_GetChvPinStatus(const qwf_Context_t * const pCtx,
                                            qwf_uicc_ChvNumbers_e chvNumber,
                                            qwf_uicc_ChvStatus_t * const pChvStatus);

/**
 * @brief Request UICC service to enable the PIN lock for CHV1
 *
 * This function enables or turns on the PIN (Personal Identification Number)
 * lock functionality for the device. When enabled a PIN code for Card Holder
 * Verification 1 (CHV1) must be entered to allow access to the device SIM.
 * When enabled the device must be "unlocked" by providing the correct PIN
 * Code via qwf_uicc_UnlockSim() interface. Note: the PIN code is also needed
 * to enable PIN lock. Note: Entering an incorrect PIN code will cause the
 * device PIN to be Blocked once the number or retry attempts is exhausted. If
 * blocked the device PIN must be unblocked using qwf_uicc_UnblockPin().
 *
 * @param [in]   pCtx  Pointer to Service context.
 * @param [in]   pPinCode  Password to Enable PIN lock.
 *
 * @return @c UICC_OK               Successful result
 * @return @c UICC_ERR_INACTIVE     Service inactive or not ready
 * @return @c UICC_ERR_INT          Internal Error
 * @return @c UICC_ERR_PARAM        Invalid parameter
 * @return @c UICC_ERR_PIN_INVALID  PIN not valid (PinCode wrong or PIN is
 *                                  locked and in CHV Requested state)
 *
 */
qwf_uicc_Results_e qwf_uicc_EnablePin1(const qwf_Context_t * const pCtx,
                                       const char * const pPinCode);

/**
 * @brief Request UICC service to disable the PIN lock for CHV1
 *
 * This function disables or turns off the PIN (Personal Identification
 * Number) lock functionality for the device. When disabled, the device is not
 * locked and no PIN code is required to allow access to the device SIM.
 * Note: The PIN code is also needed to disable PIN lock. Note: Entering an
 * incorrect PIN code will cause the device PIN to be Blocked once the number
 * or retry attempts is exhausted. If blocked the device PIN must be unblocked
 * using @c qwf_uicc_UnblockPin().
 *
 * @param [in]   pCtx  Pointer to Service context.
 * @param [in]   pPinCode  Password (string) to Enable PIN lock.
 *
 * @return @c UICC_OK               Successful result
 * @return @c UICC_ERR_INACTIVE     Service inactive or not ready
 * @return @c UICC_ERR_INT          Internal Error
 * @return @c UICC_ERR_PARAM        Invalid parameter
 * @return @c UICC_ERR_PIN_INVALID  PIN not valid (PinCode wrong or PIN is
 *                                  locked and in CHV Requested state)
 *
 */
qwf_uicc_Results_e qwf_uicc_DisablePin1(const qwf_Context_t * const pCtx,
                                        const char * const pPinCode);

/**
 * @brief Request UICC service to Unlock the PIN Lock using the password for
 *        specified Card Holder Verification (CHV) number
 *
 * This function unlocks the PIN (Personal Identification Number) lock
 * functionality for the device. Note: if an incorrect password is used, the
 * PIN Lock functionality will become "blocked" and cannot be unlocked using
 * the PIN code until reset via the qwf_uicc_UnblockPin() interface.
 * Note: Entering an incorrect PIN code will cause the device PIN to be
 * Blocked once the number or retry attempts is exhausted. If blocked the
 * device PIN must be unblocked using qwf_uicc_UnblockPin().
 *
 * @param [in]   pCtx  Pointer to Service context.
 * @param [in]   chvNumber Specifies which CHV to use (1 or 2).
 * @param [in]   pPinCode  Password (string) to Enable PIN lock.
 *
 * @return @c UICC_OK               Successful result
 * @return @c UICC_ERR_INACTIVE     Service inactive or not ready
 * @return @c UICC_ERR_INT          Internal Error
 * @return @c UICC_ERR_PARAM        Invalid parameter
 * @return @c UICC_ERR_PIN_INVALID  Incorrect PIN provided
 */
qwf_uicc_Results_e qwf_uicc_UnlockPin(const qwf_Context_t * const pCtx,
                                      qwf_uicc_ChvNumbers_e chvNumber,
                                      const char * const pPinCode);

/**
 * @brief Request UICC service to reset the PIN for specified CHV
 *
 * This function resets the PIN (Personal Identification Number) for the
 * specified Card Holder Verification (CHV) number which has become blocked
 * (no longer able to be unlocked) due to too many failed attempts to unlock.
 * To unblock the PIN, the device PIN Unlock Key (PUK) is used as a password,
 * and a new PIN Password is created. Note: The PUK code can only be obtained
 * from the network operator and is specific for the device SIM. Unlike the
 * PIN code, the PUK code can not be changed on the device.
 *
 * @param [in]   pCtx  Pointer to Service context.
 * @param [in]   chvNumber Specifies which CHV to use (1 or 2).
 * @param [in]   pPukCode  PIN Unlock Key (string) to Unblock PIN.
 * @param [in]   pNewPinCode  New Password (string) to Enable PIN lock.
 *
 * @return @c UICC_OK               Successful result
 * @return @c UICC_ERR_INACTIVE     Service inactive or not ready
 * @return @c UICC_ERR_INT          Internal Error
 * @return @c UICC_ERR_PARAM        Invalid parameter
 * @return @c UICC_ERR_PIN_INVALID  Operation not allowed -PUK not valid
 *
 */
qwf_uicc_Results_e qwf_uicc_UnblockPin(const qwf_Context_t * const pCtx,
                                       qwf_uicc_ChvNumbers_e chvNumber,
                                       const char * const pPukCode,
                                       const char * const pNewPinCode);

/**
 * @brief Request UICC service to change the PIN for specified CHV
 *
 * This function changes the PIN (Personal Identification Number) for the
 * specified Card Holder Verification (CHV) number.
 * Note: Entering an incorrect PIN code will cause the device PIN to be
 * Blocked once the number or retry attempts is exhausted. If blocked the
 * device PIN must be unblocked using qwf_uicc_UnblockPin().
 *
 * @param [in]   pCtx  Pointer to Service context.
 * @param [in]   chvNumber Specifies which CHV to use (1 or 2).
 * @param [in]   pOldPinCode  Current Password (string) to Enable PIN lock
 * @param [in]   pNewPinCode  New Password (string) to Enable PIN lock.
 *
 * @return @c UICC_OK               Successful result
 * @return @c UICC_ERR_INACTIVE     Service inactive or not ready
 * @return @c UICC_ERR_INT          Internal Error
 * @return @c UICC_ERR_PARAM        Invalid parameter
 * @return @c UICC_ERR_PIN_INVALID  Operation not allowed -PIN not valid
 *
 */

qwf_uicc_Results_e qwf_uicc_ChangePin(const qwf_Context_t * const pCtx,
                                      qwf_uicc_ChvNumbers_e chvNumber,
                                      const char * const pOldPinCode,
                                      const char * const pNewPinCode);


/**
 * @brief Request UICC service to return file information from specified EF
 *
 * This function requests UICC service to get the file information for the
 * specified UICC elementary file (EF). EF file information is returned to
 * client using reference pointer to @c qwf_uicc_ElementaryFileInfo_t data
 * structure.
 *
 * @param [in]   pCtx  Pointer to Service context.
 * @param [in]   efName  Name or ID of the EF to get Information on
 * @param [out]  pEfInfo Reference pointer provided by client to store file
 *                      info data
 *
 * @return @c UICC_OK                  Successful result
 * @return @c UICC_ERR_INACTIVE        Service inactive or not ready
 * @return @c UICC_ERR_INT             Internal Error
 * @return @c UICC_ERR_PARAM           Invalid parameter
 * @return @c UICC_ERR_CHV_BLOCKED     CHV is Blocked
 * @return @c UICC_ERR_EF_ID_INVALID   EF ID is invalid
 * @return @c UICC_ERR_EF_NOT_PRESENT  EF File Not Present
 *
 */
qwf_uicc_Results_e qwf_uicc_GetFileInfo(const qwf_Context_t * const pCtx,
                                        qwf_uicc_ElementaryFile_e efName,
                                        qwf_uicc_ElementaryFileInfo_t * const pEfInfo);

/**
 * @brief Request UICC service to read a record of data from specified EF
 *
 * This function requests UICC service to read the requested record from from
 * the specified UICC elementary file (EF). EF file information is returned to
 * client using reference pointer to @c qwf_uicc_ElementaryFileInfo_t data
 * structure.
 *
 * @param [in]   pCtx  Pointer to Service context.
 * @param [in]   efName  Name or ID of the EF to read data from
 * @param [in]   recordNum The index of the EF record to read.
 * @param [in]   recordLength The length of the EF record.
 * @param [out]  pData Reference pointer to buffer provided by client to copy
 *                      the data (bytes) read from the requested file. (Note:
 *                      the user must allocate enough memory in buffer to
 *                      store the requested data)
 *
 * @return @c UICC_OK                  Successful result
 * @return @c UICC_ERR_INACTIVE        Service inactive or not ready
 * @return @c UICC_ERR_INT             Internal Error
 * @return @c UICC_ERR_PARAM           Invalid parameter
 * @return @c UICC_ERR_CHV_BLOCKED     CHV is Blocked
 * @return @c UICC_ERR_EF_ID_INVALID   EF ID is invalid
 * @return @c UICC_ERR_EF_NOT_PRESENT  EF File Not Present
 */
qwf_uicc_Results_e qwf_uicc_ReadFile(const qwf_Context_t * const pCtx,
                                     qwf_uicc_ElementaryFile_e efName,
                                     int recordNum,
                                     int recordLength,
                                     unsigned char * const pData);

/**
 * @brief Request UICC service to Enable Fixed Dialing Number
 *
 * This function requests UICC service to Enable Fixed Dialing Number (FDN)
 * which then limits the device to only make calls to the numbers contained
 * within the FDN Phonebook. Client must provide the 2nd Card Holder
 * Verification (CHV2) password to enable FDN.
 *
 * @param [in]   pCtx  Pointer to Service context.
 * @param [in]   pChv2Code  CHV2 password (NULL terminated string) to enable
 *                          FDN.
 *
 * @return @c UICC_OK               Successful result
 * @return @c UICC_ERR_INACTIVE     Service inactive or not ready
 * @return @c UICC_ERR_INT          Internal Error
 * @return @c UICC_ERR_PARAM        Invalid parameter
 * @return @c UICC_ERR_PIN_INVALID  Operation not allowed - PIN not valid
 *
 */
qwf_uicc_Results_e qwf_uicc_EnableFdn(const qwf_Context_t * const pCtx,
                                      const char * const pChv2Code);

/**
 * @brief Request UICC service to Disable Fixed Dialing Number
 *
 * This function requests UICC service to disable Fixed Dialing Number (FDN).
 * Client must provide the 2nd Card Holder Verification (CHV2) password to
 * disable FDN.
 *
 * @param [in]   pCtx  Pointer to Service context.
 * @param [in]   pChv2Code  CHV2 password (NULL terminated string) to disable
 *                          FDN.
 *
 * @return @c UICC_OK               Successful result
 * @return @c UICC_ERR_INACTIVE     Service inactive or not ready
 * @return @c UICC_ERR_INT          Internal Error
 * @return @c UICC_ERR_PARAM        Invalid parameter
 * @return @c UICC_ERR_PIN_INVALID  Operation not allowed - PIN not valid
 *
 */
qwf_uicc_Results_e qwf_uicc_DisableFdn(const qwf_Context_t * const pCtx,
                                       const char * const pChv2Code);


/**
 * @brief Request UICC service to return file information for specified
 *        Phonebook file
 *
 * This function requests UICC service to get the file information for the
 * specified Phonebook file. Phonebook file information is returned to client
 * using reference pointer to @c qwf_uicc_ElementaryFileInfo_t data structure.
 *
 * @param [in]   pCtx  Pointer to Service context.
 * @param [in]   pbType  Name or type of the phonebook to get Information on
 * @param [out]  pPbInfo Reference pointer provided by client to store file
 *        info data
 *
 * @return @c UICC_OK           Successful result
 * @return @c UICC_ERR_INACTIVE Service inactive or not ready
 * @return @c UICC_ERR_INT      Internal Error
 * @return @c UICC_ERR_PARAM    Invalid parameter
 *
 */
qwf_uicc_Results_e qwf_uicc_GetPhonebookInfo(const qwf_Context_t * const pCtx,
                                             qwf_uicc_PhonebookType_e pbType,
                                             qwf_uicc_PhonebookFileInfo_t * const pPbInfo);

/**
 * @brief Request UICC service to read a record of data from specified
 *        Phonebook file
 *
 * This function requests UICC service to read the requested record from the
 * specified UICC Phonebook file. Phonebook record is returned to client using
 * reference pointer to @c qwf_uicc_PhonebookEntryData_t data structure.
 *
 * @param [in]   pCtx  Pointer to Service context.
 * @param [in]   pbType  Name or ID of the phonebook to get record from
 * @param [in]   recordNum The index of the phonebook record to read.
 * @param [out]  pData Reference pointer provided by client to copy the data
 *                      read from the requested file.
 *
 * @return @c UICC_OK                  Successful result
 * @return @c UICC_ERR_INACTIVE        Service inactive or not ready
 * @return @c UICC_ERR_INT             Internal Error
 * @return @c UICC_ERR_PARAM           Invalid parameter
 * @return @c UICC_ERR_CHV_BLOCKED     Operation not allowed - CHV is Blocked
 * @return @c UICC_ERR_EF_NOT_PRESENT  File or Record Not Present
 *
 */
qwf_uicc_Results_e qwf_uicc_ReadPhonebookEntry(const qwf_Context_t * const pCtx,
                                               qwf_uicc_PhonebookType_e pbType,
                                               int recordNum,
                                               qwf_uicc_PhonebookEntryData_t * const pData);

/**
 * @brief Request UICC service to update a record of data within the specified
 *        Phonebook file
 *
 * This function requests UICC service to update the requested record within
 * the specified UICC Phonebook file. Phonebook record data, including the
 * phonebook type and record to update, is provided by client using pointer to
 * @c qwf_uicc_PhonebookEntryData_t data structure.
 * Note: If the entry does not exist, it will be created within the phonebook
 * file.
 *
 * @param [in]  pCtx  Pointer to Service context.
 * @param [in]  pData Pointer to data structure containing updated phonebook
 *                    entry.
 *
 * @return @c UICC_OK                  Successful result
 * @return @c UICC_ERR_INACTIVE        Service inactive or not ready
 * @return @c UICC_ERR_INT             Internal Error
 * @return @c UICC_ERR_PARAM           Invalid parameter
 * @return @c UICC_ERR_PIN_INVALID     Operation not allowed - PIN not valid
 * @return @c UICC_ERR_CHV_BLOCKED     Operation not allowed - CHV is Blocked
 * @return @c UICC_ERR_EF_NOT_PRESENT  File or Record Not Present
 *
 */
qwf_uicc_Results_e qwf_uicc_UpdatePhonebookEntry(const qwf_Context_t * const pCtx,
                                                 const qwf_uicc_PhonebookEntryData_t * const pData);

/**
 * @brief Request UICC service to delete a record of data from specified
 *        Phonebook file
 *
 * This function requests UICC service to delete the requested record from the
 * specified UICC Phonebook file. Phonebook record is returned to client using
 * reference pointer to @c qwf_uicc_PhonebookEntryData_t data structure.
 *
 * @param [in]   pCtx  Pointer to Service context.
 * @param [in]   pbType  Name or ID of the phonebook to get record from
 * @param [in]   recordNum The index of the phonebook record to read.
 *
 * @return @c UICC_OK                  Successful result
 * @return @c UICC_ERR_INACTIVE        Service inactive or not ready
 * @return @c UICC_ERR_INT             Internal Error
 * @return @c UICC_ERR_PARAM           Invalid parameter
 * @return @c UICC_ERR_PIN_INVALID     Operation not allowed - PIN not valid
 * @return @c UICC_ERR_CHV_BLOCKED     Operation not allowed - CHV is Blocked
 * @return @c UICC_ERR_EF_NOT_PRESENT  File or Record Not Present
 *
 */
qwf_uicc_Results_e qwf_uicc_DeletePhonebookEntry(const qwf_Context_t * const pCtx,
                                                 qwf_uicc_PhonebookType_e pbType,
                                                 int recordNum);


/**
 * @brief Requests the UICC ONS service to get the network name
 *
 * This function requests the UICC ONS service to get the network name based
 * on the inputs of MCC, MNC and roaming status. This function uses the ONS
 * logic to resolve the network names based on:
 *  - EF OPL/PNN, SPN, SPDI, CPHS ON, CPHS ONS
 *  - NITZ Names
 *  - SE13 Table
 *  - MCC-MNC
 *  .
 *
 * @param [in]   pCtx  Pointer to Service context.
 * @param [in]   pMccStr  Pointer to Mobile Country Code string
 * @param [in]   pMncStr  Pointer to Mobile Network Code string
 * @param [in]   lac  Locaton Area Code
 * @param [in]   homePlmn  True if PLMN is home, false if roaming
 * @param [out]  pNetworkNames Reference pointer provided by client to return
 *                          the short and long network name strings
 *
 * @return @c UICC_OK           Successful result
 * @return @c UICC_ERR_INACTIVE Service inactive or not ready
 * @return @c UICC_ERR_INT      Internal Error
 * @return @c UICC_ERR_PARAM    Invalid parameter
 *
 */
qwf_uicc_Results_e qwf_uicc_GetOnsNetworkName(const qwf_Context_t * const pCtx,
                                              const char * const pMccStr,
                                              const char * const pMncStr,
                                              int lac,
                                              bool homePlmn,
                                              qwf_uicc_NetworkNames_t * const pNetworkNames);

/**
 * @brief Requests the UICC service to get the network name from SE13 table
 *
 * This function requests the UICC ONS service to get the network name based
 * on the inputs of MCC, MNC and roaming status. This function uses the SE13
 * table to find the corresponding network names
 *
 * @param [in]   pCtx  Pointer to Service context.
 * @param [in]   pMccStr  Pointer to Mobile Country Code string
 * @param [in]   pMncStr  Pointer to Mobile Network Code string
 * @param [out]  pNetworkNames Reference pointer provided by client to return
 *                             the short and long network name strings
 *
 * @return @c UICC_OK           Successful result
 * @return @c UICC_ERR_INACTIVE Service inactive or not ready
 * @return @c UICC_ERR_INT      Internal Error
 * @return @c UICC_ERR_PARAM    Invalid parameter
 *
 */
qwf_uicc_Results_e qwf_uicc_GetNetworkName(const qwf_Context_t * const pCtx,
                                           const char * const pMccStr,
                                           const char * const pMncStr,
                                           qwf_uicc_NetworkNames_t * const pNetworkNames);

/**
 * @brief Sets the MSISDN for the device.
 *
 * This function sets the Mobile Station International Subscriber Directory
 *        Number (MSISDN)for the device.
 *
 * @param [in]   pCtx  Pointer to Service context.
 * @param [in]   pPhoneNumber Pointer to string containing the MSISDN Phone
 *                            Number
 *
 * @return @c UICC_OK           Successful result
 * @return @c UICC_ERR_INACTIVE Service inactive or not ready
 * @return @c UICC_ERR_INT      Internal Error
 * @return @c UICC_ERR_PARAM    Invalid parameter
 *
 */
qwf_uicc_Results_e qwf_uicc_SetMSISDN(const qwf_Context_t * const pCtx,
                                      const char * const pPhoneNumber);

/** @}*/ // end group QWF_UICC

__END_DECLS

#endif /* _QWF_UICC_H_INCLUDED */

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/wireless_framework/branches/7.1/trunk/common/lib/qwf_interface/public/qwf/qwf_uicc.h $ $Rev: 2958 $")
#endif

