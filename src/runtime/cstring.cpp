/*
 * cstring.cpp
 *
 *  Created on: Jan 30, 2016
 *      Author: mihai
 */

#include <int.h>
#include <cstring>

extern "C" {

int strcmp(const char * str1, const char * str2) {
	u64 i;
	for (i = 0; *(str1 + i) && *(str2 + i); i++) {
		if (*(str1 + i) > *(str2 + i))
			return 1;
		if (*(str1 + i) < *(str2 + i))
			return -1;
	}

	if (*(str1 + i))
		return 1;
	else if (*(str2 + i))
		return -1;
	return 0;
}
}
