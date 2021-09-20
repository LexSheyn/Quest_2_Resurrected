#include "stdafx.h"
#include "Menu.h"

#pragma warning(disable:4996)

//Private functions
void Menu::initVariables()
{
	//Console variables initialization
	this->nScreenWidth = 120;
	this->nScreenHeight = 30;
	this->dwBytesWritten = 0;
	this->coordStartX = 10;
	this->menuCursorX = this->coordStartX;
	this->coordStartY = 8;
	this->menuCursorY = this->coordStartY;
	this->menuCursorAttributes = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY;
	this->menuCursorSize = 12;
	this->dwNumWritten = 0;
	this->cDefaultFontHeight = this->cfi.dwFontSize.Y;

	//Setting default language
	this->language = L"EN";

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

		this->tipText[0] = L"[W][S][A][D] - navigate cursor";
		this->tipText[1] = L"[I] - inventory";
		this->tipText[2] = L"[Enter] - choose";
		this->tipText[3] = L"[Space] - skip";
		this->tipText[4] = L"[Escape] - back to main menu";
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

		this->tipText[0] = L"[W][S][A][D] - направить курсор";
		this->tipText[1] = L"[I] - инвентарь";
		this->tipText[2] = L"[Enter] - выбрать";
		this->tipText[3] = L"[Space] - пропустить";
		this->tipText[4] = L"[Escape] - вернуться в главное меню";
	}

	//Setting menu text X
	this->textX[this->menuText[0]] = this->coordStartX + 1;
	this->textX[this->menuText[1]] = this->coordStartX + 1;
	this->textX[this->menuText[2]] = this->coordStartX + 1;
	this->textX[this->menuText[3]] = this->coordStartX + 1;
	this->textX[this->menuText[4]] = this->coordStartX + 1;
	this->textX[this->menuText[5]] = this->coordStartX + 1;

	this->textX[this->tipText[0]] = this->coordStartX + 30;
	this->textX[this->tipText[1]] = this->coordStartX + 30;
	this->textX[this->tipText[2]] = this->coordStartX + 30;
	this->textX[this->tipText[3]] = this->coordStartX + 30;
	this->textX[this->tipText[4]] = this->coordStartX + 30;
	
	//Setting menu text Y
	this->textY[this->menuText[0]] = this->coordStartY;
	this->textY[this->menuText[1]] = this->coordStartY + 2;
	this->textY[this->menuText[2]] = this->coordStartY + 4;
	this->textY[this->menuText[3]] = this->coordStartY + 6;
	this->textY[this->menuText[4]] = this->coordStartY + 8;
	this->textY[this->menuText[5]] = this->coordStartY + 10;

	this->textY[this->tipText[0]] = this->coordStartY;
	this->textY[this->tipText[1]] = this->coordStartY + 2;
	this->textY[this->tipText[2]] = this->coordStartY + 4;
	this->textY[this->tipText[3]] = this->coordStartY + 6;
	this->textY[this->tipText[4]] = this->coordStartY + 8;
}

