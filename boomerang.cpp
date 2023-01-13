#include "boomerang.h"

#include "lo_tiles.h"

// ----------------------------------------------------------------
// Boomerang
// ----------------------------------------------------------------

Boomerang::Boomerang(Resources& arg_resources, sf::Vector2f arg_position, DIRECTION arg_direction, LEVEL_THEME arg_theme, float arg_max_speed, float arg_falloff_distance) :
	Horizontal_Physics_Component(*this), throw_position(arg_position), falloff_distance(arg_falloff_distance)
{
	sprite.setTexture(arg_resources.textures.get(RESOURCE_ID::TEXTURES::BOOMERANG));
	auto animation = get_animation(AID_MISC::BOOMERANG);
	switch (arg_theme)
	{
	case(LEVEL_THEME::VERDANT):animation.row = 0; break;
	default: throw std::runtime_error("Boomerang::Boomerang: invalid theme id provided");
	}
	sprite.set_animation(animation);
	set_center(sprite);
	sprite.setPosition(throw_position);

	set_max_velocity(std::abs(arg_max_speed));
	switch (arg_direction)
	{
	case (DIRECTION::LEFT): set_velocity(-get_max_velocity()); set_going_left(true); set_going_right(false); sprite.get_animation_settings().horizontally_flipped = true; break;
	case (DIRECTION::RIGHT): set_velocity(get_max_velocity()); set_going_left(false); set_going_right(true); sprite.get_animation_settings().horizontally_flipped = false; break;
	default: throw std::runtime_error("Boomerang::Boomerang: unrecognized direction");
	}
}

sf::FloatRect Boomerang::get_hitbox() const noexcept
{
	return ::get_hitbox(sprite, 0.8f, 0.6f);
}

bool Boomerang::is_active() const noexcept
{
	return active;
}

bool Boomerang::is_finished() const noexcept
{
	return !active;
}

void Boomerang::move(sf::Vector2f vector)
{
	sprite.move(vector);
}

void Boomerang::update(Resources& resources, const sf::Time& dt,
	const hct::dynarray<std::unique_ptr<Solid_Tile>>& solid_tiles,
	const hct::dynarray<std::unique_ptr<Destructible_Tile>>& destructible_tiles)
{
	if (!active)
		return;

	sprite.update_frame(dt);

	float x_offset = update_horizontally(dt);
	sprite.move(x_offset, 0.f);
	passed_distance += std::abs(x_offset);

	auto [solid_left, solid_right] = get_range_of_closest_objects(sprite, solid_tiles, 2.5f);
	auto [destructible_left, destructible_right] = get_range_of_closest_objects(sprite, destructible_tiles, 2.5f);

	if ((check_horizontal_collisions(solid_left, solid_right, get_hitbox(), false, false) != Horizontal_Physics_Component::COLLISION::NONE) ||
		(check_horizontal_collisions(destructible_left, destructible_right, get_hitbox(), false, false) != Horizontal_Physics_Component::COLLISION::NONE))
		passed_distance = falloff_distance;

	if (!returning && passed_distance >= falloff_distance)
	{
		returning = true;

		if (is_going_left())
		{
			set_going_left(false);
			set_going_right(true);
		}
		else
		{
			set_going_left(true);
			set_going_right(false);
		}
	}
	else if (returning && ((is_going_left() && sprite.getPosition().x <= throw_position.x) || (is_going_right() && sprite.getPosition().x >= throw_position.x)))
		active = false;
}

void Boomerang::draw(sf::RenderTarget& rt) const
{
	if (active)
		rt.draw(sprite);
}

// ----------------------------------------------------------------
// Boomerang_Manager_Component
// ----------------------------------------------------------------

Boomerang_Manager_Component::Boomerang_Manager_Component(LEVEL_THEME arg_theme_id) : theme_id(arg_theme_id) {}

void Boomerang_Manager_Component::spawn_subentity(Resources& resources, sf::Vector2f position, Boomerang::DIRECTION direction, float max_speed, float falloff_distance)
{
	create_subentity(resources, position, direction, theme_id, max_speed, falloff_distance);
}