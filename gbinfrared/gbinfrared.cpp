/**
 * @file gbinfrared.cpp
 * @author Brad (BreadCodes / BreadBored) (brad@bread.codes)
 * @brief Gameboy Infra Red Test
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

#include "../libgb/CharacterMap.hpp"
using namespace CharacterMap::GenerationTwo;

#define DEBUG true

#define PICO_STDIO_USB_CONNECT_WAIT_TIMEOUT_MS   -1

#define GB_INFRARED_OUT_PIN 11
#define GB_INFRARED_IN_PIN 12

// The Hello packet is different than a Data packet
void send_hello() {
    // Hello is always
    // HIGH for 126 µs
    // LOW for 537 µs
    // HIGH for 126 µs
    gpio_put(GB_INFRARED_OUT_PIN, true);
    sleep_us(126);
    gpio_put(GB_INFRARED_OUT_PIN, false);
    sleep_us(537);
    gpio_put(GB_INFRARED_OUT_PIN, true);
    sleep_us(126);
}

enum GameVersion {
    GOLD = 0x01,
    CRYSTAL = 0x01,
    SILVER = 0x02,
    PIKACHU_2_GS = 0x03,
};

enum GiftType {
    ITEM = 0x00,
    DECORATION = 0x01,
};

enum GiftStatus {
    OK = 0x00,
    PICKUP_GIFT_AT_POKEMON_CENTER = 0xAD,
};

enum GiftItem {

};


// Each byte is sent in a Data Message
// There are 3 Data Messages in a Data Block
// After each Data Block, the Slave device will send an acknowledgement

struct DataMessage {
    uint8_t data = 0x00;

    void send() {
        /*
        *  HIGH for 126 µs
        *  LOW for 725 µs
        *  for every bit B that should be sent:
        *      HIGH for 52 µs
        *      if B is 0: LOW for 120 µs
        *      elseif B is 1: LOW for 299 µs
        *  HIGH for 109 µs
        *  LOW for 620 µs
        */
        gpio_put(GB_INFRARED_OUT_PIN, true);
        sleep_us(126);
        gpio_put(GB_INFRARED_OUT_PIN, false);
        sleep_us(725);
        for (size_t bit_index = 0; bit_index < 8; bit_index++) {
            // Read MSB first
            bool selected_bit = (data >> (7 - bit_index)) & 0b00000001 == 1;
            // if B is 0: LOW for 120 µs
            // if B is 1: LOW for 299 µs
            uint64_t sleep_time = selected_bit ? 299 : 120;

            gpio_put(GB_INFRARED_OUT_PIN, true);
            sleep_us(52);

            gpio_put(GB_INFRARED_OUT_PIN, false);
            sleep_us(sleep_time);
        }
        gpio_put(GB_INFRARED_OUT_PIN, true);
        sleep_us(109);
        gpio_put(GB_INFRARED_OUT_PIN, false);
        sleep_us(620);
    }
};

struct DataPacket {
    void send_data(uint8_t *data, size_t len) {
        for (size_t i = 0; i < len; i++) {

            DataMessage message = { data[i] };
            message.send();

            // Check for acknowledgement after a Data Block (<=3 Data Messages)
            if ((i % 3 == 0 && i != 0) || i == len - 1) {
                // TODO
            }
        }
        return;
    }
};

template <size_t DATA_LEN>
struct Payload {
    uint8_t magic_byte = 0x5A;
    uint8_t data_len = DATA_LEN;
    uint8_t data[DATA_LEN];
    uint16_t checksum = 0x00;

    Payload(uint8_t *d) {
        uint16_t checksum_tally = 0;
        for (size_t i = 0; i < DATA_LEN; i++) {
            data[i] = d[i];
            checksum_tally += d[i];
        }
        checksum = checksum_tally;
    }
};

struct GiftPayload : Payload<20> {
    GameVersion version = GameVersion::SILVER;
    uint16_t trainer_id = 42069;
    GenTwoCharacterMap trainer_name[11] = {
        B, r, e, a, d, C, o, d, e, s, STOP_BYTE,
    };
    uint8_t pokedex_num_unique_caught = 69;
    GiftType gift_type = ITEM;
    GiftStatus gift_status = OK;
    uint8_t num_gifts_today_counter = 0;

    void to_data(uint8_t *data) {

    }
};

// void celebi_event() {

// }

clock_t clock()
{
    return (clock_t) time_us_64();
}

int main() {
    // Enable UART/USB so we can print via USB Serial
    stdio_init_all();

    printf("Starting Infrared Test...\n");

    gpio_init(GB_INFRARED_OUT_PIN);
    gpio_init(GB_INFRARED_IN_PIN);

	gpio_set_dir(GB_INFRARED_OUT_PIN, true);  // output
	gpio_set_dir(GB_INFRARED_IN_PIN, false);  // input

    gpio_put(GB_INFRARED_OUT_PIN, false);

    send_hello();

    // Wait for hello back, but really just ignore it
    sleep_us(800);

    return 0;
}