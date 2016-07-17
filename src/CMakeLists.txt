set (CMAKE_CXX_STANDARD 11)

set(src
	entry/entry.s
	entry/paging.cpp
	entry/multiboot.s
	arch64/arch64.cpp
	arch64/pit.cpp
	arch64/apic.cpp
	arch64/idt.cpp
	io/io.cpp
	mm/mm.cpp
	mm/Page.cpp
	mm/Chunk.cpp
	mm/PageMap.cpp
	mm/PageMapEntry.cpp
	kernel.cpp
	mboot.cpp
	Reg.cpp
	runtime/libcxx.cpp
	runtime/cstring.cpp
	runtime/qsort.cpp
	interrupts/int.s
	interrupts/int.cpp
)

enable_language(ASM-ATT)

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/include
)



add_executable(kmd ${src})
set(CMAKE_C_FLAGS "-m64 -ffreestanding -mcmodel=large -mno-red-zone -mno-mmx -mno-sse -mno-sse2 -gdwarf-2 -O0 -nostdlib" CACHE STRING "" FORCE)
set(CMAKE_ASM-ATT_FLAGS "--64 -march=generic64 --gdwarf2" CACHE STRING "" FORCE)
SET(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -fno-rtti -fno-exceptions" CACHE STRING "" FORCE)
SET(CMAKE_EXE_LINKER_FLAGS "-Wl,-eentry -Wl,-static -Wl,-T${CMAKE_SOURCE_DIR}/res/kmd.ld -Wl,-z,max-page-size=0x1000 -nostdlib" CACHE STRING "" FORCE)
SET(CMAKE_CXX_LINK_FLAGS "${CMAKE_EXE_LINKER_FLAGS}" CACHE STRING "" FORCE)
