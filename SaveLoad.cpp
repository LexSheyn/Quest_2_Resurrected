#include "stdafx.h"
#include "SaveLoad.h"

#pragma warning(disable:4996)

//Private functions
void SaveLoad::initVariables()
{
	//Console variables initialization
	this->nScreenWidth = 120;
	this->nScreenHeight = 30;
	this->dwBytesWritten = 0;
	this->menuCursorSize = 13;
	this->saveLoadCursorSize = 26;
	this->distanceX = 2;
	this->coordStartX = 10;
	this->menuCursorX = this->coordStartX + 12 + this->distanceX;
	this->coordStartY = 8;
	this->menuCursorY = this->coordStartY;
	this->menuCursorAttributes = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY;
	this->dwNumWritten = 0;

	//Setting default language
	this->language = L"EN";

	//Creating menu text
	if (this->language == L"EN")
	{
		this->menuText[0] = L"Play";
		this->menuText[1] = L"Continue";
		this->menuText[2] = L"Save";
		this->menuText[3] = L"Load";
		this->menuText[4] = L"Settings";
		this->menuText[5] = L"Exit";

		this->emptyText = L"| Empty | ";
	}
	else if (this->language == L"RU")
	{
		this->menuText[0] = L"Играть";
		this->menuText[1] = L"Продолжить";
		this->menuText[2] = L"Сохранить";
		this->menuText[3] = L"Загрузить";
		this->menuText[4] = L"Настройки";
		this->menuText[5] = L"Выйти";

		this->emptyText = L"| Пусто | ";
	}

	//Setting menu text X
	this->textX[this->menuText[0]] = this->coordStartX + 1;
	this->textX[this->menuText[1]] = this->coordStartX + 1;
	this->textX[this->menuText[2]] = this->coordStartX + 1;
	this->textX[this->menuText[3]] = this->coordStartX + 1;
	this->textX[this->menuText[4]] = this->coordStartX + 1;
	this->textX[this->menuText[5]] = this->coordStartX + 1;

	//Setting menu text Y
	this->textY[this->menuText[0]] = this->coordStartY;
	this->textY[this->menuText[1]] = this->coordStartY + 2;
	this->textY[this->menuText[2]] = this->coordStartY + 4;
	this->textY[this->menuText[3]] = this->coordStartY + 6;
	this->textY[this->menuText[4]] = this->coordStartY + 8;
	this->textY[this->menuText[5]] = this->coordStartY + 10;

	//Creating save/load text
	for (unsigned i = 0; i < 4; i++)
	{
		this->saveLoadText[i] = std::to_wstring(i + 1) + L" | xx-xx-xxxx xx:xx:xx";
	}

	//Setting save/load text X
	this->textX[this->saveLoadText[0]] = this->coordStartX + this->menuCursorSize + this->distanceX;
	this->textX[this->saveLoadText[1]] = this->coordStartX + this->menuCursorSize + this->distanceX;
	this->textX[this->saveLoadText[2]] = this->coordStartX + this->menuCursorSize + this->distanceX;
	this->textX[this->saveLoadText[3]] = this->coordStartX + this->menuCursorSize + this->distanceX;

	//Setting save/load text Y
	this->textY[this->saveLoadText[0]] = this->coordStartY;
	this->textY[this->saveLoadText[1]] = this->coordStartY + 2;
	this->textY[this->saveLoadText[2]] = this->coordStartY + 4;
	this->textY[this->saveLoadText[3]] = this->coordStartY + 6;

	//Setting mode, there is 2 modes: "save" and "load"
	this->mode = L"NONE";
}

