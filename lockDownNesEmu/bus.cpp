#include "bus.h"

bus::bus() 
{ 
  ram.fill(0); 
}

uint8_t& bus::operator[](const uint16_t address) 
{
  // User ram space/
  return ram[address];

}