#ifndef _QWDI_MSG_H
#define _QWDI_MSG_H

/*
 * QNX WLAN DRIVER INTEGRATION (QWDI) public header file
 *
 * $QNXLicenseC:
 * Copyright 2015-2018, QNX Software Systems. All Rights Reserved.
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
 *
 * The interface names and definitions in this file are based on the following:
 *
 * WPA Supplicant
 * ==============
 * Copyright (c) 2003-2018, Jouni Malinen <j@w1.fi> and contributors
 * All Rights Reserved.
 *
 * This program is licensed under the BSD license (the one with
 * advertisement clause removed).
 *
 * If you are submitting changes to the project, please see CONTRIBUTIONS
 * file for more instructions.
 *
 * License
 * -------
 *
 * This software may be distributed, used, and modified under the terms of
 * BSD license:
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name(s) of the above-listed copyright holder(s) nor the
 *    names of its contributors may be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* initial 2.9 version 08/29/2019 */
#define QWDI_VERSION 0x0209
#define QWDI_VERSION_STR "QWDI-2.9"

#ifndef BIT
#define BIT(x) (1 << (x))
#endif

#define QWDI_RET_SUCCESS           0x00
#define QWDI_RET_ERR_GENERAL       0x01
#define QWDI_RET_ERR_INVALID_PARAM 0x02
#define QWDI_RET_ERR_UNHANDLED_CMD 0x03

#define QWDI_IFNAMSIZ 15

#define QWDI_NETIF_NAME(DEVNAME) DEVNAME"0"

#define QWDI_MAX_SCAN_SSID 16
#define QWDI_MAX_SCAN_FREQ 38
#define QWDI_MAX_EXTRA_IES_LEN 512
#define QWDI_MAX_SSID_STR_LEN 32
#define QWDI_MAX_RESPONSE_SIZE (6 * 1024)
#define QWDI_MAX_SEQ_LEN  8
#define QWDI_MAX_KEY_LEN  32
#define QWDI_MAX_NOA_LEN  64
#define QWDI_MAX_COUNTRY_LEN 4
#define QWDI_MAX_HEAD_BUFF_LEN 256
#define QWDI_MAX_TAIL_BUFF_LEN 512
#define QWDI_MAX_PROBE_RESP_LEN 768
#define QWDI_MAX_SUPPORT_RATES 12
#define QWDI_MAX_MGMT_FRAME_LEN 1800
#define QWDI_MAX_SUPPORT_MCS 16
#define QWDI_MAX_CHANNEL_NUM 50

#define QWDI_PMKID_LEN  16
#define QWDI_PMK_MAX_LEN 64

#define QWDI_FILS_NONCE_LEN 16
#define QWDI_FILS_SESSION_LEN 8
#define QWDI_FILS_CACHE_ID_LEN 2
#define QWDI_FILS_MAX_KEY_AUTH_LEN 48

#define QWDI_MAX_IE_SIZE  257
#define QWDI_ETH_ALEN 6

#define QWDI_IEEE80211_CAP_ESS  0x0001
#define QWDI_IEEE80211_CAP_IBSS 0x0002
#define QWDI_IEEE80211_CAP_PRIVACY  0x0010
#define QWDI_IEEE80211_CAP_RRM  0x1000

#define QWDI_WPA_CIPHER_NONE BIT(0)
#define QWDI_WPA_CIPHER_WEP40 BIT(1)
#define QWDI_WPA_CIPHER_WEP104 BIT(2)
#define QWDI_WPA_CIPHER_TKIP BIT(3)
#define QWDI_WPA_CIPHER_CCMP BIT(4)
#define QWDI_WPA_CIPHER_AES_128_CMAC BIT(5)
#define QWDI_WPA_CIPHER_GCMP BIT(6)
#define QWDI_WPA_CIPHER_SMS4 BIT(7)
#define QWDI_WPA_CIPHER_GCMP_256 BIT(8)
#define QWDI_WPA_CIPHER_CCMP_256 BIT(9)
#define QWDI_WPA_CIPHER_BIP_GMAC_128 BIT(11)
#define QWDI_WPA_CIPHER_BIP_GMAC_256 BIT(12)
#define QWDI_WPA_CIPHER_BIP_CMAC_256 BIT(13)
#define QWDI_WPA_CIPHER_GTK_NOT_USED BIT(14)

enum qwdi_beacon_rate_type {
        QWDI_BEACON_RATE_LEGACY,
        QWDI_BEACON_RATE_HT,
        QWDI_BEACON_RATE_VHT
 };

enum qwdi_hide_ssid {
        QWDI_NO_SSID_HIDING,
        QWDI_HIDDEN_SSID_ZERO_LEN,
        QWDI_HIDDEN_SSID_ZERO_CONTENTS
 };

/* all RSN_AUTH_KEY_MGMT defs from common.h */
/* RSN_SELECTOR MACRO */
#define QWDI_RSN_SELECTOR(a, b, c, d) \
    ((((unsigned int) (a)) << 24) | (((unsigned int) (b)) << 16) | (((unsigned int) (c)) << 8) | \
     (unsigned int) (d))

#define QWDI_RSN_AUTH_KEY_MGMT_UNSPEC_802_1X QWDI_RSN_SELECTOR(0x00, 0x0f, 0xac, 1)
#define QWDI_RSN_AUTH_KEY_MGMT_PSK_OVER_802_1X QWDI_RSN_SELECTOR(0x00, 0x0f, 0xac, 2)
#define QWDI_RSN_AUTH_KEY_MGMT_FT_802_1X QWDI_RSN_SELECTOR(0x00, 0x0f, 0xac, 3)
#define QWDI_RSN_AUTH_KEY_MGMT_FT_PSK QWDI_RSN_SELECTOR(0x00, 0x0f, 0xac, 4)
#define QWDI_RSN_AUTH_KEY_MGMT_802_1X_SHA256 QWDI_RSN_SELECTOR(0x00, 0x0f, 0xac, 5)
#define QWDI_RSN_AUTH_KEY_MGMT_PSK_SHA256 QWDI_RSN_SELECTOR(0x00, 0x0f, 0xac, 6)
#define QWDI_RSN_AUTH_KEY_MGMT_TPK_HANDSHAKE QWDI_RSN_SELECTOR(0x00, 0x0f, 0xac, 7)
#define QWDI_RSN_AUTH_KEY_MGMT_SAE QWDI_RSN_SELECTOR(0x00, 0x0f, 0xac, 8)
#define QWDI_RSN_AUTH_KEY_MGMT_FT_SAE QWDI_RSN_SELECTOR(0x00, 0x0f, 0xac, 9)
#define QWDI_RSN_AUTH_KEY_MGMT_802_1X_SUITE_B QWDI_RSN_SELECTOR(0x00, 0x0f, 0xac, 11)
#define QWDI_RSN_AUTH_KEY_MGMT_802_1X_SUITE_B_192 QWDI_RSN_SELECTOR(0x00, 0x0f, 0xac, 12)
#define QWDI_RSN_AUTH_KEY_MGMT_FT_802_1X_SHA384 QWDI_RSN_SELECTOR(0x00, 0x0f, 0xac, 13)
#define QWDI_RSN_AUTH_KEY_MGMT_FILS_SHA256 QWDI_RSN_SELECTOR(0x00, 0x0f, 0xac, 14)
#define QWDI_RSN_AUTH_KEY_MGMT_FILS_SHA384 QWDI_RSN_SELECTOR(0x00, 0x0f, 0xac, 15)
#define QWDI_RSN_AUTH_KEY_MGMT_FT_FILS_SHA256 QWDI_RSN_SELECTOR(0x00, 0x0f, 0xac, 16)
#define QWDI_RSN_AUTH_KEY_MGMT_FT_FILS_SHA384 QWDI_RSN_SELECTOR(0x00, 0x0f, 0xac, 17)
#define QWDI_RSN_AUTH_KEY_MGMT_OWE QWDI_RSN_SELECTOR(0x00, 0x0f, 0xac, 18)
#define QWDI_RSN_AUTH_KEY_MGMT_QWDI_CCKM QWDI_RSN_SELECTOR(0x00, 0x40, 0x96, 0x00)
#define QWDI_RSN_AUTH_KEY_MGMT_OSEN QWDI_RSN_SELECTOR(0x50, 0x6f, 0x9a, 0x01)
#define QWDI_RSN_AUTH_KEY_MGMT_DPP QWDI_RSN_SELECTOR(0x50, 0x6f, 0x9a, 0x02)

/*from defs.h*/
#define QWDI_WPA_AUTH_ALG_OPEN      (1 << 0)
#define QWDI_WPA_AUTH_ALG_SHARED    (1 << 1)
#define QWDI_WPA_AUTH_ALG_LEAP      (1 << 2)
#define QWDI_WPA_AUTH_ALG_FT        (1 << 3)
#define QWDI_WPA_AUTH_ALG_SAE       (1 << 4)

#define QWDI_WPA_PROTO_WPA BIT(0)
#define QWDI_WPA_PROTO_RSN BIT(1)
#define QWDI_WPA_PROTO_WAPI BIT(2)
#define QWDI_WPA_PROTO_OSEN BIT(3)

#define QWDI_WPA_KEY_MGMT_IEEE8021X BIT(0)
#define QWDI_WPA_KEY_MGMT_PSK BIT(1)
#define QWDI_WPA_KEY_MGMT_NONE BIT(2)
#define QWDI_WPA_KEY_MGMT_IEEE8021X_NO_WPA BIT(3)
#define QWDI_WPA_KEY_MGMT_WPA_NONE BIT(4)
#define QWDI_WPA_KEY_MGMT_FT_IEEE8021X BIT(5)
#define QWDI_WPA_KEY_MGMT_FT_PSK BIT(6)
#define QWDI_WPA_KEY_MGMT_IEEE8021X_SHA256 BIT(7)
#define QWDI_WPA_KEY_MGMT_PSK_SHA256 BIT(8)
#define QWDI_WPA_KEY_MGMT_WPS BIT(9)
#define QWDI_WPA_KEY_MGMT_SAE BIT(10)
#define QWDI_WPA_KEY_MGMT_FT_SAE BIT(11)
#define QWDI_WPA_KEY_MGMT_WAPI_PSK BIT(12)
#define QWDI_WPA_KEY_MGMT_WAPI_CERT BIT(13)
#define QWDI_WPA_KEY_MGMT_CCKM BIT(14)
#define QWDI_WPA_KEY_MGMT_OSEN BIT(15)
#define QWDI_WPA_KEY_MGMT_IEEE8021X_SUITE_B BIT(16)
#define QWDI_WPA_KEY_MGMT_IEEE8021X_SUITE_B_192 BIT(17)

/* HT Capabilities Info field within HT Capabilities element from ieee802_11_defs.h */
#define QWDI_HT_CAP_INFO_LDPC_CODING_CAP        ((unsigned short) BIT(0))
#define QWDI_HT_CAP_INFO_SUPP_CHANNEL_WIDTH_SET ((unsigned short) BIT(1))
#define QWDI_HT_CAP_INFO_SMPS_MASK              ((unsigned short) (BIT(2) | BIT(3)))
#define QWDI_HT_CAP_INFO_SMPS_STATIC            ((unsigned short) 0)
#define QWDI_HT_CAP_INFO_SMPS_DYNAMIC           ((unsigned short) BIT(2))
#define QWDI_HT_CAP_INFO_SMPS_DISABLED          ((unsigned short) (BIT(2) | BIT(3)))
#define QWDI_HT_CAP_INFO_GREEN_FIELD            ((unsigned short) BIT(4))
#define QWDI_HT_CAP_INFO_SHORT_GI20MHZ          ((unsigned short) BIT(5))
#define QWDI_HT_CAP_INFO_SHORT_GI40MHZ          ((unsigned short) BIT(6))
#define QWDI_HT_CAP_INFO_TX_STBC                ((unsigned short) BIT(7))
#define QWDI_HT_CAP_INFO_RX_STBC_MASK           ((unsigned short) (BIT(8) | BIT(9)))
#define QWDI_HT_CAP_INFO_RX_STBC_1              ((unsigned short) BIT(8))
#define QWDI_HT_CAP_INFO_RX_STBC_12             ((unsigned short) BIT(9))
#define QWDI_HT_CAP_INFO_RX_STBC_123            ((unsigned short) (BIT(8) | BIT(9)))
#define QWDI_HT_CAP_INFO_DELAYED_BA             ((unsigned short) BIT(10))
#define QWDI_HT_CAP_INFO_MAX_AMSDU_SIZE         ((unsigned short) BIT(11))
#define QWDI_HT_CAP_INFO_DSSS_CCK40MHZ          ((unsigned short) BIT(12))
/* B13 - Reserved (was PSMP support during P802.11n development) */
#define QWDI_HT_CAP_INFO_40MHZ_INTOLERANT       ((unsigned short) BIT(14))
#define QWDI_HT_CAP_INFO_LSIG_TXOP_PROTECT_SUPPORT  ((unsigned short) BIT(15))

/* HT Extended Capabilities field within HT Capabilities element */
#define QWDI_EXT_HT_CAP_INFO_PCO         ((unsigned short) BIT(0))
#define QWDI_EXT_HT_CAP_INFO_PCO_TRANS_TIME_MASK ((unsigned short) (BIT(1) | BIT(2)))
#define QWDI_EXT_HT_CAP_INFO_TRANS_TIME_OFFSET   1
/* B3..B7 - Reserved */
#define QWDI_EXT_HT_CAP_INFO_MCS_FEEDBACK_MASK   ((unsigned short) (BIT(8) | BIT(9)))
#define QWDI_EXT_HT_CAP_INFO_MCS_FEEDBACK_OFFSET 8
#define QWDI_EXT_HT_CAP_INFO_HTC_SUPPORT     ((unsigned short) BIT(10))
#define QWDI_EXT_HT_CAP_INFO_RD_RESPONDER        ((unsigned short) BIT(11))
/* B12..B15 - Reserved */

/* Transmit Beanforming Capabilities within HT Capabilities element */
#define QWDI_TX_BF_CAP_IMPLICIT_TXBF_RX_CAP ((unsigned long) BIT(0))
#define QWDI_TX_BF_CAP_RX_STAGGERED_SOUNDING_CAP ((unsigned long) BIT(1))
#define QWDI_TX_BF_CAP_TX_STAGGERED_SOUNDING_CAP ((unsigned long) BIT(2))
#define QWDI_TX_BF_CAP_RX_NDP_CAP ((unsigned long) BIT(3))
#define QWDI_TX_BF_CAP_TX_NDP_CAP ((unsigned long) BIT(4))
#define QWDI_TX_BF_CAP_IMPLICIT_TX_BF_CAP ((unsigned long) BIT(5))
#define QWDI_TX_BF_CAP_CALIBRATION_MASK ((unsigned long) (BIT(6) | BIT(7))
#define QWDI_TX_BF_CAP_CALIB_OFFSET 6
#define QWDI_TX_BF_CAP_EXPLICIT_CSI_TXBF_CAP ((unsigned long) BIT(8))
#define QWDI_TX_BF_CAP_EXPLICIT_NONCOMPR_STEERING_CAP ((unsigned long) BIT(9))
#define QWDI_TX_BF_CAP_EXPLICIT_COMPR_STEERING_CAP ((unsigned long) BIT(10))
#define QWDI_TX_BF_CAP_EXPLICIT_TX_BF_CSI_FEEDBACK_MASK ((unsigned long) (BIT(10) | BIT(11)))
#define QWDI_TX_BF_CAP_EXPLICIT_BF_CSI_FEEDBACK_OFFSET 11
#define QWDI_TX_BF_CAP_EXPLICIT_UNCOMPR_STEERING_MATRIX_FEEDBACK_OFFSET 13
#define QWDI_TX_BF_CAP_EXPLICIT_COMPRESSED_STEERING_MATRIX_FEEDBACK_OFFSET 15
#define QWDI_TX_BF_CAP_MINIMAL_GROUPING_OFFSET 17
#define QWDI_TX_BF_CAP_CSI_NUM_BEAMFORMER_ANT_OFFSET 19
#define QWDI_TX_BF_CAP_UNCOMPRESSED_STEERING_MATRIX_BEAMFORMER_ANT_OFFSET 21
#define QWDI_TX_BF_CAP_COMPRESSED_STEERING_MATRIX_BEAMFORMER_ANT_OFFSET 23
#define QWDI_TX_BF_CAP_SCI_MAX_OF_ROWS_BEANFORMER_SUPPORTED_OFFSET 25
#define QWDI_TX_BF_CAP_CHANNEL_ESTIMATION_CAP_MASK ((unsigned long) (BIT(27) | BIT(28)))
#define QWDI_TX_BF_CAP_CHANNEL_ESTIMATION_CAP_OFFSET 27
/* B29..B31 - Reserved */

