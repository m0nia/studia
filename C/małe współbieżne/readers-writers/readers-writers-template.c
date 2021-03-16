#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "err.h"

#define READERS 3
#define WRITERS 2
#define NAP 2
#define BSIZE 32

struct readwrite {
  pthread_mutex_t lock;         
  pthread_cond_t readers;      
  pthread_cond_t writers;    
  int rcount, wcount, rwait, wwait;
  int change;
};


/* Initialize a buffer */

void init(struct readwrite *rw) {
  int err;
  
  if ((err = pthread_mutex_init(&rw->lock, 0)) != 0)
    syserr (err, "mutex init failed");
  if ((err = pthread_cond_init(&rw->readers, 0)) != 0)
    syserr (err, "cond 'readers' init failed");
  if ((err = pthread_cond_init(&rw->writers, 0)) != 0)
    syserr (err, "cond 'writers' init 2 failed");
  rw->rcount = 0;
  rw->wcount = 0;
  rw->rwait = 0;
  rw->wwait = 0;
  rw->change = 0;  // 0 kolej pisarzy, 1 kolej czytelników
}

/* Destroy the buffer */

void destroy(struct readwrite *rw) {
  int err;

  if ((err = pthread_cond_destroy (&rw->readers)) != 0)
    syserr (err, "cond 'readers' destroy failed");
  if ((err = pthread_cond_destroy (&rw->writers)) != 0)
    syserr (err, "cond 'writers' destroy failed");
  if ((err = pthread_mutex_destroy (&rw->lock)) != 0)
    syserr (err, "mutex destroy failed");
}

void P(pthread_mutex_t *m){
  int err;
  if ((err = pthread_mutex_lock(m)) != 0)
    syserr (err, "lock failed");
}

void V(pthread_mutex_t *m){
  int err;
  if ((err = pthread_mutex_unlock(m)) != 0)
    syserr (err, "unlock failed");
}

void wait(pthread_cond_t* cond, pthread_mutex_t* m){
  int err;
  if ((err = pthread_cond_wait(cond, m)) != 0)
    syserr (err, "cond wait failed");
}

void signal(pthread_cond_t* cond){
  int err;
  if ((err = pthread_cond_signal(cond)) != 0)
    syserr (err, "cond signal failed");
}
 
void ChceCzytac(struct readwrite* library){
  P(&library->lock);
  library->rwait += 1;
  while ((library->wcount > 0 || library->wwait > 0) && library->change == 0){
    wait(&library->readers, &library->lock);
  }
  library->change = 0;
  library->rwait -= 1;  
  library->rcount += 1;
  if(library->rwait > 0){
    library->change = 1;
  	signal(&library->readers);
  }
        
  V(&library->lock);
}

void ChcePisac(struct readwrite* library){
  P(&library->lock);
  library->wwait += 1;
  while ((library->wcount > 0 || library->rcount > 0) && library->change == 0){
    wait(&library->writers, &library->lock);
  }
  library->change = 0;
  library->wwait -= 1;
  library->wcount += 1;
  V(&library->lock);
}

void KoniecCzytania(struct readwrite* library){
  P(&library->lock);
  library->rcount -= 1;
  if(library->rcount == 0){
      library->change = 1;
  	  signal(&library->writers);
  }
  V(&library->lock);
}

void KoniecPisania(struct readwrite* library){
  P(&library->lock);
  library->wcount -= 1;
  if(library->rwait > 0){
      library->change = 1;
  	  signal(&library->readers);
  } else if(library->wwait > 0) {
      library->change = 1;
      signal(&library->writers);
  }
  V(&library->lock);     
}

struct readwrite library;
char book[BSIZE];
int working = 1;

void *reader(void *data)
{
   int i = 0;
   while (working) {
	 ChceCzytac(&library);
     printf("reader read: %s\n", book); /* reading */
     KoniecCzytania(&library);
     i++;
   }
   printf("Czytelnicy byli w czytelni %d razy\n", i);
   return 0;
}

void *writer(void *data)
{  
   int l;
   int i = 0;
   while (working) {

     l = rand()%10;
     ChcePisac(&library);
     snprintf(book, BSIZE, "6 times a number %d %d %d %d %d %d", l, l, l, l, l, l);
     KoniecPisania(&library);
	 i++;
   }
   printf("Pisarze byli w czytelni %d razy\n", i);
   return 0;
}


int main() {
  pthread_t th[READERS+WRITERS];
  pthread_attr_t attr;
  int i, err;
  void *retval;

  srand((unsigned)time(0));
  
  init(&library);
  if ((err = pthread_attr_init (&attr)) != 0)
    syserr (err, "attr_init failed");
  if ((err = pthread_attr_setdetachstate (&attr,PTHREAD_CREATE_JOINABLE)) != 0)
    syserr (err, "attr_setdetachstate failed");

  for (i = 0; i < READERS + WRITERS; i++) {
    if (i < READERS) {
      if ((err = pthread_create(&th[i], &attr, reader, 0)) != 0)
	syserr (err, "create failed");
    } else 
      if ((err = pthread_create(&th[i], &attr, writer, 0)) != 0)
	syserr (err, "create failed");
  }
  
  sleep(NAP);
  working = 0;

  for (i = 0; i < READERS + WRITERS; i++) {
    if ((err = pthread_join(th[i], &retval)) != 0)
    syserr (err, "join failed");
  }
  
  if ((err = pthread_attr_destroy (&attr)) != 0)
    syserr (err, "attr_destroy failed");
  destroy(&library);
  return 0;
}
