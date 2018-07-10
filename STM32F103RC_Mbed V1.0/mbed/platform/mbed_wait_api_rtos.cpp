/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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

// This implementation of the wait functions will be compiled only
// if the RTOS is present.
#ifdef MBED_CONF_RTOS_PRESENT

#include "platform/wait_api.h"
#include "hal/us_ticker_api.h"
#include "rtos/rtos.h"
#include "platform/critical.h"

void wait(float s) {
    wait_us(s * 1000000.0f);
}

void wait_ms(int ms) {
    wait_us(ms * 1000);
}

void wait_us(int us) {
    uint32_t start = us_ticker_read();
    // Use the RTOS to wait for millisecond delays if possible
    int ms = us / 1000;
    if ((ms > 0) && core_util_are_interrupts_enabled()) {
        Thread::wait((uint32_t)ms);
    }
    // Use busy waiting for sub-millisecond delays, or for the whole
    // interval if interrupts are not enabled
    while ((us_ticker_read() - start) < (uint32_t)us);
}

#endif // #if MBED_CONF_RTOS_PRESENT

