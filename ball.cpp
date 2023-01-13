#include "ball.h"

#include "camera.h"

// ----------------------------------------------------------------
// Ball
// ----------------------------------------------------------------

Ball::Ball(Resources& arg_resources, sf::Vector2f arg_position, LEVEL_THEME arg_theme, float arg_velocity, float arg_gravity, float arg_gravity_cooldown, int arg_bounces) :
	Vertical_Physics_Component(*this), Horizontal_Physics_Component(*this),
	bounce_gravity(arg_gravity), bounce_gravity_cooldown(arg_gravity_cooldown), bounces_left(arg_bounces)
{
	sprite.setPosition(arg_position);
	sprite.setTexture(arg_resources.textures.get(RESOURCE_ID::TEXTURES::BALL));
	auto animation = get_animation(AID_MISC::BALL);
	switch (arg_theme)
	{
	case(LEVEL_THEME::ELECTRIC): animation.row = 0; break;
	case(LEVEL_THEME::WIND): animation.row = 1; break;
	default: throw std::runtime_error("Ball::Ball: invalid theme id provided");
	}
	sprite.set_animation(animation);
	set_center(sprite);

	set_max_velocity(std::abs(arg_velocity));
	set_velocity(arg_velocity);
	if (arg_velocity < 0.f - EPSILON) set_going_left(true);
	else if (arg_velocity > 0.f + EPSILON) set_going_right(true);
	set_gravity(-arg_gravity);
	set_gravity_cooldown(bounce_gravity_cooldown);
}

sf::FloatRect Ball::get_hitbox() const noexcept
{
	return ::get_hitbox(sprite, 0.8f, 0.8f);
}

bool Ball::is_active() const noexcept
{
	return active;
}

bool Ball::is_finished() const noexcept
{
	return sprite.finished();
}

void Ball::set_hit(Resources& resources)
{
	if (!active)
		return;

	active = false;

	sprite.setTexture(resources.textures.get(RESOURCE_ID::TEXTURES::BALL_CRACK));
	auto animation_row = sprite.get_animation_settings().row;
	auto animation = get_animation(AID_MISC::BALL_CRACK);
	animation.row = animation_row;
	sprite.set_animation(animation);
}

void Ball::move(sf::Vector2f vector)
{
	sprite.move(vector);
}

void Ball::update(Resources& resources, sf::Time dt,
	const hct::dynarray<std::unique_ptr<Solid_Tile>>& solid_tiles,
	const hct::dynarray<std::unique_ptr<Destructible_Tile>>& destructible_tiles)
{
	sprite.update_frame(dt);

	if (!active)
		return;

	auto [solid_left, solid_right] = get_range_of_closest_objects(sprite, solid_tiles, 2.5f);
	auto [destructible_left, destructible_right] = get_range_of_closest_objects(sprite, destructible_tiles, 2.5f);

	sprite.move(0.f, update_vertically(dt));

	if (sprite.getPosition().y > Camera::down_lim + 32.f)
		set_hit(resources);

	if ((check_vertical_collisions(solid_left, solid_right, get_hitbox()) == Vertical_Physics_Component::COLLISION::BOTTOM) ||
		(check_vertical_collisions(destructible_left, destructible_right, get_hitbox()) == Vertical_Physics_Component::COLLISION::BOTTOM))
	{
		--bounces_left;
		set_gravity(bounce_gravity);
		set_gravity_cooldown(bounce_gravity_cooldown);
	}

	sprite.move(update_horizontally(dt), 0.f);

	if ((check_horizontal_collisions(solid_left, solid_right, get_hitbox(), false, false) != Horizontal_Physics_Component::COLLISION::NONE) ||
		(check_horizontal_collisions(destructible_left, destructible_right, get_hitbox(), false, false) != Horizontal_Physics_Component::COLLISION::NONE))
		bounces_left = -1;

	if (bounces_left < 0)
		set_hit(resources);
}

void Ball::draw(sf::RenderTarget& rt) const
{
	rt.draw(sprite);
}

// ----------------------------------------------------------------
// Ball_Manager_Component
// ----------------------------------------------------------------

Ball_Manager_Component::Ball_Manager_Component(LEVEL_THEME arg_theme_id) : theme_id(arg_theme_id) {}

void Ball_Manager_Component::spawn_subentity(Resources& resources, sf::Vector2f position, float velocity, float gravity, float gravity_cooldown, int bounces)
{
	create_subentity(resources, position, theme_id, velocity, gravity, gravity_cooldown, bounces);
}