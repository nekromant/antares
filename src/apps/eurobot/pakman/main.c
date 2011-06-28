#include <arch/antares.h>
#define _DBG_ENABLED 1
#define _INF_ENABLED 1
#include <contrib/crumbo/debug.h>
#include <contrib/crumbo/systick.h>
#include <contrib/crumbo/manipulator.h>
#include <contrib/crumbo/chassis.h>
#include <contrib/crumbo/adc.h>
#include <contrib/crumbo/odetect.h>
#include <lib/callchain/callchain.h>

static struct fcall_chain_t timeupc;

static uint16_t distance_global;

static char pl;
__inline void collision_avoidance(char state)
{
  comm_putc(state);
  comm_putc(PORTL);
  switch(state)
  {
    case 1:
    __stop();
    break;
    case 0:
    if (motor_get_dir(0)!=2) motor_set_dir(0, motor_get_dir(0));
    if (motor_get_dir(1)!=2) motor_set_dir(1, motor_get_dir(1));
    break;
  }
  //state=1 - have something
  //state=0 - all clear
}


static void timeup_handler()
{
  
}


ANTARES_INIT_HIGH(pakman_init)
 {
  timeupc.func=timeup_handler;
  systick_timeup_add(&timeupc);
  systick_counter_initsimple(5);
 //code here
 }


ANTARES_APP(pakman_mainapp)
 {
    DDRJ|=1<<7;
    PORTJ&=~(1<<7);
    INF("Pakman ready, using Antares v." CONFIG_VERSION_STRING " git commit: " CONFIG_VERSION_GIT );

    //wait_start();
    //while(1)
    //{
    //  chassis_find(king, left, 0, 255);
    //  DBG("king left");
    //  chassis_find(pawn, left, 0, 255);
    //  DBG("king left");
    //  chassis_find(king, right, 0, 255);
    //  DBG("king left");
    //  chassis_find(pawn, right, 0, 255);
    //  DBG("king left");
    //}
    //distance = get_distance();
    //dump8(distance);
    //going from the start zone


    //chassis_find(king, right, 0, 255);
    //count_distance();
    //dump16(distance_global);
    //_delay_ms(3000);
    //return;

    //odct_set_active_group(group_bck);
    //chassis_move_precise(0, 255);
    //return;
    
    reset_direction();
    odct_set_active_group(group_fwd);
    chassis_move_precise(0, 56);
    chassis_turn(0, 255, 90);
    reset_direction();
    
    //getting a king
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

void count_distance()
{
  distance_global = (encoders_get(0) + encoders_get(1)) / 2 / 7.3 + 10;
  dump16(distance_global);
  _delay_ms(3000); 
}
inline void reset_distance()
{
  distance_global = 0;
}

