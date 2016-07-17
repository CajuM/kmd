/*
 * libcxx.h
 *
 *  Created on: Jan 30, 2016
 *      Author: mihai
 */

#ifndef SRC_INCLUDE_LIBCXX_H_
#define SRC_INCLUDE_LIBCXX_H_

#include <int.h>

extern "C" {

/* begin copy cat, from: http://wiki.osdev.org/C%2B%2B */

#define ATEXIT_MAX_FUNCS	128

struct atexit_func_entry_t {
	/*
	 * Each member is at least 4 bytes large. Such that each entry is 12bytes.
	 * 128 * 12 = 1.5KB exact.
	 **/
	void (*destructor_func)(void *);
	void *obj_ptr;
	void *dso_handle;
};

typedef unsigned uarch_t;

int __cxa_atexit(void (*f)(void *), void *objptr, void *dso);

typedef int (*cmpfun)(const void *, const void *);
typedef u64 size_t;
void qsort(void *base, size_t nel, size_t width, cmpfun cmp);
}

#endif /* SRC_INCLUDE_LIBCXX_H_ */
