#include <apic.h>

#include <mm.h>
#include <arch64.h>
#include <pit.h>
#include <io.h>

#define IA32_APIC_BASE 0x1B

#define APIC_TIMER_DIVIDE 0x3E0
#define APIC_TIMER_COUNT 0x380
#define APIC_TIMER_CURRENT 0x390
#define APIC_LVT_TIMER 0x320
#define APIC_SVR 0x0F0
#define APIC_LVT_LINT0 0x350
#define APIC_LVT_LINT1 0x360
#define APIC_ERROR 0x280
#define APIC_VERSION 0x030

#define TICK_MS 2

extern "C" {
	u8 * apic_regs = nullptr;
}

namespace kmd {
namespace arch64 {
namespace apic {
	u32 khz;

	u32 getVersion() {
		return getReg(APIC_VERSION);
	}

	u8 * getBase() {
		return (u8 *)(rdmsr(IA32_APIC_BASE) & (((1ull << 36) - 1) << 12));
	}

	void setBase(u64 addr) {
		wrmsr(IA32_APIC_BASE, (1 << 11) | (0 << 10) | (1 << 8));
	}

	u32 getReg(u16 reg) {
		return *((u32 *)(((u64) apic_regs) + reg));
	}

	void setReg(u16 reg, u32 val) {
		*(u32 *)(((u64) apic_regs) + reg) = val;
	}

	void disable_pic() {
	    kmd::io::out8(0x20, 0x11);
	    kmd::io::out8(0xA0, 0x11);
	    kmd::io::out8(0x21, 32);
	    kmd::io::out8(0xA1, 32 + 8);
	    kmd::io::out8(0x21, 0x04);
	    kmd::io::out8(0xA1, 0x02);
	    kmd::io::out8(0x21, 0x01);
	    kmd::io::out8(0xA1, 0x01);
	    kmd::io::out8(0x21, 0x0);
	    kmd::io::out8(0xA1, 0x0);

		kmd::io::out8(0x21, 0xFF);
		kmd::io::out8(0xA1, 0xFF);
	}

	void init() {
		disable_pic();

		apic_regs = getBase();
		bool b = kmd::mm::mapPage((u8 *) apic_regs, apic_regs, PME_PCD | PME_PWT | PME_RW | PME_PRESENT);
		if (b) kmd::mm::flushTLB();
		u32 v = getVersion();


		setReg(APIC_SVR, (1 << 8) | getReg(APIC_SVR));

		khz = getFreq();

		setReg(APIC_TIMER_DIVIDE, 0xb);
		setReg(APIC_TIMER_COUNT, khz * TICK_MS);

		setReg(APIC_LVT_LINT0, 0x7 << 8 | 1 << 16);
		setReg(APIC_LVT_LINT1, 1 << 16);

		setReg(APIC_LVT_TIMER, 0xFE | (1 << 17));
	}

	u32 getFreq() {
		kmd::arch64::pit::setMode(0x34);
		kmd::arch64::pit::modeTwoSetDivisor((PIT_HZ * PIT_WRAPAROUND_MS) / 1000);
		kmd::arch64::pit::wraparound();
		setReg(APIC_TIMER_DIVIDE, 0x0B);
		setReg(APIC_TIMER_COUNT, 0xffffffff);
		kmd::arch64::pit::wraparound();
		return (0xffffffff - getReg(APIC_TIMER_CURRENT)) / PIT_WRAPAROUND_MS;
	}
}
}
}
