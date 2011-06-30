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


//RED
#ifdef CONFIG_CONTRIB_CRUMBO_STRATEGY_FRST_1
__inline void strategy(int mirror)
{
    DBG("Resetting direction");
    reset_direction();
    odct_set_active_group(group_fwd);
    chassis_move_precise(0, 230, 56);
    chassis_turn(mirror, 255, 90);
    reset_direction();
    
    DBG("Locating the king");
    odct_set_active_group(group_fwd);
    chassis_move_precise(0, 230, 45);
    //chassis_find(pawn, side, 0, 255);
    chassis_move_precise(0, 230, 10);
    odct_set_active_group(group_none);
    chassis_turn(mirror, 255, 90);
    manipulator(grip, release);
    manipulator(move, bot);
    manipulator(get_a_pawn, 0);
    chassis_move_precise(1, 230, 2);
    chassis_move_precise(0, 230, 2);

    manipulator(grip, grip);
    
    odct_set_active_group(group_bck);
    chassis_move_precise(1, 230, 26);
    odct_set_active_group(group_rht);
    chassis_turn(!mirror, 255, 100);
    odct_set_active_group(group_bck);
    reset_direction();
    //*/
    odct_set_active_group(group_fwd);
    chassis_move_precise(0, 230, 55);

    chassis_turn(!mirror, 255, 45);

    
    chassis_move_precise(0, 230, 93);

    chassis_turn(mirror, 255, 45);

    chassis_move_precise(0, 230, 25);

    manipulator(grip, release);

    chassis_move_precise(1, 230, 20);

    chassis_turn(!mirror, 160, 90);

    chassis_move_precise(0, 100, 40);

    chassis_turn(!mirror, 160, 90);

    chassis_move_precise(0, 160, 40);

while(1);;;


}

#endif 


//BLUE
#ifdef CONFIG_CONTRIB_CRUMBO_STRATEGY_FRST__
__inline void strategy(int mirror)
{
    //mirror=1;
    DBG("Resetting direction");
    reset_direction();
    odct_set_active_group(group_fwd);
    chassis_move_precise(0, 230, 56);
    chassis_turn(mirror, 255, 90);
    reset_direction();
    
    DBG("Locating the king");
    odct_set_active_group(group_fwd);
    chassis_move_precise(0, 230, 45);
    //chassis_find(pawn, side, 0, 255);
    chassis_move_precise(0, 230, 10);
    odct_set_active_group(group_none);
    chassis_turn(mirror, 255, 90);
    manipulator(grip, release);
    manipulator(move, bot);
    manipulator(get_a_pawn, 0);
    chassis_move_precise(1, 230, 2);
    chassis_move_precise(0, 230, 2);

    manipulator(grip, grip);
    
    odct_set_active_group(group_bck);
    chassis_move_precise(1, 230, 26);
    odct_set_active_group(group_rht);
    chassis_turn(!mirror, 255, 100);
    odct_set_active_group(group_bck);
    reset_direction();
    //*/
    odct_set_active_group(group_fwd);
    chassis_move_precise(0, 230, 55);

    chassis_turn(!mirror, 255, 45);

    
    chassis_move_precise(0, 230, 93);

    chassis_turn(mirror, 255, 45);

    chassis_move_precise(0, 230, 25);

    chassis_turn(mirror, 160, 0);
    
    manipulator(grip, release);

    chassis_move_precise(1, 230, 20);

    chassis_turn(!mirror, 160, 90);

    chassis_move_precise(0, 100, 20);

    chassis_turn(!mirror, 160, 90);

    chassis_move_precise(0, 160, 20);

while(1);;;


}

#endif 



