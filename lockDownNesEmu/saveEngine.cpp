#include <fstream>
#include <string>
#include <cstdio>

using namespace std;

#include "bus.h"
#include "processor.h"
#include "ppu.h"
#include "cartridge.h"

template<typename T> 
static inline void save_write(const T& object, ofstream& out_file)
{
  const uint32_t size = sizeof(object);
  out_file.write((byte_t*)&size, sizeof(uint32_t));
  out_file.write((byte_t*)&object, size);
}

template <typename T>
static inline void save_load(T*& object, ifstream& in_file)
{
  uint32_t save_size;

  const uint32_t current_size = sizeof(*object);
  in_file.read((byte_t*)&save_size, sizeof(uint32_t));

  if (object == nullptr)
    object = (T*)malloc(save_size);
  else if (current_size < save_size)
    object = (T*)realloc(object, save_size);

  T* temp_ptr = (T*)malloc(save_size);
  in_file.read((byte_t*)temp_ptr, save_size);
  memcpy((byte_t*)object, (byte_t*)temp_ptr, save_size);
  free(temp_ptr);
}

void save_file(const cartridge_c* cartridge,
  const processor_c& processor,
  const ppu_c& ppu,
  const bus_c& bus,
  const string& file_name)
{
  ofstream out_file(file_name);
  save_write(cartridge, out_file);
  save_write(*(cartridge->mapper_ptr), out_file);
  save_write(ppu, out_file);
  save_write(bus, out_file);
  save_write(processor, out_file);
  out_file.close();
}

void load_file(cartridge_c*& cartridge, processor_c*& processor,
               ppu_c*& ppu, bus_c*& bus, const string& file_name)
{
  ifstream in_file(file_name);
  save_load(cartridge, in_file);
  save_load(cartridge->mapper_ptr, in_file);
  save_load(ppu, in_file);
  save_load(bus, in_file);
  save_load(processor, in_file);
  in_file.close();
}