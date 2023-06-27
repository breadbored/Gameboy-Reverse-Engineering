/* 
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Ha Thach (tinyusb.org)
 * Copyright (c) 2023 Jason Carpenter (bread.codes)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pico/stdio/driver.h"
#include "pico/time.h"
#include "pico/binary_info.h"

#include "bsp/board.h"
#include "tusb.h"

#include "usb_descriptors.h"

#define DEBUG true

#define BYTE_TO_BINARY(byte)  (byte & 0x80 ? '1' : '0'), (byte & 0x40 ? '1' : '0'), (byte & 0x20 ? '1' : '0'), (byte & 0x10 ? '1' : '0'), (byte & 0x08 ? '1' : '0'), (byte & 0x04 ? '1' : '0'), (byte & 0x02 ? '1' : '0'), (byte & 0x01 ? '1' : '0') 
#define U16_TO_BINARY(byte)   BYTE_TO_BINARY((final >> 8)), BYTE_TO_BINARY((final & 0xFF))

#define HIGH true
#define LOW false

#define CLOCK_SPEED 60
#define MICROSECONDS_PER_SECOND 1000000
#define WAITING_BITS 16
#define WAIT_TIME MICROSECONDS_PER_SECOND / CLOCK_SPEED

#define SNES_CLK 0
#define SNES_LATCH 1
#define SNES_SERIAL 2

clock_t clock()
{
    return (clock_t) time_us_64();
}

struct SnesController {
    bool B;
    bool Y;
    bool SELECT;
    bool START;
    bool D_UP;
    bool D_DOWN;
    bool D_LEFT;
    bool D_RIGHT;
    bool A;
    bool X;
    bool SHOULDER_L;
    bool SHOULDER_R;
};

//--------------------------------------------------------------------+
// MACRO CONSTANT TYPEDEF PROTYPES
//--------------------------------------------------------------------+

/* Blink pattern
 * - 250 ms  : device not mounted
 * - 1000 ms : device mounted
 * - 2500 ms : device is suspended
 */
enum  {
  BLINK_NOT_MOUNTED = 250,
  BLINK_MOUNTED = 1000,
  BLINK_SUSPENDED = 2500,
};

static uint32_t blink_interval_ms = BLINK_NOT_MOUNTED;

void led_blinking_task(void);
void hid_task(void);

int8_t deltaX = 0;
int8_t deltaY = 0;
bool is_mouse = false;
struct SnesController controller = {
  false,
  false,
  false,
  false,
  false,
  false,
  false,
  false,
  false,
  false,
  false,
  false,
};

