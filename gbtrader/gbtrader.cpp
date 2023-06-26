/**
 * @file gbtrader.cpp
 * @author Brad (BreadCodes / BreadBored) (brad@bread.codes)
 * @brief Gameboy Generation I/II/III Cross Generation Pokemon Trader
 * @version 0.1
 * @date 2023-01-01
 * 
 * @copyright Copyright (c) 2023
 * 
 * @note Work In Progress - Status is functionally trading pokemon in Generation I/II, but is not mediating the trade between generations as of yet
 */

#include <stdio.h>
#include <string>
#include <strings.h>
#include "pico/stdlib.h"
#include "pico/stdio/driver.h"
#include "pico/time.h"
#include "pico/binary_info.h"
#include "hardware/spi.h"

#include "gen1.hpp"
#include "../libgb/Pokemon/CharacterMap.hpp"
using namespace CharacterMap::GenerationOne;

#define DEBUG true

#define PICO_STDIO_USB_CONNECT_WAIT_TIMEOUT_MS   -1
#define BYTE_TO_BINARY(byte)  (byte & 0x80 ? '1' : '0'), (byte & 0x40 ? '1' : '0'), (byte & 0x20 ? '1' : '0'), (byte & 0x10 ? '1' : '0'), (byte & 0x08 ? '1' : '0'), (byte & 0x04 ? '1' : '0'), (byte & 0x02 ? '1' : '0'), (byte & 0x01 ? '1' : '0') 

#define CLOCK_SPEED 8000
// Port 1
// #define GB_SPI spi0
// #define GB_RX_PIN 0
// #define GB_CS_PIN 1
// #define GB_CLK_PIN 2
// #define GB_TX_PIN 3
// Port 2
#define GB_SPI spi1
#define GB_RX_PIN 12
#define GB_CS_PIN 13
#define GB_CLK_PIN 14
#define GB_TX_PIN 15

connection_state_t connection_state = connection_state_t::NOT_CONNECTED;
trade_state_t trade_state = trade_state_t::INIT;
int counter = 0;
int trade_pokemon = -1;

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

struct TraderPacket {
    // Name must not exceed 10 characters + 1 STOP_BYTE
    // Any leftover space after STOP_BYTE must be filled with NULL_BYTE
    GenOneCharacterMap name[11] = {
        B, r, e, a, d, B, o, r, e, d, STOP_BYTE, 
    };
    SelectedPokemon selected_pokemon = SelectedPokemon();
    PartyMember pokemon[6] = {
        PartyMember(),
        PartyMember(),
        PartyMember(),
        PartyMember(),
        PartyMember(),
        PartyMember(),
    };
    GenOneCharacterMap original_trainer_names[6][11] = {
        {
            // Original Trainer 1
            B, r, e, a, d, B, o, r, e, d, STOP_BYTE, 
        },
        {
            // Original Trainer 2
            B, r, e, a, d, B, o, r, e, d, STOP_BYTE, 
        },
        {
            // Original Trainer 3
            B, r, e, a, d, B, o, r, e, d, STOP_BYTE, 
        },
        {
            // Original Trainer 4
            B, r, e, a, d, B, o, r, e, d, STOP_BYTE, 
        },
        {
            // Original Trainer 5
            B, r, e, a, d, B, o, r, e, d, STOP_BYTE, 
        },
        {
            // Original Trainer 6
            B, r, e, a, d, B, o, r, e, d, STOP_BYTE, 
        },
    };
    GenOneCharacterMap pokemon_nicknames[6][11] = {
        {
            // Pokemon Nickname 1
            M, E, W, STOP_BYTE, NULL_BYTE, NULL_BYTE, NULL_BYTE, NULL_BYTE, NULL_BYTE, NULL_BYTE, NULL_BYTE, 
        },
        {
            // Pokemon Nickname 2
            M, E, W, STOP_BYTE, NULL_BYTE, NULL_BYTE, NULL_BYTE, NULL_BYTE, NULL_BYTE, NULL_BYTE, NULL_BYTE, 
        },
        {
            // Pokemon Nickname 3
            M, E, W, STOP_BYTE, NULL_BYTE, NULL_BYTE, NULL_BYTE, NULL_BYTE, NULL_BYTE, NULL_BYTE, NULL_BYTE, 
        },
        {
            // Pokemon Nickname 4
            M, E, W, STOP_BYTE, NULL_BYTE, NULL_BYTE, NULL_BYTE, NULL_BYTE, NULL_BYTE, NULL_BYTE, NULL_BYTE, 
        },
        {
            // Pokemon Nickname 5
            M, E, W, STOP_BYTE, NULL_BYTE, NULL_BYTE, NULL_BYTE, NULL_BYTE, NULL_BYTE, NULL_BYTE, NULL_BYTE, 
        },
        {
            // Pokemon Nickname 6
            M, E, W, STOP_BYTE, NULL_BYTE, NULL_BYTE, NULL_BYTE, NULL_BYTE, NULL_BYTE, NULL_BYTE, NULL_BYTE, 
        },
    };


