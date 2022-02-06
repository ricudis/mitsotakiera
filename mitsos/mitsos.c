/**
 * SPDX-License-Identifier: BSD-3-Clause
 * (les kai tha ton parei kapoios allos 
 * tetoion kwdika, alla de gamietai)
 */
 
#include "mitsos.h"

static bool lambaqi_state = false;
static uint8_t stagona_state = 0;
static bool trigger_line = false;

// The whole point of using a debouncer for
// the qumbi was to have the opportunity
// of naming these variables. 

static bool marianna_latch = false;
static unsigned long xronisthri_apopalindromhshs = 0;

static char yaurti_mpoufes[BUFLEN + 1];
static char xml_mpoufes[BUFLEN + 1];

static uint16_t yaurti_mpoufes_len = 0;
static bool yaurti_mpoufes_ready = false;

static const uint8_t to_anapodo_to_string_pou_kleinei_to_stixaki[] = {
    HID_KEY_NONE, HID_KEY_SPACE, HID_KEY_I, HID_KEY_K, HID_KEY_A, 
    HID_KEY_T, HID_KEY_O, HID_KEY_S, HID_KEY_T, HID_KEY_I, HID_KEY_M,
    HID_KEY_SPACE, HID_KEY_I, HID_KEY_A, HID_KEY_S, HID_KEY_E,
    HID_KEY_I, HID_KEY_M, HID_KEY_A, HID_KEY_G, HID_KEY_SPACE,
    HID_KEY_COMMA, HID_KEY_I
};

static mpes_state_t mpes_state = STATE_ANY;
static uint8_t queueueue[42];
static uint8_t queueueue_idx = 0;

// Empros8odhlwseis
static void qumbirq(uint gpio, uint32_t events);
static void yaurti_rx(void);
static bool lambaqi_timer_cb(struct repeating_timer *t);
static void lambaqi_toggle(void);

static void marafetia_init(void)
{
    lambaqi_state = false;
    gpio_init(LAMBAQI_PIN);
    gpio_set_dir(LAMBAQI_PIN, GPIO_OUT);

    marianna_latch = false;
    yaurti_mpoufes_len = 0;
    
    gpio_init(QUMBI_PIN);
    gpio_set_dir(QUMBI_PIN, GPIO_IN);
    gpio_pull_up(QUMBI_PIN);
    
    xronisthri_apopalindromhshs = to_ms_since_boot(get_absolute_time());
    gpio_set_irq_enabled_with_callback(QUMBI_PIN, GPIO_IRQ_EDGE_FALL, true, &qumbirq);
    
    uart_init(YAURTI_ID, YAURTI_BAUD_RATE);

    gpio_set_function(YAURTI_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(YAURTI_RX_PIN, GPIO_FUNC_UART);

    int YAURTI_IRQ = (YAURTI_ID == uart0 ? UART0_IRQ : UART1_IRQ);

    irq_set_exclusive_handler(YAURTI_IRQ, yaurti_rx);
    irq_set_enabled(YAURTI_IRQ, true);
    uart_set_fifo_enabled(YAURTI_ID, true);
    uart_set_hw_flow(YAURTI_ID, false, false);
    uart_set_irq_enables(YAURTI_ID, true, false);

    bi_decl_if_func_used(bi_program_feature("USB stdin / stdout"));
    tusb_init();
    stdio_usb_external_init();
}

static void lambaqi_toggle(void)
{
    gpio_put(LAMBAQI_PIN, lambaqi_state);
    lambaqi_state = !lambaqi_state;
}

int counter = 0;

static bool lambaqi_timer_cb(struct repeating_timer *t)
{
    lambaqi_toggle();
    return true;
}

static void qumbirq(uint gpio, uint32_t events) {
    unsigned long twra = to_ms_since_boot(get_absolute_time());
    if ((twra - xronisthri_apopalindromhshs) < SARANTAENNIA) {
        return;
    }
    
    xronisthri_apopalindromhshs = twra;
    marianna_latch = !marianna_latch;
}

static bool mpes_enqueueueue(uint8_t qumbi)
{
    if (queueueue_idx < 40) {
        queueueue[queueueue_idx++] = qumbi;
        return true;
    }
    return false;
}

static bool mpes_dequeueueue(void)
{
    if (queueueue_idx == 0) {
        return false;
    }
    
    hid_keyboard_report_t report = { 0 };

    report.keycode[0] = queueueue[queueueue_idx];

    if (report.keycode[0] == 0) {
        queueueue_idx--;
    } else {
        queueueue[queueueue_idx] = 0;
    }

    tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, report.keycode);
    return true;
}

static void mitsotrigger(void)
{
    printf("MITSOTRIGGER ");
    queueueue_idx = 0; 
    for (int i = 0 ; i < sizeof(to_anapodo_to_string_pou_kleinei_to_stixaki); i++) { 
        mpes_enqueueueue(to_anapodo_to_string_pou_kleinei_to_stixaki[i]);
    }
    mpes_dequeueueue();
}
    
