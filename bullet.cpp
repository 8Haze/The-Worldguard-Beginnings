#include "bullet.h"

#include "lo_tiles.h"

// ----------------------------------------------------------------
// Bullet
// ----------------------------------------------------------------

Bullet::Bullet(Resources& arg_resources, sf::Vector2f arg_pos, DIRECTION arg_direction, TYPE arg_type, bool arg_destroy_destructibles, float arg_speed, float arg_max_distance) :
	movement_vector(0.f, 0.f), type(arg_type), speed(arg_speed), max_distance(arg_max_distance), destroy_destructibles(arg_destroy_destructibles)
{
	sprite.setPosition(arg_pos);

	Animation_Settings animation;
	if (type >= Bullet::TYPE::PLAYER && type <= Bullet::TYPE::ICE)
	{
		sprite.setTexture(arg_resources.textures.get(RESOURCE_ID::TEXTURES::BULLET));
		animation = get_animation(AID_MISC::BULLET);
		animation.row = static_cast<int>(type);
	}
	else if (type == Bullet::TYPE::AQUA_WAVE || type == Bullet::TYPE::CRIMSON_WAVE)
	{
		sprite.setTexture(arg_resources.textures.get(RESOURCE_ID::TEXTURES::WAVE));
		animation = get_animation(AID_MISC::WAVE);
		animation.row = (type == Bullet::TYPE::AQUA_WAVE ? 0 : 1);
	}
	else if (type == Bullet::TYPE::ICE_MASS)
	{
		sprite.setTexture(arg_resources.textures.get(RESOURCE_ID::TEXTURES::MASS));
		animation = get_animation(AID_MISC::MASS);
	}
	else
		throw std::runtime_error("Bullet::Bullet: unrecognized bullet type");

	switch (arg_direction)
	{
	case(DIRECTION::LEFT): movement_vector.x = -speed; animation.horizontally_flipped = true; break;
	case(DIRECTION::RIGHT): movement_vector.x = speed; break;
	case(DIRECTION::UP): movement_vector.y = -speed; sprite.setRotation(270.f); break;
	case(DIRECTION::DOWN): movement_vector.y = speed; sprite.setRotation(90.f); break;
	default: throw std::runtime_error("Bullet::Bullet: invalid direction provided");
	}

	sprite.set_animation(animation);
	set_center(sprite);
}

sf::FloatRect Bullet::get_hitbox() const noexcept
{
	return ::get_hitbox(sprite, 0.8f, 0.8f);
}

bool Bullet::is_active() const noexcept
{
	return active;
}

bool Bullet::is_finished() const noexcept
{
	return sprite.finished();
}

void Bullet::set_speed(float arg)
{
	speed = arg;

	if (sprite.getRotation() < 1.f && !sprite.is_horizontally_flipped())
		movement_vector.x = speed;
	else if (sprite.getRotation() > 89.f && sprite.getRotation() < 91.f)
		movement_vector.y = speed;
	else if ((sprite.getRotation() > 179.f && sprite.getRotation() < 181.f) || sprite.is_horizontally_flipped())
		movement_vector.x = -speed;
	else if (sprite.getRotation() > 269.f && sprite.getRotation() < 271.f)
		movement_vector.y = -speed;
}

void Bullet::set_hit(Resources& resources)
{
	if (!active)
		return;

	active = false;
	passed_distance = max_distance;

	Animation_Settings animation;
	if (type >= Bullet::TYPE::PLAYER && type <= Bullet::TYPE::ICE)
	{
		sprite.setTexture(resources.textures.get(RESOURCE_ID::TEXTURES::BULLET_FADEOUT));
		animation = get_animation(AID_MISC::BULLET_FADEOUT);
	}
	else if (type == Bullet::TYPE::AQUA_WAVE || type == Bullet::TYPE::CRIMSON_WAVE)
	{
		sprite.setTexture(resources.textures.get(RESOURCE_ID::TEXTURES::WAVE_FADEOUT));
		animation = get_animation(AID_MISC::WAVE_FADEOUT);
	}
	else if (type == Bullet::TYPE::ICE_MASS)
	{
		sprite.setTexture(resources.textures.get(RESOURCE_ID::TEXTURES::MASS_FADEOUT));
		animation = get_animation(AID_MISC::MASS_FADEOUT);
	}
	else
		throw std::runtime_error("Bullet::set_hit: unrecognized bullet type");

	animation.row = sprite.get_animation_settings().row;
	animation.horizontally_flipped = sprite.get_animation_settings().horizontally_flipped;
	sprite.set_animation(animation);
}

void Bullet::update(Resources& resources, const sf::Time& dt,
	const hct::dynarray<std::unique_ptr<Solid_Tile>>& solid_tiles,
	const hct::dynarray<std::unique_ptr<Destructible_Tile>>& destructible_tiles)
{
	sprite.update_frame(dt);

	if (!active)
		return;

	sprite.move(movement_vector * dt.asSeconds());
	passed_distance += speed * dt.asSeconds();

	if (passed_distance >= max_distance)
		set_hit(resources);

	auto [solid_left, solid_right] = get_range_of_closest_objects(sprite, solid_tiles, 2.5f);
	auto [destructible_left, destructible_right] = get_range_of_closest_objects(sprite, destructible_tiles, 2.5f);

	for (; solid_left < solid_right; ++solid_left)
		if (collision(get_hitbox(), (*solid_left)->get_hitbox()))
			set_hit(resources);
	for (; destructible_left < destructible_right; ++destructible_left)
		if (!(*destructible_left)->is_broken() && collision(get_hitbox(), (*destructible_left)->get_hitbox()))
		{
			set_hit(resources);
			if (destroy_destructibles)
				(*destructible_left)->set_broken(resources);
		}
}

void Bullet::draw(sf::RenderTarget& rt) const
{
	rt.draw(sprite);
}

// ----------------------------------------------------------------
// Bullet_Manager_Component
// ----------------------------------------------------------------

Bullet_Manager_Component::Bullet_Manager_Component(Bullet::TYPE arg_type, bool arg_destroy_destructibles) :
	type(arg_type), destroy_destructibles(arg_destroy_destructibles) {}

void Bullet_Manager_Component::set_subentities_speed(float arg)
{
	for (auto& bullet : get_subentities())
		bullet->set_speed(arg);
}

void Bullet_Manager_Component::spawn_subentity(Resources& resources, sf::Vector2f position, Bullet::DIRECTION direction, float speed, float max_distance)
{
	create_subentity(resources, position, direction, type, destroy_destructibles, speed, max_distance);
}

Bullet::TYPE theme_to_bullet_color(LEVEL_THEME theme)
{
	switch (theme)
	{
	case (LEVEL_THEME::VERDANT): return Bullet::TYPE::VERDANT;
	case (LEVEL_THEME::WATER): return Bullet::TYPE::WATER;
	case (LEVEL_THEME::FIRE): return Bullet::TYPE::FIRE;
	case (LEVEL_THEME::ELECTRIC): return Bullet::TYPE::ELECTRIC;
	case (LEVEL_THEME::WIND): return Bullet::TYPE::WIND;
	case (LEVEL_THEME::GROUND): return Bullet::TYPE::GROUND;
	case (LEVEL_THEME::LIFEFORCE): return Bullet::TYPE::LIFEFORCE;
	case (LEVEL_THEME::ICE): return Bullet::TYPE::ICE;
	default: throw std::runtime_error("theme_to_bullet_color: invalid theme provided");
	}
}