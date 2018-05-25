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

#ifndef MBED_EXTENDED_TESTS
#error [NOT_SUPPORTED] Parallel pressure tests are not supported by default
#endif

#include "mbed.h"
#include MBED_CONF_APP_HEADER_FILE
#include "UDPSocket.h"
#include "greentea-client/test_env.h"
#include "unity/unity.h"
#include "utest.h"

using namespace utest::v1;


#ifndef MBED_CONF_APP_UDP_CLIENT_PACKET_PRESSURE_MIN
#define MBED_CONF_APP_UDP_CLIENT_PACKET_PRESSURE_MIN 64
#endif

#ifndef MBED_CONF_APP_UDP_CLIENT_PACKET_PRESSURE_MAX
#define MBED_CONF_APP_UDP_CLIENT_PACKET_PRESSURE_MAX 0x80000
#endif

#ifndef MBED_CONF_APP_UDP_CLIENT_PACKET_PRESSURE_TIMEOUT
#define MBED_CONF_APP_UDP_CLIENT_PACKET_PRESSURE_TIMEOUT 100
#endif

#ifndef MBED_CONF_APP_UDP_CLIENT_PACKET_PRESSURE_SEED
#define MBED_CONF_APP_UDP_CLIENT_PACKET_PRESSURE_SEED 0x6d626564
#endif

#ifndef MBED_CONF_APP_UDP_CLIENT_PACKET_PRESSURE_THREADS
#define MBED_CONF_APP_UDP_CLIENT_PACKET_PRESSURE_THREADS 3
#endif

#ifndef MBED_CONF_APP_UDP_CLIENT_PACKET_PRESSURE_DEBUG
#define MBED_CONF_APP_UDP_CLIENT_PACKET_PRESSURE_DEBUG false
#endif

#define STRINGIZE(x) STRINGIZE2(x)
#define STRINGIZE2(x) #x


// Simple xorshift pseudorandom number generator
class RandSeq
{
private:
    uint32_t x;
    uint32_t y;
    static const int A = 15;
    static const int B = 18;
    static const int C = 11;

public:
    RandSeq(uint32_t seed = MBED_CONF_APP_UDP_CLIENT_PACKET_PRESSURE_SEED)
        : x(seed), y(seed) {}

    uint32_t next(void)
    {
        x ^= x << A;
        x ^= x >> B;
        x ^= y ^ (y >> C);
        return x + y;
    }

    void skip(size_t size)
    {
        for (size_t i = 0; i < size; i++) {
            next();
        }
    }

    void buffer(uint8_t *buffer, size_t size)
    {
        RandSeq lookahead = *this;

        for (size_t i = 0; i < size; i++) {
            buffer[i] = lookahead.next() & 0xff;
        }
    }

    int cmp(uint8_t *buffer, size_t size)
    {
        RandSeq lookahead = *this;

        for (size_t i = 0; i < size; i++) {
            int diff = buffer[i] - (lookahead.next() & 0xff);
            if (diff != 0) {
                return diff;
            }
        }
        return 0;
    }
};

// Tries to get the biggest buffer possible on the device. Exponentially
// grows a buffer until heap runs out of space, and uses half to leave
// space for the rest of the program
void generate_buffer(uint8_t **buffer, size_t *size, size_t min, size_t max)
{
    size_t i = min;
    while (i < max) {
        void *b = malloc(i);
        if (!b) {
            i /= 8;
            if (i < min) {
                i = min;
            }
            break;
        }
        free(b);
        i *= 2;
    }

    *buffer = (uint8_t *)malloc(i);
    *size = i;
    TEST_ASSERT(buffer);
}


// Global variables shared between pressure tests
NetworkInterface* net;
SocketAddress udp_addr;
Timer timer;
Mutex iomutex;

// Single instance of a pressure test
class PressureTest
{
private:
    uint8_t *buffer;
    size_t buffer_size;

