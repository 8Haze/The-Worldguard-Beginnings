#ifndef _CONFIG_MANAGER_H_
#define _CONFIG_MANAGER_H_

#include <filesystem>
#include <map>

#include <SFML/Audio.hpp>
#include <SFML/System.hpp>

#include "resources.h"

class Config_Manager
{
public:

	enum class DATA 
	{
		RES_WIDTH,
		RES_HEIGHT, 
		MUSIC_VOL, 
		SFX_VOL, 
		COUNT 
	}; // keep COUNT last

private:

	inline static const char* filename = "resources/data/config.cfg";
	inline static const char* filename_temp = "resources/data/config_temp.cfg";

	inline static const unsigned int def_res_width = 1920u;
	inline static const unsigned int def_res_height = 1080u;
	inline static const float def_music_vol = 50.f;
	inline static const float def_sfx_vol = 50.f;

	std::map<std::string, unsigned int> data;
	bool loaded = false;

	std::string data_to_string(DATA id) const;
	void verify();
	void restore_defaults(bool restore_file);

public:

	inline static const std::filesystem::path campaign_levels_directory = "resources/levels/campaign/";
	inline static const std::filesystem::path user_levels_directory = "resources/levels/user/";
	inline static const std::filesystem::path player_data_path = "resources/data/player.bin";
	inline static const std::filesystem::path error_logs_path = "resources/data/error_logs.txt";

	Config_Manager();

	bool has_been_loaded() const noexcept;
	unsigned int get(DATA id) const;

	void update(DATA id, unsigned int value);
	void load();
	void save_changes();

};

#endif // _CONFIG_MANAGER_H_