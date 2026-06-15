#include "common.h"

void eprintf(const char *msg, ...)
{
    va_list ap;

    va_start(ap, msg);

    vfprintf(stderr, msg, ap);

    va_end(ap);
}
