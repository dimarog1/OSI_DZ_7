#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <time.h>
#include <signal.h>

#define SHM_NAME "shared_memory.h" // Имя разделяемой памяти
#define SEM_NAME "semaphore" // Имя семафора
#define SHM_SIZE 1024 // Размер разделяемой памяти

int main() {
    srand(time(NULL));

    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, SHM_SIZE);
    int* ptr = mmap(0, SHM_SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);
    sem_t *sem = sem_open(SEM_NAME, O_CREAT, 0666, 0);

    for (int i = 0; i < 10; i++) {
        ptr[i] = rand() % 1000;
    }

    sem_post(sem);
    munmap(ptr, SHM_SIZE);
    close(shm_fd);
    sem_close(sem);
    return 0;
}