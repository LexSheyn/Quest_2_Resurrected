#include "stdafx.h"
#include "Game.h"

//Private functions
void Game::initConsoleWindow()
{
	this->wDesktop = GetDesktopWindow();
	this->wConsole = GetConsoleWindow();

	GetWindowRect(this->wDesktop, &this->desktop);
	GetWindowRect(this->wConsole, &this->console);

	this->dWidth = (unsigned)this->desktop.right;
	this->dHeight = (unsigned)this->desktop.bottom;

	//this->cWidth = (unsigned)this->console.right;
	//this->cHeight = (unsigned)this->console.bottom;

	if ((unsigned)this->desktop.right >= 1280 and (unsigned)this->desktop.right < 1920)
	{
		if ((unsigned)this->desktop.bottom >= 1024 and (unsigned)this->desktop.bottom < 1080)
		{
			this->cWidth = 1220;
			this->cHeight = 630;
		}
	}
	else if ((unsigned)this->desktop.right >= 1920)
	{
		if ((unsigned)this->desktop.bottom >= 1080)
		{
			this->cWidth = 1820;
			this->cHeight = 830;
		}
	}
	
	this->style = GetWindowLong(this->wConsole, GWL_STYLE);

	this->style &= ~WS_MAXIMIZEBOX;
	this->style &= ~WS_SIZEBOX;

	SetWindowLong(this->wConsole, GWL_STYLE, this->style);

	SetWindowPos
	(
		this->wConsole,
		NULL,
		(this->dWidth / 2) - (this->cWidth / 2),
		(this->dHeight / 2) - (unsigned)((float)this->cHeight / 1.6f),
		0,
		0,
		SWP_NOSIZE | SWP_FRAMECHANGED
	);

	SetConsoleTitle(L"Saga of the Noble Barbarian: The Dancing God");
}

void Game::initVariables()
{
	//Keys
	this->keyPressed = false;
	this->escPressed = false;

	//Menu, save/load, settings
	this->opened["menu"] = true;
	this->opened["play"] = false;
	this->opened["save/load"] = false;
	this->opened["settings"] = false;
	this->opened["inventory"] = false;

	//Game over
	this->gameOver = false;

	//Loading enabling
	this->loadEnabled = false;

	//Scenario parts

}

void Game::initMusic()
{
	if (this->settings->getMusicOn() == true)
	{
		this->speaker->playMusicChannel("menu", true, true, "Resources\\music\\menu.mp3");
		this->speaker->playMusicChannel("intro", true, true, "Resources\\music\\intro.mp3");
		this->speaker->playMusicChannel("temple", true, true, "Resources\\music\\temple.mp3");
		this->speaker->playMusicChannel("battle", true, true, "Resources\\music\\battle.mp3");
		this->speaker->playMusicChannel("monster", true, true, "Resources\\music\\monster.mp3");
		this->speaker->playMusicChannel("boss", true, true, "Resources\\music\\boss.mp3");
		this->speaker->playMusicChannel("final", true, true, "Resources\\music\\final.mp3");
	}
	else
	{
		this->speaker->stopMusicChannel("menu");
		this->speaker->stopMusicChannel("intro");
		this->speaker->stopMusicChannel("temple");
		this->speaker->stopMusicChannel("battle");
		this->speaker->stopMusicChannel("monster");
		this->speaker->stopMusicChannel("boss");
		this->speaker->stopMusicChannel("final");
	}
}

//Constructors and Distructors
Game::Game()
{
	this->initConsoleWindow();
	this->initVariables();

	//Init Game objects
	this->menu = new Menu();
	this->play = new Play();
	this->saveLoad = new SaveLoad();
	this->settings = new Settings();
	this->inventory = new Inventory();

	//Set console font size
	this->setConsoleFont();

	//Init music and sounds
	this->speaker = new Speaker();

	//Loading settings
	if (this->settingsData.load(this->settings->getFilePath()) == true)
	{
		this->settingsData.setup
		(
			this->settings->getMusicOn(),
			this->speaker->getMusicVolume(),
			this->settings->getMusicVolumeSize(),
			this->settings->getSoundsOn(),
			this->speaker->getSoundVolume(),
			this->settings->getSoundVolumeSize(),
			this->settings->getLanguage()
		);
		this->menu->setLanguage(this->settings->getLanguage());
		this->play->setLanguage(this->settings->getLanguage());
		this->saveLoad->setLanguage(this->settings->getLanguage());
		this->inventory->setLanguage(this->settings->getLanguage());
	}

	//Setting up music
	this->initMusic();

	//Loading saved game data
	if (this->gameData.update(this->saveLoad->getSaveLoadText(0), this->play->getPathSave(1)) == true)
	{
		this->loadEnabled = true;
	}
	
	if (this->gameData.update(this->saveLoad->getSaveLoadText(1), this->play->getPathSave(2)) == true)
	{
		this->loadEnabled = true;
	}
	
	if (this->gameData.update(this->saveLoad->getSaveLoadText(2), this->play->getPathSave(3)) == true)
	{
		this->loadEnabled = true;
	}
	
	if (this->gameData.update(this->saveLoad->getSaveLoadText(3), this->play->getPathSave(4)) == true)
	{
		this->loadEnabled = true;
	}
}

