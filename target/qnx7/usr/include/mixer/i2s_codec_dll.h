/*
 * $QNXLicenseC:
 * Copyright 2017, QNX Software Systems.
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

#ifndef _I2S_CODEC_DLL_H_INCLUDED_
#define _I2S_CODEC_DLL_H_INCLUDED_

#define I2S_CODEC_MINOR_VERSION 2

typedef
struct ado_mixer_dll_codec_params
{
    HW_CONTEXT_T  *hw_context;
    int32_t       i2c_addr;       /* Codec I2C slave address */
    int32_t       i2c_dev;        /* I2C device number */
    int32_t       mclk;           /* MCLK Frequency */
    bool          clk_master;     /* Codec is clock master */
    int32_t       tx_sample_rate; /* Playback sample rate */
    int32_t       rx_sample_rate; /* Capture sample rate */
    int8_t        tx_voices;      /* Playback voices */
    int8_t        rx_voices;      /* Capture voices */
    int8_t        tx_sample_size; /* Playback sample size in bits */
    int8_t        rx_sample_size; /* Capture sample size in bits */
/* Callbacks for the Mixer DLL to call into the Ctrl DLL */
    bool          (*is_active)   (HW_CONTEXT_T *hw_context, int channel);
    /* Optional codec access routines used to override the routines built into the Mixer DLL */
    int32_t       (*codec_open)  (HW_CONTEXT_T *hw_context);
    int32_t       (*codec_read)  (HW_CONTEXT_T *hw_context, uint32_t regaddr, uint32_t* data, int index);
    int32_t       (*codec_write) (HW_CONTEXT_T *hw_context, uint32_t regaddr, uint32_t data, int index);
    void          (*codec_close) (HW_CONTEXT_T *hw_context);
    const snd_pcm_chmap_t* (*get_chmap) (HW_CONTEXT_T *hw_context, int channel, int index);
    void          (*reserved_9)(); /* reserved for future use */
    void          (*reserved_8)(); /* reserved for future use */
    void          (*reserved_7)(); /* reserved for future use */
    void          (*reserved_6)(); /* reserved for future use */
    void          (*reserved_5)(); /* reserved for future use */
    void          (*reserved_4)(); /* reserved for future use */
    void          (*reserved_3)(); /* reserved for future use */
    void          (*reserved_2)(); /* reserved for future use */
    void          (*reserved_1)(); /* reserved for future use */
    void          (*reserved_0)(); /* reserved for future use */
}
ado_mixer_dll_codec_params_t;

typedef
struct ado_mixer_dll_codec_capabilities
{
    uint32_t        rates;
    struct ratelist ratelist; /* used when SND_PCM_RATE_KNOT specified in rates;
                               * ratelist contains the non standard rates */
    uint32_t        formats;
    uint32_t        min_voices;
    uint32_t        max_voices;
    uint32_t        reserved[10];
} ado_mixer_dll_codec_capabilities_t;

/* Callbacks for the Ctrl DLL to call into the Mixer DLL */
typedef
struct  ado_mixer_dll_codec_callbacks
{
    /* Required routine to set the default mixer group(s) for the PCM device */
    void          (*codec_set_default_group) (ado_pcm_t *pcm, ado_mixer_t *mixer, int channel, int index);
    /* Optional codec routines to allow codec level configuration changes around start/stop of the audio stream */
    void          (*codec_on) (ado_mixer_t *mixer, int channel);
    void          (*codec_off) (ado_mixer_t *mixer, int channel);
    /* Optional codec routine to allow codec sample rate configuration changes from acquire callback */
    void          (*codec_set_rate) (ado_mixer_t *mixer, uint32_t rate, int channel);
    /* Optional codec routine to allow codec sample size configuration changes from acquire callback */
    void          (*codec_set_sample_size) (ado_mixer_t *mixer, uint32_t sample_size, int channel);
    /* Optional codec routine to allow retrieving codec capabilities */
    void          (*codec_capabilities) (ado_mixer_t *mixer, ado_mixer_dll_codec_capabilities_t *cap, int channel);
    void          (*reserved_7)();  /* reserved for future use */
    void          (*reserved_6)();  /* reserved for future use */
    void          (*reserved_5)();  /* reserved for future use */
    void          (*reserved_4)();  /* reserved for future use */
    void          (*reserved_3)();  /* reserved for future use */
    void          (*reserved_2)();  /* reserved for future use */
    void          (*reserved_1)();  /* reserved for future use */
    void          (*reserved_0)();  /* reserved for future use */
}
ado_mixer_dll_codec_callbacks_t;

#endif

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/services/audio/public/include/mixer/i2s_codec_dll.h $ $Rev: 883503 $")
#endif
