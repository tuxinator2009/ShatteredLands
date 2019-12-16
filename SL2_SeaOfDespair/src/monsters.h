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

#ifndef MONSTERS_H
#define MONSTERS_H

#define BOSS_HP						8
#define BOSS_MP						10
#define BOSS_STATS				11
#define BOSS_IMMUNITY			14
#define BOSS_SPELLS				16
#define BOSS_EXPGAIN			19
#define BOSS_GGAIN				20

#define MONSTER static const uint8_t PROGMEM
#define MONSTER_GROUP static const uint8_t PROGMEM
#define BOSS_BATTLE static const uint8_t PROGMEM

uint8_t getMonsterID(const uint8_t *group, uint8_t monster)
{
	uint8_t id = pgm_read_byte(group + (monster / 2));
	if (monster % 2 == 0)
		id >>= 4;
	return id & 15;
}

bool isBossBattle(const uint8_t *group)
{
	return (pgm_read_byte(group) & 0xF0) == 0xF0;
}

bool isMonsterGroupAvailable(const uint8_t *group, uint8_t info)
{
	uint8_t tiles = pgm_read_byte(group + 3);
	uint8_t monsterID;
	if (player.map == 0)
		return (tiles & (0x80 >> info));
	else
	{
		for (uint8_t i = 0; i < 6; ++i)
		{
			monsterID = getMonsterID(group, i);
			if (monsterID != 15 && (info & (0x80 >> monsterID)) == 0)
				return false;
		}
	}
	return true;
}

//1365 - 728 = 637 (15 monsters - 8 monsters = 637 bytes saved)
/* Monsters (24 bytes)
8 Bytes Name
2 Bytes
	-  6 bits MP
	- 10 bits HP
1 Byte Attack
1 Byte Defense
1 Byte Agility
1 Byte Magic
2 Bytes
	- 2 bits per spell (HOLY DARK AERO ROCK AQUA FIRE NUKE CURE)
		0 = Immune
		1 = 50% damage
		2 = 100% damage
		3 = 150% damage
3 Byte (CURE NUKE FIRE AQUA ROCK AERO DARK HOLY)
	- 3 bits per spell indicates spell level
1 Byte EXP Gain
1 Byte G Gain
*/

