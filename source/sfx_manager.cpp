#include "sfx_manager.h"

// ----------------------------------------------------------------
// SFX_Manager
// ----------------------------------------------------------------

SFX_Manager::SFX_Manager(RM_Sounds& arg_rm_sounds, Config_Manager& arg_config_manager) :
	rm_sounds(arg_rm_sounds), config_manager(arg_config_manager)
{
	if (!config_manager.has_been_loaded())
		config_manager.load();

	volume = config_manager.get(Config_Manager::DATA::SFX_VOL);
	reset_volumes();
}

void SFX_Manager::reset_volumes()
{
	for (auto& x : sounds)
		x.setVolume(volume);
}

float SFX_Manager::get_volume() const
{
	return volume;
}

void SFX_Manager::play_sound(RESOURCE_ID::SOUNDS id)
{
	sounds[sounds_index].setBuffer(rm_sounds.get(id));
	sounds[sounds_index].play();
	if (++sounds_index >= sound_channels_count)
		sounds_index = 0;
}

void SFX_Manager::change_volume(float offset)
{
	volume += offset;
	if (volume < 0.f)
		volume = 0.f;
	if (volume > 100.f)
		volume = 100.f;

	reset_volumes();
}