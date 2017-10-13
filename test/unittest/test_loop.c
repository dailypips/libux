#include "task.h"
#include <stdio.h>
#include <stdlib.h>
#include "ux_internal.h"
#include <assert.h>


TEST_IMPL(ux_ctx_test)
{
    ux_ctx_t ctx;

    ux_ctx_init(&ctx);

    ux_run(&ctx, UX_RUN_NOWAIT);

    return 0;
}