void SaveLoad::initConsoleScreenBuffer()
{
	//Creating chars buffer
	this->screen = new wchar_t[this->nScreenWidth * this->nScreenHeight];

	//Filling chars buffer with spaces
	std::fill_n(this->screen, this->nScreenHeight * this->nScreenWidth, ' ');

	//Creating screen buffer
	this->screenBuffer = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

	//Get the screen buffer information including size and position of window
	GetConsoleScreenBufferInfo(this->screenBuffer, &this->csbiScreenInfo);

	//Getting console cursor info to set cursor hidden
	GetConsoleCursorInfo(this->screenBuffer, &this->cursorInfo);
	this->cursorInfo.bVisible = FALSE;

	//Setting screen buffer cursor position, coordinates must be within the boundaries of the console screen buffer
	SetConsoleCursorPosition(this->screenBuffer, { short(this->nScreenWidth - 1), short(this->nScreenHeight - 1) });

	//Disabled attribute PLAY
	FillConsoleOutputAttribute(this->screenBuffer, 8, this->menuCursorSize, { short(this->textX[this->menuText[0]] - 1), this->textY[this->menuText[0]] }, &this->dwNumWritten);

	//Disabled attribute CONTINUE
	FillConsoleOutputAttribute(this->screenBuffer, 8, this->menuCursorSize, { short(this->textX[this->menuText[1]] - 1), this->textY[this->menuText[1]] }, &this->dwNumWritten);

	//SAVE and LOAD attributes
	if (this->mode == L"save")
	{
		//Active attribute SAVE
		FillConsoleOutputAttribute(this->screenBuffer, 15, this->menuCursorSize, { short(this->textX[this->menuText[2]] - 1), this->textY[this->menuText[2]] }, &this->dwNumWritten);
		
		//Disabled attribute LOAD
		FillConsoleOutputAttribute(this->screenBuffer, 8, this->menuCursorSize, { short(this->textX[this->menuText[3]] - 1), this->textY[this->menuText[3]] }, &this->dwNumWritten);
	}
	else if (this->mode == L"load")
	{
		//Disabled attribute SAVE
		FillConsoleOutputAttribute(this->screenBuffer, 8, this->menuCursorSize, { short(this->textX[this->menuText[2]] - 1), this->textY[this->menuText[2]] }, &this->dwNumWritten);
		
		//Active attribute LOAD
		FillConsoleOutputAttribute(this->screenBuffer, 15, this->menuCursorSize, { short(this->textX[this->menuText[3]] - 1), this->textY[this->menuText[3]] }, &this->dwNumWritten);
	}
	else
	{
		//Disabled attribute SAVE
		FillConsoleOutputAttribute(this->screenBuffer, 8, this->menuCursorSize, { short(this->textX[this->menuText[2]] - 1), this->textY[this->menuText[2]] }, &this->dwNumWritten);

		//Disabled attribute LOAD
		FillConsoleOutputAttribute(this->screenBuffer, 8, this->menuCursorSize, { short(this->textX[this->menuText[3]] - 1), this->textY[this->menuText[3]] }, &this->dwNumWritten);
	}

	//Disabled attribute SETTINGS
	FillConsoleOutputAttribute(this->screenBuffer, 8, this->menuCursorSize, { short(this->textX[this->menuText[4]] - 1), this->textY[this->menuText[4]] }, &this->dwNumWritten);

	//Disabled attribute EXIT
	FillConsoleOutputAttribute(this->screenBuffer, 8, this->menuCursorSize, { short(this->textX[this->menuText[5]] - 1), this->textY[this->menuText[5]] }, &this->dwNumWritten);
	
	//Default save/load attribute
	FillConsoleOutputAttribute(this->screenBuffer, 7, this->saveLoadCursorSize, { short(this->coordStartX + this->menuCursorSize + this->distanceX - 1), this->coordStartY }, &this->dwNumWritten);
	FillConsoleOutputAttribute(this->screenBuffer, 7, this->saveLoadCursorSize, { short(this->coordStartX + this->menuCursorSize + this->distanceX - 1), short(this->coordStartY + 2) }, &this->dwNumWritten);
	FillConsoleOutputAttribute(this->screenBuffer, 7, this->saveLoadCursorSize, { short(this->coordStartX + this->menuCursorSize + this->distanceX - 1), short(this->coordStartY + 4) }, &this->dwNumWritten);
	FillConsoleOutputAttribute(this->screenBuffer, 7, this->saveLoadCursorSize, { short(this->coordStartX + this->menuCursorSize + this->distanceX - 1), short(this->coordStartY + 6) }, &this->dwNumWritten);

	//Creating cursor (attribute for cursor)
	FillConsoleOutputAttribute(this->screenBuffer, this->menuCursorAttributes, this->saveLoadCursorSize, { this->menuCursorX, this->menuCursorY }, &this->dwNumWritten);
}

void SaveLoad::write(wchar_t* buffer, short x, short y, wchar_t ch)
{
	buffer[y * this->nScreenWidth + x] = ch;
}

void SaveLoad::write(wchar_t* buffer, short x, short y, const std::wstring& s)
{
	this->textX[s] = x;
	this->textY[s] = y;
	swprintf(&buffer[y * this->nScreenWidth + x], s.size() + 1, L"%s", s.c_str());
}