/* ASEL Capability field within HT Capabilities element */
#define QWDI_ASEL_CAP_ASEL_CAPABLE ((unsigned char) BIT(0))
#define QWDI_ASEL_CAP_EXPLICIT_CSI_FEEDBACK_BASED_TX_AS_CAP ((unsigned char) BIT(1))
#define QWDI_ASEL_CAP_ANT_INDICES_FEEDBACK_BASED_TX_AS_CAP ((unsigned char) BIT(2))
#define QWDI_ASEL_CAP_EXPLICIT_CSI_FEEDBACK_CAP ((unsigned char) BIT(3))
#define QWDI_ASEL_CAP_ANT_INDICES_FEEDBACK_CAP ((unsigned char) BIT(4))
#define QWDI_ASEL_CAP_RX_AS_CAP ((unsigned char) BIT(5))
#define QWDI_ASEL_CAP_TX_SOUNDING_PPDUS_CAP ((unsigned char) BIT(6))
/* B7 - Reserved */

/* First octet of HT Operation Information within HT Operation element */
#define QWDI_HT_INFO_HT_PARAM_SECONDARY_CHNL_OFF_MASK    ((unsigned char) BIT(0) | BIT(1))
#define QWDI_HT_INFO_HT_PARAM_SECONDARY_CHNL_ABOVE       ((unsigned char) BIT(0))
#define QWDI_HT_INFO_HT_PARAM_SECONDARY_CHNL_BELOW       ((unsigned char) BIT(0) | BIT(1))
#define QWDI_HT_INFO_HT_PARAM_STA_CHNL_WIDTH         ((unsigned char) BIT(2))
#define QWDI_HT_INFO_HT_PARAM_RIFS_MODE          ((unsigned char) BIT(3))
/* B4..B7 - Reserved */

/* HT Protection (B8..B9 of HT Operation Information) */
#define QWDI_HT_PROT_NO_PROTECTION           0
#define QWDI_HT_PROT_NONMEMBER_PROTECTION    1
#define QWDI_HT_PROT_20MHZ_PROTECTION        2
#define QWDI_HT_PROT_NON_HT_MIXED            3
/* Bits within ieee80211_ht_operation::operation_mode (BIT(0) maps to B8 in
 * HT Operation Information) */
#define QWDI_HT_OPER_OP_MODE_HT_PROT_MASK ((unsigned short) (BIT(0) | BIT(1))) /* B8..B9 */
#define QWDI_HT_OPER_OP_MODE_NON_GF_HT_STAS_PRESENT  ((unsigned short) BIT(2)) /* B10 */
/* BIT(3), i.e., B11 in HT Operation Information field - Reserved */
#define QWDI_HT_OPER_OP_MODE_OBSS_NON_HT_STAS_PRESENT    ((unsigned short) BIT(4)) /* B12 */
/* BIT(5)..BIT(15), i.e., B13..B23 - Reserved */

/* Last two octets of HT Operation Information (BIT(0) = B24) */
/* B24..B29 - Reserved */
#define QWDI_HT_OPER_PARAM_DUAL_BEACON           ((unsigned short) BIT(6))
#define QWDI_HT_OPER_PARAM_DUAL_CTS_PROTECTION       ((unsigned short) BIT(7))
#define QWDI_HT_OPER_PARAM_STBC_BEACON           ((unsigned short) BIT(8))
#define QWDI_HT_OPER_PARAM_LSIG_TXOP_PROT_FULL_SUPP      ((unsigned short) BIT(9))
#define QWDI_HT_OPER_PARAM_PCO_ACTIVE            ((unsigned short) BIT(10))
#define QWDI_HT_OPER_PARAM_PCO_PHASE             ((unsigned short) BIT(11))
/* B36..B39 - Reserved */

#define QWDI_BSS_MEMBERSHIP_SELECTOR_VHT_PHY 126
#define QWDI_BSS_MEMBERSHIP_SELECTOR_HT_PHY 127

/* VHT Defines ieee802_11_defs.h */
#define QWDI_VHT_CAP_MAX_MPDU_LENGTH_7991                ((unsigned long) BIT(0))
#define QWDI_VHT_CAP_MAX_MPDU_LENGTH_11454               ((unsigned long) BIT(1))
#define QWDI_VHT_CAP_MAX_MPDU_LENGTH_MASK                ((unsigned long) BIT(0) | BIT(1))
#define QWDI_VHT_CAP_MAX_MPDU_LENGTH_MASK_SHIFT          0
#define QWDI_VHT_CAP_SUPP_CHAN_WIDTH_160MHZ              ((unsigned long) BIT(2))
#define QWDI_VHT_CAP_SUPP_CHAN_WIDTH_160_80PLUS80MHZ     ((unsigned long) BIT(3))
#define QWDI_VHT_CAP_SUPP_CHAN_WIDTH_MASK                ((unsigned long) BIT(2) | BIT(3))
#define QWDI_VHT_CAP_SUPP_CHAN_WIDTH_MASK_SHIFT          2
#define QWDI_VHT_CAP_RXLDPC                              ((unsigned long) BIT(4))
#define QWDI_VHT_CAP_SHORT_GI_80                         ((unsigned long) BIT(5))
#define QWDI_VHT_CAP_SHORT_GI_160                        ((unsigned long) BIT(6))
#define QWDI_VHT_CAP_TXSTBC                              ((unsigned long) BIT(7))
#define QWDI_VHT_CAP_RXSTBC_1                            ((unsigned long) BIT(8))
#define QWDI_VHT_CAP_RXSTBC_2                            ((unsigned long) BIT(9))
#define QWDI_VHT_CAP_RXSTBC_3                            ((unsigned long) BIT(8) | BIT(9))
#define QWDI_VHT_CAP_RXSTBC_4                            ((unsigned long) BIT(10))
#define QWDI_VHT_CAP_RXSTBC_MASK                         ((unsigned long) BIT(8) | BIT(9) | BIT(10))
#define QWDI_VHT_CAP_RXSTBC_MASK_SHIFT                   8
#define QWDI_VHT_CAP_SU_BEAMFORMER_CAPABLE               ((unsigned long) BIT(11))
#define QWDI_VHT_CAP_SU_BEAMFORMEE_CAPABLE               ((unsigned long) BIT(12))
#define QWDI_VHT_CAP_BEAMFORMEE_STS_MAX                  ((unsigned long) BIT(13) | BIT(14) | BIT(15))
#define QWDI_VHT_CAP_BEAMFORMEE_STS_MAX_SHIFT            13
#define QWDI_VHT_CAP_BEAMFORMEE_STS_OFFSET               13
#define QWDI_VHT_CAP_SOUNDING_DIMENSION_MAX              ((unsigned long) BIT(16) | BIT(17) | BIT(18))
#define QWDI_VHT_CAP_SOUNDING_DIMENSION_MAX_SHIFT        16
#define QWDI_VHT_CAP_SOUNDING_DIMENSION_OFFSET           16
#define QWDI_VHT_CAP_MU_BEAMFORMER_CAPABLE               ((unsigned long) BIT(19))
#define QWDI_VHT_CAP_MU_BEAMFORMEE_CAPABLE               ((unsigned long) BIT(20))
#define QWDI_VHT_CAP_VHT_TXOP_PS                         ((unsigned long) BIT(21))
#define QWDI_VHT_CAP_HTC_VHT                             ((unsigned long) BIT(22))

#define QWDI_VHT_CAP_MAX_A_MPDU_LENGTH_EXPONENT_1        ((unsigned long) BIT(23))
#define QWDI_VHT_CAP_MAX_A_MPDU_LENGTH_EXPONENT_2        ((unsigned long) BIT(24))
#define QWDI_VHT_CAP_MAX_A_MPDU_LENGTH_EXPONENT_3        ((unsigned long) BIT(23) | BIT(24))
#define QWDI_VHT_CAP_MAX_A_MPDU_LENGTH_EXPONENT_4        ((unsigned long) BIT(25))
#define QWDI_VHT_CAP_MAX_A_MPDU_LENGTH_EXPONENT_5        ((unsigned long) BIT(23) | BIT(25))
#define QWDI_VHT_CAP_MAX_A_MPDU_LENGTH_EXPONENT_6        ((unsigned long) BIT(24) | BIT(25))
#define QWDI_VHT_CAP_MAX_A_MPDU_LENGTH_EXPONENT_MAX      ((unsigned long) BIT(23) | BIT(24) | BIT(25))
#define QWDI_VHT_CAP_MAX_A_MPDU_LENGTH_EXPONENT_MAX_SHIFT 23
#define QWDI_VHT_CAP_VHT_LINK_ADAPTATION_VHT_UNSOL_MFB   ((unsigned long) BIT(27))
#define QWDI_VHT_CAP_VHT_LINK_ADAPTATION_VHT_MRQ_MFB     ((unsigned long) BIT(26) | BIT(27))
#define QWDI_VHT_CAP_RX_ANTENNA_PATTERN                  ((unsigned long) BIT(28))
#define QWDI_VHT_CAP_TX_ANTENNA_PATTERN                  ((unsigned long) BIT(29))

#define QWDI_VHT_OPMODE_CHANNEL_WIDTH_MASK           ((unsigned char) BIT(0) | BIT(1))
#define QWDI_VHT_OPMODE_CHANNEL_RxNSS_MASK           ((unsigned char) BIT(4) | BIT(5) | BIT(6))
#define QWDI_VHT_OPMODE_NOTIF_RX_NSS_SHIFT           4

#define QWDI_VHT_RX_NSS_MAX_STREAMS     8

/* VHT channel widths */
#define QWDI_VHT_CHANWIDTH_USE_HT    0
#define QWDI_VHT_CHANWIDTH_80MHZ     1
#define QWDI_VHT_CHANWIDTH_160MHZ    2
#define QWDI_VHT_CHANWIDTH_80P80MHZ  3


/* HT Capabilities element */
struct qwdi_ieee80211_ht_cap {
    unsigned short ht_capabilities_info; /* little endian */
    unsigned char  a_mpdu_params;
    unsigned char  supported_mcs_set[16];
    unsigned short ht_extended_capabilities; /* little endian */
    unsigned int   tx_bf_capability_info; /* little endian */
    unsigned char  asel_capabilities;
} __attribute__((packed));

struct qwdi_ieee80211_vht_cap {
    unsigned int vht_capabilities_info;/* little endian */
    struct {
        unsigned short rx_map; /*little endian */
        unsigned short rx_highest;/*little endian */
        unsigned short tx_map;/*little endian */
        unsigned short tx_highest;/*little endian */
    } vht_supported_mcs_set;
} __attribute__((packed));

#define QWDI_WLAN_FC_TYPE_MGMT      0
#define QWDI_WLAN_FC_TYPE_CTRL      1
#define QWDI_WLAN_FC_TYPE_DATA      2

#define QWDI_WLAN_FC_STYPE_ACTION       13

/*From ap_config.h */
#define QWDI_NUM_WEP_KEYS 4


enum qwdi_set_band {
        QWDI_WPA_SETBAND_AUTO,
        QWDI_WPA_SETBAND_5G,
        QWDI_WPA_SETBAND_2G
};

#define QWDI_SCANTYPE_ACTIVE            0   /* d11 scan active */
#define QWDI_SCANTYPE_PASSIVE           1   /* d11 scan passive */

#define QWDI_SCAN_REQ_GUI_SCAN 2
#define QWDI_SCAN_REQ_APP_SCAN 3

/**
 * defines for driver capabilities structure elements
 * extracted from driver.h wpa_driver_capa structure definition
 */
#define QWDI_WPA_DRIVER_CAPA_KEY_MGMT_WPA        0x00000001
#define QWDI_WPA_DRIVER_CAPA_KEY_MGMT_WPA2       0x00000002
#define QWDI_WPA_DRIVER_CAPA_KEY_MGMT_WPA_PSK    0x00000004
#define QWDI_WPA_DRIVER_CAPA_KEY_MGMT_WPA2_PSK   0x00000008
#define QWDI_WPA_DRIVER_CAPA_KEY_MGMT_WPA_NONE   0x00000010
#define QWDI_WPA_DRIVER_CAPA_KEY_MGMT_FT     0x00000020
#define QWDI_WPA_DRIVER_CAPA_KEY_MGMT_FT_PSK     0x00000040
#define QWDI_WPA_DRIVER_CAPA_KEY_MGMT_WAPI_PSK   0x00000080
#define QWDI_WPA_DRIVER_CAPA_KEY_MGMT_SUITE_B    0x00000100
#define QWDI_WPA_DRIVER_CAPA_KEY_MGMT_SUITE_B_192    0x00000200
#define QWDI_WPA_DRIVER_CAPA_KEY_MGMT_OWE        0x00000400
#define QWDI_WPA_DRIVER_CAPA_KEY_MGMT_DPP        0x00000800
#define QWDI_WPA_DRIVER_CAPA_KEY_MGMT_FILS_SHA256    0x00001000
#define QWDI_WPA_DRIVER_CAPA_KEY_MGMT_FILS_SHA384    0x00002000
#define QWDI_WPA_DRIVER_CAPA_KEY_MGMT_FT_FILS_SHA256 0x00004000
#define QWDI_WPA_DRIVER_CAPA_KEY_MGMT_FT_FILS_SHA384 0x00008000
#define QWDI_WPA_DRIVER_CAPA_KEY_MGMT_SAE        0x00010000

#define QWDI_WPA_DRIVER_CAPA_ENC_WEP40          0x00000001
#define QWDI_WPA_DRIVER_CAPA_ENC_WEP104         0x00000002
#define QWDI_WPA_DRIVER_CAPA_ENC_TKIP           0x00000004
#define QWDI_WPA_DRIVER_CAPA_ENC_CCMP           0x00000008
#define QWDI_WPA_DRIVER_CAPA_ENC_WEP128         0x00000010
#define QWDI_WPA_DRIVER_CAPA_ENC_GCMP           0x00000020
#define QWDI_WPA_DRIVER_CAPA_ENC_GCMP_256       0x00000040
#define QWDI_WPA_DRIVER_CAPA_ENC_CCMP_256       0x00000080
#define QWDI_WPA_DRIVER_CAPA_ENC_BIP            0x00000100
#define QWDI_WPA_DRIVER_CAPA_ENC_BIP_GMAC_128   0x00000200
#define QWDI_WPA_DRIVER_CAPA_ENC_BIP_GMAC_256   0x00000400
#define QWDI_WPA_DRIVER_CAPA_ENC_BIP_CMAC_256   0x00000800
#define QWDI_WPA_DRIVER_CAPA_ENC_GTK_NOT_USED   0x00001000

#define QWDI_WPA_DRIVER_AUTH_OPEN       0x00000001
#define QWDI_WPA_DRIVER_AUTH_SHARED     0x00000002
#define QWDI_WPA_DRIVER_AUTH_LEAP       0x00000004

/* Driver generated WPA/RSN IE */
#define QWDI_WPA_DRIVER_FLAGS_DRIVER_IE                 0x00000001
/* Driver needs static WEP key setup after association command */
#define QWDI_WPA_DRIVER_FLAGS_SET_KEYS_AFTER_ASSOC      0x00000002
/* Driver takes care of all DFS operations */
#define QWDI_WPA_DRIVER_FLAGS_DFS_OFFLOAD               0x00000004
/** Driver takes care of RSN 4-way handshake internally; PMK is configured with
 * struct wpa_driver_ops::set_key using alg = WPA_ALG_PMK */
#define QWDI_WPA_DRIVER_FLAGS_4WAY_HANDSHAKE_8021X       0x00000008
/** Driver is for a wired Ethernet interface */
#define QWDI_WPA_DRIVER_FLAGS_WIRED                     0x00000010
/* Driver provides separate commands for authentication and association (SME in
 * wpa_supplicant). */
#define QWDI_WPA_DRIVER_FLAGS_SME                       0x00000020
/* Driver supports AP mode */
#define QWDI_WPA_DRIVER_FLAGS_AP                        0x00000040
/* Driver needs static WEP key setup after association has been completed */
#define QWDI_WPA_DRIVER_FLAGS_SET_KEYS_AFTER_ASSOC_DONE 0x00000080
/* Driver supports dynamic HT 20/40 MHz channel changes during BSS lifetime */
#define WPA_DRIVER_FLAGS_HT_2040_COEX                   0x00000100
/* Driver supports concurrent P2P operations */
#define QWDI_WPA_DRIVER_FLAGS_P2P_CONCURRENT            0x00000200
/*
 * Driver uses the initial interface as a dedicated management interface, i.e.,
 * it cannot be used for P2P group operations or non-P2P purposes.
 */
#define QWDI_WPA_DRIVER_FLAGS_P2P_DEDICATED_INTERFACE   0x00000400
/* This interface is P2P capable (P2P GO or P2P Client) */
#define QWDI_WPA_DRIVER_FLAGS_P2P_CAPABLE               0x00000800
/* Driver supports station and key removal when stopping an AP */
#define QWDI_WPA_DRIVER_FLAGS_AP_TEARDOWN_SUPPORT       0x00001000
/*
 * Driver uses the initial interface for P2P management interface and non-P2P
 * purposes (e.g., connect to infra AP), but this interface cannot be used for
 * P2P group operations.
 */
