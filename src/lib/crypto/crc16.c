#include <stdint.h>

uint16_t crc16(uint16_t crc, const unsigned char *buf, int sz)
{
        while (--sz >= 0)
        {
                int i;
                crc ^= (uint16_t) *buf++ << 8;
                for (i = 0; i < 8; i++)
                        if (crc & 0x8000)
                                crc = crc << 1 ^ 0x1021;
                        else
                                crc <<= 1;
        }
        return crc;
}

