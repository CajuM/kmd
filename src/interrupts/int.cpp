#include <int.h>
#include <arch64.h>

extern "C" void do_int(u64 interrupt, u64 error) {
	kmd::arch64::halt();
}
