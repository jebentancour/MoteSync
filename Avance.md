## TSCH

En tsch-slot-operation.c

Thread que opera el slot y es llamado por la interrupcion del rtimer. linea 945 (aprox)


/* Protothread for slot operation, called from rtimer interrupt
 * and scheduled from tsch_schedule_slot_operation */
static
PT_THREAD(tsch_slot_operation(struct rtimer *t, void *ptr))

El timer se programa mediante la funcion tsch_schedule_slot_operation que es llamada en el thread (como condicion de un while) en la linea 1091 (aprox).

Es al programar el timer que copio el ASN y el tiempo de inicio (rtimer_clock_t) del proximo slot.

Al imprmir el tiempo y el ASN veo que en realidad lo que se detecta es el inico del slotframe y no de cada timeslot. Esto lo digo porque habiendo seteado el tamaño del slotframe en 3 y la duracion del timeslot en 10ms, la diferencia entre tiempos consecutivos es de 30ms (894 ticks) y los ASN se incrementan en 3. 

## SubGHz

Al activar la radio CC1200 con TSCH se imprime el siguiente mensaje de error: 
	TSCH:! radio does not support getting last packet timestamp. Abort init.
Este print se haec en la linea 839 de tsch.c dada la condición: NETSTACK_RADIO.get_object(RADIO_PARAM_LAST_PACKET_TIMESTAMP, &t, sizeof(rtimer_clock_t)) != RADIO_RESULT_OK.

Viendo en el archivo cc1200.c la funcion static radio_result_t
get_object(radio_param_t param, void *dest, size_t size) (linea 1287) lo unico que hace es retornar e codigo de error RADIO_RESULT_NOT_SUPPORTED, es decir, falta la implementacion de esta función.

