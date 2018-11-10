## Deteccion de inicio del slot y ASN.

Se estudió la implementación de TSCH en Contiki-NG (bajo el directorio os/net/mac/tsch/ ) buscando obtener el tiempo de inicio de cada timeslot y el ASN correspondiente, con la motivación de utilizar la sincoronización provista por TSCH para obtener mayor precisión en el calculo de las distancias entre el lugar donde se produce el evento y los distintos motes. 

En particular es de interés el módulo tsch-slot-operation.{h, c} donde se implementan las funciones vinculadas a la operación de los slots. Se define un thread principal `static PT_THREAD(tsch_slot_operation(struct rtimer *t, void *ptr));` para operación de los slots, el cual se activa dada la interrupción de un rtimer (`struct rtimer *t)`. Para la planificación del rtimer se utiliza la funcion `static uint8_t tsch_schedule_slot_operation(struct rtimer *tm, rtimer_clock_t ref_time, rtimer_clock_t offset, const char *str)`.

El ASN y el inicio del slot actual se guardan en las variables ´static rtimer_clock_t volatile current_slot_start´ y ´struct tsch_asn_t tsch_current_asn´.

Para obtener el inicio de slot y el ASN se agregan al modulo tsch-slot-operation las funciones `struct tsch_asn_t get_asn(void)` y `uint32_t get_slot_start(void)` y se crearon las variables auxiliares  `uint32_t event_ref` y `struct tsch_asn_t event_ASN` para realizar una copia de las variables `current_slot_start` y `tsch_slot_operation`. 

Los prototipos de estas funciones se encuentran en os/net/mac/tsch/tsch-sync.h. 


## Driver para el sensor

El driver se implementa en los arcivos user-sensor.{h,c}, bajo el directorio arch/zoul/dev/. 
Además se modifcó el archivo arch/platform/zoul/dev/zoul-sensors.c para agregar el sensor de sonido. En este archivo, mediante el macro  `SENSORS`, se incluyen los sensores a la lista que maneja el módulo.

### Interfaz publica del módulo

Se define el pin donde se conecta el sensor, mediante las siguientes constantes:

```c
#ifndef USER_SENSOR_PORT
#define USER_SENSOR_PORT       GPIO_C_NUM
#endif
#ifndef USER_SENSOR_PIN
#define USER_SENSOR_PIN        3
#endif
#ifndef USER_SENSOR_VECTOR
#define USER_SENSOR_VECTOR     GPIO_C_IRQn
#endif
```

Por defecto se utiliza el pin C3, pero puede configurarse otro redefiniendo estas constantes en el project-conf.h del proyecto.

Se crea la siguiente estructura para almacenar en el handler de la interrupcion generada por sensor el tiempo de ocurrencia de la interrupcion y el tiempo de inicio y ASN del slot de referencia.

```c
struct data_sensor_t {
  uint32_t ref_time; 
  uint32_t asn_ls4b;
  uint8_t asn_ms1b;
  uint32_t event_time; 
};
```

Este módulo además cuenta con la función pública `struct data_sensor_t get_data_sensor(void)`, que devuelve la estructura con el tiempo de ocurrencia y los datos del slot de referencia del último evento ocurrido.

Esta función se utiliza en el proceso principal de los nodos clientes al detectarse un evento del sensor para obtener los datos y enviarlos al server.

### Implementación del driver

Se configura el pin C3 para generar interrupciones ante la ocurrencia de flancos de bajada. Estas interrupciones son manejadas por el módulo del sistema *sensors* (os/lib/sensors.{h,c}), desde donde se postea el evento a todos los procesos (`PROCESS_BROADCAST`) pasando como data el sensor que generó la interrupción.

Para que el sensor pueda ser manejado por el modulo *sensors* se define una estructura, mediante el macro `SENSOR_SENSORS`: 

```c
#define SENSORS_SENSOR(name, type, value, configure, status)        \
const struct sensors_sensor name = { type, value, configure, status }
```

Donde el tipo `struct sensors_sensor ` está definido como sigue:

```c
struct sensors_sensor {
  char *       type;
  int          (* value)     (int type);
  int          (* configure) (int type, int value);
  int          (* status)    (int type);
};
```

De los miembros de esta estructura, los punteros a funciones *value* y *status* son opcionales. Para el sensor en cuestión no son necesarias, por lo que no se definen, y al definir el sensor con el macro SENSOR_SENSORS los argumentos value y status son NULL.

La función *configure*, es llamada al inicializar (o desactivar) el sensor desde la aplicación con el macro SENSORS_ACTIVATE(sensor). Es aquí donde se realizan las configuraciones necesarias para generar interrupciones a partir de los flancos de bajada detectados en el pin C3.

```c
static int
configure(int type, int value)
{
  if(type != USER_ACTIVE) {
    return USER_ERROR;
  }
  if(!value) /* Disable sensor*/
  { 
    GPIO_DISABLE_INTERRUPT(USER_SENSOR_PORT_BASE, USER_SENSOR_PIN_MASK);
    return USER_SUCCESS;
  }
  else /* Enable sensor*/
  {
    GPIO_SOFTWARE_CONTROL(USER_SENSOR_PORT_BASE, USER_SENSOR_PIN_MASK);
    GPIO_SET_INPUT(USER_SENSOR_PORT_BASE, USER_SENSOR_PIN_MASK);
    GPIO_DETECT_FALLING(USER_SENSOR_PORT_BASE, USER_SENSOR_PIN_MASK);
    GPIO_TRIGGER_SINGLE_EDGE(USER_SENSOR_PORT_BASE, USER_SENSOR_PIN_MASK);
    ioc_set_over(USER_SENSOR_PORT, USER_SENSOR_PIN, IOC_OVERRIDE_DIS);
    gpio_hal_register_handler(&user_handler);
    GPIO_ENABLE_INTERRUPT(USER_SENSOR_PORT_BASE, USER_SENSOR_PIN_MASK);
    NVIC_EnableIRQ(USER_SENSOR_VECTOR);
  }
  return USER_SUCCESS;
}
```

También se implemnta en este módulo el handler para la interrupción generada por el sensor:

```c
static void
user_interrupt_handler(gpio_hal_pin_mask_t pin_mask)
{
  event_time = RTIMER_NOW();
  if(!timer_expired(&filtertimer))
  {
    return;
  }
  else
  {
	timer_set(&filtertimer, USER_SENSOR_FILTER_DURATION);
    data_sensor.ref_time = get_slot_start();
    event_asn = get_asn();
    data_sensor.asn_ls4b = event_asn.ls4b;
    data_sensor.asn_ms1b = event_asn.ms1b;
    data_sensor.event_time = event_time;
  	sensors_changed(&user_sensor);
  }
}
```

Al generarse una interrupción se guarda en la estructura de datos del modulo el tiempo de ocurrencia de la interrupciones (`event_time`) y el inicio y ASN del slot actual. El inicio de slot y el ASN se obtienen mediante las funciones `get_asn()´ y ´uint32_t get_slot_start()`, y el tiempo de ocurrencia del evento mediante el macro `RTIMER_NOW();`, definido en os/sys/rtimer.h.
Luego se llama a la función sensors_changed en la cual se hace un poll al proceso sensors_process, proceso principal del modulo *sensors*. 

### Programa de prueba del driver.

Para probar el sensor se realizó un programa que consiste en un proceso que espera por un evento en el sensor (`PROCESS_WAIT_EVENT_UNTIL(ev == sensors_event && data == &user_sensor);`), llama a la funcion `get_data_sensor();` e imprime el tiempo de ocurrencia de la interrupción.
