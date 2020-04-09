#include "ppu.h"

uint32_t pixel_s::to_rgba()
{
  return (uint32_t(255) << 24 | (uint32_t(r) << 16) | (uint32_t(g) << 8) | uint32_t(b));
}

ppu_c::ppu_c(std::array<uint32_t, 340 * 260>& vid, cartridge_c& cart)
    : vidmem(vid), cartridge(cart)
{
  palette_table[0x00] = pixel_s(84, 84, 84);
  palette_table[0x01] = pixel_s(0, 30, 116);
  palette_table[0x02] = pixel_s(8, 16, 144);
  palette_table[0x03] = pixel_s(48, 0, 136);
  palette_table[0x04] = pixel_s(68, 0, 100);
  palette_table[0x05] = pixel_s(92, 0, 48);
  palette_table[0x06] = pixel_s(84, 4, 0);
  palette_table[0x07] = pixel_s(60, 24, 0);
  palette_table[0x08] = pixel_s(32, 42, 0);
  palette_table[0x09] = pixel_s(8, 58, 0);
  palette_table[0x0A] = pixel_s(0, 64, 0);
  palette_table[0x0B] = pixel_s(0, 60, 0);
  palette_table[0x0C] = pixel_s(0, 50, 60);
  palette_table[0x0D] = pixel_s(0, 0, 0);
  palette_table[0x0E] = pixel_s(0, 0, 0);
  palette_table[0x0F] = pixel_s(0, 0, 0);

  palette_table[0x10] = pixel_s(152, 150, 152);
  palette_table[0x11] = pixel_s(8, 76, 196);
  palette_table[0x12] = pixel_s(48, 50, 236);
  palette_table[0x13] = pixel_s(92, 30, 228);
  palette_table[0x14] = pixel_s(136, 20, 176);
  palette_table[0x15] = pixel_s(160, 20, 100);
  palette_table[0x16] = pixel_s(152, 34, 32);
  palette_table[0x17] = pixel_s(120, 60, 0);
  palette_table[0x18] = pixel_s(84, 90, 0);
  palette_table[0x19] = pixel_s(40, 114, 0);
  palette_table[0x1A] = pixel_s(8, 124, 0);
  palette_table[0x1B] = pixel_s(0, 118, 40);
  palette_table[0x1C] = pixel_s(0, 102, 120);
  palette_table[0x1D] = pixel_s(0, 0, 0);
  palette_table[0x1E] = pixel_s(0, 0, 0);
  palette_table[0x1F] = pixel_s(0, 0, 0);

  palette_table[0x20] = pixel_s(236, 238, 236);
  palette_table[0x21] = pixel_s(76, 154, 236);
  palette_table[0x22] = pixel_s(120, 124, 236);
  palette_table[0x23] = pixel_s(176, 98, 236);
  palette_table[0x24] = pixel_s(228, 84, 236);
  palette_table[0x25] = pixel_s(236, 88, 180);
  palette_table[0x26] = pixel_s(236, 106, 100);
  palette_table[0x27] = pixel_s(212, 136, 32);
  palette_table[0x28] = pixel_s(160, 170, 0);
  palette_table[0x29] = pixel_s(116, 196, 0);
  palette_table[0x2A] = pixel_s(76, 208, 32);
  palette_table[0x2B] = pixel_s(56, 204, 108);
  palette_table[0x2C] = pixel_s(56, 180, 204);
  palette_table[0x2D] = pixel_s(60, 60, 60);
  palette_table[0x2E] = pixel_s(0, 0, 0);
  palette_table[0x2F] = pixel_s(0, 0, 0);

  palette_table[0x30] = pixel_s(236, 238, 236);
  palette_table[0x31] = pixel_s(168, 204, 236);
  palette_table[0x32] = pixel_s(188, 188, 236);
  palette_table[0x33] = pixel_s(212, 178, 236);
  palette_table[0x34] = pixel_s(236, 174, 236);
  palette_table[0x35] = pixel_s(236, 174, 212);
  palette_table[0x36] = pixel_s(236, 180, 176);
  palette_table[0x37] = pixel_s(228, 196, 144);
  palette_table[0x38] = pixel_s(204, 210, 120);
  palette_table[0x39] = pixel_s(180, 222, 120);
  palette_table[0x3A] = pixel_s(168, 226, 144);
  palette_table[0x3B] = pixel_s(152, 226, 180);
  palette_table[0x3C] = pixel_s(160, 214, 228);
  palette_table[0x3D] = pixel_s(160, 162, 160);
  palette_table[0x3E] = pixel_s(0, 0, 0);
  palette_table[0x3F] = pixel_s(0, 0, 0);

  cycle = 0;
  scanline = 0;
}

