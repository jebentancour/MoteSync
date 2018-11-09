### Detección de inicio de slotframe y ASN.

Se estudió la implementación de TSCH en Contiki-NG (bajo el directorio os/net/mac/tsch/ ) buscando obtener el tiempo de inicio de cada timeslot y el ASN correspondiente, con la motivación de utilizar la sincoronización provista por TSCH para obtener mayor precisión en el calculo de las distancias entre el lugar donde se produce el evento y los distintos motes. 

En particular es de interés el módulo tsch-slot-operation.{h, c} donde se implementan las funciones vinculadas a la operación de los slots. Se define un thread principal `static PT_THREAD(tsch_slot_operation(struct rtimer *t, void *ptr));` para operación de los slots, el cual se activa dada la interrupción de un rtimer (`struct rtimer *t)`. Para la planificación del rtimer se utiliza la funcion `static uint8_t tsch_schedule_slot_operation(struct rtimer *tm, rtimer_clock_t ref_time, rtimer_clock_t offset, const char *str)`...................

### Driver para el sensor.

El driver para el sensor se implementa en el módulo *user-sensor* (arch/zoul/dev/user-sensor.{c, h}). Además se modifcó el archivo arch/platform/zoul/dev/zoul-sensors.c para agregar el sensor de sonido. En este archivo, mediante el macro  `SENSORS`, se incluyen los sensores a la lista que maneja el módulo.

Se configura el pin C3 para generar interrupciones ante flancos de bajada. Estas interrupciones son manejadas por el módulo del sistema *sensors* (os/lib/sensors.{h,c}), desde donde se postea el evento a todos los procesos (`PROCESS_BROADCAST `) pasando como data el sensor.

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

De los miembros de esta estructura, los punteros a funciones *value*y *status* son opcionales. Para el sensor en cuestión no son necesarias, por lo que no se definen, y al definir el sensor con el macro SENSOR_SENSORS los argumentos value y status son NULL.

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
static void user_interrupt_handler(gpio_hal_pin_mask_t pin_mask)
{
  if(!timer_expired(&filtertimer))
  {
    return;
  }
  else
  {
	timer_set(&filtertimer, USER_SENSOR_FILTER_DURATION);
  	sensors_changed(&user_sensor);
  }
}
```

Al generarse una interrupción se llama a la función sensors_changed en la cual se hace un poll al proceso sensors_process, proceso principal del modulo *sensors*. Además se utiliza un timer para evitar detectar duplicados debido a un evento muy largo. 
