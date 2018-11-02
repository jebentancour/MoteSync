# Commands

```
make TARGET=zoul savetarget
sudo make clean BOARD=remote-revb

sudo make sender.upload BOARD=remote-revb WITH_TSCH=1 MOTES=/dev/ttyUSB0 NODEID=0x0014
sudo make login BOARD=remote-revb MOTES=/dev/ttyUSB0

sudo make receiver.upload BOARD=remote-revb WITH_TSCH=1 MOTES=/dev/ttyUSB1 NODEID=0x0012
sudo make login BOARD=remote-revb MOTES=/dev/ttyUSB1
```
