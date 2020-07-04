#pragma once
/* ****************************************************************
   LockDownNesEmu Nes Emulator

   Header file for the memory class
   Date: 27 March 2020        Author : William Kosasih

**************************************************************** */
#include <cstdint>
#include <array>
#include <memory>

#include "enum.h"
#include "ppu.h"
#include "processor.h"

class cartridge_c;
class ppu_c;

class bus_c
{
private:
  std::array<uint8_t, 65536> ram;
  uint8_t cycle;

  std::array<uint8_t, 2> controller_state;

  uint16_t dma_page = 0x0;
  uint16_t dma_address = 0x0;
  bool_t dma_transfer = false;

public:
  bus_c(cartridge_c*, ppu_c*);
  uint8_t cpu_read(const uint16_t address);
  void cpu_write(const uint16_t address, uint8_t data);

  void plug_in_processor(processor_c* const processor_ptr);

  std::array<uint8_t, 2> controller;

  void clock();

  cartridge_c* cartridge;
  ppu_c* ppu;
  processor_c* processor_ptr;
};