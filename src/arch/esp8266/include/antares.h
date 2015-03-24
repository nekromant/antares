#ifndef ANTARES_H
#define ANTARES_H

static inline unsigned long arch_local_irq_save(void)
{
        unsigned long flags;
        asm volatile("rsil %0, 1"
                     : "=a" (flags) :: "memory");
        return flags;
}

static inline void arch_local_irq_restore(unsigned long flags)
{
        asm volatile("wsr %0, ps; rsync"
                     :: "a" (flags) : "memory");
}

#define ANTARES_DISABLE_IRQS() cli()
#define ANTARES_ENABLE_IRQS() sei()

#define ANTARES_ATOMIC_BLOCK()						\
	for(unsigned long fl = 1, tmp = arch_local_irq_save();		\
	    fl;								\
	    arch_local_irq_restore(tmp), fl = 0)


#endif
