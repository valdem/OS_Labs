#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_rwlock_t rwlock;
int count = 0;

void* reader() {
    sleep(1);
    while (1) {
        pthread_rwlock_tryrdlock(&rwlock);
        printf("TID: %ld\n", (long)pthread_self());
        printf("Count = %d\n", count);
        pthread_rwlock_unlock(&rwlock);
        sleep(3);
    }
}

void* writer() {
    while (1) {
        pthread_rwlock_trywrlock(&rwlock);
        count++;
        pthread_rwlock_unlock(&rwlock);
        sleep(3);
    }
}

int main(int argc, char** argv) {
    int mas[10];
    pthread_t readers[10];
    pthread_t writers;
    
    pthread_rwlock_init(&rwlock, NULL);
    
    pthread_create(&writers, NULL, writer, NULL);
    
    for (int i = 0; i < 10; i++) {
        mas[i] = i*i;
        pthread_create(&readers[i], NULL, reader, &mas[i]);
    }
    
    for (int i = 0; i < 10; i++) {
        pthread_join(readers[i], NULL);
    }
    
    pthread_join(writers, NULL);
    
    pthread_rwlock_destroy(&rwlock);
    return 0;
}
