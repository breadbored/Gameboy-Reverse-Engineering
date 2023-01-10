#ifndef GEN1_H_
#define GEN1_H_

#include <stdio.h>
#include <string>
#include <strings.h>
#include "pico/stdlib.h"
#include "pico/stdio/driver.h"
#include "pico/time.h"
#include "pico/binary_info.h"

#define PKMN_BLANK						0x00

#define ITEM_1_HIGHLIGHTED				0xD0
#define ITEM_2_HIGHLIGHTED				0xD1
#define ITEM_3_HIGHLIGHTED				0xD2
#define ITEM_1_SELECTED					0xD4
#define ITEM_2_SELECTED					0xD5
#define ITEM_3_SELECTED					0xD6

#define PKMN_MASTER						0x01
#define PKMN_SLAVE						0x02
#define PKMN_CONNECTED					0x60
#define PKMN_WAIT						0x7F

#define PKMN_ACTION						0x60

#define PKMN_TRADE_CENTRE				ITEM_1_SELECTED
#define PKMN_COLOSSEUM					ITEM_2_SELECTED
#define PKMN_BREAK_LINK					ITEM_3_SELECTED

#define TRADE_CENTRE_WAIT				0xFD

enum connection_state_t {
	TRADE_CENTRE,
	NOT_CONNECTED,
	CONNECTED,
	COLOSSEUM,
};

enum trade_state_t {
	INIT,
	READY,
	FIRST_DETECTED_WAIT,
	DATA_TX,
	DATA_TX_WAIT,
	DATA_TX_START,
	DATA_TX_RANDOM,
    DATA_TX_PATCH,
    TRADE_WAIT,
    TRADE_DONE,
    DONE
};

