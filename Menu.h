#pragma once
class Menu
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

	//X
	short menuCursorX;
	short coordStartX;
	//Y
	short menuCursorY;
	short coordStartY;

	WORD  menuCursorAttributes;
	DWORD menuCursorSize;
	DWORD dwNumWritten;

	std::wstring menuText[30];
	std::wstring tipText[30];

	//Language
	std::wstring language;

	//Private functions
	void initVariables();
	void initConsoleScreenBuffer();

	void write(wchar_t* buffer, short x, short y, wchar_t ch);
	void write(wchar_t* buffer, short x, short y, const std::wstring& s);

public:
	//Constructors and Distructors
	Menu();
	virtual ~Menu();

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
	
	const unsigned& getDefaultFont() const;

	//Modifiers
	void setDisabled(const std::wstring menu_text, const bool set_disabled);
	void setCursorX(const short x);
	void setCursorY(const short y);

	void setLanguage(const std::wstring language_name);

	void setFontHeight(unsigned font_height);

	//Functions
	void clearScreen();

	std::wstring ASCII_to_UTF16(const std::string& str);
	std::wstring returnCurrentTimeAndDate();

	void fillMenuOutputAttribute(int attribute_number, int length, short x, short y);

	void updateLanguage();
	void update();
	void render();
};

