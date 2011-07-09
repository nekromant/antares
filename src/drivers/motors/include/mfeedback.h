#ifdef _MFEEDBACK_H
#define _MFEEDBACK_H

//8bit feedbacks
#include "genmot.h"
//TODO: Use f*cking cool preprocessor magic to generate all variants
//Oh, yeah
struct mfeedback8_t
{
  struct gmotor8_t* a;
  struct gmotor8_t* b;
  uint8_t mode; //mode of operation 
  int16_t (*get_left_enc)(void);
  int16_t (*get_right_enc)(void);
  //TODO: Fill other stuff here
  int16_t prev_sa;
  int16_t prev_sb;
  int16_t acc_a; //accel a
  int16_t acc_b; //accel b
};


#endif