#include <io.h>
#include <pit.h>

/* PIT (i8253) registers */
#define PIT_REG_MODE 0x43
#define PIT_REG_CH0  0x40

/* Count frequency in Hz */
#define PIT_HZ 1193180

namespace kmd {
namespace arch64 {
namespace pit {

void setMode(u8 mode) {
	kmd::io::out8(PIT_REG_MODE, mode);
}

void modeTwoSetDivisor(u16 divisor) {
	kmd::io::out8(PIT_REG_CH0, divisor & 0xff);
	kmd::io::out8(PIT_REG_CH0, divisor >> 8);
}

u16 getCount() {
	u16 count;
	setMode(0x00);
	count = kmd::io::in8(PIT_REG_CH0);
	count |= kmd::io::in8(PIT_REG_CH0) << 8;
	return count;
}

void wraparound() {
	u16 count, oldCount;
	count = oldCount = getCount();
	while(count <= oldCount) {
		oldCount = count;
		count = getCount();
	}
}

}
}
}
