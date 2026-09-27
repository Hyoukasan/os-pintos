#include <stdio.h>
#include "tests/threads/tests.h"
#include "threads/init.h"
#include "threads/malloc.h"
#include "threads/synch.h"
#include "threads/thread.h"
#include "devices/timer.h"

static void 
empty_thread(void *aux UNUSED)
{
    for(;;) {
        thread_yield();
    }
}

void
test_max_threads(void)
{
    size_t count_threads = 0;

    while (thread_create("empty", PRI_DEFAULT, empty_thread, NULL) != TID_ERROR) {
        count_threads++;
    }

    msg("Maximum threads: %zu", count_threads);
}