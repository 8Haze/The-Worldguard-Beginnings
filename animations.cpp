#include "animations.h"

Animation_Settings get_animation(AID_PLAYER id)
{
	switch (id)
	{
	case(AID_PLAYER::IDLE): return Animation_Settings(0, 0, 1, 48, 48, 0.1f, true);
	case(AID_PLAYER::WALKING): return Animation_Settings(0, 0, 6, 48, 48, 0.6f, true);
	case(AID_PLAYER::IN_AIR): return Animation_Settings(2, 0, 1, 48, 48, 0.1f, true);
	case(AID_PLAYER::ON_HOOK): return Animation_Settings(2, 1, 1, 48, 48, 0.1f, true);
	case(AID_PLAYER::SHOCKED): return Animation_Settings(2, 2, 1, 48, 48, 0.1f, true);
	case(AID_PLAYER::TELEPORT_OUT): return Animation_Settings(4, 0, 6, 48, 48, 0.3f, false);
	case(AID_PLAYER::TELEPORT_IN): return Animation_Settings(5, 0, 6, 48, 48, 0.3f, false);
	default: throw std::runtime_error("get_animation: unrecognized AID_PLAYER id");
	}
}

Animation_Settings get_animation(AID_ITEM id)
{
	switch (id)
	{
	case(AID_ITEM::FOOD): return Animation_Settings(0, 0, 1, 32, 32, 1.f, true);
	case(AID_ITEM::GEM): return Animation_Settings(0, 0, 8, 32, 32, 0.75f, true);
	case(AID_ITEM::COIN): return Animation_Settings(0, 0, 10, 32, 32, 1.f, true);
	case(AID_ITEM::LIFEUP): return Animation_Settings(0, 0, 4, 32, 32, 0.5f, true);
	default: throw std::runtime_error("get_animation: unrecognized AID_ITEM id");
	}
}

