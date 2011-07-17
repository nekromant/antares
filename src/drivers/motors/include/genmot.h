#ifndef _GENMOT_H
#define _GENMOT_H

struct gmotor8_t
{
 void (*apply)(struct gmotor8_t*);
 uint8_t spd;
 uint8_t params;
};

struct gmotor16_t
{
 void (*apply);
 uint16_t spd;
 uint8_t params;
};

struct gmotor32_t
{
 void (*apply);
 uint32_t spd;
 uint8_t params;
};

#endif