/*
 *   Copyright (C) 2014  Andrew 'Necromant' Andrianov <andrew@ncrmnt.org>
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#include <arch/antares.h>
#include <avr/boot.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <generated/usbconfig.h>
#include <arch/vusb/usbportability.h>
#include <arch/vusb/usbdrv.h>
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <lib/circ_buf.h>
#include <lib/earlycon.h>
#include <lib/printk.h>
#include <lib/earlycon-vusb-lowserial.h>

#define OB_SIZE 256
#define PKT_SIZE  8

#if (OB_SIZE % 2)
#error "CB must be power of two"
#endif


static char out_buffer[OB_SIZE];
static int o_head, o_tail;

static char in_buffer[OB_SIZE];
static int i_head, i_tail;

static usbMsgLen_t towrite;

usbMsgLen_t usbFunctionSetup(uchar data[8])
{
	usbRequest_t *rq = (void *)data;
	towrite = rq->wLength.word;
	return USB_NO_MSG;
}


uchar usbFunctionRead(uchar *data, uchar len)
{
	if (CIRC_CNT(o_head, o_tail, OB_SIZE)) {
		int len = min_t(int,
				PKT_SIZE,
				CIRC_CNT_TO_END(o_head, o_tail, OB_SIZE));
		/* Actual data transfer here */
		memcpy(data, &out_buffer[o_tail], len);
		o_tail = (o_tail + len) & (OB_SIZE - 1);
		return len;
	}
	return 0;
}

uchar usbFunctionWrite(uchar *data, uchar len)
{
	do {
		int tocopy = min_t(uchar,
				   len,
				   CIRC_SPACE_TO_END(i_head, i_tail, OB_SIZE));
		memcpy(&in_buffer[i_head], data, tocopy);
		len -= tocopy;
		towrite -= tocopy;
		i_head = (i_head + tocopy) & (OB_SIZE - 1);
	} while (len);

	/* Postpone requests till we process all data */
	if (CIRC_SPACE(i_head, i_tail, OB_SIZE) < PKT_SIZE)
		usbDisableAllRequests();

	if (!towrite)
		return 1;
	else
		return 0;
}

void lowserial_poll()
{
	usbPoll();
	if (usbAllRequestsAreDisabled() &&
	    (CIRC_SPACE(i_head, i_tail, OB_SIZE) > PKT_SIZE))
		usbEnableAllRequests();
}



static void usb_putc(char data)
{
	while (!CIRC_SPACE(o_head, o_tail, OB_SIZE))
		lowserial_poll();
	out_buffer[o_head] = data;
	o_head = (o_head + 1) & (OB_SIZE - 1);
}

static int usb_getc(void)
{
	while (!CIRC_CNT(i_head, i_tail, OB_SIZE))
		lowserial_poll();
	unsigned char data = in_buffer[i_tail];
	i_tail = (i_tail + 1) & (OB_SIZE - 1);
	return data;
}

static int usb_avail()
{
	return CIRC_CNT(i_head, i_tail, OB_SIZE);
}

static void usb_init()
{
	lowserial_usb_reconnect();
	usbInit();
#ifdef CONFIG_LIB_EARLYCON_ANNOUNCE
	printk("earlycon: vusb-based early console enabled\n");
#endif
}

struct early_console g_early_console = {
	.havechar = usb_avail,
	.rxchar = usb_getc,
	.txchar = usb_putc,
	.init = usb_init
};


ANTARES_APP(usb_poll_app) {
	lowserial_poll();
}


