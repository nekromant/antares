#ifndef XSSCU_H
#define XSSCU_H

struct xsscu_unit {
	char* name;
	void (*clk_ctl)(char n);
	void (*sout_ctl)(char n);
	void (*progb_ctl)(char n);
	char (*get_initb)(void);
	char (*get_done)(void);
	void (*delay)(int n);
	int delay_pb;
	int delay_clk;
} ;

int xsscu_reset(const struct xsscu_unit* x);
int xsscu_finalize(const struct xsscu_unit* x, int c);
void xsscu_write(const struct xsscu_unit* x, const unsigned char* fw, int fw_size);


#endif