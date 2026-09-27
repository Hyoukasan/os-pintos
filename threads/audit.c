#include "threads/audit.h"
#include <debug.h>
#include <string.h>

#include "devices/timer.h"
#include "threads/malloc.h"

static struct audit audit_log;

static struct entry* get_entry (tid_t tid);

void
init_audit (void)
{
    list_init (&audit_log.entry_list);
    audit_log.count = 0;
}

void 
entry_record_start (tid_t tid, const char *name)
{
    struct entry* e = (struct audit*)malloc (sizeof(struct audit));
    ASSERT (new_entry != NULL);

    e->tid = tid;
    strlcpy (e->name, name, sizeof (e->name));
    e->tick_start = timer_ticks ();
    e->tick_end = 0;

    list_push_back (&audit_log.entry_list, &e);

    audit_log.count++;
}

void 
entry_record_end (tid_t tid)
{
    struct entry* e = get_entry (tid);

    if (e != NULL) {
        e->end_tick = timer_ticks ();
    }
}

static struct entry* 
get_entry (tid_t tid) 
{

}