#define QWDI_WPA_DRIVER_FLAGS_P2P_MGMT_AND_NON_P2P      0x00002000
/*
 * Driver is known to use sane error codes, i.e., when it indicates that
 * something (e.g., association) fails, there was indeed a failure and the
 * operation does not end up getting completed successfully later.
 */
#define QWDI_WPA_DRIVER_FLAGS_SANE_ERROR_CODES          0x00004000
/* Driver supports off-channel TX */
#define QWDI_WPA_DRIVER_FLAGS_OFFCHANNEL_TX             0x00008000
/* Driver indicates TX status events for EAPOL Data frames */
#define QWDI_WPA_DRIVER_FLAGS_EAPOL_TX_STATUS           0x00010000
/* Driver indicates TX status events for Deauth/Disassoc frames */
#define QWDI_WPA_DRIVER_FLAGS_DEAUTH_TX_STATUS          0x00020000
/* Driver supports roaming (BSS selection) in firmware */
#define QWDI_WPA_DRIVER_FLAGS_BSS_SELECTION             0x00040000
/* Driver supports operating as a TDLS peer */
#define QWDI_WPA_DRIVER_FLAGS_TDLS_SUPPORT              0x00080000
/* Driver requires external TDLS setup/teardown/discovery */
#define QWDI_WPA_DRIVER_FLAGS_TDLS_EXTERNAL_SETUP       0x00100000
/* Driver indicates support for Probe Response offloading in AP mode */
#define QWDI_WPA_DRIVER_FLAGS_PROBE_RESP_OFFLOAD        0x00200000
/* Driver supports U-APSD in AP mode */
#define QWDI_WPA_DRIVER_FLAGS_AP_UAPSD                  0x00400000
/* Driver supports inactivity timer in AP mode */
#define QWDI_WPA_DRIVER_FLAGS_INACTIVITY_TIMER          0x00800000
/* Driver expects user space implementation of MLME in AP mode */
#define QWDI_WPA_DRIVER_FLAGS_AP_MLME                   0x01000000
/* Driver supports SAE with user space SME */
#define QWDI_WPA_DRIVER_FLAGS_SAE                       0x02000000
/* Driver makes use of OBSS scan mechanism in wpa_supplicant */
#define QWDI_WPA_DRIVER_FLAGS_OBSS_SCAN                 0x04000000
/* Driver supports IBSS (Ad-hoc) mode */
#define QWDI_WPA_DRIVER_FLAGS_IBSS                      0x08000000
/* Driver supports radar detection */
#define QWDI_WPA_DRIVER_FLAGS_RADAR                     0x10000000
/* Driver supports a dedicated interface for P2P Device */
#define QWDI_WPA_DRIVER_FLAGS_DEDICATED_P2P_DEVICE      0x20000000
/* Driver supports QoS Mapping */
#define QWDI_WPA_DRIVER_FLAGS_QOS_MAPPING               0x40000000
/* Driver supports CSA in AP mode */
#define QWDI_WPA_DRIVER_FLAGS_AP_CSA                    0x80000000
/* Driver supports mesh */
#define QWDI_WPA_DRIVER_FLAGS_MESH                   0x0000000100000000ULL
/* Driver support ACS offload */
#define QWDI_WPA_DRIVER_FLAGS_ACS_OFFLOAD            0x0000000200000000ULL
/* Driver supports key management offload */
#define QWDI_WPA_DRIVER_FLAGS_KEY_MGMT_OFFLOAD       0x0000000400000000ULL
/* Driver supports TDLS channel switching */
#define QWDI_WPA_DRIVER_FLAGS_TDLS_CHANNEL_SWITCH    0x0000000800000000ULL
/* Driver supports IBSS with HT datarates */
#define QWDI_WPA_DRIVER_FLAGS_HT_IBSS                0x0000001000000000ULL
/* Driver supports IBSS with VHT datarates */
#define QWDI_WPA_DRIVER_FLAGS_VHT_IBSS               0x0000002000000000ULL
/* Driver supports automatic band selection */
#define QWDI_WPA_DRIVER_FLAGS_SUPPORT_HW_MODE_ANY    0x0000004000000000ULL
/** Driver supports simultaneous off-channel operations */
#define QWDI_WPA_DRIVER_FLAGS_OFFCHANNEL_SIMULTANEOUS    0x0000008000000000ULL
/** Driver supports full AP client state */
#define QWDI_WPA_DRIVER_FLAGS_FULL_AP_CLIENT_STATE   0x0000010000000000ULL
/** Driver supports P2P Listen offload */
#define QWDI_WPA_DRIVER_FLAGS_P2P_LISTEN_OFFLOAD     0x0000020000000000ULL
/** Driver supports FILS */
#define QWDI_WPA_DRIVER_FLAGS_SUPPORT_FILS       0x0000040000000000ULL
/** Driver supports Beacon frame TX rate configuration (legacy rates) */
#define QWDI_WPA_DRIVER_FLAGS_BEACON_RATE_LEGACY 0x0000080000000000ULL
/** Driver supports Beacon frame TX rate configuration (HT rates) */
#define QWDI_WPA_DRIVER_FLAGS_BEACON_RATE_HT     0x0000100000000000ULL
/** Driver supports Beacon frame TX rate configuration (VHT rates) */
#define QWDI_WPA_DRIVER_FLAGS_BEACON_RATE_VHT    0x0000200000000000ULL
/** Driver supports mgmt_tx with random TX address in non-connected state */
#define QWDI_WPA_DRIVER_FLAGS_MGMT_TX_RANDOM_TA  0x0000400000000000ULL
/** Driver supports mgmt_tx with random TX addr in connected state */
#define QWDI_WPA_DRIVER_FLAGS_MGMT_TX_RANDOM_TA_CONNECTED    0x0000800000000000ULL
/** Driver supports better BSS reporting with sched_scan in connected mode */
#define QWDI_WPA_DRIVER_FLAGS_SCHED_SCAN_RELATIVE_RSSI   0x0001000000000000ULL
/** Driver supports HE capabilities */
#define QWDI_WPA_DRIVER_FLAGS_HE_CAPABILITIES    0x0002000000000000ULL
/** Driver supports FILS shared key offload */
#define QWDI_WPA_DRIVER_FLAGS_FILS_SK_OFFLOAD    0x0004000000000000ULL
/** Driver supports all OCE STA specific mandatory features */
#define QWDI_WPA_DRIVER_FLAGS_OCE_STA        0x0008000000000000ULL
/** Driver supports all OCE AP specific mandatory features */
#define QWDI_WPA_DRIVER_FLAGS_OCE_AP         0x0010000000000000ULL
/**
 * Driver supports all OCE STA-CFON specific mandatory features only.
 * If a driver sets this bit but not the %WPA_DRIVER_FLAGS_OCE_AP, the
 * userspace shall assume that this driver may not support all OCE AP
 * functionality but can support only OCE STA-CFON functionality.
 */
#define QWDI_WPA_DRIVER_FLAGS_OCE_STA_CFON       0x0020000000000000ULL
/** Driver supports MFP-optional in the connect command */
#define QWDI_WPA_DRIVER_FLAGS_MFP_OPTIONAL       0x0040000000000000ULL
/** Driver is a self-managed regulatory device */
#define QWDI_WPA_DRIVER_FLAGS_SELF_MANAGED_REGULATORY       0x0080000000000000ULL
/** Driver supports FTM responder functionality */
#define QWDI_WPA_DRIVER_FLAGS_FTM_RESPONDER      0x0100000000000000ULL
/** Driver support 4-way handshake offload for WPA-Personal */
#define QWDI_WPA_DRIVER_FLAGS_4WAY_HANDSHAKE_PSK 0x0200000000000000ULL

#ifdef __QNXNTO__ /* PR 281605 */
#define QWDI_WPA_DRIVER_FLAGS_MASK              0xFFFFFFFFFFFFFFFFULL
    /**
     * Indicates that the driver should perform the FT 4-way handshake
     * internally; PMK is configured with struct wpa_driver_ops::set_key
     * using alg = WPA_ALG_PMK. This is used in conjunction with
     * WPA_DRIVER_FLAGS_4WAY_HANDSHAKE since it's a special case on
     * top of that capability.
     */
#define QWDI_WPA_DRIVER_FLAGS_FT_4WAY_HANDSHAKE         0x01000000
#endif

#define QWDI_WPA_DRIVER_SMPS_MODE_STATIC             0x00000001
#define QWDI_WPA_DRIVER_SMPS_MODE_DYNAMIC            0x00000002

#define QWDI_WPA_DRIVER_PROBE_RESP_OFFLOAD_WPS      0x00000001
/* Driver Probe Response offloading support for WPS ver. 2 */
#define QWDI_WPA_DRIVER_PROBE_RESP_OFFLOAD_WPS2     0x00000002
/* Driver Probe Response offloading support for P2P */
#define QWDI_WPA_DRIVER_PROBE_RESP_OFFLOAD_P2P      0x00000004
/* Driver Probe Response offloading support for IEEE 802.11u (Interworking) */
#define QWDI_WPA_DRIVER_PROBE_RESP_OFFLOAD_INTERWORKING 0x00000008

/* Driver adds the DS Params Set IE in Probe Request frames */
#define QWDI_WPA_DRIVER_FLAGS_DS_PARAM_SET_IE_IN_PROBES 0x00000001
/* Driver adds the WFA TPC IE in Probe Request frames */
#define QWDI_WPA_DRIVER_FLAGS_WFA_TPC_IE_IN_PROBES      0x00000002
/* Driver handles quiet period requests */
#define QWDI_WPA_DRIVER_FLAGS_QUIET                     0x00000004
/**
 * Driver is capable of inserting the current TX power value into the body of
 * transmitted frames.
 * Background: Some Action frames include a TPC Report IE. This IE contains a
 * TX power field, which has to be updated by lower layers. One such Action
 * frame is Link Measurement Report (part of RRM). Another is TPC Report (part
 * of spectrum management). Note that this insertion takes place at a fixed
 * offset, namely the 6th byte in the Action frame body.
 */
#define QWDI_WPA_DRIVER_FLAGS_TX_POWER_INSERTION        0x00000008

/**
 * Driver supports RRM. With this support, the driver will accept to use RRM in
 * (Re)Association Request frames, without supporting quiet period.
 */
#define QWDI_WPA_DRIVER_FLAGS_SUPPORT_RRM            0x00000010

/** Driver supports setting the scan dwell time */
#define QWDI_WPA_DRIVER_FLAGS_SUPPORT_SET_SCAN_DWELL     0x00000020
/** Driver supports Beacon Report Measurement */
#define QWDI_WPA_DRIVER_FLAGS_SUPPORT_BEACON_REPORT      0x00000040


#define QWDI_EXTENDED_CAPA_MAX_LEN       128
#define QWDI_EXTENDED_CAPA_MASK_MAX_LEN  128


/**
 * struct  _QwdiWowlanTriggers - Driver capability information
 * modified from driver.h
 */
typedef struct _QwdiWowlanTriggers {
    unsigned char any;
    unsigned char disconnect;
    unsigned char magic_pkt;
    unsigned char gtk_rekey_failure;
    unsigned char eap_identity_req;
    unsigned char four_way_handshake;
    unsigned char rfkill_release;
} QwdiWowlanTriggers_t;

/**
 * struct _QwdiDriverCapa - Driver capability information
 * modified from driver.h  struct wpa_driver_capa
 */
typedef struct _QwdiDriverCapa {
    unsigned int key_mgmt;
    unsigned int enc;
    unsigned int auth;
    unsigned long long flags;
    unsigned int smps_modes;
    unsigned int wmm_ac_supported:1;
    unsigned int mac_addr_rand_scan_supported:1;
    unsigned int mac_addr_rand_sched_scan_supported:1;
    /* Maximum number of supported active probe SSIDs */
    int max_scan_ssids;
    /* Maximum number of supported active probe SSIDs for sched_scan */
    int max_sched_scan_ssids;
    /* Whether sched_scan (offloaded scanning) is supported */
    int sched_scan_supported;
    /* Maximum number of supported match sets for sched_scan */
    int max_match_sets;
    /* max_remain_on_chan - Maximum remain-on-channel duration in msec */
    unsigned int max_remain_on_chan;
    /* max_stations - Maximum number of associated stations the driver supports in AP mode*/
    unsigned int max_stations;
    /* probe_resp_offloads - Bitmap of supported protocols by the driver for Probe Response offloading */
    /* Driver Probe Response offloading support for WPS ver. 1 */
    unsigned int probe_resp_offloads;
    unsigned int max_acl_mac_addrs;
    /* Number of supported concurrent channels */
    unsigned int num_multichan_concurrent;
    /**
     * extended_capa - extended capabilities in driver/device
     *
     * Must be allocated and freed by driver and the pointers must be
     * valid for the lifetime of the driver, i.e., freed in deinit()
     * driver.h
     *   const unsigned char *extended_capa
     *   const unsigned char *extended_capa_mask
     *
     *   QWDI - for moment we will allocate fixed length arrays to hold these.
     *     will allocate length vars to keep up with how much of the arrays are used.
     */
    unsigned char extended_capa[QWDI_EXTENDED_CAPA_MAX_LEN];
    unsigned int extended_capa_len;

    unsigned char extended_capa_mask[QWDI_EXTENDED_CAPA_MASK_MAX_LEN];
        unsigned int extended_capa_mask_len;

        /* Qwdi version of struct wowlan_triggers */
        QwdiWowlanTriggers_t wowlan_triggers;

        unsigned long rrm_flags;
    /* Driver concurrency capabilities */
    unsigned int conc_capab;
    /* Maximum number of concurrent channels on 2.4 GHz */
    unsigned int max_conc_chan_2_4;
    /* Maximum number of concurrent channels on 5 GHz */
    unsigned int max_conc_chan_5_0;

        /* Maximum number of supported CSA counters */
        unsigned short max_csa_counters;

} QwdiDriverCapa_t;

/* p2p state */
#define QWDI_P2P_DISC_ST_SCAN   0
#define QWDI_P2P_DISC_ST_LISTEN 1
#define QWDI_P2P_DISC_ST_SEARCH 2

