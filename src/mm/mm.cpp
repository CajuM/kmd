/*
 * mm.cpp
 *
 *  Created on: Nov 22, 2015
 *      Author: mihai
 */

#include <mm.h>
#include <Reg.h>
#include <libcxx.h>

#define SECTION(NAME) __attribute__((section(#NAME)))

namespace kmd {

namespace mm {

extern "C" u64 * tmp_pml4t;

u64 * PageMapLevel4;
Zone zones[255];
u8 zonesNr;
u8 * arena;
Page * pageTable;

void init(u64 addr) {
	PageMapLevel4 = tmp_pml4t;
	struct multiboot_tag_basic_meminfo * mbi_basic_meminfo =
			(struct multiboot_tag_basic_meminfo *) getMBITag(addr,
			MULTIBOOT_TAG_TYPE_BASIC_MEMINFO);
	struct multiboot_tag_mmap * mbi_mmap =
			(struct multiboot_tag_mmap *) getMBITag(addr,
			MULTIBOOT_TAG_TYPE_MMAP);
	parseMmap(mbi_mmap);
	crawlPagingTree(PageMap(PageMapLevel4), 4);
	flushTLB();
	initArena();
}

u8 * smalloc(u64 size, u64 alignement) {
	u64 i = (u64) ((Arena *) arena)->firstFree;
	while ((u8 *) i != nullptr) {
		Chunk c = Chunk((u8 *) i);
		u64 aa;
		if (i % alignement == 0) {
			aa = i;
		} else {
			aa = i / alignement;
			aa += 1;
			aa *= alignement;
		}
		if (aa + size + sizeof(FreeChunk) <= i + c.getFullSize()) {
			c.split(aa + size - i);
			return (u8 *) aa;
		}
		i = (u64) c.getNextFree();
	}
	return nullptr;
}

u8 * salloc(u8 * src, u64 size, u64 alignement,
		u64 flags = PME_RW | PME_PRESENT) {
	u64 ret = (u64) smalloc(size, alignement);
	if ((u8 *) ret == nullptr)
		return (u8 *) ret;
	u64 nret = (ret >> 12) << 12;
	map((u8 *) nret, src, ret - nret + size, flags);
	return (u8 *) ret;
}

u8 * kalloc(u64 size, u64 alignement, u64 flags = PME_RW | PME_PRESENT) {
	u64 ret = (u64) smalloc(size, alignement);
	if ((u8 *) ret == nullptr)
		return (u8 *) ret;
	u64 nret = (ret >> 12) << 12;
	mapFirst((u8 *) nret, ret - nret + size, flags);
	return (u8 *) ret;
}

void initZones(multiboot_tag_mmap * mmap) {
	zonesNr = 0;
	parseMmap(mmap);
}

PageMapEntry getPME(PageMap root, u8 * addr) {
	int i;
	for (i = 4; i > 0; i--) {
		PageMapEntry pme(root.getFields(),
				((u64) addr >> ((i - 1) * 9 + 12)) & 0x1FF);
		if (!pme.getOption(PME_PRESENT))
			return PageMapEntry();
		if (i == 1)
			return pme;
		root = PageMap(pme);
	}
	return PageMapEntry();
}

void flushTLB() {
	Reg cr3 = Reg("cr3");
	u64 tcr3 = cr3.get<u64>();
	cr3.set<u64>(tcr3);
}

void parseMmap(struct multiboot_tag_mmap * mmap) {
	u64 fin = (mmap->size - sizeof(*mmap) + sizeof(mmap->entries))
			/ mmap->entry_size;
	u8 i;
	for (i = 0; i < fin; i++) {
		u64 addr = mmap->entries[i].addr;
		u64 end = addr + mmap->entries[i].len;
		zones[i].begin = addr;
		zones[i].end = end;
		zones[i].pages = nullptr;
	}
	zonesNr = i + 1;
	qsort(zones, zonesNr, sizeof(Zone),
			[](const void * za, const void * zb)->int {
				return ((Zone *) za)->begin > ((Zone *) zb)->begin;
			});
	u8 z1 = 0;
	for (u8 i = 1; i < zonesNr; i++) {
		if (zones[z1].end >= zones[i].begin) {
			zones[z1].end = zones[i].end;
		} else {
			z1++;
			zones[z1] = zones[i];
		}
	}
	zonesNr = z1;
	pageTable = (Page *) (arena + sizeof(Arena) + sizeof(AllocatedChunk));
	u64 p = 0;
	z1 = 0;
	for (i = 0; i < fin; i++) {
		u64 addr = mmap->entries[i].addr;
		u64 end = addr + mmap->entries[i].len;

		if (addr > zones[z1].end)
			z1++;

		if (zones[z1].pages == nullptr)
			zones[z1].pages = pageTable + p;

		for (u64 j = 0; addr + j * PAGE_SIZE < end; j++) {
			pageTable[p] = Page((u8 *) addr + j * PAGE_SIZE);
			if (mmap->entries[i].type != 1)
				pageTable[p].setAvailable(false);
			p++;
		}
	}
}

void initArena() {
}

PageMapEntry findPME(u8 * address, PageMap root, u8 level) {
	u16 i;
	for (i = 0; i < 512; i++) {
		PageMapEntry pme = root.getEntry(i);
		if (pme.getOption(PME_PRESENT) && pme.getAddress() == address)
			return pme;
		else if (level > 1) {
			pme = findPME(address, (PageMap) pme, level - 1);
			if (pme.getField())
				return pme;
		}
	}
	return PageMapEntry();
}

void crawlPagingTree(PageMap root, u8 level) {
	u16 i;
	for (i = 0; i < 512; i++) {
		PageMapEntry pme = root.getEntry(i);
		if (pme.getOption(PME_PRESENT)) {
			if (level == 1)
				getPage(pme.getAddress())->incReferences();
			else
				crawlPagingTree((PageMap) pme, level - 1);
		}
	}
}

Page * getFreePage() {
	u8 i;
	for (i = 0; i < zonesNr; i++) {
		u64 j;
		for (j = 0; (j << 12) < zones[i].end; j++) {
			if (zones[i].pages[j].isFree())
				return zones[i].pages + j;
		}
	}
	return nullptr;
}

Page * getPage(u8 * base) {
	u8 i;
	for (i = 0; i < zonesNr; i++) {
		if ((u64) base >= zones[i].begin && (u64) base <= zones[i].end) {
			return zones[i].pages + (((u64) base - zones[i].begin) >> 12);
		}
	}
	return nullptr;
}

void invalidateTLBEntry(u8 * addr) {
	asm volatile(
			"invlpg (%0)" ::
			"r"((u64) addr) :
	);
}

bool mapPage(u8 * dst, u8 * src, u64 flags) {
	bool ptMod = false;
	u8 i;
	PageMap self = PageMap(PageMapLevel4);
	Page * npg = getPage((u8 *) src);
	if (npg == nullptr) {
		return false;
	}
	npg->incReferences();
	for (i = 4; i > 1; i--) {
		u16 index = (u64) dst >> (12 + 9 * (i - 1));
		index &= 0x1FF;
		PageMapEntry pme = self.getEntry(index);
		if (!pme.getOption(PME_PRESENT)) {
			ptMod = true;
			Page *pg = getFreePage();
			pme.setAddress(pg->getBase());
			pme.setOption(PME_PRESENT | PME_RW, true);
			pg->incReferences();
			self = ((PageMap) pme);
			self.clear();
		} else {
			self = ((PageMap) pme);
		}
	}
	u16 nindex = ((u64) dst >> 12) & 0x1FF;
	PageMapEntry npme = self.getEntry(nindex);
	invalidateTLBEntry(dst);
	npme.setAddress(npg->getBase());
	npme.setOption(flags, true);

	return ptMod;
}

void map(u8 * dst, u8 * src, u64 len, u64 flags) {
	u64 nrPages = len >> 12;
	nrPages = nrPages << 12 == len ? nrPages : nrPages + 1;
	u64 i;
	bool ptMod = false;
	for (i = 0; i < nrPages; i++) {
		ptMod |= mapPage((u8 *) ((u64) dst + (i << 12)),
				(u8 *) (((u64) src + i << 12)), flags);
	}
	if (ptMod) {
		flushTLB();
	}
}

void mapFirst(u8 *dst, u64 len, u64 flags) {
	u64 nrPages = len >> 12;
	nrPages = nrPages << 12 == len ? nrPages : nrPages + 1;
	u64 i;
	bool ptMod = false;
	for (i = 0; i < nrPages; i++) {
		ptMod |= mapPage(dst + (i << 12), getFreePage()->getBase(), flags);
	}
	if (ptMod) {
		flushTLB();
	}
}

}
}
