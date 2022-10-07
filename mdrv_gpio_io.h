//probably obsolete

#include <asm/types.h>
#include "mdrv_types.h"

struct GPIO_Reg
{
    U32 u32Reg;
    U8 u8Enable;
    U8 u8BitMsk;
} __attribute__ ((packed));

typedef struct GPIO_Reg GPIO_Reg_t;

#define GPIO_IOC_MAGIC               'g'

#define MDRV_GPIO_INIT               _IO(GPIO_IOC_MAGIC, 0)
#define MDRV_GPIO_SET                _IOW(GPIO_IOC_MAGIC, 1, U8)
#define MDRV_GPIO_OEN                _IOW(GPIO_IOC_MAGIC, 2, U8)
#define MDRV_GPIO_ODN                _IOW(GPIO_IOC_MAGIC, 3, U8)
#define MDRV_GPIO_READ               _IOWR(GPIO_IOC_MAGIC, 4, U8)
#define MDRV_GPIO_PULL_HIGH          _IOW(GPIO_IOC_MAGIC, 5, U8)
#define MDRV_GPIO_PULL_LOW           _IOW(GPIO_IOC_MAGIC, 6, U8)
#define MDRV_GPIO_INOUT              _IOWR(GPIO_IOC_MAGIC, 7, U8)
#define MDRV_GPIO_WREGB              _IOW(GPIO_IOC_MAGIC, 8, GPIO_Reg_t)

#define GPIO_IOC_MAXNR               9

void __mod_gpio_init(void);
