#include "stdafx.h"
#include "Settings.h"

#pragma warning(disable:4996)

//Private functions
void Settings::initVariables()
{
	//Settings file path initialization
	this->filePath = L"Resources\\settings\\settings.ini";

	//Console variables initialization
	this->nScreenWidth = 120;
	this->nScreenHeight = 30;
	this->dwBytesWritten = 0;
	this->menuCursorSize = 13;
	this->distanceX = 2;
	this->coordStartX = 10;
	this->menuCursorX = this->coordStartX + 12 + this->distanceX;
	this->coordStartY = 8;
	this->menuCursorY = this->coordStartY;
	this->menuCursorAttributes = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY;
	this->dwNumWritten = 0;

	//Music and sound ON/OFF
	this->musicOn = true;
	this->soundsOn = true;

	//Setting default language
	this->language = L"EN";
	this->languageCursorSize = 10;

	//Creating menu text
	if (this->language == L"EN")
	{
		this->menuText[0] = L"Play";
		this->menuText[1] = L"Continue";
		this->menuText[2] = L"Save";
		this->menuText[3] = L"Load";
		this->menuText[4] = L"Settings";
		this->menuText[5] = L"Exit";
	}
	else if (this->language == L"RU")
	{
		this->menuText[0] = L"Играть";
		this->menuText[1] = L"Продолжить";
		this->menuText[2] = L"Сохранить";
		this->menuText[3] = L"Загрузить";
		this->menuText[4] = L"Настройки";
		this->menuText[5] = L"Выйти";
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

	if (this->language == L"EN")
	{
		//Creating settings text
		this->settingsText[0] = L"Music ON";
		this->settingsText[1] = L"Music OFF";
		this->settingsText[2] = L"Sounds ON";
		this->settingsText[3] = L"Sounds OFF";
		this->settingsText[4] = L"Language";
		this->settingsText[5] = L"EN";
		this->settingsText[6] = L"RU";
	}
	else if (this->language == L"RU")
	{
		//Creating settings text
		this->settingsText[0] = L"Музыка ВКЛ";
		this->settingsText[1] = L"Музыка ВЫКЛ";
		this->settingsText[2] = L"Звуки ВКЛ";
		this->settingsText[3] = L"Звуки Выкл";
		this->settingsText[4] = L"Язык";
		this->settingsText[5] = L"EN";
		this->settingsText[6] = L"RU";
	}

	//Setting settings text X
	this->textX[this->settingsText[0]] = this->coordStartX + this->menuCursorSize + this->distanceX;
	this->textX[this->settingsText[1]] = this->coordStartX + this->menuCursorSize + this->distanceX;
	this->textX[this->settingsText[2]] = this->coordStartX + this->menuCursorSize + this->distanceX;
	this->textX[this->settingsText[3]] = this->coordStartX + this->menuCursorSize + this->distanceX;
	this->textX[this->settingsText[4]] = this->coordStartX + this->menuCursorSize + this->distanceX;
	this->textX[this->settingsText[5]] = this->coordStartX + this->menuCursorSize * 2 + 3 + this->distanceX;
	this->textX[this->settingsText[6]] = this->coordStartX + this->menuCursorSize * 2 + 9 + this->distanceX;

	//Setting settings text Y
	this->textY[this->settingsText[0]] = this->coordStartY;
	this->textY[this->settingsText[1]] = this->coordStartY;
	this->textY[this->settingsText[2]] = this->coordStartY + 2;
	this->textY[this->settingsText[3]] = this->coordStartY + 2;
	this->textY[this->settingsText[4]] = this->coordStartY + 4;
	this->textY[this->settingsText[5]] = this->coordStartY + 4;
	this->textY[this->settingsText[6]] = this->coordStartY + 4;

	//Setting music volume X, Y and size
	this->mVolumeSizeMax = 10;
	this->mVolumeSize = 5;
	this->mVolumeX = this->coordStartX + this->menuCursorSize * 2 + 2 + this->distanceX;
	this->mVolumeY = this->coordStartY;

	//Setting sound volume X, Y and size
	this->sVolumeSizeMax = 10;
	this->sVolumeSize = 5;
	this->sVolumeX = this->coordStartX + this->menuCursorSize * 2 + 2 + this->distanceX;
	this->sVolumeY = this->coordStartY + 2;
}

void Settings::initConsoleScreenBuffer()
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

	//Disabled attribute SAVE
	FillConsoleOutputAttribute(this->screenBuffer, 8, this->menuCursorSize, { short(this->textX[this->menuText[2]] - 1), this->textY[this->menuText[2]] }, &this->dwNumWritten);

	//Disabled attribute LOAD
	FillConsoleOutputAttribute(this->screenBuffer, 8, this->menuCursorSize, { short(this->textX[this->menuText[3]] - 1), this->textY[this->menuText[3]] }, &this->dwNumWritten);

	//Active attribute SETTINGS
	FillConsoleOutputAttribute(this->screenBuffer, 15, this->menuCursorSize, { short(this->textX[this->menuText[4]] - 1), this->textY[this->menuText[4]] }, &this->dwNumWritten);

	//Disabled attribute EXIT
	FillConsoleOutputAttribute(this->screenBuffer, 8, this->menuCursorSize, { short(this->textX[this->menuText[5]] - 1), this->textY[this->menuText[5]] }, &this->dwNumWritten);
	
	//Default attribute MUSIC ON
	FillConsoleOutputAttribute(this->screenBuffer, 7, this->menuCursorSize, { short(this->textX[this->settingsText[0]] - 1), this->textY[this->settingsText[0]] }, &this->dwNumWritten);
	
	//Default attribute SOUNDS ON
	FillConsoleOutputAttribute(this->screenBuffer, 7, this->menuCursorSize, { short(this->textX[this->settingsText[2]] - 1), this->textY[this->settingsText[2]] }, &this->dwNumWritten);
	
	//Default attribute LANGUAGE
	FillConsoleOutputAttribute(this->screenBuffer, 7, this->menuCursorSize, { short(this->textX[this->settingsText[4]] - 1), this->textY[this->settingsText[4]] }, &this->dwNumWritten);
		
	//Default music volume attribute
	FillConsoleOutputAttribute(this->screenBuffer, 128, this->mVolumeSizeMax, { this->mVolumeX, this->mVolumeY }, &this->dwNumWritten);
	FillConsoleOutputAttribute(this->screenBuffer, this->menuCursorAttributes, this->mVolumeSize, { this->mVolumeX, this->mVolumeY }, &this->dwNumWritten);
	
	//Default sound volume attribute
	FillConsoleOutputAttribute(this->screenBuffer, 128, this->sVolumeSizeMax, { this->sVolumeX, this->sVolumeY }, &this->dwNumWritten);
	FillConsoleOutputAttribute(this->screenBuffer, this->menuCursorAttributes,	this->sVolumeSize, { this->sVolumeX, this->sVolumeY }, &this->dwNumWritten);
	
	//Default language attribute
	FillConsoleOutputAttribute(this->screenBuffer, 128, this->languageCursorSize, { short(this->textX[this->settingsText[5]] - 1), this->textY[this->settingsText[5]] }, &this->dwNumWritten);

	//Selecred language attribute
	if (this->language == L"EN")
	{
		FillConsoleOutputAttribute(this->screenBuffer, this->menuCursorAttributes, short(this->languageCursorSize / 2), { short(this->textX[this->settingsText[5]] - 1), this->textY[this->settingsText[5]] }, &this->dwNumWritten);
	}
	else if (this->language == L"RU")
	{
		FillConsoleOutputAttribute(this->screenBuffer, this->menuCursorAttributes, short(this->languageCursorSize / 2), { short(this->textX[this->settingsText[6]] - 2), this->textY[this->settingsText[6]] }, &this->dwNumWritten);
	}

	//Creating cursor (attribute for cursor)
	FillConsoleOutputAttribute(this->screenBuffer, this->menuCursorAttributes, this->menuCursorSize, { this->menuCursorX, this->menuCursorY }, &this->dwNumWritten);
}

