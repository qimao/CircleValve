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

#include "mbed.h"
#include "greentea-client/test_env.h"
#include "unity.h"
#include "utest.h"
#include "rtos.h"
#include "hal/us_ticker_api.h"

using namespace utest::v1;

extern uint32_t SystemCoreClock;

/* Macro to define delta based on CPU clock frequency.
 *
 * Note that some extra time is counted by the timer.
 * Additional time is caused by the function calls and
 * additional operations performed by wait and
 * stop functions before in fact timer is stopped. This may
 * add additional time to the counted result.
 *
 * To take in to account this extra time we introduce DELTA
 * value based on CPU clock (speed):
 *   DELTA = TOLERANCE_FACTOR / SystemCoreClock * US_FACTOR
 *
 *   e.g.
 *   For K64F          DELTA = (30000 / 120000000) * 1000000 = 250[us]
 *   For NUCLEO_F070RB DELTA = (30000 /  48000000) * 1000000 = 625[us]
 *   For NRF51_DK      DELTA = (30000 /  16000000) * 1000000 = 1875[us]
 */
#define US_PER_SEC       1000000
#define US_PER_MSEC      1000
#define TOLERANCE_FACTOR 30000.0f
#define US_FACTOR        1000000.0f

static const int delta_sys_clk_us = ((int) (TOLERANCE_FACTOR / (float)SystemCoreClock * US_FACTOR));

/* When test performs time measurement using Timer in sequence, then measurement error accumulates
 * in the successive attempts. */
 #define DELTA_US(i) (delta_sys_clk_us * i)
 #define DELTA_S(i)  ((float)delta_sys_clk_us * i / US_PER_SEC)
 #define DELTA_MS(i) (1 + ( (i * delta_sys_clk_us) / US_PER_MSEC))

#define TICKER_FREQ_1MHZ 1000000
#define TICKER_BITS 32

static Timer *p_timer = NULL;

/* Global variable used to simulate passage of time
 * in case when timer which uses user ticker is tested.
 */
static uint32_t curr_ticker_ticks_val;

/* User ticker interface function. */
static void stub_interface_init()
{
    /* do nothing. */
}

/* User ticker interface function - only this
 * ticker interface function is used by Timer API. */
static uint32_t stub_ticker_read(void)
{
    /* Simulate elapsed time. */
    return curr_ticker_ticks_val;
}

/* User ticker interface function. */
static void stub_disable_interrupt(void)
{
    /* do nothing. */
}

/* User ticker interface function. */
static void stub_clear_interrupt(void)
{
    /* do nothing. */
}

/* User ticker interface function. */
static void stub_set_interrupt(timestamp_t timestamp)
{
    /* do nothing. */
}

/* User ticker interface function. */
static void stub_fire_interrupt(void)
{
    /* do nothing. */
}

ticker_info_t info =
{ TICKER_FREQ_1MHZ, TICKER_BITS };

const ticker_info_t * stub_get_info(void)
{
    return &info;
}

/* User ticker event queue. */
static ticker_event_queue_t my_events = { 0 };

/* User ticker interface data. */
static const ticker_interface_t us_interface = {
    .init = stub_interface_init,
    .read = stub_ticker_read, /* Only this function is used by the Timer. */
    .disable_interrupt = stub_disable_interrupt,
    .clear_interrupt = stub_clear_interrupt,
    .set_interrupt = stub_set_interrupt,
    .fire_interrupt = stub_fire_interrupt,
    .get_info = stub_get_info,
};

/* User ticker data structure. */
static const ticker_data_t us_data = {
    .interface = &us_interface,
    .queue = &my_events
};

/* Function which returns user ticker data. */
const ticker_data_t* get_user_ticker_data(void)
{
    return &us_data;
}

/* Initialisation of the Timer object which uses
 * ticker data provided by the user.
 *
 * */
utest::v1::status_t timer_user_ticker_setup_handler(const Case *const source, const size_t index_of_case)
{
    p_timer = new Timer(get_user_ticker_data());

    /* Check if Timer object has been created. */
    TEST_ASSERT_NOT_NULL(p_timer);

    return greentea_case_setup_handler(source, index_of_case);
}

