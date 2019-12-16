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

#ifndef TEXT_H
#define TEXT_H

#define TEXT static const uint8_t PROGMEM
#define TEXT_POINTER static const uint8_t* const PROGMEM
#define MESSAGE_BUFFER_SIZE	255

static const uint8_t *huffmanNode = huffmanNodes;

uint8_t messageBuffer[MESSAGE_BUFFER_SIZE];
uint8_t bufferCursor = 0;
//bool useMessageBuffer = false;

uint8_t nextCharInCompressedMessage()
{
	uint8_t ch;
	do
	{
		huffmanNode += pgm_read_byte(huffmanNode + bitReaderRead8(1)) & 127;//globalBitReader/.read8(1)) & 127;
		ch = pgm_read_byte(huffmanNode);
	} while (ch & 128);
	huffmanNode = huffmanNodes;
	return ch;
}

void drawChar(uint8_t ch)
{
	if (ch == CHAR_NULL)
		return;
	else if (flags & FLAG_USE_MESSAGE_BUFFER)
	{
		messageBuffer[bufferCursor] = ch;
		++bufferCursor;
		return;
	}
	else if (ch == CHAR_LINE)
	{
		cursorX = lineStartX;
		cursorY += 8;
		return;
	}
	else if (ch == CHAR_NONE)
		arduboy.drawBitmap(cursorX, cursorY, font + (CHAR_NONE - 1) * 5, 5, 8, INVERT);
	else if (ch > 0)
		arduboy.drawBitmap(cursorX, cursorY, font + (ch - 1) * 5, 5, 8, INVERT);
	cursorX += 6;
}

void drawNumber(int16_t number)
{
	bool drawMinus = false;
	if (number == 0)
		drawChar(CHAR_0);
	if (number < 0)
	{
		drawMinus = true;
		number *= -1;
	}
	while (number > 0)
	{
		drawChar((number % 10) + CHAR_0);
		cursorX -= 12;
		number /= 10;
	}
	if (drawMinus)
		drawChar(CHAR_DASH);
}

void initMessageRead(const uint8_t *sentence, uint8_t &chars)
{
	bitReaderInit(sentence, 0);
	//globalBitReader/.init(sentence, 0);
	if (chars == 0)
		chars = bitReaderRead8(8);//globalBitReader/.read8(8);
}

void drawName(const uint8_t *name, uint8_t numChars)
{
	for (uint8_t i = 0; i < numChars; ++i)
		drawChar(pgm_read_byte(name + i));
}

void drawMessageCompressed(const uint8_t *sentence, uint8_t chars=0)
{
	uint8_t numChars = chars;
	initMessageRead(sentence, numChars);
	do
	{
		drawChar(nextCharInCompressedMessage());
		--numChars;
	} while(numChars > 0);
}

void fillMessageBuffer()
{
	for (uint8_t i = bufferCursor;i < MESSAGE_BUFFER_SIZE; ++i)
		messageBuffer[i] = CHAR_NULL;
	bufferCursor = 0;
	flags &= ~FLAG_USE_MESSAGE_BUFFER;
}

void drawSmallMessageBox()
{
	for (int16_t i = 896; i < 1024; ++i)
		arduboy.sBuffer[i] = 0x00;
}

void drawMessageBox()
{
	for (int16_t i = 768; i < 1024; ++i)
		arduboy.sBuffer[i] = 0x00;
}

//TEXT wordExp[] = {4, 0x15, 0x84, 0x00};
//TEXT wordGold[] = {4, 0x1C, 0xF3, 0x04};
//TEXT wordLevelUp[] = {8, 0x30, 0x55, 0x85, 0x30, 0x05, 0x50};
//TEXT wordAttacks[] = {7, 0x05, 0x45, 0x01, 0x0C, 0xB4, 0xFE};
//TEXT wordUses[] = {4, 0x55, 0x31, 0x53};
//TEXT wordCasts[] = {6, 0x0C, 0x14, 0xD4, 0x4C, 0x0F, 0xBE};
//TEXT wordFor[] = {4, 0x00, 0x63, 0xD2};
//TEXT wordDmg[] = {4, 0x00, 0x43, 0x47};
//TEXT wordOn[] = {4, 0x3C, 0xE0, 0x3E};
//TEXT wordYesNo[] = {6, 0x64, 0x54, 0xC0, 0x38, 0xFF, 0xBE};
//TEXT wordLevel[] = {7, 0x30, 0x55, 0x85, 0x30, 0x00, 0x3E};
//TEXT wordHeal[] = {4, 0x20, 0x50, 0x4C};