void Settings::write(wchar_t* buffer, short x, short y, wchar_t ch)
{
	buffer[y * this->nScreenWidth + x] = ch;
}

void Settings::write(wchar_t* buffer, short x, short y, const std::wstring& s)
{
	this->textX[s] = x;
	this->textY[s] = y;
	swprintf(&buffer[y * this->nScreenWidth + x], s.size() + 1, L"%s", s.c_str());
}

//Constructors and Distructors
Settings::Settings()
{
	this->initVariables();
	this->initConsoleScreenBuffer();
}

Settings::~Settings()
{

}

//Accessors
HANDLE& Settings::getHandle()
{
	return this->screenBuffer;
}

std::wstring& Settings::getFilePath()
{
	return this->filePath;
}

std::wstring& Settings::getLanguage()
{
	return this->language;
}

bool& Settings::isDisabled(std::wstring menu_text)
{
	return this->disabled[menu_text];
}

const short& Settings::getCursorX() const
{
	return this->menuCursorX;
}

const short& Settings::getCursorY() const
{
	return this->menuCursorY;
}

const DWORD Settings::getCursorSize() const
{
	return this->menuCursorSize;
}

const DWORD Settings::getCursorAttributes() const
{
	return this->menuCursorAttributes;
}

short& Settings::getTextX(std::wstring menu_text)
{
	return this->textX[menu_text];
}

