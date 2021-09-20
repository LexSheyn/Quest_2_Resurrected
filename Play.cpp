#include "stdafx.h"
#include "Play.h"

#pragma warning(disable:4996)

//Private functions
void Play::initFiles()
{
	this->pathDialogues[L"EN"] = L"Resources\\dialogue\\scenario_EN.plot";
	this->pathDialogues[L"RU"] = L"Resources\\dialogue\\scenario_RU.plot";
	this->pathChoices[L"EN"] = L"Resources\\dialogue\\choice_EN.plot";
	this->pathChoices[L"RU"] = L"Resources\\dialogue\\choice_RU.plot";
	this->pathSave[1] = L"Resources\\save\\File01.save";
	this->pathSave[2] = L"Resources\\save\\File02.save";
	this->pathSave[3] = L"Resources\\save\\File03.save";
	this->pathSave[4] = L"Resources\\save\\File04.save";
}

void Play::initDialogues(std::wstring language_name)
{
	int ch = 0;  //Variable to store the characters
	std::string s;  //String to put the characters in

	//Read text from the file
	std::ifstream in(this->pathDialogues[language_name]); //Open file for reading
	while ((ch = in.get()) != EOF) //Get character by character until the end of file
	{
		if (char(ch) != '\n')
		{
			s = s + char(ch); //Store character to string if character not equals '\n'
		}
		else
		{
			this->dialogues[language_name].push_back(this->ASCII_to_UTF16(s)); //If character equals '\n' - put string into the end of vector
			s.clear(); //Clear the string
		}
	}
	this->dialogues[language_name].push_back(this->ASCII_to_UTF16(s)); //Put the last string into the end of vector
	in.close(); //Close file
}

void Play::initChoices(std::wstring language_name)
{
	int ch = 0;  //Variable to store the characters
	std::string s;  //String to put the characters in

	//Read text from the file
	std::ifstream in(this->pathChoices[language_name]); //Open file for reading
	while ((ch = in.get()) != EOF) //Get character by character until the end of file
	{
		if (char(ch) != '\n')
		{
			s = s + char(ch); //Store character to string if character not equals '\n'
		}
		else
		{
			this->choices[language_name].push_back(this->ASCII_to_UTF16(s)); //If character equals '\n' - put string into the end of vector
			s.clear(); //Clear the string
		}
	}
	this->choices[language_name].push_back(this->ASCII_to_UTF16(s)); //Put the last string into the end of vector
	in.close(); //Close file
}

void Play::initVariables()
{
	//Chapter initialization
	this->chapter = 0;

	this->visited[104] = false;
	this->visited[108] = false;
	this->visited[110] = false;

	this->metGhost = false;

	//Dialogue position initialization
	this->dialogueRow = 0;
	this->dialogueChar = 0;

	//Setting skip animation variable
	this->animation = true;

	//Setting text written variable
	this->written = false;

	//Setting disabled choices
	this->disabled[1] = false;
	this->disabled[2] = false;

	//Console variables initialization
	this->nScreenWidth = 120;
	this->nScreenHeight = 30;
	this->dwBytesWritten = 0;
	this->dwNumWritten = 0;
	this->coordStartX = 14;
	this->menuCursorX = this->coordStartX;
	this->coordStartY = 3;
	this->menuCursorY = 21;
	this->menuCursorAttributes = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY;
	this->menuCursorSize = 83;

	//Setting default language
	this->language = L"EN";

	//Creating choices text
	this->choice[0] = this->choices[this->language][0];
	this->choice[1] = this->choices[this->language][1];
	this->choice[2] = this->choices[this->language][2];
	this->choice[3] = this->choices[this->language][3];

	if (this->language == L"EN")
	{
		this->choice[4] = L" Next";
	}
	else if (this->language == L"RU")
	{
		this->choice[4] = L" Далее";
	}

	//Setting choice coord X
	this->textX[this->choice[0]] = this->coordStartX;
	this->textX[this->choice[1]] = this->coordStartX;
	this->textX[this->choice[2]] = this->coordStartX;
	this->textX[this->choice[3]] = this->coordStartX;
	this->textX[this->choice[4]] = this->coordStartX;

	//Setting choice coord Y
	this->textY[this->choice[0]] = 21;
	this->textY[this->choice[1]] = 23;
	this->textY[this->choice[2]] = 25;
	this->textY[this->choice[3]] = 27;
	this->textY[this->choice[4]] = 21;

	//Initialize timer
	this->spawnTimerMax = 0.5f;
	this->spawnTimer = this->spawnTimerMax;

	for (unsigned i = 0; i < 30; i++)
	{
		this->shownDialogue[i] = L"";
	}

	//Game start
	this->started = false;
}