/* Initialisation of the Timer object which uses
 * default os ticker data.
 *
 * */
utest::v1::status_t timer_os_ticker_setup_handler(const Case *const source, const size_t index_of_case)
{
    p_timer = new Timer();

    /* Check if Timer object has been created. */
    TEST_ASSERT_NOT_NULL(p_timer);

    return greentea_case_setup_handler(source, index_of_case);
}

/* Test finalisation.
 *
 * */
utest::v1::status_t cleanup_handler(const Case *const source, const size_t passed, const size_t failed, const failure_t reason)
{
    delete p_timer;

    p_timer = NULL;

    return greentea_case_teardown_handler(source, passed, failed, reason);
}

/* This test verifies if timer is stopped after
 * creation.
 *
 * Note: this function assumes that Timer uses os ticker.
 *
 * Given Timer has been successfully created.
 * When read of timer elapsed time is requested.
 * Then result is always 0.
 */
void test_timer_creation_os_ticker()
{
    /* Check results. */
    TEST_ASSERT_EQUAL_FLOAT(0, p_timer->read());
    TEST_ASSERT_EQUAL_INT32(0, p_timer->read_ms());
    TEST_ASSERT_EQUAL_INT32(0, p_timer->read_us());
    TEST_ASSERT_EQUAL_UINT64(0, p_timer->read_high_resolution_us());

    /* Wait 10 ms.
     * After that operation timer read routines should still return 0. */
    wait_ms(10);

    /* Check results. */
    TEST_ASSERT_EQUAL_FLOAT(0, p_timer->read());
    TEST_ASSERT_EQUAL_INT32(0, p_timer->read_ms());
    TEST_ASSERT_EQUAL_INT32(0, p_timer->read_us());
    TEST_ASSERT_EQUAL_UINT64(0, p_timer->read_high_resolution_us());
}

/* This test verifies if timer is stopped after
 * creation.
 *
 * Note: this function assumes that Timer uses user/fake ticker
 * which returns time value provided in curr_ticker_ticks_val
 * global variable.
 *
 * Given Timer has been successfully created.
 * When read of timer elapsed time is requested.
 * Then result is always 0.
 */
void test_timer_creation_user_ticker()
{
    /* For timer which is using user ticker simulate timer
     * creation time (irrelevant in case of os ticker). */
    curr_ticker_ticks_val = 10000;

    /* Check results. */
    TEST_ASSERT_EQUAL_FLOAT(0, p_timer->read());
    TEST_ASSERT_EQUAL_INT32(0, p_timer->read_ms());
    TEST_ASSERT_EQUAL_INT32(0, p_timer->read_us());
    TEST_ASSERT_EQUAL_UINT64(0, p_timer->read_high_resolution_us());

    /* Simulate that 10 ms has elapsed.
     * After that operation timer read routines should still return 0. */
    curr_ticker_ticks_val += 10000;

    /* Check results. */
    TEST_ASSERT_EQUAL_FLOAT(0, p_timer->read());
    TEST_ASSERT_EQUAL_INT32(0, p_timer->read_ms());
    TEST_ASSERT_EQUAL_INT32(0, p_timer->read_us());
    TEST_ASSERT_EQUAL_UINT64(0, p_timer->read_high_resolution_us());
}

/* This test verifies verifies if read(), read_us(), read_ms(),
 * read_high_resolution_us() functions returns valid values.
 *
 * Note: this function assumes that Timer uses user/fake ticker
 * which returns time value provided in curr_ticker_ticks_val
 * global variable.
 *
 * Given Timer has been successfully created and
 * few times started and stopped after a specified period of time.
 * When timer read request is performed.
 * Then read functions return accumulated time elapsed between starts
 * and stops.
 */
