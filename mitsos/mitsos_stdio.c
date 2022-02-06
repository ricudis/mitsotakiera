


#include "mitsos.h"

bool stdio_usb_connected(void) {
    return tud_cdc_connected();
}

static void stdio_usb_out_chars(const char *buf, int length) {
    static uint64_t last_avail_time;
    if (tud_cdc_connected()) {
        for (int i = 0; i < length;) {
            int n = length - i;
            int avail = (int) tud_cdc_write_available();
            if (n > avail) n = avail;
            if (n) {
                int n2 = (int) tud_cdc_write(buf + i, (uint32_t)n);
                tud_task();
                tud_cdc_write_flush();
                i += n2;
                last_avail_time = time_us_64();
            } else {
                tud_task();
                tud_cdc_write_flush();
                if (!tud_cdc_connected() ||
                    (!tud_cdc_write_available() && time_us_64() > last_avail_time + PICO_STDIO_USB_STDOUT_TIMEOUT_US)) {
                    break;
                }
            }
        }
    } else {
        // reset our timeout
        last_avail_time = 0;
    }
}

static int stdio_usb_in_chars(char *buf, int length) {
    int rc = PICO_ERROR_NO_DATA;
    if (tud_cdc_connected() && tud_cdc_available()) {
        int count = (int) tud_cdc_read(buf, (uint32_t) length);
        rc = count ? count : PICO_ERROR_NO_DATA;
    }
    return rc;
}

static stdio_driver_t stdio_usb_external = {
    .out_chars = stdio_usb_out_chars,
    .in_chars = stdio_usb_in_chars,
#if PICO_STDIO_ENABLE_CRLF_SUPPORT
    .crlf_enabled = PICO_STDIO_USB_DEFAULT_CRLF
#endif
};

void stdio_usb_external_init(void)
{
    stdio_set_driver_enabled(&stdio_usb_external, true);
}