short& Settings::getTextY(std::wstring menu_text)
{
	return this->textY[menu_text];
}

std::wstring& Settings::getMenuText(unsigned text_number)
{
	return this->menuText[text_number];
}

std::wstring& Settings::getSettingsText(int text_number)
{
	return this->settingsText[text_number];
}

bool& Settings::getMusicOn()
{
	return this->musicOn;
}

short& Settings::getMusicVolumeX()
{
	return this->mVolumeX;
}

short& Settings::getMusicVolumeSize()
{
	return this->mVolumeSize;
}

bool& Settings::getSoundsOn()
{
	return this->soundsOn;
}

short& Settings::getSoundVolumeX()
{
	return this->sVolumeX;
}

short& Settings::getSoundVolumeSize()
{
	return this->sVolumeSize;
}

bool& Settings::getAnimationOn()
{
	return this->animationOn;
}

//Modifiers
void Settings::setDisabled(const std::wstring menu_text, const bool set_disabled)
{
	this->disabled[menu_text] = set_disabled;
}

void Settings::setCursorX(const short x)
{
	this->menuCursorX = x;
}

void Settings::setCursorY(const short y)
{
	this->menuCursorY = y;
}

void Settings::turnMusicOn(const bool turn_on)
{
	this->musicOn = turn_on;
}

void Settings::setMusicVolumeSize(const short value)
{
	this->mVolumeSize = value;
}

void Settings::increaseMusicVolumeSize(const short value)
{
	this->mVolumeSize += value;
	if (this->mVolumeSize > this->mVolumeSizeMax)
	{
		this->mVolumeSize = this->mVolumeSizeMax;
	}
	else if (this->mVolumeSize < 0)
	{
		this->mVolumeSize = 0;
	}
}

void Settings::turnSoundsOn(const bool turn_on)
{
	this->soundsOn = turn_on;
}

void Settings::setSoundsVolumeSize(const short value)
{
	this->sVolumeSize = value;
}

void Settings::increaseSoundsVolumeSize(const short value)
{
	this->sVolumeSize += value;
	if (this->sVolumeSize > this->sVolumeSizeMax)
	{
		this->sVolumeSize = this->sVolumeSizeMax;
	}
	else if (this->sVolumeSize < 0)
	{
		this->sVolumeSize = 0;
	}
}

void Settings::setLanguage(const std::wstring language_name)
{
	this->language = language_name;
}

void Settings::setFontHeight(unsigned font_height)
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

void Settings::turnAnimationOn(const bool turn_on)
{
	this->animationOn = turn_on;
}

//Functions
void Settings::clearScreen()
{
	//Filling chars buffer with spaces
	std::fill_n(this->screen, this->nScreenWidth * this->nScreenHeight, ' ');
}

std::wstring Settings::ASCII_to_UTF16(const std::string& str)
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

std::wstring Settings::returnCurrentTimeAndDate()
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

void Settings::fillMenuOutputAttribute(int attribute_number, int length, short x, short y)
{
	FillConsoleOutputAttribute(this->screenBuffer, attribute_number, length, { x, y }, &this->dwNumWritten);
}

