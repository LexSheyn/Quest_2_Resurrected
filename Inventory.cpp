#include "stdafx.h"
#include "Inventory.h"

#pragma warning(disable:4996)

//Private functions
void Inventory::initVariables()
{
	//Tip position initialization
	this->tipRow = 0;
	this->tipChar = 0;

	//Console variables initialization
	this->nScreenWidth = 120;
	this->nScreenHeight = 30;
	this->dwBytesWritten = 0;
	this->coordStartX = 10;
	this->cursorX = this->coordStartX - 1;
	this->coordStartY = 7;
	this->cursorY = this->coordStartY;
	this->cursorAttributes = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY;
	this->cursorSize = 20;
	this->dwNumWritten = 0;
	this->cDefaultFontHeight = this->cfi.dwFontSize.Y;

	//Inventory slots initialization
	this->slots = 8;
	this->emptySlot = L"::::::::::::::::::";

	//Language names initialization
	this->EN = L"EN";
	this->RU = L"RU";
}

void Inventory::initFiles()
{
	this->pathTips[this->EN] = L"Resources\\dialogue\\tips_EN.plot";
	this->pathTips[this->RU] = L"Resources\\dialogue\\tips_RU.plot";
}

void Inventory::initTips(std::wstring language_name)
{
	int ch = 0;  //Variable to store the characters
	std::string s;  //String to put the characters in

	//Read text from the file
	std::ifstream in(this->pathTips[language_name]); //Open file for reading
	while ((ch = in.get()) != EOF) //Get character by character until the end of file
	{
		if (char(ch) != '\n')
		{
			s = s + char(ch); //Store character to string if character not equals '\n'
		}
		else
		{
			this->tips[language_name].push_back(this->ASCII_to_UTF16(s)); //If character equals '\n' - put string into the end of vector
			s.clear(); //Clear the string
		}
	}
	this->tips[language_name].push_back(this->ASCII_to_UTF16(s)); //Put the last string into the end of vector
	in.close(); //Close file
}

void Inventory::initItems()
{
	//Items initialization
	this->itemEN[0] = L"Millipede claw";
	this->itemEN[1] = L"Guard clothes";
	this->itemEN[2] = L"Lucky solid";
	this->itemEN[3] = L"Priest's talisman";
	this->itemEN[4] = L"Priest's chlamys";
	this->itemEN[5] = L"Wand";
	this->itemEN[6] = L"Rope";
	this->itemEN[7] = L"Scroll";

	this->itemRU[0] = L"Коготь многоножки";
	this->itemRU[1] = L"Одежда стражника";
	this->itemRU[2] = L"Счастливый солид";
	this->itemRU[3] = L"Талисман жреца";
	this->itemRU[4] = L"Хламида жреца";
	this->itemRU[5] = L"Жезл";
	this->itemRU[6] = L"Веревка";
	this->itemRU[7] = L"Свиток";

	//Creating items text
	for (unsigned i = 0; i < this->slots; i++)
	{
		this->itemSlot[i] = this->emptySlot;
	}

	//Setting items text X
	this->textX[this->itemSlot[0]] = this->coordStartX;
	this->textX[this->itemSlot[1]] = this->coordStartX;
	this->textX[this->itemSlot[2]] = this->coordStartX;
	this->textX[this->itemSlot[3]] = this->coordStartX;
	this->textX[this->itemSlot[4]] = this->coordStartX;
	this->textX[this->itemSlot[5]] = this->coordStartX;
	this->textX[this->itemSlot[6]] = this->coordStartX;
	this->textX[this->itemSlot[7]] = this->coordStartX;

	//Setting items text Y
	this->textY[this->itemSlot[0]] = this->coordStartY;
	this->textY[this->itemSlot[1]] = this->coordStartY + 2;
	this->textY[this->itemSlot[2]] = this->coordStartY + 4;
	this->textY[this->itemSlot[3]] = this->coordStartY + 6;
	this->textY[this->itemSlot[4]] = this->coordStartY + 8;
	this->textY[this->itemSlot[5]] = this->coordStartY + 10;
	this->textY[this->itemSlot[6]] = this->coordStartY + 12;
	this->textY[this->itemSlot[7]] = this->coordStartY + 14;
}

