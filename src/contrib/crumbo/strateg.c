#include <arch/antares.h>
#define _DBG_ENABLED 1
#define _INF_ENABLED 1
#include <contrib/crumbo/debug.h>
#include <contrib/crumbo/systick.h>
#include <contrib/crumbo/manipulator.h>
#include <contrib/crumbo/chassis.h>
#include <contrib/crumbo/adc.h>
#include <contrib/crumbo/odetect.h>
#include <contrib/crumbo/encoder.h>
#include <lib/callchain/callchain.h>



//PH0/PH1
ANTARES_INIT_HIGH(strategy_init)
{
  DDRH|=(1<<0)|(1<<1);
  PORTH=(1<<0)|(1<<1);
}

#ifdef CONFIG_CONTRIB_CRUMBO_STRATEGY_DUMMY
__inline void strategy(int mirror)
{
  DBG("Current strategy mirror flag");
  dump16(mirror);
  DBG("<----->");
}
#endif

#ifdef CONFIG_CONTRIB_CRUMBO_STRATEGY_FRST

__inline void strategy(int mirror)
{
    DBG("Resetting direction");
    reset_direction();
    odct_set_active_group(group_fwd);
    chassis_move_precise(0, 56);
    chassis_turn(0, 255, 90);
    reset_direction();
    
    DBG("Locating the king");
    odct_set_active_group(group_fwd);
    chassis_move_precise(0, 55);
    chassis_find(king, left, 0, 255);
    odct_set_active_group(group_none);
    chassis_turn(0, 255, 90);
    manipulator(grip, release);
    manipulator(move, bot);
    manipulator(get_a_pawn, 0);
    chassis_move_precise(1, 2);
    
    
    //have a king, putting it in
    pacman_eat();
    manipulator(move, mid2);
   
    //lets go back
    odct_set_active_group(group_bck);
    chassis_move_precise(1, 30);
    odct_set_active_group(group_rht);
    chassis_turn(1, 255, 90);
    odct_set_active_group(group_bck);
    reset_direction();
    
    //having a pawn
    odct_set_active_group(group_fwd);
    chassis_move_precise(0, 55);
    chassis_find(pawn, left, 0, 255);
    odct_set_active_group(group_lft);
    chassis_turn(0, 255, 90);
    manipulator(move, bot);
    manipulator(get_a_pawn, 0);
    manipulator(grip, grip);

    //got a pawn, going back
    odct_set_active_group(group_bck);
    chassis_move_precise(1, 40);
    manipulator(grip, release);

    //building
    manipulator(stand, open);
    manipulator(move, mid);
    manipulator(grip, grip);
    manipulator(move, mid2);
    manipulator(stand, close);
    manipulator(grip, release);

    odct_set_active_group(group_bck);
    chassis_move_precise(1, 20);
    manipulator(move, bot);
    odct_set_active_group(group_none);
    chassis_move_precise(0, 20);

    //grip a pyramide and try to deliver it
    manipulator(grip, grip);
    chassis_turn(1, 255, 90);
    reset_direction();
    odct_set_active_group(group_fwd);
    chassis_move_precise(0, 140);
    odct_set_active_group(group_rht);
    chassis_turn(1, 255, 93);
    odct_set_active_group(group_fwd);
    chassis_move_precise(0, 45);
    chassis_turn(0, 255, 80);    
    chassis_move_precise(0, 10);
    
    while(1);;
    reset_direction();
}
#endif

void execute_strategy()
{
  DBG("Determinig strategy to use and executing it...");
  if (PINH & (1<<0))
  {
    strategy(0);    
  }
  
  if (PINH & (1<<1))
  {
    strategy(1);
  }
  DBG("The strategy is now executed!");
}