#include <arch/antares.h> 
#include <lib/capsense.h>





uint16_t capsense_sense_once(struct capsense_pin *pin)
{
	uint16_t i=0;
	pin->out_ctl(1, 0);
	pin->in_ctl (1, 0);

	unsigned int timeout = pin->timeout;

	pin->in_ctl (0, 0);

	ANTARES_ATOMIC_BLOCK() { 
		pin->out_ctl(1, 1);
		while (!(pin->in_state()) && timeout--) 
			i++;
	}

	timeout = pin->timeout; 

	pin->in_ctl (1, 1);
	pin->in_ctl (0, 0);

	ANTARES_ATOMIC_BLOCK() { 
		pin->out_ctl(1, 0);
		while ((pin->in_state()) && timeout--) 
			i++;
	}
		
	return i;
	
}

uint16_t capsense_sense(struct capsense_pin *pin)
{
	int i;
	uint16_t ret;
	for (i=0; i< pin->num_samples; i++)
		ret += sense_once();
	return ret / pin->num_samples - pin->offset;	
}