TEXT wordItems[] =
{
	//POTION
	CHAR_P, CHAR_O, CHAR_T, CHAR_I, CHAR_O, CHAR_N, CHAR_SPACE, CHAR_SPACE,
	//S-POTION
	CHAR_S, CHAR_DASH, CHAR_P, CHAR_O, CHAR_T, CHAR_I, CHAR_O, CHAR_N,
	//X-POTION
	CHAR_X, CHAR_DASH, CHAR_P, CHAR_O, CHAR_T, CHAR_I, CHAR_O, CHAR_N,
	//ETHER
	CHAR_E, CHAR_T, CHAR_H, CHAR_E, CHAR_R, CHAR_SPACE, CHAR_SPACE, CHAR_SPACE,
	//S-ETHER
	CHAR_S, CHAR_DASH, CHAR_E, CHAR_T, CHAR_H, CHAR_E, CHAR_R, CHAR_SPACE,
	//X-ETHER
	CHAR_X, CHAR_DASH, CHAR_E, CHAR_T, CHAR_H, CHAR_E, CHAR_R, CHAR_SPACE,
	//ELIXIR
	CHAR_E, CHAR_L, CHAR_I, CHAR_X, CHAR_I, CHAR_R, CHAR_SPACE, CHAR_SPACE,
	//X-ELIXIR
	CHAR_X, CHAR_DASH, CHAR_E, CHAR_L, CHAR_I, CHAR_X, CHAR_I, CHAR_R
};

TEXT wordSpells[] =
{
	//CURE
	CHAR_C, CHAR_U, CHAR_R, CHAR_E,
	//NUKE
	CHAR_N, CHAR_U, CHAR_K, CHAR_E,
	//FIRE
	CHAR_F, CHAR_I, CHAR_R, CHAR_E,
	//AQUA
	CHAR_A, CHAR_Q, CHAR_U, CHAR_A,
	//ROCK
	CHAR_R, CHAR_O, CHAR_C, CHAR_K,
	//AERO
	CHAR_A, CHAR_E, CHAR_R, CHAR_O,
	//DARK
	CHAR_D, CHAR_A, CHAR_R, CHAR_K,
	//HOLY
	CHAR_H, CHAR_O, CHAR_L, CHAR_Y
};

/*TEXT wordMapNames[] =
{
	CHAR_P, CHAR_A, CHAR_R, CHAR_A, CHAR_G, CHAR_O, CHAR_N, CHAR_NULL,
	CHAR_T, CHAR_A, CHAR_R, CHAR_T, CHAR_A, CHAR_R, CHAR_U, CHAR_S,
	CHAR_SPACE, CHAR_SPACE, CHAR_T, CHAR_O, CHAR_W, CHAR_N, CHAR_SPACE, CHAR_SPACE
};*/

TEXT_POINTER messageItemDescription[] =
{
	messageItemDescription0,
	messageItemDescription1,
	messageItemDescription2,
	messageItemDescription3,
	messageItemDescription4,
	messageItemDescription5,
	messageItemDescription6,
	messageItemDescription7
};

TEXT_POINTER messageStory[] =
{
	messagePrologue0,
	messagePrologue1,
	messagePrologue2,
	messagePrologue3,
	NULL,
	messageEpilogue0,
	messageEpilogue1,
	messageEpilogue2,
	messageEpilogue3
};

TEXT_POINTER messageEvents[] =
{
	messageCastle1,
	messageCastle2,
	messageCastle3,
	messageCastle4,
	messageCastle5,
	messageBookshelf1,
	messageBookshelf2,
	messageBookshelf3,
	messageBookshelf4,
	messageBookshelf5
};

TEXT_POINTER battleActionText[] =
{
	wordAttacks,
	messageBattleDefends,
	wordUses,
	wordCasts,
	messageBattleFocuses,
	messageBattleEscape
};

TEXT_POINTER messageLargeChestItem[] =
{
	messageSword,
	messageArmor,
	messageBoots,
	messageCharm
};

#endif //TEXT_H