void test_timer_time_accumulation_user_ticker()
{
    /* Simulate that current time is equal to 0 us. */
    curr_ticker_ticks_val = 0;

    /* Start the timer. */
    p_timer->start();

    /* -- Simulate that current time is equal to 1 us -- */
    curr_ticker_ticks_val = 1;

    /* Stop the timer. */
    p_timer->stop();

    /* Check results - 1 us has elapsed. */
    TEST_ASSERT_EQUAL_FLOAT(0.000001f, p_timer->read());
    TEST_ASSERT_EQUAL_INT32(0, p_timer->read_ms());
    TEST_ASSERT_EQUAL_INT32(1, p_timer->read_us());
    TEST_ASSERT_EQUAL_UINT64(1, p_timer->read_high_resolution_us());

    /* Simulate that 100 us has elapsed between stop and start. */
    curr_ticker_ticks_val = 101;

    /* Start the timer. */
    p_timer->start();

    /* -- Simulate that current time is equal to 225 us -- */
    curr_ticker_ticks_val = 225;

    /* Stop the timer. */
    p_timer->stop();

    /* Check results - 125 us have elapsed. */
    TEST_ASSERT_EQUAL_FLOAT(0.000125f, p_timer->read());
    TEST_ASSERT_EQUAL_INT32(0, p_timer->read_ms());
    TEST_ASSERT_EQUAL_INT32(125, p_timer->read_us());
    TEST_ASSERT_EQUAL_UINT64(125, p_timer->read_high_resolution_us());

    /* Simulate that 100 us has elapsed between stop and start. */
    curr_ticker_ticks_val = 325;

    /* Start the timer. */
    p_timer->start();

    /* -- Simulate that current time is equal to 1200 us -- */
    curr_ticker_ticks_val = 1200;

    /* Stop the timer. */
    p_timer->stop();

    /* Check results - 1 ms has elapsed. */
    TEST_ASSERT_EQUAL_FLOAT(0.001000f, p_timer->read());
    TEST_ASSERT_EQUAL_INT32(1, p_timer->read_ms());
    TEST_ASSERT_EQUAL_INT32(1000, p_timer->read_us());
    TEST_ASSERT_EQUAL_UINT64(1000, p_timer->read_high_resolution_us());

    /* Simulate that 100 us has elapsed between stop and start. */
    curr_ticker_ticks_val = 1300;

    /* Start the timer. */
    p_timer->start();

    /* -- Simulate that current time is equal to 125300 us -- */
    curr_ticker_ticks_val = 125300;

    /* Stop the timer. */
    p_timer->stop();

    /* Check results - 125 ms have elapsed. */
    TEST_ASSERT_EQUAL_FLOAT(0.125000f, p_timer->read());
    TEST_ASSERT_EQUAL_INT32(125, p_timer->read_ms());
    TEST_ASSERT_EQUAL_INT32(125000, p_timer->read_us());
    TEST_ASSERT_EQUAL_UINT64(125000, p_timer->read_high_resolution_us());

    /* Simulate that 100 us has elapsed between stop and start. */
    curr_ticker_ticks_val = 125400;

    /* Start the timer. */
    p_timer->start();

    /* -- Simulate that current time is equal to 1000400 us -- */
    curr_ticker_ticks_val = 1000400;

    /* Stop the timer. */
    p_timer->stop();

    /* Check results - 1 s has elapsed. */
    TEST_ASSERT_EQUAL_FLOAT(1.000000f, p_timer->read());
    TEST_ASSERT_EQUAL_INT32(1000, p_timer->read_ms());
    TEST_ASSERT_EQUAL_INT32(1000000, p_timer->read_us());
    TEST_ASSERT_EQUAL_UINT64(1000000, p_timer->read_high_resolution_us());

    /* Simulate that 100 us has elapsed between stop and start. */
    curr_ticker_ticks_val = 1000500;

    /* Start the timer. */
    p_timer->start();

    /* -- Simulate that current time is equal to 125000500 us -- */
    curr_ticker_ticks_val = 125000500;

    /* Stop the timer. */
    p_timer->stop();

    /* Check results - 125 s have elapsed. */
    TEST_ASSERT_EQUAL_FLOAT(125.000000f, p_timer->read());
    TEST_ASSERT_EQUAL_INT32(125000, p_timer->read_ms());
    TEST_ASSERT_EQUAL_INT32(125000000, p_timer->read_us());
    TEST_ASSERT_EQUAL_UINT64(125000000, p_timer->read_high_resolution_us());

    /* Simulate that 100 us has elapsed between stop and start. */
    curr_ticker_ticks_val = 125000600;

    /* Start the timer. */
    p_timer->start();

    /* -- Simulate that current time is equal to MAX_INT_32 us + 600 us (delays
     *    between stops and starts) -- */

    /* Note that ticker is based on unsigned 32-bit int microsecond counters
     * while timers are based on 32-bit signed int microsecond counters,
     * so timers can only count up to a maximum of 2^31-1 microseconds i.e.
     * 2147483647 us (about 35 minutes). */
    curr_ticker_ticks_val = 2147484247;

    /* Stop the timer. */
    p_timer->stop();

    /* Check results - 2147483647 (MAX_INT_32) us have elapsed. */
    TEST_ASSERT_EQUAL_FLOAT(2147.483647f, p_timer->read());
    TEST_ASSERT_EQUAL_INT32(2147483, p_timer->read_ms());
    TEST_ASSERT_EQUAL_INT32(2147483647, p_timer->read_us());
    TEST_ASSERT_EQUAL_UINT64(2147483647, p_timer->read_high_resolution_us());
}

