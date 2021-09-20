#pragma once
class Play
{
private:
	//Variables
	std::map<std::wstring, std::wstring> pathDialogues;
	std::map<std::wstring, std::wstring> pathChoices;
	std::map<unsigned, std::wstring> pathSave;
	std::map<std::wstring, std::vector<std::wstring>> dialogues;
	std::map<std::wstring, std::vector<std::wstring>> choices;

	std::map<unsigned, std::vector<std::wstring>> dialogueEN;
	std::map<unsigned, std::vector<std::wstring>> dialogueRU;

	std::map<unsigned, std::vector<std::wstring>> choiceEN;
	std::map<unsigned, std::vector<std::wstring>> choiceRU;

	std::map<unsigned, std::vector<std::wstring>> choiceWay;
	
	std::wstring shownDialogue[30];
	std::wstring choice[5];

	unsigned chapter;
	std::map<unsigned, bool> visited;
	bool metGhost;

	unsigned dialogueRow;
	unsigned dialogueChar;

	bool animation;
	bool written;

	std::map<unsigned, bool> disabled;

	std::map<std::wstring, short> textX;
	std::map<std::wstring, short> textY;

	std::map<wchar_t, short> charX;
	std::map<wchar_t, short> charY;

	//Screen buffer
	short nScreenWidth;
	short nScreenHeight;
	wchar_t* screen;

	HANDLE screenBuffer;
	DWORD dwBytesWritten;
	CONSOLE_SCREEN_BUFFER_INFO csbiScreenInfo;
	CONSOLE_CURSOR_INFO cursorInfo;
	CONSOLE_FONT_INFOEX cfi;
	unsigned cDefaultFontHeight;

	short menuCursorX;
	short coordStartX;
	short menuCursorY;
	short coordStartY;

	WORD  menuCursorAttributes;
	DWORD menuCursorSize;
	DWORD dwNumWritten;

	std::wstring language;

	//Timer
	float spawnTimerMax;
	float spawnTimer;

	//Game start
	bool started;

	//Private functions
	void initFiles();
	void initDialogues(std::wstring language_name);
	void initChoices(std::wstring language_name);

	void initVariables();
	void initConsoleScreenBuffer();

public:
	//Constructors and Distructors
	Play();
	virtual ~Play();

	//Accessors
	HANDLE& getHandle();

	std::wstring& getPathSave(unsigned save_number);

	std::vector<std::wstring>& getDialogues(std::wstring language_name);
	std::vector<std::wstring>& getDialogue(std::wstring language_name, unsigned chapter_number);
	std::wstring& getDialogue(std::wstring language_name, unsigned chapter_number, unsigned row_number);
	wchar_t& getDialogue(std::wstring language_name, unsigned chapter_number, unsigned row_number, unsigned char_number);

	std::vector<std::wstring>& getChoices(std::wstring language_name);
	std::vector<std::wstring>& getChoice(std::wstring language_name, unsigned chapter_number);
	std::wstring& getChoice(std::wstring language_name, unsigned chapter_number, unsigned row_number);
	wchar_t& getChoice(std::wstring language_name, unsigned chapter_number, unsigned row_number, unsigned char_number);

	bool& isDisabled(unsigned choice_number);

	const short& getCursorX() const;
	const short& getCursorY() const;
	const DWORD getCursorSize() const;
	const DWORD getCursorAttributes() const;

	short& getChoiceTextX(std::wstring choice_text);
	short& getChoiceTextY(std::wstring choice_text);
	std::wstring& getChoiceText(std::wstring language_name , unsigned choice_number);
	std::wstring& getChoiceText(unsigned choice_number);

	short& getCharX(wchar_t character);
	short& getCharY(wchar_t character);
	wchar_t& getDialogueChar(std::wstring language_name, unsigned row_number, unsigned char_number);

	const std::wstring& getLanguage() const;

	const bool& isWritten() const;
	const bool& getAnimation() const;
	
	unsigned& getChapter();
	bool& getVisited(unsigned chapter_number);
	bool& getMetGhost();
	std::wstring& getChoiceWay(unsigned chapter_number, unsigned wstring_number);
	const bool& getStarted() const;

	//Modifiers
	void setDisabled(const unsigned choice_number, const bool disable);

	void setCursorX(const short x);
	void setCursorY(const short y);

	void setLanguage(const std::wstring language_name);

	void setFontHeight(unsigned font_height);

	void setAnimation(const bool skip);
	void setWritten(const bool set_written);

	void setChapter(unsigned chapter_number);
	void setVisited(unsigned chapter_number, const bool set_visited);
	void setMetGhost(const bool set_met);
	void setStarted(bool game_started);

	//Functions
	void clearScreen();

	std::wstring ASCII_to_UTF16(const std::string& str);
	std::wstring returnCurrentTimeAndDate();

	void write(int x, int y, wchar_t ch);
	void write(int x, int y, const std::wstring& s);
	void write(int x, int y, const std::string& s);

	void findDialogue(std::wstring language_name, unsigned chapter_number);
	void findChoice(std::wstring language_name, unsigned chapter_number, unsigned choice_number);
	void findWay(std::wstring language_name, unsigned chapter_number, unsigned choice_number);

	void choose();
	void clearDialogue();
	void reset();

	void fillMenuOutputAttribute(unsigned attribute_number, unsigned length, short x, short y);

	void updateLanguage();
	void updateDialogue();
	void update();

	void renderDialogue();
	void renderChoices();
	void render();
};