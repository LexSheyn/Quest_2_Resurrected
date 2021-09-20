#pragma once
#include "stdafx.h"

#pragma warning(disable:4996)

struct GameData
{
//Variables:
	//Time and date
	std::string timeAndDate;
	std::string timeAndDateText = "Time and date: ";
	std::wstring saveSlotText = L"xx-xx-xxxx xx:xx:xx";

	//Chapter
	unsigned chapter = 600;
	std::string chapterText = "Chapter: ";

	//Rooms visited and met ghost
	bool visited_104 = false;
	std::string visited_104_text = "104: ";
	bool visited_108 = false;
	std::string visited_108_text = "108: ";
	bool visited_110 = false;
	std::string visited_110_text = "110: ";
	bool metGhost = false;
	std::string metGhostText = "Met ghost: ";

	//Items
	unsigned slotsNumber = 8;
	std::string slotsNumberText = "Slots number: ";
	std::wstring itemSlot[30];
	std::string itemSlotText = "Item slot";

//Functions:
	std::wstring stringToWstring(const std::string& str)
	{
		using convert_typeX = std::codecvt_utf8<wchar_t>;
		std::wstring_convert<convert_typeX, wchar_t> converterX;

		return converterX.from_bytes(str);
	}

	std::string wstringToString(const std::wstring& wstr)
	{
		using convert_typeX = std::codecvt_utf8<wchar_t>;
		std::wstring_convert<convert_typeX, wchar_t> converterX;

		return converterX.to_bytes(wstr);
	}
		
	std::string returnCurrentTimeAndDate()
	{
		time_t rawtime;
		struct tm* timeinfo;
		char buffer[80];

		time(&rawtime);
		timeinfo = localtime(&rawtime);

		strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M:%S", timeinfo);
		//std::string str(buffer);
		std::string str = "";
		str = buffer;

		return str;
	}

	void save(unsigned& chapter_number, bool& visited_104, bool& visited_108, bool& visited_110, bool& met_ghost, unsigned& slots_number, std::wstring item_slot[], std::wstring& file_path)
	{
		//Saving current time and date
		this->timeAndDate = this->returnCurrentTimeAndDate();

		//Saving chapter
		this->chapter = chapter_number;

		//Saving visited rooms and met ghost
		this->visited_104 = visited_104;
		this->visited_108 = visited_108;
		this->visited_110 = visited_110;
		this->metGhost = met_ghost;

		//Saving slots number
		this->slotsNumber = slots_number;

		//Saving inventory
		for (unsigned i = 0; i < this->slotsNumber; i++)
		{
			this->itemSlot[i] = item_slot[i];
		}

		//Writing to file
		std::ofstream out;

		out.open(file_path);

		if (out.is_open())
		{
			out << this->timeAndDateText << this->timeAndDate << "\n";

			out << this->chapterText << this->chapter << "\n";

			out << this->visited_104_text << this->visited_104 << "\n";
			out << this->visited_108_text << this->visited_108 << "\n";
			out << this->visited_110_text << this->visited_110 << "\n";
			out << this->metGhostText << this->metGhost << "\n";

			out << this->slotsNumberText << this->slotsNumber << "\n";

			for (unsigned i = 0; i < this->slotsNumber; i++)
			{
				out << this->itemSlotText + "(" + std::to_string(i) + "): " << this->wstringToString(this->itemSlot[i]) << "\n";
			}
		}
	}

	bool update(std::wstring& save_slot_name, std::wstring& file_path)
	{
		std::ifstream in;

		in.open(file_path);

		if (in.fail())
		{
			return false;
		}

		//Temporary varibales
		int ch = 0;
		std::string str;
		std::vector<std::string> temp;

		while ((ch = in.get()) != EOF)
		{
			if (char(ch) != '\n')
			{
				str += char(ch);
			}
			else
			{
				temp.push_back(str);
				str.clear();
			}
		}
		temp.push_back(str);
		in.close();

		//Loading time and date
		this->timeAndDate = temp[0].erase(0, temp[0].find(this->timeAndDateText) + this->timeAndDateText.length());

		//Setting save slot name
		save_slot_name = L"    " + this->stringToWstring(this->timeAndDate);		

		return true;
	}

	bool load(std::wstring& file_path)
	{
		//Opening file
		std::ifstream in;

		in.open(file_path);

		if (in.fail())
		{
			return false;
		}

		//Temporary variables
		int ch = 0;
		std::string str;
		std::vector<std::string> temp;

		while ((ch = in.get()) != EOF)
		{
			if (char(ch) != '\n')
			{
				str = str + char(ch);
			}
			else
			{
				temp.push_back(str);
				str.clear();
			}
		}
		temp.push_back(str);
		in.close();

		//Loading chapter
		this->chapter = std::stoi(temp[1].erase(0, temp[1].find(this->chapterText) + this->chapterText.length()));

		//Loading visited rooms and met ghost
		this->visited_104 = bool(std::stoi(temp[2].erase(0, temp[2].find(this->visited_104_text) + this->visited_104_text.length())));
		this->visited_108 = bool(std::stoi(temp[3].erase(0, temp[3].find(this->visited_108_text) + this->visited_108_text.length())));
		this->visited_110 = bool(std::stoi(temp[4].erase(0, temp[4].find(this->visited_110_text) + this->visited_110_text.length())));
		this->metGhost = bool(std::stoi(temp[5].erase(0, temp[5].find(this->metGhostText) + this->metGhostText.length())));

		//Loading slots number
		this->slotsNumber = unsigned(std::stoi(temp[6].erase(0, temp[6].find(this->slotsNumberText) + this->slotsNumberText.length())));

		//Loading inventory
		for (unsigned i = 7; i < (this->slotsNumber + 7); i++)
		{
			this->itemSlot[i - 7] = this->stringToWstring(temp[i].erase(0, temp[i].find(this->itemSlotText + "(" + std::to_string(i) + "): ") + this->itemSlotText.length() + 6));
		}

		return true;
	}

