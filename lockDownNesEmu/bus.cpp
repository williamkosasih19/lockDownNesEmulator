#include "bus.h"
#include "cartridge.h"
#include "enum.h"

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
    return cartridge.access<cpu_component>(address);
  }
  if (address >= 0x2000 && address <= 0x3fff)
  {
    // ppu.access<cpu_component>
  }
}

