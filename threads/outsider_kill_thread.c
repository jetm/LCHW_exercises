#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>

/* Returns 1 (true) if the mutex is unlocked, which is the
 * thread's signal to terminate.
 */
int needQuit(pthread_mutex_t *mtx) {
  switch (pthread_mutex_trylock(mtx)) {
    case 0: /* if we got the lock, unlock and return 1 (true) */
      pthread_mutex_unlock(mtx);
      return 1;
    case EBUSY: /* return 0 (false) if the mutex was locked */
      return 0;
  }
  return 1;
}

/* Thread function, containing a loop that's infinite except that it checks for
 * termination with needQuit()
 */
void *thread_do(void *arg) {
  pthread_mutex_t *mx = arg;
  int i = 0;
  while (!needQuit(mx)) {
    printf("%d\n", ++i);
    sleep(1);
  }
  return NULL;
}

int main(int argc, char *argv[]) {
  pthread_t th;
  pthread_mutex_t mxq; /* mutex used as quit flag */

  /* init and lock the mutex before creating the thread.  As long as the
   * mutex stays locked, the thread should keep running.  A pointer to the
   * mutex is passed as the argument to the thread function.
   */
  pthread_mutex_init(&mxq, NULL);
  pthread_mutex_lock(&mxq);
  pthread_create(&th, NULL, thread_do, &mxq);

  sleep(2);

  /* unlock mxq to tell the thread to terminate, then join the thread */
  pthread_mutex_unlock(&mxq);
  pthread_join(th, NULL);

  sleep(2);
  return 0;
}
