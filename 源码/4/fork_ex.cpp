#include<unistd.h>
#include<stdio.h>
int main()
{
    pid_t pid;
    int n; //count of child processes to create;
    char outputContent;

    scanf("%d", &n);
    for(int i = 0; i < n; i++){
        pid=fork();
        if(pid==-1){
            printf("creation failed. error.\n");
            break;
        }
        else if(pid == 0){
            outputContent = i + 'A' ;
            printf("This is child process (id:%d) (parent process id:%d): %c\n", getpid(), getppid(), outputContent);
            break;
        }
        else {
            printf("This is parent process (id:%d), creating child Process %d\n", getpid(), pid);
        }
    }
    return 0;
}
