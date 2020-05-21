#include "ppu.h"

#include <intrin.h>

#include <queue>

uint32_t pixel_s::to_rgba()
{
  return (uint32_t(128) << 24 | (uint32_t(r) << 16) | (uint32_t(g) << 8) | uint32_t(b));
}

ppu_c::ppu_c(std::array<uint32_t, 256 * 240>& vid, cartridge_c& cart)
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

  fine_x = 0;
  load_low_byte = false;
  data_buffer = 0;
  next.bg_tile_id = 0;
  next.bg_tile_attribute = 0;
  next.bg_tile_lsb = 0;
  next.bg_tile_msb = 0;
  bg_lsb_pattern_shifter = 0;
  bg_msb_pattern_shifter = 0;
  status.value = 0;
  control.value = 0;
  vram.value = 0;
  temp_vram.value = 0;
}

uint8_t ppu_c::cpu_read(const uint16_t address)
{
  uint8_t data = 0x00;;
  switch (address)
  {
  case 0x00:
  case 0x01:
    break;
  case 0x02:
    data = status.value;
    status.vblank = 0;
    load_low_byte = false;
    break;
  case 0x03:
    break;
  case 0x04:
    data = ((byte_t*)oam_memory.data())[address];
    break;
  case 0x05:
  case 0x06:
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
    break;
  case 0x04:
    ((byte_t*)oam_memory.data())[address] = data;
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
      temp_vram.value = (uint16_t(data & 0x3f) << 8) | (temp_vram.value & 0xff);
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
    const uint16_t remm = (address &= 0xfff) & 0x3ff;
    if (cartridge.mirror == vertical_cartridge_mirror)
    {
      switch (address / 0x400)
      {
      case 0:
      case 2:
        data = name_table[0][remm];
        break;
      default:
        data = name_table[1][remm];
        break;
      }
    }
    else if (cartridge.mirror == horizontal_cartridge_mirror)
    {
      switch (address / 0x400)
      {
      case 0:
      case 1:
        data = name_table[0][remm];
        break;
      default:
        data = name_table[1][remm];
        break;
      }
    }
  }
  else if (address >= 0x3f00 && address <= 0x3fff)
  {
    switch ((address &= 0x1f))
    {
    case 0x10:
    case 0x14:
    case 0x18:
    case 0x1c:
      address -= 0x10;
      break;
    }
    data = palette_memory[address];
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
    const uint16_t remm = (address &= 0xfff) & 0x3ff;
    if (cartridge.mirror == vertical_cartridge_mirror)
    {
      switch (address / 0x400)
      {
      case 0:
      case 2:
        name_table[0][remm] = data;
        break;
      default:
        name_table[1][remm] = data;
        break;
      }
    }
    else if (cartridge.mirror == horizontal_cartridge_mirror)
    {
      switch (address / 0x400)
      {
      case 0:
      case 1:
        name_table[0][remm] = data;
        break;
      default:
        name_table[1][remm] = data;
        break;
      }
    }
  }
  else if (address >= 0x3f00 && address <= 0x3fff)
  {
    switch ((address &= 0x1f))
    {
    case 0x10:
    case 0x14:
    case 0x18:
    case 0x1c:
      address -= 0x10;
      break;
    }
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
    bg_attribute = next.bg_tile_attribute;
  };

  if (scanline >= -1 && scanline < 240)
  {
    if (scanline == -1 && cycle == 1)
      status.vblank = 0;
    if (scanline == 0 && cycle == 0)
      cycle++;
    if ((cycle > 1 && cycle < 258) || (cycle > 320 && cycle < 338))
    {
      // Upadte the shift register.
      if (mask.render_background)
      {
        bg_lsb_pattern_shifter = bg_lsb_pattern_shifter << 1;
        bg_msb_pattern_shifter = bg_msb_pattern_shifter << 1;
      }
      for (uint8_t i = 0; i < scanline_sprites.size(); i++)
      {
        if (scanline_sprites[i].x == 0)
        {
          sprite_shifter_lsb[i] <<= 1;
          sprite_shifter_msb[i] <<= 1;
        }
        scanline_sprites[i].x--;
      }

      switch ((cycle - 1) % 8)
      {
      case 0:
        //if (vram.coarse_x == 2 && vram.coarse_y == 4 && vram.nametable_x == 1 && vram.fine_y == 0)
        //  __debugbreak();
        // Load the next 8 bits worth of data to the shifters.
        load_bg_shifter();

        next.bg_tile_id = ppu_read(0x2000 | (vram.value & 0xfff));
        break;
      case 2:
        next.bg_tile_attribute = ppu_read(
            0x23c0 | (vram.nametable_y << 11) | (vram.nametable_x << 10) |
            ((vram.coarse_y >> 2) << 3) | (vram.coarse_x >> 2));
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
                     (uint16_t(next.bg_tile_id) << 4) + 8 + vram.fine_y);
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

    if (cycle == 256 && (mask.render_background || mask.render_sprites))
    {
      if (vram.fine_y == 7)
      {
        vram.fine_y = 0;
        if (vram.coarse_y == 29)
        {
          vram.coarse_y = 0;
          vram.nametable_y = !vram.nametable_y;
        }
        // Don't flip the name table if coarse_y is in the attribute area.
        else if (vram.coarse_y == 31)
          vram.coarse_y = 0;
        else
          vram.coarse_y++;
      }
      else
        vram.fine_y++;
    }

    if (cycle == 257)
    {
      load_bg_shifter();
      if (mask.render_background || mask.render_sprites)
      {
        vram.nametable_x = temp_vram.nametable_x;
        vram.coarse_x = temp_vram.coarse_x;
      }
      if (scanline >= -1)
      {
        scanline_sprites.clear();
        for (uint8_t i = 0;
             i < oam_memory.size() && scanline_sprites.size() < 9; i++)
        {
          const uint16_t diff = uint16_t(scanline) - oam_memory[i].y;
          if (diff < (control.sprite_size ? 16 : 8))
          {
            if (i == 0)
            {
              // sprite zero hit possible
            }
            scanline_sprites.push_back(oam_memory[i]);
          }
          if (scanline_sprites.size() == 8)
          {
            status.sprite_overflow = true;
            break;
          }
        }
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
  }

  if (scanline == 241 && cycle == 1)
  {
    status.vblank = 1;
    if (control.enable_nmi) nmi=true;
  }
  
  if (scanline >= 0 && scanline < 240 && cycle >= 0 &&
      cycle < 256)
  {
    uint8_t bg_pixel;
    uint8_t fg_pixel;
    uint8_t fg_priority;
    uint8_t fg_pallete;

    if (mask.render_background)
    {
      const uint16_t bit_selector = 0x8000 >> fine_x;
      bg_pixel = (uint8_t(bool_t(bg_lsb_pattern_shifter & bit_selector)) |
         uint8_t(bool_t(bg_msb_pattern_shifter & bit_selector)) << 1);
    }
    if (mask.render_sprites)
    {
      for (uint8_t i = 0; i < scanline_sprites.size(); i++)
      {
        if (scanline_sprites[i].x == 0)
        {
          bool_t sprite_msb = bool_t(sprite_shifter_msb[i] & 0x80);
          bool_t sprite_lsb = bool_t(sprite_shifter_lsb[i] & 0x80);
          fg_pixel = (uint8_t(sprite_msb) << 1) | uint8_t(sprite_lsb);
          fg_pallete = (scanline_sprites[i].attribute & 0x03) + 4;
          fg_priority = !(scanline_sprites[i].attribute & 0x20);

          if (fg_pixel)
          {
            if (i==0)
            {
              // sprite zero rendered
            }
            break;
          }
        }
      }
    }

    vidmem[cycle + uint32_t(scanline) * 256] =
        palette_table[ppu_read(0x3f00 + (bg_attribute << 2) + bg_pixel) & 0x3f]
            .to_rgba(); 
  }

  if (cycle >= 340)
  {
    cycle = 0;
    if (scanline >= 260)
    {
      scanline = -1;
      frame_complete = true;
    }
    else scanline++;
  }
  else cycle++;
}
