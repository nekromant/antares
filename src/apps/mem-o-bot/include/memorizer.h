#ifndef MEMORIZER_H
#define MEMORIZER_H


enum {
  ACT_END,
  ACT_FWD,
  ACT_LEFT,
  ACT_RIGHT
};

/*
 * Initializes the memorizer
 */

void memorizer_init();

/*
 * Pushes an action onto the memory
 * The eeprom write cycle occurs as needed
 * return: 0 - Everything ok
 * 1 - an error occured (we've hit the limit)
 */
char action_add(char act); 

/* 
 * Finalizes action list, synchronizes to eeprom 
 * Next call to action_add will start from the beginning
 * of the memory
 */
void action_finalize();


/*
 * Pops the next action from the memory and advances pointer
 */
char action_pop();





#endif