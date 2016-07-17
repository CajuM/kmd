/*
 * PageMap.h
 *
 *  Created on: Nov 22, 2015
 *      Author: mihai
 */

#ifndef SRC_MM_PAGEMAP_H_
#define SRC_MM_PAGEMAP_H_

#include <PageMapEntry.h>
#include <int.h>


namespace kmd {
namespace mm {

class PageMap {
public:
#define PAGE_MAP_FIELDS_NR 512

	virtual ~PageMap();

	PageMapEntry getEntry(u16 index);

	PageMap();
	PageMap(u64 * fields);
	PageMap(PageMapEntry & pme);

	u64 * getFields();
	void clear();

private:
	u64 *fields;
};

} /* namespace mm */
} /* namespace kmd */

#endif /* SRC_MM_PAGEMAP_H_ */
