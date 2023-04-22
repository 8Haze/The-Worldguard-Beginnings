#include "ice_chunk.h"

#include "camera.h"

// ----------------------------------------------------------------
// Ice_Chunk
// ----------------------------------------------------------------

Ice_Chunk::Ice_Chunk(Resources& arg_resources, sf::Vector2f arg_position, DIRECTION arg_direction) :
	Vertical_Physics_Component(*this), 
	Horizontal_Physics_Component(*this)
{
	sprite.setPosition(arg_position);
	sprite.setTexture(arg_resources.textures.get(RESOURCE_ID::TEXTURES::ICE_CHUNK));
	sprite.set_animation(get_animation(AID_MISC::ICE_CHUNK));
	set_center(sprite);

	set_max_velocity(x_speed);
	switch (arg_direction)
	{
	case (DIRECTION::LEFT): set_going_left(true); set_velocity(-x_speed); break;
	case (DIRECTION::RIGHT): set_going_right(true); set_velocity(x_speed); break;
	default: throw std::runtime_error("Ice_Chunk::Ice_Chunk: invalid direction provided");
	}
}

sf::FloatRect Ice_Chunk::get_hitbox() const noexcept
{
	auto result = ::get_hitbox(sprite, 0.6f, 0.6f);
	result.top += sprite.getGlobalBounds().height * 0.2f;
	return result;
}

bool Ice_Chunk::is_active() const noexcept
{
	return active;
}

bool Ice_Chunk::is_finished() const noexcept
{
	return sprite.finished();
}

void Ice_Chunk::set_hit(Resources& resources)
{
	if (!active)
		return;

	active = false;

	sprite.setTexture(resources.textures.get(RESOURCE_ID::TEXTURES::ICE_CHUNK_CRACK));
	auto animation = get_animation(AID_MISC::ICE_CHUNK_CRACK);
	sprite.set_animation(animation);
}

void Ice_Chunk::move(sf::Vector2f vector)
{
	sprite.move(vector);
}

void Ice_Chunk::update(Resources& resources, const sf::Time& dt,
	const hct::dynarray<std::unique_ptr<Solid_Tile>>& solid_tiles,
	const hct::dynarray<std::unique_ptr<Destructible_Tile>>& destructible_tiles)
{
	sprite.update_frame(dt);

	if (!active)
		return;

	auto [solid_left, solid_right] = get_range_of_closest_objects(sprite, solid_tiles, 2.5f);
	auto [destructible_left, destructible_right] = get_range_of_closest_objects(sprite, destructible_tiles, 2.5f);

	auto passed_distance_now = update_horizontally(dt);
	passed_distance += std::abs(passed_distance_now);
	sprite.move(passed_distance_now, 0.f);

	if ((check_horizontal_collisions(solid_left, solid_right, get_hitbox(), false, false) != Horizontal_Physics_Component::COLLISION::NONE) ||
		(check_horizontal_collisions(destructible_left, destructible_right, get_hitbox(), false, false) != Horizontal_Physics_Component::COLLISION::NONE))
		set_hit(resources);

	sprite.move(0.f, update_vertically(dt));

	check_vertical_collisions(solid_left, solid_right, get_hitbox());
	check_vertical_collisions(destructible_left, destructible_right, get_hitbox());

	if (sprite.getPosition().y > Camera::down_lim + 32.f || passed_distance >= max_distance)
		set_hit(resources);
}

void Ice_Chunk::draw(sf::RenderTarget& rt) const
{
	rt.draw(sprite);
}

// ----------------------------------------------------------------
// Ice_Chunk_Manager_Component
// ----------------------------------------------------------------

void Ice_Chunk_Manager_Component::spawn_subentity(Resources& resources, sf::Vector2f position, Ice_Chunk::DIRECTION direction)
{
	create_subentity(resources, position, direction);
}