/************************************************************************************/
/* NOTE: there is a string array of names for the commands enum. If any changes are */
/* made to this enum you will need to update the ..src/drivers/driver_qwdi.c file.  */
/************************************************************************************/
/****************************/
/* QWDI command definitions */
/****************************/
enum qwdi_commands {
    QWDI_CMD_NONE,
    QWDI_CMD_GET_BSSID,
    QWDI_CMD_GET_SSID,
    QWDI_CMD_SET_KEY,
    QWDI_CMD_INIT,
    QWDI_CMD_DEINIT,
    QWDI_CMD_SET_PARAM,
    QWDI_CMD_SET_COUNTERMEASURES,
    QWDI_CMD_DEAUTHENTICATE,
    QWDI_CMD_ASSOCIATE,
    QWDI_CMD_ADD_PMKID,
    QWDI_CMD_REMOVE_PMKID,
    QWDI_CMD_FLUSH_PMKID,
    QWDI_CMD_GET_CAPA,
    QWDI_CMD_POLL,
    QWDI_CMD_GET_IFNAME,
    QWDI_CMD_GET_MAC_ADDR,
    QWDI_CMD_SEND_EAPOL,
    QWDI_CMD_SET_OPERSTATE,
    QWDI_CMD_MLME_SETPROTECTION,
    QWDI_CMD_GET_HW_FEATURE_DATA,
    QWDI_CMD_SEND_MLME,
    QWDI_CMD_UPDATE_FT_IES,
    QWDI_CMD_SEND_FT_ACTION,
    QWDI_CMD_GET_SCAN_RESULTS,
    QWDI_CMD_SET_COUNTRY,
    QWDI_CMD_GLOBAL_INIT,
    QWDI_CMD_GLOBAL_DEINIT,
    QWDI_CMD_INIT2,
    QWDI_CMD_GET_INTERFACES,
    QWDI_CMD_SCAN2,
    QWDI_CMD_SCHED_SCAN,
    QWDI_CMD_STOP_SCHED_SCAN,
    QWDI_CMD_AUTHENTICATE,
    QWDI_CMD_SET_AP,
    QWDI_CMD_HAPD_INIT,
    QWDI_CMD_HAPD_DEINIT,
    QWDI_CMD_SET_IEEE8021X,
    QWDI_CMD_SET_PRIVACY,
    QWDI_CMD_GET_SEQNUM,
    QWDI_CMD_FLUSH,
    QWDI_CMD_SET_GENERIC_ELEM,
    QWDI_CMD_READ_STA_DATA,
    QWDI_CMD_HAPD_SEND_EAPOL,
    QWDI_CMD_STA_DEAUTH,
    QWDI_CMD_STA_DISASSOC,
    QWDI_CMD_STA_REMOVE,
    QWDI_CMD_HAPD_GET_SSID,
    QWDI_CMD_HAPD_SET_SSID,
    QWDI_CMD_HAPD_SET_COUNTERMEASURES,
    QWDI_CMD_STA_ADD,
    QWDI_CMD_GET_INACT_SEC,
    QWDI_CMD_STA_CLEAR_STATS,
    QWDI_CMD_SET_FREQ,
    QWDI_CMD_SET_RTS,
    QWDI_CMD_SET_FRAG,
    QWDI_CMD_STA_SET_FLAGS,
    QWDI_CMD_SET_TX_QUEUE_PARAMS,
    QWDI_CMD_IF_ADD,
    QWDI_CMD_IF_REMOVE,
    QWDI_CMD_SET_STA_VLAN,
    QWDI_CMD_COMMIT,
    QWDI_CMD_SEND_ETHER,
    QWDI_CMD_SET_RADIUS_ACL_AUTH,
    QWDI_CMD_SET_RADIUS_ACL_EXPIRE,
    QWDI_CMD_SET_AP_WPS_IE,
    QWDI_CMD_SET_SUPP_PORT,
    QWDI_CMD_WDS_STA,
    QWDI_CMD_SEND_ACTION,
    QWDI_CMD_SEND_ACTION_CANCEL_WAIT,
    QWDI_CMD_REMAIN_ON_CHANNEL,
    QWDI_CMD_CANCEL_REMAIN_ON_CHANNEL,
    QWDI_CMD_SET_PRIORITY,
    QWDI_CMD_PROBE_REQ_REPORT,
    QWDI_CMD_DEINIT_AP,
    QWDI_CMD_DEINIT_P2P_CLI,
    QWDI_CMD_SUSPEND,
    QWDI_CMD_RESUME,
    QWDI_CMD_SIGNAL_MONITOR,
    QWDI_CMD_SEND_FRAME,
    QWDI_CMD_SHARED_FREQ,
    QWDI_CMD_GET_NOA,
    QWDI_CMD_SET_NOA,
    QWDI_CMD_SET_P2P_POWERSAVE,
    QWDI_CMD_AMPDU,
    QWDI_CMD_GET_RADIO_NAME,
    QWDI_CMD_P2P_FIND,
    QWDI_CMD_P2P_LISTEN,
    QWDI_CMD_P2P_CONNECT,
    QWDI_CMD_WPS_SUCCESS_CB,
    QWDI_CMD_P2P_GROUP_FORMATION_FAILED,
    QWDI_CMD_P2P_SET_PARAMS,
    QWDI_CMD_P2P_PROV_DISC_REQ,
    QWDI_CMD_P2P_SD_REQUEST,
    QWDI_CMD_P2P_SD_CANCEL_REQUEST,
    QWDI_CMD_P2P_SD_RESPONSE,
    QWDI_CMD_P2P_SERVICE_UPDATE,
    QWDI_CMD_P2P_REJECT,
    QWDI_CMD_P2P_INVITE,
    QWDI_CMD_SEND_TDLS_MGMT,
    QWDI_CMD_TDLS_OPER,
    QWDI_CMD_WNM_OPER,
    QWDI_CMD_SIGNAL_POLL,
    QWDI_CMD_SET_AUTHMODE,
    QWDI_CMD_SET_REKEY_INFO,
    QWDI_CMD_STA_ASSOC,
    QWDI_CMD_STA_AUTH,
    QWDI_CMD_ADD_TSPEC,
    QWDI_CMD_ADD_STA_NODE,
    QWDI_CMD_POLL_CLIENT,
    QWDI_CMD_RADIO_DISABLE,
    QWDI_CMD_SWITCH_CHANNEL,
    QWDI_CMD_DRIVER_CMD,
    QWDI_CMD_DISABLE,
    QWDI_CMD_ENABLE,
    /* additional commands for QWDI implementation*/
    QWDI_CMD_GET_SCAN_RESULTS_SIZE,
    QWDI_CMD_GET_ASSOC_INFO,
    QWDI_CMD_SET_P2P_MODE, /*may be used to release p2p resource in firmware if anything goes wrong, device can choose to ignore it */
    QWDI_CMD_P2P_SET_AP, /*for p2p GO role */
    QWDI_CMD_GET_SCAN_COMPLETE,
    QWDI_CMD_TEST,
    QWDI_CMD_VENDOR,    /* vendor pass through command */
    QWDI_CMD_SEND_EXTERNAL_AUTH_STATUS,
    QWDI_CMD_AFTER_LAST,
    QWDI_CMD_MAX = QWDI_CMD_AFTER_LAST - 1

};

/**
 * enum qwdi_events - Driver Event Types for wpa_supplicant_event() calls
 * modified from driver.h  enum   wpa_event_type
 * events are generally in the same order as is driver.h. for specific event
 * information see wpa_event_type enum in driver.h
 */
enum qwdi_events {
    QWDI_EVENT_NONE,
    QWDI_EVENT_ASSOC,
    QWDI_EVENT_DISASSOC,
    QWDI_EVENT_MICHAEL_MIC_FAILURE,
    QWDI_EVENT_SCAN_RESULTS,
    QWDI_EVENT_ASSOCINFO,
    QWDI_EVENT_INTERFACE_STATUS,
    QWDI_EVENT_PMKID_CANDIDATE,
    QWDI_EVENT_STKSTART,
    QWDI_EVENT_TDLS,
    QWDI_EVENT_FT_RESPONSE,
    QWDI_EVENT_IBSS_RSN_START,
    QWDI_EVENT_AUTH,
    QWDI_EVENT_DEAUTH,
    QWDI_EVENT_ASSOC_REJECT,
    QWDI_EVENT_AUTH_TIMED_OUT,
    QWDI_EVENT_ASSOC_TIMED_OUT,
    QWDI_EVENT_WPS_BUTTON_PUSHED,
    QWDI_EVENT_TX_STATUS,
    QWDI_EVENT_RX_FROM_UNKNOWN,
    QWDI_EVENT_RX_MGMT,
    QWDI_EVENT_RX_ACTION,
    QWDI_EVENT_REMAIN_ON_CHANNEL,
    QWDI_EVENT_CANCEL_REMAIN_ON_CHANNEL,
    QWDI_EVENT_RX_PROBE_REQ,
    QWDI_EVENT_NEW_STA,
    QWDI_EVENT_EAPOL_RX,
    QWDI_EVENT_SIGNAL_CHANGE,
    QWDI_EVENT_INTEFACE_ENABLED,
    QWDI_EVENT_INTERFACE_DISABLED,
    QWDI_EVENT_CHANNEL_LIST_CHANGED,
    QWDI_EVENT_INTERFACE_UNAVAILABLE,
    QWDI_EVENT_BEST_CHANNEL,
    QWDI_EVENT_UNPROT_DEAUTH,
    QWDI_EVENT_UNPROT_DISASSOC,
    QWDI_EVENT_STATION_LOW_ACK,
    QWDI_EVENT_IBSS_PEER_LOST,
    QWDI_EVENT_GTK_REKEY,
    QWDI_EVENT_SCHED_SCAN_STOPPED,
    QWDI_EVENT_DRIVER_CLIENT_POLL_OK,
    QWDI_EVENT_EAPOL_TX_STATUS,
    QWDI_EVENT_CH_SWITCH,
    QWDI_EVENT_CH_SWITCH_STARTED,
    QWDI_EVENT_WNM,
    QWDI_EVENT_CONNECT_FAILED_REASON,
    QWDI_EVENT_DFS_RADAR_DETECTED,
    QWDI_EVENT_DFS_CAC_FINISHED,
    QWDI_EVENT_DFS_CAC_ABORTED,
    QWDI_EVENT_DFS_NOP_FINISHED,
    QWDI_EVENT_SURVEY,
    QWDI_EVENT_SCAN_STARTED,
    QWDI_EVENT_AVOID_FREQUENCIES,
    QWDI_EVENT_NEW_PEER_CANDIDATE,
    QWDI_EVENT_ACS_CHANNEL_SELECTED,
    QWDI_EVENT_DFS_CAC_STARTED,
    QWDI_EVENT_FW_FAILURE,
    QWDI_EVENT_P2P_LO_STOP,
    /* these events are in driver.h not previously in "msg_2_8.h" file */
    QWDI_EVENT_BEACON_LOSS,
    QWDI_EVENT_DFS_PRE_CAC_EXPIRED,
    QWDI_EVENT_EXTERNAL_AUTH,
    QWDI_EVENT_PORT_AUTHORIZED,
    QWDI_EVENT_STATION_OPMODE_CHANGED,
    QWDI_EVENT_INTERFACE_MAC_CHANGED,
    QWDI_EVENT_WDS_STA_INTERFACE_STATUS,
    QWDI_EVENT_UPDATE_DH,
    /* additional events for QWDI. not found in driver.h*/
    QWDI_EVENT_P2P_DEV_FOUND,
    QWDI_EVENT_P2P_GO_NEG_REQ_RX,
    QWDI_EVENT_P2P_GO_NEWG_COMPLETED,
    QWDI_EVENT_P2P_PROV_DISC_REQUEST,
    QWDI_EVENT_P2P_PROV_DISC_RESPONSES,
    QWDI_EVENT_P2P_SD_REQUEST,
    QWDI_EVENT_P2P_SD_RESPONSE,
    QWDI_EVENT_START_ROAMING,
    QWDI_EVENT_ROAM_TO_SPECIFIC_AP,
    QWDI_EVENT_SMART_CONFIG_SYNC,
    QWDI_EVENT_SMART_CONFIG_DECODE,
    QWDI_EVENT_ASSOC_IND,
    QWDI_EVENT_TEST,
    QWDI_EVENT_AFTER_LAST,
    QWDI_EVENT_MAX = QWDI_EVENT_AFTER_LAST - 1
};


#define DCMD_QWDI_CMD 0x1000
/* Interface Type supported */
/**
 * enum QwdiIfType_e - Driver Interface Type
 * modified from driver.h  enum wpa_driver_if_type
 */
typedef enum {
    /**
     * QWDI_WPA_IF_STATION - Station mode interface
     */
    QWDI_WPA_IF_STATION,

    /**
     * QWDI_WPA_IF_AP_VLAN - AP mode VLAN interface
     *
     * This interface shares its address and Beacon frame with the main
     * BSS.
     */
    QWDI_WPA_IF_AP_VLAN,

    /**
     * QWDI_WPA_IF_AP_BSS - AP mode BSS interface
     *
     * This interface has its own address and Beacon frame.
     */
    QWDI_WPA_IF_AP_BSS,

    /**
     * QWDI_WPA_IF_P2P_GO - P2P Group Owner
     */
    QWDI_WPA_IF_P2P_GO,

    /**
     * QWDI_WPA_IF_P2P_CLIENT - P2P Client
     */
    QWDI_WPA_IF_P2P_CLIENT,

    /**
     * QWDI_WPA_IF_P2P_GROUP - P2P Group interface (will become either
     * QWDI_WPA_IF_P2P_GO or QWDI_ WPA_IF_P2P_CLIENT, but the role is not yet known)
     */
    QWDI_WPA_IF_P2P_GROUP,

    /**
     * QWDI_WPA_IF_P2P_DEVICE - P2P Device interface is used to indentify the
     * abstracted P2P Device function in the driver
     */
    QWDI_WPA_IF_P2P_DEVICE,

    /*
     * QWDI_WPA_IF_MESH - Mesh interface
     */
    QWDI_WPA_IF_MESH,

    /*
     * QWDI_WPA_IF_TDLS - TDLS offchannel interface (used for pref freq only)
     */
    QWDI_WPA_IF_TDLS,

    /*
     * QWDI_WPA_IF_IBSS - IBSS interface (used for pref freq only)
     */
    QWDI_WPA_IF_IBSS,

    /**
     * QWDI_WPA_IF_UNKNOWN - unknow yet
     */
    QWDI_WPA_IF_UNKNOWN

} QwdiIfType_e;


/* interface role, somewhat related to interface type */
typedef enum
{
    QWDI_IF_MODE_SYS,
    QWDI_IF_MODE_STA,
    QWDI_IF_MODE_SAP,
    QWDI_IF_MODE_P2PDEV,
    QWDI_IF_MODE_P2PCL,
    QWDI_IF_MODE_P2PGO,
    QWDI_IF_MODE_IBSS,
    QWDI_IF_MODE_NETMON,
    QWDI_IF_MODE_MAX,
    QWDI_IF_MODE_UNKNOWN
} QwdiIfMode_e;

/**
 * enum QwdiRegInitiator_e - Regulatory change initiator from
 * from driver.h  reg_change_initiator
 */
typedef enum
{
    QWDI_REGDOM_SET_BY_CORE,
    QWDI_REGDOM_SET_BY_USER,
    QWDI_REGDOM_SET_BY_DRIVER,
    QWDI_REGDOM_SET_BY_COUNTRY_IE,
    QWDI_REGDOM_BEACON_HINT
} QwdiRegInitiator_e;

/* Channel width definitions */
typedef enum {
        QWDI_CHAN_WIDTH_20_NOHT,
        QWDI_CHAN_WIDTH_20,
        QWDI_CHAN_WIDTH_40,
        QWDI_CHAN_WIDTH_80,
        QWDI_CHAN_WIDTH_80P80,
        QWDI_CHAN_WIDTH_160,
        QWDI_CHAN_WIDTH_UNKNOWN
} QWDIChanWidth_e;

/* Regulatory change types */
/**
 * enum QwdiRegType_e - Regulatory change initiator
 * from driver.h  reg_change_initiator
 */
typedef enum
{
    QWDI_REGDOM_TYPE_UNKNOWN,
    QWDI_REGDOM_TYPE_COUNTRY,
    QWDI_REGDOM_TYPE_WORLD,
    QWDI_REGDOM_TYPE_CUSTOM_WORLD,
    QWDI_REGDOM_TYPE_INTERSECTION
} QwdiRegType_e;

typedef struct _QwdiWmmAcParams {
    int cwmin;
    int cwmax;
    int aifs;
    int txop_limit; /* in units of 32us */
    int admission_control_mandatory;
} QwdiWmmAcParams_t;

typedef struct _QwdiTxQueueParams {
    int aifs;
    int cwmin;
    int cwmax;
    int burst; /* maximum burst time in 0.1 ms, i.e., 10 = 1 ms */
} QwdiTxQueueParams_t;


typedef struct _QwdiKey {
    unsigned int key_len;
    char    key[QWDI_MAX_KEY_LEN];
} QwdiKey_t;

typedef struct _QwdiWepKeys {
    unsigned char idx;
    QwdiKey_t keys[QWDI_NUM_WEP_KEYS];
    int keys_set;
    int default_len; /* key length used for dynamic key generation */
}QwdiWepKeys_t ;

/* Message header */
typedef struct _QwdiMsgHeader {
    unsigned int     command_id;  /* command identifier*/
    unsigned int     msg_size;    /* number of bytes in message payload; doesn't include this header*/
} QwdiMsgHeader_t;

#define QWDI_MAX_EVENT_DATA_SIZE 2048

typedef struct _QwdiEventHeader {
    unsigned int event_id;
    unsigned int event_size;
}QwdiEventHeader_t;

/* Event structure */
typedef struct _QwdiEvent {
    QwdiEventHeader_t hdr;
    unsigned char  eventBuffer[QWDI_MAX_EVENT_DATA_SIZE];
} QwdiEvent_t;

/**
 * enum struct QwdiWpaChannelInfo_t - Information about the current channel
 * @frequency: Center frequency of the primary 20 MHz channel
 * @chanwidth: Width of the current operating channel
 * @sec_channel: Location of the secondary 20 MHz channel (either +1 or -1).
 *  This field is only filled in when using a 40 MHz channel.
 * @center_frq1: Center frequency of frequency segment 0
 * @center_frq2: Center frequency of frequency segment 1 (for 80+80 channels)
 * @seg1_idx: Frequency segment 1 index when using a 80+80 channel. This is
 *  derived from center_frq2 for convenience.
 *
 *  from driver.h  struct wpa_channel_info.
 *  is used with the qwdi_get_channel_info() api function as yet
 *  not defined. enum QWDIChanWidth_e defined above
 */
typedef struct _QwdiWpaChannelInfo {
    unsigned long frequency;
    QWDIChanWidth_e chanwidth;
    int sec_channel;
    int center_frq1;
    int center_frq2;
    unsigned char seg1_idx;
} QwdiWpaChannelInfo_t;


/***********************/
/* QWDI VENDOR command */
/***********************/
/* vendor command request struct */
typedef struct _QwdiVendorCmdReq {
    unsigned int vendorId;              /* numeric value of <id> argument, 1st arg, driver specific */
    unsigned int vendorSubCmd;          /* numeric value of <cmd> argument, 2nd arg, driver specific */
    unsigned int payloadSize;           /* actual payload size of vendorCmdPayload[] */
    unsigned char vendorCmdPayload[1];  /* payload data, 3rd arg, varies based on command, reserve a spot for it */
} QwdiVendorCmdReq_t;

/* vendor command response struct */
typedef struct _QwdiVendorCmdRsp {
    unsigned int responseSize;          /* actual size of vendorRspPayload[] */
    unsigned char vendorRspPayload[1];  /* payload data varies based on command, reserve a spot for it */
} QwdiVendorCmdRsp_t;


typedef struct _QwdiInitReq {
    unsigned int version; /* supplicant's version */
    char ifname[QWDI_IFNAMSIZ+1];
} QwdiInitReq_t;

