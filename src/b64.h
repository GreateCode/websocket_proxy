/*
 * base64.h
 * base64 encode/decode implementation
 *
 */
#ifndef B64_H
#define B64_H
#include <stdlib.h>


char *base64encode(const unsigned char *buf, size_t *size);
unsigned char *base64decode(const char *buf, size_t *size);

#endif