Game::~Game()
{
	//Delete menu
	delete this->menu;

	//Delete play
	delete this->play;

	//Delete save and load
	delete this->saveLoad;

	//Delete settings
	delete this->settings;

	//Delete inventory
	delete this->inventory;

	//Delete speaker
	delete this->speaker;
}

//Functions
void Game::run()
{
	while (this->gameOver == false)
	{
		if (this->wConsole == GetForegroundWindow())
		{
			this->updateInput();
		}

		this->update();

		this->render();

		this->speaker->updateSystems();

		Sleep(15);
	}
}

void Game::setConsoleFont()
{
	unsigned fontSize = 16;

	if ((unsigned)this->desktop.right >= 1280 and (unsigned)this->desktop.right < 1920)
	{
		if ((unsigned)this->desktop.bottom >= 1024 and (unsigned)this->desktop.bottom < 1080)
		{
			fontSize = 22;
		}
	}
	else if ((unsigned)this->desktop.right >= 1920)
	{
		if ((unsigned)this->desktop.bottom >= 1080)
		{
			fontSize = 32;
		}
	}

	this->menu->setFontHeight(fontSize);
	this->play->setFontHeight(fontSize);
	this->inventory->setFontHeight(fontSize);
	this->saveLoad->setFontHeight(fontSize);
	this->settings->setFontHeight(fontSize);
}

unsigned Game::chooseWay()
{
	//Checking cursor position
	if (this->play->getCursorY() == this->play->getChoiceTextY(this->play->getChoiceText(0))) //Choice 1
	{
		bool redirected = false;

		if (this->play->getChapter() == 39 or this->play->getChapter() == 157)
		{
			for (unsigned i = 0; i < this->inventory->getSize(); i++)
			{
				if (this->inventory->getSlot(i) == this->inventory->getItem(0))
				{
					return 0;
				}
			}
		}
		else if (this->play->getChapter() == 31 or this->play->getChapter() == 33)
		{
			for (unsigned i = 0; i < this->inventory->getSize(); i++)
			{
				if (this->inventory->getSlot(i) == this->inventory->getItem(1))
				{
					this->play->setChapter(this->play->getChapter() + 20);
					redirected = true;
					break;
				}
			}
		}
		else if (this->play->getChapter() == 158)
		{
			for (unsigned i = 0; i < this->inventory->getSize(); i++)
			{
				if (this->inventory->getSlot(i) == this->inventory->getItem(3))
				{
					this->play->setChapter(147);
					redirected = true;
					break;
				}
			}
		}
		else if (this->play->getChapter() == 47)
		{
			for (unsigned i = 0; i < this->inventory->getSize(); i++)
			{
				if (this->inventory->getSlot(i) == this->inventory->getItem(5))
				{
					this->play->setChapter(87);
					redirected = true;
					break;
				}
			}
		}
		else if (this->play->getChapter() == 140)
		{
			for (unsigned i = 0; i < this->inventory->getSize(); i++)
			{
				if (this->inventory->getSlot(i) == this->inventory->getItem(5))
				{
					this->play->setChapter(116);
					redirected = true;
					break;
				}
			}
		}
		else if (this->play->getChapter() == 145)
		{
			for (unsigned i = 0; i < this->inventory->getSize(); i++)
			{
				if (this->inventory->getSlot(i) == this->inventory->getItem(5))
				{
					this->play->setChapter(87);
					redirected = true;
					break;
				}
			}
		}
		else if (this->play->getChapter() == 16)
		{
			if (this->play->getVisited(108) == true)
			{
				this->play->setChapter(144);
				redirected = true;
			}
		}

		if (redirected == false)
		{
			this->play->setChapter(std::stoi(this->play->getChoiceWay(this->play->getChapter(), 0)));
		}
	}
	else if (this->play->getCursorY() == this->play->getChoiceTextY(this->play->getChoiceText(1))) //Choice 2
	{
		bool redirected = false;

		if (this->play->getChapter() == 5)
		{
			for (unsigned i = 0; i < this->inventory->getSize(); i++)
			{
				if (this->inventory->getSlot(i) == this->inventory->getItem(0))
				{
					return 0;
				}
			}
		}
		else if (this->play->getChapter() == 39 or this->play->getChapter() == 157)
		{
			for (unsigned i = 0; i < this->inventory->getSize(); i++)
			{
				if (this->inventory->getSlot(i) == this->inventory->getItem(2))
				{
					this->play->setChapter(139);
					redirected = true;
					break;
				}
			}
		}
		else if (this->play->getChapter() == 75 or this->play->getChapter() == 99)
		{
			if (this->play->getMetGhost() == true)
			{
				this->play->setChapter(79);
				redirected = true;
			}
		}
		else if (this->play->getChapter() == 117)
		{
			if (this->play->getMetGhost() == true)
			{
				this->play->setChapter(127);
				redirected = true;
			}
		}
		else if (this->play->getChapter() == 130)
		{
			if (this->play->getVisited(104) == true)
			{
				this->play->setChapter(144);
				redirected = true;
			}
		}

		if (redirected == false)
		{
			this->play->setChapter(std::stoi(this->play->getChoiceWay(this->play->getChapter(), 1)));
		}
	}
	else if (this->play->getCursorY() == this->play->getChoiceTextY(this->play->getChoiceText(2))) //Choice 3
	{
		bool redirected = false;

		if (this->play->getChapter() == 99)
		{
			for (unsigned i = 0; i < this->inventory->getSize(); i++)
			{
				if (this->inventory->getSlot(i) == this->inventory->getItem(2))
				{
					this->play->setChapter(139);
					redirected = true;
					break;
				}
			}
		}
		else if (this->play->getChapter() == 5)
		{
			if (this->play->getMetGhost() == true)
			{
				this->play->setChapter(59);
				redirected = true;
			}
		}
		else if (this->play->getChapter() == 39)
		{
			if (this->play->getMetGhost() == true)
			{
				this->play->setChapter(79);
				redirected = true;
			}
		}
		else if (this->play->getChapter() == 130)
		{
			if (this->play->getVisited(110) == true)
			{
				this->play->setChapter(144);
				redirected = true;
			}
		}

		if (redirected == false)
		{
			this->play->setChapter(std::stoi(this->play->getChoiceWay(this->play->getChapter(), 2)));
		}
	}
	else if (this->play->getCursorY() == this->play->getChoiceTextY(this->play->getChoiceText(3))) //Choice 4
	{
		this->play->setChapter(std::stoi(this->play->getChoiceWay(this->play->getChapter(), 3)));
	}

	//Resetting music position
	if (this->play->getChapter() == 67 or this->play->getChapter() == 78 or this->play->getChapter() == 85 or this->play->getChapter() == 93
		or this->play->getChapter() == 132 or this->play->getChapter() == 144)
	{
		this->speaker->setMusicChannelPosition("battle", 0);
	}

	if (this->play->getChapter() == 138 or this->play->getChapter() == 8 or this->play->getChapter() == 13 or this->play->getChapter() == 145)
	{
		this->speaker->setMusicChannelPosition("monster", 0);
	}

	return 1;
}

