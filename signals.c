#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/uio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <sys/fcntl.h>
#include <sys/wait.h>
#include <stdbool.h>

int j;
int _children[5];


void signalCatcher(int sig_num)
{
	signal(SIGTERM,signalCatcher);
	printf("PID %d is caught one\n",getpid());
	if(j >-1){
		kill(_children[j],SIGTERM);
	}
}

int main() 
{ 
	pid_t pid;
	int zombie;
	int state;

	signal(SIGTERM, signalCatcher);

	for (int i = 0; i < 5; i++)
	{
		pid = fork();
		if(pid == 0){		//chlid Process
			printf("PID %d is ready \n", getpid());
			j = i-1;
	pause(); // wait for singal
	exit(0); // become a zombie
		}
	// Parent process collects child process.
	_children[i] = pid;
	}
	sleep(1);
	kill(_children[4],SIGTERM);
	sleep(1);
	
	for (size_t i = 0; i < 5; i++)
	{
		zombie = wait(&state);
		printf("process %d is dead\n",zombie);
		kill(zombie,SIGKILL);
	}


return 0;
} 