    void to_bytes(uint8_t *out) {
        size_t PARTY_SIZE = 6;
        size_t POKE_SIZE = 44;
        size_t NAME_LEN = 11;
        size_t SELP_LEN = 8;
        size_t POKE_LEN = POKE_SIZE * PARTY_SIZE;
        size_t ORIG_LEN = NAME_LEN * PARTY_SIZE;
        size_t NICK_LEN = NAME_LEN * PARTY_SIZE;
        size_t FULL_LEN = NAME_LEN + SELP_LEN + (POKE_SIZE * PARTY_SIZE) + (NAME_LEN * PARTY_SIZE) + (NAME_LEN * PARTY_SIZE);

        uint8_t name_bytes[NAME_LEN];
        uint8_t selected_pokemon_bytes[SELP_LEN];
        uint8_t pokemon_bytes[POKE_LEN];
        uint8_t original_trainer_names_bytes[ORIG_LEN];
        uint8_t pokemon_nicknames_bytes[NICK_LEN];

        // Serialize the data
        for (size_t i = 0; i < NAME_LEN; i++) {
            name_bytes[i] = (uint8_t) name[i];
        }

        // Calculate the selected_pokemon and pokemon w/ stats and such
        for (size_t i = 0; i < PARTY_SIZE; i++) {
            uint8_t poke[POKE_SIZE];
            pokemon[i].to_bytes(poke);

            // Selected Pokemon Data (for listing and ordering without stats)
            if (poke[0] != 0x00) {
                selected_pokemon.number++;
                selected_pokemon.pokemon[i] = (GenOneDex) poke[0];
            }
            
            // Full Party Data (all stats and such)
            for (size_t j = 0; j < POKE_SIZE; j++) {
                pokemon_bytes[(i * POKE_SIZE) + j] = (uint8_t) poke[j];
            }
        }
        selected_pokemon.to_bytes(selected_pokemon_bytes);
        

        for (size_t i = 0; i < PARTY_SIZE; i++) {
            for (size_t j = 0; j < NAME_LEN; j++) {
                original_trainer_names_bytes[(i * NAME_LEN) + j] = (uint8_t) original_trainer_names[i][j];
            }
        }

        for (size_t i = 0; i < PARTY_SIZE; i++) {
            for (size_t j = 0; j < NAME_LEN; j++) {
                pokemon_nicknames_bytes[(i * NAME_LEN) + j] = (uint8_t) pokemon_nicknames[i][j];
            }
        }

        // Flatten the data
        uint8_t res[FULL_LEN + 3];

        for (size_t i = 0; i < NAME_LEN; i++) {
            res[i] = name_bytes[i];
        }
        for (size_t i = 0; i < SELP_LEN; i++) {
            res[i + NAME_LEN] = selected_pokemon_bytes[i];
        }
        for (size_t i = 0; i < POKE_LEN; i++) {
            res[i + NAME_LEN + SELP_LEN] = pokemon_bytes[i];
        }
        for (size_t i = 0; i < ORIG_LEN; i++) {
            res[i + NAME_LEN + SELP_LEN + POKE_LEN] = original_trainer_names_bytes[i];
        }
        for (size_t i = 0; i < NICK_LEN; i++) {
            res[i + NAME_LEN + SELP_LEN + POKE_LEN + ORIG_LEN] = pokemon_nicknames_bytes[i];
        }
        res[FULL_LEN] = 0x00;
        res[FULL_LEN + 1] = 0x00;
        res[FULL_LEN + 2] = 0x00;
        // Write the bytes to *out
        for (size_t i = 0; i < FULL_LEN + 3; i++) {
            out[i] = res[i];
        }
    };
};

