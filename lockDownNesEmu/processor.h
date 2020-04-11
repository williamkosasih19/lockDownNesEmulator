#pragma once
/* ****************************************************************
   LockDownNesEmu Nes Emulator

   Header file for the 6502 processor class
   Date: 27 March 2020        Author : William Kosasih

**************************************************************** */

#include <string>
#include <cstdint>
#include <array>
#include <unordered_map>

#include "types.h"
#include "enum.h"

#include <fstream>

class bus_c;

class processor_c{
private:
  using opcode_addr_pair = std::pair<opcode_e, addr_mode_e>;
  using instr_cycle_pair = std::pair<opcode_addr_pair, uint16_t>;
  using op_row_t = std::array<instr_cycle_pair, 16>;
  using op_table_t = std::array<op_row_t, 16>;

  void set_flag(const cpu_flags_e, const bool_t value);
  bool_t get_flag(const cpu_flags_e);

  uint16_t bank_relative_addr;

  uint8_t a, x, y, sp, instruction, data, status, cycles;
  uint16_t pc, addr, addr_relative;
  uint64_t clock_count;
  bus_c& bus;
  op_table_t op_table;

  // for debugging purposes.
  std::unordered_map<opcode_e, std::string> opcode_string_map;
  std::unordered_map<addr_mode_e, std::string> addr_string_map;
  uint8_t op, addr_mode;

  public:
  processor_c(bus_c&);
  void init();
  void reset();
  void execute();
  void query();

  void irq();
  void nmi();

  std::ofstream logOut;
};