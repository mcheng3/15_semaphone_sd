#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>

#define KEY 10982

int main(){
	int shmid = (KEY, 0, 0);
	int *offset = shmat( shmid, 0, SHM_RDONLY);
	
	int fd = open("story", O_RDWR | O_APPEND);
	int fs = lseek(fd, offset, SEEK_END);
	char * buff;
	read(fd, buff, *offset);
}
	