enum GenOneDex {
    NULL_POKEMON = 0xFF,
    RHYDON = 0x01,
    KANGASKHAN = 0x02,
    NIDORAN_MALE = 0x03,
    CLEFAIRY = 0x04,
    SPEAROW = 0x05,
    VOLTORB = 0x06,
    NIDOKING = 0x07,
    SLOWBRO = 0x08,
    IVYSAUR = 0x09,
    EXEGGUTOR = 0x0A,
    LICKITUNG = 0x0B,
    EXEGGCUTE = 0x0C,
    GRIMER = 0x0D,
    GENGAR = 0x0E,
    NIDORAN_FEMALE = 0x0F,
    NIDOQUEEN = 0x10,
    CUBONE = 0x11,
    RHYHORN = 0x12,
    LAPRAS = 0x13,
    ARCANINE = 0x14,
    MEW = 0x15,
    GYARADOS = 0x16,
    SHELLDER = 0x17,
    TENTACOOL = 0x18,
    GASTLY = 0x19,
    SCYTHER = 0x1A,
    STARYU = 0x1B,
    BLASTOISE = 0x1C,
    PINSIR = 0x1D,
    TANGELA = 0x1E,
    GROWLITHE = 0x21,
    ONIX = 0x22,
    FEAROW = 0x23,
    PIDGEY = 0x24,
    SLOWPOKE = 0x25,
    KADABRA = 0x26,
    GRAVELER = 0x27,
    CHANSEY = 0x28,
    MACHOKE = 0x29,
    MR_MIME = 0x2A,
    HITMONLEE = 0x2B,
    HITMONCHAN = 0x2C,
    ARBOK = 0x2D,
    PARASECT = 0x2E,
    PSYDUCK = 0x2F,
    DROWZEE = 0x30,
    GOLEM = 0x31,
    MAGMAR = 0x33,
    ELECTABUZZ = 0x35,
    MAGNETON = 0x36,
    KOFFING = 0x37,
    MANKEY = 0x39,
    SEEL = 0x3A,
    DIGLETT = 0x3B,
    TAUROS = 0x3C,
    FARFETCH = 0x40,
    VENONAT = 0x41,
    DRAGONITE = 0x42,
    DODUO = 0x46,
    POLIWAG = 0x47,
    JYNX = 0x48,
    MOLTRES = 0x49,
    ARTICUNO = 0x4A,
    ZAPDOS = 0x4B,
    DITTO = 0x4C,
    MEOWTH = 0x4D,
    KRABBY = 0x4E,
    VULPIX = 0x52,
    NINETALES = 0x53,
    PIKACHU = 0x54,
    RAICHU = 0x55,
    DRATINI = 0x58,
    DRAGONAIR = 0x59,
    KABUTO = 0x5A,
    KABUTOPS = 0x5B,
    HORSEA = 0x5C,
    SEADRA = 0x5D,
    SANDSHREW = 0x60,
    SANDSLASH = 0x61,
    OMANYTE = 0x62,
    OMASTAR = 0x63,
    JIGGLYPUFF = 0x64,
    WIGGLYTUFF = 0x65,
    EEVEE = 0x66,
    FLAREON = 0x67,
    JOLTEON = 0x68,
    VAPOREON = 0x69,
    MACHOP = 0x6A,
    ZUBAT = 0x6B,
    EKANS = 0x6C,
    PARAS = 0x6D,
    POLIWHIRL = 0x6E,
    POLIWRATH = 0x6F,
    WEEDLE = 0x70,
    KAKUNA = 0x71,
    BEEDRILL = 0x72,
    DODRIO = 0x74,
    PRIMEAPE = 0x75,
    DUGTRIO = 0x76,
    VENOMOTH = 0x77,
    DEWGONG = 0x78,
    CATERPIE = 0x7B,
    METAPOD = 0x7C,
    BUTTERFREE = 0x7D,
    MACHAMP = 0x7E,
    GOLDUCK = 0x80,
    HYPNO = 0x81,
    GOLBAT = 0x82,
    MEWTWO = 0x83,
    SNORLAX = 0x84,
    MAGIKARP = 0x85,
    MUK = 0x88,
    KINGLER = 0x8A,
    CLOYSTER = 0x8B,
    ELECTRODE = 0x8D,
    CLEFABLE = 0x8E,
    WEEZING = 0x8F,
    PERSIAN = 0x90,
    MAROWAK = 0x91,
    HAUNTER = 0x93,
    ABRA = 0x94,
    ALAKAZAM = 0x95,
    PIDGEOTTO = 0x96,
    PIDGEOT = 0x97,
    STARMIE = 0x98,
    BULBASAUR = 0x99,
    VENUSAUR = 0x9A,
    TENTACRUEL = 0x9B,
    GOLDEEN = 0x9D,
    SEAKING = 0x9E,
    PONYTA = 0xA3,
    RAPIDASH = 0xA4,
    RATTATA = 0xA5,
    RATICATE = 0xA6,
    NIDORINO = 0xA7,
    NIDORINA = 0xA8,
    GEODUDE = 0xA9,
    PORYGON = 0xAA,
    AERODACTYL = 0xAB,
    MAGNEMITE = 0xAD,
    CHARMANDER = 0xB0,
    SQUIRTLE = 0xB1,
    CHARMELEON = 0xB2,
    WARTORTLE = 0xB3,
    CHARIZARD = 0xB4,
    ODDISH = 0xB9,
    GLOOM = 0xBA,
    VILEPLUME = 0xBB,
    BELLSPROUT = 0xBC,
    WEEPINBELL = 0xBD,
    VICTREEBEL = 0xBE,
};

