#include <stdio.h>
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>

pthread_mutex_t Mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t Cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int play = 0;

void mywait(int timeInSec) {
  struct timespec timeToWait;
  struct timeval now;

  gettimeofday(&now, NULL);

  timeToWait.tv_sec = now.tv_sec + timeInSec;
  timeToWait.tv_nsec = now.tv_usec * 1000;

  pthread_mutex_lock(&Mutex);
  pthread_cond_timedwait(&Cond, &Mutex, &timeToWait);
  pthread_mutex_unlock(&Mutex);
}

/* void *fun(void *arg) { */
/* printf("\nIn thread\n"); */
/* int i = 0; */
/* while (1) { */
/* printf("%d\n", i++); */
/* mywait(1); */
/* } */
/* pthread_exit(NULL); */
/* } */

void *fun(void *arg) {
  int i = 0;
  for (;;) { /* Playback loop */
    pthread_mutex_lock(&lock);
    while (!play) { /* We're paused */
      printf("%d\n", i++);
      mywait(1);
      pthread_cond_wait(&cond, &lock); /* Wait for play signal */
    }
    pthread_mutex_unlock(&lock);
    /* Continue playback */
  }
}

int main() {
  pthread_t thread;

  pthread_create(&thread, NULL, fun, NULL);

  pthread_mutex_lock(&lock);
  play = 1;
  pthread_cond_signal(&cond);
  pthread_mutex_unlock(&lock);

  sleep(8);

  pthread_mutex_lock(&lock);
  play = 0;
  pthread_mutex_unlock(&lock);

  sleep(2);

  pthread_mutex_lock(&lock);
  play = 1;
  pthread_cond_signal(&cond);
  pthread_mutex_unlock(&lock);

  sleep(4);

  pthread_mutex_lock(&lock);
  play = 0;
  pthread_mutex_unlock(&lock);

  pthread_cancel(thread);

  /* Last thing that main() should do */
  pthread_exit(NULL);
}
