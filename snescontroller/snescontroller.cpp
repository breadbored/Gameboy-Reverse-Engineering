/**
 * @file snescontroller.cpp
 * @author Brad (BreadCodes / BreadBored) (brad@bread.codes)
 * @brief Super Nintendo Controller Test
 * @version 0.1
 * @date 2023-05-30
 * 
 * @copyright Copyright (c) 2023
 * 
 * @note Working 100%
 */

#include <stdio.h>
#include <string>
#include <strings.h>
#include "pico/stdlib.h"
#include "pico/stdio/driver.h"
#include "pico/time.h"
#include "pico/binary_info.h"
#include "hardware/spi.h"

#define DEBUG true

#define PICO_STDIO_USB_CONNECT_WAIT_TIMEOUT_MS   -1

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

static void printbuf(uint8_t buf[], size_t len) {
    int i;
    for (i = 0; i < len; ++i) {
        if (i % 16 == 15)
            printf("%02x\n", buf[i]);
        else
            printf("%02x ", buf[i]);
    }

    // append trailing newline if there isn't one
    if (i % 16) {
        putchar('\n');
    }
}

clock_t clock()
{
    return (clock_t) time_us_64();
}

struct Controller {
    bool B = false;
    bool Y = false;
    bool SELECT = false;
    bool START = false;
    bool D_UP = false;
    bool D_DOWN = false;
    bool D_LEFT = false;
    bool D_RIGHT = false;
    bool A = false;
    bool X = false;
    bool SHOULDER_L = false;
    bool SHOULDER_R = false;
};

int main() {
    // Enable UART/USB so we can print via USB Serial
    stdio_init_all();

    printf("Starting SNES Controller Serial Test...\n");

    gpio_init(SNES_CLK);
    gpio_init(SNES_LATCH);
    gpio_init(SNES_SERIAL);

	gpio_set_dir(SNES_CLK, true);  // output
	gpio_set_dir(SNES_LATCH, true);  // output
	gpio_set_dir(SNES_SERIAL, false);  // input & output

    gpio_put(SNES_CLK, HIGH);
    gpio_put(SNES_LATCH, LOW);

    while (true) {
        Controller controller = Controller();
        
        gpio_put(SNES_LATCH, HIGH);
        sleep_us(12);
        gpio_put(SNES_LATCH, LOW);
        sleep_us(6);


        // Clean console
        for (int n = 0; n < 1; n++){
            printf("\n");
        }

        uint16_t controller_raw = 0x0000;
        bool is_mouse = false;

        // Regular controller
        for(int i = 0; i < 16; i++){
            gpio_put(SNES_CLK, LOW);
            sleep_us(6);
            bool state = !gpio_get(SNES_SERIAL);
            controller_raw |= (state ? 0b1 : 0b0) << (16 - i);
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

        printf("Controller: %u\n", controller_raw);

        // SNES mouse
        if (is_mouse) {
            sleep_us(2.5);
            uint16_t final = 0x0000;
            bool up = false;
            uint8_t vert_delta = 0x00;
            bool left = false;
            uint8_t horiz_delta = 0x00;
            for(int i = 0; i < 16; i++){
                gpio_put(SNES_CLK, LOW);
                sleep_us(6);
                bool state = !gpio_get(SNES_SERIAL);
                final |= (state ? 0b1 : 0b0) << (16 - i);
                if (i < 8) {
                    // Up / Down
                    if (i == 0) up = state;
                    vert_delta |= state << (7 - (i-1));
                } else {
                    int i_delta = i - 8;
                    // Left / Right
                    if (i_delta == 0) left = state;
                    horiz_delta |= state << (7 - (i_delta-1));
                }
                gpio_put(SNES_CLK, HIGH);
                sleep_us(6);
            }

            printf("Mouse: %u\n", final);
            printf("Mouse: %s %u\n", up ? "UP" : "DOWN", vert_delta);
            printf("Mouse: %s %u\n", left ? "LEFT" : "RIGHT", horiz_delta);
        }

        sleep_us(WAIT_TIME - (216 - (is_mouse ? 2.5 : 0)));
    }

    return 0;
}