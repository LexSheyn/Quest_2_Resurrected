#pragma once
class Settings
{
private:
	//Variables
	std::map<std::wstring, bool> disabled;
	std::map<std::wstring, short> textX;
	std::map<std::wstring, short> textY;
	bool musicOn;
	bool soundsOn;
	bool animationOn;

	//Settings file path
	std::wstring filePath;

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
	DWORD dwNumWritten;

	std::wstring menuText[30];
	std::wstring settingsText[30];
	std::wstring language;

	short distanceX;

	short mVolumeSizeMax;
	short mVolumeSize;
	short mVolumeX;
	short mVolumeY;

	short sVolumeSizeMax;
	short sVolumeSize;
	short sVolumeX;
	short sVolumeY;

	short languageCursorSize;

	//Private functions
	void initVariables();
	void initConsoleScreenBuffer();

	void write(wchar_t* buffer, short x, short y, wchar_t ch);
	void write(wchar_t* buffer, short x, short y, const std::wstring& s);

public:
	//Constructors and Distructors
	Settings();
	virtual ~Settings();

	//Accessors
	HANDLE& getHandle();
	
	std::wstring& getFilePath();

	std::wstring& getLanguage();

	bool& isDisabled(std::wstring menu_text);

	const short& getCursorX() const;
	const short& getCursorY() const;
	const DWORD getCursorSize() const;
	const DWORD getCursorAttributes() const;

	short& getTextX(std::wstring menu_text);
	short& getTextY(std::wstring menu_text);

	std::wstring& getMenuText(unsigned text_number);
	std::wstring& getSettingsText(int text_number);

	bool& getMusicOn();
	short& getMusicVolumeX();
	short& getMusicVolumeSize();

	bool& getSoundsOn();
	short& getSoundVolumeX();
	short& getSoundVolumeSize();

	bool& getAnimationOn();

	//Modifiers
	void setDisabled(const std::wstring menu_text, const bool set_disabled);
	void setCursorX(const short x);
	void setCursorY(const short y);

	void turnMusicOn(const bool turn_on);
	void setMusicVolumeSize(const short value);
	void increaseMusicVolumeSize(const short value);

	void turnSoundsOn(const bool turn_on);
	void setSoundsVolumeSize(const short value);
	void increaseSoundsVolumeSize(const short value);

	void setLanguage(const std::wstring language_name);

	void setFontHeight(unsigned font_height);

	void turnAnimationOn(const bool turn_on);

	//Functions
	void clearScreen();

	std::wstring ASCII_to_UTF16(const std::string& str);
	std::wstring returnCurrentTimeAndDate();

	void fillMenuOutputAttribute(int attribute_number, int length, short x, short y);

	void updateLanguage();
	void update();
	void render();
};