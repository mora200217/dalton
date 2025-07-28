TARGET=top
TOP=top
GATE_DIR=build/gateware
SOFT_DIR=build/software
LITEX_DIR=/home/carlos/Embedded/litex/
SERIAL?=/dev/ttyUSB0

all: gateware firmware

${GATE_DIR}/${TARGET}.bit:
	./base2.py

gateware: ${GATE_DIR}/${TARGET}.bit

${SOFT_DIR}/common.mak: gateware

firmware: ${SOFT_DIR}/common.mak
	$(MAKE) -C firmware/ -f Makefile all

litex_term: firmware
	litex_term ${SERIAL} --kernel firmware/hello_world.bin

configure: ${GATE_DIR}/${TARGET}.bit
	sudo openFPGALoader -c ft232RL --pins=0:3:4:1 -m ${GATE_DIR}/${TARGET}.bit
