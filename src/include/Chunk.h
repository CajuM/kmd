/*
 * Chunk.h
 *
 *  Created on: Dec 5, 2015
 *      Author: mihai
 */

#ifndef SRC_MM_ARENAELEMENT_H_
#define SRC_MM_ARENAELEMENT_H_

#include <arena.h>

#define CHUNK_FREE 1

namespace kmd {
namespace mm {

class Chunk {
public:
	Chunk(u8 * chunk);
	virtual ~Chunk();

	void free();

	u8 * getNext();
	u8 * getPrev();
	u8 * getNextFree();
	u8 * getPrevFree();
	u8 * getSelf();
	u64 getFullSize();
	u64 getSize();
	u64 getFlags();

	void setPrev(u8 * addr);
	void setNext(u8 * addr);
	void setPrevFree(u8 * addr);
	void setNextFree(u8 * addr);
	void setSize(u64 size);
	void setFlags(u64 flags, bool value);
	void setFree();

	bool isFree();

	void merge(Chunk c2);
	void split(u64 newSize);

private:
	u8 * chunk;
};

} /* namespace mm */
} /* namespace kmd */

#endif /* SRC_MM_ARENAELEMENT_H_ */
