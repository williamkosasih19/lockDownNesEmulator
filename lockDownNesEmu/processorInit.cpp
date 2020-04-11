#include "processor.h"
#include "enum.h"

void processor_c::init()
{
  op_row_t row1 = {instr_cycle_pair(opcode_addr_pair(brk_op, imm_addr), 7),
                   instr_cycle_pair(opcode_addr_pair(ora_op, izx_addr), 6),
                   instr_cycle_pair(opcode_addr_pair(ill_op, imp_addr), 2),
                   instr_cycle_pair(opcode_addr_pair(ill_op, imp_addr), 8),
                   instr_cycle_pair(opcode_addr_pair(nop_op, imp_addr), 3),
                   instr_cycle_pair(opcode_addr_pair(ora_op, zp_addr), 3),
                   instr_cycle_pair(opcode_addr_pair(asl_op, zp_addr), 5),
                   instr_cycle_pair(opcode_addr_pair(ill_op, imp_addr), 5),
                   instr_cycle_pair(opcode_addr_pair(php_op, imp_addr), 3),
                   instr_cycle_pair(opcode_addr_pair(ora_op, imm_addr), 2),
                   instr_cycle_pair(opcode_addr_pair(asl_op, imp_addr), 2),
                   instr_cycle_pair(opcode_addr_pair(ill_op, imp_addr), 2),
                   instr_cycle_pair(opcode_addr_pair(nop_op, imp_addr), 4),
                   instr_cycle_pair(opcode_addr_pair(ora_op, abs_addr), 4),
                   instr_cycle_pair(opcode_addr_pair(asl_op, abs_addr), 6),
                   instr_cycle_pair(opcode_addr_pair(ill_op, imp_addr), 6)};

  op_row_t row2 = {instr_cycle_pair(opcode_addr_pair(bpl_op, rel_addr), 2),
                   instr_cycle_pair(opcode_addr_pair(ora_op, izy_addr), 5),
                   instr_cycle_pair(opcode_addr_pair(ill_op, imp_addr), 2),
                   instr_cycle_pair(opcode_addr_pair(ill_op, imp_addr), 8),
                   instr_cycle_pair(opcode_addr_pair(nop_op, imp_addr), 4),
                   instr_cycle_pair(opcode_addr_pair(ora_op, zpx_addr), 4),
                   instr_cycle_pair(opcode_addr_pair(asl_op, zpx_addr), 6),
                   instr_cycle_pair(opcode_addr_pair(ill_op, imp_addr), 6),
                   instr_cycle_pair(opcode_addr_pair(clc_op, imp_addr), 2),
                   instr_cycle_pair(opcode_addr_pair(ora_op, aby_addr), 4),
                   instr_cycle_pair(opcode_addr_pair(nop_op, imp_addr), 2),
                   instr_cycle_pair(opcode_addr_pair(ill_op, imp_addr), 7),
                   instr_cycle_pair(opcode_addr_pair(nop_op, imp_addr), 4),
                   instr_cycle_pair(opcode_addr_pair(ora_op, abx_addr), 4),
                   instr_cycle_pair(opcode_addr_pair(asl_op, abx_addr), 7),
                   instr_cycle_pair(opcode_addr_pair(ill_op, imp_addr), 7)};

  op_row_t row3 = {instr_cycle_pair(opcode_addr_pair(jsr_op, abs_addr), 6),
                   instr_cycle_pair(opcode_addr_pair(and_op, izx_addr), 6),
                   instr_cycle_pair(opcode_addr_pair(ill_op, imp_addr), 2),
                   instr_cycle_pair(opcode_addr_pair(ill_op, imp_addr), 8),
                   instr_cycle_pair(opcode_addr_pair(bit_op, zp_addr), 3),
                   instr_cycle_pair(opcode_addr_pair(and_op, zp_addr), 3),
                   instr_cycle_pair(opcode_addr_pair(rol_op, zp_addr), 5),
                   instr_cycle_pair(opcode_addr_pair(ill_op, imp_addr), 5),
                   instr_cycle_pair(opcode_addr_pair(plp_op, imp_addr), 4),
                   instr_cycle_pair(opcode_addr_pair(and_op, imm_addr), 2),
                   instr_cycle_pair(opcode_addr_pair(rol_op, imp_addr), 2),
                   instr_cycle_pair(opcode_addr_pair(ill_op, imp_addr), 2),
                   instr_cycle_pair(opcode_addr_pair(bit_op, abs_addr), 4),
                   instr_cycle_pair(opcode_addr_pair(and_op, abs_addr), 4),
                   instr_cycle_pair(opcode_addr_pair(rol_op, abs_addr), 6),
                   instr_cycle_pair(opcode_addr_pair(ill_op, imp_addr), 6)};

  op_row_t row4 = {instr_cycle_pair(opcode_addr_pair(bmi_op, rel_addr), 2),
                   instr_cycle_pair(opcode_addr_pair(and_op, izy_addr), 5),
                   instr_cycle_pair(opcode_addr_pair(ill_op, imp_addr), 2),
                   instr_cycle_pair(opcode_addr_pair(ill_op, imp_addr), 8),
                   instr_cycle_pair(opcode_addr_pair(nop_op, imp_addr), 4),
                   instr_cycle_pair(opcode_addr_pair(and_op, zpx_addr), 4),
                   instr_cycle_pair(opcode_addr_pair(rol_op, zpx_addr), 6),
                   instr_cycle_pair(opcode_addr_pair(ill_op, imp_addr), 6),
                   instr_cycle_pair(opcode_addr_pair(sec_op, imp_addr), 2),
                   instr_cycle_pair(opcode_addr_pair(and_op, aby_addr), 4),
                   instr_cycle_pair(opcode_addr_pair(nop_op, imp_addr), 2),
                   instr_cycle_pair(opcode_addr_pair(ill_op, imp_addr), 7),
                   instr_cycle_pair(opcode_addr_pair(nop_op, imp_addr), 4),
                   instr_cycle_pair(opcode_addr_pair(and_op, abx_addr), 4),
                   instr_cycle_pair(opcode_addr_pair(rol_op, abx_addr), 7),
                   instr_cycle_pair(opcode_addr_pair(ill_op, imp_addr), 7)};

  op_row_t row5 = {instr_cycle_pair(opcode_addr_pair(rti_op, imp_addr), 6),
                   instr_cycle_pair(opcode_addr_pair(eor_op, izx_addr), 6),
                   instr_cycle_pair(opcode_addr_pair(ill_op, imp_addr), 2),
                   instr_cycle_pair(opcode_addr_pair(ill_op, imp_addr), 8),
                   instr_cycle_pair(opcode_addr_pair(nop_op, imp_addr), 3),
                   instr_cycle_pair(opcode_addr_pair(eor_op, zp_addr), 3),
                   instr_cycle_pair(opcode_addr_pair(lsr_op, zp_addr), 5),
                   instr_cycle_pair(opcode_addr_pair(ill_op, imp_addr), 5),
                   instr_cycle_pair(opcode_addr_pair(pha_op, imp_addr), 3),
                   instr_cycle_pair(opcode_addr_pair(eor_op, imm_addr), 2),
                   instr_cycle_pair(opcode_addr_pair(lsr_op, imp_addr), 2),
                   instr_cycle_pair(opcode_addr_pair(ill_op, imp_addr), 2),
                   instr_cycle_pair(opcode_addr_pair(jmp_op, abs_addr), 3),
                   instr_cycle_pair(opcode_addr_pair(eor_op, abs_addr), 4),
                   instr_cycle_pair(opcode_addr_pair(lsr_op, abs_addr), 6),
                   instr_cycle_pair(opcode_addr_pair(ill_op, imp_addr), 6)};

  op_row_t row6 = {instr_cycle_pair(opcode_addr_pair(bvc_op, rel_addr), 2),
                   instr_cycle_pair(opcode_addr_pair(eor_op, izy_addr), 5),
                   instr_cycle_pair(opcode_addr_pair(ill_op, imp_addr), 2),
                   instr_cycle_pair(opcode_addr_pair(ill_op, imp_addr), 8),
                   instr_cycle_pair(opcode_addr_pair(nop_op, imp_addr), 4),
                   instr_cycle_pair(opcode_addr_pair(eor_op, zpx_addr), 4),
                   instr_cycle_pair(opcode_addr_pair(lsr_op, zpx_addr), 6),
                   instr_cycle_pair(opcode_addr_pair(ill_op, imp_addr), 6),
                   instr_cycle_pair(opcode_addr_pair(cli_op, imp_addr), 2),
                   instr_cycle_pair(opcode_addr_pair(eor_op, aby_addr), 4),
                   instr_cycle_pair(opcode_addr_pair(nop_op, imp_addr), 2),
                   instr_cycle_pair(opcode_addr_pair(ill_op, imp_addr), 7),
                   instr_cycle_pair(opcode_addr_pair(nop_op, imp_addr), 4),
                   instr_cycle_pair(opcode_addr_pair(eor_op, abx_addr), 4),
                   instr_cycle_pair(opcode_addr_pair(lsr_op, abx_addr), 7),
                   instr_cycle_pair(opcode_addr_pair(ill_op, imp_addr), 7)};

  op_row_t row7 = {instr_cycle_pair(opcode_addr_pair(rts_op, imp_addr), 6),
                   instr_cycle_pair(opcode_addr_pair(adc_op, izx_addr), 6),
                   instr_cycle_pair(opcode_addr_pair(ill_op, imp_addr), 2),
                   instr_cycle_pair(opcode_addr_pair(ill_op, imp_addr), 8),
                   instr_cycle_pair(opcode_addr_pair(nop_op, imp_addr), 3),
                   instr_cycle_pair(opcode_addr_pair(adc_op, zp_addr), 3),
                   instr_cycle_pair(opcode_addr_pair(ror_op, zp_addr), 5),
                   instr_cycle_pair(opcode_addr_pair(ill_op, imp_addr), 5),
                   instr_cycle_pair(opcode_addr_pair(pla_op, imp_addr), 4),
                   instr_cycle_pair(opcode_addr_pair(adc_op, imm_addr), 2),
                   instr_cycle_pair(opcode_addr_pair(ror_op, imp_addr), 2),
                   instr_cycle_pair(opcode_addr_pair(ill_op, imp_addr), 2),
                   instr_cycle_pair(opcode_addr_pair(jmp_op, ind_addr), 5),
                   instr_cycle_pair(opcode_addr_pair(adc_op, abs_addr), 4),
                   instr_cycle_pair(opcode_addr_pair(ror_op, abs_addr), 6),
                   instr_cycle_pair(opcode_addr_pair(ill_op, imp_addr), 6)};

  op_row_t row8 = {instr_cycle_pair(opcode_addr_pair(bvs_op, rel_addr), 2),
                   instr_cycle_pair(opcode_addr_pair(adc_op, izy_addr), 5),
                   instr_cycle_pair(opcode_addr_pair(ill_op, imp_addr), 2),
                   instr_cycle_pair(opcode_addr_pair(ill_op, imp_addr), 8),
                   instr_cycle_pair(opcode_addr_pair(nop_op, imp_addr), 4),
                   instr_cycle_pair(opcode_addr_pair(adc_op, zpx_addr), 4),
                   instr_cycle_pair(opcode_addr_pair(ror_op, zpx_addr), 6),
                   instr_cycle_pair(opcode_addr_pair(ill_op, imp_addr), 6),
                   instr_cycle_pair(opcode_addr_pair(sei_op, imp_addr), 2),
                   instr_cycle_pair(opcode_addr_pair(adc_op, aby_addr), 4),
                   instr_cycle_pair(opcode_addr_pair(nop_op, imp_addr), 2),
                   instr_cycle_pair(opcode_addr_pair(ill_op, imp_addr), 7),
                   instr_cycle_pair(opcode_addr_pair(nop_op, imp_addr), 4),
                   instr_cycle_pair(opcode_addr_pair(adc_op, abx_addr), 4),
                   instr_cycle_pair(opcode_addr_pair(ror_op, abx_addr), 7),
                   instr_cycle_pair(opcode_addr_pair(ill_op, imp_addr), 7)};

  op_row_t row9 = {instr_cycle_pair(opcode_addr_pair(nop_op, imp_addr), 2),
                   instr_cycle_pair(opcode_addr_pair(sta_op, izx_addr), 6),
                   instr_cycle_pair(opcode_addr_pair(nop_op, imp_addr), 2),
                   instr_cycle_pair(opcode_addr_pair(ill_op, imp_addr), 6),
                   instr_cycle_pair(opcode_addr_pair(sty_op, zp_addr), 3),
                   instr_cycle_pair(opcode_addr_pair(sta_op, zp_addr), 3),
                   instr_cycle_pair(opcode_addr_pair(stx_op, zp_addr), 3),
                   instr_cycle_pair(opcode_addr_pair(ill_op, imp_addr), 3),
                   instr_cycle_pair(opcode_addr_pair(dey_op, imp_addr), 2),
                   instr_cycle_pair(opcode_addr_pair(nop_op, imp_addr), 2),
                   instr_cycle_pair(opcode_addr_pair(txa_op, imp_addr), 2),
                   instr_cycle_pair(opcode_addr_pair(ill_op, imp_addr), 2),
                   instr_cycle_pair(opcode_addr_pair(sty_op, abs_addr), 4),
                   instr_cycle_pair(opcode_addr_pair(sta_op, abs_addr), 4),
                   instr_cycle_pair(opcode_addr_pair(stx_op, abs_addr), 4),
                   instr_cycle_pair(opcode_addr_pair(ill_op, imp_addr), 4)};

  op_row_t row10 = {instr_cycle_pair(opcode_addr_pair(bcc_op, rel_addr), 2),
                    instr_cycle_pair(opcode_addr_pair(sta_op, izy_addr), 6),
                    instr_cycle_pair(opcode_addr_pair(ill_op, imp_addr), 2),
                    instr_cycle_pair(opcode_addr_pair(ill_op, imp_addr), 6),
                    instr_cycle_pair(opcode_addr_pair(sty_op, zpx_addr), 4),
                    instr_cycle_pair(opcode_addr_pair(sta_op, zpx_addr), 4),
                    instr_cycle_pair(opcode_addr_pair(stx_op, zpy_addr), 4),
                    instr_cycle_pair(opcode_addr_pair(ill_op, imp_addr), 4),
                    instr_cycle_pair(opcode_addr_pair(tya_op, imp_addr), 2),
                    instr_cycle_pair(opcode_addr_pair(sta_op, aby_addr), 5),
                    instr_cycle_pair(opcode_addr_pair(txs_op, imp_addr), 2),
                    instr_cycle_pair(opcode_addr_pair(ill_op, imp_addr), 5),
                    instr_cycle_pair(opcode_addr_pair(nop_op, imp_addr), 5),
                    instr_cycle_pair(opcode_addr_pair(sta_op, abx_addr), 5),
                    instr_cycle_pair(opcode_addr_pair(ill_op, imp_addr), 5),
                    instr_cycle_pair(opcode_addr_pair(ill_op, imp_addr), 5)};

  op_row_t row11 = {instr_cycle_pair(opcode_addr_pair(ldy_op, imm_addr), 2),
                    instr_cycle_pair(opcode_addr_pair(lda_op, izx_addr), 6),
                    instr_cycle_pair(opcode_addr_pair(ldx_op, imm_addr), 2),
                    instr_cycle_pair(opcode_addr_pair(ill_op, imp_addr), 6),
                    instr_cycle_pair(opcode_addr_pair(ldy_op, zp_addr), 3),
                    instr_cycle_pair(opcode_addr_pair(lda_op, zp_addr), 3),
                    instr_cycle_pair(opcode_addr_pair(ldx_op, zp_addr), 3),
                    instr_cycle_pair(opcode_addr_pair(ill_op, imp_addr), 3),
                    instr_cycle_pair(opcode_addr_pair(tay_op, imp_addr), 2),
                    instr_cycle_pair(opcode_addr_pair(lda_op, imm_addr), 2),
                    instr_cycle_pair(opcode_addr_pair(tax_op, imp_addr), 2),
                    instr_cycle_pair(opcode_addr_pair(ill_op, imp_addr), 2),
                    instr_cycle_pair(opcode_addr_pair(ldy_op, abs_addr), 4),
                    instr_cycle_pair(opcode_addr_pair(lda_op, abs_addr), 4),
                    instr_cycle_pair(opcode_addr_pair(ldx_op, abs_addr), 4),
                    instr_cycle_pair(opcode_addr_pair(ill_op, imp_addr), 4)};

  op_row_t row12 = {instr_cycle_pair(opcode_addr_pair(bcs_op, rel_addr), 2),
                    instr_cycle_pair(opcode_addr_pair(lda_op, izy_addr), 5),
                    instr_cycle_pair(opcode_addr_pair(ill_op, imp_addr), 2),
                    instr_cycle_pair(opcode_addr_pair(ill_op, imp_addr), 5),
                    instr_cycle_pair(opcode_addr_pair(ldy_op, zpx_addr), 4),
                    instr_cycle_pair(opcode_addr_pair(lda_op, zpx_addr), 4),
                    instr_cycle_pair(opcode_addr_pair(ldx_op, zpy_addr), 4),
                    instr_cycle_pair(opcode_addr_pair(ill_op, imp_addr), 4),
                    instr_cycle_pair(opcode_addr_pair(clv_op, imp_addr), 2),
                    instr_cycle_pair(opcode_addr_pair(lda_op, aby_addr), 4),
                    instr_cycle_pair(opcode_addr_pair(tsx_op, imp_addr), 2),
                    instr_cycle_pair(opcode_addr_pair(ill_op, imp_addr), 4),
                    instr_cycle_pair(opcode_addr_pair(ldy_op, abx_addr), 4),
                    instr_cycle_pair(opcode_addr_pair(lda_op, abx_addr), 4),
                    instr_cycle_pair(opcode_addr_pair(ldx_op, aby_addr), 4),
                    instr_cycle_pair(opcode_addr_pair(ill_op, imp_addr), 4)};

  op_row_t row13 = {instr_cycle_pair(opcode_addr_pair(cpy_op, imm_addr), 2),
                    instr_cycle_pair(opcode_addr_pair(cmp_op, izx_addr), 6),
                    instr_cycle_pair(opcode_addr_pair(nop_op, imp_addr), 2),
                    instr_cycle_pair(opcode_addr_pair(ill_op, imp_addr), 8),
                    instr_cycle_pair(opcode_addr_pair(cpy_op, zp_addr), 3),
                    instr_cycle_pair(opcode_addr_pair(cmp_op, zp_addr), 3),
                    instr_cycle_pair(opcode_addr_pair(dec_op, zp_addr), 5),
                    instr_cycle_pair(opcode_addr_pair(ill_op, imp_addr), 5),
                    instr_cycle_pair(opcode_addr_pair(iny_op, imp_addr), 2),
                    instr_cycle_pair(opcode_addr_pair(cmp_op, imm_addr), 2),
                    instr_cycle_pair(opcode_addr_pair(dex_op, imp_addr), 2),
                    instr_cycle_pair(opcode_addr_pair(ill_op, imp_addr), 2),
                    instr_cycle_pair(opcode_addr_pair(cpy_op, abs_addr), 4),
                    instr_cycle_pair(opcode_addr_pair(cmp_op, abs_addr), 4),
                    instr_cycle_pair(opcode_addr_pair(dec_op, abs_addr), 6),
                    instr_cycle_pair(opcode_addr_pair(ill_op, imp_addr), 6)};

  op_row_t row14 = {instr_cycle_pair(opcode_addr_pair(bne_op, rel_addr), 2),
                    instr_cycle_pair(opcode_addr_pair(cmp_op, izy_addr), 5),
                    instr_cycle_pair(opcode_addr_pair(ill_op, imp_addr), 2),
                    instr_cycle_pair(opcode_addr_pair(ill_op, imp_addr), 8),
                    instr_cycle_pair(opcode_addr_pair(nop_op, imp_addr), 4),
                    instr_cycle_pair(opcode_addr_pair(cmp_op, zpx_addr), 4),
                    instr_cycle_pair(opcode_addr_pair(dec_op, zpx_addr), 6),
                    instr_cycle_pair(opcode_addr_pair(ill_op, imp_addr), 6),
                    instr_cycle_pair(opcode_addr_pair(cld_op, imp_addr), 2),
                    instr_cycle_pair(opcode_addr_pair(cmp_op, aby_addr), 4),
                    instr_cycle_pair(opcode_addr_pair(nop_op, imp_addr), 2),
                    instr_cycle_pair(opcode_addr_pair(ill_op, imp_addr), 7),
                    instr_cycle_pair(opcode_addr_pair(nop_op, imp_addr), 4),
                    instr_cycle_pair(opcode_addr_pair(cmp_op, abx_addr), 4),
                    instr_cycle_pair(opcode_addr_pair(dec_op, abx_addr), 7),
                    instr_cycle_pair(opcode_addr_pair(ill_op, imp_addr), 7)};

  op_row_t row15 = {instr_cycle_pair(opcode_addr_pair(cpx_op, imm_addr), 2),
                    instr_cycle_pair(opcode_addr_pair(sbc_op, izx_addr), 6),
                    instr_cycle_pair(opcode_addr_pair(nop_op, imp_addr), 2),
                    instr_cycle_pair(opcode_addr_pair(ill_op, imp_addr), 8),
                    instr_cycle_pair(opcode_addr_pair(cpx_op, zp_addr), 3),
                    instr_cycle_pair(opcode_addr_pair(sbc_op, zp_addr), 3),
                    instr_cycle_pair(opcode_addr_pair(inc_op, zp_addr), 5),
                    instr_cycle_pair(opcode_addr_pair(ill_op, imp_addr), 5),
                    instr_cycle_pair(opcode_addr_pair(inx_op, imp_addr), 2),
                    instr_cycle_pair(opcode_addr_pair(sbc_op, imm_addr), 2),
                    instr_cycle_pair(opcode_addr_pair(nop_op, imp_addr), 2),
                    instr_cycle_pair(opcode_addr_pair(sbc_op, imp_addr), 2),
                    instr_cycle_pair(opcode_addr_pair(cpx_op, abs_addr), 4),
                    instr_cycle_pair(opcode_addr_pair(sbc_op, abs_addr), 4),
                    instr_cycle_pair(opcode_addr_pair(inc_op, abs_addr), 6),
                    instr_cycle_pair(opcode_addr_pair(ill_op, imp_addr), 6)};

  op_row_t row16 = {instr_cycle_pair(opcode_addr_pair(beq_op, rel_addr), 2),
                    instr_cycle_pair(opcode_addr_pair(sbc_op, izy_addr), 5),
                    instr_cycle_pair(opcode_addr_pair(ill_op, imp_addr), 2),
                    instr_cycle_pair(opcode_addr_pair(ill_op, imp_addr), 8),
                    instr_cycle_pair(opcode_addr_pair(nop_op, imp_addr), 4),
                    instr_cycle_pair(opcode_addr_pair(sbc_op, zpx_addr), 4),
                    instr_cycle_pair(opcode_addr_pair(inc_op, zpx_addr), 6),
                    instr_cycle_pair(opcode_addr_pair(ill_op, imp_addr), 6),
                    instr_cycle_pair(opcode_addr_pair(sed_op, imp_addr), 2),
                    instr_cycle_pair(opcode_addr_pair(sbc_op, aby_addr), 4),
                    instr_cycle_pair(opcode_addr_pair(nop_op, imp_addr), 2),
                    instr_cycle_pair(opcode_addr_pair(ill_op, imp_addr), 7),
                    instr_cycle_pair(opcode_addr_pair(nop_op, imp_addr), 4),
                    instr_cycle_pair(opcode_addr_pair(sbc_op, abx_addr), 4),
                    instr_cycle_pair(opcode_addr_pair(inc_op, abx_addr), 7),
                    instr_cycle_pair(opcode_addr_pair(ill_op, imp_addr), 7)};

  op_table = {row1, row2,  row3,  row4,  row5,  row6,  row7,  row8,
              row9, row10, row11, row12, row13, row14, row15, row16};

  opcode_string_map[adc_op] = "ADC";
  opcode_string_map[and_op] = "AND";
  opcode_string_map[asl_op] = "ASL";
  opcode_string_map[bcc_op] = "BCC";
  opcode_string_map[bcs_op] = "BCS";
  opcode_string_map[beq_op] = "BEQ";
  opcode_string_map[bit_op] = "BIT";
  opcode_string_map[bmi_op] = "BMI";
  opcode_string_map[bne_op] = "BNE";
  opcode_string_map[bpl_op] = "BPL";
  opcode_string_map[brk_op] = "BRK";
  opcode_string_map[bvc_op] = "BVC";
  opcode_string_map[bvs_op] = "BVS";
  opcode_string_map[clc_op] = "CLC";
  opcode_string_map[cld_op] = "CLD";
  opcode_string_map[cli_op] = "CLI";
  opcode_string_map[clv_op] = "CLV";
  opcode_string_map[cmp_op] = "CMP";
  opcode_string_map[cpx_op] = "CPX";
  opcode_string_map[cpy_op] = "CPY";
  opcode_string_map[dec_op] = "DEC";
  opcode_string_map[dex_op] = "DEX";
  opcode_string_map[dey_op] = "DEY";
  opcode_string_map[eor_op] = "EOR";
  opcode_string_map[inc_op] = "INC";
  opcode_string_map[iny_op] = "INY";
  opcode_string_map[jmp_op] = "JMP";
  opcode_string_map[jsr_op] = "JSR";
  opcode_string_map[lda_op] = "LDA";
  opcode_string_map[ldx_op] = "LDX";
  opcode_string_map[ldy_op] = "LDY";
  opcode_string_map[lsr_op] = "LSR";
  opcode_string_map[nop_op] = "NOP";
  opcode_string_map[ora_op] = "ORA";
  opcode_string_map[pha_op] = "PHA";
  opcode_string_map[php_op] = "PHP";
  opcode_string_map[rti_op] = "RTI";
  opcode_string_map[rts_op] = "RTS";
  opcode_string_map[sbc_op] = "SBC";
  opcode_string_map[sec_op] = "SEC";
  opcode_string_map[sed_op] = "SED";
  opcode_string_map[sei_op] = "SEI";
  opcode_string_map[sta_op] = "STA";
  opcode_string_map[stx_op] = "STX";
  opcode_string_map[sty_op] = "STY";
  opcode_string_map[tax_op] = "TAX";
  opcode_string_map[tay_op] = "TAY";
  opcode_string_map[tsx_op] = "TSX";
  opcode_string_map[tsy_op] = "TSY";
  opcode_string_map[tya_op] = "TYA";
  opcode_string_map[ror_op] = "ROR";
  opcode_string_map[ill_op] = "???";
  opcode_string_map[inx_op] = "INX";
  opcode_string_map[txa_op] = "TXA";
  opcode_string_map[txs_op] = "TXS";
  opcode_string_map[pla_op] = "PLA";
  opcode_string_map[rol_op] = "ROL";
  opcode_string_map[plp_op] = "PLP";

  addr_string_map[imp_addr] = "IMP";
  addr_string_map[imm_addr] = "IMM";
  addr_string_map[abs_addr] = "ABS";
  addr_string_map[zp_addr] = "ZP";
  addr_string_map[zpx_addr] = "ZPX";
  addr_string_map[zpy_addr] = "ZPY";
  addr_string_map[abx_addr] = "ABX";
  addr_string_map[aby_addr] = "ABY";
  addr_string_map[rel_addr] = "REL";
  addr_string_map[izx_addr] = "IZX";
  addr_string_map[izy_addr] = "IZY";
  addr_string_map[ind_addr] = "IND";

}
