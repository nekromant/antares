#include <arch/antares.h>

struct xferbuffer_t {
  int bsize;
  int head;
  int tail;
  char data[1];//The length is a lie =)
};


void xbuffer_putc(struct xferbuffer_t* xb)
{
  xb->data[xb->head++]=c;
  xb->head &= xb->bsize;
}

char xbuffer_getc(struct xferbuffer_t* xb)
{
  if (xb->tail!=xb->head)
  {
  xb->tail++;
  xb->tail &= xb->bsize;
  return xb->data[xb->tail];
  }
}

void xbuffer_puts(struct xferbuffer_t* xb)