    UDPSocket sock;
    Thread thread;

public:
    PressureTest(uint8_t *buffer, size_t buffer_size)
        : buffer(buffer), buffer_size(buffer_size)
    {
    }

    void start()
    {
        osStatus status = thread.start(callback(this, &PressureTest::run));
        TEST_ASSERT_EQUAL(osOK, status);
    }

    void join()
    {
        osStatus status = thread.join();
        TEST_ASSERT_EQUAL(osOK, status);
    }

    void run()
    {
        // Tests exponentially growing sequences
        for (size_t size = MBED_CONF_APP_UDP_CLIENT_PACKET_PRESSURE_MIN;
                size < MBED_CONF_APP_UDP_CLIENT_PACKET_PRESSURE_MAX;
                size *= 2) {
            int err = sock.open(net);
            TEST_ASSERT_EQUAL(0, err);
            iomutex.lock();
            printf("UDP: %s:%d streaming %d bytes\r\n",
                   udp_addr.get_ip_address(), udp_addr.get_port(), size);
            iomutex.unlock();

            sock.set_blocking(false);

            // Loop to send/recv all data
            RandSeq tx_seq;
            RandSeq rx_seq;
            size_t rx_count = 0;
            size_t tx_count = 0;
            int known_time = timer.read_ms();
            size_t window = buffer_size;

            while (tx_count < size || rx_count < size) {
                // Send out packets
                if (tx_count < size) {
                    size_t chunk_size = size - tx_count;
                    if (chunk_size > window) {
                        chunk_size = window;
                    }

                    tx_seq.buffer(buffer, chunk_size);
                    int td = sock.sendto(udp_addr, buffer, chunk_size);

                    if (td > 0) {
                        if (MBED_CONF_APP_UDP_CLIENT_PACKET_PRESSURE_DEBUG) {
                            iomutex.lock();
                            printf("UDP: tx -> %d\r\n", td);
                            iomutex.unlock();
                        }
                        tx_seq.skip(td);
                        tx_count += td;
                    } else if (td != NSAPI_ERROR_WOULD_BLOCK) {
                        // We may fail to send because of buffering issues, revert to
                        // last good sequence and cut buffer in half
                        if (window > MBED_CONF_APP_UDP_CLIENT_PACKET_PRESSURE_MIN) {
                            window /= 2;
                        }

                        if (MBED_CONF_APP_UDP_CLIENT_PACKET_PRESSURE_DEBUG) {
                            iomutex.lock();
                            printf("UDP: Not sent (%d), window = %d\r\n", td, window);
                            iomutex.unlock();
                        }
                    }
                }

                // Prioritize receiving over sending packets to avoid flooding
                // the network while handling erronous packets
                while (rx_count < size) {
                    int rd = sock.recvfrom(NULL, buffer, buffer_size);
                    TEST_ASSERT(rd > 0 || rd == NSAPI_ERROR_WOULD_BLOCK);

                    if (rd > 0) {
                        if (MBED_CONF_APP_UDP_CLIENT_PACKET_PRESSURE_DEBUG) {
                            iomutex.lock();
                            printf("UDP: rx <- %d\r\n", rd);
                            iomutex.unlock();
                        }

                        if (rx_seq.cmp(buffer, rd) == 0) {
                            rx_seq.skip(rd);
                            rx_count += rd;
                            known_time = timer.read_ms();
                            if (window < MBED_CONF_APP_UDP_CLIENT_PACKET_PRESSURE_MAX) {
                                window += MBED_CONF_APP_UDP_CLIENT_PACKET_PRESSURE_MIN;
                            }
                        }
                    } else if (timer.read_ms() - known_time >
                               MBED_CONF_APP_UDP_CLIENT_PACKET_PRESSURE_TIMEOUT) {
                        // Dropped packet or out of order, revert to last good sequence
                        // and cut buffer in half
                        tx_seq = rx_seq;
                        tx_count = rx_count;
                        known_time = timer.read_ms();
                        if (window > MBED_CONF_APP_UDP_CLIENT_PACKET_PRESSURE_MIN) {
                            window /= 2;
                        }

                        if (MBED_CONF_APP_UDP_CLIENT_PACKET_PRESSURE_DEBUG) {
                            iomutex.lock();
                            printf("UDP: Dropped, window = %d\r\n", window);
                            iomutex.unlock();
                        }
                    } else if (rd == NSAPI_ERROR_WOULD_BLOCK) {
                        break;
                    }
                }
            }

            err = sock.close();
            TEST_ASSERT_EQUAL(0, err);
        }
    }
};

