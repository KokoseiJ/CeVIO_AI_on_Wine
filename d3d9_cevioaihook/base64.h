#ifndef INCL_BASE64_H
#define INCL_BASE64_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

size_t b64enclen(size_t datalen);

size_t b64declen(char* data);

int b64encode(void* in, size_t inlen, char* out, size_t outlen);

int b64decode(char* in, size_t inlen, void* out, size_t outlen);

#endif