typedef struct _QwdiInitRsp {
    unsigned int version; /* driver preferred wpa_supplicant version*/
    QwdiIfType_e iftype;
    QwdiIfMode_e ifmode;
    int  scan_timeout; /* scan timeout in seconds */
    char ifname[QWDI_IFNAMSIZ+1];
    int  p2p_supported;
    int  roc_timeout_offset; /*remain on channel timeout offset in microseconds*/
    int  version_mismatch; /*driver can't support the version of wpa_supplicant */
} QwdiInitRsp_t;

/**
 * enum wps_mode - WPS mode
 * driver.h  enum wps_mode
 */
typedef enum
{
    /**
     * QWDI_WPS_MODE_NONE - No WPS provisioning being used
     */
    QWDI_WPS_MODE_NONE,

    /**
     * QWDI_WPS_MODE_OPEN - WPS provisioning with AP that is in open mode
     */
    QWDI_WPS_MODE_OPEN,

    /**
     * QWDI_WPS_MODE_PRIVACY - WPS provisioning with AP that is using protection
     */
    QWDI_WPS_MODE_PRIVACY
}  QwdiWPSMode_e;

/**
 * typedef enum QwdiHostapdHwMode_e - Hardware mode
 * from defs.h  enum hostapd_hw_mode - Hardware mode
 */
typedef enum {
    QWDI_HOSTAPD_MODE_IEEE80211B,
    QWDI_HOSTAPD_MODE_IEEE80211G,
    QWDI_HOSTAPD_MODE_IEEE80211A,
    QWDI_HOSTAPD_MODE_IEEE80211AD,
    QWDI_HOSTAPD_MODE_IEEE80211ANY,
    QWDI_NUM_HOSTAPD_MODES
} QwdiHostapdHwMode_e;

/**
 * struct QwdiHapdFreqParams_t - Channel parameters
 * from driver.h struct hostapd_freq_params see typedef below
 * QwdiSetFreqReq_t QwdiHapdFreqParams_t
 */
typedef struct _QwdiSetFreqReq {
    /**
     * mode - Mode/band (HOSTAPD_MODE_IEEE80211A, ..)
     */
    QwdiHostapdHwMode_e mode;
    /**
     * freq - Primary channel center frequency in MHz
     */
    int freq;
    /**
     * channel - Channel number
     */
    int channel;
    /**
      * ht_enabled - Whether HT is enabled
      */
    int ht_enabled;
    /**
     * sec_channel_offset - Secondary channel offset for HT40
     *
     * 0 = HT40 disabled,
     * -1 = HT40 enabled, secondary channel below primary,
     * 1 = HT40 enabled, secondary channel above primary
     */
    int sec_channel_offset;
    /**
     * vht_enabled - Whether VHT is enabled
     */
    int vht_enabled;
    /**
     * he_enabled - Whether HE is enabled
     */
    int he_enabled;
    /**
     * center_freq1 - Segment 0 center frequency in MHz
     *
     * Valid for both HT and VHT.
     */
    int center_freq1;
    /**
     * center_freq2 - Segment 1 center frequency in MHz
     *
     * Non-zero only for bandwidth 80 and an 80+80 channel
     */
    int center_freq2;
    /**
     * bandwidth - Channel bandwidth in MHz (20, 40, 80, 160)
     */
    int bandwidth;

    /* QNX - set by supplicant when it completes setup of */
    /* the structure  1 = valid, 0 = not valid            */
    int valid;

} QwdiSetFreqReq_t;

typedef QwdiSetFreqReq_t QwdiHapdFreqParams_t;


/**
 * from driver.h
 * struct wpa_driver_scan_ssid - SSIDs to scan for
 * @ssid_len - Length of the SSID in octets
 * @ssid - specific SSID to scan for (ProbeReq)
 *      %NULL or zero-length SSID is used to indicate active scan
 *      with wildcard SSID.
 */
typedef struct _QwdiSsid {
    unsigned int    ssid_len;
    char    ssid[QWDI_MAX_SSID_STR_LEN];
} QwdiSsid_t;

typedef struct _QwdiSchedScanPlan {
        unsigned long interval;   /* In seconds */
        unsigned long iterations; /* Zero to run infinitely */
} QwdiSchedScanPlan_t;


#define QWDI_SCAN_FLAG_SUPP_RATES   BIT(0) /* remove 802.11b rates from supported rates in probe request frames */
#define QWDI_SCAN_FLAG_FLUSH        BIT(1) /* flush old cached BSSes, only report BSSes detected after this scan request */
#define QWDI_SCAN_FLAG_LOW_PRIORITY BIT(2) /* driver to use a lower scan priority */
#define QWDI_SCAN_FLAG_RANDOM_ADDR  BIT(3) /* river to randomize MAC address */

#define QWDI_SCHED_SCAN_PLAN_ELEMENTS  16  /* QWDI define */

typedef struct _QwdiScanReq {
        /**
         * ssids - SSIDs to scan for
         * driver.h struct wpa_driver_scan_ssid ssids[WPAS_MAX_SCAN_SSIDS];
         */
    QwdiSsid_t    ssids[QWDI_MAX_SCAN_SSID];

        /**
         * num_ssids - Number of entries in ssids array
         * Zero indicates a request for a passive scan.
         * driver.h  size_t num_ssids;
         */
    unsigned long num_ssids;

        /**
         * extra_ies - Extra IE(s) to add into Probe Request or %NULL
         * driver.h  const u8 *extra_ies;
         */
    unsigned char extra_ies[QWDI_MAX_EXTRA_IES_LEN];

        /**
         * extra_ies_len - Length of extra_ies in octets
         * driver.h  size_t extra_ies_len;
         */
    unsigned short extra_ies_len;

        /**
         * freqs - Array of frequencies to scan or %NULL for all frequencies
         * The frequency is set in MHz. The array is zero-terminated.
         * driver.h   int *freqs;
         */
    unsigned int  freqs[QWDI_MAX_SCAN_FREQ];

    /**
         * num_freqs - number of frequencies used in the array
         * QWDI variable
         */
    unsigned int num_freqs;

        /**
         * filter_ssids - Filter for reporting SSIDs
         *
         * This optional parameter can be used to request the driver wrapper to
         * filter scan results to include only the specified SSIDs. %NULL
         * indicates that no filtering is to be done. This can be used to
         * reduce memory needs for scan results in environments that have large
         * number of APs with different SSIDs.
         *
         * The driver wrapper is allowed to take this allocated buffer into its
         * own use by setting the pointer to %NULL. In that case, the driver
         * wrapper is responsible for freeing the buffer with os_free() once it
         * is not needed anymore.
         *
         * driver.h
         *      struct wpa_driver_scan_filter {
         *      u8 ssid[SSID_MAX_LEN];
         *      size_t ssid_len;
         *   } *filter_ssids;
         *
         */
    QwdiSsid_t  filter_ssids[QWDI_MAX_SCAN_SSID];

        /**
         * num_filter_ssids - Number of entries in filter_ssids array
         * driver   size_t num_filter_ssids;
         */
    unsigned int num_filter_ssids;

        /**
         * filter_rssi - Filter by RSSI
         *
         * The driver may filter scan results in firmware to reduce host
         * wakeups and thereby save power. Specify the RSSI threshold in s32
         * dBm.
         * driver.h   s32 filter_rssi;
         */
    int           filter_rssi;

        /**
         * p2p_probe - Used to disable CCK (802.11b) rates for P2P probes
         *
         * When set, the driver is expected to remove rates 1, 2, 5.5, and 11
         * Mbps from the support rates element(s) in the Probe Request frames
         * and not to transmit the frames at any of those rates.
         * driver.h   unsigned int p2p_probe:1;
         */
        unsigned int p2p_probe:1;

        /**
         * only_new_results - Request driver to report only new results
         *
         * This is used to request the driver to report only BSSes that have
         * been detected after this scan request has been started, i.e., to
         * flush old cached BSS entries.
         * driver.h    unsigned int only_new_results:1;
         */
        unsigned int only_new_results:1;

        /**
         * low_priority - Requests driver to use a lower scan priority
         *
         * This is used to request the driver to use a lower scan priority
         * if it supports such a thing.
         * unsigned int   low_priority:1;
         */
        unsigned int low_priority:1;

        /**
         * mac_addr_rand - Requests driver to randomize MAC address
         * driver.h   unsigned int mac_addr_rand:1;
         */
        unsigned int mac_addr_rand:1;

        /**
         * mac_addr - MAC address used with randomization. The address cannot be
         * a multicast one, i.e., bit 0 of byte 0 should not be set.
         * driver.h   const u8 *mac_addr;
         */
    unsigned char mac_addr[QWDI_ETH_ALEN];

        /**
         * mac_addr_mask - MAC address mask used with randomization.
         *
         * Bits that are 0 in the mask should be randomized. Bits that are 1 in
         * the mask should be taken as is from mac_addr. The mask should not
         * allow the generation of a multicast address, i.e., bit 0 of byte 0
         * must be set.
         * driver.h   const u8 *mac_addr_mask;
         */
    unsigned char mac_addr_mask[QWDI_ETH_ALEN];

        /**
         * sched_scan_plans - Scan plans for scheduled scan
         *
         * Each scan plan consists of the number of iterations to scan and the
         * interval between scans. When a scan plan finishes (i.e., it was run
         * for the specified number of iterations), the next scan plan is
         * executed. The scan plans are executed in the order they appear in
         * the array (lower index first). The last scan plan will run infinitely
         * (until requested to stop), thus must not specify the number of
         * iterations. All other scan plans must specify the number of
         * iterations.
         * driver.h
         *   struct sched_scan_plan {
         *         u32 interval;    In seconds
         *         u32 iterations;  Zero to run infinitely
         *    } sched_scan_plans;
         */
    QwdiSchedScanPlan_t sched_scan_plan[QWDI_SCHED_SCAN_PLAN_ELEMENTS];

        /**
         * sched_scan_plans_num - Number of scan plans in sched_scan_plans array
         * driver.h   unsigned int sched_scan_plans_num;
         */
        unsigned int sched_scan_plans_num;

        /**
         * sched_scan_start_delay - Delay to use before starting the first scan
         *
         * Delay (in seconds) before scheduling first scan plan cycle. The
         * driver may ignore this parameter and start immediately (or at any
         * other time), if this feature is not supported.
         * driver.h  u32 sched_scan_start_delay;
         */
        unsigned long sched_scan_start_delay;

        /**
         * bssid - Specific BSSID to scan for
         *
         * This optional parameter can be used to replace the default wildcard
         * BSSID with a specific BSSID to scan for if results are needed from
         * only a single BSS. If NULL = not used
         * driver.h   const u8 *bssid;
         */
        unsigned char bssid[QWDI_ETH_ALEN];

        /**
         * int bssid_length - length of the bssid. 0 = not used
         * QWDI - variable
         */
        int bssid_length;

        /**
         * scan_cookie - Unique identification representing the scan request
         *
         * This scan_cookie carries a unique identification representing the
         * scan request if the host driver/kernel supports concurrent scan
         * requests. This cookie is returned from the corresponding driver
         * interface.
         *
         * Note: Unlike other parameters in this structure, scan_cookie is used
         * only to return information instead of setting parameters for the
         * scan.
         * driver.h   u64 scan_cookie
         */
        unsigned long long scan_cookie;

         /**
          * duration - Dwell time on each channel
          *
          * This optional parameter can be used to set the dwell time on each
          * channel. In TUs.
          * driver.h   u16 duration
          */
        unsigned short duration;

         /**
          * duration_mandatory - Whether the specified duration is mandatory
          *
          * If this is set, the duration specified by the %duration field is
          * mandatory (and the driver should reject the scan request if it is
          * unable to comply with the specified duration), otherwise it is the
          * maximum duration and the actual duration may be shorter.
          * driver.h   unsigned int duration_mandatory:1
          */
        unsigned int duration_mandatory:1;

         /**
          * relative_rssi_set - Whether relative RSSI parameters are set
          * driver.h   unsigned int relative_rssi_set:1;
          */
        unsigned int relative_rssi_set:1;

         /**
          * relative_rssi - Relative RSSI for reporting better BSSs
          *
          * Amount of RSSI by which a BSS should be better than the current
          * connected BSS to report the new BSS to user space.
          * driver.h   s8 relative_rssi;
          */
        char relative_rssi;

        /**
         * relative_adjust_band - Band to which RSSI should be adjusted
         *
         * The relative_adjust_rssi should be added to the band specified
         * by relative_adjust_band.
         * driver.h   enum set_band relative_adjust_band;
         * defs.h
         *   enum set_band {
         *      WPA_SETBAND_AUTO,
         *      WPA_SETBAND_5G,
         *      WPA_SETBAND_2G
         *      see above for QWDI version
         */
        enum qwdi_set_band relative_adjust_band;

        /**
         * relative_adjust_rssi - RSSI to be added to relative_adjust_band
         *
         * An amount of relative_band_rssi should be added to the BSSs that
         * belong to the band specified by relative_adjust_band while comparing
         * with other bands for BSS reporting.
         * driver.h   s8 relative_adjust_rssi
         */
        char relative_adjust_rssi;

        /***  TEMP ADD OF (int flag). Don't know if QWDI needs ***/
        int flag;

} QwdiScanReq_t;

typedef struct _Qwdi_Variable_IE {
    unsigned char type;
    unsigned char length; /*bytes in data */
    unsigned char data[1];
}Qwdi_Variable_IE_t;


typedef struct _Qwdi_Bssid_Info {
    unsigned int  length; /*total length of bssid info */
    unsigned char  mac_addr[QWDI_ETH_ALEN];
    QwdiSsid_t     ssid;
    unsigned int   flags;
    unsigned short caps;
    int            rssi;
    int            freq;
    unsigned short beacon_int;
    int            qual;
    int            noise;
    unsigned long long tsf;
    unsigned int   age;
    unsigned int est_throughput;
    int snr;
    unsigned int   ie_len;
    unsigned char  ie[1];
} Qwdi_Bssid_Info_t;

typedef struct _QwdiGetScanResultsRsp {
    unsigned int num_bssid;
    unsigned int list_size; /*bssid list size, equals all the length added together*/
    Qwdi_Bssid_Info_t bssid[1];
}QwdiGetScanResultsRsp_t;

typedef struct _QwdiGetScanResultsSizeRsp {
    int scan_result_size;
    int num_bssid;
    int set_timeout;
} QwdiGetScanResultsSizeRsp_t;

typedef struct _QwdiGetScanCompleteRsp {
    int completed;
    int set_timeout;
} QwdiGetScanCompleteRsp_t;

typedef enum _QwdiWpaAlg {
    QWDI_WPA_ALG_NONE,
    QWDI_WPA_ALG_WEP,
    QWDI_WPA_ALG_TKIP,
    QWDI_WPA_ALG_CCMP,
    QWDI_WPA_ALG_IGTK,
    QWDI_WPA_ALG_PMK,
    QWDI_WPA_ALG_GCMP,
    QWDI_WPA_ALG_SMS4,
    QWDI_WPA_ALG_KRK,
    QWDI_WPA_ALG_GCMP_256,
    QWDI_WPA_ALG_CCMP_256,
    QWDI_WPA_ALG_BIP_GMAC_128,
    QWDI_WPA_ALG_BIP_GMAC_256,
    QWDI_WPA_ALG_BIP_CMAC_256,
    QWDI_WAPI_ALG_SMS4
} QwdiWpaAlg_e;

typedef struct _QwdiSetKeyReq {
    char ifname[QWDI_IFNAMSIZ+1];
    QwdiWpaAlg_e   alg;
    unsigned char  addr[QWDI_ETH_ALEN];
    int            key_idx;
    int            set_tx;
    unsigned char  seq[QWDI_MAX_SEQ_LEN];
    int            seq_len;
    unsigned char  key[QWDI_MAX_KEY_LEN];
    int            key_len;
    int            key_mgmt;
} QwdiSetKeyReq_t;

typedef struct _QwdiGetMacAddrRsp {
    unsigned char  addr[QWDI_ETH_ALEN];
}QwdiGetMacAddrRsp_t;

typedef struct _QwdiHapdInitReq {
    unsigned int version; /*hostapd version */
    char ifname[QWDI_IFNAMSIZ+1];
    int  ieee80211n;
    unsigned short ht_capab;
    int  isolate;
    int  max_tx_power;
    int  wmm_uapsd;
    int  wmm_enabled;
    QwdiWmmAcParams_t wmm_ac_params[4];
    int  ieee80211ac;
    unsigned int vht_capab;
    QwdiTxQueueParams_t tx_queue_params[4];
} QwdiHapdInitReq_t;

typedef struct _QwdiHapdInitRsp {
    unsigned int version; /* driver preferred hostapd version */
    QwdiIfType_e iftype;
    unsigned char  addr[QWDI_ETH_ALEN];
    int          version_mismatch; /* driver can't support the version of hostapd */
} QwdiHapdInitRsp_t;

