
#ifndef _CONFIG_H_
#define _CONFIG_H_

// Compiler switch (do not change these settings)
#include "../lib_mcu/compiler.h"             // Compiler definitions
#ifdef __GNUC__
   #include <avr/io.h>                    // Use AVR-GCC library
#elif __ICCAVR__
   #define ENABLE_BIT_DEFINITIONS
   #include <ioavr.h>                     // Use IAR-AVR library
#else
   #error Current COMPILER not supported
#endif

//! @defgroup global_config Application configuration
//! @{

#include "conf_scheduler.h" //!< Scheduler tasks declaration


//! target board
//#define TARGET_BOARD STK526
//#include "lib_board\stk_526\stk_526.h"


//! CPU core frequency in kHz
#define FOSC 8000


// -------- END Generic Configuration -------------------------------------





//! @}

#endif // _CONFIG_H_