void Inventory::initConsoleScreenBuffer()
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

	//Default items attributes
	for (unsigned i = 0; i < 8; i++)
	{
		if (this->itemSlot[i] == this->emptySlot)
		{
			FillConsoleOutputAttribute(this->screenBuffer, 8, this->cursorSize, { this->cursorX, short(this->coordStartY + i * 2) }, &this->dwNumWritten);
		}
		else
		{
			FillConsoleOutputAttribute(this->screenBuffer, 7, this->cursorSize, { this->cursorX, short(this->coordStartY + i * 2) }, &this->dwNumWritten);
		}
	}
	
	//Tip attributes
	for (unsigned i = 0; i < 8; i++)
	{
		FillConsoleOutputAttribute(this->screenBuffer, 8, 60, { (short)(this->coordStartX + 30), short(this->coordStartY + (short)i) }, &this->dwNumWritten);
	}

	//Creating cursor (cursor attributes)
	FillConsoleOutputAttribute(this->screenBuffer, this->cursorAttributes, this->cursorSize, { this->cursorX, this->cursorY }, &this->dwNumWritten);
}

void Inventory::write(wchar_t* buffer, int x, int y, wchar_t ch)
{
	buffer[(short)y * this->nScreenWidth + (short)x] = ch;
}

void Inventory::write(wchar_t* buffer, int x, int y, const std::wstring& s)
{
	this->textX[s] = (short)x;
	this->textY[s] = (short)y;
	swprintf(&buffer[y * this->nScreenWidth + x], s.size() + 1, L"%s", s.c_str());
}

//Constructors and Distructors
Inventory::Inventory()
{
	this->initVariables();

	this->initFiles();
	this->initTips(this->EN);
	this->initTips(this->RU);

	//Setting default language
	this->language = this->EN;

	this->initItems();
	
	for (unsigned i = 0; i < 8; i++)
	{
		this->findTip(this->EN, this->itemEN[i]);
	}

	for (unsigned i = 0; i < 8; i++)
	{
		this->findTip(this->RU, this->itemRU[i]);
	}

	this->initConsoleScreenBuffer();
}

Inventory::~Inventory()
{

}

//Accessors
std::vector<std::wstring>& Inventory::getTip(std::wstring language_name, std::wstring item_name)
{
	if (language_name == this->EN)
	{
		return this->tipEN[item_name];
	}
	else if (language_name == this->RU)
	{
		return this->tipRU[item_name];
	}
}

std::wstring& Inventory::getTip(std::wstring language_name, std::wstring item_name, unsigned row_number)
{
	if (language_name == this->EN)
	{
		return this->tipEN[item_name][row_number];
	}
	else if (language_name == this->RU)
	{
		return this->tipRU[item_name][row_number];
	}
}

wchar_t& Inventory::getTip(std::wstring language_name, std::wstring item_name, unsigned row_number, unsigned char_number)
{
	if (language_name == this->EN)
	{
		return this->tipEN[item_name][row_number][char_number];
	}
	else if (language_name == this->RU)
	{
		return this->tipRU[item_name][row_number][char_number];
	}
}

bool& Inventory::isDisabled(std::wstring menu_text)
{
	return this->disabled[menu_text];
}

const short& Inventory::getCursorX() const
{
	return this->cursorX;
}

const short& Inventory::getCursorY() const
{
	return this->cursorY;
}

const DWORD Inventory::getCursorSize() const
{
	return this->cursorSize;
}

const DWORD Inventory::getCursorAttributes() const
{
	return this->cursorAttributes;
}

short& Inventory::getTextX(std::wstring menu_text)
{
	return this->textX[menu_text];
}

short& Inventory::getTextY(std::wstring menu_text)
{
	return this->textY[menu_text];
}

std::wstring* Inventory::getInventory()
{
	return this->itemSlot;
}

std::wstring& Inventory::getSlot(unsigned text_number)
{
	return this->itemSlot[text_number];
}

std::wstring& Inventory::getItem(unsigned item_number)
{
	if (this->language == this->EN)
	{
		return this->itemEN[item_number];
	}
	else if (this->language == this->RU)
	{
		return this->itemRU[item_number];
	}
}

unsigned& Inventory::getSize()
{
	return this->slots;
}

