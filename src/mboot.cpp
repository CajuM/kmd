#include <multiboot2.h>

namespace kmd {

multiboot_tag * getMBITag(multiboot_uint64_t addr, multiboot_uint32_t type) {
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

}
