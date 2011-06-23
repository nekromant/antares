#include <arch/antares.h>

#define _INF_ENABLED 1
#define _DBG_ENABLED 1
#define _ERR_ENABLED 1
#define _WNR_ENABLED 1

#include "include/debug.h"

#define pin_init_button(DDR,PORT,PIN) DDR&=~(1<<PIN); PORT|=(1<<PIN);
#define pin_is_set(PORT, PIN) (PORT & (1<<PIN))

#include "include/manipulator.h"


void pacman_eat()
{
    manipulator(grip, release);
    manipulator(move, bot);
    manipulator(get_a_pawn, 0);
    manipulator(grip, grip);
    manipulator(move, mid2);
    manipulator(stand, open);
    manipulator(move, mid);
    manipulator(grip, release);
    manipulator(move, bot);
    manipulator(stand, close);
    INF("Yammy!")
}

void pacman_eatmore()
{
    manipulator(grip, release);
    manipulator(move, bot);
    manipulator(get_a_pawn, 0);
    manipulator(grip, grip);
    manipulator(move, stdby);
    manipulator(stand, open);
    manipulator(grip, release);
    manipulator(move, bot);
    manipulator(stand, close);
    INF("Nyaaam!")
}
