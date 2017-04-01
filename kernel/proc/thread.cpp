#include <stddef.h>
#include <stdint.h>

#include <kernel/tty.hpp>
#include <kernel/cpu.hpp>
#include <kernel/idt.hpp>
#include <kernel/timer.hpp>

#include <kernel/proc/process.hpp>
#include <kernel/proc/scheduler.hpp>
#include <kernel/proc/thread.hpp>

#include <kernel/memory/physical.hpp>
#include <kernel/memory/heap.hpp>
#include <kernel/memory/alloc.hpp>

std::vector<Thread *> thread_list = std::vector<Thread *>();

static int next_tid = 1;

static void test_thread() {
	for(;;) {
		bool hello = true;
		//BochsConsolePrintChar('a');
	}
		//terminal_printf("1");
}

// static void test_thread2() {
// 	//MAGIC_BREAK;
// 	BochsConsolePrintChar('B');
// 	for(;;) {
// 		BochsConsolePrintChar('b');
// 	}
// 		//terminal_printf("2");
// }

/**
*  This is the hub of the process list. For the moment, this function is only called
* 	ONCE - for the "kernel" process. Every new thread is attached to this initial
*   process for the time being.
*/
void start_thread(char* title, void_fn entry) {
	Thread *thread = new Thread();
	memcpy(thread->title, title, strlen(title));
	thread->thread_id = next_tid;

	thread->entry_ptr = entry;
	thread->state_reg.eip = (uint32_t)entry;

	thread_list.push_back(thread);

	next_tid++;
}

void init_kthreads() {
	start_thread("tty_update", &tty_update);
	start_thread("test", &test_thread);
}