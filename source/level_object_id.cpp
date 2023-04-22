#include "level_object_id.h"

#include <stdexcept>

std::string index_to_interactive_tile_type_name(int index)
{
	switch (index)
	{
	case (0): return "Hazardous";
	case (1): return "Light";
	case (2): return "Dark";
	case (3): return "Info Sign";
	case (4): return "Ice";
	case (5): return "Hook";
	case (6): return "Cold Air";
	case (7): return "Hot Air";
	case (8): return "Anti-Gravity";
	case (9): return "Conveyor Belt";
	case (10): return "Warp";
	case (11): return "Finish Warp";
	case (12): return "Water";
	default: throw std::runtime_error("index_to_interactive_tile_type: index out of range");
	}
}

std::string index_to_food_type_name(int index)
{
	switch (index)
	{
	case (0): return "Apple";
	case (1): return "Cherry";
	case (2): return "Tomato";
	case (3): return "Orange";
	case (4): return "Lemon";
	case (5): return "Pear";
	case (6): return "Light Cupcake";
	case (7): return "Ice Cube";
	case (8): return "Cherry Soda";
	case (9): return "Stawberry";
	case (10): return "Lettuce";
	case (11): return "Cheese";
	case (12): return "Carrot";
	case (13): return "Ice Cream";
	case (14): return "Lime";
	case (15): return "Grapes";
	case (16): return "Intense Cupcake";
	case (17): return "Banana";
	case (18): return "Orange Soda";
	case (19): return "Watermelon";
	case (20): return "Sandwich";
	case (21): return "Burger";
	case (22): return "Pizza";
	case (23): return "Drumstick";
	case (24): return "Light Cake";
	case (25): return "Dark Cake";
	case (26): return "Cream Cupcake";
	case (27): return "Ham";
	case (28): return "Iced Soda";
	case (29): return "Brown Sandwich";
	default: throw std::runtime_error("index_to_food_name: index out of range");
	}
}

std::string index_to_gem_color_name(int index)
{
	switch (index)
	{
	case (0): return "Red";
	case (1): return "Green";
	case (2): return "Blue";
	case (3): return "Purple";
	case (4): return "Light Blue";
	case (5): return "Orange";
	case (6): return "Magenta";
	case (7): return "Yellow";
	default: throw std::runtime_error("index_to_gem_color: index out of range");
	}
}

std::string index_to_coin_color_name(int index)
{
	switch (index)
	{
	case (0): return "Silver";
	case (1): return "Gold";
	default: throw std::runtime_error("index_to_coin_color: index out of range");
	}
}

std::string index_to_lifeup_type_name(int index)
{
	switch (index)
	{
	case (0): return "Normal";
	case (1): return "Golden";
	default: throw std::runtime_error("index_to_lifeup_type: index out of range");
	}
}

std::string stationary_entity_type_to_name(STATIONARY_ENTITY_SUBTYPE subtype)
{
	switch (subtype)
	{
	case (STATIONARY_ENTITY_SUBTYPE::TRAMPOLINE): return "Trampoline";
	case (STATIONARY_ENTITY_SUBTYPE::WIND_BLASTER): return "Wind Blaster";
	case (STATIONARY_ENTITY_SUBTYPE::MINE): return "Mine";
	case (STATIONARY_ENTITY_SUBTYPE::LIVING_SPIKES): return "Living Spikes";
	case (STATIONARY_ENTITY_SUBTYPE::HEALING_PLANT): return "Healing Plant";
	case (STATIONARY_ENTITY_SUBTYPE::THORN): return "Thorn";
	case (STATIONARY_ENTITY_SUBTYPE::THORN_SPEAR): return "Thorn Spear";
	default: throw std::runtime_error("stationary_entity_type_to_name: subtype out of range");
	}
}

