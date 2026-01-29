#ifndef COMMON_H
#define COMMON_H

#include <sys/syscall.h>
#include <unistd.h>

#ifndef __NR_memfd_create
  #if defined(__x86_64__)
    #define __NR_memfd_create 319
  #elif defined(__aarch64__)
    #define __NR_memfd_create 279
  #elif defined(__i386__)
    #define __NR_memfd_create 356
  #endif
#endif

#endif
