#include <pthread.h>
#include "python-locking.h"

static pthread_mutex_t python_lock;

void init_global_lock()
{
  pthread_mutex_init(&python_lock, NULL);  
}

void global_lock()
{
  pthread_mutex_lock(&python_lock);
}

void global_unlock()
{
  pthread_mutex_unlock(&python_lock);
}
