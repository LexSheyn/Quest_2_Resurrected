#include "stdafx.h"
#include "Speaker.h"

//Private functions
void Speaker::initVariables()
{
	this->musicVolume = 0.5f;
	this->soundVolume = 0.5f;	

	this->musicFadeStep = 0.007f;

	this->musicPlaying["menu"] = false;

	this->musicFadeRate["menu"] = 1.f;
	this->musicFadeRate["intro"] = 0.f;
	this->musicFadeRate["temple"] = 0.f;
	this->musicFadeRate["battle"] = 0.f;
	this->musicFadeRate["catacombs"] = 0.f;
	this->musicFadeRate["monster"] = 0.f;
	this->musicFadeRate["boss"] = 0.f;
	this->musicFadeRate["final"] = 0.f;
}

void Speaker::initSoundSystem()
{
	FMOD::System_Create(&this->soundSystem);
	this->soundSystem->init(10, FMOD_LOOP_NORMAL, 0); // 10 - number of channels available to use simultaneously
}

void Speaker::initMusicSystem()
{
	FMOD::System_Create(&this->musicSystem);
	this->musicSystem->init(10, FMOD_LOOP_NORMAL, 0);
}

void Speaker::initSound(std::string sound_name, bool looped, const char file_path[256])
{
	if (looped == true)
	{
		this->soundSystem->createSound(file_path, FMOD_LOOP_NORMAL, 0, &this->sounds[sound_name]);
	}
	else
	{
		this->soundSystem->createSound(file_path, FMOD_DEFAULT, 0, &this->sounds[sound_name]);
	}
}

//Constructors and Distructors
Speaker::Speaker()
{
	this->initVariables();
	this->initSoundSystem();
	this->initMusicSystem();
	this->initSound("next", false, "Resources\\sounds\\next.mp3");
	this->initSound("menu_select", false, "Resources\\sounds\\menu_select.mp3");
	this->initSound("menu_click", false, "Resources\\sounds\\menu_click.mp3");
	this->initSound("play_click", false, "Resources\\sounds\\play_click.mp3");
	this->initSound("settings_click", false, "Resources\\sounds\\settings_click.mp3");
	this->initSound("choice", false, "Resources\\sounds\\choice.mp3");
	this->initSound("skip", false, "Resources\\sounds\\skip.mp3");
	this->initSound("save", false, "Resources\\sounds\\save.mp3");
	this->initSound("load", false, "Resources\\sounds\\load.mp3");
	this->initSound("type", false, "Resources\\sounds\\type.mp3");
}

Speaker::~Speaker()
{
	this->soundSystem->release();
	this->musicSystem->release();
}

//Accessors
float& Speaker::getMusicVolume()
{
	return this->musicVolume;
}

bool& Speaker::getMusicPlaying(std::string music_name)
{
	this->channels[music_name]->isPlaying(&this->musicPlaying[music_name]);
	return this->musicPlaying[music_name];
}

float& Speaker::getMusicFadeRate(std::string music_name)
{
	return this->musicFadeRate[music_name];
}

float& Speaker::getSoundVolume()
{
	return this->soundVolume;
}

bool& Speaker::getSoundPlaying(std::string sound_name)
{
	this->channels[sound_name]->isPlaying(&this->soundPlaying[sound_name]);
	return this->soundPlaying[sound_name];
}

//Modifiers
void Speaker::setMusicVolume(std::string music_name, const float value)
{
	this->channels[music_name]->setVolume(value);
}

void Speaker::setMusicVolume(const float value)
{
	this->musicVolume = value;

	if (this->musicVolume > 1.f)
	{
		this->musicVolume = 1.f;
	}
	else if (this->musicVolume < 0.f)
	{
		this->musicVolume = 0.f;
	}
}

void Speaker::increaseMusicVolume(const float value)
{
	this->musicVolume += value;

	if (this->musicVolume > 1.f)
	{
		this->musicVolume = 1.f;
	}
	else if (this->musicVolume < 0.f)
	{
		this->musicVolume = 0.f;
	}
}

void Speaker::musicFade(std::string music_name, float value)
{
	this->musicFadeRate[music_name] += value;

	if (this->musicFadeRate[music_name] > 1.f)
	{
		this->musicFadeRate[music_name] = 1.f;
	}
	else if (this->musicFadeRate[music_name] < 0.f)
	{
		this->musicFadeRate[music_name] = 0.f;
	}
}

void Speaker::setSoundVolume(std::string sound_name, const float value)
{	
	this->channels[sound_name]->setVolume(value);
}

void Speaker::setSoundVolume(const float value)
{
	this->soundVolume = value;

	if (this->soundVolume > 1.f)
	{
		this->soundVolume = 1.f;
	}
	else if (this->soundVolume < 0.f)
	{
		this->soundVolume = 0.f;
	}
}

void Speaker::increaseSoundVolume(const float value)
{
	this->soundVolume += value;

	if (this->soundVolume > 1.f)
	{
		this->soundVolume = 1.f;
	}
	else if (this->soundVolume < 0.f)
	{
		this->soundVolume = 0.f;
	}
}

