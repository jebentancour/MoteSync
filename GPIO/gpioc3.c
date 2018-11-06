/*
 * gpioc3.c
 * Este archivo va en contiki/platform/zoul/dev
 *  Created on: Nov 6, 2018
 *      Author: user
 */

/*
 * Copyright (c) 2012, Texas Instruments Incorporated - http://www.ti.com/
 * Copyright (c) 2015, Zolertia - http://www.zolertia.com
 * Copyright (c) 2015, University of Bristol - http://www.bristol.ac.uk
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/*---------------------------------------------------------------------------*/
/**
 * \addtogroup zoul-gpio-sensor
 * @{
 *
 * \file
 *
 */
/*---------------------------------------------------------------------------*/
#include "contiki.h"
#include "dev/nvic.h"
#include "dev/ioc.h"
#include "dev/gpio.h"
#include "dev/gpioc3.h"
#include "sys/timer.h"
#include "sys/ctimer.h"
#include "sys/process.h"

#include <stdint.h>
#include <string.h>
/*---------------------------------------------------------------------------*/
#define GPIO_USER_PORT_BASE  GPIO_PORT_TO_BASE(GPIO_USER_PORT)
#define GPIO_USER_PIN_MASK   GPIO_PIN_MASK(GPIO_USER_PIN)
/*---------------------------------------------------------------------------*/
#define DEBOUNCE_DURATION_GPIO (CLOCK_SECOND >> 4)

static struct timer debouncetimergpio;
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/**
 * \brief Returns the pin level
 */
static int
value(int type)
{
    return GPIO_READ_PIN(GPIO_USER_PORT_BASE, GPIO_USER_PIN_MASK);
}
/*---------------------------------------------------------------------------*/
/**
 * \brief Callback registered with the GPIO module. Gets fired with a
 * pin generates an interrupt
 * \param port The port number that generated the interrupt
 * \param pin The pin number that generated the interrupt. This is the pin
 * absolute number (i.e. 0, 1, ..., 7), not a mask
 */
static void
gpio_user_callback(uint8_t port, uint8_t pin)
{
  if(!timer_expired(&debouncetimergpio))
  {
    return;
  }
  else
  {
	timer_set(&debouncetimergpio, DEBOUNCE_DURATION_GPIO);
  	sensors_changed(&gpio_sensor);
  }
}
/*---------------------------------------------------------------------------*/
/**
 * \brief Init function
 * \param type SENSORS_ACTIVE: Activate / Deactivate the sensor (value == 1
 *             or 0 respectively)
 *
 * \param value Depends on the value of the type argument
 * \return Depends on the value of the type argument
 */
static int
config_user(int type, int value)
{
  switch(type) {
  case SENSORS_HW_INIT:
    /* Software controlled */
    GPIO_SOFTWARE_CONTROL(GPIO_USER_PORT_BASE, GPIO_USER_PIN_MASK);

    /* Set pin to input */
    GPIO_SET_INPUT(GPIO_USER_PORT_BASE, GPIO_USER_PIN_MASK);

    /* Enable edge detection */
    GPIO_DETECT_EDGE(GPIO_USER_PORT_BASE, GPIO_USER_PIN_MASK);

    /* Both Edges */
    GPIO_TRIGGER_BOTH_EDGES(GPIO_USER_PORT_BASE, GPIO_USER_PIN_MASK); //Flancos??

    ioc_set_over(GPIO_USER_PORT, GPIO_USER_PIN, IOC_OVERRIDE_PUE); //PUE = Pull up Enable

    gpio_register_callback(gpio_user_callback, GPIO_USER_PORT, GPIO_USER_PIN);
    break;
  case SENSORS_ACTIVE:
    if(value) {
      GPIO_ENABLE_INTERRUPT(GPIO_USER_PORT_BASE, GPIO_USER_PIN_MASK);
      NVIC_EnableIRQ(GPIO_USER_VECTOR);
    } else {
      GPIO_DISABLE_INTERRUPT(GPIO_USER_PORT_BASE, GPIO_USER_PIN_MASK);
      NVIC_DisableIRQ(GPIO_USER_VECTOR);
    }
    return value;
    break;
  default:
    break;
  }

  return 1;
}
/*---------------------------------------------------------------------------*/
SENSORS_SENSOR(gpio_sensor, GPIO_SENSOR, value, config_user, NULL);
/*---------------------------------------------------------------------------*/
/** @} */

