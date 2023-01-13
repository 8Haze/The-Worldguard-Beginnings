#ifndef _BALL_H_
#define _BALL_H_

class Solid_Tile;
class Destructible_Tile;

#include "animatable_sprite.h"
#include "advanced_components.h"
#include "utility.h"

class Ball : private Vertical_Physics_Component<Ball>, private Horizontal_Physics_Component<Ball>
{
private:

	Animatable_Sprite sprite;
	float bounce_gravity;
	float bounce_gravity_cooldown;
	int bounces_left;
	bool active = true;

public:

	Ball(Resources& arg_resources, sf::Vector2f arg_position, LEVEL_THEME arg_theme, 
		float arg_velocity, float arg_gravity, float arg_gravity_cooldown, int arg_bounces);

	sf::FloatRect get_hitbox() const noexcept;
	bool is_active() const noexcept;
	bool is_finished() const noexcept;

	void set_hit(Resources& resources);
	void move(sf::Vector2f vector);
	void update(Resources& resources, sf::Time dt,
		const hct::dynarray<std::unique_ptr<Solid_Tile>>& solid_tiles,
		const hct::dynarray<std::unique_ptr<Destructible_Tile>>& destructible_tiles);
	void draw(sf::RenderTarget& rt) const;

};

class Ball_Manager_Component : public Subentity_Manager_Component<Ball, 16>
{
private:

	LEVEL_THEME theme_id;

public:

	Ball_Manager_Component(LEVEL_THEME arg_theme_id);

	void spawn_subentity(Resources& resources, sf::Vector2f position, 
		float velocity, float gravity, float gravity_cooldown, int bounces);

};


#endif // _BALL_H_