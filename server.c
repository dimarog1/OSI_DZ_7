#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdio.h>
#include <semaphore.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

#define SHM_NAME "shared_memory.h" // Имя разделяемой памяти
#define SEM_NAME "semaphore" // Имя семафора
#define SHM_SIZE 1024 // Размер разделяемой памяти

void handle_alarm(int signum) {
    printf("Сервер завершен по таймеру.\n");
    exit(0);
}

int main(int argc, char *argv[]) {
    // Проверка, запущена ли программа с флагом "timer"
    int useTimer = 0;
    if (argc > 1 && strcmp(argv[1], "timer") == 0) {
        useTimer = 1;
    }

    // Установка обработчика сигнала будильника, если необходимо
    if (useTimer) {
        signal(SIGALRM, handle_alarm);

        // Установка таймера на 10 секунд (можно изменить по необходимости)
        alarm(10);
    }

    sem_t *sem = sem_open(SEM_NAME, O_CREAT, 0666, 0);
    sem_wait(sem); // Ожидание сигнала от клиента о готовности данных
    int shm_fd = shm_open(SHM_NAME, O_RDONLY, 0666);
    int* ptr = mmap(0, SHM_SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
    for (int i = 0; i < 10; i++) {
        printf("%d\n", ptr[i]);
    }
    munmap(ptr, SHM_SIZE);
    close(shm_fd);
    shm_unlink(SHM_NAME);
    sem_close(sem);
    sem_unlink(SEM_NAME);

    return 0;
}