Animation_Settings get_animation(AID_ENTITY id)
{
	switch (id)
	{
	case(AID_ENTITY::TRAMPOLINE): return Animation_Settings(0, 0, 8, 32, 32, 0.3f, false);
	case(AID_ENTITY::CANNON): return Animation_Settings(0, 0, 8, 32, 32, 0.5f, false);
	case(AID_ENTITY::BOUNCER): return Animation_Settings(0, 0, 7, 32, 32, 0.75f, true);
	case(AID_ENTITY::WIND_BLASTER): return Animation_Settings(0, 0, 8, 32, 32, 1.f, true);
	case(AID_ENTITY::MINE): return Animation_Settings(0, 0, 8, 32, 32, 1.f, true);
	case(AID_ENTITY::LIVING_SPIKES): return Animation_Settings(0, 0, 8, 32, 32, 0.3f, false);
	case(AID_ENTITY::THORN): return Animation_Settings(0, 0, 1, 32, 32, 1.f, false);
	case(AID_ENTITY::THORN_SPEAR_ROOT): return Animation_Settings(0, 0, 1, 32, 32, 1.f, true);
	case(AID_ENTITY::FALLING_HAZARD): return Animation_Settings(0, 0, 8, 32, 32, 1.f, true);
	case(AID_ENTITY::ROCK_DISPOSER): return Animation_Settings(0, 0, 8, 64, 64, 0.5f, true);
	case(AID_ENTITY::ICE_LAUNCHER): return Animation_Settings(0, 0, 8, 32, 32, 0.25f, false);
	case(AID_ENTITY::BUBBLE_MACHINE): return Animation_Settings(0, 0, 8, 32, 32, 0.5f, false);
	case(AID_ENTITY::HEALING_PLANT): return Animation_Settings(0, 0, 4, 32, 32, 0.5f, true);
	case(AID_ENTITY::PRESSURE_ORB): return Animation_Settings(0, 0, 8, 32, 32, 0.8f, true);
	case(AID_ENTITY::BIONIC_RAY_DEVICE): return Animation_Settings(0, 0, 8, 32, 32, 0.5f, true);
	case(AID_ENTITY::ELECTRIC_GATE_DEVICE): return Animation_Settings(0, 0, 8, 32, 32, 0.5f, true);
	case(AID_ENTITY::FLAMETHROWER): return Animation_Settings(0, 0, 8, 32, 32, 0.5f, false);
	case(AID_ENTITY::ROAMING_TRAMPOLINE): return Animation_Settings(0, 0, 8, 32, 32, 0.3f, false);
	case(AID_ENTITY::SCOUTER): return Animation_Settings(0, 0, 6, 32, 32, 0.4f, true);
	case(AID_ENTITY::WARRIORS): return Animation_Settings(0, 0, 6, 48, 48, 0.6f, true);
	case(AID_ENTITY::CASTERS): return Animation_Settings(0, 0, 5, 48, 48, 0.3f, false);
	case(AID_ENTITY::GOLEMS): return Animation_Settings(0, 0, 6, 48, 48, 0.6f, true);
	case(AID_ENTITY::ELEMENTALS): return Animation_Settings(0, 0, 12, 32, 32, 0.75f, true);
	case(AID_ENTITY::JUNGLE_BOOMERANGER): return Animation_Settings(0, 0, 5, 48, 48, 0.3f, false);
	case(AID_ENTITY::AQUAVEIL_WEAVER_FORM): return Animation_Settings(0, 0, 6, 48, 48, 0.3f, false);
	case(AID_ENTITY::AQUAVEIL_WEAVER_RELEASE): return Animation_Settings(1, 0, 6, 48, 48, 0.3f, false);
	case(AID_ENTITY::SCORCHER): return Animation_Settings(0, 0, 8, 32, 32, 0.3f, false);
	case(AID_ENTITY::SCORCHER_AFTERMATH_1): return Animation_Settings(0, 0, 7, 32, 32, 0.25f, false);
	case(AID_ENTITY::SCORCHER_AFTERMATH_2): return Animation_Settings(1, 0, 7, 32, 32, 0.25f, true);
	case(AID_ENTITY::ENERGIZED_GALVANIZER): return Animation_Settings(0, 0, 8, 32, 32, 1.f, true);
	case(AID_ENTITY::AERIAL_BOMBARDIER): return Animation_Settings(0, 0, 6, 48, 48, 0.2f, false);
	case(AID_ENTITY::ROCK_HURLER): return Animation_Settings(0, 0, 6, 48, 48, 0.2f, false);
	case(AID_ENTITY::CRIMSON_LASHER): return Animation_Settings(0, 0, 5, 48, 48, 0.3f, false);
	case(AID_ENTITY::ICEBANER): return Animation_Settings(0, 0, 5, 48, 48, 0.2f, false);
	default: throw std::runtime_error("get_animation: unrecognized AID_ENTITY id");
	}
}

