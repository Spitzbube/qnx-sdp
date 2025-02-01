/*
 * $QNXLicenseC:
 * Copyright (c) 2017, QNX Software Systems. All Rights Reserved.
 *
 * You must obtain a written license from and pay applicable license fees to QNX
 * Software Systems before you may reproduce, modify or distribute this software,
 * or any work that includes all or part of this software. Free development
 * licenses are available for evaluation and non-commercial purposes. For more
 * information visit http://licensing.qnx.com or email licensing@qnx.com.
 *
 * This file may contain contributions from others. Please review this entire
 * file for other proprietary rights or license notices, as well as the QNX
 * Development Suite License Guide at http://licensing.qnx.com/license-guide/
 * for other information.
 * $
 */

#ifndef __IOHCI_H__
#define __IOHCI_H__

#include <stdbool.h>
#include <stdint.h>
#include <sys/dispatch.h>
#include <sys/iofunc.h>
#include <sys/queue.h>
#include <sys/dcmd_chr.h>


/* HCI definitions */
#define PROTO_HCI_CMD                       0x01
#define PROTO_ACL_PKT                       0x02
#define PROTO_SCO_PKT                       0x03
#define PROTO_HCI_EVT                       0x04
#define PROTO_FM_CHNL                       0x08
#define PROTO_GPS_CHNL                      0x09

#define cmd_opcode_pack(ogf, ocf)           (uint16_t)((ocf & 0x03ff) | (ogf << 10))

/* one byte packet indicator */
#define PROTO_HCI_PKT_INDICATOR_LEN         1
/* one byte Event Code and one byte Parameter Length */
#define PROTO_HCI_EVT_HEADER_LEN            2
/* two bytes Op Code and one byte Parameter Length */
#define PROTO_HCI_CMD_HEADER_LEN            3
/* two bytes Handle and Flags and two bytes Parameter Length */
#define PROTO_HCI_ACL_HEADER_LEN            4
/* one byte Parameter Length */
#define PROTO_HCI_FM_HEADER_LEN             1
/* one byte Handle and two bytes Parameter Length */
#define PROTO_HCI_GPS_HEADER_LEN            3
/* one byte Handle and two bytes Parameter Length */
#define PROTO_HCI_SCO_HEADER_LEN            3

// Header length including the packet indicator
#define PROTO_HCI_MIN_HEADER_LEN            3
#define PROTO_HCI_MAX_HEADER_LEN            5

// HCI Event code
#define PROTO_HCI_EVT_CONNECTION_COMPLETE      0x03
#define PROTO_HCI_EVT_COMMAND_COMPLETE         0x0E
#define PROTO_HCI_EVT_COMPLETE_PACKETS_NUM     0x13
#define PROTO_HCI_EVT_DATA_BUF_OVERFLOW        0x1A

/* According to the Bluetooth Spec, the size of Parameter_Total_Length for HCI
 * Command Packet is 1 Octet. So max HCI Command Packet length is:
 * 0xFF + PROTO_HCI_CMD_HEADER_LEN + PROTO_HCI_PKT_INDICATOR_LEN
 *
 * The Num_HCI_Command_Packets event parameter means the Number of HCI command
 * packets which are allowed to be sent to the Controller from the Host. Its
 * size is 1 Octet. So max Number of HCI command packet 0xFF.
 *
 * The same for HCI Event packet.
 *
 * While for ACL data packet, the Data_Total_Length parameter size is 2 Octets.
 * So max value is 0xFFFF.
 *
 * */
#define PROTO_HCI_CMD_MAX_PKT_LEN           ( 0xFF + PROTO_HCI_CMD_HEADER_LEN + PROTO_HCI_PKT_INDICATOR_LEN )     // 259
#define PROTO_HCI_CMD_MAX_PKT_NUM           0xFF
#define PROTO_HCI_EVT_MAX_PKT_LEN           ( 0xFF + PROTO_HCI_EVT_HEADER_LEN + PROTO_HCI_PKT_INDICATOR_LEN )     // 258
#define PROTO_HCI_EVT_MAX_PKT_NUM           0xFF
#define PROTO_HCI_ACL_MAX_PKT_LEN           ( 0xFFB + PROTO_HCI_ACL_HEADER_LEN + PROTO_HCI_PKT_INDICATOR_LEN )   // 4096
#define PROTO_HCI_ACL_MAX_PKT_NUM           10

/* io-hci */
#define    IOHCI_RESMGR_MAX_WRITE_BUFF_SIZE       PROTO_HCI_ACL_MAX_PKT_LEN
#define    IOHCI_RESMGR_MAX_READ_BUFF_SIZE        PROTO_HCI_ACL_MAX_PKT_LEN

