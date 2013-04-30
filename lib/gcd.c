#include <linux/kernel.h>
#include <linux/gcd.h>
#include <linux/module.h>

/* Greatest common divisor */
unsigned long gcd(unsigned long a, unsigned long b)
{
	unsigned long r;

	if (a < b)
		swap(a, b);
<<<<<<< HEAD
=======

	if (!b)
		return a;
>>>>>>> korg_linux-3.0.y/korg/linux-3.0.y
	while ((r = a % b) != 0) {
		a = b;
		b = r;
	}
	return b;
}
EXPORT_SYMBOL_GPL(gcd);
