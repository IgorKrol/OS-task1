#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/uio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <sys/fcntl.h>
#include "md5.h"
#include <sys/wait.h>


pid_t pid;
char buff[20];
bool flag = 0;  //for signal handler
int chlidpid = 0;
std::string msgrevered;
char* ans;

void catchSignal(int sig_num)
{
    if(strlen(msgrevered.c_str()) == 32){
        flag = 1;
    }
};


int main(int argc,char *argv[])
{

    int _pipe[2];
    int _pipeErrorCheck = pipe(_pipe);
    char str1[20]="";

    signal(SIGTERM, catchSignal);

    //pipe error check
    if(_pipeErrorCheck == -1){
        perror("pipes fail");
        exit(1);
    }

    pid = fork();

    if (pid == 0){
        /* Chlid Process */
        read(_pipe[0], buff, 20);
        sleep(1);    

        msgrevered = md5(buff);
        char answer[msgrevered.length() + 1];
        strcpy(answer, msgrevered.c_str());
        write(_pipe[1],answer,strlen(answer) + 1);
        kill(pid,SIGTERM);
        sleep(3);
    }
    else{
        /* Parent Process */
        for(int i=1;i<argc;i++){  
            char temp[20]="";
            strncpy(temp, argv[i], 20);
            strcat(temp," ");
            strcat(str1,temp);
        }
            printf("plain text: %s\n",str1);   
            write(_pipe[1], str1,20);
            sleep(5);
        read(_pipe[0], buff, 32);
        sleep(5);
        if(flag == 1){
            printf("encrypted by process: %d : %s\n ",getpid(),buff);
            sleep(1);

        }

    }
}

