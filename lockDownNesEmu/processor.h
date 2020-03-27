#pragma once
/* ****************************************************************
   LockDownNesEmu Nes Emulator

   Header file for the 6502 processor class
   Date: 27 March 2020        Author : William Kosasih

**************************************************************** */

#include <cstdint>
#include <array>
#include <unordered_map>

#include "bus.h"
#include "types.h"
#include "enum.h"

class processor{
private:
  using opcode_addr_pair = std::pair<opcode_e, addr_mode_e>;
  using instr_cycle_pair = std::pair<opcode_addr_pair, uint16_t>;
  using op_row_t = std::array<instr_cycle_pair, 16>;
  using op_table_t = std::array<op_row_t, 16>;

  void set_flag(const cpu_flags_e, const bool_t value);
  bool_t get_flag(const cpu_flags_e);

  uint8_t a, x, y, sp, instruction, data, status;
  uint16_t pc, addr;
  uint64_t clock_count;
  bus& main_bus;
  op_table_t op_table;
  std::unordered_map<uint8_t, uint8_t> shift_amount_map;

  public:
  processor(bus&);
  void init();
  void reset();
  void execute(number32_t instructions);
};