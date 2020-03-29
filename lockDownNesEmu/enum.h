#pragma once
/* ****************************************************************
   LockDownNesEmu Nes Emulator

   Header file cotaining various enums
   Date: 27 March 2020        Author : William Kosasih

**************************************************************** */

enum cpu_flags_e
{
  carry_cpu_fl = 1,
  zero_cpu_fl = 1 << 1,
  disable_interrupt_cpu_fl = 1 << 2,
  decimal_cpu_fl = 1 << 3,
  break_cpu_fl = 1 << 4,
  unused_cpu_fl = 1 << 5,
  overflow_cpu_fl = 1 << 6,
  negative_cpu_fl = 1 << 7
};

enum opcode_e
{
  adc_op,
  and_op,
  asl_op,
  bcc_op,
  bcs_op,
  beq_op,
  bit_op,
  bmi_op,
  bne_op,
  bpl_op,
  brk_op,
  bvc_op,
  bvs_op,
  clc_op,
  cld_op,
  cli_op,
  clv_op,
  cmp_op,
  cpx_op,
  cpy_op,
  dec_op,
  dex_op,
  dey_op,
  eor_op,
  inc_op,
  iny_op,
  jmp_op,
  jsr_op,
  lda_op,
  ldx_op,
  ldy_op,
  lsr_op,
  nop_op,
  ora_op,
  pha_op,
  php_op,
  rti_op,
  rts_op,
  sbc_op,
  sec_op,
  sed_op,
  sei_op,
  sta_op,
  stx_op,
  sty_op,
  tax_op,
  tay_op,
  tsx_op,
  tsy_op,
  tya_op,
  ror_op,
  ill_op,
  inx_op,
  txa_op,
  txs_op,
  pla_op,
  rol_op,
  plp_op
};

enum addr_mode_e
{
  imp_addr,
  imm_addr,
  abs_addr,
  zp_addr,
  zpx_addr,
  zpy_addr,
  abx_addr,
  aby_addr,
  rel_addr,
  izx_addr,
  izy_addr,
  ind_addr
};

enum cartridge_mirror_e
{
  horizontal_cartridge_mirror,
  vertical_cartridge_mirror,
  onescreenlow_cartridge_mirror,
  onescreenhigh_cartridge_mirror
};
enum component_e
{
  cpu_component,
  ppu_component
};