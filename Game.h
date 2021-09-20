#pragma once

#include "Menu.h"
#include "Play.h"
#include "SaveLoad.h"
#include "Settings.h"
#include "Inventory.h"

#include "Speaker.h"

#include "Data.h"

class Game
{
private:
	//Vatiables
	bool keyPressed;
	bool escPressed;
	std::map <std::string, bool> opened;
	bool menuOpened;
	bool playOpened;
	bool saveLoadOpened;
	bool settingsOpened;
	bool gameOver;
	bool loadEnabled;
	std::string part;

	//Desktop window
	HWND wDesktop;
	unsigned dWidth;
	unsigned dHeight;
	RECT desktop;

	//Console window
	HWND wConsole;
	DWORD style;
	unsigned cWidth;
	unsigned cHeight;
	RECT console;

	//Menu
	Menu* menu;

	//Play
	Play* play;

	//Save and Load
	SaveLoad* saveLoad;

	//Settings
	Settings* settings;

	//Inventory
	Inventory* inventory;

	//Speaker
	Speaker* speaker;

	//Data
	GameData gameData;
	SettingsData settingsData;
	SettingsData* p1;

	//Private functions
	void initConsoleWindow();
	void initVariables();
	void initMusic();

public:
	//Constructors and Distructors
	Game();
	virtual ~Game();

	//Functions
	void run();

	void setConsoleFont();

	unsigned chooseWay();
	void checkItems();

	void updateInput();
	void updateScenario();
	void updateMusic();
	void updateSettings();

	void update();
	void render();
};