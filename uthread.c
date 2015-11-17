#include "types.h"
#include "stat.h"
#include "fcntl.h"
#include "user.h"
#include "x86.h"
#define SSZ     64

/*
 * This is where you'll need to implement the user-level functions
 */

uint t_stack[SSZ];


void lock_init(lock_t *lock) {
    lock->locked = 1;
}

void lock_acquire(lock_t *lock) {
    while ((xchg(&lock->locked, 1)) != 0);
}

void lock_release(lock_t *lock) {
    xchg(&lock->locked, 0);
}

int thread_join(int pid) {
    int t_pid = join(pid);
    if (t_pid > 0 && t_pid < SSZ) free((void*)(t_stack[t_pid-1]));
    return t_pid;

}

int thread_create(void (*start_routine)(void *), void *arg) {
    void *stack = malloc(4096);
    if ((uint)stack % 4096) {
        stack += 4096 - ((uint)stack % 4096);
    }
    int t_pid = clone(start_routine, arg, stack);
    if (t_pid > 0 && t_pid < SSZ) t_stack[t_pid-1] = (uint)stack;
    return t_pid;
}
