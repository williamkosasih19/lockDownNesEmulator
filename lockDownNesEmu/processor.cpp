#include "processor.h"
#include "types.h"
#include "bus.h"
#include "enum.h"

#include <array>

using namespace std;

processor::processor(bus& mem_ref) : main_bus(mem_ref) 
{

}

void processor::reset() 
{
  // Reset registers.
  a = 0;
  x = 0;
  y = 0;
  sp = 0xfd;
  status = 0;

  // Set pc to entry address at (*(0xfffc + 1) << 8) | *(0xfffc)
  const uint16_t fetched_low = main_bus[0xfffc];
  const uint16_t fetched_high = main_bus[0xfffd];
  pc = (fetched_high << 8 | fetched_low);

  return;
}

// :TODO: simplify this, this is so ugly
void processor::set_flag(const cpu_flags_e cpu_flag, const bool_t value) 
{
  uint8_t temp_value;
  switch (cpu_flags_e(cpu_flag))
  {
  case carry_cpu_fl:
    temp_value = uint8_t(1)  << cpu_flags_shift_e::carry_shift;
    break;
  case zero_cpu_fl:
    temp_value = uint8_t(1) << cpu_flags_shift_e::zero_shift;
    break;
  case disable_interrupt_cpu_fl:
    temp_value = uint8_t(1) << cpu_flags_shift_e::disable_interrupt_shift;
    break;
  case decimal_cpu_fl:
    temp_value = uint8_t(1) << cpu_flags_shift_e::decimal_shift;
    break;
  case break_cpu_fl:
    temp_value = uint8_t(1) << cpu_flags_shift_e::break_shift;
    break;
  case unused_cpu_fl:
    temp_value = uint8_t(1) << cpu_flags_shift_e::unused_shift;
    break;
  case overflow_cpu_fl:
    temp_value = uint8_t(1) << cpu_flags_shift_e::overflow_shift;
    break;
  case negative_cpu_fl:
    temp_value = uint8_t(1) << cpu_flags_shift_e::negative_shift;
    break;
  }
  if (value)
    status |= temp_value;
  else
    status &= (~temp_value);
  return;
}

bool_t processor::get_flag(const cpu_flags_e cpu_flag)
{
  uint8_t shift_value;
  switch (cpu_flags_e(cpu_flag))
  {
  case carry_cpu_fl:
    shift_value = cpu_flags_shift_e::carry_shift;
    break;
  case zero_cpu_fl:
    shift_value = cpu_flags_shift_e::zero_shift;
    break;
  case disable_interrupt_cpu_fl:
    shift_value = cpu_flags_shift_e::disable_interrupt_shift;
    break;
  case decimal_cpu_fl:
    shift_value = cpu_flags_shift_e::decimal_shift;
    break;
  case break_cpu_fl:
    shift_value = cpu_flags_shift_e::break_shift;
    break;
  case unused_cpu_fl:
    shift_value = cpu_flags_shift_e::unused_shift;
    break;
  case overflow_cpu_fl:
    shift_value = cpu_flags_shift_e::overflow_shift;
    break;
  case negative_cpu_fl:
    shift_value = cpu_flags_shift_e::negative_shift;
    break;
  }
  return (status >> shift_value) % 2;
}

void processor::execute(number32_t instructions)
{
  // fetch_opcode later here
  instruction = 0x00; // read from pc address

  for (index32_t i = 0; i < instructions; i++)
  {
    const uint8_t lower_half = instruction & 0xff;
    const uint8_t upper_half = (instruction & 0xff00) >> 4;
    const auto& instr_cycle_pair = op_table[upper_half][lower_half];

    const uint8_t op = instr_cycle_pair.first.first;
    const uint8_t addr_mode = instr_cycle_pair.first.second;

    const uint8_t cycle = instr_cycle_pair.second;
    uint8_t additional_cycle = 0;

    bool_t op_extra_cycle = false;

    switch (addr_mode_e(addr_mode))
    {
    case imp_addr:
      data = a;
      break;
    case imm_addr:
      addr = pc;
      pc++;
      break;
    case zp_addr:
      addr = main_bus[pc];
      addr &= 0xff;
      pc++;
      break;
    case zpx_addr:
      addr = main_bus[pc] + x;
      addr &= 0xff;
      pc++;
      break;
    case zpy_addr:
      addr = main_bus[pc++] + y;
      addr &= 0xff;
      break;
    case rel_addr:
      addr = main_bus[pc];
      // Sign extend to 16 bits if the 8-bit offset is negative.
      if (addr & 0x80) addr |= 0xFF00;
      pc++;
      break;
    case abs_addr:
      addr = (main_bus[pc] << 8) | main_bus[pc];
      pc += 2;
      break;
    case abx_addr:
      const uint16_t lower_half = main_bus[pc];
      const uint16_t upper_half = main_bus[pc + 1];
      addr = ((upper_half << 8) | lower_half) + x;
      // If the addition with x results in the page  incremented, 
      // then add an additional cycle.
      if ((addr & 0xff00) != (upper_half << 8)) additional_cycle += 1;
      pc += 2;
      break;
    case aby_addr:
      const uint16_t lower_half = main_bus[pc];
      const uint16_t upper_half = main_bus[pc + 1];
      addr = ((upper_half << 8) | lower_half) + y;
      // If the addition with y results in the page  incremented,
      // then add an additional cycle.
      if ((addr & 0xff00) != (upper_half << 8))
        additional_cycle += 1;
      pc += 2;
      break;
    case ind_addr:
      const uint16_t lower_half = main_bus[pc];
      const uint16_t upper_half = main_bus[pc + 1];
      const uint16_t ptr = (upper_half << 8) | lower_half;
      if (lower_half == 0xff)
      {
        // Simulate the bug where the lower 8 bit overflow but 
        // the upper half doesn't get incremented.
        addr = (main_bus[ptr & 0xff00] | main_bus[ptr]);
      }
      else
      {
        addr = (main_bus[ptr + 1] | main_bus[ptr]);
      }
      pc += 2;
      break;
    case izx_addr:
      const uint16_t ptr = main_bus[pc];
      addr = (main_bus[(ptr + uint16_t(x)) & 0xff] << 8) |
             main_bus[(ptr + uint16_t(x) + 1) & 0xff];
      pc++;
      break;
    case izy_addr:
      const uint16_t ptr = main_bus[pc];
      uint16_t lower_half = main_bus[ptr & 0xff];
      uint16_t upper_half = main_bus[(ptr + 1) & 0xff];
      addr = ((upper_half << 8) | lower_half) + y;
      if ((addr & 0xff00) != (upper_half << 8))
        additional_cycle += 1;
      pc++;
      break;
    }

    switch (opcode_e(op))
    {
    case adc_op:
      data = main_bus[addr];
      uint16_t temp = uint16_t(a) + uint16_t(data) +
                      uint16_t(get_flag(cpu_flags_e::carry_cpu_fl));
      set_flag(cpu_flags_e::carry_cpu_fl, temp > 255);
      set_flag(cpu_flags_e::zero_cpu_fl, (temp & 0xff) == 0);
      // clang-format off
      set_flag(
        cpu_flags_e::overflow_cpu_fl, (~((uint16_t)a ^ (uint16_t)data) & 
          ((uint16_t)a ^ (uint16_t)temp)) & 0x0080);
      // clang-format on
      set_flag(cpu_flags_e::negative_cpu_fl, temp & 0x80);
      a = temp & 0xff;
      break;
    }

  }
}