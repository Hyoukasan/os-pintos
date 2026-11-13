/*
  File for 'bear' task implementation.
*/

#include <stdio.h>
#include "tests/threads/tests.h"
#include "threads/malloc.h"
#include "threads/thread.h"
#include "threads/synch.h"
#include "devices/timer.h"

typedef unsigned int u32;

struct _monitor 
{
  struct lock pot;

  struct condition bear_sleep;
  struct condition pot_full;

  u32 capacity_pot;
  u32 honey_in_pot;
};

static struct _monitor m;

static void init(u32 pot_size UNUSED)
{
    lock_init (&m.pot);

    m.capacity_pot = pot_size;
    m.honey_in_pot = 0;

    cond_init (&m.bear_sleep);
    cond_init (&m.pot_full);
}

static void bear(void* arg UNUSED)
{
    msg("bear created.");

    while (true) {
      lock_acquire (&m.pot);
      if (m.honey_in_pot == m.capacity_pot) {
        m.honey_in_pot = 0;

        msg("Bear ate all the honey!");

        cond_broadcast (&m.pot_full, &m.pot);
      } else {
        cond_wait(&m.bear_sleep, &m.pot);
      }

      lock_release (&m.pot);
    }
}

static void bee(void* arg UNUSED)
{
    msg("bee %d created.", (int) arg);

    while (true) {
      timer_sleep(50);

      lock_acquire (&m.pot);
      
      if (m.honey_in_pot < m.capacity_pot) {
        m.honey_in_pot++;

        msg("Bee: %4s, total honey: %4u, ticks=%4llu", 
            thread_current()->name,
            m.honey_in_pot,
            (unsigned long long) timer_ticks ());
      }

      if (m.honey_in_pot >= m.capacity_pot) {
        cond_signal(&m.bear_sleep, &m.pot);
        cond_wait (&m.pot_full, &m.pot);
      }

      lock_release (&m.pot);
    }
}


void test_bear(u32 num_bees, u32 pot_size)
{
  u32 i;
  init(pot_size);

  thread_create("bear", PRI_DEFAULT, &bear, NULL);

  for(i = 0; i < num_bees; i++)
  {
    char name[32];
    snprintf(name, sizeof(name), "bee_%d", i + 1);
    thread_create(name, PRI_DEFAULT, &bee, (void*) (i+1) );
  }

  timer_msleep(5000);
  pass();    
}
