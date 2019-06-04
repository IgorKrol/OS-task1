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


#define MSGSIZE 20 
  
int main() 
{ 
    char inbuf[MSGSIZE]; 
    int p1[2], p2[2], pid, nbytes; 
  	
  	/* pipes creation check */
    if (pipe(p1) < 0) 
        exit(-1); 
    if (pipe(p2) < 0) 
        exit(-1); 

    /* Parent */
    if ((pid = fork()) > 0) {

    	scanf("%s",inbuf);	//get input string from user length<=20 
        close(p1[0]);
        write(p1[1], inbuf, MSGSIZE);	//send string via pipe
        close(p1[1]);

        //wait for child to send string
        wait(NULL);

        close(p2[1]);
        char* res;
        //read MD5 result from child
        read(p2[0], res, 33);
        //print result
        printf("MD5::%s",res);
    } 
  	/* Child */
    else {
    	char userMsg[MSGSIZE];
		close(p1[1]);
    	read(p1[0],userMsg,MSGSIZE);
    	//create MD5 from userInput
    	char* result;
        MD5(userMsg);
    	

    	close(p1[0]);
    	close(p2[0]);
    	//write MD5 result to Parent
    	write(p2[1], result, strlen(result));
    	close(p2[1]); 
    } 
    return 0; 
} 
