#ifndef AUDIT_H
#define AUDIT_H

#include <list.h>
#include <stdint.h>

typedef int tid_t;

struct audit
  {
     tid_t tid;
     char name[16];
     uint64_t tick_start;
     uint64_t tick_end;

     struct list_elem elem; 
  };

void init_audit (void);
void audit_record_start (tid_t tid, const char *name);
void audit_record_end (tid_t tid);
void audit_print_all (void);

#endif /* threads/audit.h */