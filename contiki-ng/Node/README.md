## README

Esta carpeta va en ``contiki-ng/examples/``.

Programas principales de los nodos: ``client.c`` y ``server.c``.

Bash script ``compilar.sh`` para compilar server en ``/dev/ttyUSB0`` y client en ``/dev/ttyUSB1``. Además hace make login del server.
Para ejecutar el script correr ``./compilar.sh`` en la terminal (estando en el directorio ``contiki-ng/examples/Node``).
Si no deja correr cambiarle permisos al archivos para que sea ejecutable corriendo el comando ``chmod +x compilar.sh``.

```
make clean
make sender.upload MOTES=/dev/ttyUSB0 NODEID=0x0014
make login MOTES=/dev/ttyUSB0

make clean
make receiver.upload MOTES=/dev/ttyUSB1 NODEID=0x0012
make login MOTES=/dev/ttyUSB1
```

# Funcionamiento

El módulo implementado es ``decoder``, el mismo posee dos funciones una es ``decoder_receiver`` encargada de procesar recibir los datos de los otros nodos y la otra ``decoder_process`` encargada de realizar los cálculos necesarios para detectar el evento.

Los datos son transmitidos utilizando la estructura ``my_msg_t`` definida en ``data.h``, la misma incluye:

- ``uint8_t id`` ID del nodo.
- ``uint32_t x_pos`` posición en el eje x en cm.
- ``uint32_t y_pos`` posición en el eje x en cm.
- ``uint32_t event_asn_ls4b`` 4 bytes menos significativos del ASN en que se detectó el evento.
- ``uint8_t event_asn_ms1b`` byte más significativo del ASN en que se detectó el evento.
- ``uint32_t event_offset`` offset en Ticks desde el inicio del Slot Frame en que se detectó el evento.

Se definen constantes para el sistema:

```c
#define TICKS_PER_S 32768         // Ticks por segundo
#define TIMESLOTS_PER_S 100       // Cantidad de Time Slots en un segundo, vale 100 para Time Slots de 10 ms
#define PROPAGATION_CM_S 33100    // Velocidad de propagación en cm/s, valor correspondiente a 331 m/s de la
                                  // velocidad del sonido en el aire
```

El módulo mantiene una lista de nodos con sus datos y una bandera que indica que posee datos válidos, además mantiene la cuenta de la cantidad de nodos que hay enviando información. Los nodos son identificados con el ID que envían en el paquete de datos, no se hacen verificaciones sobre este dato, por ejemplo la IP. Si bien en esta implementación los nodos tienen si posición fija y está determinada al momento de su progración, este módulo permite que la posición enviada sea variable. No se quitan nodos de la lista, solo se agregan si llegan datos con un ID distinto a los ya conocidos.

Los datos correspondiente al evento son verificados antes de actualizarse. Una de las tareas que se realiza en la función ``decoder_process`` es calcuar la distancia máxima entre nodos, este valor es usado para validar los datos que envían los nodos. Un nuevo valor de ASN y offset es marcado cómo válido solo si tiene una diferencia equivalente en tiempo de al menos la distancia máxima entre nodos.

La función ``decoder_process`` debe ser llamada periódicamente por el proceso principal para llebara cabo la tarea de proceamiento de datos. Se espera a tener al menos tres nodos en la lista para comenzar a realizar los cálculos. Cuando se cuenta con la informaciónde tres nodos o más se calcula la distancia máxima ente ellos y se verifica que todos estén marcados con la bandera de dato válido. En caso de que todos los datos sean válidos se procede a calcular el epicentro, para ello se realiza el siguiente algoritmo:

1. Se determina cuál fue el primer nodo en detectar el evento.
2. Se marca el inicio del evento igual al momento en en que es detectado por primera vez. 
3. Se determina el área a escanear.
4. Para cada x e y en el área y para cada nodo se calcula:
   1. La distancia al nodo.
   2. El radio equivalente en distancia desde el inicio del evento a el momento en que el nodo lo detecta.
   3. Si la distancia al nodo es menor al radio de detección el algoritmo termina, de lo contratio se retrasa el inicio del evento y se vuelve al paso 4.

El algoritmo es muy ineficiente dado que necesita una gran cantidad de iteraciones para determinar el epicentro, es tanto así que debimos reducir el área de escaneo a un área de 20 cm x 20 cm para que el programa pueda ser ejecutado en tiempos razonables en el RE-mote.

En el estado acual se puede utilizar para detectar el evento en un área específica del espacio, de lo contrario se debería o bien mejorar el algoritmo o bien enviar los datos a un nodo con mayor capacidad de procesamiento.