enum PokemonMove {
    ABSORB = 0x47,
    ACID = 0x33,
    ACID_ARMOR = 0x97,
    AGILITY = 0x61,
    AMNESIA = 0x85,
    AURORA_BEAM = 0x3E,
    BARRAGE = 0x8C,
    BARRIER = 0x70,
    BIDE = 0x75,
    BIND = 0x14,
    BITE = 0x2C,
    BLIZZARD = 0x3B,
    BODY_SLAM = 0x22,
    BONE_CLUB = 0x7D,
    BONEMERANG = 0x9B,
    BUBBLE = 0x91,
    BUBBLEBEAM = 0x3D,
    CLAMP = 0x80,
    COMET_PUNCH = 0x04,
    CONFUSE_RAY = 0x6D,
    CONFUSION = 0x5D,
    CONSTRICT = 0x84,
    CONVERSION = 0xA0,
    COUNTER = 0x44,
    CRABHAMMER = 0x98,
    CUT = 0x0F,
    DEFENSE_CURL = 0x6F,
    DIG = 0x5B,
    DISABLE = 0x32,
    DIZZY_PUNCH = 0x92,
    DOUBLE_EDGE = 0x26,
    DOUBLE_KICK = 0x18,
    DOUBLE_SLAP = 0x03,
    DOUBLE_TEAM = 0x68,
    DRAGON_RAGE = 0x52,
    DREAM_EATER = 0x8A,
    DRILL_PECK = 0x41,
    EARTHQUAKE = 0x59,
    EGG_BOMB = 0x79,
    EMBER = 0x34,
    EXPLOSION = 0x99,
    FIRE_BLAST = 0x7E,
    FIRE_PUNCH = 0x07,
    FIRE_SPIN = 0x53,
    FISSURE = 0x5A,
    FLAMETHROWER = 0x35,
    FLASH = 0x94,
    FLY = 0x13,
    FOCUS_ENERGY = 0x74,
    FURY_ATTACK = 0x1F,
    FURY_SWIPES = 0x9A,
    GLARE = 0x89,
    GROWL = 0x2D,
    GROWTH = 0x4A,
    GUILLOTINE = 0x0C,
    GUST = 0x10,
    HARDEN = 0x6A,
    HAZE = 0x72,
    HEADBUTT = 0x1D,
    HI_JUMP_KICK = 0x88,
    HORN_ATTACK = 0x1E,
    HORN_DRILL = 0x20,
    HYDRO_PUMP = 0x38,
    HYPER_BEAM = 0x3F,
    HYPER_FANG = 0x9E,
    HYPNOSIS = 0x5F,
    ICE_BEAM = 0x3A,
    ICE_PUNCH = 0x08,
    JUMP_KICK = 0x1A,
    KARATE_CHOP = 0x02,
    KINESIS = 0x86,
    LEECH_LIFE = 0x8D,
    LEECH_SEED = 0x49,
    LEER = 0x2B,
    LICK = 0x7A,
    LIGHT_SCREEN = 0x71,
    LOVELY_KISS = 0x8E,
    LOW_KICK = 0x43,
    MEDITATE = 0x60,
    MEGA_DRAIN = 0x48,
    MEGA_KICK = 0x19,
    MEGA_PUNCH = 0x05,
    METRONOME = 0x76,
    MIMIC = 0x66,
    MINIMIZE = 0x6B,
    MIRROR_MOVE = 0x77,
    MIST = 0x36,
    NIGHT_SHADE = 0x65,
    PAYDAY = 0x06,
    PECK = 0x40,
    PETAL_DANCE = 0x50,
    PIN_MISSILE = 0x2A,
    POISON_GAS = 0x8B,
    POISON_STING = 0x28,
    POISON_POWDER = 0x4D,
    POUND = 0x01,
    PSYBEAM = 0x3C,
    PSYCHIC = 0x5E,
    PSYWAVE = 0x95,
    QUICK_ATTACK = 0x62,
    RAGE = 0x63,
    RAZOR_LEAF = 0x4B,
    RAZOR_WIND = 0x0D,
    RECOVER = 0x69,
    REFLECT = 0x73,
    REST = 0x9C,
    ROAR = 0x2E,
    ROCK_SLIDE = 0x9D,
    ROCK_THROW = 0x58,
    ROLLING_KICK = 0x1B,
    SAND_ATTACK = 0x1C,
    SCRATCH = 0x0A,
    SCREECH = 0x67,
    SEISMIC_TOSS = 0x45,
    SELFDESTRUCT = 0x78,
    SHARPEN = 0x9F,
    SING = 0x2F,
    SKULL_BASH = 0x82,
    SKY_ATTACK = 0x8F,
    SLAM = 0x15,
    SLASH = 0xA3,
    SLEEP_POWDER = 0x4F,
    SLUDGE = 0x7C,
    SMOG = 0x7B,
    SMOKESCREEN = 0x6C,
    SOFTBOILED = 0x87,
    SOLAR_BEAM = 0x4C,
    SONIC_BOOM = 0x31,
    SPIKE_CANNON = 0x83,
    SPLASH = 0x96,
    SPORE = 0x93,
    STOMP = 0x17,
    STRENGTH = 0x46,
    STRING_SHOT = 0x51,
    STRUGGLE = 0xA5,
    STUN_SPORE = 0x4E,
    SUBMISSION = 0x42,
    SUBSTITUTE = 0xA4,
    SUPER_FANG = 0xA2,
    SUPERSONIC = 0x30,
    SURF = 0x39,
    SWIFT = 0x81,
    SWORDS_DANCE = 0x0E,
    TACKLE = 0x21,
    TAIL_WHIP = 0x27,
    TAKE_DOWN = 0x24,
    TELEPORT = 0x64,
    THRASH = 0x25,
    THUNDER = 0x57,
    THUNDERBOLT = 0x55,
    THUNDER_PUNCH = 0x09,
    THUNDERSHOCK = 0x54,
    THUNDERWAVE = 0x56,
    TOXIC = 0x5C,
    TRANSFORM = 0x90,
    TRI_ATTACK = 0xA1,
    TWINEEDLE = 0x29,
    VICE_GRIP = 0x0B,
    VINE_WHIP = 0x16,
    WATER_GUN = 0x37,
    WATERFALL = 0x7F,
    WHIRLWIND = 0x12,
    WING_ATTACK = 0x11,
    WITHDRAW = 0x6E,
    WRAP = 0x23,
};

