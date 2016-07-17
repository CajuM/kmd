/*
 * PageMap.cpp
 *
 *  Created on: Nov 22, 2015
 *      Author: mihai
 */

#include <PageMap.h>
#include <memory_layout.h>

namespace kmd {
namespace mm {

PageMap::PageMap() {
	this->fields = nullptr;
}

PageMap::PageMap(u64 * fields) {
	this->fields = fields;
}

void PageMap::clear() {
	u16 i;
	for (i = 0; i < 512; i++)
		this->fields[i] = 0;
}

PageMap::PageMap(PageMapEntry & pme) {
	this->fields = (u64 *) pme.getAddress();
}

PageMap::~PageMap() {
	// TODO Auto-generated destructor stub
}

PageMapEntry PageMap::getEntry(u16 index) {
	return PageMapEntry(this->fields, index);
}

u64 * PageMap::getFields() {
	return this->fields;
}

} /* namespace mm */
} /* namespace kmd */
