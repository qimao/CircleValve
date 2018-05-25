/*
 * Copyright (c) 2017, Arm Limited and affiliates.
 * SPDX-License-Identifier: Apache-2.0
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

#include "stdint.h"
#include "stdbool.h"
#include <cstddef>
#include "nsapi_types.h"
#include "ATHandler.h"
#include "FileHandle_stub.h"
#include "Callback.h"


namespace ATHandler_stub {
    extern nsapi_error_t nsapi_error_value;
    extern uint8_t nsapi_error_ok_counter;
    extern int int_value;
    extern ssize_t ssize_value;
    extern char* read_string_value;
    extern size_t size_value;
    extern size_t return_given_size;
    extern bool bool_value;
    extern uint8_t resp_info_true_counter;
    extern uint8_t uint8_value;
    extern mbed::FileHandle_stub *fh_value;
    extern mbed::device_err_t device_err_value;
    extern mbed::Callback<void()> callback;
}
