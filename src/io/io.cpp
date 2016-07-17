/*
 * io.cpp
 *
 *  Created on: Nov 28, 2015
 *      Author: mihai
 */

#include <io.h>

namespace kmd {
namespace io {

	u8 in8(u16 port) {
		u8 data;

		asm (
				"inb %%dx, %%al;" :
				"=a"(data) :
				"d"(port) :
		);
		return data;
	}

	u16 in16(u16 port) {
		u16 data;

		asm (
				"inw %%dx, %%ax;" :
				"=a"(data) :
				"d"(port) :
		);
		return data;
	}

	u32 in32(u16 port) {
		u32 data;

		asm (
				"inl %%dx, %%eax;" :
				"=a"(data) :
				"d"(port) :
		);
		return data;
	}


	void out8(u16 port, u8 data) {
		asm (
				"outb %%al, %%dx;" :
				:
				"d"(port), "a"(data) :
		);
	}

	void out16(u16 port, u16 data) {
		asm (
				"outw %%ax, %%dx;" :
				:
				"d"(port), "a"(data) :
		);
	}

	void out32(u16 port, u32 data) {
		asm (
				"outl %%eax, %%dx;" :
				:
				"d"(port), "a"(data) :
		);
	}
}
}
