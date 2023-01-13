#include "bubble.h"

// ----------------------------------------------------------------
// Bubble
// ----------------------------------------------------------------

Bubble::Bubble(Resources& arg_resources, sf::Vector2f arg_pos) : Vertical_Physics_Component(*this)
{
	sprite.setPosition(arg_pos);
	sprite.setTexture(arg_resources.textures.get(RESOURCE_ID::TEXTURES::BUBBLE));
	sprite.set_animation(get_animation(AID_MISC::BUBBLE));
	sprite.setColor({ 255, 255, 255, transparency_value });
	set_center(sprite);

	set_reversed_gravity(true);
	set_gravity(0.f);
}

sf::FloatRect Bubble::get_hitbox() const noexcept
{
	return ::get_hitbox(sprite, 0.8f, 0.8f);
}

bool Bubble::is_active() const noexcept
{
	return active;
}

bool Bubble::is_finished() const noexcept
{
	return sprite.finished();
}

void Bubble::set_popped(Resources& resources)
{
	if (!active)
		return;

	active = false;

	sprite.setTexture(resources.textures.get(RESOURCE_ID::TEXTURES::BUBBLE_POP));
	auto animation = get_animation(AID_MISC::BUBBLE_POP);
	sprite.set_animation(animation);
}

void Bubble::move(sf::Vector2f vector)
{
	sprite.move(vector);
}

void Bubble::update(Resources& resources, const sf::Time& dt,
	const hct::dynarray<std::unique_ptr<Solid_Tile>>& solid_tiles,
	const hct::dynarray<std::unique_ptr<Destructible_Tile>>& destructible_tiles)
{
	sprite.update_frame(dt);

	if (!active)
		return;

	auto [solid_left, solid_right] = get_range_of_closest_objects(sprite, solid_tiles, 2.5f);
	auto [destructible_left, destructible_right] = get_range_of_closest_objects(sprite, destructible_tiles, 2.5f);

	auto passed_distance_now = update_vertically(dt) * gravity_update_factor;
	passed_distance += std::abs(passed_distance_now);
	sprite.move(0.f, passed_distance_now);

	if ((check_vertical_collisions(solid_left, solid_right, get_hitbox()) == Vertical_Physics_Component::COLLISION::TOP) ||
		(check_vertical_collisions(destructible_left, destructible_right, get_hitbox()) == Vertical_Physics_Component::COLLISION::TOP))
		set_popped(resources);

	if (sprite.getPosition().y < Camera::up_lim || passed_distance >= max_distance)
		set_popped(resources);
}

void Bubble::draw(sf::RenderTarget& rt) const
{
	rt.draw(sprite);
}

// ----------------------------------------------------------------
// Bubble_Manager_Component
// ----------------------------------------------------------------

void Bubble_Manager_Component::spawn_subentity(Resources& resources, sf::Vector2f position)
{
	create_subentity(resources, position);
}