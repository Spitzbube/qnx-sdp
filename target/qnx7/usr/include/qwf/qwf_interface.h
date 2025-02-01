/*
 * $QNXLicenseC:
 * Copyright 2015-2020, QNX Software Systems. All Rights Reserved.
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
 * @brief  This interface is the point of entry to all the QNX Wireless
 *         Framework APIs.
 *
 *         Top level public interface to QNX Wireless Framework APIs.
 *          Provides interface functions common to all the QWF components
 *          (Cellular Data Services, Radio Control Services, UICC Services,
 *          SMS and Voice services)
 *
 *          Usage:
 *          - Client application calls qwf_Init() to initialize the QWF
 *            Context data used as container for QWF and service data (like
 *            CDS)
 *          - Client application calls the service specific function to
 *            register to receive events from that service via a callback
 *            function (like qwf_cds_RegisterCallback() )
 *            @b NOTE: callbacks will be registered for each service the
 *            application is interested in.
 *          - Client application calls the qwf_ProcessEvents() function to
 *            block and wait for events to be sent to each services callback
 *            which was registered during initialization
 *          - The client application processes the events as required.
 *
 *
 */

#ifndef _QWF_INTERFACE_H_INCLUDED
#define _QWF_INTERFACE_H_INCLUDED

#include  <inttypes.h>

__BEGIN_DECLS

/**
 * @brief QWF service types
 *
 * This enumeration type defines the active QWF services
 */
typedef enum
{
    QWF_SERVICE_NONE  = 0x00000000,    ///< No active QWF service
    QWF_SERVICE_UICC  = 0x00000001,    ///< UICC service is active
    QWF_SERVICE_RC    = 0x00000002,    ///< Radio control service is active
    QWF_SERVICE_CDS   = 0x00000004,    ///< Cellular data service is active
    QWF_SERVICE_SMS   = 0x00000008,    ///< SMS service is active
    QWF_SERVICE_VOICE = 0x00000010,    ///< Voice service is active
    QWF_SERVICE_NET   = 0x00000020,    ///< Networking service is active
    QWF_SERVICE_LCS   = 0x00000040,    ///< LCS service is active
    QWF_SERVICE_WIFI  = 0x00000080,    ///< Wi-Fi service is active
    QWF_SERVICE_ALL   = 0x000000ff     ///< All possible services
} qwf_Service_e;

#define QWF_SERVICE_CELLULAR    (QWF_SERVICE_UICC + QWF_SERVICE_RC + QWF_SERVICE_CDS)
#define QWF_SERVICE_NETWORKING  (QWF_SERVICE_WIFI + QWF_SERVICE_NET)

/**
 * @brief QWF return status
 *
 * This enumeration type defines the possible QWF return status
 */
typedef enum
{
    RETURN_STATUS_SUCCESS,         ///< QWF returns success
    RETURN_STATUS_FAILURE          ///< QWF returns failure
} qwf_ReturnStatus_e;

/**
 * @brief An opaque data type representing the service context
 *
 * @details The context is used by this API to do its work. Context is created
 * by calling the qwf_Init() function and properly disposed of in the
 * qwf_Cleanup() function
 */
typedef struct qwf_Context_s qwf_Context_t;


/**
 * @brief Initialize QWF API Interface
 *
 * @details This function must be executed successfully before any service
 * information can be accessed using this API. The function returns a service
 * context that needs to be passed to all QWF API function calls.
 * This interface creates and initializes both the API's event handling core as
 * well each individual service (e.g. UICC, CDS, Wi-Fi, etc.).
 *
 * If not using all services, individual services can be initialized by using
 * the qwf_IntializeCore() and qwf_InitializeServices() interfaces instead.
 *
 * qwf_Init() is only called once. If qwf_Init() fails, it is because an
 * unrecoverable error (e.g., memory issue) occurs and QWF system needs to be
 * rebooted.
 *
 * @param[out] ppCtx Returned pointer to the service context.
 *
 * @return ::RETURN_STATUS_SUCCESS if successful
 * @return ::RETURN_STATUS_FAILURE otherwise.
 *
 */
qwf_ReturnStatus_e qwf_Init(qwf_Context_t **ppCtx);

/**
 * @brief Deallocate API resources
 *
 * @details This function frees all resources belonging to this API. The
 * service context will be destroyed and event handles, if any, will become
 * obsolete. No more service event notifications will be generated after this
 * function returns. The function only needs to be called once, calling multiple
 * times with same service context pointer will have unexpected results.
 *
 * @param[in] pCtx Pointer to the service context.
 */
void qwf_Cleanup(qwf_Context_t *pCtx);