void Menu::initConsoleScreenBuffer()
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

	//Default attribute PLAY
	FillConsoleOutputAttribute(this->screenBuffer, 7, this->menuCursorSize, { short(this->textX[this->menuText[0]] - 1), this->textY[this->menuText[0]] }, &this->dwNumWritten);

	if (disabled[this->menuText[1]])
	{
		//Disabled attribute CONTINUE
		FillConsoleOutputAttribute(this->screenBuffer, 8, this->menuCursorSize, { short(this->textX[this->menuText[1]] - 1), this->textY[this->menuText[1]] }, &this->dwNumWritten);
	}
	else
	{
		//Default attribute CONTINUE
		FillConsoleOutputAttribute(this->screenBuffer, 7, this->menuCursorSize, { short(this->textX[this->menuText[1]] - 1), this->textY[this->menuText[1]] }, &this->dwNumWritten);
	}

	if (disabled[this->menuText[2]])
	{
		//Disabled attribute SAVE
		FillConsoleOutputAttribute(this->screenBuffer, 8, this->menuCursorSize, { short(this->textX[this->menuText[2]] - 1), this->textY[this->menuText[2]] }, &this->dwNumWritten);
	}
	else
	{
		//Default attribute SAVE
		FillConsoleOutputAttribute(this->screenBuffer, 7, this->menuCursorSize, { short(this->textX[this->menuText[2]] - 1), this->textY[this->menuText[2]] }, &this->dwNumWritten);
	}

	if (disabled[this->menuText[3]])
	{
		//Disabled attribute LOAD
		FillConsoleOutputAttribute(this->screenBuffer, 8, this->menuCursorSize, { short(this->textX[this->menuText[3]] - 1), this->textY[this->menuText[3]] }, &this->dwNumWritten);
	}
	else
	{
		//Default attribute LOAD
		FillConsoleOutputAttribute(this->screenBuffer, 7, this->menuCursorSize, { short(this->textX[this->menuText[3]] - 1), this->textY[this->menuText[3]] }, &this->dwNumWritten);
	}

	//Default attribute SETTINGS
	FillConsoleOutputAttribute(this->screenBuffer, 7, this->menuCursorSize, { short(this->textX[this->menuText[4]] - 1), this->textY[this->menuText[4]] }, &this->dwNumWritten);

	//Default attribute EXIT
	FillConsoleOutputAttribute(this->screenBuffer, 7, this->menuCursorSize, { short(this->textX[this->menuText[5]] - 1), this->textY[this->menuText[5]] }, &this->dwNumWritten);
	
	//Tips attribute
	FillConsoleOutputAttribute(this->screenBuffer, 8, 50, { this->textX[this->tipText[0]], this->textY[this->tipText[0]] }, &this->dwNumWritten);
	FillConsoleOutputAttribute(this->screenBuffer, 8, 50, { this->textX[this->tipText[1]], this->textY[this->tipText[1]] }, &this->dwNumWritten);
	FillConsoleOutputAttribute(this->screenBuffer, 8, 50, { this->textX[this->tipText[2]], this->textY[this->tipText[2]] }, &this->dwNumWritten);
	FillConsoleOutputAttribute(this->screenBuffer, 8, 50, { this->textX[this->tipText[3]], this->textY[this->tipText[3]] }, &this->dwNumWritten);
	FillConsoleOutputAttribute(this->screenBuffer, 8, 50, { this->textX[this->tipText[4]], this->textY[this->tipText[4]] }, &this->dwNumWritten);

	//Creating cursor (attribute for cursor)
	FillConsoleOutputAttribute(this->screenBuffer, this->menuCursorAttributes, this->menuCursorSize, { this->menuCursorX, this->menuCursorY }, &this->dwNumWritten);
}

void Menu::write(wchar_t* buffer, short x, short y, wchar_t ch)
{
	buffer[y * this->nScreenWidth + x] = ch;
}

void Menu::write(wchar_t* buffer, short x, short y, const std::wstring& s)
{
	this->textX[s] = x;
	this->textY[s] = y;
	swprintf(&buffer[y * this->nScreenWidth + x], s.size() + 1, L"%s", s.c_str());
}

//Constructors and Distructors
Menu::Menu()
{
	this->initVariables();
	this->initConsoleScreenBuffer();
}

Menu::~Menu()
{

}

//Accessors
HANDLE& Menu::getHandle()
{
	return this->screenBuffer;
}

bool& Menu::isDisabled(std::wstring menu_text)
{
	return this->disabled[menu_text];
}

const short& Menu::getCursorX() const
{
	return this->menuCursorX;
}

const short& Menu::getCursorY() const
{
	return this->menuCursorY;
}

const DWORD Menu::getCursorSize() const
{
	return this->menuCursorSize;
}

const DWORD Menu::getCursorAttributes() const
{
	return this->menuCursorAttributes;
}

short& Menu::getTextX(std::wstring menu_text)
{
	return this->textX[menu_text];
}

short& Menu::getTextY(std::wstring menu_text)
{
	return this->textY[menu_text];
}

std::wstring& Menu::getMenuText(unsigned text_number)
{
	return this->menuText[text_number];
}

