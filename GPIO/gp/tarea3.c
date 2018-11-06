/**
 * @file tarea3.c
 * @brief prueba interrupcion gpio
 * @author  Carolina Cabrera - <cabrera.carolina@outlook.com>
 * @date 27/08/2018
 * @version 1.0
 *
 **/

#include "contiki.h"
#include "dev/leds.h"
#include "dev/button-sensor.h"
#include <stdio.h>
#include "dev/gpioc3.h"

/*----------------------------------------------------------------------------*/


PROCESS(publicar, "Publicar");

AUTOSTART_PROCESSES(&publicar);

/*----------------------------------------------------------------------------*/


PROCESS_THREAD(publicar, ev, data)
{
	PROCESS_BEGIN();
	SENSORS_ACTIVATE(gpio_sensor);
	while(1){
		PROCESS_WAIT_EVENT_UNTIL(ev == sensors_event && data == &gpio_sensor);
printf("Flanco en pin C2 \n");
	}
	PROCESS_END();
	
}


