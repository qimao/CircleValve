/*
 * PackageLicenseDeclared: Apache-2.0
 * Copyright (c) 2016 ARM Limited
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

#ifndef _ARM_GATT_CLIENT_H_
#define _ARM_GATT_CLIENT_H_

#include <stddef.h>

#include "GattClient.h"

class ArmGattClient : public GattClient
{
public:
    static ArmGattClient &getInstance() {
        static ArmGattClient m_instance;
        return m_instance;
    }

public:
    ArmGattClient() {
        /* empty */
    }

private:

};

#endif /* _ARM_GATT_CLIENT_H_ */