const unsigned& Menu::getDefaultFont() const
{
	return this->cDefaultFontHeight;
}

//Modifiers
void Menu::setDisabled(const std::wstring menu_text, const bool set_disabled)
{
	this->disabled[menu_text] = set_disabled;
}

void Menu::setCursorX(const short x)
{
	this->menuCursorX = x;
}

void Menu::setCursorY(const short y)
{
	this->menuCursorY = y;
}

void Menu::setLanguage(const std::wstring language_name)
{
	this->language = language_name;
}

void Menu::setFontHeight(unsigned font_height)
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

//Functions
void Menu::clearScreen()
{
	//Filling chars buffer with spaces
	std::fill_n(this->screen, this->nScreenWidth * this->nScreenHeight, ' ');
	//FillConsoleOutputAttribute(this->screenBuffer, 7, this->nScreenWidth * this->nScreenHeight, { 0, 0 }, &this->dwNumWritten);
}

std::wstring Menu::ASCII_to_UTF16(const std::string& str)
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

std::wstring Menu::returnCurrentTimeAndDate()
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

void Menu::fillMenuOutputAttribute(int attribute_number, int length, short x, short y)
{
	FillConsoleOutputAttribute(this->screenBuffer, attribute_number, length, { x, y }, &this->dwNumWritten);
}

void Menu::updateLanguage()
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

		this->tipText[0] = L"[W][S][A][D] - navigate cursor";
		this->tipText[1] = L"[I] - inventory";
		this->tipText[2] = L"[Enter] - choose";
		this->tipText[3] = L"[Space] - skip";
		this->tipText[4] = L"[Escape] - back to main menu";
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

		this->tipText[0] = L"[W][S][A][D] - направить курсор";
		this->tipText[1] = L"[I] - инвентарь";
		this->tipText[2] = L"[Enter] - выбрать";
		this->tipText[3] = L"[Space] - пропустить";
		this->tipText[4] = L"[Escape] - вернуться в главное меню";
	}

	//Setting menu text X
	this->textX[this->menuText[0]] = this->coordStartX + 1;
	this->textX[this->menuText[1]] = this->coordStartX + 1;
	this->textX[this->menuText[2]] = this->coordStartX + 1;
	this->textX[this->menuText[3]] = this->coordStartX + 1;
	this->textX[this->menuText[4]] = this->coordStartX + 1;
	this->textX[this->menuText[5]] = this->coordStartX + 1;

	this->textX[this->tipText[0]] = this->coordStartX + 30;
	this->textX[this->tipText[1]] = this->coordStartX + 30;
	this->textX[this->tipText[2]] = this->coordStartX + 30;
	this->textX[this->tipText[3]] = this->coordStartX + 30;
	this->textX[this->tipText[4]] = this->coordStartX + 30;

	//Setting menu text Y
	this->textY[this->menuText[0]] = this->coordStartY;
	this->textY[this->menuText[1]] = this->coordStartY + 2;
	this->textY[this->menuText[2]] = this->coordStartY + 4;
	this->textY[this->menuText[3]] = this->coordStartY + 6;
	this->textY[this->menuText[4]] = this->coordStartY + 8;
	this->textY[this->menuText[5]] = this->coordStartY + 10;

	this->textY[this->tipText[0]] = this->coordStartY;
	this->textY[this->tipText[1]] = this->coordStartY + 2;
	this->textY[this->tipText[2]] = this->coordStartY + 4;
	this->textY[this->tipText[3]] = this->coordStartY + 6;
	this->textY[this->tipText[4]] = this->coordStartY + 8;
}

