/*
 * Chunk.cpp
 *
 *  Created on: Dec 5, 2015
 *      Author: mihai
 */

#include <Chunk.h>

namespace kmd {
namespace mm {

Chunk::Chunk(u8 * chunk) {
	this->chunk = chunk;
}

Chunk::~Chunk() {
	// TODO Auto-generated destructor stub
}

void Chunk::free() {
	if (isFree())
		return;
	setFree();
	if (Chunk(getNext()).isFree())
		merge(Chunk(getNext()));
	if (Chunk(getPrev()).isFree())
		Chunk(getPrev()).merge(*this);
}

u64 Chunk::getFlags() {
	return ((AllocatedChunk *) this->chunk)->flags;
}

u64 Chunk::getSize() {
	return ((AllocatedChunk *) this->chunk)->size;
}

bool Chunk::isFree() {
	return getFlags() & CHUNK_FREE;
}

u8 * Chunk::getNext() {
	return (u8 *) ((AllocatedChunk *) this->chunk)->next;
}

u8 * Chunk::getPrev() {
	return (u8 *) ((AllocatedChunk *) this->chunk)->prev;
}

u8 * Chunk::getNextFree() {
	if (!isFree()) return nullptr;
	return (u8 *) ((FreeChunk *) this->chunk)->nextFree;
}

u8 * Chunk::getPrevFree() {
	if (!isFree()) return nullptr;
	return (u8 *) ((FreeChunk *) this->chunk)->prevFree;
}

u8 * Chunk::getSelf() {
	return this->chunk;
}

void Chunk::setPrev(u8 * addr) {
	((AllocatedChunk *) this->chunk)->prev = addr;
}

void Chunk::setNext(u8 * addr) {
	((AllocatedChunk *) this->chunk)->next = addr;
}

void Chunk::setPrevFree(u8 * addr) {
	if (! isFree()) return;
	((FreeChunk *) this->chunk)->prevFree = addr;
}

void Chunk::setNextFree(u8 * addr) {
	if (! isFree()) return;
	((FreeChunk *) this->chunk)->nextFree = addr;
}

void Chunk::setSize(u64 size) {
	((AllocatedChunk *) this->chunk)->size = size;
}

void Chunk::setFlags(u64 flags, bool value) {
	u64 nFlags = ((AllocatedChunk *) this->chunk)->flags;
	if (value) nFlags |= flags;
	else nFlags &= ~flags;
	((AllocatedChunk *) this->chunk)->flags = nFlags;
}

void Chunk::setFree() {
	setFlags(CHUNK_FREE, true);
}

u64 Chunk::getFullSize() {
	if (isFree())
		return getSize() + sizeof(struct FreeChunk);
	else return getSize() + sizeof(struct AllocatedChunk);
}

void Chunk::merge(Chunk c2) {
	Chunk c1 = *this;
	Chunk c3 = c2.getNext();

	c3.setPrev(c1.getSelf());
	c1.setNext(c3.getSelf());

	c1.setSize(getSize() + c2.getFullSize());
}

void Chunk::split(u64 newSize) {
	if (! isFree())
		return;

	if (newSize > getSize() - sizeof(struct FreeChunk))
		return;

	Chunk c1 = *this;
	Chunk c3 = c1.getNext();

	c1.setSize(newSize);
	Chunk c2 = Chunk(c1.getSelf() + sizeof(struct FreeChunk) + newSize);
	c2.setSize(c1.getSize() - newSize - sizeof(struct FreeChunk));

	c1.setNext(c2.getSelf());
	c2.setPrev(c1.getSelf());
	c2.setNext(c3.getSelf());
	c3.setPrev(c2.getSelf());
}

} /* namespace mm */
} /* namespace kmd */
