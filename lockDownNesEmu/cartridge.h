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
  uint8_t& operator[](const uint16_t adress);
  bool_t image_valid;
  uint8_t address_invalid;

  cartridge_mirror_e mirror;
};