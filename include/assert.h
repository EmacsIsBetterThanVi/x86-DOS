#include <stdio.h>
#undef assert
#ifdef NDEBUG
#define assert(ignore) ((void)0)
#else
#define assert(expr) if(expr); else {printf("Assertion on line %d in %s failed: %s\n", __LINE__, __FILE__, #expr); abort();}
#endif
#ifndef ASSERT_H
#define ASSERT_H
#endif