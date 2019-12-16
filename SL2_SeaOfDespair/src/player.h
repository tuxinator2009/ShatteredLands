/*************************************************************************
 * Shattered Lands: Towers of Perdition                                  *
 * C opyright (C) 2019  Justin (tuxinator2009) Davis                     *
 *                                                                       *
 * This program is free software: you can redistribute it and/or modify  *
 * it under the terms of the GNU General Public License as published by  *
 * the Free Software Foundation, either version 3 of the License, or     *
 * (at your option) any later version.                                   *
 *                                                                       *
 * This program is distributed in the hope that it will be useful,       *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 * GNU General Public License for more details.                          *
 *                                                                       *
 * You should have received a copy of the GNU General Public License     *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 *************************************************************************/

#ifndef PLAYER_H
#define PLAYER_H

#include <Arduino.h>

#define PLAYER_START_MAP		0
#define PLAYER_START_CHUNK	0

#define PLAYER_LEVEL1_HP				16
#define PLAYER_LEVEL99_HP				999
#define PLAYER_LEVEL1_MP				1
#define PLAYER_LEVEL99_MP				99
//#define PLAYER_LEVEL1_ATTACK		3
//#define PLAYER_LEVEL99_ATTACK		255
//#define PLAYER_LEVEL1_DEFENSE		1
//#define PLAYER_LEVEL99_DEFENSE	255
//#define PLAYER_LEVEL1_AGILITY		2
//#define PLAYER_LEVEL99_AGILITY	255
//#define PLAYER_LEVEL1_MAGIC			1
//#define PLAYER_LEVEL99_MAGIC		255
#define PLAYER_LEVEL1_NEXTEXP		16
#define PLAYER_LEVEL99_NEXTEXP	999

#define PLAYER_HP				PLAYER_LEVEL1_HP, PLAYER_LEVEL99_HP
#define PLAYER_MP				PLAYER_LEVEL1_MP, PLAYER_LEVEL99_MP
//#define PLAYER_ATTACK		PLAYER_LEVEL1_ATTACK, PLAYER_LEVEL99_ATTACK
//#define PLAYER_DEFENSE	PLAYER_LEVEL1_DEFENSE, PLAYER_LEVEL99_DEFENSE
//#define PLAYER_AGILITY	PLAYER_LEVEL1_AGILITY, PLAYER_LEVEL99_AGILITY
//#define PLAYER_MAGIC		PLAYER_LEVEL1_MAGIC, PLAYER_LEVEL99_MAGIC
#define PLAYER_NEXTEXP	PLAYER_LEVEL1_NEXTEXP, PLAYER_LEVEL99_NEXTEXP

#define SEAL_TOWER_1	1
#define SEAL_TOWER_2	2
#define SEAL_TOWER_3	4
#define SEAL_TOWER_4	8

#define KEY_TOWER_1		16
#define KEY_TOWER_2		32
#define KEY_TOWER_3		64
#define KEY_TOWER_4		128

static const uint8_t START_MAP = 1;//0;
static const int8_t START_X = 3;//2;
static const int8_t START_Y = 3;//5;
static const int8_t START_CHUNK = 1;//28;

static const uint8_t PROGMEM level1Stats[] = {3, 1, 2, 1};

struct Player //location: xxx yyy ff
{
	uint8_t game[3];
	uint8_t name[8];
	uint8_t map;
	int8_t chunkID;
	int8_t x;
	int8_t y;
	int8_t dir;
	int16_t hp;
	uint8_t mp;
	uint8_t level;
	uint8_t statBonus[4];
	int16_t exp;
	int16_t gold;
	uint32_t largeChests;
	uint16_t immunity; //HY DK AO RK AQ FR NK CR (2-bits per spell)
	uint8_t items[8]; //8 bytes instead of 4 bytes in SRAM but saves 
	//uint32_t items; //POTN XPTN ETHR ELXR ANDT BELL EDRP RMDY (4-bits each indicating quantity)
	uint8_t spells[8]; //HOLY DARK AERO ROCK AQUA FIRE NUKE CURE (4-bits each indicating spell level)
	uint8_t seal;
	uint8_t equipment; //2-bits per type (accessory boots armor sword)
};

