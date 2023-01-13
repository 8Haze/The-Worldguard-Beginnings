#ifndef _BOOMERANG_H_
#define _BOOMERANG_H_

class Solid_Tile;
class Destructible_Tile;

#include "animatable_sprite.h"
#include "advanced_components.h"
#include "resources.h"
#include "utility.h"

class Boomerang : private Horizontal_Physics_Component<Boomerang>
{
public:

	inline static const float default_max_speed = 400.f;
	inline static const float default_falloff_distance = 32.f * 20.f;

	enum class DIRECTION 
	{ 
		LEFT,
		RIGHT,
		COUNT
	}; // keep COUNT last

private:

	Animatable_Sprite sprite;
	sf::Vector2f throw_position;
	float falloff_distance = default_falloff_distance;
	float passed_distance = 0.f;
	bool returning = false;
	bool active = true;

public:

	Boomerang(Resources& arg_resources, sf::Vector2f arg_position, DIRECTION arg_direction, 
		LEVEL_THEME arg_theme, float arg_max_speed, float arg_falloff_distance);

	sf::FloatRect get_hitbox() const noexcept;
	bool is_active() const noexcept;
	bool is_finished() const noexcept;

	void move(sf::Vector2f vector);
	void update(Resources& resources, const sf::Time& dt,
		const hct::dynarray<std::unique_ptr<Solid_Tile>>& solid_tiles,
		const hct::dynarray<std::unique_ptr<Destructible_Tile>>& destructible_tiles);
	void draw(sf::RenderTarget& rt) const;

};

class Boomerang_Manager_Component : public Subentity_Manager_Component<Boomerang, 8>
{
private:

	LEVEL_THEME theme_id;

public:

	Boomerang_Manager_Component(LEVEL_THEME arg_theme_id);

	void spawn_subentity(Resources& resources, sf::Vector2f position, Boomerang::DIRECTION direction,
		float max_speed = Boomerang::default_max_speed, float falloff_distance = Boomerang::default_falloff_distance);

};

#endif // _BOOMERANG_H_