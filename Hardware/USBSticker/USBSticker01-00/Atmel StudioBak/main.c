
//_____  I N C L U D E S ___________________________________________________
#define F_CPU 			800000UL

#include "config.h"
#include "modules/scheduler/scheduler.h"
#include "lib_mcu/wdt/wdt_drv.h"
#include "lib_mcu/power/power_drv.h"
#include "lib_mcu/usb/usb_drv.h"
#include "lib_mcu/util/start_boot.h"

#include <util/delay.h>
#include "spi_drv.h"
#include "RFmodule.h"
#include "main.h"


//_____ M A C R O S ________________________________________________________

//_____ D E F I N I T I O N S ______________________________________________


int main(void)
{
	/* Configure LEDS Pins */
	DDRD|=(1<<PD1)|(1<<PD2);

	/* Configure RF nIRQ PIN */
	DDRB&=~(1<<nIRQ);

	Usb_enable_regulator();
   	wdtdrv_disable();
   	start_boot_if_required();
   	boot_key=0;
  	//Clear_prescaler();

   	scheduler();

   	return 0;
}

//! \name Procedure to speed up the startup code
//! This one increment the CPU clock before RAM initialisation
//! @{
#ifdef  __GNUC__
// Locate low level init function before RAM init (init3 section)
// and remove std prologue/epilogue
char __low_level_init(void) __attribute__ ((section (".init3"),naked));
#endif

#ifdef __cplusplus
extern "C" {
#endif
char __low_level_init()
{
 // Clear_prescaler();
  return 1;
}
#ifdef __cplusplus
}
#endif
//! @}