/* This test verifies if read(), read_us(), read_ms(),
 * read_high_resolution_us()
 * functions return time accumulated between
 * timer starts and stops.
 *
 * Note this function assumes that Timer uses os ticker.
 *
 * Given Timer has been successfully created and
 * few times started and stopped after a specified period of time.
 * When timer read request is performed.
 * Then read functions return accumulated time elapsed between starts
 * and stops.
 */
void test_timer_time_accumulation_os_ticker()
{
    /* Start the timer. */
    p_timer->start();

    /* Wait 10 ms. */
    wait_ms(10);

    /* Stop the timer. */
    p_timer->stop();

    /* Check results - totally 10 ms have elapsed. */
    TEST_ASSERT_FLOAT_WITHIN(DELTA_S(1), 0.010f, p_timer->read());
    TEST_ASSERT_INT32_WITHIN(DELTA_MS(1), 10, p_timer->read_ms());
    TEST_ASSERT_INT32_WITHIN(DELTA_US(1), 10000, p_timer->read_us());
    TEST_ASSERT_UINT64_WITHIN(DELTA_US(1), 10000, p_timer->read_high_resolution_us());

    /* Wait 50 ms - this is done to show that time elapsed when
     * the timer is stopped does not have influence on the
     * timer counted time. */
    wait_ms(50);

    /* ------ */

    /* Start the timer. */
    p_timer->start();

    /* Wait 20 ms. */
    wait_ms(20);

    /* Stop the timer. */
    p_timer->stop();

    /* Check results - totally 30 ms have elapsed. */
    TEST_ASSERT_FLOAT_WITHIN(DELTA_S(2), 0.030f, p_timer->read());
    TEST_ASSERT_INT32_WITHIN(DELTA_MS(2), 30, p_timer->read_ms());
    TEST_ASSERT_INT32_WITHIN(DELTA_US(2), 30000, p_timer->read_us());
    TEST_ASSERT_UINT64_WITHIN(DELTA_US(2), 30000, p_timer->read_high_resolution_us());

    /* Wait 50 ms - this is done to show that time elapsed when
     * the timer is stopped does not have influence on the
     * timer counted time. */

    /* ------ */

    /* Start the timer. */
    p_timer->start();

    /* Wait 30 ms. */
    wait_ms(30);

    /* Stop the timer. */
    p_timer->stop();

    /* Check results - totally 60 ms have elapsed. */
    TEST_ASSERT_FLOAT_WITHIN(DELTA_S(3), 0.060f, p_timer->read());
    TEST_ASSERT_INT32_WITHIN(DELTA_MS(3), 60, p_timer->read_ms());
    TEST_ASSERT_INT32_WITHIN(DELTA_US(3), 60000, p_timer->read_us());
    TEST_ASSERT_UINT64_WITHIN(DELTA_US(3), 60000, p_timer->read_high_resolution_us());

    /* Wait 50 ms - this is done to show that time elapsed when
     * the timer is stopped does not have influence on the
     * timer time. */
    wait_ms(50);

    /* ------ */

    /* Start the timer. */
    p_timer->start();

    /* Wait 1 sec. */
    wait_ms(1000);

    /* Stop the timer. */
    p_timer->stop();

    /* Check results - totally 1060 ms have elapsed. */
    TEST_ASSERT_FLOAT_WITHIN(DELTA_S(4), 1.060f, p_timer->read());
    TEST_ASSERT_INT32_WITHIN(DELTA_MS(4), 1060, p_timer->read_ms());
    TEST_ASSERT_INT32_WITHIN(DELTA_US(4), 1060000, p_timer->read_us());
    TEST_ASSERT_UINT64_WITHIN(DELTA_US(4), 1060000, p_timer->read_high_resolution_us());
}

