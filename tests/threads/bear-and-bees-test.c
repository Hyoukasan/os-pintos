#include <stdio.h>
#include "tests/threads/tests.h"
#include "threads/thread.h"
#include "threads/synch.h"
#include "threads/interrupt.h"
#include "devices/timer.h"
#include <list.h>
#include "bear-and-bees.h"

void create_bee (u32 count, thread_func* func);

void test_bear_and_bees (u32 count_bees, u32 capacity)
{
    monitor_init (capacity);

    create_bee (count_bees, bee_loop);
    thread_create("bear", PRI_DEFAULT, bear_loop, NULL)
}

void create_bee (u32 count, thread_func* func)
{
    u32 i;
    u32 bee_id = 0;
    for (i = 0; i < count; i++) {
        char name[8];
        snprintf (name, sizeof(name), "%u", ++bee_id);
        thread_create (name, PRI_DEFAULT, func, 0);
    }
}

void bee_loop (void *aux UNUSED) 
{
    collect_nectar ();
    return_to_hive ();
}