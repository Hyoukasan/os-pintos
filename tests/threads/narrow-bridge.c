
/* File for 'narrow_bridge' task implementation.  
   SPbSTU, IBKS, 2017 */

#include <stdio.h>
#include "tests/threads/tests.h"
#include "threads/thread.h"
#include "threads/synch.h"
#include "narrow-bridge.h"

static struct lock bridge;

static enum car_direction current_dir;
static struct semaphore wait_sema[2][2];
static bool is_waiting[2][2];
static int cars_on_bridge;

static bool was_last_emer = 0;

// Called before test. Can initialize some synchronization objects.
void narrow_bridge_init(void)
{
	lock_init (&bridge);	

	for (int p = 0; p < 2; p++) {
		for (int d = 0; d < 2; d++) {
			sema_init (&wait_sema[p][d], 0);
			is_waiting[p][d] = false;
		}
	}

	cars_on_bridge = 0;
	current_dir    = 0;
}

void arrive_bridge(enum car_priority prio UNUSED, enum car_direction dir UNUSED)
{
	lock_acquire (&bridge);

	while (true) {
		bool cannot_go = false;

		if (cars_on_bridge > 0 && dir != current_dir) {
			cannot_go = true;
		} 
		
		if (cars_on_bridge > 1) {
			cannot_go = true;
		}

		if (prio == 0 && (is_waiting[1][!dir] == true || is_waiting[1][dir] == true) && !was_last_emer) {
			cannot_go = true;
		}

		if (!cannot_go) {
			current_dir = dir;
			cars_on_bridge++;

			if (was_last_emer == 1) was_last_emer = 0;

			if (prio == car_emergency && (!list_empty (&wait_sema[car_normal][dir].waiters))) {
				was_last_emer = 1;
				sema_up (&wait_sema[car_normal][dir]);
			}

			lock_release (&bridge);
			break;
		} else {
			is_waiting[prio][dir] = true;

			lock_release (&bridge);
			sema_down (&wait_sema[prio][dir]);

			lock_acquire (&bridge);
			
			if (!list_empty (&wait_sema[prio][dir].waiters)) {
				sema_up (&wait_sema[prio][dir]);
			} else is_waiting[prio][dir] = false;
		}

	}

}

void exit_bridge(enum car_priority prio UNUSED, enum car_direction dir UNUSED)
{
	lock_acquire (&bridge);

	cars_on_bridge--;
	

	enum car_direction opp_dir = !dir; 
	for (int p = 1; p >= 0; p--) {
		if (cars_on_bridge == 0 && is_waiting[p][opp_dir]) {
			sema_up (&wait_sema[p][opp_dir]);
			break;
		}
		
		if (is_waiting[p][dir]) {
			sema_up (&wait_sema[p][dir]);
			break;
		}
	}


	lock_release (&bridge);
}
