/* Copyright Joyent, Inc. and other Node contributors. All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include <uv.h>

TEST_DECLARE(bus_attach)
TEST_DECLARE(bus_signle_pipe_event_order)
TEST_DECLARE(bus_signle_timer_event_order)
TEST_DECLARE(bus_full_event_order)
TEST_DECLARE(ux_heap_test)
TEST_DECLARE(ux_loop_test)
TASK_LIST_START
    TEST_ENTRY(bus_signle_pipe_event_order)
    TEST_ENTRY(bus_signle_timer_event_order)
    TEST_ENTRY(bus_full_event_order)
    TEST_ENTRY(bus_attach)
    TEST_ENTRY(ux_heap_test)
    TEST_ENTRY(ux_loop_test)
TASK_LIST_END
