#include <cstdint>

#include "mapper.h"
#include "mapper_000.h"
#include "types.h"

mapper_000_c::mapper_000_c(const uint8_t program_banks,
  const uint8_t character_banks)
  : mapper_c(program_banks, character_banks)
{

}

void mapper_000_c::reset()
{

}

bool_t mapper_000_c::map_cpu_read(const uint16_t address, uint32_t& mapped_address)
{
  if (address >= 0x8000 && address <= 0xffff)
  {
    // Wrap around either 32K or 16K depending on the number of program banks
    // installed on the cartridge.
    mapped_address = address & ((program_banks > 1) ? 0x7fff : 0x3fff);
    return true;
  }
  return false;
}

bool_t mapper_000_c::map_cpu_write(const uint16_t address,
                                uint32_t& mapped_address)
{
  if (address >= 0x8000 && address <= 0xffff)
  {
    // Wrap around either 32K or 16K depending on the number of program banks 
    // installed on the cartridge.
    mapped_address = address & ((program_banks > 1) ? 0x7fff : 0x3fff);
    return true;
  }
  return false;
}

bool_t mapper_000_c::map_ppu_read(const uint16_t address, uint32_t& mapped_address)
{
  if (address >= 0x00 && address <= 0x1fff)
  {
    mapped_address = address;
    return true;
  }
  return false;
}

bool_t mapper_000_c::map_ppu_write(const uint16_t address,
                                uint32_t& mapped_address)
{
  if (address >= 0x00 && address <= 0x1fff)
  {
    if (character_banks == 0)
    {
      mapped_address = address;
      return true;
    }
  }
  return false;
}