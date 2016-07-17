/*
 * arena.h
 *
 *  Created on: Nov 27, 2015
 *      Author: mihai
 */

#ifndef SRC_MM_ARENA_H_
#define SRC_MM_ARENA_H_

#include <int.h>

#define PACKED __attribute__((__packed__))

namespace kmd {
namespace mm {

struct PACKED AllocatedChunk {
	u64 size;
	u64 flags;
	u8 * prev;
	u8 * next;
};

struct PACKED FreeChunk {
	u64 size;
	u64 flags;
	u8 * prev;
	u8 * next;

	u8 * prevFree;
	u8 * nextFree;
};

struct PACKED Arena {
	struct FreeChunk * firstFree;
};

}
}

#endif
