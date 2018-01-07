/**
  \file gpio.h
   
  \author G. Icking-Konert
  \date 2013-11-22
  \version 0.1
   
  \brief declaration of functions/macros for GPIO control
   
  declaration of functions/macros for port pin control
  For speed use macros instead of functions where reasonable
  Optional functionality via #define:
    - USE_PORT_ISR: allow attaching user function to port interrupts (EXTIn)
    - USE_TLI_ISR:  allow attaching user function to interrupt on pin PD7 (TLI) 
*/

/*-----------------------------------------------------------------------------
    MODULE DEFINITION FOR MULTIPLE INCLUSION
-----------------------------------------------------------------------------*/
#ifndef _GPIO_H_
#define _GPIO_H_

/*-----------------------------------------------------------------------------
    INCLUDE FILES
-----------------------------------------------------------------------------*/
#include "stm8as.h"
#include "config.h"
#include "misc.h"


/*-----------------------------------------------------------------------------
    DECLARATION OF GLOBAL MACROS
-----------------------------------------------------------------------------*/

//////////
// port/pin control macros. For PORT_x see file "stm8as.h"
//////////

// rename bits to pins for readability
#define pin0  b0
#define pin1  b1
#define pin2  b2
#define pin3  b3
#define pin4  b4
#define pin5  b5
#define pin6  b6
#define pin7  b7

// pin configurations for below pinMode(). For details see 'stm8as.h'
#define  INPUT                    0x00     ///< configure pin as: input, float, no port interrupt
#define  INPUT_INTERRUPT          0x01     ///< configure pin as: input, float, with port interrupt
#define  INPUT_PULLUP             0x02     ///< configure pin as: input, pull-up, no port interrupt
#define  INPUT_PULLUP_INTERRUPT   0x03     ///< configure pin as: input, pull-up, with port interrupt
#define  OUTPUT_OPENDRAIN_SLOW    0x04     ///< configure pin as: output, open-drain, slow (2MHz)
#define  OUTPUT_OPENDRAIN         0x05     ///< configure pin as: output, open-drain, fast (10MHz)
#define  OUTPUT_SLOW              0x06     ///< configure pin as: output, push-pull, slow (2MHz)
#define  OUTPUT                   0x07     ///< configure pin as: output, push-pull, fast (10MHz)

// edge sensitivities for port/EXINT and pin/TLI interrupts
#define  LOW                      0        ///< EXINT on low level (EXINT). Warning: may stall device!
#define  CHANGE                   1        ///< EXINT on both edges (EXINT)
#define  RISING                   2        ///< EXINT on rising edge (EXINT & TLI)
#define  FALLING                  3        ///< EXINT on falling edge (EXINT & TLI)
#define  TLI       ((PORT_t*) NULL)        ///< TLI pin indicator (for configExintEdge())

/// set pin mode
#define pinMode(port,pin,mode)  { \
   port.DDR.bit.pin = ((mode & 0x04) >> 2); /* input(=0) or output(=1) */ \
   port.CR1.bit.pin = ((mode & 0x02) >> 1); /* input: 0=float, 1=pull-up; output: 0=open-drain, 1=push-pull */ \
   port.CR2.bit.pin = ((mode & 0x01) >> 0); /* input: 0=no exint, 1=exint; output: 0=2MHz slope, 1=10MHz slope */ \
}

/// set port mode
#define portMode(port,dir,cr1, cr2)  { \
   port.DDR.byte = dir;                     /* input(=0) or output(=1) */ \
   port.CR1.byte = cr1;                     /* input: 0=float, 1=pull-up; output: 0=open-drain, 1=push-pull */ \
   port.CR2.byte = cr2;                     /* input: 0=no exint, 1=exint; output: 0=2MHz slope, 1=10MHz slope */ \
}


// access port pins directly (for speed)
#define pinSet(port,pin)    (port.ODR.bit.pin)    ///< pin output state (1 pin)
#define pinRead(port,pin)   (port.IDR.bit.pin)    ///< pin input state (1 pin)
#define portSet(port)       (port.ODR.byte)       ///< port output state (8 pins)
#define portRead(port)      (port.IDR.byte)       ///< port input state (8 pins)


/*-----------------------------------------------------------------------------
    DECLARATION OF GLOBAL FUNCTIONS
-----------------------------------------------------------------------------*/

#if defined(USE_PORTA_ISR) || defined(USE_PORTB_ISR) || defined(USE_PORTC_ISR) || defined(USE_PORTD_ISR) || \
    defined(USE_PORTE_ISR) || defined(USE_PORTF_ISR) || defined(USE_PORT_ISR) || defined(USE_TLI_ISR)

  /// auxiliary macro which allows using port as function argument (else pointer)
  #define configExintEdge(port,edge)    mConfigExintEdge(&port, edge)    
  
  /// configure edge sensitivity for EXINT
  void    mConfigExintEdge(PORT_t *addrPort, uint8_t edge);

#endif

/*-----------------------------------------------------------------------------
    END OF MODULE DEFINITION FOR MULTIPLE INLUSION
-----------------------------------------------------------------------------*/
#endif // _GPIO_H_
