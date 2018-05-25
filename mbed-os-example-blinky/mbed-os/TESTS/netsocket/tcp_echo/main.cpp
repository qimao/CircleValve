/*
 * Copyright (c) 2013-2017, ARM Limited, All Rights Reserved
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

#ifndef MBED_CONF_APP_CONNECT_STATEMENT
#error [NOT_SUPPORTED] No network configuration found for this target.
#endif

#include "mbed.h"
#include MBED_CONF_APP_HEADER_FILE
#include "TCPSocket.h"
#include "greentea-client/test_env.h"
#include "unity/unity.h"
#include "utest.h"

using namespace utest::v1;

#ifndef MBED_CONF_APP_TCP_CLIENT_ECHO_BUFFER_SIZE
#define MBED_CONF_APP_TCP_CLIENT_ECHO_BUFFER_SIZE 256
#endif

namespace
{
char tx_buffer[MBED_CONF_APP_TCP_CLIENT_ECHO_BUFFER_SIZE] = {0};
char rx_buffer[MBED_CONF_APP_TCP_CLIENT_ECHO_BUFFER_SIZE] = {0};
}

void prep_buffer(char *tx_buffer, size_t tx_size)
{
    for (size_t i = 0; i < tx_size; ++i) {
        tx_buffer[i] = (rand() % 10) + '0';
    }
}

void test_tcp_echo()
{
    int n = 0;
    NetworkInterface* net = MBED_CONF_APP_OBJECT_CONSTRUCTION;
    int err =  MBED_CONF_APP_CONNECT_STATEMENT;

    if (err) {
        printf("MBED: failed to connect with an error of %d\r\n", err);
        TEST_ASSERT_EQUAL(0, err);
    }

    printf("MBED: TCPClient IP address is '%s'\n", net->get_ip_address());

    bool result = false;

    TCPSocket sock(net);

#if defined(MBED_CONF_APP_ECHO_SERVER_ADDR) && defined(MBED_CONF_APP_ECHO_SERVER_PORT)
    printf("TCP: Connect to %s:%d\r\n", MBED_CONF_APP_ECHO_SERVER_ADDR, MBED_CONF_APP_ECHO_SERVER_PORT);
    SocketAddress tcp_addr(MBED_CONF_APP_ECHO_SERVER_ADDR, MBED_CONF_APP_ECHO_SERVER_PORT);
#else /* MBED_CONF_APP_ECHO_SERVER_ADDR && MBED_CONF_APP_ECHO_SERVER_PORT */
    char recv_key[] = "host_port";
    char ipbuf[60] = {0};
    char portbuf[16] = {0};
    unsigned int port = 0;

    greentea_send_kv("target_ip", net->get_ip_address());
    greentea_send_kv("host_ip", " ");
    greentea_parse_kv(recv_key, ipbuf, sizeof(recv_key), sizeof(ipbuf));

    greentea_send_kv("host_port", " ");
    greentea_parse_kv(recv_key, portbuf, sizeof(recv_key), sizeof(ipbuf));
    sscanf(portbuf, "%u", &port);

    printf("TCP: Connect to %s:%d\r\n", ipbuf, port);
    SocketAddress tcp_addr(ipbuf, port);
#endif /* MBED_CONF_APP_ECHO_SERVER_ADDR && MBED_CONF_APP_ECHO_SERVER_PORT */

    if (sock.connect(tcp_addr) == 0) {
        printf("tx_buffer buffer size: %u\r\n", sizeof(tx_buffer));
        printf("rx_buffer buffer size: %u\r\n", sizeof(rx_buffer));

        prep_buffer(tx_buffer, sizeof(tx_buffer));
#if defined(MBED_CONF_APP_TCP_ECHO_PREFIX)
        n = sock.recv(rx_buffer, sizeof(MBED_CONF_APP_TCP_ECHO_PREFIX));
        if (n >= 0) {
            printf("recv-ed prefix: %d bytes - %.*s  \n", n, n, rx_buffer);
        } else {
            printf("Network error in receiving prefix: %d\n", n);
        }
#endif /* MBED_CONF_APP_TCP_ECHO_PREFIX */
        const int ret = sock.send(tx_buffer, sizeof(tx_buffer));
        if (ret >= 0) {
            printf("sent %d bytes - %.*s  \n", ret, ret, tx_buffer);
        } else {
            printf("Network error %d\n", ret);
        }

        n = sock.recv(rx_buffer, sizeof(rx_buffer));
        if (n >= 0) {
            printf("recv %d bytes - %.*s  \n", n, n, rx_buffer);
        } else {
            printf("Network error %d\n", n);
        }

        result = !memcmp(tx_buffer, rx_buffer, sizeof(tx_buffer));
        TEST_ASSERT_EQUAL(ret, sizeof(rx_buffer));
        TEST_ASSERT_EQUAL(true, result);
    }

    sock.close();
    net->disconnect();
    TEST_ASSERT_EQUAL(true, result);
}


// Test setup
utest::v1::status_t test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(240, "tcp_echo");
    return verbose_test_setup_handler(number_of_cases);
}

Case cases[] = {
    Case("TCP echo", test_tcp_echo),
};

Specification specification(test_setup, cases);

int main()
{
    return !Harness::run(specification);
}
