#include "mhal_gpio_reg.h"

unsigned int MHal_GPIO_REG(unsigned int addr) {
	return (REG_MIPS_BASE + (((addr) & ~1)<<1) + (addr & 1) );
}

unsigned int MHal_Get_Reg_Address(unsigned int addr) {
	return (((addr) & ~1)<<1) + (addr & 1);
}
