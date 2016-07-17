/*
 * Arch64.h
 *
 *  Created on: Nov 22, 2015
 *      Author: mihai
 */

#ifndef ARCH64_H_
#define ARCH64_H_

#include <int.h>

namespace kmd {

	namespace arch64 {
		void halt();
		u64 rdmsr(u32 msr);
		void wrmsr(u32 msr, u64 val);
		void int0();
		void init();
	};

} /* namespace kmd */

#endif /* ARCH64_H_ */
