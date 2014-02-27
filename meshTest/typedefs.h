#pragma once
#include <bitset>

typedef std::bitset<8> keyBuffer;

struct eID_Pair { int eID_1; int eID_2; };

enum gameState  { RETURN, MENU, GAME, MAP, SHOP };
enum KeyState   { KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_ASC, KEY_DSC, KEY_FIRE, KEY_EXIT };

