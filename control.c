#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>

union semun {
  int val; // for SETVAL
  struct semid_ds *buf; // for IPC_STAT and IPC_SET
  unsigned short *array; // SETALL
  struct seminfo *__buf;
};

int main(int argc, char* argv[]){

  key_t semkey,shmkey;
  int shmid,semid;
  FILE *fp;
    
  if (argc != 2){
    printf("Incorrect number of arguments\n");
    return 1;
  }

  if (!strcmp(argv[1],"-c")){
    semkey = ftok("semkey", 65);
    shmkey = ftok("shmkey", 65);
    semid = semget(semkey, 1, 0666 | IPC_CREAT);
    shmid = shmget(shmkey, 1024, 0666 | IPC_CREAT);
    fp = open(

  }
  



}
