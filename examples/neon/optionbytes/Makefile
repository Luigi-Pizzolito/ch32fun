all : flash monitor
TARGET:=optionbytes

CFLAGS+=-DTINYVECTOR

# ch32fun:=../../../ch32fun/ch32fun
# MINICHLINK:=../../../ch32fun/minichlink
TARGET_MCU?=CH32V003

include ../../../ch32fun/ch32fun.mk

flash : cv_flash
clean : cv_clean