//Modifiers
void Inventory::setDisabled(const std::wstring menu_text, const bool set_disabled)
{
	this->disabled[menu_text] = set_disabled;
}

void Inventory::setCursorX(const short x)
{
	this->cursorX = x;
}

void Inventory::setCursorY(const short y)
{
	this->cursorY = y;
}

void Inventory::setLanguage(const std::wstring language_name)
{
	this->language = language_name;
}

void Inventory::setFontHeight(unsigned font_height)
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
void Inventory::clearScreen()
{
	//Filling chars buffer with spaces
	std::fill_n(this->screen, this->nScreenWidth * this->nScreenHeight, ' ');
	//FillConsoleOutputAttribute(this->screenBuffer, 7, this->nScreenWidth * this->nScreenHeight, { 0, 0 }, &this->dwNumWritten);
}

std::wstring Inventory::ASCII_to_UTF16(const std::string& str)
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

std::wstring Inventory::returnCurrentTimeAndDate()
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

void Inventory::findTip(std::wstring language_name, std::wstring item_name)
{
	unsigned begin = 0;
	unsigned end = 0;

	std::wstring start = L'@' + item_name + L'@';
	std::wstring finish = L'#' + item_name + L'#';

	for (unsigned i = 0; i < this->tips[language_name].size(); i++)
	{
		if (this->tips[language_name][i].find(start) != std::wstring::npos)
		{
			begin = i;
			this->getTip(language_name, item_name).push_back(this->tips[language_name][begin]);
			this->getTip(language_name, item_name, 0).erase(0, this->getTip(language_name, item_name, 0).find(start) + start.length());
		}
	}

	for (unsigned i = begin; i < this->tips[language_name].size(); i++)
	{
		if (this->tips[language_name][i].find(finish) != std::wstring::npos)
		{
			end = i;
			if (end == begin)
			{
				this->getTip(language_name, item_name, 0).erase(this->getTip(language_name, item_name, 0).find(finish), finish.length());
			}
			else if (end - begin == 1)
			{
				this->getTip(language_name, item_name).push_back(this->tips[language_name][end]);
				this->getTip(language_name, item_name, (getTip(language_name, item_name)).size() - 1).erase(getTip(language_name, item_name, (getTip(language_name, item_name)).size() - 1).find(finish), finish.length());
			}
			else if (end - begin > 1)
			{
				for (unsigned j = begin + 1; j < end; j++)
				{
					this->getTip(language_name, item_name).push_back(tips[language_name][j]);
				}
				this->getTip(language_name, item_name).push_back(this->tips[language_name][end]);
				this->getTip(language_name, item_name, (getTip(language_name, item_name)).size() - 1).erase(getTip(language_name, item_name, (getTip(language_name, item_name)).size() - 1).find(finish), finish.length());
			}
		}
	}
}

void Inventory::fillMenuOutputAttribute(int attribute_number, int length, short x, short y)
{
	FillConsoleOutputAttribute(this->screenBuffer, attribute_number, length, { x, y }, &this->dwNumWritten);
}

void Inventory::addItem(std::wstring item_name)
{
	for (unsigned i = 0; i < this->slots; i++)
	{
		if (this->itemSlot[i] == this->emptySlot)
		{
			this->itemSlot[i] = item_name;
			break;
		}
	}
}

void Inventory::addItem(unsigned item_number)
{
	for (unsigned i = 0; i < this->slots; i++)
	{
		if (this->itemSlot[i] == this->emptySlot)
		{
			if (this->language == this->EN)
			{
				this->itemSlot[i] = this->itemEN[item_number];
			}
			else if (this->language == this->RU)
			{
				this->itemSlot[i] = this->itemRU[item_number];
			}
			break;
		}
	}
}

void Inventory::deleteItems()
{
	for (unsigned i = 0; i < this->slots; i++)
	{
		this->itemSlot[i] = this->emptySlot;
	}

	this->cursorY = this->coordStartY;
}