/* This test verifies if reset() function resets the timer
 * counted time.
 *
 * Note this function assumes that Timer uses os ticker.
 *
 * Given timer has been started and stopped once, then reset
 * operation was performed.
 * When timer is started and stopped next time.
 * Then timer read functions returns only the the second
 * measured time.
 */
void test_timer_reset_os_ticker()
{
    /* First measure 10 ms delay. */
    p_timer->start();

    /* Wait 10 ms. */
    wait_ms(10);

    /* Stop the timer. */
    p_timer->stop();

    /* Check results - totally 10 ms elapsed. */
    TEST_ASSERT_FLOAT_WITHIN(DELTA_S(1), 0.010f, p_timer->read());
    TEST_ASSERT_INT32_WITHIN(DELTA_MS(1), 10, p_timer->read_ms());
    TEST_ASSERT_INT32_WITHIN(DELTA_US(1), 10000, p_timer->read_us());
    TEST_ASSERT_UINT64_WITHIN(DELTA_US(1), 10000, p_timer->read_high_resolution_us());

    /* Reset the timer - previous measured time should be lost now. */
    p_timer->reset();

    /* Now measure 20 ms delay. */
    p_timer->start();

    /* Wait 20 ms. */
    wait_ms(20);

    /* Stop the timer. */
    p_timer->stop();

    /* Check results - 20 ms elapsed since the reset. */
    TEST_ASSERT_FLOAT_WITHIN(DELTA_S(1), 0.020f, p_timer->read());
    TEST_ASSERT_INT32_WITHIN(DELTA_MS(1), 20, p_timer->read_ms());
    TEST_ASSERT_INT32_WITHIN(DELTA_US(1), 20000, p_timer->read_us());
    TEST_ASSERT_UINT64_WITHIN(DELTA_US(1), 20000, p_timer->read_high_resolution_us());
}

/* This test verifies if reset() function resets the timer
 * counted time.
 *
 * Note this function assumes that Timer uses user ticker.
 *
 * Given timer has been started and stopped once, then reset
 * operation was performed.
 * When timer is started and stopped next time.
 * Then timer read functions returns only the the second
 * measured time.
 */
void test_timer_reset_user_ticker()
{
    /* For timer which is using user ticker simulate set current
     * time (irrelevant in case of os ticker). */
    curr_ticker_ticks_val = 0;

    /* First measure 10 ms delay. */
    p_timer->start();

    /* Simulate that 10 ms have elapsed. */
    curr_ticker_ticks_val = 10000;

    /* Stop the timer. */
    p_timer->stop();

    /* Check results - totally 10 ms elapsed. */
    TEST_ASSERT_EQUAL_FLOAT(0.010f, p_timer->read());
    TEST_ASSERT_EQUAL_INT32(10, p_timer->read_ms());
    TEST_ASSERT_EQUAL_INT32(10000, p_timer->read_us());
    TEST_ASSERT_EQUAL_UINT64(10000, p_timer->read_high_resolution_us());

    /* Reset the timer - previous measured time should be lost now. */
    p_timer->reset();

    /* Now measure 20 ms delay. */
    p_timer->start();

    /* Simulate that 20 ms have elapsed. */
    curr_ticker_ticks_val = 30000;

    /* Stop the timer. */
    p_timer->stop();

    /* Check results - 20 ms elapsed since the reset. */
    TEST_ASSERT_EQUAL_FLOAT(0.020f, p_timer->read());
    TEST_ASSERT_EQUAL_INT32(20, p_timer->read_ms());
    TEST_ASSERT_EQUAL_INT32(20000, p_timer->read_us());
    TEST_ASSERT_EQUAL_UINT64(20000, p_timer->read_high_resolution_us());
}

