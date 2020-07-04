#pragma once
/* ****************************************************************
   LockDownNesEmu Nes Emulator

   Header file for the cartdridge class
   Date: 29 March 2020        Author : William Kosasih

**************************************************************** */

#include <vector>
#include <string>
#include <memory>

#include "enum.h"
#include "types.h"
#include "mapper.h"

class mapper;

class cartridge_c
{
private:
  std::vector<uint8_t> program_memory;
  std::vector<uint8_t> character_memory;

  uint8_t mapper_id;
  uint8_t program_banks;
  uint8_t character_banks;
  uint8_t char_banks;

public:
  void load(const std::string& file_name);
  bool_t image_valid;
  uint8_t address_invalid;

  bool_t cpu_read(const uint16_t address, uint8_t& data);
  bool_t cpu_write(const uint16_t address, const uint8_t data);

  bool_t ppu_read(const uint16_t address, uint8_t& data);
  bool_t ppu_write(const uint16_t address, const uint8_t data);

  cartridge_mirror_e mirror;
  mapper_c* mapper_ptr;
};