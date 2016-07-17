/*
 * PageMapEntry.h
 *
 *  Created on: Nov 30, 2015
 *      Author: mihai
 */

#ifndef SRC_MM_PAGEMAPENTRY_H_
#define SRC_MM_PAGEMAPENTRY_H_

#include <int.h>

namespace kmd {
namespace mm {

class PageMapEntry {
public:
#define PME_PRESENT (1ul << 0)
#define PME_RW (1ul << 1)
#define PME_PAT (1ull << 7)
#define PME_PCD (1ull << 4)
#define PME_PWT (1ull << 3)

	PageMapEntry();
	PageMapEntry(u64 * parent, u16 index);
	PageMapEntry(u64 * field);
	~PageMapEntry();

	u8 * getAddress();
	u64 getField();
	bool getOption(u64 option);

	void setField(u64 field);
	void setOption(u64 option, bool value);
	void setAddress(u8 * address);

private:
	u64 * parent;
	u16 index;
	u64 *field;
};

} /* namespace mm */
} /* namespace kmd */

#endif /* SRC_MM_PAGEMAPENTRY_H_ */