//Constructors and Distructors
SaveLoad::SaveLoad()
{
	this->initVariables();
	this->initConsoleScreenBuffer();
}

SaveLoad::~SaveLoad()
{

}

//Accessors
HANDLE& SaveLoad::getHandle()
{
	return this->screenBuffer;
}

bool& SaveLoad::isDisabled(std::wstring menu_text)
{
	return this->disabled[menu_text];
}

const short& SaveLoad::getCursorX() const
{
	return this->menuCursorX;
}

const short& SaveLoad::getCursorY() const
{
	return this->menuCursorY;
}

const DWORD SaveLoad::getCursorSize() const
{
	return this->menuCursorSize;
}

const DWORD SaveLoad::getCursorAttributes() const
{
	return this->menuCursorAttributes;
}

short& SaveLoad::getTextX(std::wstring menu_text)
{
	return this->textX[menu_text];
}

short& SaveLoad::getTextY(std::wstring menu_text)
{
	return this->textY[menu_text];
}

std::wstring& SaveLoad::getMenuText(unsigned text_number)
{
	return this->menuText[text_number];
}

std::wstring& SaveLoad::getSaveLoadText(int text_number)
{
	return this->saveLoadText[text_number];
}

std::wstring& SaveLoad::getMode()
{
	return this->mode;
}

//Modifiers
void SaveLoad::setDisabled(const std::wstring menu_text, const bool set_disabled)
{
	this->disabled[menu_text] = set_disabled;
}

void SaveLoad::setCursorX(const short x)
{
	this->menuCursorX = x;
}

void SaveLoad::setCursorY(const short y)
{
	this->menuCursorY = y;
}

void SaveLoad::setLanguage(const std::wstring language_name)
{
	this->language = language_name;
}

void SaveLoad::setFontHeight(unsigned font_height)
{
	this->cfi.cbSize = sizeof(cfi);
	this->cfi.nFont = 0;
	this->cfi.dwFontSize.X = 0; // Width of each character in the font
	this->cfi.dwFontSize.Y = font_height; // Height
	this->cfi.FontFamily = FF_DONTCARE;
	this->cfi.FontWeight = FW_NORMAL;
	wcscpy_s(cfi.FaceName, L"Consolas"); // Choose your font
	SetCurrentConsoleFontEx(this->screenBuffer, FALSE, &this->cfi);
}

void SaveLoad::setMode(const std::wstring mode_name)
{
	this->mode = mode_name;
}

//Functions
void SaveLoad::clearScreen()
{
	//Filling chars buffer with spaces
	std::fill_n(this->screen, this->nScreenWidth * this->nScreenHeight, ' ');
}

std::wstring SaveLoad::ASCII_to_UTF16(const std::string& str)
{
	if (str.empty())
	{
		return std::wstring();
	}

	size_t charsNeeded = ::MultiByteToWideChar(CP_ACP, 0, str.data(), (int)str.size(), NULL, 0);

	if (charsNeeded == 0)
	{
		throw std::runtime_error("Failed converting ASCII string to UTF-16");
	}

	std::vector<wchar_t> buffer(charsNeeded);

	int charsConverted = ::MultiByteToWideChar(CP_ACP, 0, str.data(), (int)str.size(), &buffer[0], buffer.size());

	if (charsConverted == 0)
	{
		throw std::runtime_error("Failed converting ASCII string to UTF-16");
	}

	return std::wstring(&buffer[0], charsConverted);
}

std::wstring SaveLoad::returnCurrentTimeAndDate()
{
	time_t rawtime;
	struct tm* timeinfo;
	char buffer[80];

	time(&rawtime);
	timeinfo = localtime(&rawtime);

	strftime(buffer, sizeof(buffer), "%d-%m-%Y | %H:%M:%S", timeinfo);
	std::string str(buffer);

	return this->ASCII_to_UTF16(str);
}

void SaveLoad::fillMenuOutputAttribute(int attribute_number, int length, short x, short y)
{
	FillConsoleOutputAttribute(this->screenBuffer, attribute_number, length, { x, y }, &this->dwNumWritten);
}

