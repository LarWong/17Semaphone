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

/*
union semun {
  int val; // for SETVAL
  struct semid_ds *buf; // for IPC_STAT and IPC_SET
  unsigned short *array; // SETALL
  struct seminfo *__buf;
};
*/

int main(int argc, char* argv[]){
    
  if (argc != 2){
    printf("Incorrect number of arguments\n");
    return 1;
  }

  if (!strcmp(argv[1],"-c")){
    key_t semkey = ftok("./", 65);
    key_t shmkey = ftok("./", 65);
    int semid = semget(semkey, 1, IPC_CREAT | IPC_EXCL | 0644);
    int shmid = shmget(shmkey, 1024, IPC_CREAT | IPC_EXCL | 0644);
    int fd = open("story.txt",O_TRUNC|O_CREAT,0666);

    struct sembuf x;
    x.sem_num = 0;
    x.sem_op =  1;
    x.sem_flg = SEM_UNDO;

    semop(semid, &x, 1);
    
  }

  else if (!strcmp(argv[1],"-r")){
    key_t semkey = ftok("./",65);
    key_t shmkey = ftok("./",65);
    int semid = semget(semkey, 1,0644);
    int shmid = shmget(shmkey, 1024, 0644);
    char * data = shmat(shmkey, 0, 0);
    
    int fd = open("story.txt",O_RDONLY);

    char text[10000];
    read(fd, text, 10000);
    printf("File Contains:  %s\n", text);

    shmdt(data);
    shmctl(shmid,IPC_RMID,NULL);
    semctl(semid,0,IPC_RMID);

    close(fd);
    remove("story.txt");


  }

  else if (!strcmp(argv[1],"-v")){

    int fd = open("story.txt",O_RDONLY);
    char text[10000];
    read(fd, text, 10000);
    printf("File Contains:  %s\n", text);

  }
  
  else printf("invalid argument\n");

  return 0;

}
