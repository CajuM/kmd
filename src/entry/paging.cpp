#include <mm.h>
#include <int.h>
#include <multiboot2.h>

extern void *kernel_phy;
extern void *kernel_phy_end;
extern void *kernel_virt;
extern void *kernel_virt_end;
extern void *_end;
extern void *_start;

#define SB  __attribute__((section(".boot")))
#define SBD  __attribute__((section(".boot.data")))

#define APIC_BASE 0xFFE00000
#define APIC_VERSION 0x030

void *kernel_phy_sym SBD = &kernel_phy;
void *kernel_phy_end_sym SBD = &kernel_phy_end;
void *kernel_virt_sym SBD = &kernel_virt;
void *kernel_virt_end_sym SBD = &kernel_virt_end;
void *_end_sym SBD = &_end;
void *_start_sym SBD = &_start;

u64 *tmp_pml4t SBD;
u64 *tmp_pt SBD;

void alloc_pml4t() SB;
void alloc_pt() SB;
void map_address_range_pt(int depth, u64 *self, u64 dst, u64 src, u64 len,
		unsigned perm, u8 last_level) SB;
void map_address_range(u64 dst, u64 src, u64 len, unsigned perm, u8 last_level)
		SB;
u64 calc_hierarchy_size(u64 size, u64 page_size) SB;
u8 page_level_from_size(u64 page_size) SB;
multiboot_tag * get_mbit_tag(multiboot_uint64_t addr, multiboot_uint32_t type)
		SB;
u64 get_phy_size(struct multiboot_tag_mmap * mmap) SB;

extern "C" void build_paging_table(u64 mbi) SB;

multiboot_tag * get_mbit_tag(multiboot_uint64_t addr, multiboot_uint32_t type) {
	struct multiboot_tag * tag;
	for (tag = (struct multiboot_tag *) (addr + 8);
			tag->type != MULTIBOOT_TAG_TYPE_END;
			tag = (struct multiboot_tag *) ((multiboot_uint8_t *) tag
					+ ((tag->size + 7) & ~7))) {
		if (tag->type == type)
			return tag;
	}
	return nullptr;
}

void alloc_pml4t() {
	int i;
	tmp_pml4t = (u64 *) ((((u64) _end_sym >> 21) + 1) << 21);
	tmp_pt = tmp_pml4t;
	for (i = 0; i < 512; i++)
		tmp_pml4t[i] = 0;
}

void alloc_pt() {
	int i;
	tmp_pt += 512;
	for (i = 0; i < 512; i++)
		tmp_pt[i] = 0;
}

void map_address_range_pt(int depth, u64* self, u64 dst, u64 src, u64 len,
		unsigned perm, u8 last_level) {

	u64 cl = 0;
	struct {
		u16 i;
		u64* self;
	} l[5];
	l[4].i = (dst >> (3 * 9 + 12)) & 0x1FF;
	l[4].self = self;
	l[3].i = (dst >> (2 * 9 + 12)) & 0x1FF;
	l[2].i = (dst >> (1 * 9 + 12)) & 0x1FF;
	l[1].i = (dst >> 12) & 0x1FF;
	u8 ll = 4;
	while (cl <= len) {
		if (l[ll].i >= 512) {
			l[ll].i = 0;
			ll++;
			self = l[ll].self;
			continue;
		}
		if (ll != last_level) {
			u64 ns = self[l[ll].i];
			if (ns & 1) {
				ns = (ns >> 12) << 12;
			} else {
				alloc_pt();
				ns = (u64) tmp_pt;
				self[l[ll].i] = ns | 3;
			}
			l[ll].i++;
			self = (u64*) ns;
			ll--;
			l[ll].self = self;
		} else {
			self[l[ll].i] = (src + cl) | 3;
			cl += 1 << 12;
			l[ll].i++;
		}
	}
}