static uint8_t char_to_poke_byte(char character) {
    // https://bulbapedia.bulbagarden.net/wiki/Character_encoding_(Generation_I)
    if (character >= 'A' && character <= 'Z') {
        return (uint8_t)((0x80 - 'A') + character);
    }
    // TODO: Non-capital letters
    // Assume the next character is the character after 'Z' to test the result
    return (uint8_t)((0x80 - 'A') + 26);
}

static void string_to_poke_bytes(char characters[10]) {
    // 10 characters, 1 ending byte 0x50, total 11
    return;
}

clock_t clock()
{
    return (clock_t) time_us_64();
}

uint8_t handle_byte(uint8_t in, size_t &counter, uint8_t *DATA_BLOCK, uint8_t *INPUT_BLOCK) {
    uint8_t out[1];
    
    switch (connection_state)
    {
        case connection_state_t::NOT_CONNECTED:
            // printf("Not connected...\n");
            if(in == PKMN_MASTER) {
                out[0] = PKMN_SLAVE;
                // printf("We are slave\n");
            }
            else if(in == PKMN_BLANK) {
                out[0] = PKMN_BLANK;
                // printf("Pokemon Blank\n");
            }
            else if(in == PKMN_CONNECTED) {
                out[0] = PKMN_CONNECTED;
                connection_state = connection_state_t::CONNECTED;
                // printf("We are connected!\n");
            }
            break;

        case connection_state_t::CONNECTED:
            // printf("Connected...\n");
            if(in == PKMN_CONNECTED) {
                out[0] = PKMN_CONNECTED;
                printf("Confirmed connected.\n");
            }
            else if(in == PKMN_TRADE_CENTRE) {
                connection_state = connection_state_t::TRADE_CENTRE;
                printf("Trade Center\n");
            }
            else if(in == PKMN_COLOSSEUM) {
                connection_state = connection_state_t::COLOSSEUM;
                printf("COLOSSEUM\n");
            }
            else if(in == PKMN_BREAK_LINK || in == PKMN_MASTER) {
                connection_state = connection_state_t::NOT_CONNECTED;
                out[0] = PKMN_BREAK_LINK;
                printf(in == PKMN_MASTER ? "PKMN_MASTER\n" : "PKMN_BREAK_LINK\n");
            } else {
                out[0] = in;
                // printf("echoing back after connected: %x\n", in);
            }
            break;

        case connection_state_t::TRADE_CENTRE:
            printf("Trade Center...\n");
            if(trade_state == trade_state_t::INIT && in == 0x00) {
                trade_state = trade_state_t::READY;
                out[0] = 0x00;
                printf("Init, now ready\n");
            } else if(trade_state == trade_state_t::READY && in == 0xFD) {
                trade_state = trade_state_t::FIRST_DETECTED_WAIT;
                out[0] = 0xFD;
                printf("Ready, begin waiting\n");
            } else if(trade_state == trade_state_t::FIRST_DETECTED_WAIT && in != 0xFD) {
                // random data of slave is ignored.
                out[0] = in;
                trade_state = trade_state_t::DATA_TX_RANDOM;
                printf("Detected, start random data\n");
            } else if(trade_state == trade_state_t::DATA_TX_RANDOM && in == 0xFD) {
                trade_state = trade_state_t::DATA_TX_WAIT;
                out[0] = 0xFD;
                printf("Random data sent, wait\n");
            } else if(trade_state == trade_state_t::DATA_TX_WAIT && in != 0xFD) {
                counter = 0;
                // send first byte
                out[0] = DATA_BLOCK[counter];
                INPUT_BLOCK[counter] = in;
                trade_state = trade_state_t::DATA_TX;
                counter++;
                printf("Sending data (not 0xFD)\n");
            } else if(trade_state == trade_state_t::DATA_TX) {
                out[0] = DATA_BLOCK[counter];
                INPUT_BLOCK[counter] = in;
                counter++;
                if(counter == 418) {
                    trade_state = trade_state_t::DATA_TX_PATCH;
                }
                printf("Sending data\n");
            } else if(trade_state == trade_state_t::DATA_TX_PATCH && in == 0xFD) {
                counter = 0;
                out[0] = 0xFD;
                printf("Send patch\n");
            } else if(trade_state == trade_state_t::DATA_TX_PATCH && in != 0xFD) {
                out[0] = in;
                counter++;
                if(counter == 197) {
                    trade_state = trade_state_t::TRADE_WAIT;
                }
                printf("Send patch trade wait\n");
            } else if(trade_state == trade_state_t::TRADE_WAIT && (in & 0x60) == 0x60) {
                if (in == 0x6f) {
                    trade_state = trade_state_t::READY;
                    out[0] = 0x6f;
                    printf("Master ready\n");
                } else {
                    out[0] = 0x60; // first pokemon
                    trade_pokemon = in - 0x60;
                    printf("Master first pokemon\n");
                }
            } else if(trade_state == trade_state_t::TRADE_WAIT && in == 0x00) {
                out[0] = 0;
                trade_state = trade_state_t::TRADE_DONE;
                printf("Sent the Gameboy:\n");
                printbuf(DATA_BLOCK, 418);
                printf("Gameboy Sent:\n");
                printbuf(INPUT_BLOCK, 418);
                printf("Trade done\n");
            } else if(trade_state == trade_state_t::TRADE_DONE && (in & 0x60) == 0x60) {
                out[0] = in;
                if (in  == 0x61) {
                    trade_pokemon = -1;
                    trade_state = trade_state_t::TRADE_WAIT;
                    printf("Trade done, wait\n");
                } else {
                    trade_state = trade_state_t::DONE;
                    printf("Trade done, mark done\n");
                }
            } else if(trade_state == trade_state_t::DONE && in == 0x00) {
                out[0] = 0;
                trade_state = trade_state_t::INIT;
                printf("Done, reinit\n");
            } else {
                out[0] = in;
                printf("Echo back\n");
            }
            break;
        
        default:
            out[0] = in;
            printf("Echo back 2\n");
            break;
    }

    return out[0];
}

