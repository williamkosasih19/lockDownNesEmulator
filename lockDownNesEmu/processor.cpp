#include "processor.h"
#include "types.h"
#include "bus.h"
#include "busImpl.h"
#include "enum.h"

#include <array>
#include <iostream>
#include <bitset>

using namespace std;

processor_c::processor_c(bus_c& mem_ref) : bus(mem_ref) 
{

}

void processor_c::reset() 
{
  // Reset registers.
  a = 0;
  x = 0;
  y = 0;
  sp = 0xfd;
  status = 0;

  // Set pc to entry address at (*(0xfffc + 1) << 8) | *(0xfffc)
  const uint16_t fetched_low = bus[0xfffc];
  const uint16_t fetched_high = bus[0xfffd];
  pc = (fetched_high << 8 | fetched_low);

  cycles = 8;

  return;
}

void processor_c::set_flag(const cpu_flags_e cpu_flag, const bool_t value) 
{
  if (value)
    status |= cpu_flag;
  else
    status &= (~cpu_flag);
}

bool_t processor_c::get_flag(const cpu_flags_e cpu_flag)
{
  return (status & cpu_flag);
}

void processor_c::execute()
{
  // fetch_opcode from bus(cartridge)
  instruction = bus.access<cpu_component>(pc);
  pc++;

  if (cycles == 0)
  {
    const uint8_t lower_half = instruction & 0xf;
    const uint8_t upper_half = instruction >> 4;
    const auto& instr_cycle_pair = op_table[upper_half][lower_half];

    op = instr_cycle_pair.first.first;
    addr_mode = instr_cycle_pair.first.second;

    cycles = instr_cycle_pair.second;
    uint8_t additional_cycle = 0;

    bool_t op_additional_cycle = false;

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
      addr = bus[pc];
      addr &= 0xff;
      pc++;
      break;
    case zpx_addr:
      addr = bus[pc] + x;
      addr &= 0xff;
      pc++;
      break;
    case zpy_addr:
      addr = bus[pc++] + y;
      addr &= 0xff;
      break;
    case rel_addr:
      addr = bus[pc];
      // Sign extend to 16 bits if the 8-bit offset is negative.
      if (addr & 0x80) addr_relative |= 0xFF00;
      pc++;
      break;
    case abs_addr:
      addr = (bus[pc + 1] << 8) | bus[pc];
      pc += 2;
      break;
    case abx_addr: 
    {
      const uint16_t lower_half = bus[pc];
      const uint16_t upper_half = bus[pc + 1];
      addr = ((upper_half << 8) | lower_half) + x;
      // If the addition with x results in the page  incremented,
      // then add an additional cycle.
      if ((addr & 0xff00) != (upper_half << 8))
        additional_cycle += 1;
      pc += 2;
      break;
    }
    case aby_addr: 
    {
      const uint16_t lower_half = bus[pc];
      const uint16_t upper_half = bus[pc + 1];
      addr = ((upper_half << 8) | lower_half) + y;
      // If the addition with y results in the page  incremented,
      // then add an additional cycle.
      if ((addr & 0xff00) != (upper_half << 8))
        additional_cycle += 1;
      pc += 2;
      break;
    }
    case ind_addr: 
    {
      const uint16_t lower_half = bus[pc];
      const uint16_t upper_half = bus[pc + 1];
      const uint16_t ptr = (upper_half << 8) | lower_half;
      if (lower_half == 0xff)
      {
        // Simulate the bug where the lower 8 bit overflow but
        // the upper half doesn't get incremented.
        addr = (bus[ptr & 0xff00] | bus[ptr]);
      }
      else
      {
        addr = (bus[ptr + 1] | bus[ptr]);
      }
      pc += 2;
      break;
    }
    case izx_addr: 
    {
      const uint16_t ptr = bus[pc];
      addr = (bus[(ptr + uint16_t(x) + 1) & 0xff] << 8) |
             bus[(ptr + uint16_t(x)) & 0xff];
      pc++;
      break;
    }

    case izy_addr: 
    {
      const uint16_t ptr = bus[pc];
      uint16_t lower_half = bus[ptr & 0xff];
      uint16_t upper_half = bus[(ptr + 1) & 0xff];
      addr = ((upper_half << 8) | lower_half) + y;
      if ((addr & 0xff00) != (upper_half << 8))
        additional_cycle += 1;
      pc++;
      break;
    }

    }

    switch (opcode_e(op))
    {
    case adc_op: 
    {
      data = bus[addr];
      uint16_t temp = uint16_t(a) + uint16_t(data) +
                      uint16_t(get_flag(carry_cpu_fl));
      set_flag(carry_cpu_fl, temp > 255);
      set_flag(zero_cpu_fl, (temp & 0xff) == 0);
      // clang-format off
      set_flag(
        overflow_cpu_fl, (~(uint16_t(a) ^ uint16_t(data)) & 
          (uint16_t(a) ^ uint16_t(temp))) & 0x0080);
      // clang-format on
      set_flag(cpu_flags_e::negative_cpu_fl, temp & 0x80);
      a = temp & 0xff;
      op_additional_cycle = true;
      break;
    }
    // A = A - M - (1 - C)
    case sbc_op: 
    {
      data = bus[addr];

      // Flip the lower half => (lh - 1) 
      const uint16_t flipped = (uint16_t(data) ^ 0xff);

      uint16_t temp =
          uint16_t(a) + uint16_t(flipped) + uint16_t(get_flag(carry_cpu_fl));
      set_flag(carry_cpu_fl, temp > 255);
      set_flag(zero_cpu_fl, (temp & 0xff) == 0);
      // clang-format off
      set_flag(
        overflow_cpu_fl, (temp ^ flipped) & 
          (uint16_t(a) ^ temp) & 0x80);
      // clang-format on
      set_flag(cpu_flags_e::negative_cpu_fl, temp & 0x80);
      a = temp & 0xff;
      op_additional_cycle = true;
      break;
    }
    case and_op:
      data = bus[addr];
      a = a & data;
      set_flag(zero_cpu_fl, a == 0);
      set_flag(negative_cpu_fl, a & 0x80);
      op_additional_cycle = true;
      break;
    case asl_op: 
    {
      if (addr_mode != imp_addr)
        data = bus[addr];
      uint16_t temp = uint8_t(data) << 1;
      set_flag(carry_cpu_fl, temp & 0xff00);
      set_flag(zero_cpu_fl, !(temp & 0xff));
      set_flag(negative_cpu_fl, temp & 0x80);
      break;
    }
    case bcc_op:
    {
      if (!get_flag(carry_cpu_fl))
      {
        cycles++;
        addr = pc + addr_relative;

        if ((addr & 0xff00) != (pc & 0xff00))
          cycles++;

        pc = addr;
      }
      break;
    }
    case sec_op:
      set_flag(carry_cpu_fl, true);
      break;
    case sed_op:
      set_flag(decimal_cpu_fl, true);
      break;
    case sei_op:
      set_flag(disable_interrupt_cpu_fl, true);
      break;
    case sta_op:
      bus[addr] = a;
      break;
    case stx_op:
      bus[addr] = x;
      break;
    case sty_op:
      bus[addr] = y;
      break;
    }
    if (op_additional_cycle)
      cycles += additional_cycle;
    set_flag(unused_cpu_fl, true);

  }
  else
    cycles--;
}

void processor_c::query() 
{ 
  cout << "########## Registers ##########" << endl;
  printf("a : %u\n", a);
  printf("x : %u\n", x);
  printf("y : %u\n", y);
  printf("pc : %04x\n", pc);
  printf("status : %s\n", bitset<8>(status).to_string().c_str());
  cout << "########## Stats ##########" << endl;
  printf("instruction : %s\n", opcode_string_map[opcode_e(op)].c_str());
  printf("adressing mode : %s\n",
         addr_string_map[addr_mode_e(addr_mode)].c_str());
  printf("cycles left %u\n", cycles);
  printf("addr : %04x\n", addr);
  printf("addr_relative : %04x\n", addr_relative);
  printf("\n\n");
}