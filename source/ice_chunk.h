#ifndef _ICE_CHUNK_H_
#define _ICE_CHUNK_H_

class Solid_Tile;
class Destructible_Tile;

#include "animatable_sprite.h"
#include "advanced_components.h"
#include "utility.h"

class Ice_Chunk : private Vertical_Physics_Component<Ice_Chunk>, private Horizontal_Physics_Component<Ice_Chunk>
{
private:

	inline static const float max_distance = 32 * 40;
	inline static const float x_speed = 180.f;

	Animatable_Sprite sprite;
	float passed_distance = 0.f;
	bool active = true;

public:

	enum class DIRECTION 
	{ 
		LEFT,
		RIGHT,
		COUNT
	}; // keep COUNT last

	Ice_Chunk(Resources& arg_resources, sf::Vector2f arg_position, DIRECTION arg_direction);

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

class Ice_Chunk_Manager_Component : public Subentity_Manager_Component<Ice_Chunk, 64>
{
public:

	Ice_Chunk_Manager_Component() = default;

	void spawn_subentity(Resources& resources, sf::Vector2f position, Ice_Chunk::DIRECTION direction);

};

#endif // _ICE_CHUNK_H_