void Inventory::updateLanguage()
{
	//Creating items text
	for (unsigned i = 0; i < this->slots; i++)
	{
		for (unsigned j = 0; j < this->slots; j++)
		{
			if (this->itemSlot[i] == this->itemEN[j])
			{
				if (this->language == this->RU)
				{
					this->itemSlot[i] = this->itemRU[j];
				}
			}
			else if (this->itemSlot[i] == this->itemRU[j])
			{
				if (this->language == this->EN)
				{
					this->itemSlot[i] = this->itemEN[j];
				}
			}
		}
	}

	//Setting items text X
	this->textX[this->itemSlot[0]] = this->coordStartX;
	this->textX[this->itemSlot[1]] = this->coordStartX;
	this->textX[this->itemSlot[2]] = this->coordStartX;
	this->textX[this->itemSlot[3]] = this->coordStartX;
	this->textX[this->itemSlot[4]] = this->coordStartX;
	this->textX[this->itemSlot[5]] = this->coordStartX;
	this->textX[this->itemSlot[6]] = this->coordStartX;
	this->textX[this->itemSlot[7]] = this->coordStartX;

	//Setting items text Y
	this->textY[this->itemSlot[0]] = this->coordStartY;
	this->textY[this->itemSlot[1]] = this->coordStartY + 2;
	this->textY[this->itemSlot[2]] = this->coordStartY + 4;
	this->textY[this->itemSlot[3]] = this->coordStartY + 6;
	this->textY[this->itemSlot[4]] = this->coordStartY + 8;
	this->textY[this->itemSlot[5]] = this->coordStartY + 10;
	this->textY[this->itemSlot[6]] = this->coordStartY + 12;
	this->textY[this->itemSlot[7]] = this->coordStartY + 14;
}

void Inventory::update()
{
	//Setting screen buffer cursor position, coordinates must be within the boundaries of the console screen buffer
	SetConsoleCursorPosition(this->screenBuffer, { short(this->nScreenWidth - 1), short(this->nScreenHeight - 1) });

	//Updating language
	this->updateLanguage();

	if (this->cursorY < this->coordStartY)
	{
		this->cursorY = this->coordStartY + 14;
	}
	else if (this->cursorY > this->coordStartY + 14)
	{
		this->cursorY = this->coordStartY;
	}

	//Default items attributes
	for (unsigned i = 0; i < this->slots; i++)
	{
		if (this->itemSlot[i] == this->emptySlot)
		{
			FillConsoleOutputAttribute(this->screenBuffer, 8, this->cursorSize, { this->cursorX, short(this->coordStartY + i * 2) }, &this->dwNumWritten);
		}
		else
		{
			FillConsoleOutputAttribute(this->screenBuffer, 7, this->cursorSize, { this->cursorX, short(this->coordStartY + i * 2) }, &this->dwNumWritten);
		}
	}

	//Tip attributes
	for (unsigned i = 0; i < this->slots; i++)
	{
		FillConsoleOutputAttribute(this->screenBuffer, 8, 60, { short(this->coordStartX + 30), short(this->coordStartY + i) }, &this->dwNumWritten);
	}

	//Creating cursor (cursor attributes)
	FillConsoleOutputAttribute(this->screenBuffer, this->cursorAttributes, this->cursorSize, { this->cursorX, this->cursorY }, &this->dwNumWritten);
}

void Inventory::renderTip()
{	
	for (unsigned i = 0; i < this->slots; i++)
	{
		if (this->cursorY == this->textY[this->itemSlot[i]])
		{
			for (unsigned j = 0; j < this->getTip(this->language, this->itemSlot[i]).size(); j++)
			{
				write(this->screen, this->coordStartX + 30, this->coordStartY + j, this->getTip(this->language, this->itemSlot[i], j));
			}
		}
	}
}

void Inventory::render()
{
	//Setting screen buffer to be active
	SetConsoleActiveScreenBuffer(this->screenBuffer);

	//Clearing screen buffer
	this->clearScreen();

	//Hiding console cursor
	SetConsoleCursorInfo(this->screenBuffer, &this->cursorInfo);

	//Writing text to chars buffer
	for (unsigned i = 0; i < this->slots; i++)
	{
		write(this->screen, this->coordStartX, this->coordStartY + i * 2, this->itemSlot[i]);
	}

	//Showing tip
	this->renderTip();

	//Output chars buffer to the screen buffer
	WriteConsoleOutputCharacterW(this->screenBuffer, this->screen, this->nScreenWidth * this->nScreenHeight, { 0,0 }, &this->dwBytesWritten);
}