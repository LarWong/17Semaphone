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
    key_t semkey = 1234;
    key_t shmkey = 12345;
    int semid = semget(semkey, 1, 0);
    if (semid == -1){
        printf("semaphore does not exist yet! try creating a story?: %s\n", strerror(errno));
        exit(1);
    }
    struct sembuf buffer;
    buffer.sem_num = 0;
    buffer.sem_flg = SEM_UNDO;
    buffer.sem_op = -1;
    semop(semid, &buffer, 1);
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
        printf("previous line:\n%s\n", attached);
    }
    else {
        printf("no previous line\n");
    }
    printf("add a line to the story:\n");
    char nxt[1000];
    fgets(nxt, 1000, stdin);
    strcat(nxt,"\0");
    strcpy(attached, nxt);
    int desc = open("story.txt", O_WRONLY|O_APPEND);
    if (desc == -1){
        printf("could not open story! try creating a story?: %s\n", strerror(errno));
        exit(1);
    }
    write(desc, nxt, strlen(nxt));
    buffer.sem_op = 1;
    semop(semid, &buffer, 1);
    return 0;

}