void map_address_range(u64 dst, u64 src, u64 len, unsigned perm,
		u8 last_level) {
	u8 offset = (12 + 9 * (last_level - 1));
	u64 ndst = (dst >> offset) << offset;
	u64 nsrc = (src >> offset) << offset;
	u64 md = (src - nsrc) > (dst - ndst) ? (src - nsrc) : (dst - ndst);
	u64 nlen = len + md;
	u64 nnlen = (nlen >> offset) << offset;
	nlen = nlen == nnlen ? nlen : (((nlen >> offset) + 1) << offset);
	map_address_range_pt(4, tmp_pml4t, ndst, nsrc, nlen, perm, last_level);
}

u8 page_level_from_size(u64 page_size) {
	page_size >>= 12;
	int i = 0;
	while (page_size != 0) {
		i++;
		page_size >>= 9;
	}
	return i;
}

u64 calc_hierarchy_size(u64 size, u64 page_size) {
	u64 nr_pages = size / page_size;
	nr_pages = nr_pages * page_size == size ? nr_pages : nr_pages + 1;
	u8 page_level = page_level_from_size(page_size);
	if (nr_pages <= 512)
		return page_level * 512 * 8;
	u8 i;
	u64 ptt = nr_pages;
	u64 pt;
	for (i = page_size; i <= 4; i++) {
		ptt = ptt / 512 + 1;
		pt += ptt;
	}
	return ptt * 512 * 8 + calc_hierarchy_size(ptt * 512 * 8, page_size);
}

u64 get_phy_size(struct multiboot_tag_mmap * mmap) {
	u64 fin = (mmap->size - sizeof(*mmap) + sizeof(mmap->entries))
			/ mmap->entry_size;
	u64 i;
	u64 size = 0;
	for (i = 0; i < fin; i++) {
		u64 end = mmap->entries[i].addr + mmap->entries[i].len;
		u64 addr = mmap->entries[i].addr;
		map_address_range(addr, addr, end - addr, 3, 1);
		for (; addr < end; addr += PAGE_SIZE) {
			size += PAGE_SIZE;
		}
	}
	return size;
}

extern "C" void build_paging_table(u64 mbi) {

	u64 kernel_phy_size = (u64) _end_sym - (u64) _start;
	u64 kernel_virt_size = (u64) kernel_virt_end_sym - (u64) kernel_virt_sym;

	alloc_pml4t();

	map_address_range((u64) _start_sym, (u64) _start_sym,
			(u64) _end_sym - (u64) _start_sym, 3, 1);
	map_address_range(KERNEL_LINEAR_ADDR, (u64) kernel_virt_sym, kernel_virt_size,
			3, 1);

	struct multiboot_tag_mmap * mmap =
			(struct multiboot_tag_mmap *) get_mbit_tag(mbi,
			MULTIBOOT_TAG_TYPE_MMAP);

	u64 cSize = get_phy_size(mmap) >> 12;
	cSize = sizeof(kmd::mm::Page[cSize]);

	u64 arna = KERNEL_LINEAR_ADDR + kernel_virt_size;
	arna >>= 12;
	arna += 1;
	arna <<= 12;
	u64 freeAddr = ((u64) tmp_pt) + (1ull << 12);
	map_address_range((u64) arna, freeAddr,
			sizeof(struct kmd::mm::Arena)
					+ sizeof(struct kmd::mm::AllocatedChunk) + cSize,
			PME_RW | PME_PRESENT, 1);

	asm volatile(
			"movq $tmp_pml4t, %%rax;"
			"movq (%%rax), %%rax;"
			"movq %%rax, %%cr3;"
			::: "%rax", "memory"
	);
	kmd::mm::arena = (u8 *) arna;
	kmd::mm::Chunk c(kmd::mm::arena + sizeof(struct kmd::mm::Arena));
	c.setSize(cSize);
	c.setPrev(nullptr);
	c.setNext(nullptr);
	c.setFlags(CHUNK_FREE, false);
	c.setPrevFree(nullptr);
	c.setNextFree(nullptr);
	((struct kmd::mm::Arena *) kmd::mm::arena)->firstFree =
			(struct kmd::mm::FreeChunk *) (c.getSelf() + cSize
					+ sizeof(struct kmd::mm::AllocatedChunk));
}