uint8_t ppu_c::cpu_read(const uint16_t address)
{
  uint8_t data = 0x00;;
  switch (address)
  {
  case 0x00:
  case 0x01:
  case 0x03:
  case 0x04:
  case 0x05:
  case 0x06:
    break;
  case 0x02:
    data = (status.value & 0xe0) | (data_buffer & 0x1f);
    status.vblank = 0;
    load_low_byte = false;
    break;
  case 0x07:
    data = data_buffer;
    data_buffer = ppu_read(vram.value);
    if (vram.value >= 0x3f00) data = data_buffer;
    vram.value += (control.vram_increment_mode ? 32 : 1);
    break;
  }
  return data;
}

void ppu_c::cpu_write(const uint16_t address, const uint8_t data)
{
  switch (address)
  {
  case 0x00:
    control.value = data;
    temp_vram.nametable_x = control.name_table_x;
    temp_vram.nametable_y = control.name_table_y;
    break;
  case 0x01:
    mask.value = data;
    break;
  case 0x02:
  case 0x03:
  case 0x04:
    break;
  case 0x05:
    if (load_low_byte)
    {
      temp_vram.fine_y = data & 0x07;
      temp_vram.coarse_y = data >> 3;
      load_low_byte = false;
    }
    else
    {
      fine_x = data & 0x07;
      temp_vram.coarse_x = data >> 3;
      load_low_byte = true;
    }
  case 0x06:
    if (load_low_byte)
    {
      temp_vram.value = (temp_vram.value & 0xff00) | data;
      vram = temp_vram;
      load_low_byte = false;
    }
    else
    {
      temp_vram.value = uint16_t(data & 0x3f) << 8 | (temp_vram.value & 0xff);
      load_low_byte = true;
    }
    break;
    case 0x07:
    ppu_write(vram.value, data);
    vram.value += (control.vram_increment_mode ? 32 : 1);
      break;
  }
}

uint8_t ppu_c::ppu_read(uint16_t address)
{
  uint8_t data = 0x00;
  address &= 0x3fff;
  if (cartridge.ppu_read(address, data))
  {

  }
  else if (address >= 0x00 && address <= 0x1fff)
  {
    data = pattern_table[(address & 0x1000) >> 12][address & 0xfff];
  }
  else if (address >= 0x2000 && address <= 0x3eff)
  {
    address &= 0x0fff;
    if (cartridge.mirror == vertical_cartridge_mirror)
    {
      if (address >= 0x00 && address <= 0x3ff)
        data = name_table[0][address & 0x3ff];
      else if (address >= 0x400 && address <= 0x7ff)
        data = name_table[1][address & 0x3ff];
      else if (address >= 0x800 && address <= 0xbff)
        data = name_table[0][address & 0x3ff];
      else if (address >= 0xc00 && address <= 0xfff)
        data = name_table[1][address & 0x3ff];
    }
    else if (cartridge.mirror == horizontal_cartridge_mirror)
    {
      if (address >= 0x00 && address <= 0x3ff)
        data = name_table[0][address & 0x3ff];
      else if (address >= 0x400 && address <= 0x7ff)
        data = name_table[0][address & 0x3ff];
      else if (address >= 0x800 && address <= 0xbff)
        data = name_table[1][address & 0x3ff];
      else if (address >= 0xc00 && address <= 0xfff)
        data = name_table[1][address & 0x3ff];
    }
  }
  else if (address >= 0x3f00 && address <= 0x3fff)
  {
    address &= 0x1f;
	if (address == 0x0010) address = 0x0000;
	if (address == 0x0014) address = 0x0004;
	if (address == 0x0018) address = 0x0008;
	if (address == 0x001C) address = 0x000C;
	data = palette_memory[address] & (mask.grayscale ? 0x30 : 0x3F);
  }
  
  return data;
}

