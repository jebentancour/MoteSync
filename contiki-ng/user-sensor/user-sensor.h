/**
*   Proyecto RSI - MoteSync
*   Año: 2018
*   Grupo: Bentancour, Cabrera, Díaz
*
*   path: $(CONTIKI)/arch/platform/zoul/dev/user-sensor.h
*
*   Basado en $(CONTIKI)/arch/platform/zoul/dev/motion-sensor.h
*   
*/

#include "lib/sensors.h"
/* -------------------------------------------------------------------------- */
#ifndef USER_SENSOR_H_
#define USER_SENSOR_H_
/* -------------------------------------------------------------------------- */

/**
 * User-sensor return and operation values
 */
#define USER_ACTIVE     SENSORS_ACTIVE
#define USER_SUCCESS    0
#define USER_ERROR      (-1)

/* -------------------------------------------------------------------------- */

/**
 * Identifier for the sensor in the sensors os module.
 */
#define USER_SENSOR "user_sensor"

/* -------------------------------------------------------------------------- */

/**
 * Identifier for the sensor in the sensors os module.
 */
extern const struct sensors_sensor user_sensor;

/* -------------------------------------------------------------------------- */
#endif /* ifndef USER_SENSOR_H_ */