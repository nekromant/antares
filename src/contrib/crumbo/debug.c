#include "arch/antares.h"
#include "include/debug.h"

void send8(uint8_t data)
{
  putc_func(data);
}

void send16(uint16_t data)
{
  putc_func(data);
  putc_func(data>>8);
}

void send32(uint32_t data)
{
  putc_func(data);
  putc_func(data>>8);
  putc_func(data>>16);
  putc_func(data>>24);
}


void dump8(uint8_t data)
{
  putc_func(DMP8);
  putc_func(data);
}

void dump16(uint16_t data)
{
  putc_func(DMP16);
  putc_func(data);
  putc_func(data>>8);
}

void test32()
{
  putc_func(DMP32);
  putc_func(0xaa);
  putc_func(0xbb);
  putc_func(0xcc);
  putc_func(0xdd);
}


void dump32( _u32 data)
{
  putc_func(DMP32);
  putc_func(data.bytes[0]);
  putc_func(data.bytes[1]);
  putc_func(data.bytes[2]);
  putc_func(data.bytes[3]);
}
