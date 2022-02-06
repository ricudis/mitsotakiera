/**
 * SPDX-License-Identifier: BSD-3-Clause
 * (les kai tha ton parei kapoios allos
 * tetoion kwdika, alla de gamietai)
 */

#include "mitsos.h"

#define _PID_MAP(itf, n)  ( (CFG_TUD_##itf) << (n) )
#define USBD_VID 	   (0x2E8A) // Raspberry Pi
#define USBD_PID           (0x4000 | _PID_MAP(CDC, 0) | _PID_MAP(MSC, 1) | _PID_MAP(HID, 2) | _PID_MAP(MIDI, 3) | _PID_MAP(VENDOR, 4) )

#define USBD_DESC_LEN	(TUD_CONFIG_DESC_LEN + TUD_CDC_DESC_LEN + TUD_HID_DESC_LEN)
#define USBD_MAX_POWER_MA (250)

#define USBD_CDC_EP_CMD (0x81)
#define USBD_CDC_EP_OUT (0x02)
#define USBD_CDC_EP_IN (0x82)
#define USBD_CDC_CMD_MAX_SIZE (8)

#define USBD_HID_EP 0x84

enum
{
  USBD_STR_0 = 0,
  USBD_STR_MANUF,
  USBD_STR_PRODUCT,
  USBD_STR_SERIAL,
  USBD_STR_CDC,
  USBD_STR_HID
};

enum
{
  ITF_NUM_CDC = 0,
  ITF_NUM_CDC_DATA,
  ITF_NUM_HID,
  ITF_NUM_TOTAL
};

// Note: descriptors returned from callbacks must exist long enough for transfer to complete

static const tusb_desc_device_t usbd_desc_device = {
    .bLength = sizeof(tusb_desc_device_t),
    .bDescriptorType = TUSB_DESC_DEVICE,
    .bcdUSB = 0x0200,
    .bDeviceClass = TUSB_CLASS_MISC,
    .bDeviceSubClass = MISC_SUBCLASS_COMMON,
    .bDeviceProtocol = MISC_PROTOCOL_IAD,
    .bMaxPacketSize0 = CFG_TUD_ENDPOINT0_SIZE,
    .idVendor = USBD_VID,
    .idProduct = USBD_PID,
    .bcdDevice = 0x0100,
    .iManufacturer = USBD_STR_MANUF,
    .iProduct = USBD_STR_PRODUCT,
    .iSerialNumber = USBD_STR_SERIAL,
    .bNumConfigurations = 1,
};

uint8_t const desc_hid_keyboard_report[] =
{
  TUD_HID_REPORT_DESC_KEYBOARD(HID_REPORT_ID(REPORT_ID_KEYBOARD))
};

uint8_t const * tud_hid_descriptor_report_cb(uint8_t instance)
{
  return desc_hid_keyboard_report;
};

static const uint8_t usbd_desc_cfg[USBD_DESC_LEN] = {
    TUD_CONFIG_DESCRIPTOR(1, ITF_NUM_TOTAL, USBD_STR_0, USBD_DESC_LEN, 0, USBD_MAX_POWER_MA),
    TUD_CDC_DESCRIPTOR(ITF_NUM_CDC, USBD_STR_CDC, USBD_CDC_EP_CMD, USBD_CDC_CMD_MAX_SIZE, USBD_CDC_EP_OUT, USBD_CDC_EP_IN, CFG_TUD_CDC_EP_BUFSIZE),
    TUD_HID_DESCRIPTOR(ITF_NUM_HID, USBD_STR_HID, HID_ITF_PROTOCOL_NONE, sizeof(desc_hid_keyboard_report), USBD_HID_EP, CFG_TUD_HID_EPIN_BUFSIZE, 5),
};

static char usbd_serial_str[PICO_UNIQUE_BOARD_ID_SIZE_BYTES * 2 + 1] = { 0 };

static const char *const usbd_desc_str[] = {
    [USBD_STR_MANUF] = "Rikafr Systems",
    [USBD_STR_PRODUCT] = "Mitsos",
    [USBD_STR_SERIAL] = usbd_serial_str,
    [USBD_STR_CDC] = "Debug Serial",
    [USBD_STR_HID] = "Kalws syngerasmeno kleidokymvalo",
};

const uint8_t *tud_descriptor_device_cb(void) {
    return (const uint8_t *)&usbd_desc_device;
}

const uint8_t *tud_descriptor_configuration_cb(__unused uint8_t index) {
    return usbd_desc_cfg;
}

const uint16_t *tud_descriptor_string_cb(uint8_t index, __unused uint16_t langid) {
    #define DESC_STR_MAX (20)
    static uint16_t desc_str[DESC_STR_MAX];

    // Assign the SN using the unique flash id
    if (!usbd_serial_str[0]) {
        pico_get_unique_board_id_string(usbd_serial_str, sizeof(usbd_serial_str));
    }

    uint8_t len = 0;
    
    if (index == 0) {
        desc_str[1] = 0x0409; // supported language is English
        len = 1;
    } else {
        if (index >= sizeof(usbd_desc_str) / sizeof(usbd_desc_str[0])) {
            return NULL;
        }
        const char *str = usbd_desc_str[index];
        for (len = 0; len < DESC_STR_MAX - 1 && str[len]; ++len) {
            desc_str[1 + len] = str[len];
        }
    }

    // first byte is length (including header), second byte is string type
    desc_str[0] = (uint16_t) ((TUSB_DESC_STRING << 8) | (2 * len + 2));

    return desc_str;
}

