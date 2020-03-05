#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void *process1 ();
void *process2 ();
void *process3 ();

int shared = 0;

int main () {
	pthread_t t1, t2, t3;
	pthread_create(&t1, NULL, process1, NULL);
	pthread_create(&t2, NULL, process2, NULL);
	pthread_create(&t3, NULL, process3, NULL);
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	pthread_join(t3, NULL);
}

void *process1 () {
	printf("Thread 1\n");
	shared++;
	printf("1 Shared = %d\n", shared);
	return NULL;
}

void *process2 () {
	printf("Thread 2\n");
	shared++;
	printf("2 Shared = %d\n", shared);
	return NULL;
}

void *process3 () {
	printf("Thread 3\n");
	shared++;
	printf("3 Shared = %d\n", shared);
	return NULL;
}
