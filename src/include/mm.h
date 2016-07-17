/*
 * mm.h
 *
 *  Created on: Nov 22, 2015
 *      Author: mihai
 */

#ifndef SRC_MM_MM_H_
#define SRC_MM_MM_H_

#include <PageMap.h>
#include <Page.h>
#include <PageMapEntry.h>
#include <memory_layout.h>
#include <arena.h>
#include <Chunk.h>
#include <int.h>
#include <mboot.h>

namespace kmd {
namespace mm {

struct Zone {
	u64 begin;
	u64 end;
	Page * pages;
};

extern u64 * PageMapLevel4;
extern u8 * arena;
extern Zone zones[255];
extern u8 zonesNr;
extern Page * pageTable;

void init(u64 addr);
u64 * allocPageMap();
void buildPageMapHierarchy();
bool mapPage(u8 * dst, u8 * src, u64 flags);
void map(u8 * dst, u8 * src, u64 len, u64 flags);
void pagesShadow(u64 index, u64 nrPages);
bool pagesFree(u64 index, u64 nrPages);
void mapFirst(u8 *dst, u64 len, u64 flags);
Page * getFreePage();
Page * getPage(u8 * base);
void crawlPagingTree(PageMap root, u8 level);
PageMapEntry findPME(u8 * address, PageMap root, u8 level);
void initArena();
void parseMmap(struct multiboot_tag_mmap * mmap);
void flushTLB();
void invalidateTLBEntry(u8 * addr);
PageMapEntry getPME(PageMap root, u8 * addr);
void initZones(multiboot_tag_mmap * mmap);
u8 * smalloc(u64 size, u64 alignement);
u8 * kalloc(u64 size, u64 alignement, u64 flags);
u8 * salloc(u8 * src, u64 size, u64 alignement, u64 flags);
}
}

#endif /* SRC_MM_MM_H_ */
