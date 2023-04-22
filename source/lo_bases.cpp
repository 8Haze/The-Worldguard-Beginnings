#include "lo_bases.h"

// ----------------------------------------------------------------
// LO_Static
// ----------------------------------------------------------------

void LO_Static::set_sprite_rect(sf::IntRect rect)
{
	sprite.setTextureRect(rect);
}

// ----------------------------------------------------------------
// LO_Animated
// ----------------------------------------------------------------

void LO_Animated::set_sprite_animation(Animation_Settings animation)
{
	sprite.set_animation(animation);
	if (sprite.getOrigin().x != 0.f || sprite.getOrigin().y != 0.f)
		set_center(sprite);
}

Animation_Settings& LO_Animated::get_sprite_animation()
{
	return sprite.get_animation_settings();
}

void LO_Animated::restart_sprite_animation()
{
	sprite.restart_animation();
}

void LO_Animated::update_frame(const sf::Time& dt)
{
	sprite.update_frame(dt);
}
