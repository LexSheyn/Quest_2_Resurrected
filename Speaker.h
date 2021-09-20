#pragma once
class Speaker
{
private:
	//Variables
	float musicVolume;
	float soundVolume;

	float musicFadeStep;

	//Sound system
	FMOD_RESULT result;
	FMOD::System* soundSystem;
	FMOD::System* musicSystem;

	//Sounds
	std::map <std::string, FMOD::Sound*> sounds;
	std::map <std::string, bool> soundPlaying;

	//Music
	std::map <std::string, FMOD::Sound*> music;
	std::map <std::string, bool> musicPlaying;
	std::map <std::string, float> musicFadeRate;

	//Channels
	std::map <std::string, FMOD::Channel*> channels;

	//Private functions
	void initVariables();
	void initSoundSystem();
	void initMusicSystem();
	void initSound(std::string sound_name, bool looped, const char file_path[256]);

public:
	//Constructors and Distructors
	Speaker();
	virtual ~Speaker();

	//Accessors
	float& getMusicVolume();
	bool& getMusicPlaying(std::string music_name);
	float& getMusicFadeRate(std::string music_name);

	float& getSoundVolume();
	bool& getSoundPlaying(std::string sound_name);

	//Modifiers
	void setMusicVolume(std::string music_name, const float value);
	void setMusicVolume(const float value);
	void increaseMusicVolume(const float value);
	void musicFade(std::string music_name, float value);

	void setSoundVolume(std::string sound_name, const float value);
	void setSoundVolume(const float value);
	void increaseSoundVolume(const float value);
	
	//Functions
	void playMusicChannel(std::string music_name, bool looped, const char file_path[256]);
	void playMusicChannel(std::string music_name, bool looped, bool paused, const char file_path[256]);
	void setMusicChannelPaused(std::string music_name, bool paused);
	void stopMusicChannel(std::string music_name);
	void setMusicChannelPosition(std::string music_name, unsigned music_time);
	void musicFadeIn(std::string music_name);
	void musicFadeOut(std::string music_name);
	void musicFadeOut(std::string music_name, unsigned music_time);

	void playSoundChannel(std::string sound_name);
	void playSoundChannel(std::string sound_name, bool paused);
	void setSoundChannelPaused(std::string sound_name, bool paused);
	void stopSoundChannel(std::string sound_name);

	void updateSystems();
};