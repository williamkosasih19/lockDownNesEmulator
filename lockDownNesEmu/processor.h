#pragma once
/* ****************************************************************
   LockDownNesEmu Nes Emulator

   Header file for the 6502 processor class
   Date: 27 March 2020        Author : William Kosasih

**************************************************************** */

#include <cstdint>

class processor{
private:
  uint8_t a = 0;
  uint8_t x = 0;
  uint8_t y = 0;
  uint8_t sp = 0;
  uint16_t pc = 0;

  uint8_t status = 0;

  public:
  processor();

};