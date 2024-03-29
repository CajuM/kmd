/*
 * memory_layout.h
 *
 *  Created on: Nov 23, 2015
 *      Author: mihai
 */

#ifndef SRC_INCLUDE_MEMORY_LAYOUT_H_
#define SRC_INCLUDE_MEMORY_LAYOUT_H_

#define KERNEL_PHY_ADDR 0x100000
#define KERNEL_LINEAR_ADDR 0xFFFFC00000000000

#define MM_HIGH_BEGIN 0xFFFF800000000000
#define MM_HIGH_END   0xFFFFFFFFFFFFFFFF

#define MM_LOW_BEGIN 0x0000000000000000
#define MM_LOW_END   0x00007FFFFFFFFFFF

#define MM_MAX_SIZE (1ul << 48)

#define PAGE_SIZE (1ul << 12)
#define PAGE_NR (MM_MAX_SIZE / PAGE_SIZE)

#define PAGE_MAPS_NR (1ul + 512ul + 512ul*512ul + 512ul*512ul*512ul)
#define PAGE_MAP_ENTRIES_NR (PAGE_MAPS_NR * 512ul)
#define PAGE_MAP_ENTRIES_HIERARCHY_SIZE ((PAGE_MAP_ENTRIES_NR * 8ul *8ul) >> 21)

#endif /* SRC_INCLUDE_MEMORY_LAYOUT_H_ */
