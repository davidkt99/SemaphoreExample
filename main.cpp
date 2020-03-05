#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <list>

using namespace std;

void *process1 (void* dummy);
void *process2 (void* dummy);
void *process3 (void* dummy);

void waitFreeOne();

int n = 10;
sem_t freefull, onefull, twofull, freeedit, oneedit, twoedit, p1, p2, p3;
list<int> freelist(n, 0), list1(0), list2(0);
int shared = 0;

int main () {
	sem_init(&freefull, 0, n);
	sem_init(&onefull, 0, 0);
	sem_init(&twofull, 0, 0);
	sem_init(&freeedit, 0, 1);
	sem_init(&oneedit, 0, 1);
	sem_init(&twoedit, 0, 1);
	sem_init(&p1, 0, 1);
	sem_init(&p2, 0, 0);
	sem_init(&p3, 0, 0);
	pthread_t t1, t2, t3;
	pthread_create(&t1, NULL, process1, NULL);
	pthread_create(&t2, NULL, process2, NULL);
	pthread_create(&t3, NULL, process3, NULL);
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	pthread_join(t3, NULL);
}

void *process1 (void* dummy) {
	int b;
	while (1) {
		//sem_wait(&p1);
		sem_wait(&freefull);
		sem_wait(&freeedit);
		b = freelist.front();
		freelist.pop_front();
		sem_post(&freeedit);

		b = 2;

		sem_wait(&oneedit);
		list1.push_back(b);
		sem_post(&oneedit);
		sem_post(&onefull);
		//sem_post(&p2);
	}
}

void *process2 (void* dummy) {
	int x, y;
	while (1) {
		//sem_wait(&p2);
		waitFreeOne();

		sem_wait(&oneedit);
		x = list1.front();
		list1.pop_front();
		sem_post(&oneedit);

		sem_wait(&freeedit);
		y = freelist.front();
		freelist.pop_front();
		freelist.push_back(x);
		sem_post(&freeedit);
		sem_post(&freefull);

		y = x*5;

		sem_wait(&twoedit);
		list2.push_back(y);
		sem_post(&twoedit);
		sem_post(&twofull);
		//sem_post(&p3);
	}
}

void *process3 (void* dummy) {
	int c;
	while (1) {
		//sem_wait(&p3);
		sem_wait(&twofull);
		sem_wait(&twoedit);
		c = list2.front();
		list2.pop_front();
		sem_post(&twoedit);

		printf("%d\n", c);
		fflush(stdout);

		sem_wait(&freeedit);
		freelist.push_back(c);
		sem_post(&freeedit);
		sem_post(&freefull);
		//sem_post(&p1);
	}
}

void waitFreeOne() {
	int freeval, oneval;
	while (1) {
		sem_wait(&freeedit);
		sem_getvalue(&freefull, &freeval);
		if (freeval < 1) {
			sem_post(&freeedit);
			sem_wait(&freefull);
			continue; //restarts loop
		}

		sem_wait(&oneedit);
		sem_getvalue(&onefull, &oneval);
		if (oneval < 1) {
			sem_post(&freeedit);
			sem_post(&oneedit);
			sem_wait(&onefull);
			continue;
		}
		break;
	}
	sem_getvalue(&freefull, &freeval);
	sem_wait(&freefull);
	sem_wait(&onefull);
	sem_post(&freeedit);
	sem_post(&oneedit);

	printf("out\n");
	fflush(stdout);
}
