#include "controls_manager.h"

#include <fstream>
#include <filesystem>

#include "hct/scope_guard.h"

#include "utility.h"

// ----------------------------------------------------------------
// Controls_Manager
// ----------------------------------------------------------------

Controls_Manager::Controls_Manager()
{
	if (!load_saved_bindings())
	{
		load_default_bindings();
		save_changes();
	}
}

bool Controls_Manager::load_saved_bindings()
{
	std::ifstream file;
	const int bindings_count = static_cast<int>(Controls_Manager::ACTION::COUNT);
	int read_data[bindings_count];

	file.open(filename);
	if (!file.is_open())
		return false;
	auto guard = hct::make_guard_exit([&file]() { file.close(); });

	for (int a = 0; a < bindings_count; ++a)
		file >> read_data[a];

	if (file.bad() || file.fail())
		return false;

	for (int a = 0; a < bindings_count; ++a)
		for (int b = a + 1; b < bindings_count; ++b)
			if (read_data[a] == read_data[b])
				return false;

	bindings.emplace(ACTION::LEFT, static_cast<sf::Keyboard::Key>(read_data[0]));
	bindings.emplace(ACTION::RIGHT, static_cast<sf::Keyboard::Key>(read_data[1]));
	bindings.emplace(ACTION::JUMP, static_cast<sf::Keyboard::Key>(read_data[2]));
	bindings.emplace(ACTION::SHOOT, static_cast<sf::Keyboard::Key>(read_data[3]));

	return true;
}

void Controls_Manager::load_default_bindings()
{
	bindings.clear();
	bindings.emplace(ACTION::LEFT, def_left);
	bindings.emplace(ACTION::RIGHT, def_right);
	bindings.emplace(ACTION::JUMP, def_jump);
	bindings.emplace(ACTION::SHOOT, def_shoot);
}

sf::Keyboard::Key Controls_Manager::get_key(ACTION action) const
{
	return bindings.find(action)->second;
}

void Controls_Manager::set_key(ACTION action, sf::Keyboard::Key key)
{
	for (const auto& x : bindings)
		if (x.second == key)
			return;

	bindings.find(action)->second = key;
}

void Controls_Manager::save_changes()
{
	{
		std::ofstream file(filename_temp);
		if (!file.is_open())
			throw std::runtime_error("Controls_Manager::_save_bindings(): cannot create temporary key bindings file");
		auto guard = hct::make_guard_exit([&file]() { file.close(); });
		file << bindings[ACTION::LEFT] << " " << bindings[ACTION::RIGHT] << " " << bindings[ACTION::JUMP] << " " << bindings[ACTION::SHOOT];
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