#define    IOHCI_DEFAULT_CMD_TIMEOUT              1000       // 1000 milli seconds
#define    IOHCI_DEFAULT_SER_BAUD_RATE            115200     // Default initial baud rate
#define    IOHCI_DEFAULT_SER_BAUD_RATE_MID        921600     // Default baud rate for downloading script file
#define    IOHCI_DEFAULT_SER_BAUD_RATE_HIGH       3000000    // Default baud rate for streaming

/* Based on our calculation, the character size = start bit + data_bits + stop_bits
 * which is 10. Then set serial port fifo size to 10, so the inter bytes timeout is
 * calculated by ( ( ( 1 + data_bits + stop_bits ) * 10 * 1000 ) / speed ) which is
 * 0.9ms. So we define the default inter byte timeout is 0.1s.
 * */
#define    IOHCI_DEFAULT_INTER_BYTE_TIMEOUT       1          // tenths of second: 100 milli seconds
#define    IOHCI_DEFAULT_SERIAL_READ_TIMEOUT      20         // tenths of second: 2 seconds

/* io-hci flags */
#define    IOHCI_RESETTING                        ( 1 << 0 )   // HCI is resetting. Set when iohci_reset() is called.
#define    IOHCI_SCRIPT_LOADED                    ( 1 << 1 )   // Script has been loaded
#define    IOHCI_INIT_RESETTING                   ( 1 << 2 )   // HCI init reset. Happens in hci_init()
#define    IOHCI_FLUSHING_BUF                     ( 1 << 3 )   // Flushing buffers

/* Virtual device flags */
#define    IOHCI_NO_EMPTY_BUFFER                  ( 1 << 0 )   // Driver has no empty buffer to receive data
#define    IOHCI_CLIENT_PRESENT                   ( 1 << 1 )   // Have client

/* io-hci devctl commands */
/*
 * Note: IOHCI_DEVCTL_CHANGE_BAUD_RATE command will change the SOC baud rate only.
 * The client should send an updating baud rate HCI command first to change the BT
 * chip baud rate before issue this devctl command.
 */
#define    IOHCI_DEVCTL_CHANGE_BAUD_RATE         __DIOT(_DCMD_MISC,  1, uint32_t)
#define    IOHCI_DEVCTL_GET_STATUS               __DIOF(_DCMD_MISC,  2, uint32_t)

/* Virtual devices */
#define    IOHCI_VDEV_MAX_NUM                    3
#define    IOHCI_VDEV_NAME_BT                    "/dev/hcibt"
#define    IOHCI_VDEV_NAME_GPS                   "/dev/hcigps"
#define    IOHCI_VDEV_NAME_FM                    "/dev/hcifm"

/* Packet queue types */
#define    IOHCI_EMPTY_QUEUE                     0             // Empty packets queue
#define    IOHCI_USED_QUEUE                      1             // Used packets queue

/* Client queue types */
#define    IOHCI_READ_QUEUE                      0             // Reading blocked clients queue
#define    IOHCI_WRITE_QUEUE                     1             // Writing blocked clients queue

/* Bluetooth chip toggle */
#define   IOHCI_BT_CHIP_ON                       1
#define   IOHCI_BT_CHIP_OFF                      0

typedef enum {
    IOHCI_VDEV_BT,
    IOHCI_VDEV_FM,
    IOHCI_VDEV_GPS,
    IOHCI_VDEV_NUM,
} vdev_list;


typedef struct _iohci_ctrl iohci_ctrl_t;
typedef struct _iohci_vdev iohci_vdev_t;
typedef struct _iohci_device_dll_ctrl iohci_device_dll_ctrl_t;
typedef struct _iohci_board_dll_ctrl iohci_board_dll_ctrl_t;
typedef struct _hci_pkt_t hci_pkt_t;
typedef struct _client_t client_t;
typedef struct _iohci_dev iohci_dev_t;
typedef struct _iohci_resmgr_ctrl iohci_resmgr_ctrl_t;


