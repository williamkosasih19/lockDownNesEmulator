#pragma once
/* ****************************************************************
   LockDownNesEmu Nes Emulator

   Header file for the mapper class
   Date: 5 April 2020        Author : javidx9 (github)

**************************************************************** */

#include <cstdint>

#include "types.h"
class mapper_c
{
public:
  mapper_c(const uint8_t program_banks, uint8_t character_banks);

  virtual bool_t map_cpu_read(const uint16_t address,
                            uint32_t& mapped_address) = 0;
  virtual bool_t map_cpu_write(const uint16_t address,
                             uint32_t& mapped_address) = 0;
  virtual bool_t map_ppu_read(const uint16_t address,
                            uint32_t& mapped_address) = 0;
  virtual bool_t map_ppu_write(const uint16_t address,
                             uint32_t& mapped_address) = 0;
  virtual void reset() = 0;

  protected:
  uint8_t program_banks = 0;
  uint8_t character_banks = 0;
};