PressureTest *pressure_tests[MBED_CONF_APP_UDP_CLIENT_PACKET_PRESSURE_THREADS];


void test_udp_packet_pressure_parallel()
{
    uint8_t *buffer;
    size_t buffer_size;
    generate_buffer(&buffer, &buffer_size,
                    MBED_CONF_APP_UDP_CLIENT_PACKET_PRESSURE_MIN,
                    MBED_CONF_APP_UDP_CLIENT_PACKET_PRESSURE_MAX);

    size_t buffer_subsize = buffer_size / MBED_CONF_APP_UDP_CLIENT_PACKET_PRESSURE_THREADS;
    printf("MBED: Generated buffer %d\r\n", buffer_size);
    printf("MBED: Split into %d buffers %d\r\n",
           MBED_CONF_APP_UDP_CLIENT_PACKET_PRESSURE_THREADS,
           buffer_subsize);

    net = MBED_CONF_APP_OBJECT_CONSTRUCTION;
    int err =  MBED_CONF_APP_CONNECT_STATEMENT;
    TEST_ASSERT_EQUAL(0, err);

    printf("MBED: UDPClient IP address is '%s'\n", net->get_ip_address());

#if defined(MBED_CONF_APP_ECHO_SERVER_ADDR) && defined(MBED_CONF_APP_ECHO_SERVER_PORT)
    udp_addr.set_ip_address(MBED_CONF_APP_ECHO_SERVER_ADDR);
    udp_addr.set_port(MBED_CONF_APP_ECHO_SERVER_PORT);
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

    udp_addr.set_ip_address(ipbuf);
    udp_addr.set_port(port);
#endif /* MBED_CONF_APP_ECHO_SERVER_ADDR && MBED_CONF_APP_ECHO_SERVER_PORT */

    timer.start();

    // Startup pressure tests in parallel
    for (int i = 0; i < MBED_CONF_APP_UDP_CLIENT_PACKET_PRESSURE_THREADS; i++) {
        pressure_tests[i] = new PressureTest(&buffer[i * buffer_subsize], buffer_subsize);
        pressure_tests[i]->start();
    }

    for (int i = 0; i < MBED_CONF_APP_UDP_CLIENT_PACKET_PRESSURE_THREADS; i++) {
        pressure_tests[i]->join();
        delete pressure_tests[i];
    }

    timer.stop();
    printf("MBED: Time taken: %fs\r\n", timer.read());
    printf("MBED: Speed: %.3fkb/s\r\n",
           MBED_CONF_APP_UDP_CLIENT_PACKET_PRESSURE_THREADS *
           8 * (2 * MBED_CONF_APP_UDP_CLIENT_PACKET_PRESSURE_MAX -
                MBED_CONF_APP_UDP_CLIENT_PACKET_PRESSURE_MIN) / (1000 * timer.read()));

    net->disconnect();
}


// Test setup
utest::v1::status_t test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(120, "udp_echo");
    return verbose_test_setup_handler(number_of_cases);
}

Case cases[] = {
    Case("UDP packet pressure parallel", test_udp_packet_pressure_parallel),
};

Specification specification(test_setup, cases);

int main()
{
    return !Harness::run(specification);
}