/**
 * @brief Initialize QWF API's core event handling memory and queues
 *
 * @details This function is intended to be used with the
 * qwf_InitializeServices() interface when the user intent is to run a subset of
 * the QWF services. The qwf_InitializeCore() interface can be used instead of
 * qwf_init(), however, unlike qwf_Init(), the qwf_InitializeServices()
 * interface must be used to complete the initialization of individual services.
 * Both qwf_InitializeCore() and qwf_InitializeServices() interfaces must be
 * executed successfully before any service information can be accessed using
 * the QWF API. The function returns a service context that needs to be passed
 * to all QWF API function calls. This interface creates and intializes only the
 * API's event handling core.
 *
 * qwf_InitializeCore() is only called once. If qwf_InitializeCore() fails, it
 * is because an unrecoverable error (e.g., memory issue) occurs and QWF system
 * needs to be rebooted.
 *
 * @param[out] ppCtx Returned pointer to the service context.
 *
 * @return ::RETURN_STATUS_SUCCESS if successful
 * @return ::RETURN_STATUS_FAILURE otherwise.
 *
 */
qwf_ReturnStatus_e qwf_InitializeCore(qwf_Context_t **ppCtx);

/**
 * @brief Initialize a subset of the QWF API service interfaces
 *
 * @details This function is intended to be used with the
 * qwf_InitializeCore() interface when the user intent is to run a subset of
 * the QWF services. Unlike qwf_Init(), which initialize ALL the service
 * interfaces (e.g. UICC, CDS, Wi-Fi, etc.), the qwf_InitializeServices()
 * interface can be used to complete the intialization of individual services
 * (e.g. only Wi-Fi and Networking) as defined in ::qwf_Service_e.
 * Both qwf_InitializeCore() and qwf_InitializeServices() interfaces must be
 * executed successfully before any service information can be accessed using
 * the QWF API. The qwf_InitializeServices() interface must be called after a
 * valid service context is obtained from qwf_InitializeCore. The
 * qwf_InitializeServices() interface can be called multiple times with
 * different services or after a service has been terminated using the
 * qwf_CleanupServices() interface.
 * If qwf_InitializeServices() fails, it is because an unrecoverable error
 * (e.g., memory issue) occurs and QWF system needs to be rebooted.
 *
 * @param[in] pCtx Pointer to the service context.
 * @param[in] servicesMask A bit mask of services to be initialized.
 *
 * @return ::RETURN_STATUS_SUCCESS if successful
 * @return ::RETURN_STATUS_FAILURE otherwise.
 *
 */
qwf_ReturnStatus_e qwf_InitializeServices(qwf_Context_t *pCtx,
                                          qwf_Service_e servicesMask);
/**
 * @brief Deallocate API resources for specified services
 *
 * @details This function frees the resources belonging to the specified
 * services as defined in ::qwf_Service_e. The QWF core context will be remain
 * intact but the interface to specified services will be removed. No more event
 * notifications will be generated for the specified services after calling this
 * function. The function only needs to be called once but can be called
 * multiple times for different services.
 * To remove the QWF core context and all service resources, the qwf_cleanup()
 * interface can be used.
 *
 * @param[in] pCtx Pointer to the cellular service context.
 * @param[in] servicesMask A bit mask of services to be cleaned up.
 *
 */
void qwf_CleanupServices(qwf_Context_t *pCtx,
                         qwf_Service_e servicesMask);

/**
 * @brief Process event indication received at the handle
 *
 * @details This function will process all QWF events currently in its queue and
 * the associated client callback functions will be invoked.
 * The function blocks waiting to process events and only exits if error
 * detected or when the QWF interface is shutdown via QWF_Cleanup().
 *
 * @param[in]  pCtx Pointer to the cellular service context.
 *
 * @return ::RETURN_STATUS_SUCCESS Normal termination due to QWF shutdown via
 *         qwf_Cleanup()
 * @return ::RETURN_STATUS_FAILURE otherwise
 */
qwf_ReturnStatus_e qwf_ProcessEvents(qwf_Context_t *pCtx);

/**
 * @brief Query active QWF services
 *
 * @details This function queries the currently running QWF services.
 *
 * @param[in] pCtx Pointer to the cellular service context.
 *
 * @return bitmap of the active QWF services.
 *
 * @see ::qwf_Service_e
 */
qwf_Service_e qwf_QueryActiveService(qwf_Context_t *pCtx);

__END_DECLS

#endif /* _QWF_INTERFACE_H_INCLUDED */


#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/wireless_framework/branches/7.1/trunk/common/lib/qwf_interface/public/qwf/qwf_interface.h $ $Rev: 3098 $")
#endif
