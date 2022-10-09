#define REG_ARM_BASE              0x1f200000 //set nonpm
#define SET_REG(addr)        (*(volatile unsigned char*)(REG_ARM_BASE + ( ((addr) & ~1) << 1 ) + (addr & 1) ))
#define SET_REG2(addr)        (*(volatile unsigned short*)(REG_ARM_BASE + ( ((addr) & ~1) << 1 ) + (addr & 1) ))

#define LOGO_MIU0_MEM_START (0x40000000)
#define LOGO_MIU1_MEM_START (0xA0000000)
#define REG_ADDR(addr)              (*((volatile U16*)(0x1f200000 + (addr << 1))))
#define REG_ADDR2(addr)              (*((volatile U16*)(0x1f220000 + (addr << 1))))
#define GOP0MIUSEL          0x12F6
