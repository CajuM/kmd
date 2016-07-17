#include <int.h>

#define APIC_EOI 0B0h

namespace kmd {
namespace arch64 {
namespace apic {
	u8 * getBase();
	u32 getReg(u16 reg);
	void setReg(u16 reg, u32 val);
	void init();
	u32 getFreq();
}
}
}
