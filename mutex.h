#ifndef __MUTEX__
#define __MUTEX__

#include <stdatomic.h>

typedef struct{
    int available; //valoarea 1 daca este liber si 0 in caz contrar
}mutex_special;

void initialize(mutex_special* mutex);
void acquire(mutex_special* mutex);
void release(mutex_special* mutex);


typedef struct{
    int count;
    mutex_special myMutex;
}semafor_special;

int wait(semafor_special *s);
int signal(semafor_special *s);
void initialize_sem(semafor_special *s, int val);

typedef struct{
    mutex_special citire;
    mutex_special scriere;
    int numar_cititori;
}rw_lock_special;

void init_rw_lock(rw_lock_special* rw);
void begin_read(rw_lock_special* rw);
void end_read(rw_lock_special* rw);
void begin_write(rw_lock_special* rw);
void end_write(rw_lock_special* rw);

#endif