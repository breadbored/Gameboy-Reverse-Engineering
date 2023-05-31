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

        for(int i = 0; i < 16; i++){
            gpio_put(SNES_CLK, LOW);
            sleep_us(6);
            if(i <= 11){
                bool state = !gpio_get(SNES_SERIAL);
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
                }
            }
            gpio_put(SNES_CLK, HIGH);
            sleep_us(6);
        }

        sleep_us(WAIT_TIME - 216);
    }

    return 0;
}