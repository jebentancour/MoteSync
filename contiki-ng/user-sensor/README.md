## Readme

La implementación del driver está en user-sensor.{c,h}. 
Además se modifica el archivo Makefile.zoul para agregar el codigo fuente del sensor, y también el archivo zoul-sensors.c para incluir al sensor en la lista que maneja el módulo *sensors*.

El archivo Makefile.zoul va en arch/platform/zoul. Se modifica para agregar el codigo fuente del driver user-sensor.c
Los archios user-sensor.{c,h} y zoul-sensors.c van en arch/platform/zoul/dev.