/* Device driver DLL APIs */
struct _iohci_device_dll_ctrl {

/* Handle to the device driver DLL, returned by dlopen() */
    void     *dll_hdl;

/**
 *  @brief                 HCI open function
 *
 *  @return                EOK --success otherwise fail
 */
    int      ( *hci_start )( void );

/**
 *  @brief                 HCI close function
 *
 *  @return                EOK --success otherwise fail
 */
    int      ( *hci_stop )( void );

/**
 *  @brief                 Init function. Start bluetooth device driver
 *  @param param           The io-hci ctrl structure
 *
 *  @return                EOK --success otherwise fail
 */
    int      ( *hci_init )( void *param );

/**
 *  @brief                 HCI write function. Write the HCI command or ACL data
 *  @param packet          A pointer to a TX packet that contains the data the client wants to write
 *
 *  @return                EOK --success otherwise fail
 */
    int      ( *hci_write )( hci_pkt_t *tx_pkt );

/**
 *  @brief                 Device driver DLL cleanup function
 *
 *  @return                EOK --success otherwise fail
 */
    int      ( *hci_fini )( void );

/**
 *  @brief                 Allocate packet buffer function
 *  @param size            The size of the buffer
 *
 *  @return                NULL or the allocated buffer pointer
 */
    uint8_t  *( *hci_alloc_buffer )( int size );

/**
 *  @brief                 Free packet buffer function
 *  @param buf             The address of the buffer
 *  @param size            The size of the buffer
 *
 *  @return                EOK --success otherwise fail
 */
    int      ( *hci_free_buffer )( uint8_t *buf, int size );

/**
 *  @brief                 Transport level reset function
 *
 *  @return                EOK --success otherwise fail
 */
    int      ( *hci_transport_reset )( void );

/**
 *  @brief                 Change SOC baud rate function
 *  @param baud_rate       The new baudrate
 *
 *  @return                EOK --success otherwise fail
 */
    int      ( *hci_change_soc_baudrate )( uint32_t baud_rate );

/**
 *  @brief                 Get line status of serial device function
 *  @param linestatus      The line status
 *
 *  @return                EOK --success otherwise fail
 */
    int      ( *hci_get_line_status )( uint32_t *linestatus );
};

/* Board DLL APIs */
struct _iohci_board_dll_ctrl {

/* Handle to the board DLL, returned by dlopen() */
    void     *dll_hdl;

/* The control structure of board specific DLL. Returned by board_init() */
    void     *dll_ctrl;

/**
 *  @brief                 Init function.
 *  @param param           The io-hci ctrl structure
 *
 *  @return                The control structure of board specific DLL
 */
    void     *( *board_init )( void *param );

/**
 *  @brief                 Bluetooth chip toggle function.
 *  @param hdl             The address of the control structure returned from init function
 *  @param state           The BT status. ON or OFF
 *
 *  @return                EOK --success otherwise fail
 */
    int      ( *chip_toggle )( void *hdl, int state );

/**
 *  @brief                 Board DLL cleanup function.
 *  @param hdl             The address of the control structure returned from init function
 *
 *  @return                EOK --success otherwise fail
 */
    int      ( *board_fini )( void *hdl );

};

/* HCI packet structure */
struct _hci_pkt_t {
    TAILQ_ENTRY( _hci_pkt_t )     pkt_link;
    iohci_vdev_t                  *vdev_ptr;     // Which vdev this packet belongs to
    uint32_t                      ndata;         // Bytes of data in buffer
    uint32_t                      nleft;         // Left bytes of data in buffer
    uint32_t                      offset;        // Read offset of buffer
    off64_t                       phys_addr;     // Physical address of the buffer
    uint8_t                       *buffer;       // HCI packet including packet indicator
};

/* Blocking client structure */
struct _client_t {
    TAILQ_ENTRY( _client_t )      client_link;
    int                           rcvid;         // Receive ID
    _Int32t                       scoid;         // Server connection ID
    _Int32t                       coid;          // Connection ID
};

/* Virtual device structure */
struct _iohci_vdev {
    iofunc_attr_t                                attr;                          // Note: attr has to be the first member!!!
                                                                                //       We'll use it to get the addr of vdev
    iohci_ctrl_t                                 *iohci_ptr;                    // The address of iohci_ctrl structure

    uint32_t                                     flags;                         // vdev flags
    pthread_mutex_t                              mutex;                         // vdev mutex

    pthread_cond_t                               empty_buf_cond;                // Empty buffer available to receive data from hardware

    char                                         *vdev_name;

    int                                          resmgr_id;

    iofunc_notify_t                              notify[3];

    TAILQ_HEAD(empty_pkt_list_head,_hci_pkt_t)   empty_pkt_dlist;               // Empty packet queue: The empty packet ready to use
    pthread_mutex_t                              empty_pkt_dlist_mutex;         // Empty packet queue mutex

    TAILQ_HEAD(used_pkt_list_head,_hci_pkt_t)    used_pkt_dlist;                // Used packet queue: Contains HCI packets from hardware
    pthread_mutex_t                              used_pkt_dlist_mutex;          // Used packet queue mutex

    TAILQ_HEAD(rcleint_list_head,_client_t)      read_client_dlist;             // Reading blocked client queue
    pthread_mutex_t                              read_client_dlist_mutex;       // Reading blocked client queue mutex