void ppu_c::ppu_write(uint16_t address, const uint8_t data)
{
  address &= 0x3fff;
  if (cartridge.ppu_write(address, data))
  {
  }
  else if (address >= 0x00 && address <= 0x1fff)
  {
    pattern_table[(address & 0x1000) >> 12][address & 0xfff] = data;
  }
  else if (address >= 0x2000 && address <= 0x3eff)
  {
    address &= 0x0fff;
    if (cartridge.mirror == vertical_cartridge_mirror)
    {
      if (address >= 0x00 && address <= 0x3ff)
        name_table[0][address & 0x3ff] = data;
      else if (address >= 0x400 && address <= 0x7ff)
        name_table[1][address & 0x3ff] = data;
      else if (address >= 0x800 && address <= 0xbff)
        name_table[0][address & 0x3ff] = data;
      else if (address >= 0xc00 && address <= 0xfff)
        name_table[1][address & 0x3ff] = data;
    }
    else if (cartridge.mirror == horizontal_cartridge_mirror)
    {
      if (address >= 0x00 && address <= 0x3ff)
        name_table[0][address & 0x3ff] = data;
      else if (address >= 0x400 && address <= 0x7ff)
        name_table[0][address & 0x3ff] = data;
      else if (address >= 0x800 && address <= 0xbff)
        name_table[1][address & 0x3ff] = data;
      else if (address >= 0xc00 && address <= 0xfff)
        name_table[1][address & 0x3ff] = data;
    }
  }
  else if (address >= 0x3f00 && address <= 0x3fff)
  {
    address &= 0x1f;
    if (address == 0x0010)
      address = 0x0000;
    if (address == 0x0014)
      address = 0x0004;
    if (address == 0x0018)
      address = 0x0008;
    if (address == 0x001C)
      address = 0x000C;
    palette_memory[address] = data;
  }

  return;
}

