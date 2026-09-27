#include "threads/audit.h"
#include <debug.h>
#include <string.h>

#include "devices/timer.h"
#include "threads/malloc.h"

static struct list audit_list;

void
init_audit (void)
{
    list_init (&audit_list);
}

void 
audit_record_start (tid_t tid, const char *name)
{
    struct audit* new_entry = (struct audit*)malloc (sizeof(struct audit*));
    ASSERT (entry != NULL);

    entry->tid = tid;
    strlcpy (entry->name, name, sizeof (entry->name));
    entry->tick_start = 
}


void 
audit_record_end (tid_t tid)
{

}