void Game::checkItems()
{
	if (this->play->getChapter() == 75)
	{
		this->inventory->addItem(0); //Millipede claw
		this->play->setDisabled(1, true);
		this->play->setDisabled(2, true);
	}
	else if (this->play->getChapter() == 135)
	{
		this->inventory->addItem(1); //Guard clothes
	}
	else if (this->play->getChapter() == 5)
	{
		this->inventory->addItem(2); //Lucky solid
	}
	else if (this->play->getChapter() == 116)
	{
		this->inventory->addItem(3); //Priest's talisman
	}
	else if (this->play->getChapter() == 2)
	{
		this->inventory->addItem(4); //Priest's chlamys
	}
	else if (this->play->getChapter() == 76)
	{
		this->inventory->addItem(5); //Wand
	}
	else if (this->play->getChapter() == 131)
	{
		this->inventory->addItem(6); //Rope
	}
	else if (this->play->getChapter() == 146)
	{
		this->inventory->addItem(7); //Scroll
	}
	else if (this->play->getChapter() == 0)
	{
		this->inventory->deleteItems();
	}
}

void Game::updateInput()
{
	if ((GetAsyncKeyState((unsigned short)'W') & 0x8000) or (GetAsyncKeyState(VK_UP) & 0x8000))
	{
		if (this->keyPressed == false)
		{
			this->keyPressed = true;
			if (this->opened["menu"] == true)
			{
				if (this->settings->getSoundsOn() == true)
				{
					this->speaker->playSoundChannel("menu_select");
				}
				
				this->menu->setCursorY(this->menu->getCursorY() - 2);
			}
			else if (this->opened["play"] == true and this->opened["inventory"] == false)
			{
				if (this->play->isWritten() == true)
				{
					if (this->settings->getSoundsOn() == true)
					{
						this->speaker->playSoundChannel("menu_select");
					}
					
					this->play->setCursorY(this->play->getCursorY() - 2);
				}
			}
			else if (this->opened["play"] == true and this->opened["inventory"] == true)
			{
				if (this->settings->getSoundsOn() == true)
				{
					this->speaker->playSoundChannel("menu_select");
				}
				
				this->inventory->setCursorY(this->inventory->getCursorY() - 2);
			}
			else if (this->opened["save/load"] == true)
			{
				if (this->settings->getSoundsOn() == true)
				{
					this->speaker->playSoundChannel("menu_select");
				}
				
				this->saveLoad->setCursorY(this->saveLoad->getCursorY() - 2);
			}
			else if (this->opened["settings"] == true)
			{
				if (this->settings->getSoundsOn() == true)
				{
					this->speaker->playSoundChannel("menu_select");
				}
				
				this->settings->setCursorY(this->settings->getCursorY() - 2);
			}
		}
	}
	else if ((GetAsyncKeyState((unsigned short)'S') & 0x8000) or (GetAsyncKeyState(VK_DOWN) & 0x8000))
	{
		if (this->keyPressed == false)
		{
			this->keyPressed = true;
			if (this->opened["menu"] == true)
			{
				if (this->settings->getSoundsOn() == true)
				{
					this->speaker->playSoundChannel("menu_select");
				}
				
				this->menu->setCursorY(this->menu->getCursorY() + 2);
			}
			else if (this->opened["play"] == true and this->opened["inventory"] == false)
			{
				if (this->play->isWritten() == true)
				{
					if (this->settings->getSoundsOn() == true)
					{
						this->speaker->playSoundChannel("menu_select");
					}
					
					this->play->setCursorY(this->play->getCursorY() + 2);
				}
			}
			else if (this->opened["play"] == true and this->opened["inventory"] == true)
			{
				if (this->settings->getSoundsOn() == true)
				{
					this->speaker->playSoundChannel("menu_select");
				}
				
				this->inventory->setCursorY(this->inventory->getCursorY() + 2);
			}
			else if (this->opened["save/load"] == true)
			{
				if (this->settings->getSoundsOn() == true)
				{
					this->speaker->playSoundChannel("menu_select");
				}
				
				this->saveLoad->setCursorY(this->saveLoad->getCursorY() + 2);
			}
			else if (this->opened["settings"] == true)
			{
				if (this->settings->getSoundsOn() == true)
				{
					this->speaker->playSoundChannel("menu_select");
				}
				
				this->settings->setCursorY(this->settings->getCursorY() + 2);
			}
		}
	}
	else if ((GetAsyncKeyState((unsigned short)'A') & 0x8000) or (GetAsyncKeyState(VK_LEFT) & 0x8000))
	{
		if (this->keyPressed == false)
		{
			this->keyPressed = true;
			if (this->opened["settings"] == true)
			{
				if (this->settings->getSoundsOn() == true)
				{
					this->speaker->playSoundChannel("settings_click");
				}
				
				if (this->settings->getCursorY() == this->settings->getTextY(this->settings->getSettingsText(0)))
				{					
					this->speaker->increaseMusicVolume(-0.1f);
					this->settings->increaseMusicVolumeSize(-1);
				}
				else if (this->settings->getCursorY() == this->settings->getTextY(this->settings->getSettingsText(2)))
				{
					this->speaker->increaseSoundVolume(-0.1f);
					this->settings->increaseSoundsVolumeSize(-1);
				}
				else if (this->settings->getCursorY() == this->settings->getTextY(this->settings->getSettingsText(4)))
				{
					this->settings->setLanguage(L"EN");
					this->menu->setLanguage(L"EN");
					this->play->setLanguage(L"EN");
					this->saveLoad->setLanguage(L"EN");
					this->inventory->setLanguage(L"EN");
					this->play->reset();
				}
			}
		}
	}
	else if ((GetAsyncKeyState((unsigned short)'D') & 0x8000) or (GetAsyncKeyState(VK_RIGHT) & 0x8000))
	{
		if (this->keyPressed == false)
		{
			this->keyPressed = true;
			if (this->opened["settings"] == true)
			{
				if (this->settings->getSoundsOn() == true)
				{
					this->speaker->playSoundChannel("settings_click");
				}
				
				if (this->settings->getCursorY() == this->settings->getTextY(this->settings->getSettingsText(0)))
				{					
					this->speaker->increaseMusicVolume(0.1f);
					this->settings->increaseMusicVolumeSize(1);
				}
				else if (this->settings->getCursorY() == this->settings->getTextY(this->settings->getSettingsText(2)))
				{
					this->speaker->increaseSoundVolume(0.1f);
					this->settings->increaseSoundsVolumeSize(1);
				}
				else if (this->settings->getCursorY() == this->settings->getTextY(this->settings->getSettingsText(4)))
				{
					this->settings->setLanguage(L"RU");
					this->menu->setLanguage(L"RU");
					this->play->setLanguage(L"RU");
					this->saveLoad->setLanguage(L"RU");
					this->inventory->setLanguage(L"RU");
					this->play->reset();
				}
			}
		}
	}
	else if (GetAsyncKeyState((unsigned short)'I') & 0x8000)
	{
		if (this->keyPressed == false)
		{
			this->keyPressed = true;
			if (this->opened["play"] == true)
			{
				if (this->opened["inventory"] == false)
				{
					this->opened["inventory"] = true;
				}
				else if (this->opened["inventory"] == true)
				{
					this->opened["inventory"] = false;
				}
			}
		}
	}
	else if ((GetAsyncKeyState(VK_RETURN) & 0x8000))
	{
		if (this->keyPressed == false)
		{
			this->keyPressed = true;
			if (this->opened["menu"] == true)
			{
				if ((this->menu->getCursorY() == this->menu->getTextY(this->menu->getMenuText(0))))
				{
					if (this->settings->getSoundsOn() == true)
					{
						this->speaker->playSoundChannel("play_click");
					}
					
					this->opened["menu"] = false;
					this->opened["play"] = true;
					this->inventory->deleteItems();
					this->speaker->setMusicChannelPosition("intro", 0);
					this->speaker->setMusicChannelPosition("temple", 0);
					this->speaker->setMusicChannelPosition("battle", 0);
					this->speaker->setMusicChannelPosition("catacombs", 0);
					this->speaker->setMusicChannelPosition("monster", 0);
					this->speaker->setMusicChannelPosition("boss", 0);
					this->speaker->setMusicChannelPosition("final", 0);
					this->play->setVisited(104, false);
					this->play->setVisited(108, false);
					this->play->setVisited(110, false);
					this->play->setMetGhost(false);
					this->play->setChapter(600);
					this->play->reset();
				}
				else if ((this->menu->getCursorY() == this->menu->getTextY(this->menu->getMenuText(1))) and this->menu->isDisabled(this->menu->getMenuText(1)) != true)
				{
					if (this->settings->getSoundsOn() == true)
					{
						this->speaker->playSoundChannel("menu_click");
					}
					
					this->opened["menu"] = false;
					this->opened["play"] = true;
				}
				else if ((this->menu->getCursorY() == this->menu->getTextY(this->menu->getMenuText(2))) and this->menu->isDisabled(this->menu->getMenuText(2)) != true)
				{
					if (this->settings->getSoundsOn() == true)
					{
						this->speaker->playSoundChannel("menu_click");
					}
					
					this->opened["menu"] = false;
					this->opened["save/load"] = true;
					this->saveLoad->setMode(L"save");
				}
				else if ((this->menu->getCursorY() == this->menu->getTextY(this->menu->getMenuText(3))) and this->menu->isDisabled(this->menu->getMenuText(3)) != true)
				{
					if (this->settings->getSoundsOn() == true)
					{
						this->speaker->playSoundChannel("menu_click");
					}
					
					this->opened["menu"] = false;
					this->opened["save/load"] = true;
					this->saveLoad->setMode(L"load");
				}
				else if ((this->menu->getCursorY() == this->menu->getTextY(this->menu->getMenuText(4))))
				{
					if (this->settings->getSoundsOn() == true)
					{
						this->speaker->playSoundChannel("menu_click");
					}
					

					this->opened["menu"] = false;
					this->opened["settings"] = true;
				}
				else if (this->menu->getCursorY() == this->menu->getTextY(this->menu->getMenuText(5)))
				{
					this->menu->setFontHeight(16);
					this->play->setFontHeight(16);
					this->inventory->setFontHeight(16);
					this->saveLoad->setFontHeight(16);
					this->settings->setFontHeight(16);

					//Save settings
					this->settingsData.save
					(
						this->settings->getMusicOn(),
						this->speaker->getMusicVolume(),
						this->settings->getMusicVolumeSize(),
						this->settings->getSoundsOn(),
						this->speaker->getSoundVolume(),
						this->settings->getSoundVolumeSize(),
						this->settings->getLanguage(),
						this->settings->getFilePath()
					);

					this->gameOver = true;
				}
			}
			else if (this->opened["play"] == true)
			{
				if (this->play->isWritten() == true)
				{
					if (this->play->getChapter() == 0)
					{
						this->opened["play"] = false;
						this->opened["menu"] = true;
					}
					else if (this->play->getChapter() == 155)
					{
						this->speaker->setMusicChannelPosition("menu", 0);
						this->opened["play"] = false;
						this->opened["menu"] = true;
						this->inventory->deleteItems();
						this->play->setVisited(104, false);
						this->play->setVisited(108, false);
						this->play->setVisited(110, false);
						this->play->setMetGhost(false);
						this->play->setChapter(0);
						this->play->reset();
					}
					else
					{
						if (this->chooseWay() != 0)
						{
							if (this->settings->getSoundsOn() == true)
							{
								this->speaker->playSoundChannel("choice");
							}
							
							this->checkItems();
							this->play->reset();
						}
					}
				}
			}
			else if (this->opened["save/load"] == true)
			{
				if (this->saveLoad->getMode() == L"save")
				{
					if (this->settings->getSoundsOn() == true)
					{
						this->speaker->playSoundChannel("save");
					}
					
				}

				if ((this->saveLoad->getCursorY() == this->saveLoad->getTextY(this->saveLoad->getSaveLoadText(0))))
				{
					if (this->saveLoad->getMode() == L"save")
					{
						this->gameData.save
						(
							this->play->getChapter(),
							this->play->getVisited(104),
							this->play->getVisited(108),
							this->play->getVisited(110),
							this->play->getMetGhost(),
							this->inventory->getSize(),
							this->inventory->getInventory(),
							this->play->getPathSave(1)
						);
						this->gameData.update(this->saveLoad->getSaveLoadText(0), this->play->getPathSave(1));
						this->loadEnabled = true;
					}
					else if (this->saveLoad->getMode() == L"load")
					{
						if (this->gameData.load(this->play->getPathSave(1)) == true)
						{
							this->speaker->setMusicChannelPosition("intro", 0);
							this->speaker->setMusicChannelPosition("temple", 0);
							this->speaker->setMusicChannelPosition("battle", 0);
							this->speaker->setMusicChannelPosition("catacombs", 0);
							this->speaker->setMusicChannelPosition("monster", 0);
							this->speaker->setMusicChannelPosition("boss", 0);
							this->speaker->setMusicChannelPosition("final", 0);
							this->gameData.setup
							(
								this->play->getChapter(),
								this->play->getVisited(104),
								this->play->getVisited(108),
								this->play->getVisited(110),
								this->play->getMetGhost(),
								this->inventory->getInventory(),
								this->saveLoad->getSaveLoadText(0)
							);

							if (this->settings->getSoundsOn() == true)
							{
								this->speaker->playSoundChannel("load");
							}
							
							this->opened["save/load"] = false;
							this->play->setAnimation(false);
							this->opened["play"] = true;
						}
					}
				}
				else if ((this->saveLoad->getCursorY() == this->saveLoad->getTextY(this->saveLoad->getSaveLoadText(1))))
				{
					if (this->saveLoad->getMode() == L"save")
					{
						this->gameData.save
						(
							this->play->getChapter(),
							this->play->getVisited(104),
							this->play->getVisited(108),
							this->play->getVisited(110),
							this->play->getMetGhost(),
							this->inventory->getSize(),
							this->inventory->getInventory(),
							this->play->getPathSave(2)
						);
						this->gameData.update(this->saveLoad->getSaveLoadText(1), this->play->getPathSave(2));
						this->loadEnabled = true;
					}
					else if (this->saveLoad->getMode() == L"load")
					{
						if (this->gameData.load(this->play->getPathSave(2)) == true)
						{
							this->speaker->setMusicChannelPosition("intro", 0);
							this->speaker->setMusicChannelPosition("temple", 0);
							this->speaker->setMusicChannelPosition("battle", 0);
							this->speaker->setMusicChannelPosition("catacombs", 0);
							this->speaker->setMusicChannelPosition("monster", 0);
							this->speaker->setMusicChannelPosition("boss", 0);
							this->speaker->setMusicChannelPosition("final", 0);
							this->gameData.setup
							(
								this->play->getChapter(),
								this->play->getVisited(104),
								this->play->getVisited(108),
								this->play->getVisited(110),
								this->play->getMetGhost(),
								this->inventory->getInventory(),
								this->saveLoad->getSaveLoadText(1)
							);

							if (this->settings->getSoundsOn() == true)
							{
								this->speaker->playSoundChannel("load");
							}
							
							this->opened["save/load"] = false;
							this->play->setAnimation(false);
							this->opened["play"] = true;
						}
					}
				}
				else if ((this->saveLoad->getCursorY() == this->saveLoad->getTextY(this->saveLoad->getSaveLoadText(2))))
				{
					if (this->saveLoad->getMode() == L"save")
					{
						this->gameData.save
						(
							this->play->getChapter(),
							this->play->getVisited(104),
							this->play->getVisited(108),
							this->play->getVisited(110),
							this->play->getMetGhost(),
							this->inventory->getSize(),
							this->inventory->getInventory(),
							this->play->getPathSave(3)
						);
						this->gameData.update(this->saveLoad->getSaveLoadText(2), this->play->getPathSave(3));
						this->loadEnabled = true;
					}
					else if (this->saveLoad->getMode() == L"load")
					{
						if (this->gameData.load(this->play->getPathSave(3)) == true)
						{
							this->speaker->setMusicChannelPosition("intro", 0);
							this->speaker->setMusicChannelPosition("temple", 0);
							this->speaker->setMusicChannelPosition("battle", 0);
							this->speaker->setMusicChannelPosition("catacombs", 0);
							this->speaker->setMusicChannelPosition("monster", 0);
							this->speaker->setMusicChannelPosition("boss", 0);
							this->speaker->setMusicChannelPosition("final", 0);
							this->gameData.setup
							(
								this->play->getChapter(),
								this->play->getVisited(104),
								this->play->getVisited(108),
								this->play->getVisited(110),
								this->play->getMetGhost(),
								this->inventory->getInventory(),
								this->saveLoad->getSaveLoadText(2)
							);

							if (this->settings->getSoundsOn() == true)
							{
								this->speaker->playSoundChannel("load");
							}
							
							this->opened["save/load"] = false;
							this->play->setAnimation(false);
							this->opened["play"] = true;
						}
					}
				}
				else if ((this->saveLoad->getCursorY() == this->saveLoad->getTextY(this->saveLoad->getSaveLoadText(3))))
				{
					if (this->saveLoad->getMode() == L"save")
					{
						this->gameData.save
						(
							this->play->getChapter(),
							this->play->getVisited(104),
							this->play->getVisited(108),
							this->play->getVisited(110),
							this->play->getMetGhost(),
							this->inventory->getSize(),
							this->inventory->getInventory(),
							this->play->getPathSave(4)
						);
						this->gameData.update(this->saveLoad->getSaveLoadText(3), this->play->getPathSave(4));
						this->loadEnabled = true;
					}
					else if (this->saveLoad->getMode() == L"load")
					{
						if (this->gameData.load(this->play->getPathSave(4)) == true)
						{
							this->speaker->setMusicChannelPosition("intro", 0);
							this->speaker->setMusicChannelPosition("temple", 0);
							this->speaker->setMusicChannelPosition("battle", 0);
							this->speaker->setMusicChannelPosition("catacombs", 0);
							this->speaker->setMusicChannelPosition("monster", 0);
							this->speaker->setMusicChannelPosition("boss", 0);
							this->speaker->setMusicChannelPosition("final", 0);
							this->gameData.setup
							(
								this->play->getChapter(),
								this->play->getVisited(104),
								this->play->getVisited(108),
								this->play->getVisited(110),
								this->play->getMetGhost(),
								this->inventory->getInventory(),
								this->saveLoad->getSaveLoadText(3)
							);

							if (this->settings->getSoundsOn() == true)
							{
								this->speaker->playSoundChannel("load");
							}
							
							this->opened["save/load"] = false;
							this->play->setAnimation(false);
							this->opened["play"] = true;
						}
					}
				}
			}
			else if (this->opened["settings"] == true)
			{
				if ((this->settings->getCursorY() == this->settings->getTextY(this->settings->getSettingsText(0))))
				{
					if (this->settings->getSoundsOn() == true)
					{
						this->speaker->playSoundChannel("menu_click");
					}
					

					if (this->settings->getMusicOn() == true)
					{
						//Turning music OFF
						this->settings->turnMusicOn(false);
						this->speaker->stopMusicChannel("menu");
						this->speaker->stopMusicChannel("intro");
						this->speaker->stopMusicChannel("temple");
						this->speaker->stopMusicChannel("battle");
						this->speaker->stopMusicChannel("monster");
						this->speaker->stopMusicChannel("boss");
						this->speaker->stopMusicChannel("final");
					}
					else
					{
						//Turning music ON
						this->settings->turnMusicOn(true);
						this->speaker->playMusicChannel("menu", true, true, "Resources\\music\\menu.mp3");
						this->speaker->playMusicChannel("intro", true, true, "Resources\\music\\intro.mp3");
						this->speaker->playMusicChannel("temple", true, true, "Resources\\music\\temple.mp3");
						this->speaker->playMusicChannel("battle", true, true, "Resources\\music\\battle.mp3");
						this->speaker->playMusicChannel("monster", true, true, "Resources\\music\\monster.mp3");
						this->speaker->playMusicChannel("boss", true, true, "Resources\\music\\boss.mp3");
						this->speaker->playMusicChannel("final", true, true, "Resources\\music\\final.mp3");
					}
				}
				else if ((this->settings->getCursorY() == this->settings->getTextY(this->settings->getSettingsText(2))))
				{
					if (this->settings->getSoundsOn() == true)
					{
						this->settings->turnSoundsOn(false);
					}
					else
					{
						this->settings->turnSoundsOn(true);
					}

					if (this->settings->getSoundsOn() == true)
					{
						this->speaker->playSoundChannel("menu_click");
					}					
				}
				else if ((this->settings->getCursorY() == this->settings->getTextY(this->settings->getSettingsText(3))))
				{
					if (this->settings->getSoundsOn() == true)
					{
						this->speaker->playSoundChannel("menu_click");
					}					
				}
				else if ((this->settings->getCursorY() == this->settings->getTextY(this->settings->getSettingsText(4))))
				{
					//this->speaker->playSoundChannel("menu_click");
				}
			}
		}
	}
	else if ((GetAsyncKeyState(VK_SPACE) & 0x8000))
	{
		if (this->keyPressed == false)
		{
			this->keyPressed = true;
			if (this->opened["play"] == true)
			{
				if (this->play->getAnimation() == true)
				{
					if (this->settings->getSoundsOn() == true)
					{
						this->speaker->playSoundChannel("skip");
					}
					
					this->play->setAnimation(false);
				}
			}
		}
	}
	else
	{
		this->keyPressed = false;
	}

	if (((GetAsyncKeyState(VK_ESCAPE) & 0x8000)))
	{
		if (this->escPressed == false)
		{
			this->escPressed = true;
			if (this->opened["menu"] == false)
			{
				if (this->settings->getSoundsOn() == true)
				{
					this->speaker->playSoundChannel("menu_click");
				}
				
				this->opened["play"] = false;
				this->opened["save/load"] = false;
				this->opened["settings"] = false;
				this->opened["inventory"] = false;
				this->opened["menu"] = true;
			}
			else if (this->menu->isDisabled(this->menu->getMenuText(1)) == false)
			{
				if (this->settings->getSoundsOn() == true)
				{
					this->speaker->playSoundChannel("menu_click");
				}
				
				this->opened["play"] = true;
				this->opened["save/load"] = false;
				this->opened["settings"] = false;
				this->opened["inventory"] = false;
				this->opened["menu"] = false;
			}
		}
	}
	else
	{
		this->escPressed = false;
	}
}

