#ifndef _CAPSENSE_H
#define _CAPSENSE_H


struct capsense_pin
{
	void     (*out_ctl)(uint8_t dir, uint8_t val);
	void     (*in_ctl) (uint8_t dir, uint8_t val);
	uint8_t  (*in_state)(void);
	uint16_t num_samples;
	uint16_t offset; 
	uint16_t timeout;
};


#define AVR_CTL_FUNC(name, PRT, PIN) \
	void name(uint8_t dir, uint8_t val) {	      \
	DDR ## PRT &= ~(1 << PIN);		      \
	DDR ## PRT |= (dir << PIN);		      \
						      \
	PORT ## PRT &= ~(1 << PIN);		      \
	PORT ## PRT |= (val << PIN);		      \
}						\


#define AVR_STATE_FUNC(name, PRT, PN)		\
	int name() {				\
		return PIN ## PRT & (1 << PN);	\
	};


uint16_t capsense_sense_once(struct capsense_pin *pin);
uint16_t capsense_sense(struct capsense_pin *pin);


#endif