enum StatusCondition {
    NONE = 0x00,
    ASLEEP = 0x04,
    BURNED = 0x10,
    FROZEN = 0x20,
    PARALYZED = 0x40,
    POISONED = 0x08,
};

enum PokemonType {
    NORMAL = 0x00,
    FIGHTING = 0x01,
    FLYING = 0x02,
    POISON = 0x03,
    GROUND = 0x04,
    ROCK = 0x05,
    BUG = 0x07,
    GHOST = 0x08,
    FIRE = 0x14,
    WATER = 0x15,
    GRASS = 0x16,
    ELECTRIC = 0x17,
    PSYCHIC_TYPE = 0x18,
    ICE = 0x19,
    DRAGON = 0x1A,
};

struct SelectedPokemon {
    // Number of pokemon in the trainer's party
    uint8_t number = 0x00;
    GenOneDex pokemon[6] = {
        GenOneDex::NULL_POKEMON,
        GenOneDex::NULL_POKEMON,
        GenOneDex::NULL_POKEMON,
        GenOneDex::NULL_POKEMON,
        GenOneDex::NULL_POKEMON,
        GenOneDex::NULL_POKEMON,
    };

    void to_bytes(uint8_t *out) {
        uint8_t res[8] = {
            number,
            pokemon[0],
            pokemon[1],
            pokemon[2],
            pokemon[3],
            pokemon[4],
            pokemon[5],
            0xFF,
        };
        for (size_t i = 0; i < 8; i++) {
            out[i] = res[i];
        }
    };
};


// Luckily it follows the save file structure closely enough
// https://bulbapedia.bulbagarden.net/wiki/Pok%C3%A9mon_data_structure_(Generation_I)
struct PartyMember {
    GenOneDex pokemon = GenOneDex::MEW;
    uint16_t current_hp = 100;
    uint16_t max_hp = 130;
    uint8_t level = 40;
    StatusCondition status = StatusCondition::NONE;
    PokemonType type1 = PokemonType::PSYCHIC_TYPE;
    PokemonType type2 = PokemonType::PSYCHIC_TYPE; // If only one type, copy the first
    uint8_t catch_rate_or_held_item = 0xFF; // R/G/B/Y (catch rate), G/S/C (held item), and Stadium (held item) use this byte differently
    PokemonMove move1 = PokemonMove::TELEPORT;
    PokemonMove move2 = PokemonMove::PSYWAVE;
    PokemonMove move3 = PokemonMove::PSYCHIC;
    PokemonMove move4 = PokemonMove::FLY;
    uint16_t original_trainer_id = 0xA455; // In decimal, these are the funny numbers

