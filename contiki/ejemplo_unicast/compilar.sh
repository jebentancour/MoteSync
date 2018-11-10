#!/bin/bash
make TARGET=zoul savetarget
make clean BOARD=remote-revb
make sender.upload BOARD=remote-revb MOTES=/dev/ttyUSB0
make clean BOARD=remote-revb
make receiver.upload BOARD=remote-revb MOTES=/dev/ttyUSB1 NODEID=0x0013
make login BOARD=remote-revb MOTES=/dev/ttyUSB1