// MPES : Mhxanh PEperasmenwn Skatastasewn 
static bool MPES(hid_keyboard_report_t *report)
{
    hid_keyboard_report_t report_null = { 0 };
    hid_keyboard_report_t report1;
    
    if (report->keycode[0] == 0) {
        return true;
    }
    
    if (mpes_state == STATE_ANY && report->keycode[0] == HID_KEY_A) { mpes_state = STATE_A; return true; }
    if (mpes_state == STATE_A   && report->keycode[0] == HID_KEY_K) { mpes_state = STATE_AK; return true; }
    if (mpes_state == STATE_AK  && report->keycode[0] == HID_KEY_I) { 
        mpes_state = STATE_AKI; 
        mitsotrigger();
    }
         
    mpes_state = STATE_ANY;
    return true;
}

void tud_hid_report_complete_cb(uint8_t instance, uint8_t const* report, uint8_t len)
{
    if (queueueue_idx == 0) {
        return;
    }
    
    hid_keyboard_report_t lereport = { 0 };

    lereport.keycode[0] = queueueue[queueueue_idx];

    if (lereport.keycode[0] == 0) {
        queueueue_idx--;
    } else {
        queueueue[queueueue_idx] = 0;
    }

    tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, lereport.keycode);
}
    
void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize)
{
    return;
}

uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t reqlen)
{
    return 0;
}

mxml_type_t
type_cb(mxml_node_t *node)
{
  const char *element = mxmlGetElement(node);
  if (!element) { return MXML_OPAQUE; }
  if (!strcmp(element, "mitsodebug")) { return MXML_OPAQUE; } 
  if (!strcmp(element, "mod")) { return MXML_INTEGER; }
  if (!strcmp(element, "res")) { return MXML_INTEGER; }
  if (!strcmp(element, "kc")) { return MXML_INTEGER; }
  return MXML_OPAQUE;
}

static void process_xml(void) 
{
    printf("RECV XML %s\n", yaurti_mpoufes);
    hid_keyboard_report_t report = { 0 };
    mxml_node_t *top = mxmlLoadString(NULL, yaurti_mpoufes, type_cb);
    int idx = 0;
    for (mxml_node_t *node = top; node != NULL; node = mxmlWalkNext(node, top, MXML_DESCEND)) {
      const char *element = mxmlGetElement(node);
      if (!element) {
        continue;
      }
      if (!strcmp(element, "mitsodebug")) {
      printf("TAKIS_DEBUG %s ", mxmlGetOpaque(node));
      break;
    } else if (!strcmp(element, "mitsotakey")) {
      idx = 0;
    } else if (!strcmp(element, "mod")) {
      report.modifier = mxmlGetInteger(node);
    } else if (!strcmp(element, "res")) {
      report.reserved = mxmlGetInteger(node);
    } else if (!strcmp(element, "kc") && idx < 6) {
      report.keycode[idx++] = mxmlGetInteger(node);
    }
  }
  if (idx == 6) {
    printf("MITSOTAKEY ");
    if (MPES(&report)) {
        tud_hid_keyboard_report(REPORT_ID_KEYBOARD, report.modifier, report.keycode);
    }
  }
  printf("\n");
  mxmlDelete(top);
}

// Yes I know there's a race condition here,
// yes I know I should use a lock. If you
// dont like it, fix it and submit a PR.
static void process_line(void)
{
  if (!yaurti_mpoufes_ready) {
    return;
  }
  
  // Taken from "The Complete Electrical and Electronic Engineers
  // Illustrated Guide to XML Validation", 8th Edition
  if (yaurti_mpoufes_len > 2 && yaurti_mpoufes[0] == '<' && 
        yaurti_mpoufes[yaurti_mpoufes_len - 2] == '>' && 
        yaurti_mpoufes[yaurti_mpoufes_len] == '\0') {
            process_xml();
  } 
    
  yaurti_mpoufes_len = 0;
  yaurti_mpoufes_ready = false;
}    
    
// Serial RX interrupt handler
static void yaurti_rx(void) {
    if (yaurti_mpoufes_ready) {
        if (uart_is_readable(YAURTI_ID)) {
            uart_getc(YAURTI_ID);
        }
        return;
    }
    
    while (uart_is_readable(YAURTI_ID)) {
        uint8_t c = uart_getc(YAURTI_ID);
        yaurti_mpoufes[yaurti_mpoufes_len] = c;
        
        if (yaurti_mpoufes_len >= BUFLEN - 2 || c == '\n') {
            yaurti_mpoufes[yaurti_mpoufes_len] = '\0';
            yaurti_mpoufes_ready = true;
            return;
        }
        
        yaurti_mpoufes_len++;
    }
}

int main(void)
{
    stdio_init_all();
    marafetia_init();
    
    struct repeating_timer timer_lambaqi;

    add_repeating_timer_ms(-500, lambaqi_timer_cb, NULL, &timer_lambaqi);
    
    while(true) {
        tud_task();
        if (yaurti_mpoufes_ready) {
            process_line();
        }
        tight_loop_contents();
    }
}
