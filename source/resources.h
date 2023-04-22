#ifndef _RESOURCES_H_
#define _RESOURCES_H_

#include <string>
#include <map>

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "utility.h"

namespace RESOURCE_ID
{
	// rearrange and comment categories
	enum class TEXTURES 
	{
		MENU_BG,
		MAIN_MENU_LOGO,
		GUI_BUTTON, 
		GUI_DIALOGUE_BOX,
		GUI_TEXT_FIELD, 
		GUI_USER_LEVEL_LIST, 
		GUI_BUTTON_USER_LEVEL_LIST,
		GUI_CAMPAIGN_LEVEL_PANEL,
		THEMES_LOGOS,
		TOOLBELT,
		TOOLBELT_INFO_PANEL,
		LEVEL_CREATOR_GRID,
		LCOS_TILE,
		LCOS_TILE_LAYER,
		LCOS_ITEM,
		LCOS_FOOD,
		LCOS_GEM,
		LCOS_COIN,
		LCOS_LIFEUP,
		LCOS_ENTITY,
		LCOS_LIGHT,
		FOOD,
		GEM,
		COIN,
		LIFEUP,
		ITEM_FADEOUT,
		BULLET, 
		BULLET_FADEOUT,
		WAVE,
		WAVE_FADEOUT,
		MASS,
		MASS_FADEOUT,
		ROCK, 
		ROCK_CRACK, 
		ICE_CHUNK,
		ICE_CHUNK_CRACK,
		BALL,
		BALL_CRACK,
		BUBBLE,
		BUBBLE_POP,
		BOOMERANG,
		THORN_SPEAR,
		BIONIC_RAY,
		ELECTRIC_GATE,
		FLAMETHROWER_FLAME,
		HEALING_BLAST, 
		PRESSURE_BLAST,
		VERDANT_TILESET,
		WATER_TILESET, 
		FIRE_TILESET,
		ELECTRIC_TILESET, 
		WIND_TILESET, 
		GROUND_TILESET,
		LIFEFORCE_TILESET,
		ICE_TILESET, 
		ANIMATED_TILESET, 
		INTERACTIVE_TILESET,
		VERDANT_BACKGROUND, 
		WATER_BACKGROUND, 
		FIRE_BACKGROUND, 
		ELECTRIC_BACKGROUND, 
		WIND_BACKGROUND, 
		GROUND_BACKGROUND, 
		LIFEFORCE_BACKGROUND,
		ICE_BACKGROUND,
		TRAMPOLINE, 
		CANNON, 
		BOUNCER, 
		WIND_BLASTER, 
		MINE, 
		LIVING_SPIKES,
		THORN,
		THORN_SPEAR_ROOT,
		FALLING_HAZARD,
		ROCK_DISPOSER,
		ICE_LAUNCHER,
		BUBBLE_MACHINE,
		BIONIC_RAY_DEVICE,
		ELECTRIC_GATE_DEVICE,
		FLAMETHROWER,
		ROAMING_TRAMPOLINE,
		SCOUTER,
		WARRIOR,
		CASTER,
		GOLEM,
		ELEMENTAL,
		JUNGLE_BOOMERANGER,
		AQUAVEIL_WEAVER,
		SCORCHER,
		ENERGIZED_GALVANIZER,
		AERIAL_BOMBARDIER,
		ROCK_HURLER,
		CRIMSON_LASHER,
		ICEBANER,
		HEALING_PLANT, 
		PRESSURE_ORB,
		POWER_CELLS, 
		EXPLOSION_32,
		EXPLOSION_48,
		SCORCHER_AFTERMATH,
		PLAYER_LIFEBAR_FRAME,
		PLAYER_LIFEBAR_FILLING,
		TEMPERATURE_BAR_FRAME,
		TEMPERATURE_BAR_FILLING,
		NAZAR,
	};

	enum class SOUNDS 
	{ 
		GUI_BUTTON_SELECT, 
		GUI_BUTTON_EXECUTE,
		GUI_TEXT_ENTERED
	};

	enum class FONTS 
	{
		MAIN 
	};
}

template <typename T_Derived, typename T_ID, typename T_Resource, typename T_Storage = std::string>
class Resource_Manager : private No_Copy_No_Move
{
protected:

	std::map<T_ID, std::unique_ptr<T_Resource>> cache;
	std::map<T_ID, T_Storage> storage;

public:

	Resource_Manager() = default;

	T_Resource& get(T_ID id)
	{
		auto file_pair = storage.find(id);
		if (file_pair == storage.end())
			throw (std::invalid_argument("Resource_Manager::get: invalid ID provided"));

		auto cache_pair = cache.find(id);
		if (cache_pair == cache.end())
			return static_cast<T_Derived*>(this)->load_resource(file_pair->second, id);
		return *cache_pair->second;
	}

	void free(T_ID id)
	{
		if (cache.find(id) != cache.end())
			cache.erase(id);
	}

};

class RM_Textures : public Resource_Manager<RM_Textures, RESOURCE_ID::TEXTURES, sf::Texture>
{
private:

	inline static const std::string path = "resources/graphics/";

	friend class Resource_Manager<RM_Textures, RESOURCE_ID::TEXTURES, sf::Texture>;

	sf::Texture& load_resource(std::string file, RESOURCE_ID::TEXTURES id);

public:

	RM_Textures();

};

class RM_Sounds : public Resource_Manager<RM_Sounds, RESOURCE_ID::SOUNDS, sf::SoundBuffer>
{
private:

	inline static const std::string path = "resources/sounds/";

	friend class Resource_Manager<RM_Sounds, RESOURCE_ID::SOUNDS, sf::SoundBuffer>;

	sf::SoundBuffer& load_resource(std::string file, RESOURCE_ID::SOUNDS id);

public:

	RM_Sounds();

};

class RM_Fonts : public Resource_Manager<RM_Fonts, RESOURCE_ID::FONTS, sf::Font>
{
private:

	inline static const std::string path = "resources/fonts/";

	friend class Resource_Manager<RM_Fonts, RESOURCE_ID::FONTS, sf::Font>;

	sf::Font& load_resource(std::string file, RESOURCE_ID::FONTS id);

public:

	RM_Fonts();

};

struct Resources
{
	RM_Textures textures;
	RM_Sounds sounds;
	RM_Fonts fonts;
};

#endif // _RESOURCES_H_