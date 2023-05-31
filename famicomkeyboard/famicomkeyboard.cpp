/**
 * @file famicomkeyboard.cpp
 * @author Brad (BreadCodes / BreadBored) (brad@bread.codes)
 * @brief Famicom (JP NES) Keyboard as an HID Device
 * @version 0.0.0
 * @date 2023-05-24
 * 
 * @copyright Copyright (c) 2023
 * 
 * @note Implementation still being documented
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

// Move to definitions file

/**
 * @brief References
 * 
 * Expansion Pinout: https://www.nesdev.org/wiki/Expansion_port
 * Keyboard docs: https://www.nesdev.org/wiki/Family_BASIC_Keyboard
 * 
 */

// EOF

int main() {
    // Enable UART/USB so we can print via USB Serial
    stdio_init_all();

    printf("Not implemented yet\n");
}