/* This test verifies if calling start() for already
 * started timer does nothing.
 *
 * Note this function assumes that Timer uses os ticker.
 *
 * Given timer is already started.
 * When timer is started again.
 * Then second start operation is ignored.
 */
void test_timer_start_started_timer_os_ticker()
{
    /* Start the timer. */
    p_timer->start();

    /* Wait 10 ms. */
    wait_ms(10);

    /* Now start timer again. */
    p_timer->start();

    /* Wait 20 ms. */
    wait_ms(20);

    /* Stop the timer. */
    p_timer->stop();

    /* Check results - 30 ms have elapsed since the first start. */
    TEST_ASSERT_FLOAT_WITHIN(DELTA_S(2), 0.030f, p_timer->read());
    TEST_ASSERT_INT32_WITHIN(DELTA_MS(2), 30, p_timer->read_ms());
    TEST_ASSERT_INT32_WITHIN(DELTA_US(2), 30000, p_timer->read_us());
    TEST_ASSERT_UINT64_WITHIN(DELTA_US(2), 30000, p_timer->read_high_resolution_us());
}

/* This test verifies if calling start() for already
 * started timer does nothing.
 *
 * Note this function assumes that Timer uses user ticker.
 *
 * Given timer is already started.
 * When timer is started again.
 * Then second start operation is ignored.
 */
void test_timer_start_started_timer_user_ticker()
{
    /* For timer which is using user ticker set current
     * time (irrelevant in case of os ticker). */
    curr_ticker_ticks_val = 0;

    /* Start the timer. */
    p_timer->start();

    /* Simulate that 10 ms have elapsed. */
    curr_ticker_ticks_val = 10000;

    /* Now start timer again. */
    p_timer->start();

    /* Simulate that 20 ms have elapsed. */
    curr_ticker_ticks_val = 30000;

    /* Stop the timer. */
    p_timer->stop();

    /* Check results - 30 ms have elapsed since the first start. */
    TEST_ASSERT_FLOAT_WITHIN(DELTA_S(2), 0.030f, p_timer->read());
    TEST_ASSERT_INT32_WITHIN(DELTA_MS(2), 30, p_timer->read_ms());
    TEST_ASSERT_INT32_WITHIN(DELTA_US(2), 30000, p_timer->read_us());
    TEST_ASSERT_UINT64_WITHIN(DELTA_US(2), 30000, p_timer->read_high_resolution_us());
}

/* This test verifies Timer float operator.
 *
 * Note this function assumes that Timer uses os ticker.
 *
 * Given timer is created and a time period time is counted.
 * When timer object is casted on float type.
 * Then counted type in seconds is returned by means of
 * read() function.
 */
void test_timer_float_operator_os_ticker()
{
    /* Start the timer. */
    p_timer->start();

    /* Wait 10 ms. */
    wait_ms(10);

    /* Stop the timer. */
    p_timer->stop();

    /* Check result - 10 ms elapsed. */
    TEST_ASSERT_FLOAT_WITHIN(DELTA_S(1), 0.010f, (float)(*p_timer));
}

/* This test verifies Timer float operator.
 *
 * Note this function assumes that Timer uses user ticker.
 *
 * Given timer is created and a time period time is counted.
 * When timer object is casted on float type.
 * Then counted type in seconds is returned by means of
 * read() function.
 */
