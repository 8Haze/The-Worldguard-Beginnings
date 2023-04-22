#ifndef _BULLET_H_
#define _BULLET_H_

class Solid_Tile;
class Destructible_Tile;

#include "animatable_sprite.h"
#include "advanced_components.h"
#include "resources.h"
#include "utility.h"

class Bullet
{
public:

	inline static const float default_speed = 480.f;
	inline static const float default_max_distance = 320.f;

	enum class DIRECTION 
	{ 
		LEFT,
		RIGHT,
		UP,
		DOWN, 
		COUNT
	}; // keep COUNT last

	enum class TYPE
	{
		PLAYER,
		VERDANT,
		WATER,
		FIRE,
		ELECTRIC,
		WIND,
		GROUND,
		LIFEFORCE,
		ICE,
		AQUA_WAVE,
		CRIMSON_WAVE,
		ICE_MASS,
		COUNT
	}; // keep COUNT last

private:

	Animatable_Sprite sprite;
	sf::Vector2f movement_vector;
	TYPE type;
	float speed;
	float max_distance;
	float passed_distance = 0.f;
	bool active = true;
	bool destroy_destructibles = false;

public:

	Bullet(Resources& arg_resources, sf::Vector2f arg_pos, DIRECTION arg_direction, TYPE arg_type, bool arg_destroy_destructibles, 
		float arg_speed = default_speed, float arg_max_distance = default_max_distance);

	sf::FloatRect get_hitbox() const noexcept;
	bool is_active() const noexcept;
	bool is_finished() const noexcept;

	void set_speed(float arg);
	void set_hit(Resources& resources);

	void update(Resources& resources, const sf::Time& dt,
		const hct::dynarray<std::unique_ptr<Solid_Tile>>& solid_tiles,
		const hct::dynarray<std::unique_ptr<Destructible_Tile>>& destructible_tiles);
	void draw(sf::RenderTarget& rt) const;

};

class Bullet_Manager_Component : public Subentity_Manager_Component<Bullet, 16>
{
private:

	Bullet::TYPE type;
	bool destroy_destructibles;

public:

	Bullet_Manager_Component(Bullet::TYPE arg_type, bool arg_destroy_destructibles);

	void set_subentities_speed(float speed);

	void spawn_subentity(Resources& resources, sf::Vector2f position, Bullet::DIRECTION direction,
		float speed = Bullet::default_speed, float max_distance = Bullet::default_max_distance);

};

Bullet::TYPE theme_to_bullet_color(LEVEL_THEME theme);

#endif // _BULLET_H_