#include <arch64.h>
#include <mm.h>
#include <mboot.h>

using namespace kmd;

extern "C" void kMain(u64 addr, u64 magic) {
	mm::init(addr);
	arch64::init();
	arch64::halt();
}