void Play::initConsoleScreenBuffer()
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
}

//Constructors and Distructors
Play::Play()
{
	this->initFiles();
	this->initDialogues(L"EN");
	this->initDialogues(L"RU");
	this->initChoices(L"EN");
	this->initChoices(L"RU");
	this->initVariables();
		
	for (unsigned i = 600; i <= 606; i++)
	{
		//Finding intro
		this->findDialogue(L"EN", i);
		this->findDialogue(L"RU", i);

		for (unsigned j = 1; j <= 4; j++)
		{
			//Finding choices
			this->findChoice(L"EN", i, j);
			this->findChoice(L"RU", i, j);
		}
	}

	for (unsigned i = 0; i <= 158; i++)
	{
		//Finding dialogues
		this->findDialogue(L"EN", i);
		this->findDialogue(L"RU", i);

		for (unsigned j = 1; j <= 4; j++)
		{
			//Finding choices
			this->findChoice(L"EN", i, j);
			this->findChoice(L"RU", i, j);
		}
	}

	for (unsigned i = 600; i <= 606; i++)
	{
		for (unsigned j = 1; j <= 4; j++)
		{
			//Finding ways
			this->findWay(L"EN", i, j);
		}
	}

	for (unsigned i = 0; i <= 158; i++)
	{
		for (unsigned j = 1; j <= 4; j++)
		{
			//Finding ways
			this->findWay(L"EN", i, j);
		}
	}

	//Console buffer initialization
	this->initConsoleScreenBuffer();
}

Play::~Play()
{

}

//Accessors
HANDLE& Play::getHandle()
{
	return this->screenBuffer;
}

std::wstring& Play::getPathSave(unsigned save_number)
{
	return this->pathSave[save_number];
}

std::vector<std::wstring>& Play::getDialogues(std::wstring language_name)
{
	return this->dialogues[language_name];
}

std::vector<std::wstring>& Play::getDialogue(std::wstring language_name, unsigned chapter_number)
{
	if (language_name == L"EN")
	{
		return this->dialogueEN[chapter_number];
	}
	else if (language_name == L"RU")
	{
		return this->dialogueRU[chapter_number];
	}
}

std::wstring& Play::getDialogue(std::wstring language_name, unsigned chapter_number, unsigned row_number)
{
	if (language_name == L"EN")
	{
		return this->dialogueEN[chapter_number][row_number];
	}
	else if (language_name == L"RU")
	{
		return this->dialogueRU[chapter_number][row_number];
	}
}

wchar_t& Play::getDialogue(std::wstring language_name, unsigned chapter_number, unsigned row_number, unsigned char_number)
{
	if (language_name == L"EN")
	{
		return this->dialogueEN[chapter_number][row_number][char_number];
	}
	else if (language_name == L"RU")
	{
		return this->dialogueRU[chapter_number][row_number][char_number];
	}
}

std::vector<std::wstring>& Play::getChoices(std::wstring language_name)
{
	return this->choices[language_name];
}

