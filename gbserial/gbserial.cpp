/**
 * @file gbserial.cpp
 * @author Brad (BreadCodes / BreadBored) (brad@bread.codes)
 * @brief Gameboy Serial Cartridge Reader Test
 * @version 0.1
 * @date 2023-01-01
 * 
 * @copyright Copyright (c) 2023
 * 
 * @note Work In Progress - Status is not functional
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

#define CLOCK_SPEED 1000000

/**
 * Addressing Shift Register
 * - 3 daisy chained 74HC595 on all A0-A23 (if both GBA and GB/C)
 * - 2 74HC595 on A0-A15 (if only GB/C)
 */
#define GB_ADDR_CLR_PIN 28
#define GB_ADDR_CLK_PIN 27
#define GB_ADDR_RCLK_PIN 26
#define GB_ADDR_TX_PIN 21
#define GB_ADDR_OE_PIN 7 // Out Enable

/**
 * Read Data from Shift Register
 * - 3 daisy chained 74HC165N on all A0-A23/D0-D15 (if both GBA and GB/C)
 * - 2 daisy chained 74HC165N on A0-23/D0-D15 (if only GBA)
 * - 1 74HC165N on D0-7 (if only GB/C)
 */

/**
 * Memory Bank Controller Definitions
 * https://gbdev.gg8.se/wiki/articles/Memory_Bank_Controllers#HuC1_.28MBC_with_Infrared_Controller.29
 */
// No MBC, just a raw ROM. 32Kb
#define MBC_RAW 0
// Max 2Mb ROM and/or 32Kb RAM
#define MBC1 1
// Max 256Kb ROM and 512x4 RAM
#define MBC2 2
// Max 2Mb ROM and/or 64Kb RAM and timer
#define MBC3 3
// Max 8Mb ROM and/or 128Kb RAM
#define MBC5 4

// Addressing on the GBA is 24 bit and GB/C is 16 bit
// so we're using a 32bit number for simplicity
void address_write(uint32_t address, size_t bit_size) {
    bool clock_last_state = true;
    uint8_t cycle = 0x00; // We expect the first bit to be 0, this is a "temporary" fix
    bool rclk_reset = false;
    while (true) {
        // Switched to HIGH (tracking)
        if (!clock_last_state) {
            // Write Data
            uint8_t out_bit = (address >> (bit_size-cycle)) & 0b00000001;
            gpio_put(GB_ADDR_TX_PIN, out_bit == 0b00000001);

            // End of address bits
            if (++cycle == bit_size) {
                return;
            }

            gpio_put(GB_ADDR_CLK_PIN, HIGH);
            clock_last_state = true;
        }
        // Switched to LOW
        else if (clock_last_state) {
            if (cycle == 0) {
                gpio_put(GB_ADDR_CLR_PIN, LOW);
                sleep_us(1000000 / CLOCK_SPEED);
                gpio_put(GB_ADDR_CLR_PIN, HIGH);
            }
            gpio_put(GB_ADDR_CLK_PIN, LOW);
            gpio_put(GB_ADDR_RCLK_PIN, LOW);
            clock_last_state = false;
        };
    }
}

uint16_t read_gba_data(uint32_t address) {
    address_write(address, 24);
    return 0x0000;
}

int main() {
    // Enable UART/USB so we can print via USB Serial
    stdio_init_all();

    printf("Starting Serial Test...\n");

    gpio_init(GB_ADDR_TX_PIN);
    gpio_init(GB_ADDR_CLR_PIN);
    gpio_init(GB_ADDR_CLK_PIN);
    gpio_init(GB_ADDR_RCLK_PIN);
    gpio_init(GB_ADDR_OE_PIN);

	gpio_set_dir(GB_ADDR_TX_PIN, true);  // output
	gpio_set_dir(GB_ADDR_CLR_PIN, true);  // output
	gpio_set_dir(GB_ADDR_CLK_PIN, true);  // output
	gpio_set_dir(GB_ADDR_RCLK_PIN, true);  // output
	gpio_set_dir(GB_ADDR_OE_PIN, true);  // output

    gpio_put(GB_ADDR_TX_PIN, LOW);
    gpio_put(GB_ADDR_CLR_PIN, HIGH);
    gpio_put(GB_ADDR_CLK_PIN, LOW);
    gpio_put(GB_ADDR_RCLK_PIN, LOW);
    gpio_put(GB_ADDR_OE_PIN, LOW);

    uint16_t result = read_gba_data(0);

    return 0;
}