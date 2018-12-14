#include <stdlib.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <string.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>


/*
union semun {
  int val; // for SETVAL
  struct semid_ds *buf; // for IPC_STAT and IPC_SET
  unsigned short *array; // SETALL
  struct seminfo *__buf;
};
*/

int main () {
    key_t semkey = 12345;
    int semid = semget(semkey, 1, 0);
    if (semid == -1){
        printf("semaphore does not exist yet! try creating a story?: %s\n", strerror(errno));
        exit(1);
    }
    struct sembuf buffer = {0, -1, SEM_UNDO};
    semop(semid, &buffer, 1);
    key_t shmkey = 12345;
    int shmid = shmget(shmkey, 1024, 0);
    if (shmid == -1){
        printf("could not grab shared memory! try creating a story?: %s\n", strerror(errno));
        exit(1);
    }
    char *attached = shmat(shmid, 0, 0);
    if (*attached == -1){
        printf("could not attach: %s\n", strerror(errno));
        exit(1);
    }
    else if (*attached){
        printf("%s\n", attached);
        //print the shm data
        //printf()
    }
    else {
        printf("no previous line\n");
    }

}