std::vector<std::wstring>& Play::getChoice(std::wstring language_name, unsigned chapter_number)
{
	if (language_name == L"EN")
	{
		return this->choiceEN[chapter_number];
	}
	else if (language_name == L"RU")
	{
		return this->choiceRU[chapter_number];
	}
}

std::wstring& Play::getChoice(std::wstring language_name, unsigned chapter_number, unsigned row_number)
{
	if (language_name == L"EN")
	{
		return this->choiceEN[chapter_number][row_number];
	}
	else if (language_name == L"RU")
	{
		return this->choiceRU[chapter_number][row_number];
	}
}

wchar_t& Play::getChoice(std::wstring language_name, unsigned chapter_number, unsigned row_number, unsigned char_number)
{
	if (language_name == L"EN")
	{
		return this->choiceEN[chapter_number][row_number][char_number];
	}
	else if (language_name == L"RU")
	{
		return this->choiceRU[chapter_number][row_number][char_number];
	}
}

bool& Play::isDisabled(unsigned choice_number)
{
	return this->disabled[choice_number];
}

const short& Play::getCursorX() const
{
	return this->menuCursorX;
}

const short& Play::getCursorY() const
{
	return this->menuCursorY;
}

const DWORD Play::getCursorSize() const
{
	return this->menuCursorSize;
}

const DWORD Play::getCursorAttributes() const
{
	return this->menuCursorAttributes;
}

short& Play::getChoiceTextX(std::wstring choice_text)
{
	return this->textX[choice_text];
}

short& Play::getChoiceTextY(std::wstring choice_text)
{
	return this->textY[choice_text];
}

std::wstring& Play::getChoiceText(std::wstring language_name, unsigned choice_number)
{
	return this->choices[language_name][choice_number];
}

std::wstring& Play::getChoiceText(unsigned choice_number)
{
	return this->choice[choice_number];
}

short& Play::getCharX(wchar_t character)
{
	return this->charX[character];
}

short& Play::getCharY(wchar_t character)
{
	return this->charY[character];
}

wchar_t& Play::getDialogueChar(std::wstring language_name, unsigned row_number, unsigned char_number)
{
	return this->dialogues[language_name][row_number][char_number];
}

const std::wstring& Play::getLanguage() const
{
	return this->language;
}

const bool& Play::isWritten() const
{
	return this->written;
}

const bool& Play::getAnimation() const
{
	return this->animation;
}

unsigned& Play::getChapter()
{
	return this->chapter;
}

bool& Play::getVisited(unsigned chapter_number)
{
	return this->visited[chapter_number];
}

bool& Play::getMetGhost()
{
	return this->metGhost;
}

std::wstring& Play::getChoiceWay(unsigned chapter_number, unsigned wstring_number)
{
	return this->choiceWay[chapter_number][wstring_number];
}

const bool& Play::getStarted() const
{
	return this->started;
}

//Modifiers
void Play::setDisabled(const unsigned choice_number, const bool disable)
{
	this->disabled[choice_number] = disable;
}

void Play::setCursorX(const short x)
{
	this->menuCursorX = x;
}

void Play::setCursorY(const short y)
{
	this->menuCursorY = y;
}

void Play::setLanguage(const std::wstring language_name)
{
	this->language = language_name;
}

void Play::setFontHeight(unsigned font_height)
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

void Play::setAnimation(const bool skip)
{
	this->animation = skip;
}

void Play::setWritten(const bool set_written)
{
	this->written = set_written;
}

void Play::setChapter(unsigned chapter_number)
{
	this->chapter = chapter_number;
}

void Play::setVisited(unsigned chapter_number, const bool set_visited)
{
	this->visited[chapter_number] = set_visited;
}

void Play::setMetGhost(const bool set_met)
{
	this->metGhost = set_met;
}

void Play::setStarted(bool game_started)
{
	this->started = game_started;
}

//Functions
void Play::clearScreen()
{
	//Filling chars buffer with spaces
	std::fill_n(this->screen, this->nScreenWidth * this->nScreenHeight, ' ');
}

