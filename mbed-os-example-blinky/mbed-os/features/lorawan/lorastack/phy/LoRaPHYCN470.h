/**
 *  @file LoRaPHYCN470.h
 *
 *  @brief Implements LoRaPHY for Chinese 470 MHz band
 *
 *  \code
 *   ______                              _
 *  / _____)             _              | |
 * ( (____  _____ ____ _| |_ _____  ____| |__
 *  \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 *  _____) ) ____| | | || |_| ____( (___| | | |
 * (______/|_____)_|_|_| \__)_____)\____)_| |_|
 *   (C)2013 Semtech
 *  ___ _____ _   ___ _  _____ ___  ___  ___ ___
 * / __|_   _/_\ / __| |/ / __/ _ \| _ \/ __| __|
 * \__ \ | |/ _ \ (__| ' <| _| (_) |   / (__| _|
 * |___/ |_/_/ \_\___|_|\_\_| \___/|_|_\\___|___|
 * embedded.connectivity.solutions===============
 *
 * \endcode
 *
 *
 * License: Revised BSD License, see LICENSE.TXT file include in the project
 *
 * Maintainer: Miguel Luis ( Semtech ), Gregory Cristian ( Semtech ) and Daniel Jaeckle ( STACKFORCE )
 *
 * Copyright (c) 2017, Arm Limited and affiliates.
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef MBED_OS_LORAPHY_CN470_H_
#define MBED_OS_LORAPHY_CN470_H_

#include "LoRaPHY.h"

// Definitions
/*!
 * LoRaMac maximum number of channels
 */
#define CN470_MAX_NB_CHANNELS                        96

/*!
 * LoRaMac maximum number of bands
 */
#define CN470_MAX_NB_BANDS                           1


#define CN470_CHANNEL_MASK_SIZE                     6


class LoRaPHYCN470 : public LoRaPHY {

public:

    LoRaPHYCN470(LoRaWANTimeHandler &lora_time);
    virtual ~LoRaPHYCN470();

    virtual bool rx_config(rx_config_params_t* config, int8_t* datarate );

    virtual bool tx_config(tx_config_params_t* config, int8_t* tx_power,
                           lorawan_time_t* tx_toa);

    virtual uint8_t link_ADR_request(adr_req_params_t* params, int8_t* dr_out,
                                     int8_t* tx_power_out, uint8_t* nb_rep_out,
                                     uint8_t* nb_bytes_parsed);

    virtual uint8_t accept_rx_param_setup_req(rx_param_setup_req_t* params);

private:

    /*!
     * LoRaMAC channels
     */
    channel_params_t channels[CN470_MAX_NB_CHANNELS];

    /*!
     * LoRaMac bands
     */
    band_t bands[CN470_MAX_NB_BANDS];

    /*!
     * LoRaMac channel mask
     */
    uint16_t channel_mask[CN470_CHANNEL_MASK_SIZE];

    /*!
     * LoRaMac default channel mask
     */
    uint16_t default_channel_mask[CN470_CHANNEL_MASK_SIZE];
};

#endif /* MBED_OS_LORAPHY_CN470_H_ */
