#pragma once
/* ****************************************************************
   LockDownNesEmu Nes Emulator

   Header file for the memory class
   Date: 27 March 2020        Author : William Kosasih

**************************************************************** */
#include <cstdint>
#include <array>

class bus
{
private:
  std::array<uint8_t, 65536> ram;

public:
  bus();
  uint8_t& operator[](const uint16_t address);

};