    // -   Experience is complicated. You must look up the Pokemon you are trying to trade
    //      in the following table and apply the experience points that match the level.
    //      EXP LVL Table for gen 1: https://pwo-wiki.info/index.php/Generation_I_Experience_Charts
    //      That source was the best I could find for Gen 1. If you find another, submit a PR or open an issue and I'll fix it
    // -   Experience is a 24bit number, we will be dropping the MSB to acheive that
    uint32_t experience = 190148;

    // Effort Values
    // These are very specific to the Pokemon and who they battled in the past or what vitamins they were fed
    // Luckily, these get recalculated when you level them up, or when you put them in a box and then put them back in your party
    // For this example, I will take the max value and scale it to the level (65535 * 0.40) = 26214
    uint16_t HP_ev = 26214;
    uint16_t attack_ev = 26214;
    uint16_t defense_ev = 26214;
    uint16_t speed_ev = 26214;
    uint16_t special_ev = 26214;

    // IVs are a 4 bit number, so the max value is 15 (0-15 = 0b0000-0b1111 = 0x0-0xF)
    // These have been broken out for legibility, but will be condensed to only 2 bytes
    uint8_t attack_iv = 0xF;
    uint8_t defense_iv = 0xF;
    uint8_t speed_iv = 0xF;
    uint8_t special_iv = 0xF;

    uint8_t move1_pp = 20;
    uint8_t move2_pp = 15;
    uint8_t move3_pp = 10;
    uint8_t move4_pp = 15;

    uint16_t attack = 100;
    uint16_t defense = 100;
    uint16_t speed = 100;
    uint16_t special = 100;

    void to_bytes(uint8_t *out) {
        uint8_t res[44] = {
            pokemon,
            (uint8_t) (current_hp >> 8),
            (uint8_t) (current_hp & 0x00FF),
            level,
            status,
            type1,
            type2,
            catch_rate_or_held_item,
            move1,
            move2,
            move3,
            move4,
            (uint8_t) (original_trainer_id >> 8),
            (uint8_t) (original_trainer_id & 0x00FF),
            (uint8_t) (experience & 0x000000FF),
            (uint8_t) (experience & 0x0000FF00) >> 8,
            (uint8_t) (experience & 0x00FF0000) >> 16,
            (uint8_t) (HP_ev >> 8),
            (uint8_t) (HP_ev & 0x00FF),
            (uint8_t) (attack_ev >> 8),
            (uint8_t) (attack_ev & 0x00FF),
            (uint8_t) (defense_ev >> 8),
            (uint8_t) (defense_ev & 0x00FF),
            (uint8_t) (speed_ev >> 8),
            (uint8_t) (speed_ev & 0x00FF),
            (uint8_t) (special_ev >> 8),
            (uint8_t) (special_ev & 0x00FF),
            (uint8_t) (((attack_iv & 0xF) << 4) | (defense_iv & 0xF)),
            (uint8_t) (((speed_iv & 0xF) << 4) | (special_iv & 0xF)),
            move1_pp,
            move2_pp,
            move3_pp,
            move4_pp,
            level,
            (uint8_t) (max_hp >> 8),
            (uint8_t) (max_hp & 0x00FF),
            (uint8_t) (attack >> 8),
            (uint8_t) (attack & 0x00FF),
            (uint8_t) (defense >> 8),
            (uint8_t) (defense & 0x00FF),
            (uint8_t) (speed >> 8),
            (uint8_t) (speed & 0x00FF),
            (uint8_t) (special >> 8),
            (uint8_t) (special & 0x00FF),
        };
        for (size_t i = 0; i < 44; i++) {
            out[i] = res[i];
        }
    };
};

#endif /* GEN1_H_ */