void Settings::updateLanguage()
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

	if (this->language == L"EN")
	{
		//Creating settings text
		this->settingsText[0] = L"Music ON";
		this->settingsText[1] = L"Music OFF";
		this->settingsText[2] = L"Sounds ON";
		this->settingsText[3] = L"Sounds OFF";
		this->settingsText[4] = L"Language";
		this->settingsText[5] = L"EN";
		this->settingsText[6] = L"RU";
	}
	else if (this->language == L"RU")
	{
		//Creating settings text
		this->settingsText[0] = L"Музыка ВКЛ";
		this->settingsText[1] = L"Музыка ВЫКЛ";
		this->settingsText[2] = L"Звуки ВКЛ";
		this->settingsText[3] = L"Звуки Выкл";
		this->settingsText[4] = L"Язык";
		this->settingsText[5] = L"EN";
		this->settingsText[6] = L"RU";
	}

	//Setting settings text X
	this->textX[this->settingsText[0]] = this->coordStartX + this->menuCursorSize + this->distanceX;
	this->textX[this->settingsText[1]] = this->coordStartX + this->menuCursorSize + this->distanceX;
	this->textX[this->settingsText[2]] = this->coordStartX + this->menuCursorSize + this->distanceX;
	this->textX[this->settingsText[3]] = this->coordStartX + this->menuCursorSize + this->distanceX;
	this->textX[this->settingsText[4]] = this->coordStartX + this->menuCursorSize + this->distanceX;
	this->textX[this->settingsText[5]] = this->coordStartX + this->menuCursorSize * 2 + 3 + this->distanceX;
	this->textX[this->settingsText[6]] = this->coordStartX + this->menuCursorSize * 2 + 9 + this->distanceX;

	//Setting settings text Y
	this->textY[this->settingsText[0]] = this->coordStartY;
	this->textY[this->settingsText[1]] = this->coordStartY;
	this->textY[this->settingsText[2]] = this->coordStartY + 2;
	this->textY[this->settingsText[3]] = this->coordStartY + 2;
	this->textY[this->settingsText[4]] = this->coordStartY + 4;
	this->textY[this->settingsText[5]] = this->coordStartY + 4;
	this->textY[this->settingsText[6]] = this->coordStartY + 4;
}

