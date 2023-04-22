#include "config_manager.h"

#include <fstream>
#include <filesystem>
#include <stdexcept>

#include "hct/scope_guard.h"

// ----------------------------------------------------------------
// Config_Manager
// ----------------------------------------------------------------

Config_Manager::Config_Manager()
{
	restore_defaults(false);
}

void Config_Manager::verify()
{
	bool error = false;

	if (data[data_to_string(DATA::MUSIC_VOL)] > 100) error = true;
	if (data[data_to_string(DATA::SFX_VOL)] > 100) error = true;

	if (error)
		restore_defaults(true);
}

void Config_Manager::restore_defaults(bool restore_file)
{
	data[data_to_string(DATA::RES_WIDTH)] = def_res_width;
	data[data_to_string(DATA::RES_HEIGHT)] = def_res_height;
	data[data_to_string(DATA::MUSIC_VOL)] = def_music_vol;
	data[data_to_string(DATA::SFX_VOL)] = def_sfx_vol;

	if (restore_file)
		save_changes();
}

std::string Config_Manager::data_to_string(DATA id) const
{
	switch (id)
	{
	case(DATA::RES_WIDTH): return "RES_WIDTH";
	case(DATA::RES_HEIGHT): return "RES_HEIGHT";
	case(DATA::MUSIC_VOL): return "MUSIC_VOL";
	case(DATA::SFX_VOL): return "SFX_VOL";
	default: throw std::runtime_error("Config_Manager::_enum_to_string(): Invalid id provided");
	}
}

bool Config_Manager::has_been_loaded() const noexcept
{
	return loaded;
}

unsigned int Config_Manager::get(DATA id) const
{
	return data.at(data_to_string(id));
}

void Config_Manager::update(DATA id, unsigned int value)
{
	data[data_to_string(id)] = value;
}

void Config_Manager::load()
{
	bool error = false;

	{
		std::ifstream file(filename);
		if (!file.is_open())
			error = true;
		auto guard = hct::make_guard_exit([&file]() { if (file.is_open()) file.close(); });

		std::string var_name;
		unsigned int value = 0;

		while (!file.eof() && !file.bad() && !file.fail() && !error)
		{
			var_name.clear();
			std::getline(file, var_name, ' ');
			if (var_name.empty())
				break;

			if (!(file >> value) || !data.contains(var_name))
			{
				error = true;
				break;
			}

			data[var_name] = value;

			file.get();
		}
	}

	if (error)
		restore_defaults(true);

	verify();

	loaded = true;
}

void Config_Manager::save_changes()
{
	{
		std::ofstream file(filename_temp);
		if (!file.is_open())
			throw std::runtime_error("Config_Manager::save_changes(): cannot create temporary config file");
		auto guard = hct::make_guard_exit([&file]() { file.close(); });

		std::string str;
		for (DATA current = static_cast<DATA>(0); current < DATA::COUNT; current = static_cast<DATA>(static_cast<int>(current) + 1))
		{
			str = data_to_string(current);
			file << str << " " << data[str] << "\n";
		}
	}

	std::filesystem::path curr_path = std::filesystem::current_path();
	try
	{
		std::filesystem::remove(curr_path / filename);
		std::filesystem::rename(curr_path / filename_temp, curr_path / filename);
	}
	catch (std::filesystem::filesystem_error&)
	{
		std::filesystem::remove(curr_path / filename_temp);
	}
}