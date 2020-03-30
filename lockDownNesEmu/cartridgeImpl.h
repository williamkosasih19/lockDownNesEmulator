#include "cartridge.h"

#include <cstdint>

template <component_e component>
uint8_t& cartridge_c::access(const uint16_t address)
{
  switch (mapper_id)
  {
  case 0:
    if (component == cpu_component)
    {
      return program_memory[(program_banks > 1) ? address % 32768
                                                : address % 16384];
    }
    else if (component == ppu_component)
    {
      if (address >= 0x00 && address <= 0x1FFF)
      {
        return character_memory[address];
      }
    }
    break;
  }
}