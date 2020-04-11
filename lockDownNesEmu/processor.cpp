#include "processor.h"
#include "types.h"
#include "bus.h"
#include "enum.h"

#include "outputImpl.h"

#include <array>
#include <iostream>
#include <bitset>

using namespace std;

processor_c::processor_c(bus_c& mem_ref) : bus(mem_ref) 
{
  logOut.open("C:\\Users\\willi\\Documents\\lockDownCpu.log");
}

void processor_c::reset() 
{
  // Reset registers.
  a = 0;
  x = 0;
  y = 0;
  sp = 0xfd;
  status = 0 | cpu_flags_e::unused_cpu_fl;

  // Set pc to entry address at (*(0xfffc + 1) << 8) | *(0xfffc)
  const uint16_t fetched_low = bus.cpu_read(0xfffc);
  const uint16_t fetched_high = bus.cpu_read(0xfffd);
  pc = (fetched_high << 8 | fetched_low);

  bank_relative_addr = pc;

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

void processor_c::irq()
{
  if (get_flag(disable_interrupt_cpu_fl)) return;
  // Push the upper half of the pc to stack.
  bus.cpu_write(0x100 + sp - 1, pc >> 8);
  bus.cpu_write(0x100 + sp - 2, pc & 0xff);

  set_flag(break_cpu_fl, false);
  set_flag(unused_cpu_fl, true);
  set_flag(disable_interrupt_cpu_fl, true);

  bus.cpu_write(0x100 + sp - 3, status);
  sp -= 3;

  addr = 0xfffe;
  // Fetch handler location.
  const uint16_t low = bus.cpu_read(addr);
  const uint16_t high = bus.cpu_read(addr + 1);

  pc = (high << 8) | low;
  cycles = 7;
}

void processor_c::nmi()
{
  //// Push the upper half of the pc to stack.
  bus.cpu_write(0x100 + sp, pc >> 8);
  bus.cpu_write(0x100 + sp - 1, pc & 0xff);

  set_flag(break_cpu_fl, false);
  set_flag(unused_cpu_fl, true);
  set_flag(disable_interrupt_cpu_fl, true);

  bus.cpu_write(0x100 + sp - 2, status);
  sp -= 3;

  addr = 0xfffa;
  //// Fetch handler location.
  const uint16_t low = bus.cpu_read(addr);
  const uint16_t high = bus.cpu_read(addr + 1);

  pc = (high << 8) | low;
  cycles = 7;
}

void processor_c::execute()
{
  // fetch_opcode from bus(cartridge)
  if (cycles == 0)
  {
    instruction = bus.cpu_read(pc);
    pc++;

    set_flag(unused_cpu_fl, true);

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
      addr = bus.cpu_read(pc) & 0xff;
      pc++;
      break;
    case zpx_addr:
      addr = bus.cpu_read(pc) + x;
      addr &= 0xff;
      pc++;
      break;
    case zpy_addr:
      addr = bus.cpu_read(pc++) + y;
      addr &= 0xff;
      break;
    case rel_addr:
      addr_relative = bus.cpu_read(pc);
      // Sign extend to 16 bits if the 8-bit offset is negative.
      if (addr_relative & 0x80) addr_relative |= 0xFF00;
      pc++;
      break;
    case abs_addr:
      addr = (bus.cpu_read(pc + 1) << 8) | bus.cpu_read(pc);
      pc += 2;
      break;
    case abx_addr: 
    {
      const uint16_t lower_half = bus.cpu_read(pc);
      const uint16_t upper_half = bus.cpu_read(pc + 1);
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
      const uint16_t lower_half = bus.cpu_read(pc);
      const uint16_t upper_half = bus.cpu_read(pc + 1);

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
      const uint16_t lower_half = bus.cpu_read(pc);
      const uint16_t upper_half = bus.cpu_read(pc + 1);
      const uint16_t ptr = (upper_half << 8) | lower_half;
      if (lower_half == 0xff)
      {
        // Simulate the bug where the lower 8 bit overflow but
        // the upper half doesn't get incremented.
        addr = ((bus.cpu_read(ptr & 0xff00) << 8) | bus.cpu_read(ptr));
      }
      else
      {
        addr = ((bus.cpu_read(ptr + 1) << 8) | bus.cpu_read(ptr));
      }
      pc += 2;
      break;
    }
    case izx_addr: 
    {
      const uint16_t ptr = bus.cpu_read(pc);
      addr = (bus.cpu_read((ptr + uint16_t(x) + 1) & 0xff) << 8) |
             bus.cpu_read((ptr + uint16_t(x)) & 0xff);
      pc++;
      break;
    }

    case izy_addr: 
    {
      const uint16_t ptr = bus.cpu_read(pc);
      uint16_t lower_half = bus.cpu_read(ptr & 0xff);
      uint16_t upper_half = bus.cpu_read((ptr + 1) & 0xff);
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
      data = bus.cpu_read(addr);
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
      data = bus.cpu_read(addr);

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
      data = bus.cpu_read(addr);
      a = a & data;
      set_flag(zero_cpu_fl, a == 0);
      set_flag(negative_cpu_fl, a & 0x80);
      op_additional_cycle = true;
      break;
    case asl_op: 
    {
      if (addr_mode != imp_addr)
        data = bus.cpu_read(addr);
      uint16_t temp = uint16_t(data) << 1;
      set_flag(carry_cpu_fl, temp & 0xff00);
      set_flag(zero_cpu_fl, !(temp & 0xff));
      set_flag(negative_cpu_fl, temp & 0x80);
      if (addr_mode == imp_addr)
        a = temp & 0xff;
      else
        bus.cpu_write(addr, temp & 0xff);
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
    case bpl_op:
      if (!get_flag(negative_cpu_fl))
      {
        cycles++;
        addr = pc + addr_relative;
        if ((addr & 0xFF00) != (pc & 0xFF00))
          cycles++;

        pc = addr;
      }
      break;
    case bcs_op:
      if (get_flag(carry_cpu_fl))
      {
        cycles++;
        addr = pc + addr_relative;
        if ((addr & 0xFF00) != (pc & 0xFF00))
          cycles++;

        pc = addr;
      }
      break;
    case beq_op:
      if (get_flag(zero_cpu_fl))
      {
        cycles++;
        addr = pc + addr_relative;

        // If calculated address passes page boundary.
        if ((addr & 0xff00) != (pc & 0xff00))
          cycles++;

        pc = addr;
      }
      break;
    case bit_op:
    {
      data = bus.cpu_read(addr);
      
      const uint16_t temp = a & data;
      set_flag(zero_cpu_fl, !(temp & 0xff));
      set_flag(negative_cpu_fl, data & cpu_flags_e::negative_cpu_fl);
      set_flag(overflow_cpu_fl, data & cpu_flags_e::overflow_cpu_fl);
      break;
    }
    case bmi_op:
      if (get_flag(negative_cpu_fl))
      {
        cycles++;
        addr = pc + addr_relative;

        // If calculated address passes page boundary.
        if ((addr & 0xff00) != (pc & 0xff00))
          cycles++;

        pc = addr;
      }
      break;
    case bne_op:
      if (!get_flag(zero_cpu_fl))
      {
        cycles++;
        addr = pc + addr_relative;

        // If calculated address passes page boundary.
        if ((addr & 0xff00) != (pc & 0xff00))
          cycles++;

        pc = addr;
      }
      break;
    case rol_op:
    {
      data = bus.cpu_read(addr);
      const uint16_t temp =
          uint16_t((data << 1) | uint16_t(get_flag(carry_cpu_fl)));
      set_flag(carry_cpu_fl, temp & 0xff00);
      set_flag(zero_cpu_fl, (temp & 0xff) == 0x00);
      set_flag(negative_cpu_fl, temp & 0x80);
      if (addr_mode == imp_addr)
        a = temp & 0xff;
      else
        bus.cpu_write(addr, temp & 0xff);
      break;
    }
    case ror_op: {
      data = bus.cpu_read(addr);
      const uint16_t temp =
          uint16_t(get_flag(carry_cpu_fl) << 7 | (data >> 1));
      set_flag(carry_cpu_fl, data & 0x1);
      set_flag(zero_cpu_fl, !(temp & 0xff));
      set_flag(negative_cpu_fl, temp & 0x80);
      if (addr_mode == imp_addr)
        a = temp & 0xff;
      else
        bus.cpu_write(addr, temp & 0xff);
      break;
    }
    case rti_op:
      status = bus.cpu_read(0x100 + sp + 1);
      set_flag(break_cpu_fl, false);
      set_flag(unused_cpu_fl, false);

      pc = uint16_t(bus.cpu_read(0x100 + 2 + sp)) |
           (uint16_t(bus.cpu_read(0x100 + 3 + sp)) << 8);
      sp += 3;
      break;
    case rts_op:
      pc = uint16_t(bus.cpu_read(0x100 + sp + 1)) |
           (uint16_t(bus.cpu_read(0x100 + sp + 2)) << 8);
      pc++;
      sp += 2;
      break;
    case ldx_op:
      data = bus.cpu_read(addr);
      x = data;
      set_flag(zero_cpu_fl, !x);
      set_flag(negative_cpu_fl, x & 0x80);
      op_additional_cycle = true;
      break;
    case lda_op:
      data = bus.cpu_read(addr);
      a = data;
      set_flag(zero_cpu_fl, !a);
      set_flag(negative_cpu_fl, a & 0x80);
      op_additional_cycle = true;
      break;
    case ldy_op:
      data = bus.cpu_read(addr);
      y = data;
      set_flag(zero_cpu_fl, !y);
      set_flag(negative_cpu_fl, y & 0x80);
      op_additional_cycle = true;
      break;
    case tax_op:
      x = a;
      set_flag(zero_cpu_fl, !x);
      set_flag(negative_cpu_fl, x & 0x80);
      break;
    case tay_op:
      y = a;
      set_flag(zero_cpu_fl, !y);
      set_flag(negative_cpu_fl, y & 0x80);
      break;
    case tsx_op:
      x = sp;
      set_flag(zero_cpu_fl, !x);
      set_flag(negative_cpu_fl, x & 0x80);
      break;
    case txa_op:
      a = x;
      set_flag(zero_cpu_fl, !a);
      set_flag(negative_cpu_fl, a & 0x80);
      break;
    case txs_op:
      sp = x;
      break;
    case tya_op:
      a = y;
      set_flag(zero_cpu_fl, !y);
      set_flag(negative_cpu_fl, y & 0x80);
      break;
    case sec_op:
      set_flag(carry_cpu_fl, true);
      break;
    case sed_op:
      set_flag(decimal_cpu_fl, true);
      break;
    case sei_op:
      set_flag(disable_interrupt_cpu_fl, true);
      break;
    case clc_op:
      set_flag(carry_cpu_fl, false);
      break;
    case cld_op:
      set_flag(decimal_cpu_fl, false);
      break;
    case cli_op:
      set_flag(disable_interrupt_cpu_fl, false);
      break;
    case clv_op:
      set_flag(overflow_cpu_fl, false);
      break;
    case sta_op:
      bus.cpu_write(addr, a);
      break;
    case stx_op:
      bus.cpu_write(addr, x);
      break;
    case sty_op:
      bus.cpu_write(addr, y);
      break;
    case dec_op:
    {
      data = bus.cpu_read(addr);
      const uint16_t temp = data - 1;
      bus.cpu_write(addr, temp & 0xff);
      set_flag(zero_cpu_fl, !(temp & 0xff));
      set_flag(negative_cpu_fl, temp & 0x80);
      break;
    }
    case dex_op:
      x--;
      set_flag(zero_cpu_fl, !x);
      set_flag(negative_cpu_fl, x & 0x80);
      break;
    case dey_op:
      y--;
      set_flag(zero_cpu_fl, !y);
      set_flag(negative_cpu_fl, y & 0x80);
      break;
    case lsr_op: 
    {
      data = bus.cpu_read(addr);
      set_flag(carry_cpu_fl, data & 0x1);

      const uint16_t temp = data >> 1;
      set_flag(zero_cpu_fl, !(temp & 0xff));
      set_flag(negative_cpu_fl, temp & 0x80);
      if (addr_mode == imp_addr)
      {
        a = temp & 0xff;
      }
      else
      {
        bus.cpu_write(addr, temp & 0xff);
      }
      break;
    }
    case jsr_op:
      pc--;
      bus.cpu_write(0x100 + sp, (pc >> 8) & 0xff);
      bus.cpu_write(0x100 + sp - 1, pc & 0xff);

      sp -= 2;
      pc = addr;
      break;
    case jmp_op:
      pc = addr;
      break;
      case inc_op:
      {
        data = bus.cpu_read(addr);
        const uint16_t temp = data + 1;
        bus.cpu_write(addr, temp & 0xff);
        set_flag(zero_cpu_fl, !(temp & 0xff));
        set_flag(negative_cpu_fl, temp & 0x80);
        break;
      }
      case inx_op:
        x++;
        set_flag(zero_cpu_fl, !x);
        set_flag(negative_cpu_fl, x & 0x80);
        break;
      case iny_op:
        y++;
        set_flag(zero_cpu_fl, !y);
        set_flag(negative_cpu_fl, y & 0x80);
        break;
      case cmp_op:
      {
        data = bus.cpu_read(addr);
        const uint16_t temp = uint16_t(a) - uint16_t(data);
        set_flag(carry_cpu_fl, a >= data);
        set_flag(zero_cpu_fl, (temp & 0xff) == 0);
        set_flag(negative_cpu_fl, temp & 0x80);
        op_additional_cycle = true;
        break;
      }
      case cpx_op:
      {
        data = bus.cpu_read(addr);
        const uint16_t temp = uint16_t(x) - uint16_t(data);
        set_flag(carry_cpu_fl, x >= data);
        set_flag(zero_cpu_fl, !(temp & 0xff));
        set_flag(negative_cpu_fl, temp & 0x80);
        break;
      }
      case cpy_op: {
        data = bus.cpu_read(addr);
        const uint16_t temp = uint16_t(y) - uint16_t(data);
        set_flag(carry_cpu_fl, y >= data);
        set_flag(zero_cpu_fl, !(temp & 0xff));
        set_flag(negative_cpu_fl, temp & 0x80);
        break;
      }
      case pha_op:
        bus.cpu_write(0x100 + sp, a);
        sp--;
        break;
      case php_op:
        bus.cpu_write(0x100 + sp, status | cpu_flags_e::break_cpu_fl |
                                      cpu_flags_e::unused_cpu_fl);
        set_flag(break_cpu_fl, false);
        set_flag(unused_cpu_fl, false);
        sp--;
        break;
      case pla_op:
        sp++;
        a = bus.cpu_read(0x100 + sp);
        set_flag(zero_cpu_fl, !a);
        set_flag(negative_cpu_fl, a & 0x80);
        break;
      case plp_op:
        sp++;
        status = bus.cpu_read(0x100 + sp);
        set_flag(unused_cpu_fl, true);
        break;
      case eor_op:
        data = bus.cpu_read(addr);
        a = a ^ data;
        set_flag(zero_cpu_fl, !a);
        set_flag(negative_cpu_fl, a & 0x80);
        op_additional_cycle = true;
        break;
      case bvc_op:
        if (!get_flag(overflow_cpu_fl))
        {
          cycles++;
          addr = pc + addr_relative;

          if ((addr & 0xff00) != (pc & 0xff00)) cycles++;
          pc = addr;
        }
        break;
      case bvs_op:
        if (get_flag(overflow_cpu_fl))
        {
          cycles++;
          addr = pc + addr_relative;

          if ((addr & 0xff00) != (pc & 0xff00))
            cycles++;
          pc = addr;
        }
        break;
      case ora_op:
        data = bus.cpu_read(addr);
        a |= data;
        set_flag(zero_cpu_fl, !a);
        set_flag(negative_cpu_fl, a & 0x80);
        op_additional_cycle = true;
        break;
      case brk_op:
        pc++;

        set_flag(disable_interrupt_cpu_fl, true);
        bus.cpu_write(0x0100 + sp, (pc >> 8) & 0x00FF);
        sp--;
        bus.cpu_write(0x0100 + sp, pc & 0x00FF);
        sp--;

        set_flag(break_cpu_fl, true);
        bus.cpu_write(0x0100 + sp, status);
        sp--;
        set_flag(break_cpu_fl, false);

        pc = uint16_t(bus.cpu_read(0xFFFE)) | (uint16_t(bus.cpu_read(0xFFFF)) << 8);
        break;
      case nop_op:
        switch (op)
        {
        case 0x1c:
        case 0x3c:
        case 0x5c:
        case 0x7c:
        case 0xdc:
        case 0xfc:
          op_additional_cycle = true;
        break;
        default:false;
          break;
        }
        break;
    case ill_op:

      break;
    default:
      cout << "unimplemented op : " + opcode_string_map[opcode_e(op)] << endl;
      throw 55;
      break;
    }
    if (op_additional_cycle)
      cycles += additional_cycle;
    set_flag(unused_cpu_fl, true);

    if (verbose)
    {
      printf("pc: %04x\n"
             "operation : %s addr{%04x} data{%02x}\n"
             "addressing mode : %s\n"
             "***********************************\n",
             pc - bank_relative_addr, opcode_string_map[opcode_e(op)].c_str(),
             addr, data, addr_string_map[addr_mode_e(addr_mode)].c_str());
    }

    //char buffer[1000];
    //sprintf_s(buffer,
    //          "pc:%04x a:%02x x:%02x y:%02x sp:%04x addr:%04x addr_rel:%02x "
    //          "data:%02x status:%02x op:%s addr:%s\n",
    //          pc, a, x, y, sp, addr, addr_relative, data, status,
    //          opcode_string_map[opcode_e(op)].c_str(),
    //          addr_string_map[addr_mode_e(addr_mode)].c_str());
    //logOut << buffer;

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
  printf("sp : %04x\n", pc);
  printf("p : %04x\n", status);
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