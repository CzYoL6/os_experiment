#include<iostream>
#include<fcntl.h>
#include<string>
#include<cstdio>
#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include<unistd.h>

#define MAX_DATASIZE 2048

enum PROCESS_STATUS{
    RUNNING, READY, WAITING, ZOMBIE
};

struct PCB{
    std::string name;
    PROCESS_STATUS status;
    std::string reasonForWaiting;
    int breakpoint;
}pcb;

int semaphore;

int P(int& sem);
int V(int& sem);

void provider();
void consumer();

int main(){
    //in parent process, prepare the shared memory space
    int shmem_fd_pool = shm_open("pool", O_CREAT | O_RDWR,  S_IRWXU);
    int shmem_fd_emptySignal = shm_open("empty", O_CREAT | O_RDWR,  S_IRWXU);
    int shmem_fd_availableSignal = shm_open("available", O_CREAT | O_RDWR,  S_IRWXU);
    
        //O_CREATE:Create the shared memory object if it does not exist.
        //O_RDWR:Open the object for read-write access.
        //S_IRWXU:00700 user (file owner) has read, write, and execute permission
    //creating a mapping to the shared memory.
    int *pool = (int*)mmap(NULL, MAX_DATASIZE, PROT_READ|PROT_WRITE, MAP_SHARED, shmem_fd_pool, 0);
    int *emptySignal = (int*)mmap(NULL, MAX_DATASIZE, PROT_READ|PROT_WRITE, MAP_SHARED, shmem_fd_emptySignal, 0);
    int *availableSignal = (int*)mmap(NULL, MAX_DATASIZE, PROT_READ|PROT_WRITE, MAP_SHARED, shmem_fd_availableSignal, 0);
    
    *emptySignal = 10;
    *availableSignal = 0;

    //creating provider
    pid_t pidProvider = fork();
    if(pidProvider == 0){
        //in provider process
        printf("in provider process.\n");
        pcb.name = "provider";
        pcb.status = RUNNING;
        pcb.reasonForWaiting = "";
        pcb.breakpoint = 0;
        provider();
    }
    else if(pidProvider < 0){
        perror("error creating process.\n");
        return 0;
    }

    //creating consumer
    pid_t pidConsumer = fork();
    if(pidConsumer == 0){
        //in provider process
        printf("in consumer process.\n");
        pcb.name = "consumer";
        pcb.status = RUNNING;
        pcb.reasonForWaiting = "";
        pcb.breakpoint = 0;
        consumer();
    }
    else if(pidConsumer < 0){
        perror("error creating process.\n");
        return 0;
    }

    //in parent process
    printf("in parent process.\n");
}

void provider(){
    while(pcb.status == RUNNING){
        
    }
}

void consumer(){
    while(pcb.status == RUNNING){

    }
}

void p(int *sem){
    *sem--;
    if(*sem < 0){
        //this process wait
        pcb.status = WAITING;
        pcb.reasonForWaiting = "hhhh";
        //pcb.breakpoint = 0;
    }
}

void v(int *sem){
    *sem++;
    if(*sem >= 0){
        //release a process
        pcb.status = RUNNING;
        pcb.reasonForWaiting = "";
        //pcb.breakpoint = 0;
    }
}