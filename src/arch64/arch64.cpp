/*
 * Arch64.cpp
 *
 *  Created on: Nov 22, 2015
 *      Author: mihai
 */

#include <arch64.h>
#include <apic.h>
#include <idt.h>

#define IA32_PAT_MSR 0x277
#define IA32_PAT_MT_UNCACHEABLE     0x00
#define IA32_PAT_MT_WRITE_COMBINING 0x01
#define IA32_PAT_MT_WRITE_THROUGH   0x04
#define IA32_PAT_MT_WRITE_PROTECTED 0x05
#define IA32_PAT_MT_WRITE_BACK      0x06
#define IA32_PAT_MT_UNCACHED        0x07
#define PA(n, val) (((u64) val) << (8 * n))

namespace kmd {
namespace arch64 {

void halt() {
	asm volatile(
			"halt:"
			"jmp halt"
	);
}

u64 rdmsr(u32 msr) {
	u32 hi, lo;
	asm volatile(
			"rdmsr" :
			"=&d"(hi), "=&a"(lo) :
			"c"(msr) :
	);
	return (((u64) hi) << 32) + lo;
}

void wrmsr(u32 msr, u64 val) {
	u32 hi = val >> 32;
	u32 lo = val & 0xFFFFFFFF;
	asm volatile(
			"wrmsr" ::
			"c"(msr), "d"(hi), "a"(lo) :
	);
}

void sti() {
	asm volatile("sti");
}

void cli() {
	asm volatile("cli");
}

void int32() {
	asm volatile("int $32");
}

void init() {
	u64 mtrr = rdmsr(0x2FF);
	wrmsr(0x2FF, mtrr & 0xFFFFF300);
	u64 patMSR = rdmsr(IA32_PAT_MSR);
	wrmsr(IA32_PAT_MSR,
			patMSR | PA(0, IA32_PAT_MT_WRITE_BACK) |
			PA(1, IA32_PAT_MT_WRITE_THROUGH) |
			PA(2, IA32_PAT_MT_UNCACHED) |
			PA(3, IA32_PAT_MT_UNCACHEABLE) |
			PA(4, IA32_PAT_MT_WRITE_COMBINING)
			);
	kmd::arch64::idt::init();
	kmd::arch64::apic::init();
	sti();
}

}
}