static Player player =
{
	//game
	{'S', 'L', '2'},
	//name
	{CHAR_NULL, CHAR_NULL, CHAR_NULL, CHAR_NULL, CHAR_NULL, CHAR_NULL, CHAR_NULL, CHAR_NULL},
	//map, chunkID, x, y, dir
	START_MAP,START_CHUNK,START_X,START_Y,DIR_DOWN,
	//hp, mp
	PLAYER_LEVEL1_HP,PLAYER_LEVEL1_MP,
	//level, attackBonus, defenseBonus, agilityBonus, magicBonus, exp, gold
	1,{0,0,0,0},0,0,
	//largeChests
	0x00000000,
	//immunity (HOLY DARK AERO ROCK AQUA FIRE NUKE CURE)
	//10 10 10 10 10 10 10 10
	0xAAAA,
	//Items (POTION S-POTION, X-POTION, ETHER, S-ETHER, X-ETHER, ELIXER, X-ELIXIR)
	{0, 0, 0, 0, 0, 0, 0, 0},
	//Spells (CURE NUKE FIRE AQUA ROCK AERO DARK HOLY)
	{0, 0, 0, 0, 0, 0, 0, 0},
	//Seal
	//12
	//48
	0x00,
	//Equipment
	//mmssddaa
	0x00
};

//This takes less than 1 microsecond
//((lvl99 - lvl1) * (level - 1) * (level - 1) / 9604) + lvl1
int16_t getStat(int16_t level, int16_t lvl1, int16_t lvl99)
{
	//exponential curve doesn't work as well
	//reason: first 4 levels player has the same stats
	//return (int16_t)(((long)(lvl99 - lvl1) * (long)((level - 1) * (level - 1))) / 9604l) + lvl1;
	//linear curve works better
	return (int16_t)(((long)(lvl99 - lvl1) * (long)(level - 1)) / 98l) + lvl1;
}

int8_t numItems(int8_t id)
{
	return player.items[id];
}

void addRemoveItems(int8_t id, int8_t amount)
{
	player.items[id] += amount;
	if (player.items[id] > 99)
		player.items[id] = 99;
}

bool saveExists()
{
	if (EEPROM.read(EEPROM_STORAGE_SPACE_START) != 'S')
		return false;
	if (EEPROM.read(EEPROM_STORAGE_SPACE_START + 1) != 'L')
		return false;
	if (EEPROM.read(EEPROM_STORAGE_SPACE_START + 2) != '1')
		return false;
	return true;
}

void loadGame()
{
	uint8_t *pValue = (uint8_t*)(void*)&player;
	uint16_t address = EEPROM_STORAGE_SPACE_START;
	for (uint8_t i = 0; i < sizeof(player); ++i)
		*pValue++ = EEPROM.read(address++);
}

void saveGame()
{
	const uint8_t *pValue = (const uint8_t*)(const void*)&player;
	uint16_t address = EEPROM_STORAGE_SPACE_START;
	for (uint8_t i = 0; i < sizeof(player); ++i)
		EEPROM.update(address++, *pValue++);
}

void fillSpellRect(uint8_t pattern)
{
	uint8_t startX = (selection >> 1) * 32;
	uint8_t stopX = startX + 30;
	uint8_t y = (selection & 1) + 6;
	for (uint8_t x = startX; x <= stopX; ++x)
		arduboy.sBuffer[y * 128 + x] = pattern;
}

void drawSpells()
{
	cursorX = 1;
	cursorY = 48;
	lineStartX = 1;
	for (uint8_t i = 0; i < 8; ++i)
	{
		cursorX = ((i & 6) << 4) + 1;
		cursorY = ((i & 1) << 3) + 48;
		if (player.spells[i] > 0)
		{
			drawName(wordSpells + i * 4, 4);
			drawChar(CHAR_0 + player.spells[i]);
		}
		if (i == 3)
			drawChar(CHAR_LINE);
	}
}

void drawStat(int16_t value, int16_t maxValue)
{
	int8_t x = cursorX;
	cursorX += 12;
	drawNumber(value);
	cursorX = x + 18;
	drawChar(CHAR_SLASH);
	cursorX += 12;
	drawNumber(maxValue);
}

void drawPlayerName()
{
	for (uint8_t i = 0; i < 8; ++i)
		drawChar(player.name[i]);
}

