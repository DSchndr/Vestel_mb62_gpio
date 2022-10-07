#include <stdio.h>

#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <stdint.h>
#include <stdlib.h>

//#include "mdrv_gpio.h"
#include "mhal_gpio_reg.h"
#include "mhal_gpio.h"


#include <sys/mman.h>
int fd;
int *pm_bank = NULL;

//from mstar miomap kernel driver
#define MIOMAGIC			 'I'
#define MIOMAP_IOC_CHIPINFO 		_IOWR(MIOMAGIC, 0x00, DrvMIOMap_ChipInfo_t)
#define MIOMAP_IOC_SET_MAP 		_IOW (MIOMAGIC, 0x01, DrvMIOMap_Info_t)
#define MIOMAP_IOC_FLUSHDCACHE 		_IOR (MIOMAGIC, 0x02, DrvMIOMap_Info_t)
#define MIOMAP_IOC_GET_BLOCK_OFFSET 	_IOR (MIOMAGIC, 0x03, unsigned int) //ein retard hats im kernel code verkackt, returned 

typedef struct {
	unsigned int u32Addr;
	unsigned int u32Size;
} DrvMIOMap_Info_t;

typedef struct {
	unsigned short u16chiptype;
	unsigned short u16chipversion;
} DrvMIOMap_ChipInfo_t;

//From aurora.elf @ 0089d700 in .text :)
#define PM_BANK_ADDR 0x1f000000 //PM region
#define PM_BANK_SIZE 0x225600
#define NONPM_BANK_ADDR 0x1f200000 //non-pm region
#define NONPM_BANK_SIZE	0x25600
#define FLASH_BANK_ADDR 0x14000000 //seems to be spi flash, gets unloaded at some point in aurora
#define FLASH_BANK_SIZE 0x4000000

void DumpHex(const void* data, size_t size);
#include <unistd.h>


int main(int argc, char* argv[])
{
	printf("Hallo Welt! \n");

	int fd = open("/dev/miomap", O_RDWR);
	if (fd == -1) {
		printf("Could not open miomap, no mmio for us :(\n");
		return -1;
	}

	unsigned int blockoffset;
	DrvMIOMap_ChipInfo_t chipinfo;
	DrvMIOMap_Info_t iomem;

	iomem.u32Addr = PM_BANK_ADDR;
	iomem.u32Size = PM_BANK_SIZE;

	ioctl(fd, MIOMAP_IOC_SET_MAP, &iomem);
	ioctl(fd, MIOMAP_IOC_GET_BLOCK_OFFSET, &blockoffset);
	ioctl(fd, MIOMAP_IOC_CHIPINFO, &chipinfo);


	printf("Blockoffset: %#x | Chipinfo: %#x %#x | Trying to mmap pm bank\n", blockoffset, chipinfo.u16chiptype , chipinfo.u16chipversion);
	pm_bank = mmap(0, PM_BANK_SIZE, 3, 1, fd, 0);

        if (pm_bank == MAP_FAILED) {
                close(fd);
                printf("Map fail\n");
                return 2;
        }
	//printf("PM_BANK_BASE: %#x \n ", pm_bank);
	//printf("Dumping 0x8000 of PM bank :)\n");

	char *p;
    	long conv = strtol(argv[1], &p, 10);
	printf("Skipping %#x bytes\n", conv);
	void *pm_bank_temp = pm_bank + conv;
	DumpHex(pm_bank_temp, 0x4000);

	//MHal_print(); //print out gpio adresses
	printf("-->MHal_GPIO_Init()\n");
	MHal_GPIO_Init();
	//MHal_GPIO_Dump();
	printf("Starting Blinky on IO 7&11");
	int c = 0;
	while(c != 'x') {
        	printf("H\n");
		// the led is kinda funny on this platform, look at the schematic to see how gpio 11 and 7 drive the led, no idea why they did this xD
		//MHal_GPIO_Set_High(conv);
		MHal_GPIO_Set_Low(11);
		MHal_GPIO_Set_High(7); //gpio_set 7-1 turns off pled
		sleep(1);
		MHal_GPIO_Dump();
		printf("L\n");
		//MHal_GPIO_Set_Low(conv);
		MHal_GPIO_Set_High(11);
        	MHal_GPIO_Set_Low(7); //gpio_set 7-0 turns on pled
		sleep(1);
		MHal_GPIO_Dump();
		printf("Enter\n");
		c = getchar();
	}

	munmap(pm_bank, PM_BANK_SIZE);
	close(fd);
	return 0;
}


void DumpHex(const void* data, size_t size) {
	char ascii[17];
	size_t i, j;
	ascii[16] = '\0';
	for (i = 0; i < size; ++i) {
		printf("%02X ", ((unsigned char*)data)[i]);
		if (((unsigned char*)data)[i] >= ' ' && ((unsigned char*)data)[i] <= '~') {
			ascii[i % 16] = ((unsigned char*)data)[i];
		} else {
			ascii[i % 16] = '.';
		}
		if ((i+1) % 8 == 0 || i+1 == size) {
			printf(" ");
			if ((i+1) % 16 == 0) {
				printf("|  %s | %#x \n", ascii, i);
			} else if (i+1 == size) {
				ascii[(i+1) % 16] = '\0';
				if ((i+1) % 16 <= 8) {
					printf(" ");
				}
				for (j = (i+1) % 16; j < 16; ++j) {
					printf("   ");
				}
				printf("|  %s | %#x \n", ascii,  i);
			}
		}
	}
}
