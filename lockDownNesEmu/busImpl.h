#include "bus.h"
#include "cartridge.h"
#include "cartridgeImpl.h"

#include <cstdint>

template <component_e component> uint8_t& bus_c::access(const uint16_t address)
{
  switch (component)
  {
  case cpu_component:
    if (address >= 0x00 && address <= 0x1fff)
    {
      // Access the cpu's ram here.
      return ram[address];
    }
    if (address >= 0x8000 && address <= 0xffff)
    {
      return cartridge.access<component>(address);
    }
    if (address >= 0x2000 && address <= 0x3fff)
    {
      // ppu.access<cpu_component>
    }
    break;

  case ppu_component:
    if (address >= 0x00 && address <= 0x1fff)
    {
      // return cartridge.access<component>(address)
    }
    break;
  }
}