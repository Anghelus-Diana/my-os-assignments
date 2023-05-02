#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "a2_helper.h"
#include <stdlib.h>

int main(int argc, char **argv)
{
    init();

    info(BEGIN, 1, 0);
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
        pid_t pid5 = fork();
        if (pid5 == -1)
        {
            printf("eroare");
            return -1;
        }
        else if (pid5 == 0)
        {
            info(BEGIN, 5, 0);
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
            pid_t pid7 = fork();
            if (pid7 == -1)
            {
                printf("eroare");
                return -1;
            }
            else if (pid7 == 0)
            {
                info(BEGIN, 7, 0);
                info(END, 7, 0);
                exit(0);
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


    //PROCESUL 8 
    pid_t pid8 = fork();
    if (pid8 == -1)
    {
        printf("eroare");
        return -1;
    }
    else if (pid8 == 0)
    {
        info(BEGIN, 8, 0);
        info(END, 8, 0);
        exit(0);
    }

    waitpid(pid8, NULL, 0);
    waitpid(pid2, NULL, 0);
    waitpid(pid3, NULL, 0);
    info(END, 1, 0);
    exit(0);

    return 0;
}
