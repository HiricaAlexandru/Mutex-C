#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "mutex.h"
#include <unistd.h>

mutex_special mymutex;
semafor_special semafor;
rw_lock_special rwlock;
//pthread_mutex_t lock;

struct myArgs{
    char message[100];
};

int counter;

void* incrementex(void* argumente){
    struct myArgs* arg = (struct myArgs*) argumente;
    
    //pthread_mutex_lock(&lock);
    acquire(&mymutex);

    printf("%s - a inceput cu valoarea - %d           ",arg->message,counter);

    counter++;

    printf("%s - a terminat cu valoarea - %d\n",arg->message,counter);
    //pthread_mutex_unlock(&lock);
    release(&mymutex);
    return NULL;

}

void* semaforEx(void *arg){
    wait(&semafor);
    printf("Am intrat!\n");
    //setam thread-ul sa fie inactiv timp de 4 secunde
    sleep(4);
    printf("Am iesit!\n");
    signal(&semafor);

}

void *writer(void *args)
{
    int *writer_id = (int *)args;
    int old_writer;

    printf("Writer %d is trying to write\n", *writer_id);

    begin_write(&rwlock);

    printf("Writer %d is writing...\n", *writer_id);
    sleep(2);
    old_writer = *writer_id;
    *writer_id = *writer_id+1;

    end_write(&rwlock);

    printf("Writer %d is leaving\n", old_writer);


    return NULL;
}

void *reader(void *args)
{
    int *reader_id = (int *)args;

    printf("Reader %d is trying to read\n", *reader_id);

    begin_read(&rwlock);

    printf("Reader %d is reading...\n", *reader_id);
    sleep(1);

    end_read(&rwlock);

    printf("Reader %d is leaving\n", *reader_id);


    return NULL;
}

int main(){
    //pthread_mutex_init(&lock, NULL);
    //TESTAREA FUNCTIONALITATII MUTEX
    pthread_t thread1, thread2,thread3,thread4,thread5,thread6;
    int err1, err2,err3,err4,err5,err6;
    initialize(&mymutex);

    counter = 0;

    struct myArgs argThread1;
    struct myArgs argThread2;

    strcpy(argThread1.message, "Thread1");
    strcpy(argThread2.message, "Thread2");

    err1 = pthread_create(&thread1,NULL,&incrementex,&argThread1);
    err2 = pthread_create(&thread2,NULL,&incrementex,&argThread2);

    if(err1!=0 || err2 != 0){
        printf("Eroare!");
    }

    pthread_join(thread1,NULL);
    pthread_join(thread2,NULL);

    //pthread_mutex_destroy(&lock);
    //--------------------------------TESTARE FUNCTIONALITATE SEMAFOR
    initialize_sem(&semafor,1);//initializam semaforul cu valoarea 1 (doar un thread il poate folosi)
    err3 = pthread_create(&thread3,NULL,&semaforEx,NULL);
    pthread_join(thread3,NULL);
    sleep(2); //in functie primul thread este facut sa astepte 4 secunde
    err4 = pthread_create(&thread4,NULL,&semaforEx,NULL);//thread-ul 2 asteapta ca primul thread sa termine(am setat ca semaforul sa aiba o singura valoare)

    pthread_join(thread3,NULL);
    pthread_join(thread4,NULL);
    //-----------------------TESTARE FUNCTIONALITATE RW_LOCK
    
    init_rw_lock(&rwlock);
    int *numar_shared =(int*) malloc(sizeof(int));
    *numar_shared = 1902;

    
    pthread_create(&thread5,NULL,&writer,numar_shared);
    pthread_create(&thread6,NULL,&reader,numar_shared);

    pthread_join(thread5,NULL);
    pthread_join(thread6,NULL);

    free(numar_shared);



}