    TAILQ_HEAD(wcleint_list_head,_client_t)      write_client_dlist;            // Writing blocked client queue
    pthread_mutex_t                              write_client_dlist_mutex;      // Writing blocked client queue mutex

    hci_pkt_t                                    *tx_pkt;                       // Packet to save transmit data

    volatile uint32_t                            oband_data;
};

/* Hardware device structure */
struct _iohci_dev {

    int                  priority;                                  // The priority of the interrupt threads

    iohci_vdev_t         *vdev;                                     // Virtual device list

    int                  vdev_num;

    hci_pkt_t            *rx_pkt;                                   // Empty packet to receive data from hardware
    int                  rx_header_len;                             // HCI packet header length including indicator
    int                  rx_buffer_nbytes;                          // Number of bytes of received data
    int                  rx_length;                                 // Number of bytes of HCI packet parameter
    int                  rx_active;                                 // Index of active vdev

    uint32_t             baud_default;
    uint32_t             baud_current;
    uint32_t             baud_mid;
    uint32_t             baud_high;

    TAILQ_HEAD(tx_pkt_list_head,_hci_pkt_t)  tx_pkt_dlist;          // TX packet queue: List of vdev->tx_pkt which has data ready to send to hardware
    pthread_mutex_t                          tx_pkt_dlist_mutex;    // TX packet queue mutex
};

/* io-hci resmgr interface */
struct _iohci_resmgr_ctrl {
    dispatch_t                      *dispatch;
    thread_pool_t                   *thread_pool;
    thread_pool_attr_t              pool_attr;

    resmgr_connect_funcs_t          resmgr_connect_funcs;
    resmgr_io_funcs_t               resmgr_io_funcs;
    int                             coid;
    int                             chid;
};

/* io-hci control */
struct _iohci_ctrl {

    int                              verbose;
    pthread_mutex_t                  mutex;                       // Global io-hci mutex
    uint32_t                         cflags;
    int                              pulse;
    int                              coid;

    char                             *config_file;                // Full pathname of io-hci config file

    iohci_resmgr_ctrl_t              rctrl;                       // Resmgr structure

    char                             *dll_name;                   // Device driver DLL name
    char                             *dll_args;                   // Device driver DLL command line options
    iohci_device_dll_ctrl_t          *drvdll_ctrl;                // Device driver DLL structure

    char                             *brddll_name;                // Board DLL name
    char                             *brddll_args;                // Board DLL command line options
    iohci_board_dll_ctrl_t           *brddll_ctrl;                // Board DLL structure

    char                             script_name[_POSIX_PATH_MAX];// BT init script file name (*.bts or *.hcd)
    uint32_t                         max_retry;                   // HCI reset retries, 0 = infinite (default 5)

    pthread_cond_t                   pkt_recvd_cond;              // HCI packet received from hardware when HCI is resetting

    int                              intr_complete;               // Interrupt handler completes putting packet back to empty queue when flushing buf

    iohci_dev_t                      *iohci_dev;                  // Hardware device structure
};

//io-hci.c
int get_iohci_verbose( void );

// iohci_lib.c: libiohci functions
void iohci_slogf( int code, int verbosity, const char *fmt, ... );
void iohci_display_data_bytes( _uint8 * ptr, int len, const char *dir );
int iohci_create_vdevs( char **vdev_names, int dev_num, void *hdl );
int iohci_device_attach( iohci_ctrl_t *iohci_ctrl );
int iohci_destroy_vdevs( void *hdl );
int iohci_device_detach( iohci_ctrl_t *iohci_ctrl );
void iohci_unblock_all_clients( iohci_vdev_t  *vdev );
void iohci_unblock_one_client( iohci_vdev_t  *vdev, _Int32t scoid, _Int32t coid );
hci_pkt_t *iohci_get_pkt( iohci_vdev_t  *vdev, int type );
int iohci_put_pkt ( hci_pkt_t * pkt, int type );
void iohci_packet_received( iohci_vdev_t  *vdev );
void iohci_packet_transmitted( iohci_vdev_t  *vdev );
int iohci_empty_buffer_cnt( iohci_vdev_t  *vdev );
void iohci_flush_buffers( iohci_vdev_t  *vdev );
int iohci_reset_variables ( iohci_dev_t  *dev );
int iohci_reset( iohci_vdev_t  *vdev );
int iohci_hci_init_reset ( iohci_vdev_t  *vdev );
int iohci_process_rx_packet( iohci_dev_t * dev );
int iohci_get_vdev_pkt( iohci_dev_t *dev, uint8_t indicator );
int iohci_get_hci_para_len( iohci_dev_t *dev );

#endif   /* __IOHCI_H__ */

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/services/io-hci/public/hw/io-hci.h $ $Rev: 843356 $")
#endif