MONSTER monsters[] =
{
	//0: GOBLIN
	CHAR_G, CHAR_O, CHAR_B, CHAR_L, CHAR_I, CHAR_N, CHAR_NULL, CHAR_NULL,
	//mp/hp: 000000 0000000100
	0x00,0x04,
	3,1,3,0,
	//immunity: 10 10 10 10 01 11 10 10 (HOLY DARK AERO ROCK AQUA FIRE NUKE CURE)
	0xAA,0x7A,
	//spells: 000 000 000 000 000 000 000 000
	0x00,0x00,0x00,
	//EXP,G
	1,2,
	//1: FIREWOLF
	CHAR_F, CHAR_I, CHAR_R, CHAR_E, CHAR_W, CHAR_O, CHAR_L, CHAR_F,
	//mp/hp: 000001 0000001100
	0x04,0x0C,
	22,18,25,18,
	//immunity: 10 10 10 10 11 01 10 10 (HOLY DARK AERO ROCK AQUA FIRE NUKE CURE)
	0xAA,0xDA,
	//spells: 000 000 001 000 000 000 000 000
	0x00,0x80,0x00,
	//EXP,G
	2,3,
	//2: SCORPION
	CHAR_S, CHAR_C, CHAR_O, CHAR_R, CHAR_P, CHAR_I, CHAR_O, CHAR_N,
	//mp/hp: 000000 0000011110
	0x00,0x1E,
	42,56,50,20,
	//immunity: 10 10 01 11 10 10 10 10 (HOLY DARK AERO ROCK AQUA FIRE NUKE CURE)
	0xA7,0xAA,
	//spells: 000 000 000 000 000 000 000 000
	0x00,0x00,0x00,
	//EXP,G
	4,6,
	//3: MEGA BAT
	CHAR_M, CHAR_E, CHAR_G, CHAR_A, CHAR_SPACE, CHAR_B, CHAR_A, CHAR_T,
	//mp/hp: 000000 0000010010
	0x00,0x01,
	28,26,40,0,
	//immunity: 00 00 00 00 00 00 00 00 (HOLY DARK AERO ROCK AQUA FIRE NUKE CURE)
	0x00,0x00,
	//spells: 000 000 000 000 000 000 000 000
	0x00,0x00,0x00,
	//EXP,G
	2,3,
	//4: SKELETON
	CHAR_S, CHAR_K, CHAR_E, CHAR_L, CHAR_E, CHAR_T, CHAR_O, CHAR_N,
	//mp/hp: 000000 0000000110
	0x00,0x06,
	8, 6, 5, 3,
	//immunity: 10 10 10 10 10 11 10 10 (HOLY DARK AERO ROCK AQUA FIRE NUKE CURE)
	0xAA,0xBA,
	//spells: 000 000 000 000 000 000 000 000
	0x00,0x00,0x00,
	//EXP,G
	1,2,
	//5: SLIME
	CHAR_S, CHAR_L, CHAR_I, CHAR_M, CHAR_E, CHAR_NULL, CHAR_NULL, CHAR_NULL,
	//mp/hp: 000000 0000001111
	0x00,0x0F,
	24,22,20,9,
	//immunity: 10 10 10 01 11 10 10 10 (HOLY DARK AERO ROCK AQUA FIRE NUKE CURE)
	0xA9,0xEA,
	//spells: 000 000 000 000 000 000 000 000
	0x00,0x00,0x00,
	//EXP,G
	2,3,
	//6: WIZARD
	CHAR_W, CHAR_I, CHAR_Z, CHAR_A, CHAR_R, CHAR_D, CHAR_NULL, CHAR_NULL,
	//mp/hp: 001010 0000111100
	0x28,0x3C,
	30,0,60,60,
	//immunity: 00 00 00 00 00 00 00 00 (HOLY DARK AERO ROCK AQUA FIRE NUKE CURE)
	0x00,0x00,
	//spells: 010 000 001 001 001 001 000 000
	0x40,0x92,0x40,
	//EXP,G
	4,6,
	//7: GOLEM
	CHAR_G, CHAR_O, CHAR_L, CHAR_E, CHAR_M, CHAR_NULL, CHAR_NULL, CHAR_NULL,
	//mp/hp: 001010 0001010110
	0x28,0x56,
	71,76,1,46,
	//immunity: 10 10 01 00 11 01 10 10 (HOLY DARK AERO ROCK AQUA FIRE NUKE CURE)
	0xA4,0xDA,
	//spells: 000 000 000 000 010 000 000 000
	0x00,0x04,0x00,
	//EXP,G
	4,6
};

/* Bosses (23 bytes)
8 Bytes NAME
2 Bytes HP
1 Byte MP
1 Byte Attack
1 Byte Defense
1 Byte Agility
1 Byte Magic
2 Bytes (HOLY DARK AERO ROCK AQUA FIRE NUKE CURE)
	- 2 bits per spell
		0 = Immune
		1 = 50% damage
		2 = 100% damage
		3 = 150% damage
3 Byte (CURE NUKE FIRE AQUA ROCK AERO DARK HOLY)
	- 3 bit per spell indicates spell level
1 Byte EXP Gain
1 Byte G Gain
*/

MONSTER bosses[] =
{
	//Boss1 BIG BONE
	CHAR_B,CHAR_I,CHAR_G,CHAR_SPACE,CHAR_B,CHAR_O,CHAR_N,CHAR_E,
	0x00,0x2D,0x00,
	16,10,30,12,
	//10 10 10 10 10 11 10 10 (HOLY DARK AERO ROCK AQUA FIRE NUKE CURE)
	0xAA,0xBA,
	0x00,0x00,0x00,
	//EXP,G
	16,4,
	//Boss2 OOZE
	CHAR_O,CHAR_O,CHAR_Z,CHAR_E,CHAR_NULL,CHAR_NULL,CHAR_NULL,CHAR_NULL,
	0x00,0x3C,0x00,
	50,40,25,33,
	//10 10 10 10 11 01 10 10 (HOLY DARK AERO ROCK AQUA FIRE NUKE CURE)
	0xAA,0xDA,
	0x00,0x00,0x00,
	//EXP,G
	16,15,
	//Boss3 SORCERER
	CHAR_S,CHAR_O,CHAR_R,CHAR_C,CHAR_E,CHAR_R,CHAR_E,CHAR_R,
	0x01,0x60,0x1E,
	70,60,90,60,
	0x00,0x00,
	0x40,0x92,0x40,
	//EXP,G
	22,28,
	//Boss4 COLOSSUS
	CHAR_C,CHAR_O,CHAR_L,CHAR_O,CHAR_S,CHAR_S,CHAR_U,CHAR_S,
	0x01,0xF4,0x1E,
	83,94,2,52,
	//10 10 01 00 11 01 10 10 (HOLY DARK AERO ROCK AQUA FIRE NUKE CURE)
	0xA4,0xDA,
	//000 000 000 000 011 000 000 000
	0x00,0x06,0x00,
	//EXP,G
	32,38,
	//Boss5 DARKNESS
	CHAR_D,CHAR_A,CHAR_R,CHAR_K,CHAR_N,CHAR_E,CHAR_S,CHAR_S,
	0x0C,0x80,0xFF,
	126,106,255,102,
	//11 00 01 01 01 01 01 00 (HOLY DARK AERO ROCK AQUA FIRE NUKE CURE)
	0xC5,0x54,
	//011 000 000 000 000 000 001 000
	0x60,0x00,0x08,
	//EXP,G
	68,100
};