typedef struct _QwdiAssociateReq {
        /**
         * bssid - BSSID of the selected AP
         * This can be %NULL, if ap_scan=2 mode is used and the driver is
         * responsible for selecting with which BSS to associate.
         * driver.h const u8 *bssid;
         */
    unsigned char  bssid[QWDI_ETH_ALEN];
        /**
         * bssid_hint - BSSID of a proposed AP
         *
         * This indicates which BSS has been found a suitable candidate for
         * initial association for drivers that use driver/firmwate-based BSS
         * selection. Unlike the @bssid parameter, @bssid_hint does not limit
         * the driver from selecting other BSSes in the ESS.
         * driver.h const u8 *bssid_hint;
         */
    unsigned char  bssid_hint[QWDI_ETH_ALEN];

        /**
         * ssid - The selected SSID
         * driver.h const u8 *ssid;
         * driver.h ssid_len - Length of the SSID (1..32)
         * Allocate max 32 chars
         */
    QwdiSsid_t     ssid;

        /**
         * freq - channel parameters
         * driver.h struct hostapd_freq_params freq;
         */
    QwdiHapdFreqParams_t freq_params;

        /**
         * freq_hint - Frequency of the channel the proposed AP is using
         *
         * This provides a channel on which a suitable BSS has been found as a
         * hint for the driver. Unlike the @freq parameter, @freq_hint does not
         * limit the driver from selecting other channels for
         * driver/firmware-based BSS selection.
         * driver.h int freq_hint;
         */
    int            freq_hint;

        /**
         * bg_scan_period - Background scan period in seconds, 0 to disable
         * background scan, or -1 to indicate no change to default driver
         * configuration
         * driver.h int bg_scan_period;
         */
    int            bg_scan_period;

        /**
         * beacon_int - Beacon interval for IBSS or 0 to use driver default
         * driver.h int beacon_int;
         */
    int            beacon_int;

        /**
         * wpa_proto - Bitfield of WPA_PROTO_* values to indicate WPA/WPA2
         * driver.h unsigned int wpa_proto;
         */
    unsigned int   wpa_proto;

        /**
         * pairwise_suite - Selected pairwise cipher suite (WPA_CIPHER_*)
         *
         * This is usually ignored if @wpa_ie is used.
         * driver.h unsigned int pairwise_suite;
         */
    int pairwise_suite;

        /**
         * group_suite - Selected group cipher suite (WPA_CIPHER_*)
         *
         * This is usually ignored if @wpa_ie is used.
         * driver.h unsigned int group_suite;
         */
    int group_suite;

        /**
         * mgmt_group_suite - Selected group management cipher suite (WPA_CIPHER_*)
         *
         * This is usually ignored if @wpa_ie is used.
         * driver.h unsigned int mgmt_group_suite;
         */
    int mgmt_group_suite;

        /**
         * key_mgmt_suite - Selected key management suite (WPA_KEY_MGMT_*)
         *
         * This is usually ignored if @wpa_ie is used.
         * driver.h unsigned int key_mgmt_suite;
         */
    int key_mgmt_suite;

        /**
         * auth_alg - Allowed authentication algorithms
         * Bit field of WPA_AUTH_ALG_*
         * driver.h int auth_alg;
         */
    int            auth_alg;


        /**
         * mode - Operation mode (infra/ibss) IEEE80211_MODE_*
         * driver.h int mode;
         */
        int            mode;

        /**
         * QwdiKey_t struct for keys & key lengths array of 4
         *    key[QWDI_MAX_KEY_LEN] - WEP key for static WEP configuration
         *    key_len - actual WEP key length for static WEP configuration
         * driver.h const u8 *wep_key[4]; size_t wep_key_len[4];
         */
    QwdiKey_t      wep_key[QWDI_NUM_WEP_KEYS];

        /**
         * wep_tx_keyidx - WEP TX key index for static WEP configuration
         * driver.h int wep_tx_keyidx;
         */
    int            wep_tx_keyidx;

        /**
         * mgmt_frame_protection - IEEE 802.11w management frame protection
         * driver.h enum mfp_options mgmt_frame_protection;
         */
    int            mgmt_frame_protection;

        /**
         * passphrase - RSN passphrase for PSK
         *
         * This value is made available only for WPA/WPA2-Personal (PSK) and
         * only for drivers that set WPA_DRIVER_FLAGS_4WAY_HANDSHAKE. This is
         * the 8..63 character ASCII passphrase, if available. Please note that
         * this can be %NULL if passphrase was not used to generate the PSK. In
         * that case, the psk field must be used to fetch the PSK.
         * driver.h const char *passphrase;
         */
    int           passphrase_len;
    unsigned char passphrase[64];

        /**
         * psk - RSN PSK (alternative for passphrase for PSK)
         *
         * This value is made available only for WPA/WPA2-Personal (PSK) and
         * only for drivers that set WPA_DRIVER_FLAGS_4WAY_HANDSHAKE. This is
         * the 32-octet (256-bit) PSK, if available. The driver wrapper should
         * be prepared to handle %NULL value as an error.
         * driver.hconst u8 *psk;
         */
    unsigned char psk[QWDI_MAX_KEY_LEN];

        /**
         * drop_unencrypted - Enable/disable unencrypted frame filtering
         *
         * Configure the driver to drop all non-EAPOL frames (both receive and
         * transmit paths). Unencrypted EAPOL frames (ethertype 0x888e) must
         * still be allowed for key negotiation.
         * driver.h int drop_unencrypted;
         */
    int drop_unencrypted;

        /**
         * prev_bssid - Previously used BSSID in this ESS
         *
         * When not %NULL, this is a request to use reassociation instead of
         * association.
         * driver.h const u8 *prev_bssid;
         */
        unsigned char prev_bssid[QWDI_ETH_ALEN];

        /* qwdi private - if 1 use prev_bssid. derive from prev_bssid[] not NULL */
    int reassoc;

        /**
         * wps - WPS mode
         *
         * If the driver needs to do special configuration for WPS association,
         * this variable provides more information on what type of association
         * is being requested. Most drivers should not need ot use this.
         * driver.h enum wps_mode wps;
         */
    int wps;

        /**
         * p2p - Whether this connection is a P2P group
     * driver.hint p2p;
         */
    int p2p;

        /**
         * uapsd - UAPSD parameters for the network
         * -1 = do not change defaults
         * AP mode: 1 = enabled, 0 = disabled
         * STA mode: bits 0..3 UAPSD enabled for VO,VI,BK,BE
         * driver.h int uapsd;
         */
    int uapsd;

        /**
         * fixed_bssid - Whether to force this BSSID in IBSS mode
         * 1 = Fix this BSSID and prevent merges.
         * 0 = Do not fix BSSID.
         * driver.h int fixed_bssid;
         */
    int fixed_bssid;

        /**
         * fixed_freq - Fix control channel in IBSS mode
         * 0 = don't fix control channel (default)
         * 1 = fix control channel; this prevents IBSS merging with another
         *      channel
         * driver.h int fixed_freq;
         */
    int fixed_freq;

        /**
         * disable_ht - Disable HT (IEEE 802.11n) for this connection
         * driver.h int disable_ht;
         */
    int disable_ht;

        /**
         * htcaps - HT Capabilities over-rides
         * htcaps_mask - HT Capabilities over-rides mask
         *
         * Only bits set in the mask will be used, and not all values are used
         * by the kernel anyway. Currently, MCS, MPDU and MSDU fields are used.
         *
         * Pointer to struct ieee80211_ht_capabilities.
         * driver.h const u8 *htcaps;
         *          const u8 *htcaps_mask;
         */
    struct qwdi_ieee80211_ht_cap htcaps;
    struct qwdi_ieee80211_ht_cap htcaps_mask;  /* struct ieee80211_ht_capabilities * */

        /**
         * disable_vht - Disable VHT for this connection
         * driver.h int disable_vht;
         */
    int disable_vht;

        /**
         * VHT capability overrides.
         * driver.h const struct ieee80211_vht_capabilities *vhtcaps;
         *          const struct ieee80211_vht_capabilities *vhtcaps_mask;
         */
    struct qwdi_ieee80211_vht_cap vhtcaps;
    struct qwdi_ieee80211_vht_cap vhtcaps_mask;

        /**
         * req_key_mgmt_offload - Request key management offload for connection
         *
         * Request key management offload for this connection if the device
         * supports it.
         * driver.hint req_key_mgmt_offload;
         */
        int req_key_mgmt_offload;

        /**
         * req_handshake_offload - Request EAPOL handshake offload
         *
         * Request EAPOL handshake offload for this connection if the device
         * supports it.
         */
        int req_handshake_offload;

        /**
         * Flag for indicating whether this association includes support for
         * RRM (Radio Resource Measurements)
         * driver.h int rrm_used;
         */
        int rrm_used;

        /**
         * pbss - If set, connect to a PCP in a PBSS. Otherwise, connect to an
         * AP as usual. Valid for DMG network only.
         * driver.h int pbss;
         */
        int pbss;

        /**
         * fils_kek_len: Length of fils_kek in bytes
         * fils_kek - KEK for FILS association frame protection (AES-SIV)
         * driver.h size_t fils_kek_len;
         * driver.h const u8 *fils_kek;
         * wpa_common.h WPA_KEK_MAX_LEN=64
         */
        struct {
                size_t fils_kek_len;
                char fils_kek_char[64];
        } fils_kek;

        /**
         * fils_nonces_len: Length of fils_nonce in bytes
         * fils_nonces - Nonces for FILS association frame protection
         * (AES-SIV AAD)
         * driver.h const u8 *fils_nonces;
         *          size_t fils_nonces_len;
         * wpa_common.h WPA_NONCE_LEN=32
         */
        struct {
                size_t fils_nonces_len;
                char fils_nonces_char[32];
        } fils_nonces;

        /**
         * Username part of keyName-NAI,username portion takes up 128 octets
         * fils_erp_username_len - Length of fils_erp_username in bytes
         * fils_erp_username - Username part of keyName-NAI
         * driver.h const u8 *fils_erp_username;
         *          size_t fils_erp_username_len;
         */
        struct {
                size_t fils_erp_username_len;
                char fils_erp_username_char[128];
        } fils_erp_username;

        /**
         * http://www.rfc-editor.org/info/rfc7542 max length 253 octets
         * fils_erp_realm_len - Length of fils_erp_realm in bytes
         * fils_erp_realm - Realm/domain name to use in FILS ERP
         * driver.h const u8 *fils_erp_realm;
         *          size_t fils_erp_realm_len;
         */
        struct {
                size_t fils_erp_realm_len;
                char fils_erp_realm_char[253];
        } fils_erp_realm;

        /**
         * fils_erp_next_seq_num - The next sequence number to use in FILS ERP
         * messages
         * driver.h u16 fils_erp_next_seq_num;
         */
        unsigned int fils_erp_next_seq_num;

        /**
         * fils_erp_rrk_len - Length of fils_erp_rrk in bytes
         * fils_erp_rrk - Re-authentication root key (rRK) for the keyName-NAI
         * specified by fils_erp_username@fils_erp_realm.
         * driver.h const u8 *fils_erp_rrk;
         *          size_t fils_erp_rrk_len;
         */
        struct {
                size_t fils_erp_rrk_len;
                char fils_erp_rrk_char[253];
        } fils_erp_rrk;

        /**
         * wpa_ie - WPA information element for (Re)Association Request
         * WPA information element to be included in (Re)Association
         * Request (including information element id and length). Use
         * of this WPA IE is optional. If the driver generates the WPA
         * IE, it can use pairwise_suite, group_suite, group_mgmt_suite, and
         * key_mgmt_suite to select proper algorithms. In this case,
         * the driver has to notify wpa_supplicant about the used WPA
         * IE by generating an event that the interface code will
         * convert into EVENT_ASSOCINFO data (see below).
         *
         * When using WPA2/IEEE 802.11i, wpa_ie is used for RSN IE
         * instead. The driver can determine which version is used by
         * looking at the first byte of the IE (0xdd for WPA, 0x30 for
         * WPA2/RSN).
         * When using WPS, wpa_ie is used for WPS IE instead of WPA/RSN IE.
         * driver.h const u8 *wpa_ie;
         *          size_t wpa_ie_len;
         */
        struct {
                size_t wpa_ie_len;
                unsigned char wpa_ie_char[QWDI_MAX_IE_SIZE];
        } wpa_ie;

} QwdiAssociateReq_t;

typedef struct _QwdiWpaPmkidParams {
        unsigned char bssid[QWDI_ETH_ALEN];
        QwdiSsid_t ssid;        /* size_t ssid_len - included in the QwdiSsid_t struct */
        unsigned char fils_cache_id[QWDI_FILS_CACHE_ID_LEN];
        unsigned char pmkid[QWDI_PMK_MAX_LEN];
        unsigned char pmk[QWDI_PMK_MAX_LEN];
        size_t pmk_len;
} QwdiWpaPmkidParams_t;


typedef struct _QwdiGetBssidRsp {
    unsigned char  bssid[QWDI_ETH_ALEN];
} QwdiGetBssidRsp_t;

typedef struct _QwdiAddPmkidReq {
        QwdiWpaPmkidParams_t pmkidParams;
}QwdiAddPmkidReq_t;

typedef struct _QwdiRemovePmkidReq {
        QwdiWpaPmkidParams_t pmkidParams;
}QwdiRemovePmkidReq_t;

/**
 * typedef struct QwdiExternalAuth - External authentication trigger parameters
 *
 * These are used across the external authentication request and event interfaces.
 *
 * @action: Action type / trigger for external authentication. Only significant
 *  for the event interface.
 * @bssid: BSSID of the peer with which the authentication has to happen. Used
 *  by both the request and event interface.
 * @ssid: SSID of the AP. Used by both the request and event interface.
 * @ssid_len: SSID length in octets.
 * @key_mgmt_suite: AKM suite of the respective authentication. Optional for
 *  the request interface.
 * @status: Status code, %WLAN_STATUS_SUCCESS for successful authentication,
 *  use %WLAN_STATUS_UNSPECIFIED_FAILURE if wpa_supplicant cannot give
 *  the real status code for failures. Used only for the request interface
 *  from user space to the driver.
 * @pmkid: Generated PMKID as part of external auth exchange (e.g., SAE).
 *
 * from driver.h   struct external_auth
 */
typedef struct _QwdiExternalAuth {
    enum {
        QWDI_EXT_AUTH_START,
        QWDI_EXT_AUTH_ABORT,
    } action;
    unsigned char bssid[QWDI_ETH_ALEN];
    unsigned char ssid[QWDI_MAX_SSID_STR_LEN];
    size_t ssid_len;
    unsigned int key_mgmt_suite;
    unsigned short status;
    unsigned char pmkid[QWDI_PMK_MAX_LEN];
} QwdiExternalAuth_t;


typedef struct _QwdiRemainOnChannelReq {
    unsigned int freq;
    unsigned int duration;
}QwdiRemainOnChannelReq_t;

typedef struct _QwdiRemainOnChannelRsp {
    unsigned int roc_id;
}QwdiRemainOnChannelRsp_t;

typedef struct _QwdiCancelRemainOnChannelReq {
    unsigned int roc_id;
}QwdiCancelRemainOnChannelReq_t;


typedef struct _QwdiDeauthenticateReq {
    unsigned char  addr[QWDI_ETH_ALEN];
    unsigned int   reason_code;
}QwdiDeauthenticateReq_t;

typedef struct _QwdiGetSsidRsp {
    QwdiSsid_t ssid;
}QwdiGetSsidRsp_t;

typedef struct _QwdiHapdGetSsidRsp {
    QwdiSsid_t ssid;
}QwdiHapdGetSsidRsp_t;

typedef struct _QwdiHapdSetSsidReq {
    QwdiSsid_t ssid;
}QwdiHapdSetSsidReq_t;

typedef struct _QwdiGetNoaRsp {
    unsigned char buf[QWDI_MAX_NOA_LEN];
    unsigned int  buf_len;
}QwdiGetNoaRsp_t;

typedef struct _QwdiSetNoaReq {
    unsigned int count;
    unsigned int start;
    unsigned int duration;
}QwdiSetNoaReq_t;

typedef struct _QwdiSetCountryReq {
    unsigned char country[QWDI_MAX_COUNTRY_LEN];
}QwdiSetCountryReq_t;

typedef struct _QwdiSetFragReq {
    unsigned int frag_threshold;
}QwdiSetFragReq_t;

typedef struct _QwdiSetRtsReq {
    unsigned int rts_threshold;
}QwdiSetRtsReq_t;

typedef struct _QwdiProbeReqReportReq {
    int report;
}QwdiProbeReqReportReq_t;

typedef struct _QwdiSetP2pPowerSaveReq {
    int legacy_ps;
    int opp_ps;
    int ctwindow;
}QwdiSetP2pPowerSaveReq_t;

typedef struct _QwdiSetApWpsIeReq {
    int beacon_ie_len;
    int proberesp_ie_len;
    int assocresp_ie_len;
    /* ies followed */
}QwdiSetApWpsIeReq_t;


