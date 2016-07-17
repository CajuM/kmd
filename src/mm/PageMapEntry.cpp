/*
 * PageMapEntry.cpp
 *
 *  Created on: Nov 30, 2015
 *      Author: mihai
 */

#include <PageMapEntry.h>
#include <mm.h>

namespace kmd {
namespace mm {

PageMapEntry::PageMapEntry() {
	this->field = nullptr;
	this->parent = nullptr;
	this->index = 0;
}

PageMapEntry::PageMapEntry(u64* parent, u16 index) {
	this->field = parent + index;
	this->parent = parent;
	this->index = index;
}

PageMapEntry::PageMapEntry(u64* field) {
	this->field = field;
	this->parent = (u64 *) (((u64) field >> 9) | 0xFF80000000000000);
	this->index = ((u64) field >> 12) & 0xFFF;
}

PageMapEntry::~PageMapEntry() {
	// TODO Auto-generated destructor stub
}

u8 * PageMapEntry::getAddress() {
	return (u8 *) (*this->field & (-(1 << 12)));
}

u64 PageMapEntry::getField() {
	return *(this->field);
}

void PageMapEntry::setField(u64 field) {
	*(this->field) = field;
}

void PageMapEntry::setOption(u64 option, bool value) {
	if (value)
		*(this->field) |= option;
	else
		*(this->field) &= ~option;
	invalidateTLBEntry(getAddress());
}

bool PageMapEntry::getOption(u64 option) {
	return (*(this->field) & option) == option;
}

void PageMapEntry::setAddress(u8* address) {
	*(this->field) &= (1 << 12) - 1;
	*(this->field) |= (u64) address;
}

} /* namespace mm */
} /* namespace kmd */
