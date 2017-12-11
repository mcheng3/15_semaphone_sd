#include <stdio.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "mysem.h"

#define KEY 10982

char *get_story(){
    struct stat story_s;
    stat("story", &story_s);
    int fd = open("story", O_RDONLY);
    int size = story_s.st_size;
    char *story = calloc(1, size + 1);
    read(fd, story, size);
    return story;
}

int main( int argc, char *argv[]){
    int fd;
    if(argc < 2){ //invalid number of args
        printf("not enough arguments: 1 arguments required\n");
    }
    else{
        //create semaphore, file, and shared memory and set values
        if(!strcmp(argv[1], "-c") && argc == 2){ 
            //printf("%d\n", semdes);
            int semdes;
            if( semdes = semget(KEY, 1, IPC_EXCL | IPC_CREAT | 0644) != -1){
                int shmid = shmget(KEY, sizeof(int), IPC_CREAT | IPC_EXCL | 0644);
                fd = open("story", O_CREAT | O_TRUNC, 0644);
                //printf("semdes: %d\n", semdes);
                semdes = semget(KEY, 0, 0);
                int val;
                //printf("%d\n", val);
                union semun semopts;
                semopts.val = 1;
                semctl(semdes, 0, SETVAL, semopts);
                close(fd);
                //printf("semaphore created: %d\nvalue set: %d\n", semdes, val);
            }
            else{
                printf("semaphore already exists\n");
            }
        }
        //Remove the shared memory, the semaphore and the story.
        //Display full contents of the story
        else if(!strcmp(argv[1], "-v") && argc == 2){
            printf("Story:%s\n", get_story());
        }
        //Remove the shared memory, the semaphore and the story.
        else if(!strcmp(argv[1], "-r") && argc == 2){ //Remove the semaphore
            printf("Story:%s\n", get_story());
            int semdes = semget(KEY, 0, 0);
            //printf("semdes: %d\n", semdes);
            int val = semctl(semdes, 0, IPC_RMID);
            //printf("semaphore removed: %d\n", val);
            int shmid = shmget(KEY, sizeof(int), 0);;
            shmctl(shmid, IPC_RMID, 0);
            remove("story");
        }
        else{ //invalid arguments
            printf("invalid argument: %s\n", argv[1]);
        }
    }
}