Animation_Settings get_animation(AID_MISC id)
{
	switch (id)
	{
	case(AID_MISC::EXPLOSION_32): return Animation_Settings(0, 0, 7, 32, 32, 0.35f, false);
	case(AID_MISC::EXPLOSION_48): return Animation_Settings(0, 0, 8, 48, 48, 0.35f, false);
	case(AID_MISC::ITEM_FADEOUT): return Animation_Settings(0, 0, 4, 32, 32, 0.2f, false);
	case(AID_MISC::BULLET): return Animation_Settings(0, 0, 10, 16, 16, 0.4f, true);
	case(AID_MISC::BULLET_FADEOUT): return Animation_Settings(0, 0, 6, 16, 16, 0.2f, false);
	case(AID_MISC::WAVE): return Animation_Settings(0, 0, 4, 24, 48, 0.4f, true);
	case(AID_MISC::WAVE_FADEOUT): return Animation_Settings(0, 0, 3, 24, 48, 0.2f, false);
	case(AID_MISC::MASS): return Animation_Settings(0, 0, 10, 32, 32, 0.4f, true);
	case(AID_MISC::MASS_FADEOUT): return Animation_Settings(0, 0, 6, 32, 32, 0.2f, false);
	case(AID_MISC::ROCK): return Animation_Settings(0, 0, 4, 16, 16, 0.25f, true);
	case(AID_MISC::ROCK_CRACK): return Animation_Settings(0, 0, 6, 16, 16, 0.25f, false);
	case(AID_MISC::ICE_CHUNK): return Animation_Settings(0, 0, 8, 32, 32, 0.75f, true);
	case(AID_MISC::ICE_CHUNK_CRACK): return Animation_Settings(0, 0, 6, 32, 32, 0.25f, false);
	case(AID_MISC::BALL): return Animation_Settings(0, 0, 6, 16, 16, 0.4f, true);
	case(AID_MISC::BALL_CRACK): return Animation_Settings(0, 0, 6, 16, 16, 0.25f, false);
	case(AID_MISC::BUBBLE): return Animation_Settings(0, 0, 6, 16, 16, 0.4f, true);
	case(AID_MISC::BUBBLE_POP): return Animation_Settings(0, 0, 6, 16, 16, 0.25f, false);
	case(AID_MISC::BOOMERANG): return Animation_Settings(0, 0, 6, 16, 16, 0.3f, true);
	case(AID_MISC::THORN_SPEAR_OUT): return Animation_Settings(0, 0, 11, 32, 128, 0.25f, false);
	case(AID_MISC::THORN_SPEAR_IN): return Animation_Settings(1, 0, 11, 32, 128, 0.25f, false);
	case(AID_MISC::HEALING_BLAST): return Animation_Settings(0, 0, 11, 64, 64, 0.4f, false);
	case(AID_MISC::PRESSURE_BLAST): return Animation_Settings(0, 0, 6, 32, 32, 0.3f, false);
	case(AID_MISC::BIONIC_RAY): return Animation_Settings(0, 0, 8, 32, 32, 0.1f, true);
	case(AID_MISC::ELECTRIC_GATE): return Animation_Settings(0, 0, 8, 32, 32, 0.2f, true);
	case(AID_MISC::FLAMETHROWER_FLAME): return Animation_Settings(0, 0, 8, 32, 32, 0.5f, false);
	default: throw std::runtime_error("get_animation: unrecognized AID_MISC id");
	}
}

std::pair<Animation_Settings, sf::Uint8> get_animation(AID_TILE id)
{
	switch (id)
	{
	case(AID_TILE::COLD_AIR): return { Animation_Settings(0, 0, 8, 32, 32, 1.f, true), 50 };
	case(AID_TILE::HOT_AIR): return { Animation_Settings(0, 8, 8, 32, 32, 1.f, true), 50 };
	case(AID_TILE::SHINY_SPARKLE): return { Animation_Settings(0, 16, 8, 32, 32, 0.75f, true), 255 };
	case(AID_TILE::WARP): return { Animation_Settings(1, 0, 7, 32, 32, 0.75f, true), 255 };
	case(AID_TILE::COIN_WARP_10): return { Animation_Settings(1, 8, 7, 32, 32, 0.75f, true), 255 };
	case(AID_TILE::COIN_WARP_20): return { Animation_Settings(1, 16, 7, 32, 32, 0.75f, true), 255 };
	case(AID_TILE::COIN_WARP_50): return { Animation_Settings(2, 0, 7, 32, 32, 0.75f, true), 255 };
	case(AID_TILE::FINISH_WARP): return { Animation_Settings(2, 8, 7, 32, 32, 0.75f, true), 255 };
	case(AID_TILE::WATER): return { Animation_Settings(2, 16, 8, 32, 32, 1.f, true), 100 };
	case(AID_TILE::WATER_SURFACE): return { Animation_Settings(3, 0, 8, 32, 32, 1.f, true), 100 };
	case(AID_TILE::BLOOD_ORB): return { Animation_Settings(3, 8, 8, 32, 32, 1.f, true), 255 };
	default: throw std::runtime_error("get_animation: unrecognized AID_TILE id");
	}
}