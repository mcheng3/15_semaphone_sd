#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <fcntl.h>
#include <string.h>

#define SEMKEY 33333
#define SHMKEY 10982

int main(){
  int sem = semget(SEMKEY, 1, 0);
  struct sembuf sb;
  sb.sem_op = -1;
  sb.sem_num = 0;
  sb.sem_flag = SEM_UNDO;
  semop(sem, &sb, 1);

  int shmid = shmget(SHMKEY, 1, 0);
  int *line_size = shmat( shmid, 0, SHM_RDONLY);

  int fd = open("story", O_RDWR | O_APPEND);
  int fs = lseek(fd, line_size, SEEK_END);
  char * buff;
  read(fd, buff, *line_size);

  printf("Last line of story: %s\n\n", buff);

  printf("What's the next line?: ");
  char next_line[1000];
  scanf("%s", *next_line);

  int size = strlen(next_line);
  lseek(fd, 0, SEEK_END);
  write(fd, next_line, size);

  sb.sem_op = 1;
  semop(sem, &sb, 1);

  return 0;
}
	
