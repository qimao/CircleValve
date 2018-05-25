/* mbed Microcontroller Library
 * Copyright (c) 2006-2012 ARM Limited
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef QUEUE_H
#define QUEUE_H

#include <stdint.h>
#include <string.h>

#include "cmsis_os2.h"
#include "mbed_rtos_storage.h"
#include "platform/mbed_error.h"
#include "platform/NonCopyable.h"
#include "mbed_rtos1_types.h"

namespace rtos {
/** \addtogroup rtos */
/** @{*/
/**
 * \defgroup rtos_EventFlags EventFlags class
 * @{
 */
 
/** The Queue class allow to control, send, receive, or wait for messages.
 A message can be a integer or pointer value  to a certain type T that is send
 to a thread or interrupt service routine.
  @tparam  T         data type of a single message element.
  @tparam  queue_sz  maximum number of messages in queue.

 @note
 Memory considerations: The queue control structures will be created on current thread's stack, both for the mbed OS
 and underlying RTOS objects (static or dynamic RTOS memory pools are not being used).
*/
template<typename T, uint32_t queue_sz>
class Queue : private mbed::NonCopyable<Queue<T, queue_sz> > {
public:
    /** Create and initialize a message Queue.
     *
     * @note You cannot call this function from ISR context.
    */
    Queue() {
        memset(&_obj_mem, 0, sizeof(_obj_mem));
        osMessageQueueAttr_t attr = { 0 };
        attr.mq_mem = _queue_mem;
        attr.mq_size = sizeof(_queue_mem);
        attr.cb_mem = &_obj_mem;
        attr.cb_size = sizeof(_obj_mem);
        _id = osMessageQueueNew(queue_sz, sizeof(T*), &attr);
        MBED_ASSERT(_id);
    }
    /** Queue destructor
     *
     * @note You cannot call this function from ISR context.
     */
    ~Queue() {
        osMessageQueueDelete(_id);
    }

    /** Check if the queue is empty
     *
     * @return True if the queue is empty, false if not
     *
     * @note You may call this function from ISR context.
     */
    bool empty() const {
        return osMessageQueueGetCount(_id) == 0;
    }

    /** Check if the queue is full
     *
     * @return True if the queue is full, false if not
     *
     * @note You may call this function from ISR context.
     */
    bool full() const {
        return osMessageQueueGetSpace(_id) == 0;
    }

    /** Put a message in a Queue.
      @param   data      message pointer.
      @param   millisec  timeout value or 0 in case of no time-out. (default: 0)
      @param   prio      priority value or 0 in case of default. (default: 0)
      @return  status code that indicates the execution status of the function:
               @a osOK the message has been put into the queue.
               @a osErrorTimeout the message could not be put into the queue in the given time.
               @a osErrorResource not enough space in the queue.
               @a osErrorParameter internal error or non-zero timeout specified in an ISR.

      @note You may call this function from ISR context if the millisec parameter is set to 0.
    */
    osStatus put(T* data, uint32_t millisec=0, uint8_t prio=0) {
        return osMessageQueuePut(_id, &data, prio, millisec);
    }

    /** Get a message or Wait for a message from a Queue. Messages are retrieved in a descending priority order or
        first in first out when the priorities are the same.
      @param   millisec  timeout value or 0 in case of no time-out. (default: osWaitForever).
      @return  event information that includes the message in event.value and the status code in event.status:
               @a osEventMessage message received.
               @a osOK no message is available in the queue and no timeout was specified.
               @a osEventTimeout no message has arrived during the given timeout period.
               @a osErrorParameter a parameter is invalid or outside of a permitted range.

      @note You may call this function from ISR context if the millisec parameter is set to 0.
    */
    osEvent get(uint32_t millisec=osWaitForever) {
        osEvent event;
        T *data = NULL;
        osStatus_t res = osMessageQueueGet(_id, &data, NULL, millisec);

        switch (res) {
            case osOK:
                event.status = (osStatus)osEventMessage;
                event.value.p = data;
                break;
            case osErrorResource:
                event.status = osOK;
                break;
            case osErrorTimeout:
                event.status = (osStatus)osEventTimeout;
                break;
            case osErrorParameter:
            default:
                event.status = osErrorParameter;
                break;
        }
        event.def.message_id = _id;

        return event;
    }

private:
    osMessageQueueId_t            _id;
    char                          _queue_mem[queue_sz * (sizeof(T*) + sizeof(mbed_rtos_storage_message_t))];
    mbed_rtos_storage_msg_queue_t _obj_mem;
};
/** @}*/
/** @}*/

}
#endif