std::wstring Play::ASCII_to_UTF16(const std::string& str)
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

std::wstring Play::returnCurrentTimeAndDate()
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

void Play::write(int x, int y, wchar_t ch)
{
	this->charX[ch] = (short)x;
	this->charY[ch] = (short)y;
	this->screen[y * this->nScreenWidth + x] = ch;
}

void Play::write(int x, int y, const std::wstring& s)
{
	this->textX[s] = (short)x;
	this->textY[s] = (short)y;
	swprintf(&this->screen[y * this->nScreenWidth + x], s.size() + 1, L"%s", s.c_str());
}

void Play::write(int x, int y, const std::string& s)
{
	std::wstring ws = this->ASCII_to_UTF16(s);
	this->textX[ws] = (short)x;
	this->textY[ws] = (short)y;
	swprintf(&this->screen[y * this->nScreenWidth + x], ws.size() + 1, L"%s", ws.c_str());
}

void Play::findDialogue(std::wstring language_name, unsigned chapter_number)
{
	unsigned begin = 0;
	unsigned end = 0;

	std::wstring start = L'@' + std::to_wstring(chapter_number) + L'@';
	std::wstring finish = L'#' + std::to_wstring(chapter_number) + L'#';

	for (unsigned i = 0; i < this->dialogues[language_name].size(); i++)
	{
		if (this->dialogues[language_name][i].find(start) != std::wstring::npos)
		{
			begin = i;
			this->getDialogue(language_name , chapter_number).push_back(this->dialogues[language_name][begin]);
			this->getDialogue(language_name, chapter_number, 0).erase(0, this->getDialogue(language_name, chapter_number, 0).find(start) + start.length());
		}
	}

	for (unsigned i = begin; i < this->dialogues[language_name].size(); i++)
	{
		if (this->dialogues[language_name][i].find(finish) != std::wstring::npos)
		{
			end = i;
			if (end == begin)
			{
				this->getDialogue(language_name, chapter_number, 0).erase(this->getDialogue(language_name, chapter_number, 0).find(finish), finish.length());
			}
			else if (end - begin == 1)
			{
				this->getDialogue(language_name, chapter_number).push_back(this->dialogues[language_name][end]);
				this->getDialogue(language_name, chapter_number, (getDialogue(language_name, chapter_number)).size() - 1).erase(getDialogue(language_name, chapter_number, (getDialogue(language_name, chapter_number)).size() - 1).find(finish), finish.length());
			}
			else if (end - begin > 1)
			{
				for (unsigned j = begin + 1; j < end; j++)
				{
					this->getDialogue(language_name, chapter_number).push_back(dialogues[language_name][j]);
				}
				this->getDialogue(language_name, chapter_number).push_back(this->dialogues[language_name][end]);
				this->getDialogue(language_name, chapter_number, (getDialogue(language_name, chapter_number)).size() - 1).erase(getDialogue(language_name, chapter_number, (getDialogue(language_name, chapter_number)).size() - 1).find(finish), finish.length());
			}
		}
	}
}

void Play::findChoice(std::wstring language_name, unsigned chapter_number, unsigned choice_number)
{
	std::wstring start = L'@' + std::to_wstring(chapter_number) + L'@' + L'(' + std::to_wstring(choice_number) + L')';
	std::wstring finish = L'#' + std::to_wstring(chapter_number) + L'#';

	for (unsigned i = 0; i < this->choices[language_name].size(); i++)
	{
		if (this->choices[language_name][i].find(start) != std::wstring::npos)
		{
			this->getChoice(language_name, chapter_number).push_back(this->choices[language_name][i]);
			this->getChoice(language_name, chapter_number, choice_number - 1).erase(0, this->getChoice(language_name, chapter_number, choice_number - 1).find(start) + start.length());
			this->getChoice(language_name, chapter_number, choice_number - 1).erase(this->getChoice(language_name, chapter_number, choice_number - 1).find(L'&'), this->getChoice(language_name, chapter_number, choice_number - 1).find(finish) + finish.length());
		}
	}
}

