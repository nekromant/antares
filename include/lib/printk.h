#ifndef PRINTK_H
#define PRINTK_H

#ifdef DEBUG
#define dbg(...) printk(__VA_ARGS__)
#endif


void printk(const char *fmt, /*args*/ ...);

#endif