#include <cstdint>

#include "mapper.h"
#include "types.h"

mapper_c::mapper_c(const uint8_t program_banks, uint8_t character_banks)
{
  this->program_banks = program_banks;
  this->character_banks = character_banks;
}