void ppu_c::clock()
{
  auto load_bg_shifter = [&]() {
    bg_lsb_pattern_shifter =
        (bg_lsb_pattern_shifter & 0xff00) | next.bg_tile_lsb;
    bg_msb_pattern_shifter =
        (bg_msb_pattern_shifter & 0xff00) | next.bg_tile_msb;
    bg_lsb_attribute_shifter = (bg_lsb_attribute_shifter & 0xff00) |
                               ((next.bg_tile_attribute & 0x1) ? 0xFF : 0x00);
    bg_lsb_attribute_shifter = (bg_lsb_attribute_shifter & 0xff00) |
                               ((next.bg_tile_attribute & 0x2) ? 0xFF : 0x00);
  };

  if (scanline >= -1 && scanline < 240)
  {
    if (scanline == -1 && cycle == 1)
      status.vblank = 0;
    else if (scanline == 0 && cycle == 0)
      cycle++;
    else if ((cycle > 1 && cycle < 258) || (cycle > 320 && cycle < 338))
    {
      // Upadte the shift register.
      if (mask.render_background)
      {
        bg_lsb_pattern_shifter = bg_lsb_pattern_shifter << 1;
        bg_msb_pattern_shifter = bg_msb_pattern_shifter << 1;
        bg_lsb_pattern_shifter = bg_lsb_pattern_shifter << 1;
        bg_msb_pattern_shifter = bg_msb_pattern_shifter << 1;
      }

      switch ((cycle) % 8)
      {
      case 0:
        // Load the next 8 bits worth of data to the shifters.
        load_bg_shifter();

        next.bg_tile_id = ppu_read(0x2000 | (vram.value & 0xfff));
        break;
      case 2:
        next.bg_tile_attribute = ppu_read(
            0x23c0 | (vram.nametable_y << 11) | (vram.nametable_x << 10) |
            ((vram.coarse_x >> 2) << 3) | (vram.coarse_y >> 2));
        if (vram.coarse_y & 0x2)
          next.bg_tile_attribute = next.bg_tile_attribute >> 4;
        if (vram.coarse_x & 0x2)
          next.bg_tile_attribute = next.bg_tile_attribute >> 2;
        next.bg_tile_attribute &= 0x03;
        break;
      case 4:
        next.bg_tile_lsb =
            ppu_read((control.background_pattern_table << 12) +
                     (uint16_t(next.bg_tile_id) << 4) + vram.fine_y);
        break;
      case 6:
        next.bg_tile_msb =
            ppu_read((control.background_pattern_table << 12) +
                     (uint16_t(next.bg_tile_id) << 4) + vram.fine_y + 8);
        break;
      case 7:
        // Increment scroll x.
        if (mask.render_background || mask.render_sprites)
        {
          if (vram.coarse_x == 31)
          {
            vram.coarse_x = 0;
            vram.nametable_x = !vram.nametable_x;
          }
          else
            vram.coarse_x++;
        }
        break;
      }
    }
  }


  if (cycle == 256 && (mask.render_background || mask.render_sprites))
  {
    if (vram.fine_y == 8)
    {
      vram.fine_y = 0;
      if (vram.coarse_y == 29)
      {
        vram.coarse_y = 0;
        vram.nametable_y = !vram.nametable_y;
      }
      else if (vram.coarse_y == 31)
        vram.coarse_y = 0;
      else
      {
        vram.coarse_y++;
      }
    }
    else
      vram.fine_y++;
  }

  if (cycle == 257)
  {
    load_bg_shifter();
    if (mask.render_background || mask.render_sprites)
    {
      vram.nametable_y = temp_vram.nametable_x;
      vram.coarse_x = temp_vram.coarse_x;
    }
  }

  if (cycle == 338 || cycle == 340)
  {
    next.bg_tile_id = ppu_read(0x2000 | (vram.value & 0xfff));
  }

  if (scanline == -1 && cycle >= 280 && cycle < 305)
  {
    if (mask.render_background || mask.render_sprites)
    {
      vram.fine_y = temp_vram.fine_y;
      vram.nametable_y = temp_vram.nametable_y;
      vram.coarse_y = temp_vram.coarse_y;
    }
  }

  if (scanline == 241 && cycle == 1)
  {
    status.vblank = 1;
    if (control.enable_nmi) nmi=true;
  }
  
  uint8_t bg_pixel = 0x00, bg_palette = 0x00;

  if (mask.render_background)
  {
    const uint16_t bit_selector = 0x8000 >> fine_x;
    bg_pixel = (uint8_t(bool_t(bg_lsb_pattern_shifter & bit_selector)) |
                uint8_t(bool_t(bg_msb_pattern_shifter & bit_selector)) << 1);
    bg_palette = (uint8_t(bool_t(bg_lsb_attribute_shifter & bit_selector)) |
                  uint8_t(bool_t(bg_msb_attribute_shifter & bit_selector)) << 1);
  }

  if (scanline >= 0)
    vidmem[cycle + scanline * 340] = palette_table[ppu_read(0x3f00 + (bg_palette << 2) + bg_pixel) & 0x3f].to_rgba(); 

  cycle++;
  if (cycle >= 340)
  {
    cycle = 0;
    scanline++;
    if (scanline >= 260)
    {
      scanline = -1;
      frame_complete = true;
    }
  }
}
