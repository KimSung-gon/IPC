#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MSG_SIZE 80
void* send_message(void* arg);
void* recv_message(void* arg);

int main() {

	int fd[2];
	pthread_t snd_thread, rcv_thread;
	void *thread_result;	

	pthread_create(&snd_thread, NULL, send_message, (void*)fd[0]);	
	pthread_create(&rcv_thread, NULL, recv_message, (void*)fd[1]);	

	pthread_join(snd_thread, &thread_result);
	pthread_join(rcv_thread, &thread_result);

	return 0;
}

void* send_message(void* arg) {
	
	int pipe = (int)arg;
	char msg[MSG_SIZE];
	if((pipe = open("./fifo0", O_WRONLY)) < 0) {
		printf("fail to call fifo0()\n");
		exit(1);
	}
	while(1) {
		printf("input a message : ");
		gets(msg);
		msg[MSG_SIZE - 1] = '\0';
	
		if(msg == "exit")
			exit(1);

		if(write(pipe, msg, MSG_SIZE) == -1) {
			printf("fail to call write()\n");
			exit(1);
		}
		sleep(1);
	}
}

void* recv_message(void* arg) {

	int pipe = (int)arg;
	int nread;
	char msg[MSG_SIZE];
	
	if((pipe = open("./fifo1", O_RDONLY)) < 0) {
		printf("fail to call fifo1()\n");
		exit(1);
	}
	while(1) {	
		if((nread = read(pipe, msg,MSG_SIZE)) < 0) {
			printf("fail to call read()\n");
			exit(1);
		}
		printf("recv: %s\n", msg);
	}
}
