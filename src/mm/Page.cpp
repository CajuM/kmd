/*
 * Page.cpp
 *
 *  Created on: Nov 22, 2015
 *      Author: mihai
 */

#include <Page.h>
#include <mm.h>

namespace kmd {
namespace mm {

Page::Page() {
	this->base = nullptr;
	this->references = 0;
	this->available = true;
}

Page::Page(u8 * base) {
	this->base = base;
	this->references = 0;
	this->available = true;
}

Page::~Page() {
	// TODO Auto-generated destructor stub
}

u8 * Page::getBase() {
	return this->base;
}

void Page::incReferences() {
	this->references++;
}

void Page::decReferences() {
	this->references--;
}

bool Page::isFree() {
	return this->references <= 1 && this->available;
}

u64 Page::getReferences() {
	return this->references;
}

void Page::setAvailable(bool available) {
	this->available = available;
}

} /* namespace mm */
} /* namespace kmd */
