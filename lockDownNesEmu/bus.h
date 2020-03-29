#pragma once
/* ****************************************************************
   LockDownNesEmu Nes Emulator

   Header file for the memory class
   Date: 27 March 2020        Author : William Kosasih

**************************************************************** */
#include <cstdint>
#include <array>

#include "enum.h"

class cartridge_c;
class ppu_c;

class bus_c
{
private:
  std::array<uint8_t, 65536> ram;
  cartridge_c& cartridge;

public:
  bus_c(cartridge_c&);
  uint8_t& operator[](const uint16_t address);
  template <component_e> uint8_t& access(const uint16_t);

};