/* Monster Groups (5 bytes each)
3 Bytes
	- 4 bits monster1 ID (If boss battle this ID will be 15)
	- 4 bits monster2 ID
	- 4 bits monster3 ID
	- 4 bits monster4 ID
	- 4 bits monster5 ID
	- 4 bits monster6 ID
1 Byte tiles found on (only used if tileset=0)
*/

MONSTER_GROUP monsterGroups[] =
{
	//1 GOBLIN //01000000
	0x0F,0xFF,0xFF,0x40,
	//2 GOBLINS
	0x00,0xFF,0xFF,0x40,
	//3 GOBLINS
	0x00,0x0F,0xFF,0x20,
	//1 WOLF
	0x1F,0xFF,0xFF,0x20,
	//2 WOLF
	0x11,0xFF,0xFF,0x20,
	//1 WOLF and 1 GOBLIN
	0x01,0xFF,0xFF,0x20,
	//1 WOLF and 2 GOBLIN
	0x00,0x1F,0xFF,0x20,
	//2 SCORPION
	0x22,0xFF,0xFF,0x08,
	//3 SCORPION
	0x22,0x2F,0xFF,0x08,
	//4 SCORPION
	0x22,0x22,0xFF,0x08,
	//2 MEGABAT
	0x33,0xFF,0xFF,0x00,
	//4 MEGABAT
	0x33,0x33,0xFF,0x00,
	//6 MEGABAT
	0x33,0x33,0x33,0x00,
	//Tower 1
	//2 SKELETON and 2 GOBLIN
	0x44,0x00,0xFF,0x00,
	//4 SKELETON and 1 GOBLIN
	0x44,0x44,0x0F,0x00,
	//6 SKELETON
	0x44,0x44,0x44,0x00,
	//Tower 2
	//2 SLIME and 2 FIREWOLF
	0x55,0x11,0xFF,0x00,
	//3 SLIME and 1 FIREWOLF
	0x55,0x51,0xFF,0x00,
	//4 SLIME
	0x55,0x55,0xFF,0x00,
	//Tower 3
	//1 WIZARD
	0x6F,0xFF,0xFF,0x00,
	//2 WIZARD
	0x66,0xFF,0xFF,0x00,
	//3 WIZARD
	0x66,0x6F,0xFF,0x00,
	//Tower 4
	//2 GOLEM
	0x77,0xFF,0xFF,0x00,
	//3 GOLEM
	0x77,0x7F,0xFF,0x00,
	//4 GOLEM
	0x77,0x77,0xFF,0x00,
	//Tower 5
	//2 WIZARD and 2 GOLEM
	0x66,0x77,0xFF,0x00,
	//2 WIZARD and 4 GOLEM
	0x66,0x77,0x77,0x00,
	//3 WIZARD and 3 GOLEM
	0x66,0x67,0x77,0x00,
	//4 WIZARD and 2 GOLEM
	0x66,0x66,0x77,0x00,
	//5 WIZARD and 1 GOLEM
	0x66,0x66,0x67,0x00
};

MONSTER_GROUP bossGroups[] =
{
	//0x00 Boss 1 + 2 Skeleton
	0xF0,0x44,
	//0x03 Boss 2 + 2 Slime
	0xF1,0x55,
	//0x06 Boss 3 + 2 Wizards
	0xF2,0x66,
	//0x09 Boss 4 + 2 Golems
	0xF3,0x77,
	//0x0C Boss 5 and nothing else
	0xF4,0xFF
};

#endif //MONSTERS_H
