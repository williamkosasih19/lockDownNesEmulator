#include "bus.h"

bus_c::bus_c(cartridge_c& main_cartridge) : cartridge(main_cartridge)
{ 
  ram.fill(0); 
}

uint8_t& bus_c::operator[](const uint16_t address)
{
  // User ram space/
  if (address >= 0x00 && address <= 0x1fff)
  {
    // Access the cpu's ram here.
    return ram[address];
  }
  if (address >= 0x8000 && address <= 0xffff)
  {
    // return cartridge.access<component>(address)
  }
  if (address >= 0x2000 && address <= 0x3fff)
  {
    // ppu.access<cpu_component>
  }
}

template<component_e component> uint8_t& bus_c::access(const uint16_t address)
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
      // return cartridge.access<component>(address)
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