void test_timer_float_operator_user_ticker()
{
    /* For timer which is using user ticker set current
     * time (irrelevant in case of os ticker). */
    curr_ticker_ticks_val = 0;

    /* Start the timer. */
    p_timer->start();

    /* Simulate that 10 ms have elapsed. */
    curr_ticker_ticks_val = 10000;

    /* Stop the timer. */
    p_timer->stop();

    /* Check result - 10 ms elapsed. */
    TEST_ASSERT_EQUAL_FLOAT(0.010f, (float)(*p_timer));
}

/* This test verifies if time counted by the timer is
 * valid.
 *
 * For this test Timer which uses os ticker
 * must be used.
 *
 * Given timer is created.
 * When timer is used to measure 1ms/10ms/100ms/1s
 * delays.
 * Then the results are valid (within acceptable range).
 */
template<int wait_val_us>
void test_timer_time_measurement()
{
    /* Start the timer. */
    p_timer->start();

    /* Wait <wait_val_us> us. */
    wait_us(wait_val_us);

    /* Stop the timer. */
    p_timer->stop();

    /* Check results. */
    TEST_ASSERT_FLOAT_WITHIN(DELTA_S(1), (float)wait_val_us / 1000000, p_timer->read());
    TEST_ASSERT_INT32_WITHIN(DELTA_MS(1), wait_val_us / 1000, p_timer->read_ms());
    TEST_ASSERT_INT32_WITHIN(DELTA_US(1), wait_val_us, p_timer->read_us());
    TEST_ASSERT_UINT64_WITHIN(DELTA_US(1), wait_val_us, p_timer->read_high_resolution_us());
}

utest::v1::status_t test_setup(const size_t number_of_cases) {
    GREENTEA_SETUP(15, "default_auto");
    return verbose_test_setup_handler(number_of_cases);
}

Case cases[] = {
    Case("Test: Timer (based on os ticker) is stopped after creation.", timer_os_ticker_setup_handler, test_timer_creation_os_ticker, cleanup_handler),
    Case("Test: Timer (based on user ticker) is stopped after creation.", timer_user_ticker_setup_handler, test_timer_creation_user_ticker, cleanup_handler),

    Case("Test: Timer (based on os ticker) - measured time accumulation.", timer_os_ticker_setup_handler, test_timer_time_accumulation_os_ticker, cleanup_handler),
    Case("Test: Timer (based on user ticker) measured time accumulation.", timer_user_ticker_setup_handler, test_timer_time_accumulation_user_ticker, cleanup_handler),

    Case("Test: Timer (based on os ticker) - reset.", timer_os_ticker_setup_handler, test_timer_reset_os_ticker, cleanup_handler),
    Case("Test: Timer (based on user ticker) - reset.", timer_user_ticker_setup_handler, test_timer_reset_user_ticker, cleanup_handler),

    Case("Test: Timer (based on os ticker) - start started timer.", timer_os_ticker_setup_handler, test_timer_start_started_timer_os_ticker, cleanup_handler),
    Case("Test: Timer (based on user ticker) - start started timer.", timer_user_ticker_setup_handler, test_timer_start_started_timer_user_ticker, cleanup_handler),

    Case("Test: Timer (based on os ticker) - float operator.", timer_os_ticker_setup_handler, test_timer_float_operator_os_ticker, cleanup_handler),
    Case("Test: Timer (based on user ticker) - float operator.", timer_user_ticker_setup_handler, test_timer_float_operator_user_ticker, cleanup_handler),

    Case("Test: Timer - time measurement 1 ms.", timer_os_ticker_setup_handler, test_timer_time_measurement<1000>, cleanup_handler),
    Case("Test: Timer - time measurement 10 ms.", timer_os_ticker_setup_handler, test_timer_time_measurement<10000>, cleanup_handler),
    Case("Test: Timer - time measurement 100 ms.", timer_os_ticker_setup_handler, test_timer_time_measurement<100000>, cleanup_handler),
    Case("Test: Timer - time measurement 1 s.", timer_os_ticker_setup_handler, test_timer_time_measurement<1000000>, cleanup_handler),
};

Specification specification(test_setup, cases);

int main() {
    return !Harness::run(specification);
}