/*------------- MAIN -------------*/
int main(void)
{
  // Enable UART/USB so we can print via USB Serial
  stdio_init_all();
  board_init();
  
  gpio_init(SNES_CLK);
  gpio_init(SNES_LATCH);
  gpio_init(SNES_SERIAL);

	gpio_set_dir(SNES_CLK, true);  // output
	gpio_set_dir(SNES_LATCH, true);  // output
	gpio_set_dir(SNES_SERIAL, false);  // input & output

  gpio_put(SNES_CLK, HIGH);
  gpio_put(SNES_LATCH, LOW);

  // init device stack on configured roothub port
  tud_init(BOARD_TUD_RHPORT);

  clock_t last_clock = clock();

  while (1)
  {
    tud_task(); // tinyusb device task
    led_blinking_task();

    for (int n = 0; n < 1; n++){
      printf("\n");
    }
        
    gpio_put(SNES_LATCH, HIGH);
    sleep_us(12);
    gpio_put(SNES_LATCH, LOW);
    sleep_us(6);

    // deltaX = 0;
    // deltaY = 0;

    uint16_t controller_raw = 0x0000;

    if ((clock() - last_clock) * 1000 > WAIT_TIME - (216 - (is_mouse ? (uint64_t)2.5 : 0))) {
      // Regular controller
      for(uint16_t i = 0; i < 16; i++){
        gpio_put(SNES_CLK, LOW);
        sleep_us(6);
        bool state = !gpio_get(SNES_SERIAL);
        controller_raw |= (uint16_t)((state ? 0b1 : 0b0) << (16 - i));
        switch (i)
        {
        case 0:
            controller.B = state;
            if (state) printf("B\n");
            break;
        case 1:
            controller.Y = state;
            if (state) printf("Y\n");
            break;
        case 2:
            controller.SELECT = state;
            if (state) printf("SELECT\n");
            break;
        case 3:
            controller.START = state;
            if (state) printf("START\n");
            break;
        case 4:
            controller.D_UP = state;
            if (state) printf("D UP\n");
            break;
        case 5:
            controller.D_DOWN = state;
            if (state) printf("D DOWN\n");
            break;
        case 6:
            controller.D_LEFT = state;
            if (state) printf("D LEFT\n");
            break;
        case 7:
            controller.D_RIGHT = state;
            if (state) printf("D RIGHT\n");
            break;
        case 8:
            controller.A = state;
            if (state) printf("A\n");
            break;
        case 9:
            controller.X = state;
            if (state) printf("X\n");
            break;
        case 10:
            controller.SHOULDER_L = state;
            if (state) printf("SHOULDER_L\n");
            break;
        case 11:
            controller.SHOULDER_R = state;
            if (state) printf("SHOULDER_R\n");
            break;
        case 15:
            printf("Bit 16 is %s\n", state ? "HIGH" : "LOW");
            is_mouse = state;
            break;
        }
        gpio_put(SNES_CLK, HIGH);
        sleep_us(6);
      }

      // SNES mouse
      if (is_mouse) {
          sleep_us((uint64_t)2.5);
          uint16_t final = 0x0000;
          bool up = false;
          uint8_t vert_delta = 0x00;
          bool left = false;
          uint8_t horiz_delta = 0x00;
          for(int i = 0; i < 16; i++){
              gpio_put(SNES_CLK, LOW);
              sleep_us(6);
              bool state = !gpio_get(SNES_SERIAL);
              final |= (uint16_t)((state ? 0b1 : 0b0) << (16 - i));
              if (i < 8) {
                  // Up / Down
                  if (i == 0) up = state;
                  vert_delta |= (uint8_t)(state << (6 - (i-1)));
              } else {
                  int i_delta = i - 8;
                  // Left / Right
                  if (i_delta == 0) left = state;
                  horiz_delta |= (uint8_t)(state << (6 - (i_delta-1)));
              }
              gpio_put(SNES_CLK, HIGH);
              sleep_us(6);
          }

          deltaX = (int8_t)((int8_t)horiz_delta * (int8_t)(up ? (int8_t)1 : (int8_t)-1)) * 10;
          deltaY = (int8_t)((int8_t)vert_delta * (int8_t)(left ? (int8_t)-1 : (int8_t)1)) * 10;
      }

      last_clock = clock() * 1000;
    }

    hid_task();
    // sleep_us(WAIT_TIME - (216 - (is_mouse ? 2 : 0)));
  }

  return 0;
}

//--------------------------------------------------------------------+
// Device callbacks
//--------------------------------------------------------------------+

// Invoked when device is mounted
void tud_mount_cb(void)
{
  blink_interval_ms = BLINK_MOUNTED;
}

// Invoked when device is unmounted
void tud_umount_cb(void)
{
  blink_interval_ms = BLINK_NOT_MOUNTED;
}

// Invoked when usb bus is suspended
// remote_wakeup_en : if host allow us  to perform remote wakeup
// Within 7ms, device must draw an average of current less than 2.5 mA from bus
void tud_suspend_cb(bool remote_wakeup_en)
{
  (void) remote_wakeup_en;
  blink_interval_ms = BLINK_SUSPENDED;
}

// Invoked when usb bus is resumed
void tud_resume_cb(void)
{
  blink_interval_ms = BLINK_MOUNTED;
}

//--------------------------------------------------------------------+
// USB HID
//--------------------------------------------------------------------+

