#include "cartridge.h"
#include "enum.h"
#include "types.h"

#include <vector>
#include <string>
#include <fstream>

using namespace std;

void cartridge_c::load(const string& file_name) 
{
  image_valid = false;
  struct header_s
  {
    char_t name[4];
    uint8_t prg_rom_chunks;
    uint8_t chr_rom_chunks;
    uint8_t mapper1;
    uint8_t mapper2;
    uint8_t prg_ram_size;
    uint8_t tv_system1;
    uint8_t tv_system2;
    char_t unused[5];
  } header;

  ifstream file;
  file.open(file_name, ifstream::binary);
  if (file.is_open())
  {
    file.read((byte_t*)&header, sizeof(header));

    if (header.mapper1 & 0x04) file.seekg(512, ios_base::cur);
    
    mapper_id = ((header.mapper2 >> 4) << 4) | (header.mapper1 >> 4);

    if (header.mapper1 % 2)
      mirror = vertical_cartridge_mirror;
    else
      mirror = horizontal_cartridge_mirror;

    // :TODO: Implement this later.
    uint8_t file_type = 1;

    if (file_type)
    {
      program_banks = header.prg_rom_chunks;
      program_memory.resize(uint32_t(program_banks) * 16384);
      file.read((byte_t*)program_memory.data(), program_memory.size());

      character_banks = header.chr_rom_chunks;
      character_memory.resize(uint32_t(character_banks) * 8192);
      file.read((byte_t*)character_memory.data(), sizeof(character_memory));
    }
    image_valid = true;
    file.close();
  }
}

template<component_e component>
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