void playerUseItem(int8_t id)
{
	int16_t amountHP = 0;
	uint8_t amountMP = 0;
	int16_t maxHP = getStat(player.level, PLAYER_HP);
	uint8_t maxMP = getStat(player.level, PLAYER_MP);
	if (id < 3)
		amountHP = (int16_t)pgm_read_byte(healItems + id);
	else if (id < 6)
		amountMP = (uint8_t)pgm_read_byte(healItems + id);
	else if (id == 6)
	{
		amountHP = maxHP >> 1;
		amountMP = maxMP >> 1;
	}
	else if (id == 7)
	{
		amountHP = maxHP;
		amountMP = maxMP;
	}
	player.hp += amountHP;
	player.mp += amountMP;
	if (player.hp > maxHP)
		player.hp = maxHP;
	if (player.mp > maxMP)
		player.mp = maxMP;
}
//16*lvl + lvl * (magic / 3)
/* Player's Stats
    16  1   3   1   2   0  16
   999 99 255 255 255 255 255
Lvl HP MP ATK DEF AGL MGK Exp TExp
 1  16  1   3   1   2   0  16
 2  26  2   5   3   4   2  26
 3  36  3   8   6   7   5  36
 4  46  4  10   8   9   7  46
 5  56  5  13  11  12  10  56
 6  66  6  15  13  14  13  66
 7  76  7  18  16  17  15  76
 8  86  8  21  19  20  18  86
 9  96  9  23  21  22  20  96
10 106 10  26  24  25  23 106
11 116 11  28  26  27  26 116
12 126 12  31  29  30  28 126
13 136 13  33  32  32  31 136
14 146 14  36  34  35  33 146
15 156 15  39  37  38  36 156
16 166 16  41  39  40  39 166
17 176 17  44  42  43  41 176
18 186 18  46  45  45  44 186
19 196 19  49  47  48  46 196
20 206 20  51  50  51  49 206
21 216 21  54  52  53  52 216
22 226 22  57  55  56  54 226
23 236 23  59  58  58  57 236
24 246 24  62  60  61  59 246
25 256 25  64  63  63  62 256
26 266 26  67  65  66  65 266
27 276 27  69  68  69  67 276
28 286 28  72  70  71  70 286
29 296 29  75  73  74  72 296
30 306 30  77  76  76  75 306
31 316 31  80  78  79  78 316
32 326 32  82  81  82  80 326
33 336 33  85  83  84  83 336
34 347 34  87  86  87  85 347
35 357 35  90  89  89  88 357
36 367 36  93  91  92  91 367
37 377 37  95  94  94  93 377
38 387 38  98  96  97  96 387
39 397 39 100  99 100  98 397
40 407 40 103 102 102 101 407
41 417 41 105 104 105 104 417
42 427 42 108 107 107 106 427
43 437 43 111 109 110 109 437
44 447 44 113 112 113 111 447
45 457 45 116 115 115 114 457
46 467 46 118 117 118 117 467
47 477 47 121 120 120 119 477
48 487 48 123 122 123 122 487
49 497 49 126 125 125 124 497
50 507 50 129 128 128 127 507
51 517 51 131 130 131 130 517
52 527 52 134 133 133 132 527
53 537 53 136 135 136 135 537
54 547 54 139 138 138 137 547
55 557 55 141 140 141 140 557
56 567 56 144 143 143 143 567
57 577 57 147 146 146 145 577
58 587 58 149 148 149 148 587
59 597 59 152 151 151 150 597
60 607 60 154 153 154 153 607
61 617 61 157 156 156 156 617
62 627 62 159 159 159 158 627
63 637 63 162 161 162 161 637
64 647 64 165 164 164 163 647
65 657 65 167 166 167 166 657
66 667 66 170 169 169 169 667
67 678 67 172 172 172 171 678
68 688 68 175 174 174 174 688
69 698 69 177 177 177 176 698
70 708 70 180 179 180 179 708
71 718 71 183 182 182 182 718
72 728 72 185 185 185 184 728
73 738 73 188 187 187 187 738
74 748 74 190 190 190 189 748
75 758 75 193 192 193 192 758
76 768 76 195 195 195 195 768
77 778 77 198 197 198 197 778
78 788 78 201 200 200 200 788
79 798 79 203 203 203 202 798
80 808 80 206 205 205 205 808
81 818 81 208 208 208 208 818
82 828 82 211 210 211 210 828
83 838 83 213 213 213 213 838
84 848 84 216 216 216 215 848
85 858 85 219 218 218 218 858
86 868 86 221 221 221 221 868
87 878 87 224 223 224 223 878
88 888 88 226 226 226 226 888
89 898 89 229 229 229 228 898
90 908 90 231 231 231 231 908
91 918 91 234 234 234 234 918
92 928 92 237 236 236 236 928
93 938 93 239 239 239 239 938
94 948 94 242 242 242 241 948
95 958 95 244 244 244 244 958
96 968 96 247 247 247 247 968
97 978 97 249 249 249 249 978
98 988 98 252 252 252 252 988
99 999 99 255 255 255 255 999

*/

#endif //PLAYER_H
