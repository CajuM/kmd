namespace kmd {
namespace arch64 {
namespace idt {

void setEntry(u8 index, u64 offset, u8 flags);
void lidt(u64 base, u16 limit);
void init();

}
}
}
