/**
 * SPDX-License-Identifier: BSD-3-Clause
 * (les kai tha ton parei kapoios allos 
 * tetoion kwdika, alla de gamietai)
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/platform.h"
#include "pico/printf.h"
#include "hardware/uart.h"
#include "hardware/irq.h"
#include "bsp/board.h"
#include "tusb.h"
#include "../mitsoxmlib/mxml.h"

#define YAURTI_ID		uart0
#define YAURTI_BAUD_RATE	115200
#define YAURTI_TX_PIN		0
#define YAURTI_RX_PIN		1
#define LAMBAQI_PIN		PICO_DEFAULT_LED_PIN
#define HID_MAX_REPORT		4
#define BUFLEN 			512

