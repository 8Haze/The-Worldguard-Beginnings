#ifndef _MUSIC_MANAGER_H_
#define _MUSIC_MANAGER_H_

#include <string>
#include <map>

#include <SFML/Audio.hpp>

#include "hct/stack.h"

#include "config_manager.h"

class Music_Manager
{
public:

	enum class ID
	{ 
		MENU,
		VERDANT,
		WATER,
		FIRE, 
		ELECTRIC,
		WIND,
		GROUND,
		LIFEFORCE, 
		ICE,
		COUNT
	}; // keep COUNT last

private:

	std::map<ID, std::string> files;
	hct::stack<ID> tracks;
	sf::Music music;

	float volume = 50.f;

	Config_Manager& config_manager;

public:

	Music_Manager(Config_Manager& arg_config_manager);

	float get_volume() const noexcept;

	void set_pause(bool arg);

	void play_track(ID id);
	void pop_track();
	void clear_tracks();
	void change_volume(float offset);

};

#endif //_MUSIC_MANAGER_H_