#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "a2_helper.h"
#include <pthread.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>

typedef struct
{
    int value;
    sem_t logSem[2];
} TH_STRUCT;

// semafoare pentru Procesul 7
sem_t semafor1;
sem_t semafor2;

// semafoare pentru Procesele 8 si 7
sem_t *semafor3 = NULL;
sem_t *semafor4 = NULL;

//semfor pentru cele 5 threduri simulane 

sem_t semafor5;


void *thread_function7(void *arg)
{
    TH_STRUCT *s = (TH_STRUCT *)arg;
    // int id = *(int*)arg;

    // Thread 3
    // semafor1=(sem_t)(s->logSem[0]);
    // semafor2=(sem_t)(s->logSem[1]);

    if (s->value == 3)
    {

        info(BEGIN, 7, s->value);
        sem_post(&semafor1);

        sem_wait(&semafor2);
        info(END, 7, s->value);
    }

    else if (s->value == 1)
    {
        sem_wait(&semafor1);
        info(BEGIN, 7, s->value);

        info(END, 7, s->value);
        sem_post(&semafor2);
    }

    // Thread 2
    if (s->value == 2)
    {
        info(BEGIN, 7, s->value);
    }

    if (s->value == 2)
    {
        info(END, 7, s->value);
    }

    // Thread 4
    if (s->value == 4)
    {
        sem_wait(semafor3);
        info(BEGIN, 7, s->value);

        info(END, 7, s->value);
        sem_post(semafor4);
    }
    return NULL;
}

void *thread_function5(void *arg)
{
    int id = *(int *)arg;
    info(BEGIN, 5, id);
    info(END, 5, id);
    sem_post(&semafor5);
    return NULL;
}

void *thread_function8(void *arg)
{
    int id = *(int *)arg;

    if (id == 1)
    {
        info(BEGIN, 8, id);
        sem_post(semafor3);

        info(END, 8, id);
    }
    else if (id == 4)
    {
        sem_wait(semafor4);
        info(BEGIN, 8, id);
        info(END, 8, id);
    }
    else
    {
        info(BEGIN, 8, id);
        info(END, 8, id);
    }

    return NULL;
}

int main(int argc, char **argv)
{
    init();

    info(BEGIN, 1, 0);
    semafor3 = sem_open("/l08p1_log_semaphore", O_CREAT, 0644, 0);
    semafor4 = sem_open("/l08p1_log_semaphore2", O_CREAT, 0644, 0);
    if (semafor3 == NULL)
    {
        perror("Could not aquire the semaphore");
        return -1;
    }
    if (semafor4 == NULL)
    {
        perror("Could not aquire the semaphore");
        return -1;
    }
    pid_t pid2 = fork();

    // PROCESUL 2
    if (pid2 == -1)
    {
        printf("eroare");
        return -1;
    }
    else if (pid2 == 0)
    {
        info(BEGIN, 2, 0);
        // PROCESUL 5
        pid_t pid5 = fork();
        if (pid5 == -1)
        {
            printf("eroare");
            return -1;
        }
   
        else if (pid5 == 0)
        {
            info(BEGIN, 5, 0);
            if (sem_init(&semafor5, 0, 5) != 0)
                {
                    perror("Could not init the semaphore");
                    return -1;
                }
            // Creare threaduri in procesul 5
            pthread_t tid[45];

            int i = 0;
            int ids[45] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45};
            for (i = 0; i < 45; i++)
            {
                sem_wait(&semafor5);
                pthread_create(&tid[i], NULL, thread_function5, &ids[i]);
            }
            for (i = 0; i < 45; i++)
            {
                pthread_join(tid[i], NULL);
            }
             
            sem_destroy(&semafor5);

            // PROCESUL 6
            pid_t pid6 = fork();
            if (pid6 == -1)
            {
                printf("eroare");
                return -1;
            }
            else if (pid6 == 0)
            {
                info(BEGIN, 6, 0);
                info(END, 6, 0);
                exit(0);
            }
            // PROCESUL 7
            pid_t pid7 = fork();
            if (pid7 == -1)
            {
                printf("eroare");
                return -1;
            }
            else if (pid7 == 0)
            {
                info(BEGIN, 7, 0);
                // Creare threaduri in procesul 7
                pthread_t tid[4];
                TH_STRUCT params[4];
                // sem_t logSem[2];

                int i = 0;
                // int ids[4] = {1, 2, 3, 4};
                // sem_destroy(&logSem[0]);
                // sem_destroy(&logSem[1]);
                if (sem_init(&semafor1, 0, 0) != 0)
                {
                    perror("Could not init the semaphore");
                    return -1;
                }
                if (sem_init(&semafor2, 0, 0) != 0)
                {
                    perror("Could not init the semaphore");
                    return -1;
                }

                for (i = 0; i < 4; i++)
                {
                    params[i].value = i + 1;
                    // params[i].logSem[0] = logSem[0];
                    // params[i].logSem[1] = logSem[1];

                    pthread_create(&tid[i], NULL, thread_function7, &params[i]);
                }
                for (i = 0; i < 4; i++)
                {
                    pthread_join(tid[i], NULL);
                }

                info(END, 7, 0);
                exit(0);
                sem_destroy(&semafor1);
                sem_destroy(&semafor2);
            }

            waitpid(pid7, NULL, 0);
            waitpid(pid6, NULL, 0);
            info(END, 5, 0);
            exit(0);
        }

        waitpid(pid5, NULL, 0);
        info(END, 2, 0);
        exit(0);
    }

    // PROCESUL 3
    pid_t pid3 = fork();
    if (pid3 == -1)
    {
        printf("eroare");
        return -1;
    }

    else if (pid3 == 0)
    {
        info(BEGIN, 3, 0);
        // PROCESUL 4
        pid_t pid4 = fork();
        if (pid4 == -1)
        {
            printf("eroare");
            return -1;
        }
        else if (pid4 == 0)
        {
            info(BEGIN, 4, 0);
            info(END, 4, 0);
            exit(0);
        }
        // PROCESUL 9
        pid_t pid9 = fork();
        if (pid9 == -1)
        {
            printf("eroare");
            return -1;
        }
        else if (pid9 == 0)
        {
            info(BEGIN, 9, 0);
            info(END, 9, 0);
            exit(0);
        }

        waitpid(pid4, NULL, 0);
        waitpid(pid9, NULL, 0);
        info(END, 3, 0);
        exit(0);
    }

    // PROCESUL 8
    pid_t pid8 = fork();
    if (pid8 == -1)
    {
        printf("eroare");
        return -1;
    }
    else if (pid8 == 0)
    {
        info(BEGIN, 8, 0);
        // Creare threaduri in procesul 8
        pthread_t tid[4];

        int i = 0;
        int ids[4] = {1, 2, 3, 4};
        for (i = 0; i < 4; i++)
        {

            pthread_create(&tid[i], NULL, thread_function8, &ids[i]);
        }
        for (i = 0; i < 4; i++)
        {
            pthread_join(tid[i], NULL);
        }
        info(END, 8, 0);
        exit(0);
    }

    waitpid(pid8, NULL, 0);
    waitpid(pid2, NULL, 0);
    waitpid(pid3, NULL, 0);
    info(END, 1, 0);
    exit(0);
    sem_close(semafor3);
    sem_close(semafor4);
    return 0;
}
