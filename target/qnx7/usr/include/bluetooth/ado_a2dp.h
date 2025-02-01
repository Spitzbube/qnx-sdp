/*
 * $QNXLicenseC:
 * Copyright 2017 QNX Software Systems.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"). You
 * may not reproduce, modify or distribute this software except in
 * compliance with the License. You may obtain a copy of the License
 * at: http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTIES OF ANY KIND, either express or implied.
 *
 * This file may contain contributions from others, either as
 * contributors under the License or as licensors under other terms.
 * Please review this entire file for other proprietary rights or license
 * notices, as well as the QNX Development Suite License Guide at
 * http://licensing.qnx.com/license-guide/ for other information.
 * $
 */

#ifndef __ADO_A2DP_H__
#define __ADO_A2DP_H__

/**
 * @brief define for A2DP stream path on target
 */
#define A2DP_STREAM_PATH        "/dev/io-bluetooth/a2dp/audio_stream"

/**
 * @brief define for A2DP default frame rate
 */
#define DEFAULT_FRAME_RATE      44100

/**
 * @brief define for A2DP default channel number
 */
#define DEFAULT_NUMBER_CHANNELS 2

/**
 * @brief define for A2DP default sample size
 */
#define DEFAULT_SAMPLE_SIZE     2

/**
 * @brief struct for stream properties
 */
typedef struct _a2dp_strm_property_t{
    unsigned int    channels;      ///< number of channels present
    int             frame_rate;    ///< frame rate
    int             sample_size;   ///< sample size
} a2dp_strm_property_t;

/** @enum a2dp_packet_type_e
 * @brief Enumeration for A2DP packet type
 */
enum {
    A2DP_PACKET_TYPE_DATA = 1,        ///< Packet type data
    A2DP_PACKET_TYPE_FORMAT,          ///< Maximum szie of enum
} a2dp_packet_type_e;

/**
 * @brief struct for A2DP packet header properties
 */
typedef struct _a2dp_pkt_hdr_t{
    unsigned int data_type;    ///< define data type @see a2dp_packet_type_e
    unsigned int data_len;     ///< define data buffer len
}a2dp_pkt_hdr_t;

/**
 * @brief struct for A2DP packet data
 */
typedef struct _a2dp_pkt {
    a2dp_pkt_hdr_t   a2dp_pkt_hdr;    ///< A2DP packet header
    char             data[0];         ///< A2DP packet data buffer
}a2dp_pkt_t;

#endif	//__ADO_A2DP_H__

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.0.0/BC700_6762_sdp704/services/audio/public/include/bluetooth/ado_a2dp.h $ $Rev: 868848 $")
#endif
