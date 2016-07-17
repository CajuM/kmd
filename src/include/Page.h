/*
 * Page.h
 *
 *  Created on: Nov 22, 2015
 *      Author: mihai
 */

#ifndef SRC_INCLUDE_PAGE_H_
#define SRC_INCLUDE_PAGE_H_

#include <int.h>
#include <PageMap.h>

namespace kmd {
namespace mm {

class Page {
public:
	static const u64 flagP = 1 << 0;
	static const u64 flagRW = 1 << 1;
	static const u64 flagUS = 1 << 2;
	static const u64 flagPWT = 1 << 3;
	static const u64 flagPCD = 1 << 4;
	static const u64 flagA = 1 << 5;
	static const u64 flagD = 1 << 6;
	static const u64 flagPS = 1 << 7;
	static const u64 flagG = 1 << 8;
	static const u64 flagPAT = 1 << 12;
	static const u64 flagXD = 1ul << 63;


	Page();
	Page(u8 * base);

	virtual ~Page();

	u8 * getBase();
	u64 getReferences();

	bool addrIn(u64 * addr, u64 len);

	void incReferences();
	void decReferences();

	bool isFree();

	void setAvailable(bool available);

private:
	u8 * base;
	u64 references;
	bool available;
};

} /* namespace mm */
} /* namespace kmd */

#endif /* SRC_INCLUDE_PAGE_H_ */