void Play::findWay(std::wstring language_name, unsigned chapter_number, unsigned choice_number)
{
	std::wstring start = L'@' + std::to_wstring(chapter_number) + L'@' + L'(' + std::to_wstring(choice_number) + L')';
	std::wstring finish = L'#' + std::to_wstring(chapter_number) + L'#';

	for (unsigned i = 0; i < this->choices[language_name].size(); i++)
	{
		if (this->choices[language_name][i].find(start) != std::wstring::npos)
		{
			this->choiceWay[chapter_number].push_back(this->choices[language_name][i]);
			this->choiceWay[chapter_number][choice_number - 1].erase(0, this->choiceWay[chapter_number][choice_number - 1].find(L'&') + 1);
			this->choiceWay[chapter_number][choice_number - 1].erase(this->choiceWay[chapter_number][choice_number - 1].find(finish), finish.length());
		}
	}
}

void Play::choose()
{
	if (this->menuCursorY == this->textY[choice[0]])
	{
		this->chapter = std::stoi(this->choiceWay[this->chapter][0]);
	}
	else if (this->menuCursorY == this->textY[choice[1]])
	{
		this->chapter = std::stoi(this->choiceWay[this->chapter][1]);
	}
	else if (this->menuCursorY == this->textY[choice[2]])
	{
		this->chapter = std::stoi(this->choiceWay[this->chapter][2]);
	}
	else if (this->menuCursorY == this->textY[choice[3]])
	{
		this->chapter = std::stoi(this->choiceWay[this->chapter][3]);
	}
}

void Play::clearDialogue()
{
	for (unsigned i = 0; i < 30; i++)
	{
		this->shownDialogue[i] = L"";
	}

	this->dialogueRow = 0;
	this->dialogueChar = 0;
}

void Play::reset()
{
	//Dialogue output reset
	this->clearDialogue();
	
	//Setting default attribute to cursor position
	FillConsoleOutputAttribute(this->screenBuffer, 7, this->menuCursorSize, { this->menuCursorX, this->menuCursorY }, &this->dwNumWritten);

	//Resetting cursor position
	this->menuCursorY = this->textY[this->choice[0]];

	//Resetting animation
	this->animation = true;
	this->written = false;
}

void Play::fillMenuOutputAttribute(unsigned attribute_number, unsigned length, short x, short y)
{
	FillConsoleOutputAttribute(this->screenBuffer, attribute_number, length, { x, y }, &this->dwNumWritten);
}

void Play::updateLanguage()
{
	//Creating choices text
	this->choice[0] = this->choices[this->language][0];
	this->choice[1] = this->choices[this->language][1];
	this->choice[2] = this->choices[this->language][2];
	this->choice[3] = this->choices[this->language][3];

	if (this->language == L"EN")
	{
		this->choice[4] = L" Next";
	}
	else if (this->language == L"RU")
	{
		this->choice[4] = L" Далее";
	}

	//Setting choice coord X
	this->textX[this->choice[0]] = this->coordStartX;
	this->textX[this->choice[1]] = this->coordStartX;
	this->textX[this->choice[2]] = this->coordStartX;
	this->textX[this->choice[3]] = this->coordStartX;
	this->textX[this->choice[4]] = this->coordStartX;

	//Setting choice coord Y
	this->textY[this->choice[0]] = 21;
	this->textY[this->choice[1]] = 23;
	this->textY[this->choice[2]] = 25;
	this->textY[this->choice[3]] = 27;
	this->textY[this->choice[4]] = 21;
}

