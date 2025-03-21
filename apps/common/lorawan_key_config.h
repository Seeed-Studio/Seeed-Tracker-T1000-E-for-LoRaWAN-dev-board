/*!
 * @file      lorawan_key_config.h
 *
 * @brief     End device lorawan key configuration
 *
 * The Clear BSD License
 * Copyright Semtech Corporation 2021. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted (subject to the limitations in the disclaimer
 * below) provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Semtech corporation nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY
 * THIS LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
 * CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT
 * NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL SEMTECH CORPORATION BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef LORAWAN_KEY_CONFIG_H
#define LORAWAN_KEY_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * -----------------------------------------------------------------------------
 * --- DEPENDENCIES ------------------------------------------------------------
 */

#include <stdint.h>

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC MACROS -----------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC CONSTANTS --------------------------------------------------------
 */

/*!
 ******************************************************************************
 ********************************** WARNING ***********************************
 ******************************************************************************
  The crypto-element implementation supports both 1.0.x and 1.1.x LoRaWAN
  versions of the specification.
  Thus it has been decided to use the 1.1.x keys and EUI name definitions.
  The below table shows the names equivalence between versions:
               +---------------------+-------------------------+
               |       1.0.x         |          1.1.x          |
               +=====================+=========================+
               | LORAWAN_DEVICE_EUI  | LORAWAN_DEVICE_EUI      |
               +---------------------+-------------------------+
               | LORAWAN_APP_EUI     | LORAWAN_JOIN_EUI        |
               +---------------------+-------------------------+
               | LORAWAN_APP_KEY     | LORAWAN_NWK_KEY         |
               +---------------------+-------------------------+

 ******************************************************************************
 ******************************************************************************
 ******************************************************************************
 */

/*!
 * @brief LoRaWAN class - see @ref smtc_modem_class_t
 */
// #define LORAWAN_REGION      SMTC_MODEM_REGION_EU_868
// #define LORAWAN_CLASS       SMTC_MODEM_CLASS_A
// #define LORAWAN_DEVICE_EUI  "70B3D57ED0050417"
// #define LORAWAN_JOIN_EUI    "0000000000000000"
// #define LORAWAN_APP_KEY     "EC6B1047B76B2C3C92CF9A128A5CCFEC"

#define LORAWAN_REGION      SMTC_MODEM_REGION_US_915
#define LORAWAN_CLASS       SMTC_MODEM_CLASS_A
#define LORAWAN_DEVICE_EUI  "70B3D57ED004D794"
#define LORAWAN_JOIN_EUI    "0000000000000000"
#define LORAWAN_APP_KEY     "5C2BD02C317E34A72354D98733238C78"

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC TYPES ------------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTIONS PROTOTYPES ---------------------------------------------
 */

#ifdef __cplusplus
}
#endif

#endif  // LORAWAN_KEY_CONFIG_H

/* --- EOF ------------------------------------------------------------------ */
