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
};


/** 
 * Reset the FPGA.  
 * 
 * @param x Xilinx FPGA instance
 * 
 * @return 0 if FPGA is ready, 1 otherwise 
 */

int xsscu_reset(const struct xsscu_unit* x);

/** 
 * Send clocks until the FPGA reports being READY.  
 * 
 * @param x Xilinx FPGA instance
 * 
 * @return 0 if FPGA is ready, 1 otherwise 
 */

int xsscu_finalize(const struct xsscu_unit* x, int c);

/** 
 * Bitbang data chunk to the FPGA
 * 
 * @param x xsscu instance
 * @param fw data buffer
 * @param fw_size data buffer size
 */
void xsscu_write(const struct xsscu_unit* x, const unsigned char* fw, int fw_size);


#endif