void Play::updateDialogue()
{
	if (this->spawnTimer >= this->spawnTimerMax)
	{
		if (this->dialogueRow < this->getDialogue(this->language, this->chapter).size())
		{
			this->shownDialogue[this->dialogueRow] += this->getDialogue(this->language, this->chapter, this->dialogueRow, this->dialogueChar);

			if ((this->dialogueChar == this->getDialogue(this->language, this->chapter, this->dialogueRow).length() - 1) or (this->getDialogue(this->language, this->chapter, this->dialogueRow).size() == 0))
			{
				this->dialogueChar = 0;
				this->dialogueRow++;
			}
			else
			{
				this->dialogueChar++;
			}
		}

		this->spawnTimer = 0.f;
	}

	this->spawnTimer += 0.5f;
}

void Play::update()
{
	//Setting screen buffer cursor position, coordinates must be within the boundaries of the console screen buffer
	SetConsoleCursorPosition(this->screenBuffer, { short(this->nScreenWidth - 1), short(this->nScreenHeight - 1) });

	//Updating language
	this->updateLanguage();

	if (this->getChoice(this->language, this->chapter, 3) != L"0")
	{
		if (this->menuCursorY < this->textY[choice[0]])
		{
			this->menuCursorY = this->textY[choice[3]];
		}
		else if (this->menuCursorY > this->textY[choice[3]])
		{
			this->menuCursorY = this->textY[choice[0]];
		}
	}
	else if (this->getChoice(this->language, this->chapter, 2) != L"0")
	{
		if (this->menuCursorY < this->textY[choice[0]])
		{
			this->menuCursorY = this->textY[choice[2]];
		}
		else if (this->menuCursorY > this->textY[choice[2]])
		{
			this->menuCursorY = this->textY[choice[0]];
		}
	}
	else if (this->getChoice(this->language, this->chapter, 1) != L"0")
	{
		if (this->menuCursorY < this->textY[choice[0]])
		{
			this->menuCursorY = this->textY[choice[1]];
		}
		else if (this->menuCursorY > this->textY[choice[1]])
		{
			this->menuCursorY = this->textY[choice[0]];
		}
	}
	else
	{
		if (this->menuCursorY < this->textY[choice[0]])
		{
			this->menuCursorY = this->textY[choice[0]];
		}
		else if (this->menuCursorY > this->textY[choice[0]])
		{
			this->menuCursorY = this->textY[choice[0]];
		}
	}

	if (this->written == true)
	{
		this->animation = false;
	}

	//Creating cursor (attribute for cursor)
	if (this->animation == false)
	{
		//Choice 1 disabling
		if (this->chapter == 39 or this->chapter == 157)
		{
			if (this->disabled[1] == true)
			{
				FillConsoleOutputAttribute(this->screenBuffer, 8, this->menuCursorSize, { this->menuCursorX, 21 }, &this->dwNumWritten);
			}
			else
			{
				FillConsoleOutputAttribute(this->screenBuffer, 7, this->menuCursorSize, { this->menuCursorX, 21 }, &this->dwNumWritten);
			}
		}
		else
		{
			FillConsoleOutputAttribute(this->screenBuffer, 7, this->menuCursorSize, { this->menuCursorX, 21 }, &this->dwNumWritten);
		}

		//Choice 2 disabling
		if (this->chapter == 5)
		{
			if (this->disabled[2] == true)
			{
				FillConsoleOutputAttribute(this->screenBuffer, 8, this->menuCursorSize, { this->menuCursorX, 23 }, &this->dwNumWritten);
			}
			else
			{
				FillConsoleOutputAttribute(this->screenBuffer, 7, this->menuCursorSize, { this->menuCursorX, 23 }, &this->dwNumWritten);
			}
		}
		else
		{
			FillConsoleOutputAttribute(this->screenBuffer, 7, this->menuCursorSize, { this->menuCursorX, 23 }, &this->dwNumWritten);
		}
		
		//Choice 3
		FillConsoleOutputAttribute(this->screenBuffer, 7, this->menuCursorSize, { this->menuCursorX, 25 }, &this->dwNumWritten);

		//Choice 4
		FillConsoleOutputAttribute(this->screenBuffer, 7, this->menuCursorSize, { this->menuCursorX, 27 }, &this->dwNumWritten);

		//Cursor
		FillConsoleOutputAttribute(this->screenBuffer, this->menuCursorAttributes, this->menuCursorSize, { this->menuCursorX, this->menuCursorY }, &this->dwNumWritten);
	}
	else
	{
		FillConsoleOutputAttribute(this->screenBuffer, 7, this->menuCursorSize, { this->menuCursorX, 21 }, &this->dwNumWritten);
		FillConsoleOutputAttribute(this->screenBuffer, 7, this->menuCursorSize, { this->menuCursorX, 23 }, &this->dwNumWritten);
		FillConsoleOutputAttribute(this->screenBuffer, 7, this->menuCursorSize, { this->menuCursorX, 25 }, &this->dwNumWritten);
		FillConsoleOutputAttribute(this->screenBuffer, 7, this->menuCursorSize, { this->menuCursorX, 27 }, &this->dwNumWritten);
	}

	if (this->started == true)
	{
		this->updateDialogue();
	}
}

