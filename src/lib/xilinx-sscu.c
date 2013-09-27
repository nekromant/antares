#include <arch/antares.h>
#include <lib/xilinx-sscu.h>

int xsscu_reset(const struct xsscu_unit* x)
{
	int i = 500;
	x->clk_ctl(0);
	x->sout_ctl(0);
	x->progb_ctl(0);
	x->delay(x->delay_pb);
	x->progb_ctl(1); //progb
	x->delay(x->delay_pb);
	while (i--) {
		if (x->get_initb()) //initb
			return 0;
 		x->delay(x->delay_clk);
	}
	return 1;
}

int xsscu_finalize(const struct xsscu_unit* x, int c)
{
	while (c--) {
		x->clk_ctl(0);
		x->delay(x->delay_clk);
		x->clk_ctl(1);
 		x->delay(x->delay_clk);
		if (x->get_done())
			return 0;
	}
	return 1;
}

void xsscu_write(const struct xsscu_unit* x, const unsigned char* fw, int fw_size)
{
	int i=0;
	int k;
	while (i < fw_size) {
		for (k = 7; k >= 0; k--) {
			x->sout_ctl(fw[i] & (1 << k));
			x->clk_ctl(1);
			x->delay(x->delay_clk);
			x->clk_ctl(0);
			x->delay(x->delay_clk);
		}
		i++;
	}
}


