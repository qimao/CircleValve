/*
 * mbed SDK
 * Copyright (c) 2017 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// Automatically generated configuration file.
// DO NOT EDIT, content will be overwritten.

#ifndef __MBED_CONFIG_DATA__
#define __MBED_CONFIG_DATA__

// Configuration parameters
#define MBED_CONF_LORA_DEVICE_ADDRESS                     0x00000000                                                                                       // set by library:lora
#define MBED_CONF_LORA_DUTY_CYCLE_ON                      1                                                                                                // set by library:lora
#define MBED_CONF_LORA_DEVICE_EUI                         {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}                                                 // set by library:lora
#define MBED_CONF_EVENTS_SHARED_DISPATCH_FROM_APPLICATION 0                                                                                                // set by library:events
#define MBED_CONF_PPP_CELL_IFACE_BAUD_RATE                115200                                                                                           // set by library:ppp-cell-iface
#define MBED_CONF_PPP_CELL_IFACE_APN_LOOKUP               1                                                                                                // set by library:ppp-cell-iface
#define MBED_CONF_EVENTS_PRESENT                          1                                                                                                // set by library:events
#define MBED_CONF_LORA_PUBLIC_NETWORK                     1                                                                                                // set by library:lora
#define MBED_CONF_LORA_NB_TRIALS                          12                                                                                               // set by library:lora
#define MBED_CONF_LORA_NWKSKEY                            {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00} // set by library:lora
#define MBED_LFS_PROG_SIZE                                64                                                                                               // set by library:littlefs
#define MBED_CONF_LORA_PHY                                0                                                                                                // set by library:lora
#define MBED_CONF_PLATFORM_STDIO_FLUSH_AT_EXIT            1                                                                                                // set by library:platform
#define MBED_CONF_DRIVERS_UART_SERIAL_RXBUF_SIZE          256                                                                                              // set by library:drivers
#define NVSTORE_MAX_KEYS                                  16                                                                                               // set by library:nvstore
#define MBED_CONF_NSAPI_PRESENT                           1                                                                                                // set by library:nsapi
#define MBED_CONF_FILESYSTEM_PRESENT                      1                                                                                                // set by library:filesystem
#define MBED_CONF_CELLULAR_USE_APN_LOOKUP                 1                                                                                                // set by library:cellular
#define MBED_CONF_PPP_CELL_IFACE_AT_PARSER_TIMEOUT        8000                                                                                             // set by library:ppp-cell-iface
#define MBED_LFS_BLOCK_SIZE                               512                                                                                              // set by library:littlefs
#define MBED_CONF_PPP_CELL_IFACE_AT_PARSER_BUFFER_SIZE    256                                                                                              // set by library:ppp-cell-iface
#define MBED_CONF_PLATFORM_FORCE_NON_COPYABLE_ERROR       0                                                                                                // set by library:platform
#define MBED_CONF_LORA_LBT_ON                             0                                                                                                // set by library:lora
#define MBED_CONF_PLATFORM_STDIO_BAUD_RATE                9600                                                                                             // set by library:platform
#define CLOCK_SOURCE                                      USE_PLL_HSE_EXTC|USE_PLL_HSI                                                                     // set by target:NUCLEO_F103RB
#define MBED_CONF_PLATFORM_STDIO_BUFFERED_SERIAL          0                                                                                                // set by library:platform
#define MBED_LFS_INTRINSICS                               1                                                                                                // set by library:littlefs
#define MBED_CONF_EVENTS_SHARED_HIGHPRIO_STACKSIZE        1024                                                                                             // set by library:events
#define NVSTORE_ENABLED                                   1                                                                                                // set by library:nvstore
#define MBED_CONF_LORA_TX_MAX_SIZE                        64                                                                                               // set by library:lora
#define MBED_CONF_LORA_ADR_ON                             1                                                                                                // set by library:lora
#define MBED_LFS_READ_SIZE                                64                                                                                               // set by library:littlefs
#define MBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE       9600                                                                                             // set by library:platform
#define MBED_CONF_RTOS_PRESENT                            1                                                                                                // set by library:rtos
#define MBED_CONF_EVENTS_SHARED_EVENTSIZE                 256                                                                                              // set by library:events
#define MBED_CONF_LORA_APP_PORT                           15                                                                                               // set by library:lora
#define MBED_CONF_PLATFORM_STDIO_CONVERT_TTY_NEWLINES     0                                                                                                // set by library:platform
#define MBED_CONF_CELLULAR_RANDOM_MAX_START_DELAY         0                                                                                                // set by library:cellular
#define CLOCK_SOURCE_USB                                  0                                                                                                // set by target:NUCLEO_F103RB
#define MBED_CONF_LORA_APPSKEY                            {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00} // set by library:lora
#define MBED_CONF_LORA_APPLICATION_EUI                    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}                                                 // set by library:lora
#define MBED_CONF_DRIVERS_UART_SERIAL_TXBUF_SIZE          256                                                                                              // set by library:drivers
#define MBED_LFS_LOOKAHEAD                                512                                                                                              // set by library:littlefs
#define MBED_CONF_EVENTS_USE_LOWPOWER_TIMER_TICKER        0                                                                                                // set by library:events
#define MBED_CONF_LORA_APPLICATION_KEY                    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00} // set by library:lora
#define MBED_CONF_PLATFORM_STDIO_CONVERT_NEWLINES         0                                                                                                // set by library:platform
#define MBED_CONF_LORA_OVER_THE_AIR_ACTIVATION            1                                                                                                // set by library:lora
#define MBED_CONF_TARGET_LSE_AVAILABLE                    1                                                                                                // set by target:FAMILY_STM32
#define MBED_LFS_ENABLE_INFO                              0                                                                                                // set by library:littlefs
#define MBED_CONF_EVENTS_SHARED_STACKSIZE                 1024                                                                                             // set by library:events
#define MBED_CONF_EVENTS_SHARED_HIGHPRIO_EVENTSIZE        256                                                                                              // set by library:events
// Macros
#define UNITY_INCLUDE_CONFIG_H                                                                                                                             // defined by library:utest

#endif
