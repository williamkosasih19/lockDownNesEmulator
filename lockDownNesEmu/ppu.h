#pragma once
/* ****************************************************************
   LockDownNesEmu Nes Emulator

   Header file for the ppu (picture processing unit)
   Date: 2 April 2020        Author : William Kosasih

**************************************************************** */

#include "enum.h"
#include "cartridge.h"

#include <array>

struct pixel_s
{
  uint8_t r, g, b;
  pixel_s(){};
  pixel_s(const uint8_t r, const uint8_t g, const uint8_t b)
      : r(r), g(g), b(b){};
  uint32_t to_rgba();
};

class ppu_c
{
public:
  ppu_c(std::array<uint32_t, 256 * 240>&, cartridge_c&);
  void clock();

  uint8_t& ppu_access(uint16_t address);

  void cpu_write(uint16_t address, const uint8_t data);
  uint8_t cpu_read(uint16_t address);

  bool_t nmi;
  bool_t frame_complete;

private:
  std::array<std::array<uint8_t, 1024>, 2> name_table;
  std::array<std::array<uint8_t, 4096>, 2> pattern_table;
  std::array<pixel_s, 0x40> palette_table;

  std::array<uint8_t, 32> palette_memory;

  std::array<uint32_t, 256 * 240>& vidmem;
  cartridge_c& cartridge;

  int16_t cycle, scanline;

  union
  {
    struct
    {
      uint8_t name_table_x : 1;
      uint8_t name_table_y : 1;
      uint8_t vram_increment_mode : 1;
      uint8_t sprite_pattern_table : 1;
      uint8_t background_pattern_table : 1;
      uint8_t sprite_size : 1;
      uint8_t slave_mode : 1;
      uint8_t enable_nmi : 1;
    };
    uint8_t value = 0x0;
  } control;

  union
  {
    struct
    {
      uint8_t junk : 5;
      uint8_t sprite_overflow : 1;
      uint8_t sprite_zero_hit : 1;
      uint8_t vblank : 1;
    };
    uint8_t value = 0x0;
  } status;

  union
  {
    struct
    {
      uint8_t grayscale : 1;
      uint8_t render_background_left : 1;
      uint8_t render_sprites_left : 1;
      uint8_t render_background : 1;
      uint8_t render_sprites : 1;
      uint8_t enhance_red : 1;
      uint8_t enhance_green : 1;
      uint8_t enhance_blue : 1;
    };
    uint8_t value = 0x0;
  } mask;

  union loopy_register
  {
    struct
    {
      uint16_t coarse_x : 5;
      uint16_t coarse_y : 5;
      uint16_t nametable_x : 1;
      uint16_t nametable_y : 1;
      uint16_t fine_y : 3;
      uint16_t unused : 1;
    };
    uint16_t value = 0x0;
  };

  uint8_t fine_x;

  loopy_register temp_vram;
  loopy_register vram;

  bool_t load_high_byte;

  struct
  {
    uint8_t bg_tile_id;
    uint8_t bg_tile_attribute;
    uint8_t bg_tile_lsb;
    uint8_t bg_tile_msb;
  } next;

  uint16_t bg_lsb_pattern_shifter;
  uint16_t bg_msb_pattern_shifter;
  uint16_t bg_lsb_attribute_shifter;
  uint16_t bg_msb_attribute_shifter;
};