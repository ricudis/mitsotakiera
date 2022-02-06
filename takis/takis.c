/**
 * SPDX-License-Identifier: BSD-3-Clause
 * (les kai tha ton parei kapoios allos 
 * tetoion kwdika, alla de gamietai)
 */

#include "takis.h"

static bool lambaqi_state = false;

static struct {
  uint8_t report_count;
  tuh_hid_report_info_t report_info[HID_MAX_REPORT];
} hid_info[CFG_TUH_HID];

static char xml_mpoufes[BUFLEN + 1];
static char vsprintf_mpoufes[BUFLEN + 1];

void
mxml_printf(const char *fmt, ...)
{
  mxml_node_t *xml;
  mxml_node_t *debug_msg;
  
  mxmlSetWrapMargin(0);

  xml = mxmlNewXML("1.0");
  debug_msg = mxmlNewElement(xml, "mitsodebug");
  
  va_list ap;
  va_start(ap, fmt);
  vsnprintf(vsprintf_mpoufes, BUFLEN, fmt, ap);
  mxmlNewText(debug_msg, 0, vsprintf_mpoufes);
  mxmlSaveString(xml, xml_mpoufes, BUFLEN, MXML_NO_CALLBACK);
  printf("%s", xml_mpoufes);
  mxmlDelete(xml);
  va_end(ap);
}

void mxml_keyboard_report_print(hid_keyboard_report_t const *report)
{
  mxml_node_t *xml;
  mxml_node_t *hid_report;
  mxml_node_t *keyboard_report;
  mxml_node_t *modifier;
  mxml_node_t *reserved;
  mxml_node_t *keycode[6];
  
  xml = mxmlNewXML("1.25b3");
  hid_report = mxmlNewElement(xml, "mitsohid");
  keyboard_report = mxmlNewElement(hid_report, "mitsotakey");
  modifier = mxmlNewElement(keyboard_report, "mod");
  mxmlNewInteger(modifier, report->modifier);
  reserved = mxmlNewElement(keyboard_report, "res");
  mxmlNewInteger(reserved, report->reserved);
  
  for (int i = 0; i < 6; i++) {
    keycode[i] = mxmlNewElement(keyboard_report, "kc");
    mxmlNewInteger(keycode[i], report->keycode[i]);
  }
  
  mxmlSaveString(xml, xml_mpoufes, BUFLEN, MXML_NO_CALLBACK);
  printf("%s", xml_mpoufes);
  mxmlDelete(xml);
}

static void marafetia_init(void)
{
    lambaqi_state = false;
    gpio_init(LAMBAQI_PIN);
    gpio_set_dir(LAMBAQI_PIN, GPIO_OUT);

    uart_init(YAURTI_ID, YAURTI_BAUD_RATE);
 
    gpio_set_function(YAURTI_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(YAURTI_RX_PIN, GPIO_FUNC_UART);

    int YAURTI_IRQ = (YAURTI_ID == uart0 ? UART0_IRQ : UART1_IRQ);

    irq_set_enabled(YAURTI_IRQ, false);
    uart_set_fifo_enabled(YAURTI_ID, true);
    uart_set_hw_flow(YAURTI_ID, false, false);
    uart_set_irq_enables(YAURTI_ID, false, false);
    
    tusb_init();
}

static void lambaqi_toggle(void)
{
    gpio_put(LAMBAQI_PIN, lambaqi_state);
    lambaqi_state = !lambaqi_state;
}

static bool lambaqi_timer_cb(struct repeating_timer *t)
{
    lambaqi_toggle();
    return true;
}

static void process_kbd_report(hid_keyboard_report_t const *report)
{
  mxml_keyboard_report_print(report);
}

void tuh_hid_mount_cb(uint8_t dev_addr, uint8_t instance, const uint8_t * desc_report, uint16_t desc_len)
{
  uint16_t vid, pid;
  tuh_vid_pid_get(dev_addr, &vid, &pid);

  const char *protocol_str[] = { "None", "Keyboard", "Mouse" };
  uint8_t const itf_protocol = tuh_hid_interface_protocol(dev_addr, instance);
  mxml_printf("MOUNTED : HID device address = %d, instance = %d VID = %04x, PID = %04x, HID Interface Protocol = %s",
      dev_addr, instance, vid, pid, protocol_str[itf_protocol]);
  
  // By default host stack will use activate boot protocol on supported interface.
  if (itf_protocol == HID_ITF_PROTOCOL_NONE)
  {
    hid_info[instance].report_count = tuh_hid_parse_report_descriptor(hid_info[instance].report_info, HID_MAX_REPORT, desc_report, desc_len);
    mxml_printf("HID has %u reports", hid_info[instance].report_count);
  }

  // request to receive report
  // tuh_hid_report_received_cb() will be invoked when report is available
  if (!tuh_hid_receive_report(dev_addr, instance))
  {
    mxml_printf("Error: cannot request to receive report");
  }
}

// Invoked when device with hid interface is un-mounted
void tuh_hid_umount_cb(uint8_t dev_addr, uint8_t instance)
{
  mxml_printf("HID device address = %d, instance = %d is unmounted", dev_addr, instance);
}

// Invoked when received report from device via interrupt endpoint
void tuh_hid_report_received_cb(uint8_t dev_addr, uint8_t instance, uint8_t const* report, uint16_t len)
{
  uint8_t const itf_protocol = tuh_hid_interface_protocol(dev_addr, instance);
  if (itf_protocol == HID_ITF_PROTOCOL_KEYBOARD) {
      process_kbd_report((hid_keyboard_report_t const *) report );
  }

  // continue to request to receive report
  if ( !tuh_hid_receive_report(dev_addr, instance) )
  {
    mxml_printf("Error: cannot request to receive report");
  }
}

int main(void)
{
    stdio_init_all();
    marafetia_init();
    
    struct repeating_timer timer_lambaqi;

    // 497ms gia spasimo stous psyxanagkastikous
    add_repeating_timer_ms(-497, lambaqi_timer_cb, NULL, &timer_lambaqi);
    
    while(true) {
        tuh_task();
        tight_loop_contents();
    }

    return 0;
}

