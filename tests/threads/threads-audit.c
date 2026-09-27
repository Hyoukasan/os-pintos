#include <stdio.h>
#include "tests/threads/tests.h"
#include "threads/thread.h"
#include "devices/timer.h"

static void
audit_task (void *aux UNUSED)
{
    int dur = *((int *) aux);
    int64_t start = timer_ticks ();
    int64_t ticks_to_wait = dur * TIMER_FREQ;

    while (timer_elapsed (start) < ticks_to_wait) {
        thread_yield ();
    }
}

void 
test_threads_audit (void)
{
    static int sleep_times[10];

    for (size_t i = 0; i < 10; i++) {
        char name[16];
        snprintf(name, sizeof(name), "t_audit_%d", i);

        sleep_times[i] = 1 + (i % 3);

        thread_create(name, PRI_DEFAULT, audit_task, &sleep_times[i]);
    }

    timer_sleep (5 * TIMER_FREQ);
}