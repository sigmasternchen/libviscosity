#ifndef VISCOCITY_H
#define VISCOCITY_H

#include <stdbool.h>
#include <string.h>

void _assert(bool, const char*, const char*, const char*, int, const char*);

#define assertTrue(b, n) _assert(b, #b, n, __FILE__, __LINE__, __FUNCTION__) 
#define assertFalse(b, n) assertTrue(!b, n)
#define assertNull(o, n) assertTrue(o == NULL, n)
#define assertNotNull(o, n) assertTrue(o != NULL, n)
#define assertStringEqual(s1, s2, n) assertTrue(strcmp(s1, s2) == 0, n)

enum callAssertionBehaviour {
	MAX, MIN, EXACT
};

void _assertCall(int, const char*, enum callAssertionBehaviour, const char*, int, const char*);

#define assertCall(i, n, b) _assertCall(i, n, b, __FILE__, __LINE__, __FUNCTION__)
#define assertCallMin(i, n) assertCall(i, n, MIN)
#define assertCallMax(i, n) assertCall(i, n, MAX)
#define assertCallExact(i, n) assertCall(i, n, EXACT)


void addTest(void (*)(), const char*, const char*, const char*, int);

#define test(f, n) void test_##f(); \
__attribute__((constructor)) void register##f() { addTest(& test_##f, n, __FILE__, "test_" # f, __LINE__); }\
void test_##f()

int doTests();

#endif
