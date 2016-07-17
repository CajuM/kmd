/*
 * Reg.cpp
 *
 *  Created on: Jan 30, 2016
 *      Author: mihai
 */

#include <Reg.h>
#include <cstring>

namespace kmd {

Reg::Reg(const char * name) {
	this->name = name;
}

Reg::~Reg() {
	// TODO Auto-generated destructor stub
}

template<> void Reg::set<u64>(u64 val) {
	if (strcmp(this->name, "cr3") == 0) {
		asm volatile(
				"movq %%rax, %%cr3" ::
				"a"(val) :
		);
	}
}

template<> u64 Reg::get<u64>() {
	u64 ret;
	if (strcmp(this->name, "cr3") == 0) {
		asm volatile(
				"movq %%cr3, %%rax" :
				"=a"(ret) ::
		);
	}
	return ret;
}

} /* namespace kmd */