	void setup(unsigned& chapter_number, bool& visited_104, bool& visited_108, bool& visited_110, bool& met_ghost, std::wstring item_slot[], std::wstring& save_slot_name)
	{
		//Setting chapter
		chapter_number = this->chapter;

		//Setting visited rooms and met ghost
		visited_104 = this->visited_104;
		visited_108 = this->visited_108;
		visited_110 = this->visited_110;
		met_ghost = this->metGhost;

		//Setting inventory
		for (unsigned i = 0; i < this->slotsNumber; i++)
		{
			item_slot[i] = this->itemSlot[i];
		}
	}
};

struct SettingsData
{
//Variables:
	//Music volume
	bool musicOn = true;
	std::string musicOnText = "Music ON: ";
	float musicVolume = 0.5f;
	std::string musicVolumeText = "Music volume: ";
	short musicVolumeSize = 5;
	std::string musicVolumeSizeText = "Music volume size: ";
	
	//Sound volume
	bool soundsOn = true;
	std::string soundsOnText = "Sounds ON: ";
	float soundVolume = 0.5f;
	std::string soundVolumeText = "Sound volume: ";
	short soundVolumeSize = 5;
	std::string soundVolumeSizeText = "Sound volume size: ";

	//Language
	std::wstring language = L"RU";
	std::string languageText = "Language: ";

//Functions:
	std::wstring stringToWstring(const std::string& str)
	{
		using convert_typeX = std::codecvt_utf8<wchar_t>;
		std::wstring_convert<convert_typeX, wchar_t> converterX;

		return converterX.from_bytes(str);
	}

	std::string wstringToString(const std::wstring& wstr)
	{
		using convert_typeX = std::codecvt_utf8<wchar_t>;
		std::wstring_convert<convert_typeX, wchar_t> converterX;

		return converterX.to_bytes(wstr);
	}

	void save(bool& music_on, float& music_volume, short& music_volume_size, bool& sounds_on, float& sound_volume, short& sound_volume_size, std::wstring& language, std::wstring& file_path)
	{
		//Saving music volume
		this->musicOn = music_on;
		this->musicVolume = music_volume;
		this->musicVolumeSize = music_volume_size;

		//Savinf sound volune
		this->soundsOn = sounds_on;
		this->soundVolume = sound_volume;
		this->soundVolumeSize = sound_volume_size;

		//Saving language
		this->language = language;

		//Writing to file
		std::ofstream out;
				
		out.open(file_path);

		if (out.is_open())
		{
			out << this->musicOnText << std::to_string(this->musicOn) << "\n";
			out << this->musicVolumeText << std::to_string(this->musicVolume) << "\n";
			out << this->musicVolumeSizeText << std::to_string(this->musicVolumeSize) << "\n";

			out << this->soundsOnText << std::to_string(this->soundsOn) << "\n";
			out << this->soundVolumeText << std::to_string(this->soundVolume) << "\n";
			out << this->soundVolumeSizeText << std::to_string(this->soundVolumeSize) << "\n";

			out << this->languageText << this->wstringToString(this->language) << "\n";
		}
	}

	bool load(std::wstring& file_path)
	{
		//Opening file
		std::ifstream in;

		in.open(file_path);

		if (in.fail())
		{
			return false;
		}

		//Temporary variables
		int ch = 0;  //Variable to store the characters
		std::string str;  //String to put the characters in
		std::vector<std::string> temp;

		while ((ch = in.get()) != EOF) //Get character by character until the end of file
		{
			if (char(ch) != '\n')
			{
				str = str + char(ch); //Store character to string if character not equals '\n'
			}
			else
			{
				temp.push_back(str); //If character equals '\n' - put string into the end of vector
				str.clear(); //Clear the string
			}
		}
		temp.push_back(str); //Put the last string into the end of vector
		in.close(); //Close file

		//Loading music volume
		this->musicOn = bool(std::stoi(temp[0].erase(0, temp[0].find(this->musicOnText) + this->musicOnText.length())));
		this->musicVolume = std::stof(temp[1].erase(0, temp[1].find(this->musicVolumeText) + this->musicVolumeText.length()));
		this->musicVolumeSize = short(std::stoi(temp[2].erase(0, temp[2].find(this->musicVolumeSizeText) + this->musicVolumeSizeText.length())));
		
		//Loading sound volume
		this->soundsOn = bool(std::stoi(temp[3].erase(0, temp[3].find(this->soundsOnText) + this->soundsOnText.length())));
		this->soundVolume = std::stof(temp[4].erase(0, temp[4].find(this->soundVolumeText) + this->soundVolumeText.length()));
		this->soundVolumeSize = short(std::stoi(temp[5].erase(0, temp[5].find(this->soundVolumeSizeText) + this->soundVolumeSizeText.length())));

		//Loading language
		this->language = this->stringToWstring(temp[6].erase(0, temp[6].find(this->languageText) + this->languageText.length()));

		return true;
	}

	void setup(bool& music_on, float& music_volume, short& music_volume_size, bool& sounds_on, float& sound_volume, short& sound_volume_size, std::wstring& language)
	{
		//Setting music volume
		music_on = this->musicOn;
		music_volume = this->musicVolume;
		music_volume_size = this->musicVolumeSize;

		//Setting sound volume
		sounds_on = this->soundsOn;
		sound_volume = this->soundVolume;
		sound_volume_size = this->soundVolumeSize;

		//Setting language
		language = this->language;
	}
};