std::string colliding_entity_type_to_name(COLLIDING_ENTITY_SUBTYPE subtype)
{
	switch (subtype)
	{
	case (COLLIDING_ENTITY_SUBTYPE::BOUNCER): return "Bouncer";
	case (COLLIDING_ENTITY_SUBTYPE::CANNON): return "Cannon";
	case (COLLIDING_ENTITY_SUBTYPE::FALLING_HAZARD): return "Falling Hazard";
	case (COLLIDING_ENTITY_SUBTYPE::ROCK_DISPOSER): return "Rock Disposer";
	case (COLLIDING_ENTITY_SUBTYPE::ICE_LAUNCHER): return "Ice Launcher";
	case (COLLIDING_ENTITY_SUBTYPE::PRESSURE_ORB): return "Pressure Orb";
	case (COLLIDING_ENTITY_SUBTYPE::BUBBLE_MACHINE): return "Bubble Machine";
	case (COLLIDING_ENTITY_SUBTYPE::BIONIC_RAY_DEVICE): return "Bionic Ray Device";
	case (COLLIDING_ENTITY_SUBTYPE::ELECTRIC_GATE_DEVICE): return "Electric Gate Device";
	case (COLLIDING_ENTITY_SUBTYPE::FLAMETHROWER): return "Flamethrower";
	case (COLLIDING_ENTITY_SUBTYPE::CASTER): return "Caster";
	case (COLLIDING_ENTITY_SUBTYPE::ELEMENTAL): return "Elemental";
	case (COLLIDING_ENTITY_SUBTYPE::ICEBANER): return "Icebaner";
	case (COLLIDING_ENTITY_SUBTYPE::CRIMSON_LASHER): return "Crimson Lasher";
	case (COLLIDING_ENTITY_SUBTYPE::AQUAVEIL_WEAVER): return "Aquaveil Weaver";
	case (COLLIDING_ENTITY_SUBTYPE::ROCK_HURLER): return "Rock Hurler";
	case (COLLIDING_ENTITY_SUBTYPE::AERIAL_BOMBARDIER): return "Aerial Bombardier";
	case (COLLIDING_ENTITY_SUBTYPE::JUNGLE_BOOMERANGER): return "Jungle Boomeranger";
	case (COLLIDING_ENTITY_SUBTYPE::ENERGIZED_GALVANIZER): return "Energized Galvanizer";
	default: throw std::runtime_error("colliding_entity_type_to_name: subtype out of range");
	}
}

std::string walking_entity_type_to_name(WALKING_ENTITY_SUBTYPE subtype)
{
	switch (subtype)
	{
	case (WALKING_ENTITY_SUBTYPE::SCOUTER): return "Scouter";
	case (WALKING_ENTITY_SUBTYPE::WARRIOR): return "Warrior";
	case (WALKING_ENTITY_SUBTYPE::GOLEM): return "Golem";
	case (WALKING_ENTITY_SUBTYPE::SCORCHER): return "Scorcher";
	case (WALKING_ENTITY_SUBTYPE::ROAMING_TRAMPOLINE): return "Roaming Trampoline";
	default: throw std::runtime_error("walking_entity_type_to_name: subtype out of range");
	}
}

int food_value_by_index(int index)
{
	switch (index)
	{
	case (0): return 50;
	case (1): return 75;
	case (2): return 100;	
	default: return 0;
	}
}

int gem_value_by_index(int index)
{
	switch (index)
	{
	case (0): return 50;
	case (1): return 75;
	case (2): return 100;
	case (3): return 200;
	case (4): return 300;
	case (5): return 500;
	case (6): return 1000;
	case (7): return 2000;
	default: return 0;
	}
}

int coin_value_by_index(int index)
{
	switch (index)
	{
	case (0): return 1;
	case (1): return 5;
	default: return 0;
	}
}

int lifeup_value_by_index(int index)
{
	switch (index)
	{
	case (0): return 4;
	case (1): return 32;
	default: return 0;
	}
}

int colliding_entity_value(COLLIDING_ENTITY_SUBTYPE subtype)
{
	switch (subtype)
	{
	case (COLLIDING_ENTITY_SUBTYPE::CASTER): return 500;
	case (COLLIDING_ENTITY_SUBTYPE::ELEMENTAL): return 500;
	case (COLLIDING_ENTITY_SUBTYPE::ICEBANER): return 500;
	case (COLLIDING_ENTITY_SUBTYPE::CRIMSON_LASHER): return 500;
	case (COLLIDING_ENTITY_SUBTYPE::AQUAVEIL_WEAVER): return 500;
	case (COLLIDING_ENTITY_SUBTYPE::ROCK_HURLER): return 500;
	case (COLLIDING_ENTITY_SUBTYPE::AERIAL_BOMBARDIER): return 500;
	case (COLLIDING_ENTITY_SUBTYPE::JUNGLE_BOOMERANGER): return 500;
	case (COLLIDING_ENTITY_SUBTYPE::ENERGIZED_GALVANIZER): return 300;
	default: return 0;
	}
}

int walking_entity_value(WALKING_ENTITY_SUBTYPE subtype)
{
	switch (subtype)
	{
	case (WALKING_ENTITY_SUBTYPE::WARRIOR): return 100;
	case (WALKING_ENTITY_SUBTYPE::GOLEM): return 300;
	case (WALKING_ENTITY_SUBTYPE::SCORCHER): return 200;
	default: return 0;
	}
}