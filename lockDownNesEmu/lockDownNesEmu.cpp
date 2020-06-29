// lockDownNesEmu.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <numeric>
#include <cstdlib>
#include <memory>
#include <array>
#include <deque>

#include <windows.h>

#include <sdl.h>
#undef main

#include "processor.h"
#include "bus.h"
#include "cartridge.h"
#include "ppu.h"

#include "types.h"

using namespace std;

bool_t verbose;

struct snapshot_s
{
  array<unsigned int, 256 * 240> vidmem;
  ppu_c* ppu_ptr;
  bus_c* bus_ptr;
  processor_c* processor_ptr;
  snapshot_s(const array<unsigned int, 256 * 240>& vmem, const ppu_c& ppu,
    const bus_c& bus, const processor_c& processor) { 
    vidmem = vmem;
    ppu_ptr = (ppu_c*)malloc(sizeof(ppu));
    memcpy(ppu_ptr, &ppu, sizeof(ppu));

    bus_ptr = (bus_c*)malloc(sizeof(bus));
    memcpy(bus_ptr, &bus, sizeof(bus));

    processor_ptr = (processor_c*)malloc(sizeof(processor));
    memcpy(processor_ptr, &processor, sizeof(processor));
  }
  ~snapshot_s()
  {
    free(ppu_ptr);
    free(bus_ptr);
    free(processor_ptr);
  }
};

