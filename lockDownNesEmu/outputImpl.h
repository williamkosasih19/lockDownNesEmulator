#pragma once
/* ****************************************************************
   LockDownNesEmu Nes Emulator

   Header file for output and debugging functions.
   Date: 27 March 2020        Author : William Kosasih

**************************************************************** */

#include <cstdarg>
#include <cstdio>

#include "types.h"

extern bool_t verbose;

// Verbose print function.
inline void verbose_print(const char* const format...)
{
  if (verbose)
  {
    va_list varg;
    va_start(varg, format);
    vprintf(format, varg);
  }
}