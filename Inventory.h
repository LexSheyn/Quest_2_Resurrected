#pragma once
class Inventory
{
private:
	//Variables
	std::map<std::wstring, std::wstring> pathTips;

	std::map<std::wstring, std::vector<std::wstring>> tips;
	std::map<std::wstring, std::vector<std::wstring>> tipEN;
	std::map<std::wstring, std::vector<std::wstring>> tipRU;

	std::wstring itemEN[30];
	std::wstring itemRU[30];

	std::map<std::wstring, bool> disabled;
	std::map<std::wstring, short> textX;
	std::map<std::wstring, short> textY;

	std::wstring itemSlot[30];

	unsigned tipRow;
	unsigned tipChar;

	unsigned slots;
	std::wstring emptySlot;

	std::wstring EN;
	std::wstring RU;

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
	short cursorX;
	short coordStartX;
	//Y
	short cursorY;
	short coordStartY;

	WORD  cursorAttributes;
	DWORD cursorSize;
	DWORD dwNumWritten;

	//Language
	std::wstring language;

	//Private functions
	void initVariables();

	void initFiles();
	void initTips(std::wstring language_name);
	void initItems();

	void initConsoleScreenBuffer();

	void write(wchar_t* buffer, int x, int y, wchar_t ch);
	void write(wchar_t* buffer, int x, int y, const std::wstring& s);

public:
	//Constructors and Distructors
	Inventory();
	virtual ~Inventory();

	//Accessors
	std::vector<std::wstring>& getTip(std::wstring language_name, std::wstring item_name);
	std::wstring& getTip(std::wstring language_name, std::wstring item_name, unsigned row_number);
	wchar_t& getTip(std::wstring language_name, std::wstring item_name, unsigned row_number, unsigned char_number);

	bool& isDisabled(std::wstring menu_text);
	const short& getCursorX() const;
	const short& getCursorY() const;
	const DWORD getCursorSize() const;
	const DWORD getCursorAttributes() const;
	short& getTextX(std::wstring menu_text);
	short& getTextY(std::wstring menu_text);
	std::wstring* getInventory();
	std::wstring& getSlot(unsigned text_number);
	std::wstring& getItem(unsigned item_number);
	unsigned& getSize();

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

	void findTip(std::wstring language_name, std::wstring item_name);

	void fillMenuOutputAttribute(int attribute_number, int length, short x, short y);

	void addItem(std::wstring item_name);
	void addItem(unsigned item_number);
	void deleteItems();

	void updateLanguage();
	void update();

	void renderTip();
	void render();
};

