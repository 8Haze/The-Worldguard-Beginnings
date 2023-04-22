#ifndef _ROCK_H_
#define _ROCK_H_

class Solid_Tile;
class Destructible_Tile;

#include "animatable_sprite.h"
#include "advanced_components.h"
#include "utility.h"

class Rock : private Vertical_Physics_Component<Rock>, private Horizontal_Physics_Component<Rock>
{
private:

	inline static const int rock_shapes = 6;

	Animatable_Sprite sprite;
	bool active = true;

public:

	Rock(Resources& arg_resources, sf::Vector2f arg_pos, LEVEL_THEME arg_theme, float arg_velocity, float arg_gravity);

	sf::FloatRect get_hitbox() const noexcept;
	bool is_active() const noexcept;
	bool is_finished() const noexcept;

	void set_hit(Resources& resources);

	void move(sf::Vector2f vector);
	void update(Resources& resources, const sf::Time& dt,
		const hct::dynarray<std::unique_ptr<Solid_Tile>>& solid_tiles,
		const hct::dynarray<std::unique_ptr<Destructible_Tile>>& destructible_tiles);
	void draw(sf::RenderTarget& rt) const;

};

class Rock_Manager_Component : public Subentity_Manager_Component<Rock, 128>
{
private:

	LEVEL_THEME theme_id;

public:

	Rock_Manager_Component(LEVEL_THEME arg_theme_id);

	void spawn_subentity(Resources& resources, sf::Vector2f position, float velocity = 0.f, float gravity = 0.f);

};

#endif // _ROCK_H_