#ifdef CONFIG_CONTRIB_CRUMBO_STRATEGY_FRST
__inline void strategy(int mirror)
{
    DBG("Resetting direction");
    reset_direction();
    odct_set_active_group(group_fwd);
    chassis_move_precise(0, 230, 56);
    chassis_turn(mirror, 255, 90);
    reset_direction();
    
    DBG("Locating the king");
    odct_set_active_group(group_fwd);
    chassis_move_precise(0, 230, 45);
    //chassis_find(pawn, side, 0, 255);
    chassis_move_precise(0, 230, 10);
    odct_set_active_group(group_none);
    chassis_turn(mirror, 255, 90);
    manipulator(grip, release);
    manipulator(move, bot);
    manipulator(get_a_pawn, 0);
    chassis_move_precise(1, 230, 2);
    chassis_move_precise(0, 230, 2);

    manipulator(grip, grip);
    
    odct_set_active_group(group_bck);
    chassis_move_precise(1, 230, 26);
    odct_set_active_group(group_rht);
    chassis_turn(!mirror, 255, 100);
    odct_set_active_group(group_bck);
    reset_direction();
    //*/
    odct_set_active_group(group_fwd);
    chassis_move_precise(0, 230, 55);

    chassis_turn(!mirror, 255, 45);

    chassis_move_precise(0, 230, 10);
    
    manipulator(grip, release);

    chassis_move_precise(1, 230, 20);
    while(1);;;


}
#endif 


#ifdef CONFIG_CONTRIB_CRUMBO_STRATEGY_FRST_KOOL
__inline void strategy(int mirror)
{
  
reset_direction();
manipulator(move, bot);
odct_set_active_group(group_fwd);
chassis_move_precise(0, 230, 70);
odct_set_active_group(group_rht);
chassis_turn(mirror, 200, 92);
odct_set_active_group(group_bck);
reset_direction();
odct_set_active_group(group_fwd);
chassis_move_precise(0, 230, 54);
odct_set_active_group(group_rht);
chassis_turn(mirror, 200, 92);
manipulator(stand, close);
pacman_eat();
odct_set_active_group(group_bck);
chassis_move_precise(1, 230, 40);
odct_set_active_group(group_lft);
chassis_turn(!mirror, 200, 92);
odct_set_active_group(group_bck);
reset_direction();
odct_set_active_group(group_fwd);
chassis_move_precise(0, 230, 80);
odct_set_active_group(group_rht);
chassis_turn(mirror, 200, 92);
manipulator(get_a_pawn, 0);
manipulator(grip, grip);
odct_set_active_group(group_bck);
chassis_move_precise(1, 230, 37);
odct_set_active_group(group_lft);
chassis_turn(!mirror, 200, 98);
odct_set_active_group(group_bck);
reset_direction();
odct_set_active_group(group_fwd);
chassis_move_precise(0, 230, 146);
odct_set_active_group(group_lft);
chassis_turn(!mirror, 200, 46);
odct_set_active_group(group_fwd);
chassis_move_precise(0, 230, 20);
manipulator(grip, release);
odct_set_active_group(group_bck);
chassis_move_precise(1, 230, 20);
odct_set_active_group(group_lft);
chassis_turn(!mirror, 200, 135);
}
#endif 


#ifdef CONFIG_CONTRIB_CRUMBO_STRATEGY_FRSTU

__inline void strategy(int mirror)
{
    DBG("Resetting direction");
    reset_direction();
    odct_set_active_group(group_fwd);
    chassis_move_precise(0, 56);
    chassis_turn(mirror, 255, 90);
    reset_direction();
    
    DBG("Locating the king");
    odct_set_active_group(group_fwd);
    chassis_move_precise(0, 45);
    chassis_find(king, left, 0, 255);
    chassis_move_precise(0, 10);
    odct_set_active_group(group_none);
    chassis_turn(mirror, 255, 90);
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
    chassis_turn(!mirror, 255, 90);
    odct_set_active_group(group_bck);
    reset_direction();
    
    //having a pawn
    odct_set_active_group(group_fwd);
    chassis_move_precise(0, 55);
    chassis_find(pawn, left, 0, 255);
    chassis_move_precise(0, 10);    
    odct_set_active_group(group_lft);
    chassis_turn(mirror, 255, 90);
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
    chassis_turn(!mirror, 255, 90);
    reset_direction();
    odct_set_active_group(group_fwd);
    chassis_move_precise(0, 140);
    odct_set_active_group(group_rht);
    chassis_turn(!mirror, 255, 93);
    odct_set_active_group(group_fwd);
    chassis_move_precise(0, 45);
    chassis_turn(mirror, 255, 80);    
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