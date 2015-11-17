#include "types.h"
#include "stat.h"
#include "fcntl.h"
#include "user.h"
#include "x86.h"
#include "mmu.h"
//#define SSZ     64

/*
 * This is where you'll need to implement the user-level functions
 */

void *t_stack[64];


void lock_init(lock_t *lock) {
    lock->locked = 0;
}

void lock_acquire(lock_t *lock) {
    while ((xchg(&lock->locked, 1)) != 0);
}

void lock_release(lock_t *lock) {
    xchg(&lock->locked, 0);
}

int thread_join(int pid) {
    int t_pid = join(pid);
    if (t_pid > 0 && t_pid < 64) free(t_stack[t_pid]);
    return t_pid;

}

int thread_create(void (*start_routine)(void *), void *arg) {
    void *stack = malloc(PGSIZE*2);
    if ((uint)stack % PGSIZE) {
        stack += (PGSIZE - (uint)stack % PGSIZE);
    }
    int t_pid = clone(start_routine, arg, stack);
    if (t_pid > 0 && t_pid < 64) {
        t_stack[t_pid] = stack;
        return t_pid;
    }
    return -1;
}
