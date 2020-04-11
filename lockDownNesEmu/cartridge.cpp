#include <vector>
#include <string>
#include <fstream>
#include <memory>

#include "cartridge.h"
#include "enum.h"
#include "types.h"

#include "mapper.h"
#include "mapper_000.h"

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
  file.open(file_name, ios::binary);
  if (file.is_open())
  {
    file.read((byte_t*)&header, sizeof(header));

    if (header.mapper1 & 0x04) file.seekg(512, ios_base::cur);
    
    mapper_id = ((header.mapper2 >> 4) << 4) | (header.mapper1 >> 4);

    if (header.mapper1 & 0x1)
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
      file.read((byte_t*)character_memory.data(), character_memory.size());
    }
    image_valid = true;
    file.close();
  }

  switch (mapper_id)
  {
  case 0:
    mapper_ptr = make_shared<mapper_000_c>(program_banks, character_banks);
    break;
  case 1:

    break;
  }
}

bool_t cartridge_c::cpu_read(const uint16_t address, uint8_t& data)
{
  uint32_t mapped_address;
  if (mapper_ptr->map_cpu_read(address, mapped_address))
  {
    data = program_memory[mapped_address];
    return true;
  }
  else
    return false;
}

bool_t cartridge_c::cpu_write(const uint16_t address, uint8_t data)
{
  uint32_t mapped_address;
  if (mapper_ptr->map_cpu_write(address, mapped_address))
  {
    program_memory[mapped_address] = data;
    return true;
  }
  return false;
}

bool_t cartridge_c::ppu_read(const uint16_t address, uint8_t& data) {
  uint32_t mapped_address;
  if (mapper_ptr->map_ppu_read(address, mapped_address))
  {
    data = character_memory[mapped_address];
    return true;
  }
  return false;
}
bool_t cartridge_c::ppu_write(const uint16_t address, const uint8_t data)
{
  uint32_t mapped_address;
  if (mapper_ptr->map_ppu_write(address, mapped_address))
  {
    character_memory[mapped_address] = address;
    return true;
  }
  return false;
}