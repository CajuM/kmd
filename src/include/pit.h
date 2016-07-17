#include <int.h>

#define PIT_WRAPAROUND_MS 50
#define PIT_HZ 1193180

namespace kmd {
namespace arch64 {
namespace pit {

void setMode(u8 mode);
void modeTwoSetDivisor(u16 divisor);
u16 getCount();
void wraparound();

}
}
}
