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


union semun {
  int val; // for SETVAL
  struct semid_ds *buf; // for IPC_STAT and IPC_SET
  unsigned short *array; // SETALL
  struct seminfo *__buf;
};


int printstor(){
    int fd = open("story.txt",O_RDONLY);
    char text[10000];
    int len = read(fd, text, 10000 + 1);
    text[len] = 0;
    int val = 0;
    if (len == -1){
        printf("story does not exist!\n");
    }
    else {
        printf("File Contains:\n%s\n", text);
        val = 1;
    }
    close(fd);
    return val;

}
int main(int argc, char* argv[]){
    
  if (argc != 2){
    printf("Incorrect number of arguments\n");
    return 1;
  }
  if (!strcmp(argv[1],"-c")){
    key_t semkey = 1234;
    key_t shmkey = 12345;
    int semid = semget(semkey, 1, IPC_CREAT | IPC_EXCL | 0777);
    int shmid = shmget(shmkey, 1024, IPC_CREAT | IPC_EXCL | 0777);
      //printf("sem: %p %d\n", semkey, semid);
      //printf("shm: %p %d\n", shmkey, shmid);
      if (semid == - 1 || shmid == -1){
          printf("shm or sem already exists. try control -r or -v\n");
          exit(1);
      }
    int fd = open("story.txt",O_TRUNC|O_CREAT,0777);
    struct sembuf x;
    x.sem_num = 0;
    x.sem_op =  1;
    x.sem_flg = SEM_UNDO;

      union semun set;
      set.val = 1;
      semctl(semid, 0, SETVAL, set);
      printf("DONE: created new shm and sem\n");
  }

  else if (!strcmp(argv[1],"-r")){

      key_t semkey = 1234;
      key_t shmkey = 12345;
      int semid = semget(semkey, 1,0777);
      struct sembuf buffer;
      buffer.sem_num = 0;
      buffer.sem_flg = SEM_UNDO;
      buffer.sem_op = -1;
      semop(semid, &buffer, 1);

      int shmid = shmget(shmkey, 1024, 0777);
    char * data = shmat(shmid, 0, 0);

      if (printstor()) {
          shmdt(data);
          shmctl(shmid, IPC_RMID, NULL);
          semctl(semid, 0, IPC_RMID);
          remove("story.txt");
          buffer.sem_op = 1;
          semop(semid, &buffer, 1);
          printf("DONE: deleted story, sem, and shm\n");
      }
  }

  else if (!strcmp(argv[1],"-v")){

      if(printstor()) {
          printf("DONE: finished printing story\n");
      }
  }
  
  else printf("invalid argument\n");

  return 0;

}
