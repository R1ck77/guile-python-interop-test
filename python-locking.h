#ifndef PYTHON_LOCKING_H
#define PYTHON_LOCKING_H

#define WITH_PYTHON_LOCK(code)       \
  global_lock();                     \
  {                                  \
     code;                           \
  }                                  \
  global_unlock();

void init_global_lock();

void global_lock();

void global_unlock();

#endif
