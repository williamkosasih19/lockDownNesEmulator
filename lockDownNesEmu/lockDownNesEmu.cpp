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

int main()
{ 
  cout << "Lockdown Nes Emulator v0.01" << endl;
  cout << "    27 March 2020" << endl;
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

  array<unsigned int, 341 * 261> vidmem;

  for (int i = 0; i < vidmem.size(); i++)
  {
    vidmem[i] = 0;
  }

  bool_t quit = false;
  bool_t static_noise = false;

  // Component initialisation
  cartridge_c cartridge;
  ppu_c ppu(vidmem, cartridge);
  bus_c bus(cartridge, ppu);
  processor_c processor(bus);
  processor.init();
  bus.plug_in_processor(&processor);

  uint64_t cycle = 0;

  // nesTestDebug
  cartridge.load("C:\\Users\\willi\\Documents\\LockDownNesEmu\\nestest.nes");
  processor.reset();

  while (true)
  {
    if (cycle >= 50000)
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
          }
          break;

        case SDL_KEYUP:
          bus.controller[0] = 0;
          break;

        default:
          break;
        }
      }

      cycle = 0;
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
    else if (command_split[0] == "run")
    {
      while (true)
      {
        if (cycle >= 500)
        {
          cycle = 0;
          SDL_UpdateTexture(texture, NULL, vidmem.data(), 256 * sizeof(Uint32));

          SDL_RenderClear(renderer);
          SDL_RenderCopy(renderer, texture, NULL, NULL);
          SDL_RenderPresent(renderer);
        }

        const uint32_t first_tick = SDL_GetTicks();
        bus.clock();
        const uint32_t second_tick = SDL_GetTicks();
        cycle++;
        //SDL_Delay(5);
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
