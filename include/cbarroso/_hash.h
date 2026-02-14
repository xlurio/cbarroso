#ifndef CBARROSO_HASH_H
#define CBARROSO_HASH_H

#include <unistd.h>

typedef unsigned long long int hash_t;

hash_t hashBuffer(const void *buffer, size_t len);

#endif
