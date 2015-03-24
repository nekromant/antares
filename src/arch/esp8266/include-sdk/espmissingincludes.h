
// https://github.com/mziwisky/esp8266-dev/blob/master/esphttpd/include/espmissingincludes.h

#ifndef ESPMISSINGINCLUIDES_H
#define ESPMISSINGINCLUIDES_H

#include <ets_sys.h>

//Missing function prototypes in include folders. Gcc will warn on these if we don't define 'em anywhere.
//MOST OF THESE ARE GUESSED! but they seem to swork and shut up the compiler.

int skip_atoi(const char **nptr);
int atoi(const char *nptr);
void ets_install_putc1(void *routine);
void ets_isr_attach(int intr, void *handler, void *arg);
void ets_isr_mask(unsigned intr);
void ets_isr_unmask(unsigned intr);
int ets_memcmp(const void *s1, const void *s2, size_t n);
void *ets_memcpy(void *dest, const void *src, size_t n);
void *ets_memset(void *s, int c, size_t n);
void ets_bzero(void *s, size_t n);
int ets_sprintf(char *str, const char *format, ...)  __attribute__ ((format (printf, 2, 3)));
int ets_str2macaddr(void *, void *);
int ets_strcmp(const char *s1, const char *s2);
char *ets_strcpy(char *dest, const char *src);
size_t ets_strlen(const char *s);
int ets_strncmp(const char *s1, const char *s2, int len);
char *ets_strncpy(char *dest, const char *src, size_t n);
char *ets_strstr(const char *haystack, const char *needle);
void ets_timer_arm_new(ETSTimer *a, int b, int c, int isMstimer);
void ets_timer_disarm(ETSTimer *a);
void ets_timer_setfn(ETSTimer *t, ETSTimerFunc *fn, void *parg);
int os_random (void);

void ets_wdt_enable(void);
void ets_wdt_disable(void);
void wdt_feed(void);


void ets_delay_us (int us);

void pvPortFree(void *ptr);
void *pvPortMalloc(size_t xWantedSize);
void *pvPortRealloc(void* ptr, size_t xWantedSize);
void *pvPortZalloc(size_t);
void uart_div_modify(int no, unsigned int freq);
void vPortFree(void *ptr);
void *vPortMalloc(size_t xWantedSize);

int ets_uart_printf(const char *fmt, ...)                         __attribute__((format(printf, 1, 2)));
int os_printf_plus (const char *fmt, ...)                         __attribute__((format(printf, 1, 2)));
int os_snprintf    (char *str, size_t size, const char *fmt, ...) __attribute__((format(printf, 3, 4)));
#if 0
int os_printf      (const char *fmt, ...)                         __attribute__((format(printf, 1, 2)));
#else
#define os_printf os_printf_plus
#endif

/* These were dropped from newer SDKs */
int wifi_softap_set_station_info(uint8_t *addr, struct ip_addr *adr);
void system_station_got_ip_set(ip_addr_t * ip_addr, ip_addr_t *sn_mask, ip_addr_t *gw_addr);
void system_pp_recycle_rx_pkt (void*);

#endif
