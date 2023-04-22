#ifndef _SFX_MANAGER_H_
#define _SFX_MANAGER_H_

#include <array>

#include <SFML/Audio.hpp>

#include "resources.h"
#include "config_manager.h"

class SFX_Manager
{
private:

	inline static const unsigned int sound_channels_count = 64;

	std::array<sf::Sound, sound_channels_count> sounds;
	RM_Sounds& rm_sounds;
	Config_Manager& config_manager;
	unsigned int sounds_index = 0;
	float volume;

	void reset_volumes();

public:

	SFX_Manager(RM_Sounds& arg_rm_sounds, Config_Manager& arg_config_manager);

	float get_volume() const;

	void play_sound(RESOURCE_ID::SOUNDS id);
	void change_volume(float offset);

};

#endif // _SFX_MANAGER_H_