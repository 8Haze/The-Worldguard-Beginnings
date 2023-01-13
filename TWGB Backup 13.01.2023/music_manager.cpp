#include "music_manager.h"

// ----------------------------------------------------------------
// Music_Manager
// ----------------------------------------------------------------

Music_Manager::Music_Manager(Config_Manager& arg_config_manager) : config_manager(arg_config_manager)
{
	using namespace std::string_literals;
	using pair = std::pair<std::string, sf::Music::TimeSpan>;

	files.emplace(ID::MENU, "resources/music/awake10_megaWall.ogg"s);
	files.emplace(ID::VERDANT, "resources/music/Kinetic Harvest.ogg"s);
	files.emplace(ID::WATER, "resources/music/test2.ogg"s);
	files.emplace(ID::FIRE, "resources/music/Granular Extractor.ogg"s);
	files.emplace(ID::ELECTRIC, "resources/music/Argon Refinery.ogg"s);
	files.emplace(ID::WIND, "resources/music/Krypton Garden.ogg"s);
	files.emplace(ID::GROUND, "resources/music/Neon Mines.ogg"s);
	files.emplace(ID::LIFEFORCE, "resources/music/Xenon Home World.ogg"s);
	files.emplace(ID::ICE, "resources/music/Aurora.ogg"s);

	if (!config_manager.has_been_loaded())
		config_manager.load();

	volume = config_manager.get(Config_Manager::DATA::MUSIC_VOL);
	music.setLoop(true);
	music.setVolume(volume);
}

float Music_Manager::get_volume() const noexcept
{
	return volume;
}

void Music_Manager::set_pause(bool pause)
{
	if (pause)
		music.pause();
	else
		music.play();
}

void Music_Manager::play_track(ID id)
{
	auto file_pair = files.find(id);
	if (file_pair == files.end())
		throw (std::invalid_argument("Music_Manager::play_track: invalid ID provided"));

	if (!tracks.empty() && tracks.top() == file_pair->first)
		return;

	tracks.push(file_pair->first);
	music.stop();
	music.openFromFile(file_pair->second);
	music.play();
}

void Music_Manager::pop_track()
{
	if (tracks.empty())
		return;

	tracks.pop();
	music.stop();

	if (!tracks.empty())
	{
		music.openFromFile(files.find(tracks.top())->second);
		music.play();
	}
}

void Music_Manager::clear_tracks()
{
	music.stop();
	while (!tracks.empty())
		tracks.pop();
}

void Music_Manager::change_volume(float offset)
{
	volume += offset;
	if (volume < 0.f)
		volume = 0.f;
	if (volume > 100.f)
		volume = 100.f;

	music.setVolume(volume);
}