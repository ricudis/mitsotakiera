/**
 * SPDX-License-Identifier: BSD-3-Clause
 * (les kai tha ton parei kapoios allos
 * tetoion kwdika, alla de gamietai)
 */

// To prevent PICO SDK messing with our device config
#define _PICO_STDIO_USB_TUSB_CONFIG_H 

#include <stdio.h>
#include "pico/platform.h"
#include "pico/stdlib.h"
#include "pico/time.h"
#include "pico/stdio/driver.h"
#include "pico/binary_info.h"
#include "pico/printf.h"
#include "hardware/uart.h"
#include "hardware/irq.h"
#include "hardware/flash.h"
#include "pico/unique_id.h"
#include "bsp/board.h"
#include "tusb.h"
#include "../mitsoxmlib/mxml.h"

// 20 msec is the military standard timing for 
// debouncing switches. No seriously, it is.
// I read it somewhere in the internet.
#define SARANTAENNIA		20

#define YAURTI_ID               uart0
#define YAURTI_BAUD_RATE        115200
#define YAURTI_TX_PIN           0
#define YAURTI_RX_PIN           1
#define LAMBAQI_PIN             PICO_DEFAULT_LED_PIN
#define QUMBI_PIN		20
#define BUFLEN                  512

typedef enum {
    STATE_ANY,
    STATE_A,
    STATE_AK,
    STATE_AKI
} mpes_state_t;

// After writing this
#define UNHEXY(n) (n>=0x30&&n<=0x39?n-0x30:n>=0x41&&n<=0x46?n-0x41+10:n>=0x61&&n<=0x66?n-0x61+10:0)
// I feel like a real Empedded Zystems Endjinir

enum
{
  REPORT_ID_KEYBOARD = 1,
  REPORT_ID_MAX
};

void stdio_usb_external_init(void);