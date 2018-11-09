/**
 * @file tarea3.c
 * @brief prueba interrupcion gpio
 * @author  Carolina Cabrera - <cabrera.carolina@outlook.com>
 * @date 27/08/2018
 * @version 1.0
 *
 **/

#include "contiki.h"
#include <stdio.h>
#include "dev/user-sensor.h"

/*----------------------------------------------------------------------------*/

static unsigned int count;
PROCESS(test_sensor, "Test user sensor");

AUTOSTART_PROCESSES(&test_sensor);

/*----------------------------------------------------------------------------*/


PROCESS_THREAD(test_sensor, ev, data)
{
	PROCESS_BEGIN();
	SENSORS_ACTIVATE(user_sensor);
	while(1){
		PROCESS_WAIT_EVENT_UNTIL(ev == sensors_event && data == &user_sensor);
		printf("Flanco %u en pin C3 \n",count);
		count++;
	}
	PROCESS_END();
	
}


