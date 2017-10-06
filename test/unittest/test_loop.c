#include "task.h"
#include <stdio.h>
#include <stdlib.h>
#include "../src/ux_internal.h"
#include <assert.h>


TEST_IMPL(ux_loop_test)
{
    ux_loop_t loop;

    ux_loop_init(&loop);

    ux_loop_run(&loop, UX_RUN_NOWAIT);

    return 0;
}
