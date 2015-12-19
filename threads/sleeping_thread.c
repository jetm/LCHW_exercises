#include <stdio.h>
#include <pthread.h>
#include <sys/time.h>

pthread_mutex_t fakeMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t fakeCond = PTHREAD_COND_INITIALIZER;

void mywait(int timeInSec) {
  struct timespec timeToWait;
  struct timeval now;

  gettimeofday(&now, NULL);

  timeToWait.tv_sec = now.tv_sec + timeInSec;
  timeToWait.tv_nsec = now.tv_usec * 1000;

  pthread_mutex_lock(&fakeMutex);
  pthread_cond_timedwait(&fakeCond, &fakeMutex, &timeToWait);
  pthread_mutex_unlock(&fakeMutex);
  printf("\nDone\n");
}

void* fun(void* arg) {
  printf("\nIn thread\n");
  mywait(5);
}

int main() {
  pthread_t thread;
  void* ret;

  pthread_create(&thread, NULL, fun, NULL);
  pthread_join(thread, &ret);
}
