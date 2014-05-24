/*
 *  Enable JTAG over GPIO pins on the Raspberry Pi
 */

#define SYSTIMERCLO 0x20003004
#define GPFSEL0     0x20200000
#define GPFSEL1     0x20200004
#define GPFSEL2     0x20200008
#define GPSET0      0x2020001C
#define GPCLR0      0x20200028
#define GPPUD       0x20200094
#define GPPUDCLK0   0x20200098

#include <bsp.h>
#include <stdlib.h>
#include <stdio.h>

void dummy(unsigned int y)
{
   volatile int x;    
   x = y;
}

rtems_task Init(
  rtems_task_argument ignored
)
{
    unsigned int ra;

    printf( "\n\n*** Configure JTAG Configuration ***\n" );

    BCM2835_REG(GPPUD) = 0;
    for(ra=0;ra<150;ra++) dummy(ra);
    BCM2835_REG(GPPUDCLK0) = ((1<<4)|(1<<22)|(1<<24)|(1<<25)|(1<<27));
    for(ra=0;ra<150;ra++) dummy(ra);
    BCM2835_REG(GPPUDCLK0) = 0;

    ra=BCM2835_REG(GPFSEL0);
    ra&=~(7<<12); //gpio4
    ra|=2<<12; //gpio4 alt5 ARM_TDI
    BCM2835_REG(GPFSEL0) = ra;

    ra=BCM2835_REG(GPFSEL2);
    ra&=~(7<<6); //gpio22
    ra|=3<<6; //alt4 ARM_TRST
    ra&=~(7<<12); //gpio24
    ra|=3<<12; //alt4 ARM_TDO
    ra&=~(7<<15); //gpio25
    ra|=3<<15; //alt4 ARM_TCK
    ra&=~(7<<21); //gpio27
    ra|=3<<21; //alt4 ARM_TMS
    BCM2835_REG(GPFSEL2) = ra;

  printf( "*** GPIO Pins Configured for JTAG ***\n" );
  exit( 0 );
}

/* configuration information */

/* NOTICE: the clock driver is explicitly disabled */
#define CONFIGURE_APPLICATION_DOES_NOT_NEED_CLOCK_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE
#define CONFIGURE_MAXIMUM_TASKS 1

#define CONFIGURE_INIT
#include <rtems/confdefs.h>
/* end of file */
