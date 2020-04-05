#pragma once
/* ****************************************************************
   LockDownNesEmu Nes Emulator

   Header file for mapper 0
   Date: 5 April 2020        Author : javidx9(github)

**************************************************************** */
#include <cstdint>

#include "mapper.h"
#include "types.h"

class mapper_000_c : public mapper_c
{
public:
  mapper_000_c(const uint8_t program_banks, const uint8_t character_banks);
  bool_t map_cpu_read(const uint16_t address, uint32_t& mapped_address) override;
  bool_t map_cpu_write(const uint16_t address, uint32_t& mapped_address) override;
  bool_t map_ppu_read(const uint16_t address, uint32_t& mapped_address) override;
  bool_t map_ppu_write(const uint16_t address, uint32_t& mapped_address) override;
  void reset() override;
};