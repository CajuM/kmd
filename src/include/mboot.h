#include <multiboot2.h>

namespace kmd {
extern multiboot_tag * getMBITag(multiboot_uint64_t addr, multiboot_uint32_t type);
}