void Game::updateScenario()
{
	//Checking started
	if (this->play->getChapter() == 0)
	{
		this->play->setStarted(false);
	}
	else
	{
		this->play->setStarted(true);
	}

	//Checking disabled
	if (this->play->getChapter() == 0)
	{
		this->menu->setDisabled(this->menu->getMenuText(1), true);
		this->menu->setDisabled(this->menu->getMenuText(2), true);
		this->saveLoad->setDisabled(this->saveLoad->getMenuText(1), true);
		this->settings->setDisabled(this->settings->getMenuText(1), true);
	}
	else
	{
		this->menu->setDisabled(this->menu->getMenuText(1), false);
		this->menu->setDisabled(this->menu->getMenuText(2), false);
		this->saveLoad->setDisabled(this->saveLoad->getMenuText(1), false);
		this->settings->setDisabled(this->settings->getMenuText(1), false);
	}

	//Checking loading
	if (this->loadEnabled == true)
	{
		this->menu->setDisabled(this->menu->getMenuText(3), false);
	}
	else
	{
		this->menu->setDisabled(this->menu->getMenuText(3), true);
	}

	//Checking rooms
	if (this->play->getChapter() == 104)
	{
		this->play->setVisited(104, true);
	}
	else if (this->play->getChapter() == 108)
	{
		this->play->setVisited(108, true);
	}
	else if (this->play->getChapter() == 110)
	{
		this->play->setVisited(110, true);
	}
	else if (this->play->getChapter() == 61 or this->play->getChapter() == 69 or this->play->getChapter() == 97)
	{
		this->play->setMetGhost(true);
	}

	if (this->play->getChapter() == 606 or this->play->getChapter() == 605 or this->play->getChapter() == 604
		or this->play->getChapter() == 603 or this->play->getChapter() == 602 or this->play->getChapter() == 601
		or this->play->getChapter() == 600 or this->play->getChapter() == 126 or this->play->getChapter() == 64
		or this->play->getChapter() == 40 or this->play->getChapter() == 24 or this->play->getChapter() == 2)
	{
		this->part = "intro";
	}
	else if (this->play->getChapter() == 67 or this->play->getChapter() == 85 or this->play->getChapter() == 93
		or this->play->getChapter() == 21 or this->play->getChapter() == 78 or this->play->getChapter() == 144
		or this->play->getChapter() == 32 or this->play->getChapter() == 132 or this->play->getChapter() == 150
		or this->play->getChapter() == 116 or this->play->getChapter() == 140) //And so on...
	{
		this->part = "battle";
	}
	else if (this->play->getChapter() == 148 or this->play->getChapter() == 138 or this->play->getChapter() == 99
		or this->play->getChapter() == 75 or this->play->getChapter() == 17 or this->play->getChapter() == 81
		or this->play->getChapter() == 23 or this->play->getChapter() == 47 or this->play->getChapter() == 87
		or this->play->getChapter() == 13 or this->play->getChapter() == 145) //And so on...
	{
		this->part = "monster";
	}
	else if (this->play->getChapter() == 35 or this->play->getChapter() == 105 or this->play->getChapter() == 147
		or this->play->getChapter() == 153 or this->play->getChapter() == 154 or this->play->getChapter() == 158
		or this->play->getChapter() == 96)
	{
		this->part = "boss";
	}
	else if (this->play->getChapter() == 155)
	{
		this->part = "final";
	}
	else if (this->play->getChapter() == 0 or this->play->getChapter() == 10)
	{
		this->part = "death";
	}
	else
	{
		this->part = "temple";
	}
}

