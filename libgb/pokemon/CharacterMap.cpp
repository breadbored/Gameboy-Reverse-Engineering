#include <stdio.h>
#include <string>
#include <strings.h>
#include "CharacterMap.hpp"

namespace CharacterMap {
    inline namespace GenerationOne {
        class PokeString {
            public:
                std::string word;
                PokeString(std::string wordStr) {
                    word = wordStr;
                };
                GenOneCharacterMap *getCharMap() {
                    GenOneCharacterMap pokeword[11];
                    bool terminated = false;
                    for (int index = 0; index < 11; index++) {

                        // String end encoding
                        if (index >= word.length() || index == 10) {
                            if (terminated) {
                                pokeword[11] = NULL_BYTE;
                            } else {
                                terminated = true;
                                pokeword[11] = STOP_BYTE;
                            }
                            continue;
                        }

                        const char letter = word[index];

                        switch (letter)
                        {
                        case ' ':
                            pokeword[index] = SPACE;
                            break;
                        case 'A':
                            pokeword[index] = A;
                            break;
                        case 'B':
                            pokeword[index] = B;
                            break;
                        case 'C':
                            pokeword[index] = C;
                            break;
                        case 'D':
                            pokeword[index] = D;
                            break;
                        case 'E':
                            pokeword[index] = E;
                            break;
                        case 'F':
                            pokeword[index] = F;
                            break;
                        case 'G':
                            pokeword[index] = G;
                            break;
                        case 'H':
                            pokeword[index] = H;
                            break;
                        case 'I':
                            pokeword[index] = I;
                            break;
                        case 'J':
                            pokeword[index] = J;
                            break;
                        case 'K':
                            pokeword[index] = K;
                            break;
                        case 'L':
                            pokeword[index] = L;
                            break;
                        case 'M':
                            pokeword[index] = M;
                            break;
                        case 'N':
                            pokeword[index] = N;
                            break;
                        case 'O':
                            pokeword[index] = O;
                            break;
                        case 'P':
                            pokeword[index] = P;
                            break;
                        case 'Q':
                            pokeword[index] = Q;
                            break;
                        case 'R':
                            pokeword[index] = R;
                            break;
                        case 'S':
                            pokeword[index] = S;
                            break;
                        case 'T':
                            pokeword[index] = T;
                            break;
                        case 'U':
                            pokeword[index] = U;
                            break;
                        case 'V':
                            pokeword[index] = V;
                            break;
                        case 'W':
                            pokeword[index] = W;
                            break;
                        case 'X':
                            pokeword[index] = X;
                            break;
                        case 'Y':
                            pokeword[index] = Y;
                            break;
                        case 'Z':
                            pokeword[index] = Z;
                            break;
                        case 'a':
                            pokeword[index] = a;
                            break;
                        case 'b':
                            pokeword[index] = b;
                            break;
                        case 'c':
                            pokeword[index] = c;
                            break;
                        case 'd':
                            pokeword[index] = d;
                            break;
                        case 'e':
                            pokeword[index] = e;
                            break;
                        case 'f':
                            pokeword[index] = f;
                            break;
                        case 'g':
                            pokeword[index] = g;
                            break;
                        case 'h':
                            pokeword[index] = h;
                            break;
                        case 'i':
                            pokeword[index] = i;
                            break;
                        case 'j':
                            pokeword[index] = j;
                            break;
                        case 'k':
                            pokeword[index] = k;
                            break;
                        case 'l':
                            pokeword[index] = l;
                            break;
                        case 'm':
                            pokeword[index] = m;
                            break;
                        case 'n':
                            pokeword[index] = n;
                            break;
                        case 'o':
                            pokeword[index] = o;
                            break;
                        case 'p':
                            pokeword[index] = p;
                            break;
                        case 'q':
                            pokeword[index] = q;
                            break;
                        case 'r':
                            pokeword[index] = r;
                            break;
                        case 's':
                            pokeword[index] = s;
                            break;
                        case 't':
                            pokeword[index] = t;
                            break;
                        case 'u':
                            pokeword[index] = u;
                            break;
                        case 'v':
                            pokeword[index] = v;
                            break;
                        case 'w':
                            pokeword[index] = w;
                            break;
                        case 'x':
                            pokeword[index] = x;
                            break;
                        case 'y':
                            pokeword[index] = y;
                            break;
                        case 'z':
                            pokeword[index] = z;
                            break;
                        case '(':
                            pokeword[index] = OPEN_PARENTHESIS;
                            break;
                        case ')':
                            pokeword[index] = CLOSE_PARENTHESIS;
                            break;
                        case '[':
                            pokeword[index] = OPEN_BRACKET;
                            break;
                        case ']':
                            pokeword[index] = CLOSE_BRACKET;
                            break;
                        case ':':
                            pokeword[index] = COLON;
                            break;
                        case ';':
                            pokeword[index] = SEMICOLON;
                            break;
                        case 'é':
                            pokeword[index] = e_accent;
                            break;
                        
                        default:
                            break;
                        }
                    }
                }
        };
    }


    inline namespace GenerationTwo {
        
    }
}