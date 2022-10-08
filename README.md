# Vestel_mb62_gpio
MStar Titania 4 userspace GPIO access

Here is my reverse engineering effort to drive the GPIO (right now: the power led) of the MStar Titania 4 SOC used in Vestel MB62 boards

To use the code with other chip familys, replace the gpio mapping from the kernel sources :) @ https://github.com/Benjamin-Dobell/kogan-tv-gpl/tree/master/Kernel_updated/mstar/mstar/gpio

# How to get root

create `usb_auto_update_mb62.txt` on a USB Stick, add

`setenv bootdelay 3`

`saveenv` 

`reset`

`%`

and save, plug in and hold the `OK` button on your remote, your tv set will take 3 more seconds to boot from now on :)

Wire up an uart to SCART, reboot and `Press any button in uart console`, use 

`setenv bootargs init=/bin/sh console=ttyS0,115200 ubi.mtd=1 ubi.mtd=2,512 root=ubi1:RFS rootfstype=ubifs rw LX_MEM=0x3400000 EMAC_MEM=0 DRAM_LEN=0x8000000 mtdparts=edb64M-nand:3200k(KL),11392k(CONF),22m(UBIA),25m(UBIB),-(NA)`

`run bootcmd`

`mount -a && mount -t ubifs ubi0:CON  /conf && mount -t ubifs ubi1:VEN  /vendor && insmod /lib/modules/usb-storage.ko && echo 1 > /proc/deferred_initcalls;cat /proc/deferred_initcalls`

`mount -t vfat /dev/sda1 /mnt/sda1`

to boot the platform and access your usb stick @ /mnt/sda1

# How to compile?

Get the `Codesourcery mips 4.3-51` compiler and extract it in `cross` then run the build script, `a.out` is the binary that runs on the tv

# All nice, how do I change the theme colors tho?

Dump `/vendor/aurora.elf`, hex color values start @ 0xc95218, you probably need the same stuff the peeps @ cabot / vestel smoked when they came up with the gui framework in order to modify it :)


# PWM

The PWM Peripheral works like this:

Your base adress is `0x1f200000`, the "NONPM" area

In order to Switch to the PWM Bank, write `0x1` to `0x1f206400`

To set - for example - the PWM2 (backlight) duty cycle write a U32 with your U8 value to `0x1f206420`

What are the adresses? you might ask, register offsets are in `mdrv_pwm.h`, do `BASE + (REG_OFF << 2)`