void Menu::update()
{
	//Setting screen buffer cursor position, coordinates must be within the boundaries of the console screen buffer
	SetConsoleCursorPosition(this->screenBuffer, { short(this->nScreenWidth - 1), short(this->nScreenHeight - 1) });

	//Updating language
	this->updateLanguage();

	if (this->menuCursorY < this->textY[this->menuText[0]])
	{
		this->menuCursorY = this->textY[this->menuText[5]];
	}
	else if (this->menuCursorY > this->textY[this->menuText[5]])
	{
		this->menuCursorY = this->textY[this->menuText[0]];
	}

	//Default attribute PLAY
	FillConsoleOutputAttribute(this->screenBuffer, 7, this->menuCursorSize, { short(this->textX[this->menuText[0]] - 1), this->textY[this->menuText[0]] }, &this->dwNumWritten);

	if (disabled[this->menuText[1]] == true)
	{
		//Disabled attribute CONTINUE
		FillConsoleOutputAttribute(this->screenBuffer, 8, this->menuCursorSize, { short(this->textX[this->menuText[1]] - 1), this->textY[this->menuText[1]] }, &this->dwNumWritten);
	}
	else
	{
		//Default attribute CONTINUE
		FillConsoleOutputAttribute(this->screenBuffer, 7, this->menuCursorSize, { short(this->textX[this->menuText[1]] - 1), this->textY[this->menuText[1]] }, &this->dwNumWritten);
	}

	if (disabled[this->menuText[2]] == true)
	{
		//Disabled attribute SAVE
		FillConsoleOutputAttribute(this->screenBuffer, 8, this->menuCursorSize, { short(this->textX[this->menuText[2]] - 1), this->textY[this->menuText[2]] }, &this->dwNumWritten);
	}
	else
	{
		//Default attribute SAVE
		FillConsoleOutputAttribute(this->screenBuffer, 7, this->menuCursorSize, { short(this->textX[this->menuText[2]] - 1), this->textY[this->menuText[2]] }, &this->dwNumWritten);
	}

	if (disabled[this->menuText[3]] == true)
	{
		//Disabled attribute LOAD
		FillConsoleOutputAttribute(this->screenBuffer, 8, this->menuCursorSize, { short(this->textX[this->menuText[3]] - 1), this->textY[this->menuText[3]] }, &this->dwNumWritten);
	}
	else
	{
		//Default attribute LOAD
		FillConsoleOutputAttribute(this->screenBuffer, 7, this->menuCursorSize, { short(this->textX[this->menuText[3]] - 1), this->textY[this->menuText[3]] }, &this->dwNumWritten);
	}

	//Default attribute SETTINGS
	FillConsoleOutputAttribute(this->screenBuffer, 7, this->menuCursorSize, { short(this->textX[this->menuText[4]] - 1), this->textY[this->menuText[4]] }, &this->dwNumWritten);

	//Default attribute EXIT
	FillConsoleOutputAttribute(this->screenBuffer, 7, this->menuCursorSize, { short(this->textX[this->menuText[5]] - 1), this->textY[this->menuText[5]] }, &this->dwNumWritten);

	//Creating cursor (attribute for cursor)
	FillConsoleOutputAttribute(this->screenBuffer, this->menuCursorAttributes, this->menuCursorSize, { this->menuCursorX, this->menuCursorY }, &this->dwNumWritten);
}

void Menu::render()
{
	//Setting screen buffer to be active
	SetConsoleActiveScreenBuffer(this->screenBuffer);

	//Clearing screen buffer
	this->clearScreen();

	//Hiding console cursor
	SetConsoleCursorInfo(this->screenBuffer, &this->cursorInfo);

	//Writing text to chars buffer
	for (unsigned i = 0; i <= 5; i++)
	{
		write(this->screen, this->coordStartX + 1, this->coordStartY + i * 2, this->menuText[i]);
	}

	write(this->screen, this->textX[this->tipText[0]], this->textY[this->tipText[0]], this->tipText[0]);
	write(this->screen, this->textX[this->tipText[1]], this->textY[this->tipText[1]], this->tipText[1]);
	write(this->screen, this->textX[this->tipText[2]], this->textY[this->tipText[2]], this->tipText[2]);
	write(this->screen, this->textX[this->tipText[3]], this->textY[this->tipText[3]], this->tipText[3]);
	write(this->screen, this->textX[this->tipText[4]], this->textY[this->tipText[4]], this->tipText[4]);

	//Output chars buffer to the screen buffer
	WriteConsoleOutputCharacterW(this->screenBuffer, this->screen, this->nScreenWidth * this->nScreenHeight, { 0,0 }, &this->dwBytesWritten);
}