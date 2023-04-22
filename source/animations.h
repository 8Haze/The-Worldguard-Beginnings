#ifndef _ANIMATIONS_H_
#define _ANIMATIONS_H_

#include "animatable_sprite.h"

// ----------------------------------------------------------------
// Enumerations (AID - Animation ID)
// ----------------------------------------------------------------

enum class AID_PLAYER 
{ 
	IDLE,
	WALKING,
	IN_AIR,
	ON_HOOK,
	SHOCKED,
	TELEPORT_OUT,
	TELEPORT_IN,
	COUNT
}; // keep COUNT last

enum class AID_ITEM 
{ 
	FOOD,
	GEM,
	COIN,
	LIFEUP,
	COUNT
}; // keep COUNT last

enum class AID_TILE
{ 
	COLD_AIR,
	HOT_AIR,
	SHINY_SPARKLE,
	WARP,
	COIN_WARP_10,
	COIN_WARP_20,
	COIN_WARP_50,
	FINISH_WARP,
	WATER,
	WATER_SURFACE,
	BLOOD_ORB,
	COUNT
}; // keep COUNT last

enum class AID_ENTITY 
{
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
	HEALING_PLANT,
	PRESSURE_ORB,
	BIONIC_RAY_DEVICE,
	ELECTRIC_GATE_DEVICE,
	FLAMETHROWER,
	ROAMING_TRAMPOLINE,
	SCOUTER,
	WARRIORS,
	CASTERS,
	GOLEMS,
	ELEMENTALS,
	JUNGLE_BOOMERANGER,
	AQUAVEIL_WEAVER_FORM,
	AQUAVEIL_WEAVER_RELEASE,
	SCORCHER,
	SCORCHER_AFTERMATH_1,
	SCORCHER_AFTERMATH_2,
	ENERGIZED_GALVANIZER,
	AERIAL_BOMBARDIER,
	ROCK_HURLER,
	CRIMSON_LASHER,
	ICEBANER,
	COUNT
}; // keep COUNT last

enum class AID_MISC 
{ 
	EXPLOSION_32, 
	EXPLOSION_48,
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
	THORN_SPEAR_OUT,
	THORN_SPEAR_IN,
	HEALING_BLAST,
	PRESSURE_BLAST,
	BIONIC_RAY,
	ELECTRIC_GATE,
	FLAMETHROWER_FLAME,
	COUNT
}; // keep COUNT last

// ----------------------------------------------------------------
// Functions
// ---------------------------------------------------------------- 

// These only return the "normal" animation details
Animation_Settings get_animation(AID_PLAYER id);
Animation_Settings get_animation(AID_ITEM id);
Animation_Settings get_animation(AID_ENTITY id);
Animation_Settings get_animation(AID_MISC id);

// These also return the desired transparency value
std::pair<Animation_Settings, sf::Uint8> get_animation(AID_TILE id);

#endif // _ANIMATIONS_H_