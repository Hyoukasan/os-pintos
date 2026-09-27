#include "threads/audit.h"
#include <debug.h>
#include <string.h>
#include <stdio.h>

#include "devices/timer.h"
#include "threads/malloc.h"

static struct list audit_list;

static struct audit* get_audit (tid_t tid);

void
init_audit (void)
{
    list_init (&audit_list);
}

void 
audit_record_start (tid_t tid, const char *name)
{
    struct audit* a = malloc (sizeof(struct audit));
    ASSERT (a != NULL);

    a->tid = tid;
    strlcpy (a->name, name, sizeof (a->name));
    a->tick_start = timer_ticks ();
    a->tick_end = 0;

    list_push_back (&audit_list, &a->elem);
}

void 
audit_record_end (tid_t tid)
{
    struct audit* a = get_audit (tid);

    if (a != NULL) {
        a->tick_end = timer_ticks ();
    }
}

static struct audit* 
get_audit (tid_t tid) 
{
    struct list_elem *iter = list_begin (&audit_list);
    struct list_elem *end = list_end (&audit_list);

    struct audit *a;

    while (iter != end) {
        a = list_entry (iter, struct audit, elem);
        if (a->tid == tid) {
            return a;
        }

        iter = list_next (iter);
    }

    return NULL;
}

void audit_print_all (void) {
    if (list_empty (&audit_list)) {
        printf("Thread Audit Log: 0 processes.\n");
        return;
    }

    struct list_elem *iter;
    struct audit *a;

    while (!list_empty (&audit_list)) {
        iter = list_pop_front (&audit_list);
        a = list_entry (iter, struct audit, elem);

        printf("TID: %d NAME: %s START: %llu END: %llu ACTIVE %d\n", 
                a->tid, a->name, a->tick_start, a->tick_end, a->tick_end - a->tick_start);

        free (a);
    }


}


