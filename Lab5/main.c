#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t mutex;
int count = 0;

void* reader() {
    while (1) {
        pthread_mutex_lock(&mutex);
        printf("TID: %ld\n", (long)pthread_self());
        printf("Count = %d\n", count);
        pthread_mutex_unlock(&mutex);
        sleep(3);
    }
}

void* writer() {
    while (1) {
        pthread_mutex_lock(&mutex);
        count++;
        pthread_mutex_unlock(&mutex);
        sleep(3);
    }
}

int main(int argc, char** argv) {
    int mas[10];
    pthread_t readers[10];
    pthread_t writers;
    
    pthread_mutex_init(&mutex, NULL);
    
    pthread_create(&writers, NULL, writer, NULL);
    
    for (int i = 0; i < 10; i++) {
        mas[i] = i*i;
        pthread_create(&readers[i], NULL, reader, &mas[i]);
    }
    
    for (int i = 0; i < 10; i++) {
        pthread_join(readers[i], NULL);
    }
    
    pthread_join(writers, NULL);
    
    pthread_mutex_destroy(&mutex);
    return 0;
}