//Functions
void Speaker::playMusicChannel(std::string music_name, bool looped, const char file_path[256])
{
	if (looped == true)
	{
		this->musicSystem->createStream(file_path, FMOD_LOOP_NORMAL, 0, &this->music[music_name]);
	}
	else
	{
		this->musicSystem->createStream(file_path, FMOD_DEFAULT, 0, &this->music[music_name]);
	}

	this->musicSystem->playSound(this->music[music_name], 0, false, &this->channels[music_name]);
	this->channels[music_name]->setVolume(this->musicVolume);
}

void Speaker::playMusicChannel(std::string music_name, bool looped, bool paused, const char file_path[256])
{
	if (looped == true)
	{
		this->musicSystem->createStream(file_path, FMOD_LOOP_NORMAL, 0, &this->music[music_name]);
	}
	else
	{
		this->musicSystem->createStream(file_path, FMOD_DEFAULT, 0, &this->music[music_name]);
	}

	this->musicSystem->playSound(this->music[music_name], 0, paused, &this->channels[music_name]);
	this->channels[music_name]->setVolume(this->musicVolume);
}

void Speaker::setMusicChannelPaused(std::string music_name, bool paused)
{
	this->channels[music_name]->setPaused(paused);
}

void Speaker::stopMusicChannel(std::string music_name)
{
	this->channels[music_name]->stop();
}

void Speaker::setMusicChannelPosition(std::string music_name, unsigned music_time)
{
	this->channels[music_name]->setPosition(0, FMOD_TIMEUNIT_MS);
}

void Speaker::musicFadeIn(std::string music_name)
{
	if (this->getMusicFadeRate(music_name) != 1)
	{
		this->musicFade(music_name, this->musicFadeStep);
	}

	this->setMusicChannelPaused(music_name, false);
}

void Speaker::musicFadeOut(std::string music_name)
{
	if (this->getMusicFadeRate(music_name) != 0)
	{
		this->musicFade(music_name, -this->musicFadeStep);
		this->setMusicChannelPaused(music_name, false);
	}
	else
	{
		this->setMusicChannelPaused(music_name, true);
	}
}

void Speaker::musicFadeOut(std::string music_name, unsigned music_time)
{
	if (this->getMusicFadeRate(music_name) != 0)
	{
		this->musicFade(music_name, -this->musicFadeStep);
		this->setMusicChannelPaused(music_name, false);
	}
	else
	{
		this->setMusicChannelPaused(music_name, true);
		this->setMusicChannelPosition(music_name, music_time);
	}
}

void Speaker::playSoundChannel(std::string sound_name)
{
	this->channels[sound_name]->setVolume(this->soundVolume);
	this->soundSystem->playSound(this->sounds[sound_name], 0, false, &this->channels[sound_name]);
}

void Speaker::playSoundChannel(std::string sound_name, bool paused)
{
	this->channels[sound_name]->setVolume(this->soundVolume);
	this->soundSystem->playSound(this->sounds[sound_name], 0, paused, &this->channels[sound_name]);
}

void Speaker::setSoundChannelPaused(std::string sound_name, bool paused)
{
	this->channels[sound_name]->setPaused(paused);
}

void Speaker::stopSoundChannel(std::string sound_name)
{
	this->channels[sound_name]->stop();
}

void Speaker::updateSystems()
{	
	//Updating music volume
	this->channels["menu"]->setVolume(this->musicVolume * this->musicFadeRate["menu"]);
	this->channels["intro"]->setVolume(this->musicVolume * this->musicFadeRate["intro"]);
	this->channels["temple"]->setVolume(this->musicVolume * this->musicFadeRate["temple"]);
	this->channels["battle"]->setVolume(this->musicVolume * this->musicFadeRate["battle"]);
	this->channels["catacombs"]->setVolume(this->musicVolume * this->musicFadeRate["catacombs"]);
	this->channels["monster"]->setVolume(this->musicVolume * this->musicFadeRate["monster"]);
	this->channels["boss"]->setVolume(this->musicVolume * this->musicFadeRate["boss"]);
	this->channels["final"]->setVolume(this->musicVolume * this->musicFadeRate["final"]);

	//Updating sounds volume
	this->channels["next"]->setVolume(this->soundVolume);
	this->channels["menu_select"]->setVolume(this->soundVolume);
	this->channels["menu_click"]->setVolume(this->soundVolume);
	this->channels["play_click"]->setVolume(this->soundVolume);
	this->channels["settings_click"]->setVolume(this->soundVolume);
	this->channels["choice"]->setVolume(this->soundVolume);
	this->channels["skip"]->setVolume(this->soundVolume);
	this->channels["save"]->setVolume(this->soundVolume);
	this->channels["load"]->setVolume(this->soundVolume);
	this->channels["type"]->setVolume(this->soundVolume);

	//Updating FMOD
	this->soundSystem->update();
	this->musicSystem->update();
}