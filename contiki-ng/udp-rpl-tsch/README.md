## Readme

Poner esta carpeta en contiki-ng/examples
Para compilar el server: 

´´´
make clean
make PORT=/dev/ttyUSB0 server.upload
´´´

Para compilar los clients:

make clean
make PORT=/dev/ttyUSB1 NODEID=0x0013 client.upload
´´´
Poner un NODEID diferente para cada client, el NODEID=0x0013 es a modo de ejemplo.

