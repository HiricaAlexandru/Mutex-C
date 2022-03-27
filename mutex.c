#include "mutex.h"
#include <stdlib.h>
#include <stdio.h>

void initialize(mutex_special* mutex){
    mutex->available = 1;
}
void acquire(mutex_special* mutex){

    while(__sync_val_compare_and_swap(&mutex->available, 1,0) != 1)//cat timpul primul argument este egal cu al doilea
    ;
}
void release(mutex_special* mutex){
    mutex->available = 1;
}


int wait(semafor_special *s){
    acquire(&s->myMutex); //blocam mutex-ul
    
    while (atomic_load(&s->count) <= 0) //cat semaforul este utilizat la maxim
        ; 
    atomic_fetch_sub(&s->count, 1);     //eliberam o resursa din semafor
    release(&s->myMutex);                 //deblocam executia
    return 0;

}


int signal(semafor_special *s){
    return atomic_fetch_add(&s->count, 1); //eliberam semaforul si incremental valoarea cu 1
                                           //operatia este atomica

}
void initialize_sem(semafor_special *s, int val){
    initialize(&s->myMutex);
    atomic_init(&s->count, val); // initializam semaforul cu valoarea dorita
    //printf("%d",s->count);
}

void init_rw_lock(rw_lock_special* rw){
    rw->numar_cititori = 0;
    initialize(&rw->citire);//lock-ul pentru citire este deblocat
    initialize(&rw->scriere);//lock-ul pentru scriere este deblocat
}
void begin_read(rw_lock_special* rw){
    acquire(&rw->citire); //blocam lock-ul pentru citire
    rw->numar_cititori++; //adunam numarul de cititori

    if(rw->numar_cititori == 1){
        acquire(&rw->scriere); //blocam scrierea
    }

    release(&rw->citire); //deblocam citirea


}
void end_read(rw_lock_special* rw){
    acquire(&rw->citire);//blocam citirea
    rw->numar_cititori--;//scadem numarul de cititori

    if(rw->numar_cititori == 0){
        release(&rw->scriere);
    }
    release(&rw->citire);

}
void begin_write(rw_lock_special* rw){
    acquire(&rw->scriere);
}
void end_write(rw_lock_special* rw){
    release(&rw->scriere);
}

