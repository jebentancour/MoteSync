## Readme 

El archivo compilar.sh es un bash script. Si lo ponen en la misma carpeta que el resto del codigo y lo corren deberia de compilar receiver y sender.

El único detalle es que asume que uno de los motes esta conectado en /dev/ttyUSB0 y el otro en /dev/ttyUSB1.
En mi caso con la maquina virtual no me dejaba conectar dos motes a la vez, los reconocia al conectarlos pero me mapeaba uno solo en /dev/ttyUSB0. Por eso me instalé todo directo en mi máquina y no uso mas la VM. 

Si les pasa esto mismo simplemente copian las lineas de compilar.sh en la terminal (salvo la primera: #!/bin/bash) y compilan de a un mote. 