int main()
{
  cout << "Lockdown Nes Emulator v0.02" << endl;
  cout << "    21 April 2020" << endl;
  cout << endl;
  cout << "Type help for help" << endl;
  cout << "> ready!" << endl;

  string command;
  vector<string> command_split(10);

  SDL_Event event;

  SDL_Init(SDL_INIT_VIDEO);

  SDL_Window* window =
      SDL_CreateWindow("LockDown Nes Emulator", SDL_WINDOWPOS_UNDEFINED,
                       SDL_WINDOWPOS_UNDEFINED, 1024, 768, 0);

  auto renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                                           SDL_TEXTUREACCESS_STATIC, 256, 240);

  array<unsigned int, 256 * 240> vidmem;

  for (int i = 0; i < vidmem.size(); i++)
  {
    vidmem[i] = 0;
  }

  // Component initialisation
  cartridge_c cartridge;
  ppu_c ppu(vidmem, cartridge);
  bus_c bus(cartridge, ppu);
  processor_c processor(bus);
  processor.init();
  bus.plug_in_processor(&processor);

  uint64_t cycle = 0;

  bool_t quit = false;
  bool_t static_noise = false;
  bool_t pause = false;
  uint16_t cycle_threshold = 8000;
  uint32_t backup_interval = 175000;
  uint8_t backup_threshold = 500;

  deque<snapshot_s> snapshots;

  while (true)
  {
    SDL_UpdateTexture(texture, NULL, vidmem.data(), 256 * sizeof(Uint32));

    //SDL_WaitEvent(&event);

    //switch (event.type)
    //{
    //case SDL_QUIT:
    //  quit = true;
    //  break;
    //}

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);

    const auto random = [&vidmem]() {
      for (index32_t i = 0; i < vidmem.size(); i++)
      {
        vidmem[i] = rand() % (numeric_limits<unsigned int>::max)();
      }
    };

    if (static_noise)
    {
      Sleep(100);
      random();
      continue;
    }

    cout << "< ";
    getline(cin, command);
    stringstream sstream(command);
    index32_t i = 0;
    while (sstream.good() && i < 10) sstream >> command_split[i++];
    
    if (command_split[0] == "help")
    {
    
    }
    else if(command_split[0] == "setbackupinterval")
    {
      backup_interval = stoi(command_split[1]);
    }
    else if (command_split[0] == "setcycle")
    {
      cycle_threshold = stoi(command_split[1]);
    }
    else if (command_split[0] == "load")
    {
      cartridge.load(command_split[1]);
    }
    else if (command_split[0] == "noizze")
    {
      static_noise = true;
    }
    else if (command_split[0] == "artifact")
    {
      random();
    }
    else if (command_split[0] == "cycle")
    {
      number32_t cycles = stoi(command_split[1]);
      for (index32_t i = 0; i < cycles; i++)
        processor.execute();
    }
    else if (command_split[0] == "querycpu")
    {
      processor.query();
    }
    else if (command_split[0] == "reset")
    {
      processor.reset();
    }
    else if (command_split[0] == "verbose")
    {
      if (command_split[1] == "true") verbose = true;
      else verbose = false;
    }
    else if (command_split[0] == "disablesnapshot")
    {
      backup_interval = 0;
      snapshots.clear();
    }
    else if (command_split[0] == "run")
    {
      pause = false;

      while (true)
      {
        if (backup_interval != 0 && !(cycle % backup_interval))
        {
          if (snapshots.size() == backup_threshold)
            snapshots.pop_front();
          snapshots.emplace_back(vidmem, ppu, bus, processor);
        }
        if (!(cycle % cycle_threshold))
        {
          while (SDL_PollEvent(&event))
          {
            switch (event.type)
            {
            case SDL_KEYDOWN:
              switch (event.key.keysym.sym)
              {
              case SDLK_DOWN:
                bus.controller[0] |= 0x04;
                break;
              case SDLK_UP:
                bus.controller[0] |= 0x08;
                break;
              case SDLK_LEFT:
                bus.controller[0] |= 0x02;
                break;
              case SDLK_RIGHT:
                bus.controller[0] |= 0x01;
                break;
              case SDLK_x:
                bus.controller[0] |= 0x80;
                break;
              case SDLK_z:
                bus.controller[0] |= 0x40;
                break;
              case SDLK_a:
                bus.controller[0] |= 0x20;
                break;
              case SDLK_s:
                bus.controller[0] |= 0x10;
                break;
              case SDLK_ESCAPE:
                pause = true;
                break;
              case SDLK_BACKSLASH:
                while (true)
                {
                  SDL_PollEvent(&event);
                  if (snapshots.empty()) break;
                  if (event.type == SDL_KEYUP)
                  {
                    if (event.key.keysym.sym == SDLK_BACKSLASH)
                    {
                      const auto& snapshot = snapshots.back();
                      memcpy(&ppu, snapshot.ppu_ptr, sizeof(*snapshot.ppu_ptr));
                      memcpy(&bus, snapshot.bus_ptr, sizeof(*snapshot.bus_ptr));
                      memcpy(&processor, snapshot.processor_ptr,
                             sizeof(*snapshot.processor_ptr));
                      bus.controller[0] = 0;
                      snapshots.pop_back();
                      break;
                    }
                  }
                  vidmem = snapshots.back().vidmem;
                  SDL_Delay(20);
                  snapshots.pop_back();

                SDL_UpdateTexture(texture, NULL, vidmem.data(),
                        256 * sizeof(Uint32));

                  SDL_RenderClear(renderer);
                  SDL_RenderCopy(renderer, texture, NULL, NULL);
                  SDL_RenderPresent(renderer);
                }
                break;
              }
              break;

            case SDL_KEYUP:
              switch (event.key.keysym.sym)
              {
              case SDLK_DOWN:
                bus.controller[0] &= ~(0x04);
                break;
              case SDLK_UP:
                bus.controller[0] &= ~(0x08);
                break;
              case SDLK_LEFT:
                bus.controller[0] &= ~(0x02);
                break;
              case SDLK_RIGHT:
                bus.controller[0] &= ~(0x01);
                break;
              case SDLK_x:
                bus.controller[0] &= ~(0x80);
                break;
              case SDLK_z:
                bus.controller[0] &= ~(0x40);
                break;
              case SDLK_a:
                bus.controller[0] &= ~(0x20);
                break;
              case SDLK_s:
                bus.controller[0] &= ~(0x10);
                break;
              }
              break;

            default:
              break;
            }
          }
          if (pause)
            break;

          SDL_UpdateTexture(texture, NULL, vidmem.data(), 256 * sizeof(Uint32));

          SDL_RenderClear(renderer);
          SDL_RenderCopy(renderer, texture, NULL, NULL);
          SDL_RenderPresent(renderer);
        }

        // const uint32_t first_tick = SDL_GetTicks();
        bus.clock();
        // const uint32_t second_tick = SDL_GetTicks();
        cycle++;
        // SDL_Delay(5);
      }
    }
  }

	SDL_DestroyWindow(window);
  SDL_Quit();
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
