#ifndef _BUBBLE_H_
#define _BUBBLE_H_

class Solid_Tile;
class Destructible_Tile;

#include "animatable_sprite.h"
#include "advanced_components.h"
#include "utility.h"

class Bubble : private Vertical_Physics_Component<Bubble>
{
private:

	inline static const float max_distance = 32 * 15;
	inline static const float gravity_update_factor = 0.3f;
	inline static const int transparency_value = 165;

	Animatable_Sprite sprite;
	float passed_distance = 0.f;
	bool active = true;

public:

	Bubble(Resources& arg_resources, sf::Vector2f arg_pos);

	sf::FloatRect get_hitbox() const noexcept;
	bool is_active() const noexcept;
	bool is_finished() const noexcept;

	void set_popped(Resources& resources);

	void move(sf::Vector2f vector);
	void update(Resources& resources, const sf::Time& dt,
		const hct::dynarray<std::unique_ptr<Solid_Tile>>& solid_tiles,
		const hct::dynarray<std::unique_ptr<Destructible_Tile>>& destructible_tiles);
	void draw(sf::RenderTarget& rt) const;

};

class Bubble_Manager_Component : public Subentity_Manager_Component<Bubble, 8>
{
public:

	Bubble_Manager_Component() = default;

	void spawn_subentity(Resources& resources, sf::Vector2f position);

};

#endif // _BUBBLE_H_