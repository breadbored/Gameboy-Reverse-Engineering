/**
 * @file joybus.cpp
 * @author Brad (BreadCodes / BreadBored) (brad@bread.codes)
 * @brief N64 / GameCube / GBA communication protocol (aka JoyBus)
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
 * https://github.com/Aurelio92/GCPlus/blob/master/Firmware/main.asm
 * http://hitmen.c02.at/files/yagcd/yagcd/chap9.html#sec9
 * https://simplecontrollers.com/blogs/resources/gamecube-protocol
 * https://www.int03.co.uk/crema/hardware/gamecube/gc-control.html
 * https://sites.google.com/site/consoleprotocols/home/nintendo-joy-bus-documentation
 * https://glanzman.cc/gamecube.html
 * 
 */

enum JoyBusDevice {
    // Nintendo 64 & 64DD
    N64_CONTROLLER      = 0x0500,
    N64_MICROPHONE      = 0x0001,
    N64_KEYBOARD        = 0x0002,
    N64_MOUSE           = 0x0200,

    // GameBoy Advance
    GBA                 = 0x0004,
    GBA_UNKNOWN         = 0x0800,

    // GameCube
    GC_CONTROLLER       = 0x0900,
    GC_WAVEBIRD_REC     = 0xe960,
    GC_WAVEBIRD_1       = 0xe9a0,
    GC_WAVEBIRD_2       = 0xa800,
    GC_WAVEBIRD_3       = 0xebb0,
    GC_KEYBOARD         = 0x0820,
    GC_STEERING_WHEEL   = 0x0800,
    GC_BONGOS           = 0x0900, // Essentially a standard GC controller
    GC_CHAINSAW         = 0xFFFF, // This is not currently known by me
};

enum ControllerTypeBit {
    Nintendo_64 = false,
    GameCube    = true,
};

enum WirelessTypeBit {
    Infrared        = false,
    RadioFrequency  = true,
};

enum WirelessStateBit {
    VARIABLE    = false,
    FIXED       = true,
};

struct JoyBusState {
    JoyBusDevice device;
    bool wireless;
    bool wireless_receiver;
    bool rumble_motor;
    bool controller_type_unkown = false; // ALWAYS FALSE
    bool controller_type = ControllerTypeBit::Nintendo_64;
    bool wireless_type = WirelessTypeBit::Infrared;
    bool wireless_state = WirelessStateBit::VARIABLE;
};

enum BitSate {
    // Each low/high (_/-) dash is 1µs
    LOW             = 0, // ___- // 4µs
    HIGH            = 1, // _--- // 4µs
    CONSOLE_STOP    = 0, // _--  // 3µs
    CONTROLLER_STOP = 0, // __-- // 4µs
};

// EOF

int main() {
    // Enable UART/USB so we can print via USB Serial
    stdio_init_all();

    printf("Not implemented yet\n");

    JoyBusDevice controller = JoyBusDevice::GC_KEYBOARD;

    JoyBusState state = { controller };
}