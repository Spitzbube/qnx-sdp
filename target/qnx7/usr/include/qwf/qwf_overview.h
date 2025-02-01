/*
 * $QNXLicenseC:
 * Copyright 2016-2017 QNX Software Systems. All Rights Reserved.
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
 * @brief QNX Wireless Framework Public C API Overview and Code Examples
 *
 * The QNX Wireless Framework (QWF) provides reliable cellular (voice and
 * data), Wi-Fi connectivity and access to location services (LCS/GPS) on top of
 * QNX Neutrino. This framework allows clients to manage all aspects of modem
 * and cellular network functionality, Wi-Fi network functionality and location
 * services through a high-level, feature-rich API.
 */

/**
 * @mainpage QNX Wireless Framework Public C API Overview and Code Examples
 *
 * @section QWF_OVERVIEW QNX Wireless Framework Public C API Overview
 *
 * The QNX Wireless Framework (QWF) provides reliable cellular (voice and
 * data), Wi-Fi connectivity and access to location services (LCS/GPS) on top of
 * QNX Neutrino. This framework allows clients to manage all aspects of modem
 * and cellular network functionality, Wi-Fi network functionality and location
 * services through a high-level, feature-rich API.
 *
 * Specifically, QWF provides the following services and corresponding public interfaces:
 * - Common (see @c qwf_interface.h)
 * - Cellular Radio Control (RC) Service (see @c qwf_rc.h)
 * - Cellular Universal Integrated Circuit Card (UICC) Service (see @c qwf_uicc.h)
 * - Cellular Short Message Service (SMS) Service (see @c qwf_sms.h)
 * - Cellular Voice Service (see @c qwf_voice.h)
 * - Cellular eCall Service (see @c qwf_ecall.h)
 * - Cellular Data Service (CDS) (see @c qwf_cds.h)
 * - Network Management Service (see @c qwf_net.h)
 * - Wi-Fi Management Service (see @c qwf_wifi.h)
 * - Location Service (see @c qwf_lcs.h)
 *
 * Each service provides configuration, control and event monitoring APIs. The APIs that expose the
 * functionality generally share naming conventions and usages.
 *
 * Detailed descriptions of each service and its interfaces are provided in each
 * interface's header file and in the generated API documentation.
 *
 * Detailed information regarding QWF architecture, services and use is available at
 * http://www.qnx.com/content/qnx/en/products/wireless_framework/index.html and
 * the "Developers" section at http://www.myqnx.com.  Specifically, refer to the
 * "QWF Architecture and Users Guide" and the rest of this QWF Public C API documentation.
 *
 * @section QWF_EXAMPLES QWF C API Code Examples
 *
 * Refer to the following for code examples:
 * - @ref QWF_EXAMPLES_RC
 * - @ref QWF_EXAMPLES_UICC
 * - @ref QWF_EXAMPLES_SMS
 * - @ref QWF_EXAMPLES_VOICE
 * - @ref QWF_EXAMPLES_ECALL
 * - @ref QWF_EXAMPLES_CDS
 * - @ref QWF_EXAMPLES_NET
 * - @ref QWF_EXAMPLES_WIFI
 * - @ref QWF_EXAMPLES_LCS
 *
 * To use a QWF service interface, use the @c -l @c qwf_interface option
 * to @c qcc to link against @c libqwf_interface.
 *
 * To compile against a QWF service's interface, include the service's header file listed above.
 *
 * @b Note: qwf_Init() must be called prior to using any QWF service
 *
 * @section QWF_EXAMPLES_RC Code examples for the Cellular Radio Control (RC) service
 *
 * @b Example for the following use cases:
 *    - Use the QWF RC service to implement an event handler
 *    - Use the QWF RC service to register an event handler
 *    - Use the QWF RC service to get the available cellular services from a services change event
 *
 * @code
 *    //
 *    // QWF RC service client's implementation
 *    //
 *
 *    #include <qwf/qwf_rc.h> // QWF RC service public API
 *
 *    static int _clientData1 = 0; // event handler client data 1
 *    static int _clientData2 = 0; // event handler client data 2
 *
 *    //
 *    // sample QWF RC service client's event handler
 *    //
 *    static void eventHandler(const qwf_Context_t * const pQwfCtx, const qwf_rc_Event_t event, void * const pClientData)
 *    {
 *       // handle event - see qwf_rc_EventGetType(), qwf_rc_EventGetChanges() and qwf_rc_EventGetpayload()
 *       printf("qwf rc event handler\n");
 *       qwf_rc_Return_e stickyRet = QWF_RC_RETURN_OK;
 *       qwf_rc_Return_e ret = QWF_RC_RETURN_OK;
 *
 *        do {
 *            if (NULL == pQwfCtx) {
 *                printf("qwf rc error - NULL qwf context pointer input\n");
 *                stickyRet = QWF_RC_RETURN_INVALID_ARG;
 *                break;
 *            }
 *
 *            if (NULL == event) {
 *                printf("qwf rc error - NULL event pointer input\n");
 *                stickyRet = QWF_RC_RETURN_INVALID_ARG;
 *                break;
 *            }
 *
 *            // print the event to the system log
 *            ret = qwf_rc_EventPrint(event);
 *
 *            if (QWF_RC_RETURN_OK != ret) {
 *                printf("qwf rc failed event print\n");
 *                stickyRet = ret;
 *            }
 *
 *            // get the event type
 *            qwf_rc_EventType_e type = QWF_RC_EVENT_TYPE_INVALID;
 *            ret = qwf_rc_EventGetType(pQwfCtx, event, &type);
 *
 *            if (QWF_RC_RETURN_OK != ret) {
 *                printf("qwf rc error - failed to get event type\n");
 *                stickyRet = ret;
 *            }
 *
 *            // print the event type
 *            switch (type) {
 *                case QWF_RC_EVENT_TYPE_CELL:
 *                    printf("qwf rc event type = cell\n");
 *                    break;
 *                case QWF_RC_EVENT_TYPE_RADIO:
 *                    printf("qwf rc event type = radio\n");
 *                    break;
 *                case QWF_RC_EVENT_TYPE_SIGNAL:
 *                    printf("qwf rc event type = signal\n");
 *                    break;
 *                case QWF_RC_EVENT_TYPE_STATE_INACTIVE:
 *                    printf("qwf rc event type = state\n");
 *                    break;
 *                case QWF_RC_EVENT_TYPE_INVALID:
 *                default:
 *                    printf("qwf rc error - event type = invalid (%d)\n", type);
 *                    stickyRet = QWF_RC_RETURN_ERROR;
 *                    break;
 *            }
 *
 *            // get the event changes
 *            qwf_rc_EventChanges_e changes = QWF_RC_EVENT_CHANGES_BITMAP_UNKNOWN;
 *            ret = qwf_rc_EventGetChanges(pQwfCtx, event, &changes);
 *
 *            if (QWF_RC_RETURN_OK != ret) {
 *                printf("qwf rc error - failed to get event changes\n");
 *                stickyRet = ret;
 *            }
 *
 *            if (QWF_RC_EVENT_CHANGES_BITMAP_UNKNOWN == changes) {
 *                printf("qwf rc error - unknown event changes\n");
 *                stickyRet = QWF_RC_RETURN_ERROR;
 *            }
 *
 *            if (QWF_RC_EVENT_CHANGES_BITMAP_NONE == changes) {
 *                printf("qwf rc error - no event changes\n");
 *                stickyRet = QWF_RC_RETURN_ERROR;
 *            }
 *
 *            if (changes & QWF_RC_EVENT_CHANGES_BITMAP_SERVING_CELL) {
 *                printf("qwf rc event change = serving cell\n");
 *            }
 *
 *            if (changes & QWF_RC_EVENT_CHANGES_BITMAP_RAT) {
 *                printf("qwf rc event change = rat\n");
 *            }
 *
 *            if (changes & QWF_RC_EVENT_CHANGES_BITMAP_SERVICES) {
 *                printf("qwf rc event change = services\n");
 *            }
 *
 *            if (changes & QWF_RC_EVENT_CHANGES_BITMAP_ROAMING) {
 *                printf("qwf rc event change = roaming\n");
 *            }
 *
 *            if (changes & QWF_RC_EVENT_CHANGES_BITMAP_POWER) {
 *                printf("qwf rc event change = power\n");
 *            }
 *
 *            if (changes & QWF_RC_EVENT_CHANGES_BITMAP_DEVICE_INFO) {
 *                printf("qwf rc event change = device info\n");
 *            }
 *
 *            if (changes & QWF_RC_EVENT_CHANGES_BITMAP_SIGNAL) {
 *                printf("qwf rc event change = signal\n");
 *            }
 *
 *            if (changes & QWF_RC_EVENT_CHANGES_BITMAP_STATE_INACTIVE) {
 *                printf("qwf rc event change = inactive\n");
 *            }
 *
 *            // check client data bounced back
 *            if (NULL != pClientData) {
 *                if ((pClientData == &_clientData1) && (*((int*)pClientData) == _clientData1)) {
 *                    printf("qwf rc bump client 1 data\n");
 *                    _clientData1++;
 *                }
 *                else if ((pClientData == &_clientData2) && (*((int*)pClientData) == _clientData2)) {
 *                    printf("qwf rc bump client 2 data\n");
 *                    _clientData2++;
 *                }
 *                else {
 *                    printf("qwf rc error - invalid event client data returned\n");
 *                    stickyRet = QWF_RC_RETURN_ERROR;
 *                    break;
 *                }
 *
 *                printf("qwf rc event handler client   data = %d*\n", *((int*)pClientData));
 *                printf("qwf rc event handler client 1 data = %d%s\n", _clientData1, (pClientData == &_clientData1) ? "*" : "");
 *                printf("qwf rc event handler client 2 data = %d%s\n", _clientData2, (pClientData == &_clientData2) ? "*" : "");
 *            }
 *            else {
 *                printf("no event client data returned\n");
 *                break;
 *            }
 *        }
 *        while (false);
 *
 *        // for instance, if we're interested in cellular services changes,
 *        //   and the event indicates the available cellular services changed,
 *        //   then get the event's payload (which we know to be a Cell object
 *        //   or could determine using qwf_rc_EventGetType()) and extract the
 *        //   services information from it
 *        if (changes & QWF_RC_EVENT_CHANGES_BITMAP_SERVICES) {
 *            void *pPayload = NULL;
 *            ret = qwf_rc_EventGetPayload(pQwfCtx, event, &pPayload);
 *            qwf_rc_Cell_t cell = (qwf_rc_Cell_t) pPayload;
 *
 *            if (QWF_RC_RETURN_OK != ret) {
 *                printf("error - failed to get event payload\n");
 *                stickyRet = ret;
 *            }
 *            else {
 *                qwf_rc_Services_e services = QWF_RC_SERVICES_BITMAP_UNKNOWN;
 *                ret = qwf_rc_CellGetServices(pQwfCtx, cell, &services);
 *
 *                if (QWF_RC_RETURN_OK != ret) {
 *                    printf("error - failed to get cell services\n");
 *                    stickyRet = ret;
 *                }
 *            }
 *        }
 *
 *        if (QWF_RC_RETURN_OK != stickyRet) {
 *            printf("error - failed to handle event\n");
 *        }
 *    }
 *
 *    //
 *    // sample QWF RC service client's main()
 *    //
 *    int main(int argc, char **argv)
 *    {
 *        // initialize the QWF services (see qwf_Init())
 *        //   *** done only once ***
 *        // qwf_init() must be called once to obtain the QWF service
 *        //   context, pQwfCtx, before any service APIs can be used
 *        qwf_Context_t *pQwfCtx = NULL;
 *        int err = qwf_Init(&pQwfCtx);
 *
 *        if ((EOK != err) || (NULL == pQwfCtx)) {
 *            // client error handling of failure to init QWF services
 *            return EXIT_FAILURE;
 *        }
 *
 *        // register event handler (no client data here).  if QWF_RC_RETURN_UNSUPPORTED
 *        //   is returned, a handler already is registered (see qwf_rc_EventRegisterHandler()
 *        //   for information on deregistering a handler)
 *        qwf_rc_Return_e ret = qwf_rc_EventRegisterHandler(pQwfCtx, eventHandler, NULL);
 *
 *        if (QWF_RC_RETURN_OK != ret) {
 *           // client error handling of failure to register event handler
 *        }
 *
 *        // maybe get the initial available services from the underlying QWF RC service
 *        //   NOTE - NULL used for the cell argument to qwf_rc_CellGetServices() allows
 *        //          clients to get current available services - see qwf_rc_CellGetServices()
 *        //          without first having to receive a relevant event
 *        qwf_rc_Services_e services = QWF_RC_SERVICES_BITMAP_UNKNOWN;
 *        ret = qwf_rc_CellGetServices(pQwfCtx, NULL, &services);
 *
 *        if (QWF_RC_RETURN_OK != ret) {
 *           // client error handling of failure get cell services
 *        }
 *
 *        // maybe more start-of-day stuff...
 *
 *        // enter into events processing loop...
 *        if (RETURN_STATUS_FAILURE == qwf_ProcessEvents(pQwfCtx)) {
 *            printf("qwf_ProcessEvents() failed\n");
 *            qwf_Cleanup(pQwfCtx);
 *            return EXIT_FAILURE;
 *        }
 *        else {
 *            qwf_Cleanup(pQwfCtx);
 *            return EXIT_SUCCESS;
 *        }
 *    }
 * @endcode
 *
 * @b Example for the following use cases:
 *    - Use the QWF RC service to turn on cellular radio
 *    - Use the QWF RC service to get the available cellular services
 *    - Use the QWF RC service to get the current cellular radio signal level
 *
 * @code
 *    #include <qwf/qwf_rc.h> // QWF RC service public API
 *
 *    ...
 *
 *    // turn on cellular radio (pQwfCtx from qwf_Init() call)
 *    qwf_rc_Return_e ret = qwf_rc_RadioSetPower(pQwfCtx, QWF_RC_POWER_ON);
 *
 *    if (QWF_RC_RETURN_OK != ret) {
 *       // error handling for failure to turn on cellular radio
 *    }
 *
 *    // get the current available services from the underlying QWF RC service
 *    // NOTE: NULL used for the cell argument to qwf_rc_CellGetServices() allows
 *    //       clients to get current available services without first having to
 *    //       receive a relevant event - see qwf_rc_CellGetServices()
 *    qwf_rc_Services_e services = QWF_RC_SERVICES_BITMAP_UNKNOWN;
 *    ret = qwf_rc_CellGetServices(pQwfCtx, NULL, &services);
 *
 *    if (QWF_RC_RETURN_OK != ret) {
 *       // client error handling of failure get cell services
 *    }
 *
 *    // get the current signal level RSSI
 *    // NOTE: again, NULL used for the radio argument to qwf_rc_SignalGetRssi()
 *    //       allows clients to get current signal level without first having
 *    //       to receive a relevant event - see qwf_rc_SignalGetRssi()
 *    int16_t rssi = QWF_RC_SIGNAL_INVALID;
 *    ret = qwf_rc_SignalGetRssi(pQwfCtx, NULL, &rssi);
 *
 *    if (QWF_RC_RETURN_OK != ret) {
 *       // client error handling of failure get signal RSSI
 *    }
 * @endcode
 *
 * @b Example for the following use cases:
 *    - Use the QWF RC service to process an event payload
 *    - Use the QWF RC service to get the available cellular services
 *    - Use the QWF RC service to get the current cellular radio signal level
 *
 * @code
 *    #include <qwf/qwf_rc.h>
 *
 *    void eventHandler(const qwf_Context_t  * const pQwfCtx,
 *                      const qwf_rc_Event_t         event,
 *                            void           * const pClientData);
 *    {
 *       // printing an event will print its payload
 *       qwf_rc_Return_e ret = qwf_rc_EventPrint(event);
 *
 *       if (QWF_RC_RETURN_OK != ret) {
 *           // error handling
 *       }
 *
 *       // get event type
 *       qwf_rc_EventType_e type = QWF_RC_EVENT_TYPE_INVALID;
 *       qwf_rc_Return_e ret = qwf_rc_EventGetType(pQwfCtx, event, &type);
 *
 *       if (QWF_RC_RETURN_OK != ret) {
 *           // error handling
 *       }
 *
 *       // get event payload
 *       void *pPayload = NULL;
 *       ret = qwf_rc_EventGetPayload(pQwfCtx, event, &pPayload);
 *
 *       if (QWF_RC_RETURN_OK != ret) {
 *          // error handling
 *       }
 *
 *       switch (type) {
 *          case QWF_RC_EVENT_TYPE_CELL: {
 *             qwf_rc_Cell_t cell = (qwf_rc_Cell_t) pPayload;
 *             ret = qwf_rc_CellPrint(cell);
 *
 *             if (QWF_RC_RETURN_OK != ret){
 *                // error handling
 *             }
 *
 *             // get current services
 *             qwf_rc_Services_e services = QWF_RC_SERVICES_BITMAP_NONE;
 *             ret = qwf_rc_CellGetServices(pQwfCtx, cell, &services);
 *
 *             if (QWF_RC_RETURN_OK != ret) {
 *                // error handling
 *             }
 *
 *             break;
 *          }
 *
 *          case QWF_RC_EVENT_TYPE_RADIO: {
 *             qwf_rc_Radio_t radio = (qwf_rc_Radio_t) pPayload;
 *             ret = qwf_rc_RadioPrint(radio);
 *
 *             if (QWF_RC_RETURN_OK != ret) {
 *               // error handling
 *             }
 *
 *             break;
 *          }
 *
 *          case QWF_RC_EVENT_TYPE_SIGNAL: {
 *             qwf_rc_Signal_t signal = (qwf_rc_Signal_t) pPayload;
 *             ret = qwf_rc_SignalPrint(signal);
 *
 *             if (QWF_RC_RETURN_OK != ret) {
 *                // error handling
 *             }
 *
 *             break;
 *          }
 *
 *          case QWF_RC_EVENT_TYPE_INVALID:
 *          default:
 *             // error handling
 *             break;
 *       }
 *
 *       // and if you want to know which data within
 *       // the event's payload have changed
 *       qwf_rc_EventChanges_e changes = QWF_RC_EVENT_CHANGES_BITMAP_NONE;
 *       ret = qwf_rc_EventGetChanges(_pQwfCtx, event, &changes);
 *
 *       if (QWF_RC_RETURN_OK != ret) {
 *           // error handling
 *       }
 *
 *       if (changes & QWF_RC_EVENT_CHANGES_BITMAP_SERVING_CELL) {
 *           // handle serving cell change
 *       }
 *
 *       if (changes & QWF_RC_EVENT_CHANGES_BITMAP_RAT) {
 *           // handle serving cell RAT change
 *       }
 *
 *       // ...and so on...
 *
 *       // NOTE - the event will be destroyed on return from this handler!
 *       return;
 *    }
 * @endcode
 *
 * @b Example for the following use cases:
 *    - Use the QWF RC service to get the available cellular services from the RC service without a cell object
 *    - Use the QWF RC service to get the available cellular services from a cell object received as an event payload
 *    - Use the QWF RC service to get the available cellular services from a newly created cell object
 *
 * @code
 *    #include <qwf/qwf_rc.h>
 *
 *    // initialize the QWF services (see qwf_Init())
 *    // *** only once ***
 *    qwf_Context_t *pQwfCtx = NULL;
 *    int err = qwf_Init(&pQwfCtx);
 *
 *    if (EOK != err) {
 *       // error handling for failure to init QWF services
 *    }
 *
 *    // get the current network services bitmap from the
 *    // underlying QWF RC service (NULL cell argument)
 *    qwf_rc_Services_e services = QWF_RC_SERVICES_BITMAP_NONE;
 *    qwf_rc_Return_e ret = qwf_rc_CellGetServices(pQwfCtx, NULL, &services);
 *
 *    if (QWF_RC_RETURN_OK != ret) {
 *       // error handling
 *    }
 *
 *    // you got a network cell object delivered as an event's
 *    // payload, and you want the available network services from it
 *    // (see qwf_rc_EventGetPayload())
 *    qwf_rc_Cell_t cell = (qwf_rc_Cell_t) payload;
 *    qwf_rc_Services_e services = QWF_RC_SERVICES_BITMAP_NONE;
 *    qwf_rc_Return_e ret = qwf_rc_CellGetServices(pQwfCtx, cell, &services);
 *
 *    if (QWF_RC_RETURN_OK != ret) {
 *       // error handling
 *    }
 *
 *    // or you'll the need a cell for other purposes, so create and init a new one
 *    qwf_rc_Cell_t cell = NULL;
 *    qwf_rc_Return_e ret = qwf_rc_CellInit(_pQwfCtx, &cell);
 *
 *    if (QWF_RC_RETURN_OK != ret){
 *       // error handling
 *    }
 *
 *    // and get the available network services from the new cell
 *    qwf_rc_Services_e services = QWF_RC_SERVICES_BITMAP_NONE;
 *    ret = qwf_rc_CellGetServices(_pQwfCtx, cell, &services);
 *
 *    if (QWF_RC_RETURN_OK != ret){
 *       // error handling
 *    }
 *
 *    // and maybe print it to the system log
 *    ret = qwf_rc_CellPrint(cell);
 *
 *    if (QWF_RC_RETURN_OK != ret){
 *       // error handling
 *    }
 *
 *    // but if you created it, don't forget to destroy it!
 *    ret = qwf_rc_CellDestroy(_pQwfCtx, &cell);
 *
 *    if (QWF_RC_RETURN_OK != ret){
 *       // error handling
 *    }
 * @endcode
 *
 * @b Example for the following use case:
 *    - Use the QWF RC service to get the current cellular serving cell from a newly created cell object
 *
 * @code
 *    #include <qwf/qwf_rc.h>
 *
 *    // initialize the QWF services (see qwf_Init())
 *    // *** done only once ***
 *    qwf_Context_t *pQwfCtx = NULL;
 *    int err = qwf_Init(&pQwfCtx);
 *
 *    if (EOK != err) {
 *       // error handling for failure to init QWF services
 *    }
 *
 *    // create a network cell object and initialize it
 *    // with the current data from the underlying QWF RC service
 *    qwf_rc_Cell_t cell = NULL;
 *    qwf_rc_Return_e ret = qwf_rc_CellInit(pQwfCtx, &cell);
 *
 *    if (QWF_RC_RETURN_OK != ret) {
 *       // error handling
 *    }
 *
 *    // get the network MCC/MNC from the
 *    // network cell object
 *    uint16_t mcc = QWF_RC_MCC_INVALID;
 *    uint16_t mnc = QWF_RC_MNC_INVALID;
 *    ret = qwf_rc_CellGetMccMnc(pQwfCtx, cell, &mcc, &mnc);
 *
 *    if (QWF_RC_RETURN_OK != ret) {
 *       // error handling
 *    }
 *
 *    // don't forget to destroy it when you're finished with it
 *    ret = qwf_rc_CellDestroy(pQwfCtx, &cell);
 *
 *    if (QWF_RC_RETURN_OK != ret) {
 *       // error handling
 *    }
 * @endcode
 *
 * @section QWF_EXAMPLES_UICC Code examples for the Cellular Universal Integrated Circuit Card (UICC) service
 *
 * @b Example for the following use case:
 *    - Use the QWF UICC service to get the operator name strings
 *
 * @code
 *    // Return Code for QWF UICC API call
 *    qwf_uicc_Results_e rc;
 *    // Structure to hold the subscriber/carrier information
 *    qwf_uicc_SubscriberInfo_t carrierInfo;
 *
 *    // Invoke QWF UICC API to Get Subscriber Information. Data is returned
 *    // in the qwf_uicc_SubscriberInfo_t structure defined above.
 *    // Assumes QWF UICC initialized and QWF Context stored in variable pCtx
 *    rc = qwf_uicc_GetSubscriberInfo(pCtx, &carrierInfo);
 *
 *    // Output the Long and Short Operator Name strings
 *    if (UICC_OK == rc) {
 *        printf("Current ONS Long name=%s, Current ONS Short name=%s\n",
 *                carrierInfo.uiccOns.longName, carrierInfo.uiccOns.shortName);
 *    }
 *    else {
 *        // Just give the Error as Return Code
 *        printf("Error reading Subscriber Information, retCode=%d\n", rc);
 *    }
 * @endcode
 *
 * @section QWF_EXAMPLES_SMS Code examples for the Cellular Short Message Service (SMS) service
 *
 * @b Example for the following use cases:
 *    - Use the QWF SMS service to send a short message
 *    - Use the QWF SMS service to receive a short message
 *
 * @code
 *    //
 *    // sample QWF SMS service client's implementation
 *    //
 *
 *    #include <qwf/qwf_sms.h> // QWF SMS service public API
 *
 *    ...
 *
 *    // Send a short message (pCtx from qwf_Init() call)
 *    qwf_sms_ReturnCode_e ret;
 *    int32_t msgId = 0; // Used to identify the sent message
 *    qwf_sms_SendMsg_t * pSmsData; // See qwf_sms_SendMsg_t
 *                                   //for more details
 *    pSmsData = calloc(1,sizeof(qwf_sms_SendMsg_t));
 *
 *    ret = qwf_sms_SendMsg(pCtx, pSmsData, &msgId);
 *
 *    if (SMS_ERR_SUCCESS != ret) {
 *       // error handling for failure to send a short message
 *    }
 *
 *    ...
 *
 *    // Receive a an incoming short message
 *
 *    // Callback function to handle the incoming short message
 *    // pClientData is optional and could be NULL
 *    void qwf_sms_IncomingMsg(qwf_Context_t *pCtx,
 *                        qwf_sms_MsgListener_t *pIncomingMsg,
 *                        void *pClientData)
 *    {
 *      if(NULL == pCtx || NULL == pIncomingMsg) {
 *          // error handling for null pointer
 *          return;
 *      }
 *
 *      printf("\ns=sms, ev=incomingMsg, ");
 *      printf("msgId=%d,", pIncomingMsg->msgId);
 *      printf("senderAddress=%s, ", pIncomingMsg->senderAddress);
 *      printf("payload=%s,", pIncomingMsg->payload);
 *      printf("encoding=%d, ", pIncomingMsg->encoding);
 *      printf("class=%d, ", pIncomingMsg->smsClass);
 *   }
 *
 *    ...
 *
 *
 *    int32_t msgListenerId    // A unique ID given by the SMS
 *                             // service if client decides to
 *                             // deregister to receive any
 *                             // short messages
 *
 *     // Register a callback function to receive the
 *     // incoming short messages
 *     // pClientData is optional and could be NULL
 *     qwf_sms_ReturnCode_e ret;
 *     ret = qwf_sms_RegMsgListener(pCtx,qwf_sms_IncomingMsg,
 *                                  SMS_MSG_TYPE,
 *                                  &msgListenerId, NULL);
 *
 *     if (SMS_ERR_SUCCESS != ret) {
 *         // error handling for failure to register the callback function
 *     }
 * @endcode
 *
 * @section QWF_EXAMPLES_VOICE Code examples for the Cellular Voice service
 *
 * @b Example for the following use cases:
 *    - Use the QWF Voice service to make a voice call and end the call
 *    - Use the QWF Voice service to receive a voice call and answer the call
 *
 * @code
 *    //
 *    // Sample QWF Voice Service client's implementation
 *    //
 *
 *    #include <qwf/qwf_voice.h> // QWF Voice Service public API
 *
 *    ...
 *
 *    // Note: assume QWF context from qwf_init() is stored in variable pCtx
 *    qwf_voice_ReturnResult_e ret;  // Return code for QWF Voice APIs
 *    int callId = 0;                // A variable to store call Id
 *
 *    // To make a voice call
 *    char num[QWF_VOICE_MAX_PHONE_NUMBER_LEN] = "1234567890";
 *
 *    ret = qwf_voice_StartCall(pCtx, num, &callId);
 *    if (VOICE_RETURN_SUCCESS != ret) {
 *        // Error handling for failure to start a voice call
 *    }
 *
 *    ...
 *
 *    // To end the voice call
 *
 *    ret = qwf_voice_EndCall(pCtx, callId);
 *    if (VOICE_RETURN_SUCCESS != ret) {
 *        // Error handling for failure to end a voice call
 *    }
 *
 *    ...
 *
 *    // To receive a voice call
 *
 *    // Register voice event callback function
 *    // Assume client voice event handler is eventHandler()
 *    ret = qwf_voice_RegisterCallback(pCtx, eventHandler, NULL); // No client data here
 *    if (VOICE_RETURN_SUCCESS != ret) {
 *        // Error handling for failure to register callback function
 *    }
 *
 *    ...
 *
 *    // Handle client voice event
 *    void eventHandler(qwf_Context_t *pCtx, qwf_voice_Event_t *pEvent, void *pClientData)
 *    {
 *         ...
 *         switch (pEvent->eventCode) {
 *         case VOICE_EVENT_INCOMING_CALL:
 *         {
 *            callId = pEvent->voiceEvent.callId;   // Call Id of the incoming call
 *            ...
 *            break;
 *         }
 *         ...
 *    }
 *
 *    ...
 *
 *    // To answer the voice call
 *
 *    ret = qwf_voice_AnswerCall(pCtx, callId);
 *    if (VOICE_RETURN_SUCCESS != ret) {
 *        // Error handling for failure to answer a voice call
 *    }
 * @endcode
 *
 * @section QWF_EXAMPLES_ECALL Code examples for the Cellular eCall service
 *
 * @b Example for the following use case:
 *    - Use the QWF eCall service to start an eCall
 *
 * @code
 *    //
 *    // sample QWF ECall service client's implementation
 *    //
 *
 *    #include <qwf/qwf_ecall.h> // QWF ECall service public API
 *
 *    ...
 *
 *    // Start an eCall (pCtx from qwf_Init() call)
 *    int callId = 0;         // CallId value is returned
 *    qwf_ecall_Msd_t * pMsd; // See qwf_ecall_Msd_t for more
 *                            // details.
 *
 *    pMsd = calloc(1,sizeof(qwf_ecall_Msd_t));
 *
 *    pMsd->ecalltype = ECALL_MAN_ECALL_TYPE;
 *    pMsd->confidence = true;
 *    pMsd->vehicle = ECALL_PASSENGER_VEHICLE_CLASS_M1;
 *    strlcpy(pMsd->vin, "1M8GDM9AXKP042789",sizeof(pMsd->vin));
 *    pMsd->storageType = 0x01;
 *    pMsd->timeStamp = 1326752314;
 *    pMsd->latitude = 164565698;
 *    pMsd->longitude = 49457972;
 *    pMsd->direction = 40;
 *    pMsd->latDeltaN1 = 500;
 *    pMsd->longDeltaN1 = 501;
 *    pMsd->latDeltaN2 = 502;
 *    pMsd->longDeltaN2 = 503;
 *    pMsd->passengers = 3;
 *
 *    qwf_ecall_ReturnResult_e ret;
 *    ret = qwf_ecall_StartEcall(pCtx, pMsd, &callId);
 *
 *    if (ECALL_RETURN_SUCCESS != ret) {
 *       // error handling for failure to start an eCall
 *    }
 * @endcode
 *
 * @section QWF_EXAMPLES_CDS Code examples for the Cellular Data Service (CDS) service
 *
 * @b Example for the following use case:
 *    - Use the QWF CDS service to make a data connection and get the the status of the connection
 *
 * @code
 *    // Return Code for QWF CDS API call
 *    qwf_cds_Results_e rc;
 *    // Structure to hold the connection's status information
 *    qwf_cds_ConnectionStatus_t connStatus;
 *
 *    // Assumes QWF CDS initialized and QWF Context stored in variable pCtx.
 *    // Invoke QWF CDS API to activate the "Internet" data connection.
 *    rc = qwf_cds_Connect(pCtx, QWF_CDS_CONNECTION_INTERNET);
 *    if (CDS_OK != rc) {
 *        // Output the return code as the error
 *        printf("Error making connection, retCode=%d\n", rc);
 *    }
 *
 *    // Invoke QWF CDS API to get the status of the "Internet" data
 *    // connection. Status information is returned in the
 *    // qwf_cds_ConnectionStatus_t structure defined above.
 *    rc = qwf_cds_GetConnectionStatus(pCtx, QWF_CDS_CONNECTION_INTERNET,
 *    &connStatus);
 *
 *    // Output the Long and Short Operator Name strings
 *    if (CDS_OK == rc) {
 *        // Output some connection information
 *        printf("Connection state=%d, apn=%s, netIface=%s\n",
 *                connStatus.state, connStatus.apn,
 *                connStatus.networkingInterfaceName);
 *    }
 *    else {
 *        // Output the return code as the error
 *        printf("Error getting connection data, retCode=%d\n", rc);
 *    }
 * @endcode
 *
 * @section QWF_EXAMPLES_NET Code examples for the Network Management service
 *
 * @b Example for the following use case:
 *    - Use the QWF Network Management service to get the current default interface
 *
 * @code
 *   qwf_net_IfName_t ifName;            // holds the IfName string
 *   qwf_net_InterfaceData_t ifaceData;  // holds the interface data
 *
 *   // API calls assume QWF NET initialized and QWF Context in variable pCtx.
 *   // Query NET MGR to get the current default interface
 *   qwf_net_Results_e rc = qwf_net_GetDefaultInterface(pCtx, &ifName);
 *   // Output the interface name
 *   if (NET_OK == rc) {
 *      printf("Default network interface: %s\n", ifName);
 *   }
 *   else {
 *      printf("Error getting default interface! Error: %d\n", rc);
 *   }
 *
 *   // Query to get information for the default interface
 *   rc = qwf_net_GetInterfaceData(pCtx, &ifName, &ifaceData);
 *   // Output some of the relevent interface information
 *   if (NET_OK == rc) {
 *      printf("Interface=%s, connType=%d, connected=%s, hasIP=%s, ",
 *             ifaceData.name, ifaceData.ifType,
 *             (ifaceData.connected ? "true" : "false"),
 *             (ifaceData.hasIpAddr ? "true" : "false"));
 *      printf("hasIP4=%s, IP4addr=%s, IP4mask=%s, gateway=%s,",
 *             (ifaceData.ip4 ? "true" : "false"), ifaceData.ip4Address,
 *             ifaceData.ip4Netmask, ifaceData.ip4Gateway);
 *      printf("broadcast=%s, search_domain=%s, dns_1=%s, dns_2=%s",
 *             ifaceData.broadcast, ifaceData.searchDomain,
 *             ifaceData.nameServers[0], ifaceData.nameServers[1]);
 *   }
 *   else {
 *       printf("Error getting interface data! Error: %d\n", rc);
 *   }
 * @endcode
 *
 * @section QWF_EXAMPLES_WIFI Code examples for the Wi-Fi Management service
 *
 * @b Example for the following use case:
 *    - Use the QWF Wi-Fi Management service to connect to an available (scanned) access point
 *
 * @code
 *     // Return Code for QWF Wi-Fi API calls
 *     qwf_wifi_Results_e rc;
 *     // ID of the created Saved Network
 *     uint16_t savedNetworkId, wait;
 *     qwf_wifi_Ssid_t ssid;
 *     qwf_wifi_StationModeStatusData_t status;
 *     qwf_Context_t *pCtx = S_ctx_wifi;
 *
 *     // API calls assume QWF Wi-Fi initialized and QWF Context stored in
 *     // variable pCtx. Also assumes user knows SSID and Authentication used
 *     // by access point. This information can be obtained from the
 *     // qwf_wifi_GetScanResults() interface.
 *
 *     // Create a "saved network", if not already existing, for this access
 *     // point, by first setting the required authentication parameters, WPA2
 *     // PSK for example, requires a passphrase. For this example the access
 *     // point has SSID name of "SampleWifi" and passphrase is "Testing123".
 *
 *     rc = qwf_wifi_SetPassphrase(pCtx, "Testing123");
 *
 *     // Second, create the Saved Network.
 *     strlcpy(ssid.name, "SampleWifi", sizeof(ssid.name));
 *     rc = qwf_wifi_CreateSavedNetwork(pCtx, &ssid, WIFI_SEC_PROT_WPA2_PSK,
 *                                      WIFI_BAND_2_4_GHZ, &savedNetworkId);
 *     if (WIFI_OK == rc) {
 *         printf("Created Saved Network for %s!! Now Enable ID=%d\n",
 *                ssid.name, savedNetworkId);
 *         // Created OK, so "Enable" the Saved Network if want to connect when
 *         // not currently connected to the access point if available
 *         rc = qwf_wifi_EnableSavedNetwork(pCtx, savedNetworkId);
 *     }
 *     // Query the Wi-Fi status to see what status of connection is
 *     // and what SSID the device is connected to if any.
 *     // Using a simple loop to wait for connected, but should use the event
 *     // WIFI_EVENT_CLIENT_CONNECTION_STATUS to track connection status.
 *     for (wait=0;wait<20;wait++) {
 *         rc = qwf_wifi_GetClientStatus(pCtx, &status);
 *         if (WIFI_OK == rc) {
 *             if (status.connected) {
 *                 printf("WiFi is CONNECTED to Saved Network ID: %d, Access
 *                        Point: %s\n", status.connectedSavedNetworkId,
 *                        status.connectedSsid.name); break;
 *             }
 *             else {
 *                 printf("WiFi is NOT Connected!\n");
 *             }
 *         }
 *         delay(1000); //sleep 1 sec
 *     }
 *     break;
 * @endcode
 *
 * @section QWF_EXAMPLES_LCS Code examples for the Location service
 *
 * @b Example for the following use cases:
 *    - Use the QWF LCS service to turn on GPS device
 *    - Use the QWF LCS service to enable GPS jamming detection
 *
 * @code
 *    #include <qwf/qwf_lcs.h> // QWF LCS service public API
 *
 *    ...
 *
 *    // turn on gps device (pQwfCtx from qwf_Init() call)
 *    qwf_lcs_Results_e ret = qwf_lcs_setPower(pQwfCtx, LCS_POWER_ON, 1);
 *
 *    if (LCS_OK != ret) {
 *        printf("gps powered on\n");
 *    }
 *    else {
 *        printf("gps power on failed\n");
 *    }
 *
 *    // enable gps jamming detection
 *    ret = qwf_lcs_setJamming(pQwfCtx, true);
 *
 *    if (rc == LCS_OK) {
 *        printf("gps jamming enabled\n");
 *    }
 *    else {
 *        printf("gps jamming enable failed\n");
 *    }
 * @endcode
 */

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/wireless_framework/branches/7.1/trunk/common/lib/qwf_interface/public/qwf/qwf_overview.h $ $Rev: 1963 $")
#endif

