#include <arch/antares.h>
#include <contrib/avrlib/cmdline.h>
#include <contrib/avrlib/rprintf.h>
#define _DBG_ENABLED 1
#define _INF_ENABLED 1
#include <contrib/crumbo/debug.h>
#include <contrib/crumbo/manipulator.h>
#include <contrib/crumbo/manip_extra.h>
#include <contrib/crumbo/chassis.h>



/*
void dumpArgsHex(void)
{
        rprintfCRLF();
        DBG("Dump arguments as hex integers\r\n");
	comm_puts((cmdlineGetArgStr(1)));
        //int a = atoi(());
	rprintf("a=%d\r\n",cmdlineGetArgStr(1));
        rprintfCRLF();
}
*/

static int mirror;

static void _gripper(void)
{
  //rprintfCRLF();
  if (strcmp("relax",(const char*)cmdlineGetArgStr(1))==0)
  {
    DBG("Relax!");
    manipulator(grip,release);
  }else
  {
    DBG("Grip!");
    manipulator(grip,grip);
  }
  DBG("Done");
}

void _turn()
{
  int dir = cmdlineGetArgInt(1);
  if (mirror) dir=!dir;
  int pwm = cmdlineGetArgInt(2);
  int deg = cmdlineGetArgInt(3);
  chassis_turn(dir, pwm, deg);
}

/*static __inline void _pf()
{
  if (strcmp("rst",cmdlineGetArgStr(1))==0) p_reset(); else
  {
  int dir = cmdlineGetArgInt(1);
  int speed = cmdlineGetArgInt(2);
  int b = cmdlineGetArgInt(3);
  if (mirror) b+=2;
  chassis_move_precise_d(dir, (char) speed, b);
  }
}
*/
void _m_get()
{
  manipulator(get_a_pawn,0);
}


void _pr_move()
{
  int dir = cmdlineGetArgInt(1);
  int speed = cmdlineGetArgInt(2);
  int dist = cmdlineGetArgInt(3);
  chassis_move_precise(dir, (char) speed, (char) dist);
}

void _mir()
{
  mirror=cmdlineGetArgInt(1);
}
void _stand()
{
  if (strcmp("open",(const char*)cmdlineGetArgStr(1))==0)
  {
    DBG("Opening stand");
    manipulator(stand,open);
  }else
  {
    DBG("Closing stand");
    manipulator(stand,close);
  }
}

void _lift()
{
  if (strcmp("mid",(const char*)cmdlineGetArgStr(1))==0)
  {
   manipulator(move,mid);
   DBG("MID");
  }else if (strcmp("bot",(const char*)cmdlineGetArgStr(1))==0)
  {
    DBG("Bottom");
    manipulator(move,bot);
  }else if (strcmp("stdby",(const char*)cmdlineGetArgStr(1))==0)
  {
    DBG("SB");
    manipulator(move,stdby);
  } else if (strcmp("mid2",(const char*)cmdlineGetArgStr(1))==0)
  {
    manipulator(move,mid2);
    DBG("MID2");
  }
  DBG("Done");
}
void _end()
{
  DBG("That's all");
  while(1);;;
}

char* eeptr=0;
  
static void _restart()
{
  eeptr=0;
}

static __inline void reg_cmds()
{
  cmdlineAddCommand("gripper", _gripper);
  cmdlineAddCommand("mir", _mir);
  cmdlineAddCommand("pakman_eat", pacman_eat);
  cmdlineAddCommand("chassis_turn", _turn);
  cmdlineAddCommand("reset_direction", reset_direction);
  cmdlineAddCommand("move_presise", _pr_move);
  cmdlineAddCommand("stand", _stand);
  cmdlineAddCommand("end", _end);
  cmdlineAddCommand("lift", _lift);
  //cmdlineAddCommand("pf", _pf);
  cmdlineAddCommand("restart", _restart);
  cmdlineAddCommand("pget", _m_get); //pos to get
}



// #ifdef CONFIG_SHELL_V
ANTARES_APP(shell_init)
{
  #ifdef CONFIG_CONTRIB_CRUMBO_SHELL_INT
  DBG("Entering interactive shell");
  #else
  DBG("Entering non-interactive shell");
  #endif
  rprintfInit(comm_putc);
  cmdlineSetOutputFunc(comm_putc);
  reg_cmds();

  while(1)
  {
    cmdlineMainLoop();
    #ifndef CONFIG_CONTRIB_CRUMBO_SHELL_INT
    cmdlineInputFunc(eeprom_read_byte(eeptr++));
    #endif
  }
} //*/
//#else

/*
ANTARES_APP(shell_init)
{
 // vt100ClearScreen();
  //vt100SetCursorPos(1,0);
  DBG("Booting interactive shell");
  rprintfInit(comm_putc);

  cmdlineSetOutputFunc(comm_putc);
  cmdlineInputFunc('\r');
  reg_cmds();
  
  
  while (1) 
  {
    cmdlineMainLoop();
    //read eeprom here!
  }

}//*/