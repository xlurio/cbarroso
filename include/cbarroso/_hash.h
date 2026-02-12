#ifndef _HASH_H
#define _HASH_H

#include <unistd.h>

typedef unsigned long long int hash_t;

hash_t hashBuffer(const void *buffer, ssize_t len);

#endif
