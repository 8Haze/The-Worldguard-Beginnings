#ifndef _LEVEL_OBJECT_ID_H_
#define _LEVEL_OBJECT_ID_H_

#include <string>

enum class LO_TYPE
{
	DECORATIVE_TILE,
	ANIMATED_TILE,
	SOLID_TILE,
	DESTRUCTIBLE_TILE,
	INTERACTIVE_TILE,
	ITEM,
	STATIONARY_ENTITY,
	COLLIDING_ENTITY,
	WALKING_ENTITY,
	COUNT
}; // keep COUNT last

enum class DECORATIVE_TILE_SUBTYPE
{
	REGULAR,
	COUNT
}; // keep COUNT last

enum class ANIMATED_TILE_SUBTYPE
{
	REGULAR,
	COUNT
}; // keep COUNT last

enum class SOLID_TILE_SUBTYPE
{
	REGULAR,
	COUNT
}; // keep COUNT last

enum class DESTRUCTIBLE_TILE_SUBTYPE
{
	REGULAR,
	FRAGILE,
	COUNT
}; // keep COUNT last

enum class INTERACTIVE_TILE_SUBTYPE
{
	HAZARDOUS,
	LIGHT, // removed content
	DARK, // removed content
	INFO_SIGN,
	ICE,
	HOOK,
	COLD_AIR,
	HOT_AIR,
	ANTI_GRAVITY,
	CONVEYOR_BELT,
	WARP,
	FINISH_WARP,
	WATER,
	COUNT
}; // keep COUNT last

enum class ITEM_SUBTYPE
{
	FOOD,
	GEM,
	COIN,
	LIFEUP,
	COUNT
}; // keep COUNT last

enum class STATIONARY_ENTITY_SUBTYPE
{
	TRAMPOLINE,
	WIND_BLASTER,
	MINE,
	LIVING_SPIKES,
	HEALING_PLANT,
	THORN,
	THORN_SPEAR,
	COUNT
}; // keep COUNT last

enum class COLLIDING_ENTITY_SUBTYPE
{
	BOUNCER,
	CANNON,
	FALLING_HAZARD,
	ROCK_DISPOSER,
	ICE_LAUNCHER,
	PRESSURE_ORB,
	BUBBLE_MACHINE,
	BIONIC_RAY_DEVICE,
	ELECTRIC_GATE_DEVICE,
	FLAMETHROWER,
	CASTER,
	VETERAN_CASTER, // removed content
	ELEMENTAL,
	ELDER_ELEMENTAL, // removed content
	ICEBANER,
	CRIMSON_LASHER,
	AQUAVEIL_WEAVER,
	ROCK_HURLER,
	AERIAL_BOMBARDIER,
	JUNGLE_BOOMERANGER,
	ENERGIZED_GALVANIZER,
	COUNT
}; // keep COUNT last

enum class WALKING_ENTITY_SUBTYPE
{
	SCOUTER,
	WARRIOR,
	POWERFUL_WARRIOR, // removed content
	GOLEM,
	RESISTANT_GOLEM, // removed content
	SCORCHER,
	ROAMING_TRAMPOLINE,
	COUNT
}; // keep COUNT last

std::string index_to_interactive_tile_type_name(int index);
std::string index_to_food_type_name(int index);
std::string index_to_gem_color_name(int index);
std::string index_to_coin_color_name(int index);
std::string index_to_lifeup_type_name(int index);

std::string stationary_entity_type_to_name(STATIONARY_ENTITY_SUBTYPE subtype);
std::string colliding_entity_type_to_name(COLLIDING_ENTITY_SUBTYPE subtype);
std::string walking_entity_type_to_name(WALKING_ENTITY_SUBTYPE subtype);

int food_value_by_index(int index);
int gem_value_by_index(int index);
int coin_value_by_index(int index);
int lifeup_value_by_index(int index);

int colliding_entity_value(COLLIDING_ENTITY_SUBTYPE subtype);
int walking_entity_value(WALKING_ENTITY_SUBTYPE subtype);

#endif // _LEVEL_OBJECT_ID_H_