void SaveLoad::updateLanguage()
{
	//Creating menu text
	if (this->language == L"EN")
	{
		if (this->disabled[this->menuText[1]] == true)
		{
			this->menuText[0] = L"Play";
		}
		else
		{
			this->menuText[0] = L"Restart";
		}
		this->menuText[1] = L"Continue";
		this->menuText[2] = L"Save";
		this->menuText[3] = L"Load";
		this->menuText[4] = L"Settings";
		this->menuText[5] = L"Exit";

		this->emptyText = L"| Empty | ";
	}
	else if (this->language == L"RU")
	{
		if (this->disabled[this->menuText[1]] == true)
		{
			this->menuText[0] = L"Играть";
		}
		else
		{
			this->menuText[0] = L"Заново";
		}
		this->menuText[1] = L"Продолжить";
		this->menuText[2] = L"Сохранить";
		this->menuText[3] = L"Загрузить";
		this->menuText[4] = L"Настройки";
		this->menuText[5] = L"Выйти";

		this->emptyText = L"| Пусто | ";
	}

	//Setting menu text X
	this->textX[this->menuText[0]] = this->coordStartX + 1;
	this->textX[this->menuText[1]] = this->coordStartX + 1;
	this->textX[this->menuText[2]] = this->coordStartX + 1;
	this->textX[this->menuText[3]] = this->coordStartX + 1;
	this->textX[this->menuText[4]] = this->coordStartX + 1;
	this->textX[this->menuText[5]] = this->coordStartX + 1;

	//Setting menu text Y
	this->textY[this->menuText[0]] = this->coordStartY;
	this->textY[this->menuText[1]] = this->coordStartY + 2;
	this->textY[this->menuText[2]] = this->coordStartY + 4;
	this->textY[this->menuText[3]] = this->coordStartY + 6;
	this->textY[this->menuText[4]] = this->coordStartY + 8;
	this->textY[this->menuText[5]] = this->coordStartY + 10;

	//Setting save/load text X
	this->textX[this->saveLoadText[0]] = this->coordStartX + this->menuCursorSize + this->distanceX;
	this->textX[this->saveLoadText[1]] = this->coordStartX + this->menuCursorSize + this->distanceX;
	this->textX[this->saveLoadText[2]] = this->coordStartX + this->menuCursorSize + this->distanceX;
	this->textX[this->saveLoadText[3]] = this->coordStartX + this->menuCursorSize + this->distanceX;

	//Setting save/load text Y
	this->textY[this->saveLoadText[0]] = this->coordStartY;
	this->textY[this->saveLoadText[1]] = this->coordStartY + 2;
	this->textY[this->saveLoadText[2]] = this->coordStartY + 4;
	this->textY[this->saveLoadText[3]] = this->coordStartY + 6;
}