void Settings::update()
{
	//Setting screen buffer cursor position, coordinates must be within the boundaries of the console screen buffer
	SetConsoleCursorPosition(this->screenBuffer, { short(this->nScreenWidth - 1), short(this->nScreenHeight - 1) });

	//Updating language
	this->updateLanguage();

	if (this->menuCursorY < this->textY[this->settingsText[0]])
	{
		this->menuCursorY = this->textY[this->settingsText[4]];
	}
	else if (this->menuCursorY > this->textY[this->settingsText[4]])
	{
		this->menuCursorY = this->textY[this->settingsText[0]];
	}

	//Disabled attribute PLAY
	FillConsoleOutputAttribute(this->screenBuffer, 8, this->menuCursorSize, { short(this->textX[this->menuText[0]] - 1), this->textY[this->menuText[0]] }, &this->dwNumWritten);

	//Disabled attribute CONTINUE
	FillConsoleOutputAttribute(this->screenBuffer, 8, this->menuCursorSize, { short(this->textX[this->menuText[1]] - 1), this->textY[this->menuText[1]] }, &this->dwNumWritten);

	//Disabled attribute SAVE
	FillConsoleOutputAttribute(this->screenBuffer, 8, this->menuCursorSize, { short(this->textX[this->menuText[2]] - 1), this->textY[this->menuText[2]] }, &this->dwNumWritten);

	//Disabled attribute LOAD
	FillConsoleOutputAttribute(this->screenBuffer, 8, this->menuCursorSize, { short(this->textX[this->menuText[3]] - 1), this->textY[this->menuText[3]] }, &this->dwNumWritten);

	//Active attribute SETTINGS
	FillConsoleOutputAttribute(this->screenBuffer, 15, this->menuCursorSize, { short(this->textX[this->menuText[4]] - 1), this->textY[this->menuText[4]] }, &this->dwNumWritten);

	//Disabled attribute EXIT
	FillConsoleOutputAttribute(this->screenBuffer, 8, this->menuCursorSize, { short(this->textX[this->menuText[5]] - 1), this->textY[this->menuText[5]] }, &this->dwNumWritten);

	//Default attribute MUSIC ON
	FillConsoleOutputAttribute(this->screenBuffer, 7, this->menuCursorSize, { short(this->textX[this->settingsText[0]] - 1), this->textY[this->settingsText[0]] }, &this->dwNumWritten);

	//Default attribute SOUNDS ON
	FillConsoleOutputAttribute(this->screenBuffer, 7, this->menuCursorSize, { short(this->textX[this->settingsText[2]] - 1), this->textY[this->settingsText[2]] }, &this->dwNumWritten);

	//Default attribute LANGUAGE
	FillConsoleOutputAttribute(this->screenBuffer, 7, this->menuCursorSize, { short(this->textX[this->settingsText[4]] - 1), this->textY[this->settingsText[4]] }, &this->dwNumWritten);

	//Default music volume attribute
	FillConsoleOutputAttribute(this->screenBuffer, 128, this->mVolumeSizeMax, { this->mVolumeX, this->mVolumeY }, &this->dwNumWritten);
	FillConsoleOutputAttribute(this->screenBuffer, this->menuCursorAttributes, this->mVolumeSize, { this->mVolumeX, this->mVolumeY }, &this->dwNumWritten);

	//Default sound volume attribute
	FillConsoleOutputAttribute(this->screenBuffer, 128, this->sVolumeSizeMax, { this->sVolumeX, this->sVolumeY }, &this->dwNumWritten);
	FillConsoleOutputAttribute(this->screenBuffer, this->menuCursorAttributes, this->sVolumeSize, { this->sVolumeX, this->sVolumeY }, &this->dwNumWritten);

	//Default language attribute
	FillConsoleOutputAttribute(this->screenBuffer, 128, this->languageCursorSize, { short(this->textX[this->settingsText[5]] - 1), this->textY[this->settingsText[5]] }, &this->dwNumWritten);

	//Selecred language attribute
	if (this->language == L"EN")
	{
		FillConsoleOutputAttribute(this->screenBuffer, this->menuCursorAttributes, this->languageCursorSize / 2, { short(this->textX[this->settingsText[5]] - 1), this->textY[this->settingsText[5]] }, &this->dwNumWritten);
	}
	else if (this->language == L"RU")
	{
		FillConsoleOutputAttribute(this->screenBuffer, this->menuCursorAttributes, this->languageCursorSize / 2, { short(this->textX[this->settingsText[6]] - 2), this->textY[this->settingsText[6]] }, &this->dwNumWritten);
	}

	//Creating cursor (attribute for cursor)
	FillConsoleOutputAttribute(this->screenBuffer, this->menuCursorAttributes, this->menuCursorSize, { this->menuCursorX, this->menuCursorY }, &this->dwNumWritten);
}

void Settings::render()
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
	if (this->musicOn == true)
	{
		write(this->screen, this->textX[this->settingsText[0]], this->textY[this->settingsText[0]], this->settingsText[0]);
	}
	else
	{
		write(this->screen, this->textX[this->settingsText[1]], this->textY[this->settingsText[1]], this->settingsText[1]);
	}

	if (this->soundsOn == true)
	{
		write(this->screen, this->textX[this->settingsText[2]], this->textY[this->settingsText[2]], this->settingsText[2]);
	}
	else
	{
		write(this->screen, this->textX[this->settingsText[3]], this->textY[this->settingsText[3]], this->settingsText[3]);
	}
	
	write(this->screen, this->textX[this->settingsText[4]], this->textY[this->settingsText[4]], this->settingsText[4]);
	write(this->screen, this->textX[this->settingsText[5]], this->textY[this->settingsText[5]], this->settingsText[5]);
	write(this->screen, this->textX[this->settingsText[6]], this->textY[this->settingsText[6]], this->settingsText[6]);

	//Output chars buffer to the screen buffer
	WriteConsoleOutputCharacterW(this->screenBuffer, this->screen, this->nScreenWidth * this->nScreenHeight, { 0,0 }, &this->dwBytesWritten);
}