void Game::updateMusic()
{
	if (this->wConsole == GetForegroundWindow())
	{
		if (this->opened["menu"] == true or this->opened["save/load"] == true or this->opened["settings"] == true)
		{
			this->speaker->musicFadeIn("menu");
		}
		else
		{
			this->speaker->musicFadeOut("menu");
		}

		if (this->opened["play"] == true)
		{
			if (this->part == "intro")
			{
				this->speaker->musicFadeIn("intro");
			}
			else
			{
				this->speaker->musicFadeOut("intro");
			}

			if (this->part == "temple")
			{
				this->speaker->musicFadeIn("temple");
			}
			else
			{
				this->speaker->musicFadeOut("temple");
			}

			if (this->part == "battle")
			{
				this->speaker->musicFadeIn("battle");
			}
			else
			{
				this->speaker->musicFadeOut("battle");
			}

			if (this->part == "monster")
			{
				this->speaker->musicFadeIn("monster");
			}
			else
			{
				this->speaker->musicFadeOut("monster");
			}

			if (this->part == "boss")
			{
				this->speaker->musicFadeIn("boss");
			}
			else
			{
				this->speaker->musicFadeOut("boss");
			}

			if (this->part == "final")
			{
				this->speaker->musicFadeIn("final");
			}
			else
			{
				this->speaker->musicFadeOut("final");
			}	
		}
		else
		{
			this->speaker->musicFadeOut("intro");
			this->speaker->musicFadeOut("temple");
			this->speaker->musicFadeOut("battle");
			this->speaker->musicFadeOut("monster");
			this->speaker->musicFadeOut("boss");
			this->speaker->musicFadeOut("final");
		}
	}
	else
	{
		this->speaker->setMusicChannelPaused("menu", true);
		this->speaker->setMusicChannelPaused("intro", true);
		this->speaker->setMusicChannelPaused("temple", true);
		this->speaker->setMusicChannelPaused("battle", true);
		this->speaker->setMusicChannelPaused("monster", true);
		this->speaker->setMusicChannelPaused("boss", true);
		this->speaker->setMusicChannelPaused("final", true);
	}
}

void Game::updateSettings()
{
	//
}

void Game::update()
{
	this->updateScenario();
	this->updateMusic();

	if (this->wConsole == GetForegroundWindow())
	{
		this->menu->update();
		if (this->opened["play"] == true and this->opened["inventory"] == false)
		{
			this->play->update();
		}
		this->saveLoad->update();
		this->settings->update();
		this->inventory->update();
	}
}

void Game::render()
{	
	if (this->opened["menu"] == true)
	{
		this->menu->render();
	}

	if (this->opened["save/load"] == true)
	{
		this->saveLoad->render();
	}

	if (this->opened["settings"] == true)
	{
		this->settings->render();
	}

	if (this->opened["play"] == true and this->opened["inventory"] == false)
	{
		this->play->render();
	}
	
	if (this->opened["play"] == true and this->opened["inventory"] == true)
	{
		this->inventory->render();
	}	

	//this->play->write(1, 1, std::to_string(this->play->getChapter()));
}