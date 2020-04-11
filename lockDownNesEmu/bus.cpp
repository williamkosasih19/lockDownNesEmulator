#include <memory>

#include "bus.h"
#include "cartridge.h"
#include "enum.h"

#include <sdl.h>
#undef main

using namespace std;

bus_c::bus_c(cartridge_c& main_cartridge, ppu_c& ppu)
    : cartridge(main_cartridge), ppu(ppu)
{ 
  ram.fill(0); 
  cycle = 0;
}

void bus_c::plug_in_processor(processor_c* const processor_ptr)
{
  this->processor_ptr = processor_ptr;
  return;
}


uint8_t bus_c::cpu_read(const uint16_t address)
{
  uint8_t data;
  if(cartridge.cpu_read(address, data))
  {
  
  }
  else if (address >= 0x00 && address <= 0x1fff)
  {
    data = ram[address & 0x07ff];
  }
  else if (address >= 0x2000 && address <= 0x3fff)
  {
    data = ppu.cpu_read(address & 0x7);
  }
  else if (address >= 0x4016 && address <= 0x4017)
  {
    // Read the rightmost statte of the controller then shift left.
    data = (controller_state[address & 0b1] & 0x80) > 0;
    controller_state[address & 0b1] == controller_state[address & 0b1] << 1;
  }
  return data;
}

void bus_c::cpu_write(const uint16_t address, const uint8_t data)
{
  if (cartridge.cpu_write(address, data))
  {
  }
  else if (address >= 0x00 && address <= 0x1fff)
  {
    ram[address & 0x7ff] = data;
  }
  else if (address >= 0x2000 && address <= 0x3fff)
  {
    ppu.cpu_write(address & 0x7, data);
  }
  else if (address >= 0x4016 && address <= 0x4017)
  {
    controller_state[address & 0b1] = controller[address & 0b1];
  }
}

void bus_c::clock()
{
  if (cycle == 0)
  {
    processor_ptr->execute();
    if (ppu.nmi)
    {
      processor_ptr->nmi();
      ppu.nmi = false;
    }
      
  }
  ppu.clock();
  cycle = (cycle + 1) % 3;
}