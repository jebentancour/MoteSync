## Readme

Copiar los archivos ``user-sensor.{c,h}`` y ``zoul-sensors.c`` en ``contiki-ng/arch/platform/zoul/dev``.

Copiar el ``Makefile.zoul`` en ``contiki-ng/arch/platform/zoul/``.

En ``user-sensor.{c,h}`` esta la implementación del dirver.

En ``zoul-sensors.c`` se modifica para agregar el sensor a la lista de sensores del mote.

En ``Makefile.zoul`` se agrega el archivo fuente del driver.
