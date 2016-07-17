/*
 * io.h
 *
 *  Created on: Nov 28, 2015
 *      Author: mihai
 */

#ifndef SRC_INCLUDE_IO_H_
#define SRC_INCLUDE_IO_H_

#include <int.h>

namespace kmd {
namespace io {

u8 in8(u16 port);
u16 in16(u16 port);
u32 in32(u16 port);

void out8(u16 port, u8 data);
void out16(u16 port, u16 data);
void out32(u16 port, u32 data);

}
}

#endif /* SRC_INCLUDE_IO_H_ */
