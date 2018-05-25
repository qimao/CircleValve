/* Copyright (c) 2017-2018 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef LWIPOPTS_CONF_H
#define LWIPOPTS_CONF_H

#define LWIP_TRANSPORT_ETHERNET       1

/* The IEEE 802.3ac standard says the maximum Ethernet frame */
/* size is 1522 bytes to accommodate the four-byte VLAN tag. */
#define ETH_MAX_FLEN                  1522u /* recommended size for a VLAN frame */

/*
 * Maximum Transfer Unit
 * The IEEE 802.3 specification limits the data portion of the 802.3 frame
 * to a minimum of 46 and a maximum of 1522 bytes, this is on L2 level.
 */
#define ETH_L2_HEADER_LEN             22u

#define ETH_MAX_PAYLOAD_LEN           (ETH_MAX_FLEN - ETH_L2_HEADER_LEN)

/*
 * Set this value to 2 to ensure that payload address of packet buffers is
 * aligned on a 32 bits boundary.
 * The padding is removed before passing the packet to the ethernet driver,
 * hence defining this value to 2 will not prevent alignment issues inside the
 * ethernet driver.
 */
#define ETH_PAD_SIZE                  0

#endif /* LWIPOPTS_CONF_H */
