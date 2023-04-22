#include "rock.h"

#include "camera.h"

// ----------------------------------------------------------------
// Rock
// ----------------------------------------------------------------

Rock::Rock(Resources& arg_resources, sf::Vector2f arg_pos, LEVEL_THEME arg_theme, float arg_velocity, float arg_gravity) :
	Vertical_Physics_Component(*this), Horizontal_Physics_Component(*this)
{
	sprite.setPosition(arg_pos);
	sprite.setTexture(arg_resources.textures.get(RESOURCE_ID::TEXTURES::ROCK));
	auto animation = get_animation(AID_MISC::ROCK);
	switch (arg_theme)
	{
	case(LEVEL_THEME::FIRE): animation.row = 0; break;
	case(LEVEL_THEME::GROUND): animation.row = 1; break;
	case(LEVEL_THEME::ICE): animation.row = 2; break;
	default: throw std::runtime_error("Rock::Rock: invalid theme id provided");
	}
	sprite.set_animation(animation);
	set_center(sprite);

	set_max_velocity(std::abs(arg_velocity));
	set_velocity(arg_velocity);
	if (arg_velocity < 0.f - EPSILON) set_going_left(true);
	else if (arg_velocity > 0.f + EPSILON) set_going_right(true);
	set_gravity(arg_gravity);
}

sf::FloatRect Rock::get_hitbox() const noexcept
{
	return ::get_hitbox(sprite, 0.5f, 0.5f);
}

bool Rock::is_active() const noexcept
{
	return active;
}

bool Rock::is_finished() const noexcept
{
	return sprite.finished();
}

void Rock::set_hit(Resources& resources)
{
	if (!active)
		return;

	active = false;

	sprite.setTexture(resources.textures.get(RESOURCE_ID::TEXTURES::ROCK_CRACK));
	auto animation = get_animation(AID_MISC::ROCK_CRACK);
	animation.row = sprite.get_animation_settings().row;
	sprite.set_animation(animation);
}

void Rock::move(sf::Vector2f vector)
{
	sprite.move(vector);
}

void Rock::update(Resources& resources, const sf::Time& dt,
	const hct::dynarray<std::unique_ptr<Solid_Tile>>& solid_tiles,
	const hct::dynarray<std::unique_ptr<Destructible_Tile>>& destructible_tiles)
{
	sprite.update_frame(dt);

	if (!active)
		return;

	sprite.move(update_horizontally(dt), update_vertically(dt));

	if (sprite.getPosition().y > Camera::down_lim + 32.f)
		set_hit(resources);

	auto [solid_left, solid_right] = get_range_of_closest_objects(sprite, solid_tiles, 2.5f);
	auto [destructible_left, destructible_right] = get_range_of_closest_objects(sprite, destructible_tiles, 2.5f);

	if ((check_vertical_collisions(solid_left, solid_right, get_hitbox()) != Vertical_Physics_Component::COLLISION::NONE) ||
		(check_vertical_collisions(destructible_left, destructible_right, get_hitbox()) != Vertical_Physics_Component::COLLISION::NONE) ||
		(check_horizontal_collisions(solid_left, solid_right, get_hitbox(), false, false) != Horizontal_Physics_Component::COLLISION::NONE) ||
		(check_horizontal_collisions(destructible_left, destructible_right, get_hitbox(), false, false) != Horizontal_Physics_Component::COLLISION::NONE))
		set_hit(resources);
}

void Rock::draw(sf::RenderTarget& rt) const
{
	rt.draw(sprite);
}

// ----------------------------------------------------------------
// Rock_Manager_Component
// ----------------------------------------------------------------

Rock_Manager_Component::Rock_Manager_Component(LEVEL_THEME arg_theme_id) : theme_id(arg_theme_id) {}

void Rock_Manager_Component::spawn_subentity(Resources& resources, sf::Vector2f position, float velocity, float gravity)
{
	create_subentity(resources, position, theme_id, velocity, gravity);
}