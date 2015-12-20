#include <stdio.h>
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>

pthread_mutex_t Mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t Cond = PTHREAD_COND_INITIALIZER;

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

void* fun(void* arg) {
  printf("\nIn thread\n");
  int i = 0;
  while (1) {
    printf("%d\n", i++);
    mywait(1);
  }
  pthread_exit(NULL);
}

int main() {
  pthread_t thread;

  pthread_create(&thread, NULL, fun, NULL);

  sleep(8);
  pthread_cancel(thread);

  /* Last thing that main() should do */
  pthread_exit(NULL);
}
