#ifndef PRINT_H
#define PRINT_H

#include <stdarg.h>
#include <stddef.h>

void kprint(char *Dest, size_t N, const char *Fmt, ...);

#endif // PRINT_H