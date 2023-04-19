#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void sigint_handler();

int main() {
    int fd[2];
    int CHILD_NUM =5;
    char msg[100];
    pid_t children[CHILD_NUM];
   

    if (pipe(fd) == -1) {
   	 perror("pipe");
   	 exit(1);
    }

// Fork the child processes
    for (int i = 1; i <= CHILD_NUM; i++) {
   	 pid_t pid = fork();

// Error forking process
   	 if (pid == -1) {
   		 perror("fork");
   		 exit(1);
   	 }

// Child process
    	else if (pid == 0) {
   		 close(fd[1]);

   		 //Read message from pipe that is written from parent
   		 read(fd[0], msg, sizeof(msg));
   		 printf("\n --Child number %d --\n", i);
   	 printf("Message: %s\n", msg);
   		 close(fd[0]); // Close read
   		 exit(0);
   	 }

    else{
// Parent process
   		 children[i] = pid;}}

    signal(SIGINT, sigint_handler ); // Handle interrupt signal

    printf("Parent: Please input a message to children: \n");
    fgets(msg, sizeof(msg), stdin);
    close(fd[0]); //Close read pipe

    // Send message to child processes
    for (int i = 1; i <= CHILD_NUM; i++) {
   	 write(fd[1], msg, sizeof(msg));
    }
    close(fd[1]); // Close write pipe

    // Wait for children to finish
    for (int i = 1; i <= CHILD_NUM; i++) {
   	 waitpid(children[i], NULL, 0);
    }

    printf("All children received the message. Exiting...\n");

    return 0;
}

void sigint_handler() {
    	printf("Program was interrupted. Exiting...\n");
    	exit(0);
}
