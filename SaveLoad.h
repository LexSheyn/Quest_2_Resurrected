#pragma once
class SaveLoad
{
private:
	//Variables
	std::map<std::wstring, bool> disabled;
	std::map<std::wstring, short> textX;
	std::map<std::wstring, short> textY;

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
	short menuCursorSize;
	short saveLoadCursorSize;
	DWORD dwNumWritten;

	std::wstring menuText[30];
	std::wstring saveLoadText[30];
	std::wstring emptyText;
	std::wstring language;

	short distanceX;

	//Mode
	std::wstring mode;

	//Private functions
	void initVariables();
	void initConsoleScreenBuffer();

	void write(wchar_t* buffer, short x, short y, wchar_t ch);
	void write(wchar_t* buffer, short x, short y, const std::wstring& s);

public:
	//Constructors and Distructors
	SaveLoad();
	virtual ~SaveLoad();

	//Accessors
	HANDLE& getHandle();
	bool& isDisabled(std::wstring menu_text);
	const short& getCursorX() const;
	const short& getCursorY() const;
	const DWORD getCursorSize() const;
	const DWORD getCursorAttributes() const;
	short& getTextX(std::wstring menu_text);
	short& getTextY(std::wstring menu_text);
	std::wstring& getMenuText(unsigned text_number);
	std::wstring& getSaveLoadText(int text_number);

	std::wstring& getMode();

	//Modifiers
	void setDisabled(const std::wstring menu_text, const bool set_disabled);
	void setCursorX(const short x);
	void setCursorY(const short y);

	void setLanguage(const std::wstring language_name);

	void setFontHeight(unsigned font_height);

	void setMode(const std::wstring mode_name);

	//Functions
	void clearScreen();

	std::wstring ASCII_to_UTF16(const std::string& str);
	std::wstring returnCurrentTimeAndDate();

	void fillMenuOutputAttribute(int attribute_number, int length, short x, short y);

	void updateLanguage();
	void update();
	void render();
};