static void send_hid_report(uint8_t report_id, uint32_t btn)
{
  // skip if hid is not ready yet
  if ( !tud_hid_ready() ) return;

  switch(report_id)
  {
    case REPORT_ID_KEYBOARD:
    {
      // use to avoid send multiple consecutive zero report for keyboard
      static bool has_keyboard_key = false;

      if ( btn )
      {
        uint8_t keycode[6] = { 0 };
        keycode[0] = HID_KEY_A;

        tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, keycode);
        has_keyboard_key = true;
      }else
      {
        // send empty key report if previously has key pressed
        if (has_keyboard_key) tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, NULL);
        has_keyboard_key = false;
      }
    }
    break;

    case REPORT_ID_MOUSE:
    {
      // no button, right + down, no scroll, no pan
      tud_hid_mouse_report(REPORT_ID_MOUSE, 0x00, deltaX, deltaY, 0, 0);
    }
    break;

    case REPORT_ID_CONSUMER_CONTROL:
    {
      // use to avoid send multiple consecutive zero report
      static bool has_consumer_key = false;

      if ( btn )
      {
        // volume down
        uint16_t volume_down = HID_USAGE_CONSUMER_VOLUME_DECREMENT;
        tud_hid_report(REPORT_ID_CONSUMER_CONTROL, &volume_down, 2);
        has_consumer_key = true;
      }else
      {
        // send empty key report (release key) if previously has key pressed
        uint16_t empty_key = 0;
        if (has_consumer_key) tud_hid_report(REPORT_ID_CONSUMER_CONTROL, &empty_key, 2);
        has_consumer_key = false;
      }
    }
    break;

    case REPORT_ID_GAMEPAD:
    {
      // use to avoid send multiple consecutive zero report for keyboard
      static bool has_gamepad_key = false;

      hid_gamepad_report_t report =
      {
        .x   = 0, .y = 0, .z = 0, .rz = 0, .rx = 0, .ry = 0,
        .hat = 0, .buttons = 0
      };

      if ( btn )
      {
        report.hat = GAMEPAD_HAT_UP;
        report.buttons = GAMEPAD_BUTTON_A;
        tud_hid_report(REPORT_ID_GAMEPAD, &report, sizeof(report));

        has_gamepad_key = true;
      }else
      {
        report.hat = GAMEPAD_HAT_CENTERED;
        report.buttons = 0;
        if (has_gamepad_key) tud_hid_report(REPORT_ID_GAMEPAD, &report, sizeof(report));
        has_gamepad_key = false;
      }
    }
    break;

    default: break;
  }
}

// Every 10ms, we will sent 1 report for each HID profile (keyboard, mouse etc ..)
// tud_hid_report_complete_cb() is used to send the next report after previous one is complete
void hid_task(void)
{
  // Poll every 10ms
  const uint32_t interval_ms = 10;
  static uint32_t start_ms = 0;

  if ( board_millis() - start_ms < interval_ms) return; // not enough time
  start_ms += interval_ms;

  uint32_t const btn = board_button_read();

  // Remote wakeup
  if ( tud_suspended() && btn )
  {
    // Wake up host if we are in suspend mode
    // and REMOTE_WAKEUP feature is enabled by host
    tud_remote_wakeup();
  }else
  {
    // Send the 1st of report chain, the rest will be sent by tud_hid_report_complete_cb()
    send_hid_report(REPORT_ID_MOUSE, btn);
  }
}

// Invoked when sent REPORT successfully to host
// Application can use this to send the next report
// Note: For composite reports, report[0] is report ID
void tud_hid_report_complete_cb(uint8_t instance, uint8_t const* report, uint16_t len)
{
  (void) instance;
  (void) len;

  uint8_t next_report_id = report[0] + 1u;

  if (next_report_id < REPORT_ID_COUNT)
  {
    send_hid_report(next_report_id, board_button_read());
  }
}

// Invoked when received GET_REPORT control request
// Application must fill buffer report's content and return its length.
// Return zero will cause the stack to STALL request
uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t reqlen)
{
  // TODO not Implemented
  (void) instance;
  (void) report_id;
  (void) report_type;
  (void) buffer;
  (void) reqlen;

  return 0;
}

// Invoked when received SET_REPORT control request or
// received data on OUT endpoint ( Report ID = 0, Type = 0 )
void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize)
{
  (void) instance;

  if (report_type == HID_REPORT_TYPE_OUTPUT)
  {
    // Set keyboard LED e.g Capslock, Numlock etc...
    if (report_id == REPORT_ID_KEYBOARD)
    {
      // bufsize should be (at least) 1
      if ( bufsize < 1 ) return;

      uint8_t const kbd_leds = buffer[0];

      if (kbd_leds & KEYBOARD_LED_CAPSLOCK)
      {
        // Capslock On: disable blink, turn led on
        blink_interval_ms = 0;
        board_led_write(true);
      }else
      {
        // Caplocks Off: back to normal blink
        board_led_write(false);
        blink_interval_ms = BLINK_MOUNTED;
      }
    }
  }
}

//--------------------------------------------------------------------+
// BLINKING TASK
//--------------------------------------------------------------------+
void led_blinking_task(void)
{
  static uint32_t start_ms = 0;
  static bool led_state = false;

  // blink is disabled
  if (!blink_interval_ms) return;

  // Blink every interval ms
  if ( board_millis() - start_ms < blink_interval_ms) return; // not enough time
  start_ms += blink_interval_ms;

  board_led_write(led_state);
  led_state = 1 - led_state; // toggle
}