void SaveLoad::update()
{
	//Setting screen buffer cursor position, coordinates must be within the boundaries of the console screen buffer
	SetConsoleCursorPosition(this->screenBuffer, { short(this->nScreenWidth - 1), short(this->nScreenHeight - 1) });

	//Updating language
	this->updateLanguage();

	if (this->menuCursorY < this->coordStartY)
	{
		this->menuCursorY = (this->coordStartY + 6);
	}
	else if (this->menuCursorY > (this->coordStartY + 6))
	{
		this->menuCursorY = this->coordStartY;
	}

	//Disabled attribute PLAY
	FillConsoleOutputAttribute(this->screenBuffer, 8, this->menuCursorSize, { short(this->textX[this->menuText[0]] - 1), this->textY[this->menuText[0]] }, &this->dwNumWritten);

	//Disabled attribute CONTINUE
	FillConsoleOutputAttribute(this->screenBuffer, 8, this->menuCursorSize, { short(this->textX[this->menuText[1]] - 1), this->textY[this->menuText[1]] }, &this->dwNumWritten);

	//SAVE and LOAD attributes
	if (this->mode == L"save")
	{
		//Active attribute SAVE
		FillConsoleOutputAttribute(this->screenBuffer, 15, this->menuCursorSize, { short(this->textX[this->menuText[2]] - 1), this->textY[this->menuText[2]] }, &this->dwNumWritten);

		//Disabled attribute LOAD
		FillConsoleOutputAttribute(this->screenBuffer, 8, this->menuCursorSize, { short(this->textX[this->menuText[3]] - 1), this->textY[this->menuText[3]] }, &this->dwNumWritten);
	}
	else if (this->mode == L"load")
	{
		//Disabled attribute SAVE
		FillConsoleOutputAttribute(this->screenBuffer, 8, this->menuCursorSize, { short(this->textX[this->menuText[2]] - 1), this->textY[this->menuText[2]] }, &this->dwNumWritten);

		//Active attribute LOAD
		FillConsoleOutputAttribute(this->screenBuffer, 15, this->menuCursorSize, { short(this->textX[this->menuText[3]] - 1), this->textY[this->menuText[3]] }, &this->dwNumWritten);
	}
	else
	{
		//Disabled attribute SAVE
		FillConsoleOutputAttribute(this->screenBuffer, 8, this->menuCursorSize, { short(this->textX[this->menuText[2]] - 1), this->textY[this->menuText[2]] }, &this->dwNumWritten);

		//Disabled attribute LOAD
		FillConsoleOutputAttribute(this->screenBuffer, 8, this->menuCursorSize, { short(this->textX[this->menuText[3]] - 1), this->textY[this->menuText[3]] }, &this->dwNumWritten);
	}

	//Disabled attribute SETTINGS
	FillConsoleOutputAttribute(this->screenBuffer, 8, this->menuCursorSize, { short(this->textX[this->menuText[4]] - 1), this->textY[this->menuText[4]] }, &this->dwNumWritten);

	//Disabled attribute EXIT
	FillConsoleOutputAttribute(this->screenBuffer, 8, this->menuCursorSize, { short(this->textX[this->menuText[5]] - 1), this->textY[this->menuText[5]] }, &this->dwNumWritten);

	//Default save/load attribute
	FillConsoleOutputAttribute(this->screenBuffer, 7, this->saveLoadCursorSize, { short(this->coordStartX + this->menuCursorSize + this->distanceX - 1), this->coordStartY }, &this->dwNumWritten);
	FillConsoleOutputAttribute(this->screenBuffer, 7, this->saveLoadCursorSize, { short(this->coordStartX + this->menuCursorSize + this->distanceX - 1), short(this->coordStartY + 2) }, &this->dwNumWritten);
	FillConsoleOutputAttribute(this->screenBuffer, 7, this->saveLoadCursorSize, { short(this->coordStartX + this->menuCursorSize + this->distanceX - 1), short(this->coordStartY + 4) }, &this->dwNumWritten);
	FillConsoleOutputAttribute(this->screenBuffer, 7, this->saveLoadCursorSize, { short(this->coordStartX + this->menuCursorSize + this->distanceX - 1), short(this->coordStartY + 6) }, &this->dwNumWritten);

	//Creating cursor (attribute for cursor)
	FillConsoleOutputAttribute(this->screenBuffer, this->menuCursorAttributes, this->saveLoadCursorSize, { this->menuCursorX, this->menuCursorY }, &this->dwNumWritten);
}

void SaveLoad::render()
{
	//Setting screen buffer to be active
	SetConsoleActiveScreenBuffer(this->screenBuffer);

	//Clearing screen buffer
	this->clearScreen();

	//Hiding console cursor
	SetConsoleCursorInfo(this->screenBuffer, &this->cursorInfo);

	//Writing menu text to chars buffer
	for (unsigned i = 0; i <= 5; i++)
	{
		write(this->screen, this->coordStartX + 1, this->coordStartY + i * 2, this->menuText[i]);
	}

	//Writing settings text to chars buffer	
	write(this->screen, this->coordStartX + this->menuCursorSize + this->distanceX, this->coordStartY, this->saveLoadText[0]);
	write(this->screen, this->coordStartX + this->menuCursorSize + this->distanceX, this->coordStartY, L"1 |");
	
	write(this->screen, this->coordStartX + this->menuCursorSize + this->distanceX, this->coordStartY + 2, this->saveLoadText[1]);
	write(this->screen, this->coordStartX + this->menuCursorSize + this->distanceX, this->coordStartY + 2, L"2 |");
	
	write(this->screen, this->coordStartX + this->menuCursorSize + this->distanceX, this->coordStartY + 4, this->saveLoadText[2]);
	write(this->screen, this->coordStartX + this->menuCursorSize + this->distanceX, this->coordStartY + 4, L"3 |");
	
	write(this->screen, this->coordStartX + this->menuCursorSize + this->distanceX, this->coordStartY + 6, this->saveLoadText[3]);
	write(this->screen, this->coordStartX + this->menuCursorSize + this->distanceX, this->coordStartY + 6, L"4 |");

	//Output chars buffer to the screen buffer
	WriteConsoleOutputCharacterW(this->screenBuffer, this->screen, this->nScreenWidth * this->nScreenHeight, { 0,0 }, &this->dwBytesWritten);
}