// EPOS Machine Mediator

#ifndef __machine_h
#define __machine_h

// All machines must provide the following components
#include <machine/machine.h>
#include <machine/ic.h>
#include <machine/timer.h>

// This components have software equivalents in case the machine does not provide them
#include <machine/aes.h>
#include <machine/rtc.h>
#include <machine/display.h>

#ifdef __PCI_H
#include <machine/pci.h>
#endif
#ifdef __FPGA_H
#include <machine/fpga.h>
#endif
#ifdef __SCRATCHPAD_H
#include <machine/scratchpad.h>
#endif
#ifdef __UART_H
#include <machine/uart.h>
#endif
#ifdef __RS485_H
#include <machine/rs485.h>
#endif
#ifdef __SPI_H
#include <machine/spi.h>
#endif
#ifdef __USB_H
#include <machine/usb.h>
#endif
#ifdef __GPIO_H
#include <machine/gpio.h>
#endif
#ifdef __ADC_H
#include <machine/adc.h>
#endif
#ifdef __KEYBOARD_H
#include <machine/keyboard.h>
#endif

#endif
