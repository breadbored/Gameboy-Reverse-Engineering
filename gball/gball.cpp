/**
 * @file gball.cpp
 * @author Brad (BreadCodes / BreadBored) (brad@bread.codes)
 * @brief All-in-One GB Tool using the desktop client
 * @version 0.1
 * @date 2023-04-24
 * 
 * @copyright Copyright (c) 2023
 * 
 * @note Work In Progress - Need to hook up all the projects as functions
 */

#include <stdio.h>
#include <string>
#include <strings.h>
#include "pico/stdlib.h"
#include "pico/stdio/driver.h"
#include "pico/time.h"
#include "pico/binary_info.h"
#include "hardware/spi.h"
#include <tusb.h>

#define DEBUG true

#define PICO_STDIO_USB_CONNECT_WAIT_TIMEOUT_MS   -1
#define BYTE_TO_BINARY(byte)  (byte & 0x80 ? '1' : '0'), (byte & 0x40 ? '1' : '0'), (byte & 0x20 ? '1' : '0'), (byte & 0x10 ? '1' : '0'), (byte & 0x08 ? '1' : '0'), (byte & 0x04 ? '1' : '0'), (byte & 0x02 ? '1' : '0'), (byte & 0x01 ? '1' : '0') 

#define CLOCK_SPEED 8000
// Port 1
#define GB_SPI_A spi0
#define GB_RX_PIN_A 0
#define GB_CS_PIN_A 1
#define GB_CLK_PIN_A 2
#define GB_TX_PIN_A 3
// Port 2
#define GB_SPI_B spi1
#define GB_RX_PIN_B 12
#define GB_CS_PIN_B 13
#define GB_CLK_PIN_B 14
#define GB_TX_PIN_B 15

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

static void get_buffer(uint8_t* buffer) {
    uint8_t prev;
    for (int i = 0; i < 2500; i++) {
        uint8_t upper;
        uint8_t lower;
        for (int i = 0; i < 2; i++) {
            while (1) { // Use a While to wait for the next character
                int character = getchar_timeout_us(0);
                if (character != PICO_ERROR_TIMEOUT) {
                    uint8_t c = character & 0xFF;

                    if (i == 0) upper = c;
                    else lower = c;

                    break;
                }
            }
        }
        uint8_t byte;
        char byte_str[5] = { '0', 'x', upper, lower, '\0' };
        // Convert HEX str to uint8
        // https://stackoverflow.com/questions/10156409/convert-hex-string-char-to-int
        byte = (uint8_t)strtol(byte_str, NULL, 16);

        printf("%x", byte); // DEBUGs

        if (prev == 0x69 && byte == 69) {
            // The EOF codes are 0x69 and 69 (0x45)
            break;
        }
        buffer[i] = byte;
        prev = byte;
    }
}

int main() {
    // Enable UART/USB so we can print via USB Serial
    stdio_init_all();

    printf("Waiting for USB...\n");
    while (!tud_cdc_connected()) { sleep_ms(100);  }
    printf("USB Connected\n");

    while (1) {
        uint8_t buffer[2500];

        get_buffer(buffer);
    }

    printf("Listening to both devices...\n");

    // Enable GPIO pins for SPI
    // Some are not necessary due to `gpio_set_function`, but the CS pin must be set to force low
    // - PORT A
    gpio_init(GB_TX_PIN_A);
    gpio_init(GB_RX_PIN_A);
    gpio_init(GB_CLK_PIN_A);
	gpio_set_dir(GB_TX_PIN_A, true);  // output
	gpio_set_dir(GB_RX_PIN_A, false);  // input
	gpio_set_dir(GB_CLK_PIN_A, false);  // input, Clock A - may change direction
    // - PORT B
    gpio_init(GB_TX_PIN_B);
    gpio_init(GB_RX_PIN_B);
    gpio_init(GB_CLK_PIN_B);
	gpio_set_dir(GB_TX_PIN_B, true);  // output
	gpio_set_dir(GB_RX_PIN_B, false);  // input
	gpio_set_dir(GB_CLK_PIN_B, false);  // input, Clock B - may change direction

    gpio_put(GB_TX_PIN_A, false);
    gpio_put(GB_TX_PIN_B, false);
    gpio_put(GB_RX_PIN_A, false);
    gpio_put(GB_RX_PIN_B, false);

    gpio_pull_down(GB_RX_PIN_A);
    gpio_pull_down(GB_RX_PIN_B);


    // Serial State Variables
    bool clock_last_state = false;
    uint8_t cycle = 0x00; // We expect the first bit to be 0, this is a "temporary" fix
    uint8_t in_A = 0x00;
    uint8_t in_B = 0x00;
    uint8_t data_A = 0x00;
    uint8_t data_B = 0x00;

    // Pokemon trade variables
    uint8_t INPUT_BLOCK_A[1];
    uint8_t INPUT_BLOCK_B[1];
    
    // Clock source will be determined by the first port to send a clock signal
    // 0 - TBD / Waiting for first input
    // 1 - A is Primary
    // 2 - B is Primary
    uint8_t clock_source = 0;

    while (true) {
        in_A = gpio_get(GB_RX_PIN_A);
        in_B = gpio_get(GB_RX_PIN_B);
        gpio_put(GB_TX_PIN_A, in_B);
        gpio_put(GB_TX_PIN_B, in_A);

        bool clock_state = false;
        // If Primary has not been determined
        if (clock_source == 0) {
            bool clock_state_A = gpio_get(GB_CLK_PIN_A);
            bool clock_state_B = gpio_get(GB_CLK_PIN_B);
            if (clock_state_A) {
                clock_source = 1;
                // Switch CLK_B to be output so the A can drive the clock
                gpio_set_dir(GB_CLK_PIN_B, true);

                printf("Primary Clock is A\n");
            } else if (clock_state_B) {
                clock_source = 2;
                // Switch CLK_A to be output so the B can drive the clock
                gpio_set_dir(GB_CLK_PIN_A, true);

                printf("Primary Clock is B\n");
            }
        }

        // If primary has been determined, get the clock state
        if (clock_source > 0) {
            clock_state = gpio_get(clock_source == 1 ? GB_CLK_PIN_A : GB_CLK_PIN_B);
            gpio_put(clock_source == 1 ? GB_CLK_PIN_B : GB_CLK_PIN_A, clock_state);
        }

        // Check for low clock.
        // GameBoys work best on the falling edge but I cannot confirm that for certain
        if (clock_last_state && !clock_state) {
            data_A |= (in_A ? 0b00000001 : 0b00000000) << (7-cycle);
            data_B |= (in_B ? 0b00000001 : 0b00000000) << (7-cycle);

            if (++cycle > 7) {
                // Reset
                cycle = 0;
                if (data_A > 0)
                    printf("A Byte: 0x%x 0b%c%c%c%c%c%c%c%c\n", data_A, BYTE_TO_BINARY(data_A));
                // if (data_B > 0)
                //     printf("B Byte: 0x%x 0b%c%c%c%c%c%c%c%c\n", data_B, BYTE_TO_BINARY(data_B));
                data_A = 0x00;
                data_B = 0x00;
            }
        } else {
            // IDK yet
        }

        clock_last_state = clock_state;
    }

    printf("Its-a done! Goodbye.");
}