typedef struct _QwdiStaDeauthReq {
    unsigned char own_addr[QWDI_ETH_ALEN];
    unsigned char addr[QWDI_ETH_ALEN];
    int           reason;
}QwdiStaDeauthReq_t;

typedef struct _QwdiStaDisassocReq {
    unsigned char own_addr[QWDI_ETH_ALEN];
    unsigned char addr[QWDI_ETH_ALEN];
    int           reason;
}QwdiStaDisassocReq_t;


typedef struct _QwdiStaRemoveReq {
    unsigned char addr[QWDI_ETH_ALEN];
}QwdiStaRemoveReq_t;

typedef struct _QwdiSetCounterMeasuresReq {
    int           enabled;
}QwdiSetCounterMeasuresReq_t;

typedef struct _QwdiIfAddReq {
    char ifname[QWDI_IFNAMSIZ+1];
    QwdiIfType_e if_type;
    unsigned char  addr[QWDI_ETH_ALEN];
    int          use_existing;
}QwdiIfAddReq_t;

typedef struct _QwdiIfAddRsp {
    unsigned char force_ifname[QWDI_IFNAMSIZ+1];
    unsigned char if_addr[QWDI_ETH_ALEN];
}QwdiIfAddRsp_t;

typedef struct _QwdiIfRemoveReq {
    char ifname[QWDI_IFNAMSIZ+1];
    QwdiIfType_e if_type;
}QwdiIfRemoveReq_t;


typedef struct _QwdiSetApReq {
        /**
         * head - Beacon head from IEEE 802.11 header to IEs before TIM IE
         * driver.h   u8 *head
         */
    unsigned char  beacon_head[QWDI_MAX_HEAD_BUFF_LEN];

        /**
         * head_len - Length of the head buffer in octets
         * driver.h   int head_len
         */
    int head_len;

        /**
         * tail - Beacon tail following TIM IE
         * driver.h   u8 *tail
         */
    unsigned char  beacon_tail[QWDI_MAX_TAIL_BUFF_LEN];

        /**
         * tail_len - Length of the tail buffer in octets
         * driver.h   size_t tail_len
         */
        int tail_len;
        /**
         * dtim_period - DTIM period
         * driver.h  int dtim_period
         */
    int dtim_period;

        /**
         * beacon_int - Beacon interval
         * driver.h   int beacon_int
         */
    int beacon_int;

        /**
         * basic_rates: -1 terminated array of basic rates in 100 kbps
         *
         * This parameter can be used to set a specific basic rate set for the
         * BSS. If %NULL, default basic rate set is used.
         * driver.h   int *basic_rates;
         */
    int basic_rates[QWDI_MAX_SUPPORT_RATES];

        /**
         * basic_rates_len - number of basic rates set in the array
         * QWDI addition
         */
    int basic_rates_len;

        /**
         * beacon_rate: Beacon frame data rate
         *
         * This parameter can be used to set a specific Beacon frame data rate
         * for the BSS. The interpretation of this value depends on the
         * rate_type (legacy: in 100 kbps units, HT: HT-MCS, VHT: VHT-MCS). If
         * beacon_rate == 0 and rate_type == 0 (BEACON_RATE_LEGACY), the default
         * Beacon frame data rate is used.
         * driver.h   unsigned int beacon_rate
         */
        unsigned int beacon_rate;

        /**
         * beacon_rate_type: Beacon data rate type (legacy/HT/VHT)
         * driver.h   enum beacon_rate_type rate_type;
         */
        enum qwdi_beacon_rate_type rate_type;

        /**
         * proberesp - Probe Response template
         *
         * This is used by drivers that reply to Probe Requests internally in
         * AP mode and require the full Probe Response template.
         * driver.h   u8 *proberesp
         */
        unsigned char  proberesp[QWDI_MAX_PROBE_RESP_LEN];

        /**
         * proberesp_len - Length of the proberesp buffer in octets
         * driver.h   size_t proberesp_len
         */
    int proberesp_len;

        /**
         * ssid - The SSID to use in Beacon/Probe Response frames
         * driver.h   const u8 *ssid;
         *            size_t ssid_len;  for qwdi the length is contained within the QwdiSsid struct
         */
    QwdiSsid_t ssid;

        /**
         * hide_ssid - Whether to hide the SSID
         * driver.h   enum hide_ssid hide_ssid
         */
    enum qwdi_hide_ssid hide_ssid;

        /**
         * pairwise_ciphers - WPA_CIPHER_* bitfield
         * driver.h   unsigned int pairwise_ciphers;
         */
    unsigned int pairwise_ciphers;

        /**
         * group_cipher - WPA_CIPHER_*
         * driver.h   unsigned int group_cipher;
         */
    unsigned int group_cipher;

        /**
         * key_mgmt_suites - WPA_KEY_MGMT_* bitfield
         * driver.h   unsigned int key_mgmt_suites
         */
        unsigned int key_mgmt_suites;

        /**
         * auth_algs - WPA_AUTH_ALG_* bitfield
         * driver.h   unsigned int auth_algs
         */
        unsigned int auth_algs;

        /**
         * wpa_version - WPA_PROTO_* bitfield
         * driver.h   unsigned int wpa_version;
         */
        unsigned int wpa_version;

        /**
         * privacy - Whether privacy is used in the BSS
         * driver.h   int privacy
         */
        int privacy;

        /**
         * beacon_ies - WPS/P2P IE(s) for Beacon frames
         *
         * This is used to add IEs like WPS IE and P2P IE by drivers that do
         * not use the full Beacon template.
         * driver.h   const struct wpabuf *beacon_ies
         */
    unsigned char beacon_ies[QWDI_MAX_EXTRA_IES_LEN];

        /**
         * beacon_ies_len - how many ies in the array
         * QWDI add
         */
    int beacon_ies_len;

        /**
         * proberesp_ies - P2P/WPS IE(s) for Probe Response frames
         *
         * This is used to add IEs like WPS IE and P2P IE by drivers that
         * reply to Probe Request frames internally.
         * driver.h   const struct wpabuf *proberesp_ies
         */
    unsigned char proberesp_ies[QWDI_MAX_EXTRA_IES_LEN];

        /**
         * proberesp_ies_len - how many ies in the array
         * QWDI add
         */
    int proberesp_ies_len;

    /**
         * assocresp_ies - WPS IE(s) for (Re)Association Response frames
         *
         * This is used to add IEs like WPS IE by drivers that reply to
         * (Re)Association Request frames internally.
         * driver.h   const struct wpabuf *assocresp_ies;
         */
    unsigned char assocresp_ies[QWDI_MAX_EXTRA_IES_LEN];

        /**
         * assocresp_ies_len - how many ies in the array
         * QWDI add
         */
    int assocresp_ies_len;

        /**
         * isolate - Whether to isolate frames between associated stations
         *
         * If this is non-zero, the AP is requested to disable forwarding of
         * frames between associated stations.
         * driver.h   int isolate
         */
        int isolate;

        /**
         * cts_protect - Whether CTS protection is enabled
         * driver.h   int cts_protect
         */
        int cts_protect;

        /**
         * preamble - Whether short preamble is enabled
         * driver.h  int preamble
         */
        int preamble;

        /**
         * short_slot_time - Whether short slot time is enabled
         *
         * 0 = short slot time disable, 1 = short slot time enabled, -1 = do
         * not set (e.g., when 802.11g mode is not in use)
         * driver.h   int short_slot_time
         */
        int short_slot_time;

        /**
         * ht_opmode - HT operation mode or -1 if HT not in use
         * driver.h  int ht_opmode
         */
        int ht_opmode;

        /**
         * interworking - Whether Interworking is enabled
         * driver.h   int interworking
         */
        int interworking;

        /**
         * hessid - Homogeneous ESS identifier or %NULL if not set
         * driver.h   const u8 *hessid;
         */
    unsigned char hessid[QWDI_ETH_ALEN];

        /**
         * access_network_type - Access Network Type (0..15)
         *
         * This is used for filtering Probe Request frames when Interworking is
         * enabled.
         * driver.h     u8 access_network_type
         */
    unsigned char access_network_type;

        /**
         * ap_max_inactivity - Timeout in seconds to detect STA's inactivity
         *
         * This is used by driver which advertises this capability.
         * driver.h     int ap_max_inactivity
         */
    int ap_max_inactivity;

        /**
         * ctwindow - Client Traffic Window (in TUs)
         * driver.h     u8 p2p_go_ctwindow
         */
    unsigned char p2p_go_ctwindow;

        /**
         * smps_mode - SMPS mode
         *
         * SMPS mode to be used by the AP, specified as the relevant bits of
         * ht_capab (i.e. HT_CAP_INFO_SMPS_*).
         * driver.h     unsigned int smps_mode;
         */
    unsigned int smps_mode;

        /**
         * disable_dgaf - Whether group-addressed frames are disabled
         * driver.h     int disable_dgaf
         */
    int disable_dgaf;

        /**
         * osen - Whether OSEN security is enabled
         * driver.h     int osen
         */
    int osen;

    /**
     * freq - Channel parameters for dynamic bandwidth changes
     * driver.h     struct hostapd_freq_params *freq
     */
    QwdiHapdFreqParams_t freq;

        /**
         * reenable - Whether this is to re-enable beaconing
         * driver.h     int reenable
         */
    int reenable;

        /**
         * pbss - Whether to start a PCP (in PBSS) instead of an AP in
         * infrastructure BSS. Valid only for DMG network.
         * driver.h   int pbss
         */
        int pbss;

        /**
         * multicast_to_unicast - Whether to use multicast_to_unicast
         *
         * If this is non-zero, the AP is requested to perform multicast to
         * unicast conversion for ARP, IPv4, and IPv6 frames (possibly within
         * 802.1Q). If enabled, such frames are to be sent to each station
         * separately, with the DA replaced by their own MAC address rather
         * than the group address.
         *
         * Note that this may break certain expectations of the receiver, such
         * as the ability to drop unicast IP packets received within multicast
         * L2 frames, or the ability to not send ICMP destination unreachable
         * messages for packets received in L2 multicast (which is required,
         * but the receiver can't tell the difference if this new option is
         * enabled.)
         *
         * This also doesn't implement the 802.11 DMS (directed multicast
         * service).
         * driver.h   int multicast_to_unicast
         */
        int multicast_to_unicast;

    /**
     * ftm_responder - Whether FTM responder is enabled
     * driver.h   int ftm_responder
     */
        int ftm_responder;

    /**
     * QNX addition. Not in driver.h wpa_driver_ap_params structure.
     * QwdiWepKeys_t structure
     */
        QwdiWepKeys_t wep;

    /**
     * QNX addition. Not in driver.h wpa_driver_ap_params structure.
     * int max_num_sta
     */
        int max_num_sta;

    /**
     * lci - Binary data, the content of an LCI report IE with type 8 as
     * defined in IEEE Std 802.11-2016, 9.4.2.22.10
     * driver.h   const struct wpabuf *lci;
     */
        unsigned char lci[QWDI_MAX_IE_SIZE];

    /**
     * QWDI addition - not in driver.h
     * lci length - number of chars used in the array
     */
    int lci_len;

    /**
     * civic - Binary data, the content of a measurement report IE with
     * type 11 as defined in IEEE Std 802.11-2016, 9.4.2.22.13
     * driver.h  const struct wpabuf *civic
     */
    unsigned char civic[QWDI_MAX_IE_SIZE];

    /**
     * QWDI addition - not in driver.h
     * lci length - number of chars used in the array
     */
    int civic_len;

} QwdiSetApReq_t;


/* new structs for mesh support from driver.h */
#define QWDI_WPA_DRIVER_MESH_CONF_FLAG_AUTO_PLINKS              0x00000001
#define QWDI_WPA_DRIVER_MESH_CONF_FLAG_PEER_LINK_TIMEOUT        0x00000002
#define QWDI_WPA_DRIVER_MESH_CONF_FLAG_MAX_PEER_LINKS   0x00000004
#define QWDI_WPA_DRIVER_MESH_CONF_FLAG_HT_OP_MODE               0x00000008
#define QWDI_WPA_DRIVER_MESH_CONF_FLAG_RSSI_THRESHOLD   0x00000010

/* struct wpa_driver_mesh_bss_params */
typedef struct _QwdiWpaDriverMeshBssParams {
        /*
         * TODO: Other mesh configuration parameters would go here.
         * See NL80211_MESHCONF_* for all the mesh config parameters.
         */
        unsigned int flags;
        int auto_plinks;
        int peer_link_timeout;
        int max_peer_links;
        int rssi_threshold;
        short int ht_opmode;
} QwdiWpaDriverMeshBssParams_t;

#define QWDI_WPA_DRIVER_MESH_FLAG_USER_MPM      0x00000001
#define QWDI_WPA_DRIVER_MESH_FLAG_DRIVER_MPM    0x00000002
#define QWDI_WPA_DRIVER_MESH_FLAG_SAE_AUTH      0x00000004
#define QWDI_WPA_DRIVER_MESH_FLAG_AMPE  0x00000008

/* The Mesh ID element (7.3.2.99 and 11C.2.2). The Mesh ID is a 0 to 32 byte
 * field, close to the SSID in concept. It can be an ASCII string, and uniquely
 * identifies the MBSS
 * QWDI - addition
 */
#define QWDI_MESH_ID_MAX_LEN 32

/* struct wpa_driver_mesh_join_params  */
typedef struct _QwdiWpaDriverMeshJoinParams {
        /* driver.h const u8 *meshid */
        unsigned char meshid[QWDI_MESH_ID_MAX_LEN];
        /* driver.h length of mesh ID */
        int meshid_len;
        /* driver.h const int *basic_rates; */
        int basic_rates[QWDI_MAX_SUPPORT_RATES];
        /* driver.h  const u8 *ies */
        unsigned char ies[QWDI_MAX_EXTRA_IES_LEN];
        /* driver.h length of ies array used */
        int ie_len;
        //struct hostapd_freq_params freq;
        QwdiHapdFreqParams_t freq;
        int beacon_int;
        int dtim_period;
        // struct wpa_driver_mesh_bss_params conf;
        QwdiWpaDriverMeshBssParams_t conf;
        unsigned int flags;
} QwdiWpaDriverMeshJoinParams_t;
/* end of new mesh structs  */

/*  from driver.h */
#define QWDI_STA_DRV_DATA_TX_MCS BIT(0)
#define QWDI_STA_DRV_DATA_RX_MCS BIT(1)
#define QWDI_STA_DRV_DATA_TX_VHT_MCS BIT(2)
#define QWDI_STA_DRV_DATA_RX_VHT_MCS BIT(3)
#define QWDI_STA_DRV_DATA_TX_VHT_NSS BIT(4)
#define QWDI_STA_DRV_DATA_RX_VHT_NSS BIT(5)
#define QWDI_STA_DRV_DATA_TX_SHORT_GI BIT(6)
#define QWDI_STA_DRV_DATA_RX_SHORT_GI BIT(7)
#define QWDI_STA_DRV_DATA_LAST_ACK_RSSI BIT(8)

/**
 * typedef struct QwdiHostapStaDriverData_t
 * from driver.h   struct hostap_sta_driver_data
 */
typedef struct _QwdiHostapStaDriverData {
        unsigned long rx_packets;
        unsigned long tx_packets;
        unsigned long long rx_bytes;
        unsigned long long tx_bytes;
        unsigned long long rx_airtime;
        unsigned long long tx_airtime;
        /* whether 64-bit byte counters are supported */
        int bytes_64bit;
        unsigned long current_tx_rate;
        unsigned long current_rx_rate;
        unsigned long inactive_msec;
        /* bit field of QWDI_STA_DRV_DATA_? see above */
        unsigned long flags;
        unsigned long num_ps_buf_frames;
        unsigned long tx_retry_failed;
        unsigned long tx_retry_count;
        char last_ack_rssi;
        unsigned long backlog_packets;
        unsigned long backlog_bytes;
        char signal;
        unsigned char rx_vhtmcs;
        unsigned char tx_vhtmcs;
        unsigned char rx_mcs;
        unsigned char tx_mcs;
        unsigned char rx_vht_nss;
        unsigned char tx_vht_nss;
} QwdiHostapStaDriverData_t;



/**
 * struct QwdiIeee80211HeCapa
 * from ieee802_11_defs.h   struct ieee80211_he_capabilities
 */
struct QwdiIeee80211HeCapa {
    unsigned char he_mac_capab_info[6];
    unsigned char  he_phy_capab_info[11];
    /** Followed by 4, 8, or 12 octets of Supported HE-MCS And NSS Set field
     * and optional variable length PPE Thresholds field.
     * NOTE: QNX - will set to fixed length of 12 for the time being
     */
    unsigned char optional[12];
}  __attribute__((packed));


/* driver.h  struct hostapd_sta_add_params */
struct _QwdiHostapdStaAddParams {
        /* const u8 *addr  assume this is a MAC addr */
        unsigned char addr[QWDI_ETH_ALEN];

        unsigned short aid;
        unsigned short capability;

        /* const u8 *supp_rates */
        unsigned char supp_rates[QWDI_MAX_SUPPORT_RATES];
        size_t supp_rates_len;

        unsigned short listen_interval;

