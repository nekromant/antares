//Servo power controls
#define PWR_0_PORT	PORTD
#define PWR_0_PIN	0

#define PWR_1_PORT	PORTD
#define PWR_1_PIN	1

#define PWR_2_PORT	PORTB
#define PWR_2_PIN	1

#define PWR_3_PORT	PORTB
#define PWR_3_PIN	0



//Outlets pins & ports
#define OUTLET_0_PORT	PORTD
#define OUTLET_1_PORT	PORTD

#define OUTLET_0_PIN	4
#define OUTLET_1_PIN	6


//Here go some magic macros, that control the power for servos
//Remember, p-mosfets invert stuff
#define servo_off(num) PWR_##num##_PORT  |= (1<<PWR_##num##_PIN);
#define servo_on(num) PWR_##num##_PORT &=~ (1<<PWR_##num##_PIN);
//And some that control generic outlets
#define outlet_off(num) OUTLET_##num##_PORT  |= (1<<OUTLET_##num##_PIN);
#define outlet_on(num) OUTLET_##num##_PORT  &=~ (1<<OUTLET_##num##_PIN);
#define outlet_toggle(num) OUTLET_##num##_PORT  ^= (1<<OUTLET_##num##_PIN);


