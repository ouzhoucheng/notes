/*
 * Copyright (c) 2013 - 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * THIS SOFTWARE IS PROVIDED BY NXP "AS IS" AND ANY EXPRESSED OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL NXP OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */
/* ###################################################################
**     Filename    : main.c
**     Processor   : MPC574xP
**     Abstract    :
**         Main module.
**         This module contains user's application code.
**     Settings    :
**     Contents    :
**         No public methods
**
** ###################################################################*/
/*!
** @file main.c
** @version 01.00
** @brief
**         Main module.
**         This module contains user's application code.
*/         
/*!
**  @addtogroup main_module main module documentation
**  @{
*/         
/* MODULE main */


/* Including necessary module. Cpu.h contains other modules needed for compiling.*/
#include "Cpu.h"
#include "clockMan1.h"
#include "eTimer0.h"
#include "pin_mux.h"
  volatile int exit_code = 0;
/* User includes (#include below this line is not maintained by Processor Expert) */
#include <stdint.h>
#include <stdbool.h>
void delay(volatile int cycles)
{
    /* Delay function - do nothing for a number of cycles */
    while(cycles--);
}
#define STEP 100
#define MAX_VAL 1000

void ETIMER0_Ch0_IRQHandler(void)
{
	/* this delays the LED toggle, this value should give about 15 seconds between toggles */
#define CNT_TIMEOUT     2
	static uint32_t cnt=CNT_TIMEOUT;
	if(--cnt==0)
	{
		/* RED LED toggle */
		PINS_DRV_TogglePins(PTC,(1 << 11));
		cnt=CNT_TIMEOUT;
	}
	/* need to reset flags */
	ETIMER_DRV_ClearInterruptStatus(0,ETIMER_CH_IRQ_FLAGS_TOF,0);
}
/*! 
  \brief The main function for the project.
  \details The startup initialization sequence is the following:
 * - startup asm routine
 * - main()
*/
int main(void)
{
  /* Write your local variable definition here */
  uint16_t dutyCycle = MAX_VAL;
  bool increaseDutyCycle = false;
  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
  #ifdef PEX_RTOS_INIT
    PEX_RTOS_INIT();                   /* Initialization of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of Processor Expert internal initialization.                    ***/

  /* Write your code here */
  /* For example: for(;;) { } */
  CLOCK_SYS_Init(g_clockManConfigsArr, CLOCK_MANAGER_CONFIG_CNT, g_clockManCallbacksArr, CLOCK_MANAGER_CALLBACK_CNT);
  CLOCK_SYS_UpdateConfiguration(0U, CLOCK_MANAGER_POLICY_AGREEMENT);

  PINS_DRV_Init(NUM_OF_CONFIGURED_PINS, g_pin_mux_InitConfigArr);
  
  FLEXPWM_DRV_SetupPwm(INST_FLEXPWM1, 0U, &flexPWM1_flexpwm_module_setup_t0, &flexPWM1_flexpwm_module_signal_setup_t0);
	FLEXPWM_DRV_CounterStart(INST_FLEXPWM1, 0U);
  FLEXPWM_DRV_UpdatePulseWidth(INST_FLEXPWM1, 0U, 300, 0UL, FlexPwmEdgeAligned);
  FLEXPWM_DRV_LoadCommands(INST_FLEXPWM1, (1UL << 0));

	/* setup ETIMERx channels */
  ETIMER_DRV_Init(INST_ETIMER0);
	ETIMER_DRV_InitChannel(INST_ETIMER0, 0, &eTimer0_etimer_user_channel_config0);
  /* Enable ETIMER0 channel 0 interrupt and install an ISR */
	IRQn_Type etimer0_ch0_irq_id = ETIMER_DRV_GetInterruptNumber(0,ETIMER_IRQ_CH0);
	INT_SYS_InstallHandler(etimer0_ch0_irq_id, &ETIMER0_Ch0_IRQHandler, (isr_t*) 0);
	INT_SYS_EnableIRQ(etimer0_ch0_irq_id);
  /* enable interrupts */
	ETIMER_DRV_EnableInterruptSource(INST_ETIMER0,ETIMER_CH_IRQ_SOURCE_TOFIE,0);
  /* start channel operation */
  ETIMER_DRV_StartTimerChannels(INST_ETIMER0, (ETIMER_ENABLE_CH0));

  PINS_DRV_SetPins(PTC,(1 << 11)|(1 << 12)|(1 << 13));
  while(1)
  {
      /* Insert a small delay to make the blinking visible */
      delay(7200000);
      /* Toggle output value LED1 */
      PINS_DRV_TogglePins(PTC, (1<<12));

      if (increaseDutyCycle == false)
	     {
	         if (dutyCycle < STEP)
	             increaseDutyCycle = true;
	         else
		         dutyCycle -= STEP;
	     }
	     else
	     {
	         if (dutyCycle > MAX_VAL - STEP)
	             increaseDutyCycle = false;
	         else
		         dutyCycle += STEP;
	     }
	     FLEXPWM_DRV_UpdatePulseWidth(INST_FLEXPWM1, 0U, dutyCycle, 0UL, FlexPwmEdgeAligned);
	     FLEXPWM_DRV_LoadCommands(INST_FLEXPWM1, (1UL << 0));
  }
  /*** Don't write any code pass this line, or it will be deleted during code generation. ***/
  /*** RTOS startup code. Macro PEX_RTOS_START is defined by the RTOS component. DON'T MODIFY THIS CODE!!! ***/
  #ifdef PEX_RTOS_START
    PEX_RTOS_START();                  /* Startup of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of RTOS startup code.  ***/
  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for(;;) {
    if(exit_code != 0) {
      break;
    }
  }
  return exit_code;
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

/* END main */
/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.1 [05.21]
**     for the NXP C55 series of microcontrollers.
**
** ###################################################################
*/
