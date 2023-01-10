namespace CharacterMap {
    inline namespace GenerationOne {
        enum GenOneCharacterMap {
            // https://bulbapedia.bulbagarden.net/wiki/Character_encoding_(Generation_I)
            STOP_BYTE = 0x50, // Ends a string
            NULL_BYTE = 0x00, // Nothing, comes after the stop byte to fill in the rest

            // ENG Characters
            SPACE = 0x7F,
            A = 0x80,
            B = 0x81,
            C = 0x82,
            D = 0x83,
            E = 0x84,
            F = 0x85,
            G = 0x86,
            H = 0x87,
            I = 0x88,
            J = 0x89,
            K = 0x8A,
            L = 0x8B,
            M = 0x8C,
            N = 0x8D,
            O = 0x8E,
            P = 0x8F,
            Q = 0x90,
            R = 0x91,
            S = 0x92,
            T = 0x93,
            U = 0x94,
            V = 0x95,
            W = 0x96,
            X = 0x97,
            Y = 0x98,
            Z = 0x99,
            OPEN_PARENTHESIS = 0x9A,
            CLOSE_PARENTHESIS = 0x9B,
            COLON = 0x9C,
            SEMICOLON = 0x9D,
            OPEN_BRACKET = 0x9E,
            CLOSE_BRACKET = 0x9F,
            a = 0xA0,
            b = 0xA1,
            c = 0xA2,
            d = 0xA3,
            e = 0xA4,
            f = 0xA5,
            g = 0xA6,
            h = 0xA7,
            i = 0xA8,
            j = 0xA9,
            k = 0xAa,
            l = 0xAb,
            m = 0xAc,
            n = 0xAd,
            o = 0xAe,
            p = 0xAf,
            q = 0xB0,
            r = 0xB1,
            s = 0xB2,
            t = 0xB3,
            u = 0xB4,
            v = 0xB5,
            w = 0xB6,
            x = 0xB7,
            y = 0xB8,
            z = 0xB9,
            e_accent = 0xBA,
            apostrophe_d = 0xBB,
            apostrophe_l = 0xBC,
            apostrophe_s = 0xBD,
            apostrophe_t = 0xBE,
            apostrophe_v = 0xBF,
            apostrophe = 0xE0,
            PK_character = 0xE1,
            MN_character = 0xE2,
            hyphen = 0xE3,
            apostrophe_r = 0xE4,
            apostrophe_m = 0xE5,
            question_mark = 0xE6,
            exclamation_mark = 0xE7,
            
            // NOTE: `period` is never used in names. Instead use `decimal_point`
            // . (used as punctuation)
            period = 0xE8,

            hollow_play = 0xEC, // ▷
            solid_play = 0xED, // ▶
            solid_down = 0xEE, // ▼
            male_symbol = 0xEF, // ♂
            pokemon_dollar = 0xF0,
            multiply = 0xF1, // ×
            decimal_point = 0xF2, // . (used as a decimal point)
            slash_forward = 0xF3, // /
            comma = 0xF4, // ,
            female_symbol = 0xF5, // ♀
            zero = 0xF6,
            one = 0xF7,
            two = 0xF8,
            three = 0xF9,
            four = 0xFA,
            five = 0xFB,
            six = 0xFC,
            seven = 0xFD,
            eight = 0xFE,
            nine = 0xFF,

            // Characters that weren't removed from JPN when translating to ENG
            // Some seem bolded, some are symbols, others are katakana or hiragana letters
            JPN_A = 0x60,
            JPN_B = 0x61,
            JPN_C = 0x62,
            JPN_D = 0x63,
            JPN_E = 0x64,
            JPN_F = 0x65,
            JPN_G = 0x66,
            JPN_H = 0x67,
            JPN_I = 0x68,
            JPN_V = 0x69,
            JPN_S = 0x6A,
            JPN_L = 0x6B,
            JPN_M = 0x6C,
            JPN_colon = 0x6D,
            hiragana_i = 0x6E, // ぃ
            hiragana_u = 0x6F, // ぅ
            open_single_quote = 0x70, // ‘
            close_single_quote = 0x71, // ’
            open_double_quote = 0x72, // “
            close_double_quote = 0x73, // ”
            dot = 0x74, // ・
            ellipsis = 0x75, // ⋯
            hiragana_a = 0x76, // ぁ
            hiragana_e = 0x77, // ぇ
            hiragana_o = 0x78, // ぉ
            katakana_a = 0xE9, // ァ
            katakana_u = 0xEA, // ゥ
            katakana_e = 0xEB, // ェ
        };
    }


    inline namespace GenerationTwo {
        enum GenTwoCharacterMap {
            // https://bulbapedia.bulbagarden.net/wiki/Character_encoding_(Generation_I)
            STOP_BYTE = 0x50, // Ends a string
            NULL_BYTE = 0x00, // Nothing, comes after the stop byte to fill in the rest

            // ENG Characters
            SPACE = 0x7F,
            A = 0x80,
            B = 0x81,
            C = 0x82,
            D = 0x83,
            E = 0x84,
            F = 0x85,
            G = 0x86,
            H = 0x87,
            I = 0x88,
            J = 0x89,
            K = 0x8A,
            L = 0x8B,
            M = 0x8C,
            N = 0x8D,
            O = 0x8E,
            P = 0x8F,
            Q = 0x90,
            R = 0x91,
            S = 0x92,
            T = 0x93,
            U = 0x94,
            V = 0x95,
            W = 0x96,
            X = 0x97,
            Y = 0x98,
            Z = 0x99,
            OPEN_PARENTHESIS = 0x9A,
            CLOSE_PARENTHESIS = 0x9B,
            COLON = 0x9C,
            SEMICOLON = 0x9D,
            OPEN_BRACKET = 0x9E,
            CLOSE_BRACKET = 0x9F,
            a = 0xA0,
            b = 0xA1,
            c = 0xA2,
            d = 0xA3,
            e = 0xA4,
            f = 0xA5,
            g = 0xA6,
            h = 0xA7,
            i = 0xA8,
            j = 0xA9,
            k = 0xAa,
            l = 0xAb,
            m = 0xAc,
            n = 0xAd,
            o = 0xAe,
            p = 0xAf,
            q = 0xB0,
            r = 0xB1,
            s = 0xB2,
            t = 0xB3,
            u = 0xB4,
            v = 0xB5,
            w = 0xB6,
            x = 0xB7,
            y = 0xB8,
            z = 0xB9,

            A_umlaut = 0xC0,
            O_umlaut = 0xC1,
            U_umlaut = 0xC2,
            a_umlaut = 0xC3,
            o_umlaut = 0xC4,
            u_umlaut = 0xC5,
            e_accent = 0xEA,
            apostrophe_d = 0xD0,
            apostrophe_l = 0xD1,
            apostrophe_m = 0xD2,
            apostrophe_r = 0xD3,
            apostrophe_s = 0xD4,
            apostrophe_t = 0xD5,
            apostrophe_v = 0xD6,

            apostrophe = 0xE0,
            PK_character = 0xE1,
            MN_character = 0xE2,
            hyphen = 0xE3,
            question_mark = 0xE6,
            exclamation_mark = 0xE7,
            // NOTE: `period` is never used in names. Instead use `decimal_point`
            // . (used as punctuation)
            period = 0xE8,
            ampersand = 0xE9,

            hollow_play = 0xEC, // ▷
            solid_play = 0xED, // ▶
            solid_down = 0xEE, // ▼
            male_symbol = 0xEF, // ♂
            pokemon_dollar = 0xF0,
            multiply = 0xF1, // ×
            decimal_point = 0xF2, // . (used as a decimal point)
            slash_forward = 0xF3, // /
            comma = 0xF4, // ,
            female_symbol = 0xF5, // ♀
            zero = 0xF6,
            one = 0xF7,
            two = 0xF8,
            three = 0xF9,
            four = 0xFA,
            five = 0xFB,
            six = 0xFC,
            seven = 0xFD,
            eight = 0xFE,
            nine = 0xFF,

            // Characters that weren't removed from JPN when translating to ENG
            // Some seem bolded, some are symbols, others are katakana or hiragana letters
            JPN_A = 0x60,
            JPN_B = 0x61,
            JPN_C = 0x62,
            JPN_D = 0x63,
            JPN_E = 0x64,
            JPN_F = 0x65,
            JPN_G = 0x66,
            JPN_H = 0x67,
            JPN_I = 0x68,
            JPN_V = 0x69,
            JPN_S = 0x6A,
            JPN_L = 0x6B,
            JPN_M = 0x6C,
            JPN_colon = 0x6D,
            hiragana_i = 0x6E, // ぃ
            hiragana_u = 0x6F, // ぅ
            open_single_quote = 0x70, // ‘
            close_single_quote = 0x71, // ’
            open_double_quote = 0x72, // “
            close_double_quote = 0x73, // ”
            dot = 0x74, // ・
            ellipsis = 0x75, // ⋯
            hiragana_a = 0x76, // ぁ
            hiragana_e = 0x77, // ぇ
            hiragana_o = 0x78, // ぉ
            katakana_a = 0xE9, // ァ
            katakana_u = 0xEA, // ゥ
            katakana_e = 0xEB, // ェ
        };
    }
}