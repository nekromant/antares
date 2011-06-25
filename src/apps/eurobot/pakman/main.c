#include <arch/antares.h>
#define _DBG_ENABLED 1
#include <contrib/crumbo/debug.h>
#include <contrib/crumbo/systick.h>
#include <contrib/crumbo/manipulator.h>
#include <contrib/crumbo/chassis.h>
#include <contrib/crumbo/adc.h>
#include <contrib/crumbo/odetect.h>
#include <lib/callchain/callchain.h>

static struct fcall_chain_t timeupc;


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
    if (motor_get_dir(0)!=2) motor_set_dir(0 , motor_get_dir(0));
    if (motor_get_dir(1)!=2) motor_set_dir(1,motor_get_dir(1));
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
    DBG("Pakman ready, using Antares v." CONFIG_VERSION_STRING " git commit: " CONFIG_VERSION_GIT );
    //wait_start();

//    chassis_find(king, left, 0, 255);
//    distance = get_distance();
//    dump8(distance);

    //going from the start zone
    reset_direction();
    odct_set_active_group(group_fwd);
    chassis_move_precise(0, 56);
    chassis_turn(0, 255, 90);
    reset_direction();
    
    //getting a king
    
    chassis_move_precise(0, 55);
    chassis_find(king, left, 0, 255);
    chassis_turn(0, 255, 90);
    manipulator(grip, release);
    manipulator(move, bot);
    manipulator(get_a_pawn, 0);
    chassis_move_precise(1, 2);
    
    
    //have a king, putting it in
    pacman_eat();
    manipulator(move, mid2);
   
    //lets go back
    chassis_move_precise(1, 30);
    chassis_turn(1, 255, 88);
    reset_direction();
    
    //having a pawn
    chassis_move_precise(0, 55);
    chassis_find(pawn, left, 0, 255);
    chassis_turn(0, 255, 93);
    manipulator(move, bot);
    manipulator(get_a_pawn, 0);
    manipulator(grip, grip);

    //got a pawn, going back
    chassis_move_precise(1, 40);
    manipulator(grip, release);

    //building
    manipulator(stand, open);
    manipulator(move, mid);
    manipulator(grip, grip);
    manipulator(move, mid2);
    manipulator(stand, close);
    manipulator(grip, release);
    chassis_move_precise(1, 20);
    manipulator(move, bot);
    chassis_move_precise(0, 20);

    //grip a pyramide and try to deliver it
    manipulator(grip, grip);
    chassis_turn(1, 255, 90);
    reset_direction();
    chassis_move_precise(0, 140);
    chassis_turn(1, 255, 93);
    chassis_move_precise(0, 45);
    chassis_turn(0, 255, 80);    
    chassis_move_precise(0, 10);
    
    while(1);;
    reset_direction();
}


