/*
 * Copyright (c) 2016 ARM Limited. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef SN_CONFIG_H
#define SN_CONFIG_H

/**
* \brief Configuration options (set of defines and values)
*
*  This lists set of compile-time options that needs to be used to enable
*  or disable features selectively, and set the values for the mandatory
*  parameters.
*/

/**
 * \def SN_COAP_DUPLICATION_MAX_MSGS_COUNT
 * \brief For Message duplication detection
 * Init value for the maximum count of messages to be stored for duplication detection
 * Setting of this value to 0 will disable duplication check, also reduce use of ROM memory
 * Default is set to 1.
 */
#undef SN_COAP_DUPLICATION_MAX_MSGS_COUNT   /* 1 */

/**
 * \def SN_COAP_MAX_BLOCKWISE_PAYLOAD_SIZE
 *
 * \brief For Message blockwising
 * Init value for the maximum payload size to be sent and received at one blockwise message
 * Setting of this value to 0 will disable this feature, and also reduce use of ROM memory
 * Note: This define is common for both received and sent Blockwise messages
 */
#undef SN_COAP_MAX_BLOCKWISE_PAYLOAD_SIZE   /* 0 */ // < Must be 2^x and x is at least 4. Suitable values: 0, 16, 32, 64, 128, 256, 512 and 1024

/**
 * \def COAP_DISABLE_OBS_FEATURE
 *
 * \brief Disables CoAP 'obs' sending feature
 * as part of registration message, this might be
 * needed to be enabled for some strict LWM2M server implementation.
 * By default, this feature is disabled.
 */
#undef COAP_DISABLE_OBS_FEATURE

/**
 * \def SN_COAP_DISABLE_RESENDINGS
 *
 * \brief Disables resending feature. Resending feature should not be needed
 * when using CoAP with TCP transport for example. By default resendings are
 * enabled. Set to 1 to disable.
 */
#undef SN_COAP_DISABLE_RESENDINGS          /* 0 */ // < Default re-sending are not disabled. Set to 1 to disable re-sendings

/**
 * \def SN_COAP_RESENDING_QUEUE_SIZE_MSGS
 *
 * \brief Sets the number of messages stored
 * in the resending queue. Default is 2
 */
#undef SN_COAP_RESENDING_QUEUE_SIZE_MSGS    /* 2  */ // < Default re-sending queue size - defines how many messages can be stored. Setting this to 0 disables feature

/**
 * \def SN_COAP_RESENDING_QUEUE_SIZE_BYTES
 *
 * \brief Sets the size of the re-sending buffer.
 * Setting this to 0 disables this feature.
 * By default, this feature is disabled.
 */
#undef SN_COAP_RESENDING_QUEUE_SIZE_BYTES   /* 0  */ // Default re-sending queue size - defines size of the re-sending buffer. Setting this to 0 disables feature

/**
 * \def SN_COAP_MAX_INCOMING_MESSAGE_SIZE
 *
 * \brief Sets the maximum size (in bytes) that
 * mbed Client will allow to be handled while
 * receiving big payload in blockwise mode.
 * Application can set this value based on their
 * available storage capability.
 * By default, maximum size is UINT16_MAX, 65535 bytes.
 */
#undef SN_COAP_MAX_INCOMING_MESSAGE_SIZE    /* UINT16_MAX */

/**
 * \def SN_COAP_MAX_NONBLOCKWISE_PAYLOAD_SIZE
 * \brief Sets the maximum payload size allowed before blockwising the message.
 * This option should only be used when using TCP and TLS as transport
 * with known maximum fragment size. This optimizes the number of messages
 * if it is possible to send larger than 1kB messages without blockwise transfer.
 * If payload length is larger than SN_COAP_MAX_NONBLOCKWISE_PAYLOAD_SIZE
 * it will be sent using blockwise transfer.
 * By default, this feature is disabled, 0 disables the feature, set to positive
 * value larger than SN_COAP_MAX_BLOCKWISE_PAYLOAD_SIZE to enable.
 * Note that value should be less than transport layer maximum fragment size.
 * Note that value has no effect if blockwise transfer is disabled.
 */
#undef SN_COAP_MAX_NONBLOCKWISE_PAYLOAD_SIZE        /* 0 */

#ifdef MBED_CLIENT_USER_CONFIG_FILE
#include MBED_CLIENT_USER_CONFIG_FILE
#endif

#endif // SN_CONFIG_H
