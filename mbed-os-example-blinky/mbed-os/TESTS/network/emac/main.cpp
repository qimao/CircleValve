/*
 * Copyright (c) 2017, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#if !defined(MBED_CONF_APP_TEST_WIFI)      || \
    !defined(MBED_CONF_APP_TEST_ETHERNET)  || \
    !defined(MBED_CONF_APP_ECHO_SERVER)    || \
    !defined(MBED_CONF_APP_WIFI_SCAN)      || \
    !defined(MBED_CONF_APP_WIFI_SSID )     || \
    !defined(MBED_CONF_APP_WIFI_SECURITY)  || \
    !defined(MBED_CONF_APP_WIFI_PASSWORD)
#error [NOT_SUPPORTED] Requires parameters from mbed_app.json
#endif

#if !MBED_CONF_APP_TEST_WIFI && !MBED_CONF_APP_TEST_ETHERNET
#error [NOT_SUPPORTED] Either wifi or ethernet testing need to be enabled
#endif
#if MBED_CONF_APP_TEST_WIFI
#if !defined(TARGET_UBLOX_EVK_ODIN_W2) && !defined(TARGET_REALTEK_RTL8195AM)
#error [NOT_SUPPORTED] Tests are valid only for UBLOX_EVK_ODIN_W2 and REALTEK_RTL8195AM
#endif
#endif
#if MBED_CONF_APP_TEST_ETHERNET
#error [NOT_SUPPORTED] Ethernet testing not supported
#endif

#include "greentea-client/test_env.h"
#include "unity/unity.h"
#include "utest.h"

#include "emac_tests.h"
#include "emac_util.h"

using namespace utest::v1;

// Test setup
utest::v1::status_t test_setup(const size_t number_of_cases) {
#if !MBED_CONF_APP_ECHO_SERVER
    GREENTEA_SETUP(600, "default_auto");
#endif
    return verbose_test_setup_handler(number_of_cases);
}

Case cases[] = {
    Case("EMAC interface initialize", test_emac_initialize),
    Case("EMAC interface broadcast", test_emac_broadcast),
    Case("EMAC interface unicast", test_emac_unicast),
    Case("EMAC interface unicast frame length", test_emac_unicast_frame_len),
    Case("EMAC interface broadcast (run again)", test_emac_broadcast)
};

Specification specification(test_setup, cases);

int main()
{
    return !Harness::run(specification);
}