        struct qwdi_ieee80211_ht_cap ht_capabilities;
        struct qwdi_ieee80211_vht_cap vht_capabilities;

        int vht_opmode_enabled;
        unsigned char vht_opmode;

        //const struct ieee80211_he_capabilities *he_capab;
        const struct QwdiIeee80211HeCapa he_capab;
        size_t he_capab_len;

        unsigned long flags; /* bitmask of WPA_STA_* flags */
        unsigned long flags_mask; /* unset bits in flags */
#ifdef CONFIG_MESH
        enum mesh_plink_state plink_state;
        unsigned short peer_aid;
#endif /* CONFIG_MESH */
        int set; /* Set STA parameters instead of add */
        unsigned char qosinfo;

        /* driver.h const u8 *ext_capab */
        unsigned char ext_capab[QWDI_EXTENDED_CAPA_MAX_LEN];
        size_t ext_capab_len;

        /* driver.h const u8 *supp_channels */
        unsigned char supp_channels[128];
        size_t supp_channels_len;

        /* driver.h const u8 *supp_oper_classes; */
        unsigned char supp_oper_classes[128];
        size_t supp_oper_classes_len;

        int support_p2p_ps;
} QwdiHostapdStaAddParams_t;


typedef struct _QwdiSendActionReq {
    unsigned int  freq;
    unsigned int  wait_time;
    int           no_cck;
    unsigned char dst[QWDI_ETH_ALEN];
    unsigned char src[QWDI_ETH_ALEN];
    unsigned char bssid[QWDI_ETH_ALEN];
    int data_len;
    unsigned char data[QWDI_MAX_MGMT_FRAME_LEN];
}QwdiSendActionReq_t;

typedef struct _QwdiSendMlmeReq {
    int noack;
    int data_len;
    unsigned char data[QWDI_MAX_MGMT_FRAME_LEN];
    unsigned int freq;
    unsigned short csa_offs[2];
    size_t csa_offs_len;
}QwdiSendMlmeReq_t;

/**
 * from driver.h
 * struct freq_survey - Channel survey info
 *
 * @ifidx: Interface index in which this survey was observed
 * @freq: Center of frequency of the surveyed channel
 * @nf: Channel noise floor in dBm
 * @channel_time: Amount of time in ms the radio spent on the channel
 * @channel_time_busy: Amount of time in ms the radio detected some signal
 *     that indicated to the radio the channel was not clear
 * @channel_time_rx: Amount of time the radio spent receiving data
 * @channel_time_tx: Amount of time the radio spent transmitting data
 * @filled: bitmask indicating which fields have been reported, see
 *     SURVEY_HAS_* defines.
 */
typedef struct _QwdiFreqSurvey {
        unsigned long ifidx;
        unsigned int freq;
        char nf;
        unsigned long long channel_time;
        unsigned long long channel_time_busy;
        unsigned long long channel_time_rx;
        unsigned long long channel_time_tx;
        unsigned int filled;
} QwdiQwdiFreqSurvey_t;
/** TODO: rename this to get rid of the extra Qwdi in its name
 *  later */

/**
 * struct qwdi_hostapd_wmm_rule - WMM regulatory rule
 * @min_cwmin: Lower bound of CW_min value
 * @min_cwmax: Lower bound of CW_max value
 * @min_aifs: Lower bound of AIFS value
 * @max_txop: Upper bound of TXOP, value in units of 32 usec
 *
 * fom driver.h  hostapd_wmm_rule
 */
struct qwdi_hostapd_wmm_rule {
    int min_cwmin;
    int min_cwmax;
    int min_aifs;
    int max_txop;
};

/**
 * Access Categories / ACI to AC coding
 * defined in ieee802_11_defs.h
 */
enum qwdi_wmm_ac {
    QWDI_WMM_AC_BE = 0 /* Best Effort */,
    QWDI_WMM_AC_BK = 1 /* Background */,
    QWDI_WMM_AC_VI = 2 /* Video */,
    QWDI_WMM_AC_VO = 3 /* Voice */,
    QWDI_WMM_AC_NUM = 4
};

/**
 * struct _QwdiChannelInfo_channel_data - Channel information
 * driver.h  struct hostapd_channel_data
 */
typedef struct _QwdiChannelInfo {
        /**
         * chan - Channel number (IEEE 802.11)
         * driver.h     short chan;
         */
    unsigned int chan;
        /**
         * freq - Frequency in MHz
         * driver.h int freq;
         */
    unsigned int freq;
        /**
         * flag - Channel flags (HOSTAPD_CHAN_*)
         * driver.h   int flag;
         */

    unsigned int flag;
    /**
     * allowed_bw - Allowed channel width bitmask
     *
     * See enum hostapd_chan_width_attr.
     */
    unsigned long allowed_bw;

        /**
         * max_tx_power - Regulatory transmit power limit in dBm
         * driver.h  u8 max_tx_power;
         */
    unsigned char max_tx_power;

        /**
         * min_nf - Minimum observed noise floor, in dBm, based on all
         * surveyed channel data
         * driver.h       s8 min_nf;
         */
    char min_nf;

        /**
         * dfs_cac_ms - DFS CAC time in milliseconds
         * driver.h   unsigned int dfs_cac_ms;
         */
    unsigned int dfs_cac_ms;

#ifdef CONFIG_ACS
        /**
         * interference_factor - Computed interference factor on this
         * channel (used internally in src/ap/acs.c; driver wrappers do not
         * need to set this)
         * driver.h  long double interference_factor;   10 bytes
         */
        long double interference_factor;
#endif /* CONFIG_ACS */

        /**
        * wmm_rules_valid - Indicates wmm_rules state
        */
        int wmm_rules_valid;

        /**
        * wmm_rules - WMM regulatory rules hostapd_wmm_rule
        */
        struct qwdi_hostapd_wmm_rule wmm_rules[QWDI_WMM_AC_NUM];

        /**
         * number of freq_survey_structs in array. supplicant sends this as a linked list of
         * freq_survey_structs. will need to run the list to determine how many array elements
         * are present.
         */
        int num_freq_survey_structs;

        /**
         * array of freq_survey_structs. supplicant sends this as a linked list of
         * freq_survey_structs. will need to run the list to determine how many array elements
         * are needed then copy them in. allocate 1 element as a place holder
         */
/** TODO:  get rid of the extra Qwdi in its name */
        QwdiQwdiFreqSurvey_t freq_survey_list[1];

} QwdiChannelInfo_t;

/**
 * from driver.h
 * enum hostapd_hw_mode - Hardware mode
 */
typedef enum _QwdiHwMode {
    QWDI_HW_MODE_IEEE80211B,
    QWDI_HW_MODE_IEEE80211G,
    QWDI_HW_MODE_IEEE80211A,
    QWDI_HW_MODE_IEEE80211AD,
        QWDI_HW_MODE_IEEE80211ANY,
    QWDI_MAX_HW_MODES_NUM
} QwdiHwMode_e;


/*From ieee802_11_defs.h */
#define QWDI_WLAN_EID_RSN 48

/**
  * from driver.h
  */
#define QWDI_HE_MAX_MAC_CAPAB_SIZE  6
#define QWDI_HE_MAX_PHY_CAPAB_SIZE  11
#define QWDI_HE_MAX_MCS_CAPAB_SIZE  12
#define QWDI_HE_MAX_PPET_CAPAB_SIZE 25

/** TODO:  get rid of this */
/* old ones for 2.8 build */
#define QWDI_HE_MAX_NUM_SS      8


/**
  * struct QwdiHeCapabilities - IEEE 802.11ax HE capabilities
  * from driver.h  struct he_capabilities
  */
 typedef struct _QwdiHeCapabilities {
        unsigned char he_supported;
        unsigned char phy_cap[QWDI_HE_MAX_PHY_CAPAB_SIZE];
        unsigned char mac_cap[QWDI_HE_MAX_MAC_CAPAB_SIZE];
        unsigned char mcs[QWDI_HE_MAX_MCS_CAPAB_SIZE];
        unsigned char ppet[QWDI_HE_MAX_PPET_CAPAB_SIZE];
 } QwdiHeCapabilities_t;


#define HOSTAPD_MODE_FLAG_HT_INFO_KNOWN BIT(0)
#define HOSTAPD_MODE_FLAG_VHT_INFO_KNOWN BIT(1)

/**
  * enum qwdi_ieee80211_op_mode - IEEE 802.11ax HE capabilitiesfrom
  * from driver.h  enum ieee80211_op_mode
  */
enum qwdi_ieee80211_op_mode {
        QWDI_IEEE80211_MODE_INFRA = 0,
        QWDI_IEEE80211_MODE_IBSS = 1,
        QWDI_IEEE80211_MODE_AP = 2,
        QWDI_IEEE80211_MODE_MESH = 5,

        /* only add new entries before QWDI_IEEE80211_MODE_NUM */
        QWDI_IEEE80211_MODE_NUM
};


/* might want to make an enum */
#define QWDI_HOSTAPD_DFS_REGION_FCC     1
#define QWDI_HOSTAPD_DFS_REGION_ETSI 2
#define QWDI_HOSTAPD_DFS_REGION_JP      3

/**
 * struct QwdiHwFeature - Supported hardware mode information
 * from driver.h  hostapd_hw_modes
 */
typedef struct _QwdiHwFeature {
        /**
         * mode - Hardware mode
         * driver.h enum hostapd_hw_mode mode;
         */
    QwdiHwMode_e mode;
        /**
         * num_channels - Number of entries in the channels array
         * driver.h     int num_channels;
         */
    int num_channels;
        /**
         * channels - Array of supported channels
         * driver.h     struct hostapd_channel_data *channels;
         */
    QwdiChannelInfo_t channels[QWDI_MAX_CHANNEL_NUM];

        /**
         * num_rates - Number of entries in the rates array
         * driver.h   int num_rates
         */
    int num_rates;

        /**
         * rates - Array of supported rates in 100 kbps units
         * driver.h  int *rates;
         *
         */
    int rates[QWDI_MAX_SUPPORT_RATES];

        /**
         * ht_capab - HT (IEEE 802.11n) capabilities
         * driver.hu16 ht_capab;
         */
    unsigned short ht_capab;

        /**
         * mcs_set - MCS (IEEE 802.11n) rate parameters
         * driver.h   u8 mcs_set[16];
         */
    unsigned char mcs_set[QWDI_MAX_SUPPORT_MCS];

        /**
         * mcs_set_num  qwdi added. number of mcs_set[] entries used in array, maybe less than array size
         * driver.h   u8 mcs_set[16];
         */
    unsigned char mcs_set_num;

        /**
         * a_mpdu_params - A-MPDU (IEEE 802.11n) parameters
         * driver.h   u8 a_mpdu_params;
         */
    unsigned char a_mpdu_params;

        /**
         * vht_capab - VHT (IEEE 802.11ac) capabilities
         * driver.h   u32 vht_capab;
         */
    unsigned long vht_capab;

        /**
         * vht_mcs_set - VHT MCS (IEEE 802.11ac) rate parameters
         * driver.h   u8 vht_mcs_set[8];
         */
    unsigned char vht_mcs_set[QWDI_MAX_SUPPORT_MCS];

        /**
         * vht_mcs_set_num  qwdi added. number of vht_mcs_set[] entries used in array, maybe less than array size
         * driver.h   u8 mcs_set[16];
         */
    unsigned char vht_mcs_set_num;

        /**
         * flags - HOSTAPD_MODE_FLAG
         * driver.h   unsigned int flags;
         */
    unsigned int flags;
        /**
         * he_capab - HE (IEEE 802.11ax) capabilities
         * driver.h  struct he_capabilities he_capab
         * QWDI - create a fixed size block for this
         */
    QwdiHeCapabilities_t qwdi_he_capabilities[QWDI_IEEE80211_MODE_NUM];

} QwdiHwFeature_t;


typedef struct _QwdiGetHwFeatureDataRsp {
    int num_modes;
        /**
         * dfs - Variable for returning DFS region
         * driver.h  it's not in the structure but as a ptr in the function call
         * QWDI - provide a spot to put DFS Region
         */
    unsigned char dfs;

    QwdiHwFeature_t hw_modes[QWDI_MAX_HW_MODES_NUM];
}QwdiGetHwFeatureDataRsp_t;

typedef struct _QwdiSetOperStateReq {
    int operstate;
}QwdiSetOperStateReq_t;

typedef struct _QwdiGetCapaRsp{
    int has_capability;
    QwdiDriverCapa_t capa;
} QwdiGetCapaRsp_t;

typedef struct _QwdiGetAssocInfoRsp {
    int reassoc;
    unsigned int   freq;
    unsigned char  addr[QWDI_ETH_ALEN];
    unsigned int   req_ies_len;
    unsigned int   resp_ies_len;
    unsigned int   beacon_ies_len;
    unsigned char  ie[1]; /*req_ies + resp_ies + beacon_ies */
}QwdiGetAssocInfoRsp_t;

typedef struct _QwdiSharedFreqRsp {
    int freq;
}QwdiSharedFreqRsp_t;

typedef struct _QwdiSetP2pModeReq {
    unsigned char  mode;
    unsigned int   channel;
    unsigned short listen_ms;
}QwdiSetP2pModeReq_t;

/********************/
/* event structures */
/********************/

/* signal poll response information struct */
typedef struct _QwdiSignalPollRsp {
        unsigned long frequency;
        int above_threshold;
        int current_signal;
        int avg_signal;
        int avg_beacon_signal;
        int current_noise;
        int current_txrate;
        QWDIChanWidth_e chanwidth;
        int center_frq1;
        int center_frq2;

} QwdiSignalPollRsp_t;

typedef struct _QwdiScanInfo {
    int aborted;
    int scan_type;
    int sched_scan;
    int set_timeout;
    unsigned int freqs[QWDI_MAX_SCAN_FREQ];
    unsigned int num_freqs;
    QwdiSsid_t ssids[QWDI_MAX_SCAN_SSID];
    unsigned int num_ssids;
}QwdiScanInfo_t;

typedef struct _QwdiAssocInfo {
    int reassoc;
    unsigned int   freq;
    unsigned char  addr[QWDI_ETH_ALEN];
    unsigned int   req_ies_len;
    unsigned int   resp_ies_len;
    unsigned int   beacon_ies_len;
    unsigned char  ie[1]; /*req_ies + resp_ies + beacon_ies */
}QwdiAssocInfo_t;

typedef struct _QwdiAssocInd {
    int reassoc;
    unsigned int   freq;
    unsigned char  addr[QWDI_ETH_ALEN];
    unsigned int   req_ies_len;
    unsigned char  ie[1]; /*req_ies */
}QwdiAssocInd_t;

typedef struct _QwdiDisassocInfo {
    int            locally_generated;
    unsigned short reason_code;
    unsigned char  addr[QWDI_ETH_ALEN];
    unsigned int   ies_len;
    unsigned char  ie[1]; /*ies */
}QwdiDisassocInfo_t;

typedef struct _QwdiRemainOnChannelInfo {
    unsigned int freq;
    unsigned int duration;
    unsigned int roc_id;
}QwdiRemainOnChannelInfo_t;

typedef struct _QwdiTxStatus {
    unsigned short type;
    unsigned short stype;
    int            ack;
    unsigned char  dst[QWDI_ETH_ALEN];
    int            data_len;
    unsigned char  data[QWDI_MAX_MGMT_FRAME_LEN];
}QwdiTxStatus_t;

typedef struct _QwdiRxMgmt {
    int frame_len;
    unsigned int datarate;
    int freq; /*Frequency (in MHz) on which the frame was received */
    int ssi_signal; /*Signal strength in dBm (or 0 if not available) */
    unsigned char frame[QWDI_MAX_MGMT_FRAME_LEN];
}QwdiRxMgmt_t;

typedef struct _QwdiMichaelMicFailure {
    int unicast;
    unsigned char  src[QWDI_ETH_ALEN];
}QwdiMichaelMicFailure_t;

typedef struct _QwdiAvoidFreq {
    unsigned int num;
    struct avoid_freq_range {
        unsigned int min;
        unsigned int max;
    } range[0];
} QwdiAvoidFreq_t;

typedef struct _QwdiUnprotDisassoc {
    unsigned char sa[QWDI_ETH_ALEN];
    unsigned char da[QWDI_ETH_ALEN];
    unsigned short reason_code;
} QwdiUnprotDisassoc_t;

typedef struct _QwdiChannelListChanged {
    QwdiRegInitiator_e initiator;
    QwdiRegType_e type;
    char alpha2[3];
} QwdiChannelListChanged_t;


typedef struct _QwdiChSwitch {
    int freq;
    int ht_enabled; /* ht_enabled: Whether this is an HT channel */
    int ch_offset;  /* ch_offset: Secondary channel offset */
    QWDIChanWidth_e ch_width;
    int cf1;
    int cf2;
} QwdiChSwitch_t;

#endif /*_QWDI_MSG_H*/

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/lib/io-pkt/services/wpa-2.9/wpa_supplicant/public/qwdi_msg_2_9.h $ $Rev: 909885 $")
#endif
