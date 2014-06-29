#ifndef LOWSERIAL_H
#define LOWSERIAL_H

/* This will be called to reconnect the device */
void lowserial_usb_reconnect();

/* Call this manually to xfer data. in e.g. delays */
void lowserial_poll();

#endif