void Play::renderDialogue()
{
	short died = 0;

	if (this->chapter == 0)
	{
		died = 35;
	}

	//Writing dialogue to chars buffer
	if (this->animation == true)
	{
		for (unsigned i = 0; i < this->getDialogue(this->language, this->chapter).size(); i++)
		{
			write(this->coordStartX + died, this->coordStartY + (7 - (this->getDialogue(this->language, this->chapter).size() / 2)) + i, this->shownDialogue[i]);
		}

		if (this->dialogueRow == this->getDialogue(this->language, this->chapter).size() - 1)
		{
			this->written = true;
		}
	}
	else if (this->animation == false)
	{
		for (unsigned i = 0; i < this->getDialogue(this->language, this->chapter).size(); i++)
		{
			write(this->coordStartX + died, this->coordStartY + (7 - (this->getDialogue(this->language, this->chapter).size() / 2)) + i, this->getDialogue(this->language, this->chapter, i));
		}

		this->written = true;
	}
}

void Play::renderChoices()
{
	//Writing choices to chars buffer
	if (this->written == true)
	{
		if (this->getChoice(this->language, this->chapter, 0) != L"0")
		{
			write(this->textX[this->choice[0]], this->textY[this->choice[0]], this->getChoice(this->language, this->chapter, 0));
		}
		else
		{
			write(this->textX[this->choice[0]], this->textY[this->choice[0]], this->choice[4]);
		}

		if (this->getChoice(this->language, this->chapter, 1) != L"0")
		{
			write(this->textX[this->choice[1]], this->textY[this->choice[1]], this->getChoice(this->language, this->chapter, 1));
		}

		if (this->getChoice(this->language, this->chapter, 2) != L"0")
		{
			write(this->textX[this->choice[2]], this->textY[this->choice[2]], this->getChoice(this->language, this->chapter, 2));
		}

		if (this->getChoice(this->language, this->chapter, 3) != L"0")
		{
			write(this->textX[this->choice[3]], this->textY[this->choice[3]], this->getChoice(this->language, this->chapter, 3));
		}
	}
}

void Play::render()
{
	//Setting screen buffer to be active
	SetConsoleActiveScreenBuffer(this->screenBuffer);

	//Clearing screen buffer
	this->clearScreen();

	//Hiding console cursor
	SetConsoleCursorInfo(this->screenBuffer, &this->cursorInfo);

	//Showing dialogue and choices
	this->renderDialogue();
	this->renderChoices();

	//Output chars buffer to the screen buffer
	WriteConsoleOutputCharacterW(this->screenBuffer, this->screen, this->nScreenWidth * this->nScreenHeight, { 0,0 }, &this->dwBytesWritten);
}