int main() {
    // Enable UART/USB so we can print via USB Serial
    stdio_init_all();

    printf("Starting Pokemon (Gen 1) Trader Slave...\n");

    // Enable GPIO pins for SPI
    // Some are not neccessary due to `gpio_set_function`, but the CS pin must be set to force low
    gpio_init(GB_TX_PIN);
    gpio_init(GB_RX_PIN);
    gpio_init(GB_CLK_PIN);
	gpio_set_dir(GB_TX_PIN, true);  // output
	gpio_set_dir(GB_RX_PIN, false);  // input
	gpio_set_dir(GB_CLK_PIN, false);  // input


    // Serial State Variables
    bool clock_last_state = true;
    clock_t clock_last_pulse = clock();
    uint8_t cycle = 0x01; // We expect the first bit to be 0, this is a "temporary" fix
    uint8_t data = 0x00;
    uint8_t out = 0x00;

    // Pokemon trade variables
    uint8_t INPUT_BLOCK[418];
    uint8_t DATA_BLOCK[418];
    TraderPacket trader_packet = TraderPacket();
    trader_packet.to_bytes(DATA_BLOCK);
    size_t trade_counter = 0;

    printf("Trainer Block:\n");
    printbuf(DATA_BLOCK, 418);
    
    bool debug_data = true;
    while (true) {
        if (clock() - clock_last_pulse > (1000000 * 60)) {
            if (debug_data) {
                printf("Sent the Gameboy:\n");
                printbuf(DATA_BLOCK, 418);
                printf("Gameboy Sent:\n");
                printbuf(INPUT_BLOCK, 418);
                debug_data = false;
            }
        } else {
            debug_data = true;
        }
        bool clock_state = gpio_get(GB_CLK_PIN);

        // Switched to HIGH (tracking)
        if (!clock_last_state && clock_state) {
            clock_t clock_current_pulse = clock();
            // printf("LST STATE: %x   LTS: %x   TSC: %lu\n", clock_last_state, clock_state, clock_current_pulse - clock_last_pulse);
            clock_last_pulse = clock_current_pulse;

            // Write Data
            uint8_t out_bit = (out >> (7-cycle)) & 0b00000001;
            gpio_put(GB_TX_PIN, out_bit == 0b00000001);
        }
        // Switched to LOW (READING)
        else if (clock_last_state && !clock_state) {
            // Read Data
            data |= (gpio_get(GB_RX_PIN) ? 0b00000001 : 0b00000000) << (7-cycle);
            if (++cycle > 7) {
                if (data == 0x01) {
                    out = 0x02;
                }
                // Reset
                cycle = 0;
                out = handle_byte(data, trade_counter, DATA_BLOCK, INPUT_BLOCK);
                printf("Byte: 0x%x 0b%c%c%c%c%c%c%c%c\n", data, BYTE_TO_BINARY(data));
                data = 0x00;
            }
        }

        clock_last_state = clock_state;
    }

    printf("Its-a done! Goodbye.");
}