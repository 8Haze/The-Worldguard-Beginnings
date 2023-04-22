#ifndef _LO_TILES_H_
#define _LO_TILES_H_

#include "lo_bases.h"
#include "animations.h"
#include "basic_components.h"
#include "timer.h"

class Player;

RESOURCE_ID::TEXTURES level_theme_to_tileset_id(LEVEL_THEME id);
int max_index_by_tileset_id(LEVEL_THEME id);

// ----------------------------------------------------------------
// Tile Component Classes
// ----------------------------------------------------------------

class Tile_Layer_Component
{
public:

	enum class LAYER 
	{
		FARTHEST_BACK, 
		FAR_BACK,
		BACK,
		FAR_FRONT,
		CLOSE_FRONT,
		COUNT
	}; // keep COUNT last

private:

	LAYER layer = LAYER::FARTHEST_BACK;

protected:

	void read_layer(std::istream& stream);

public:

	LAYER get_layer() const noexcept;

};

// ----------------------------------------------------------------
// Concrete Tile Classes
// ----------------------------------------------------------------

class Decorative_Tile final : public LO_Static, public Tile_Layer_Component
{
private:

	std::int32_t index_x = 0;
	std::int32_t index_y = 0;

	void extra_setup(std::istream& stream, Resources& resources) override;
	void extra_pre_record(std::ostream& stream) const override;
	void extra_post_record(std::ostream& stream) const override;

public:

	Decorative_Tile(LEVEL_THEME arg_theme_id, Resources& arg_resources);

	static Decorative_Tile* make_lo(LEVEL_THEME theme_id, Resources& resources);

	friend bool operator<(const Decorative_Tile& lhs, const Decorative_Tile& rhs) noexcept;

};

class Animated_Tile final : public LO_Animated, public Tile_Layer_Component
{
private:

	AID_TILE animation_id;

	void extra_setup(std::istream& stream, Resources& resources) override;
	void extra_pre_record(std::ostream& stream) const override;
	void extra_post_record(std::ostream& stream) const override;

public:

	Animated_Tile() = default;

	static Animated_Tile* make_lo();

	friend bool operator<(const Animated_Tile& lhs, const Animated_Tile& rhs) noexcept;

};

class Solid_Tile final : public LO_Static
{
private:

	std::int32_t index_x = 0;
	std::int32_t index_y = 0;

	virtual void extra_setup(std::istream& stream, Resources& resources) override;
	void extra_pre_record(std::ostream& stream) const override;
	void extra_post_record(std::ostream& stream) const override;

public:

	Solid_Tile(LEVEL_THEME arg_theme_id, Resources& arg_resources);

	sf::FloatRect get_hitbox() const noexcept;

	static Solid_Tile* make_lo(LEVEL_THEME theme_id, Resources& resources);

};

class Destructible_Tile : public LO_Animated
{
private:

	std::int32_t index_x = 0;
	std::int32_t index_y = 0;
	bool broken = false;

	void extra_setup(std::istream& stream, Resources& resources) override;
	void extra_pre_record(std::ostream& stream) const override;
	void extra_post_record(std::ostream& stream) const override;
	virtual void extra_update(Resources& resources, const sf::Time& dt);

public:

	Destructible_Tile(LEVEL_THEME arg_theme_id, Resources& arg_resources);

	sf::FloatRect get_hitbox() const noexcept;
	bool is_broken() const noexcept;

	void set_broken(Resources& resources);

	void update(Resources& resources, const sf::Time& dt);

	static Destructible_Tile* make_lo(LEVEL_THEME theme_id, Resources& resources);

};

class Fragile_Tile final : public Destructible_Tile
{
private:

	inline static float crack_countdown_starting_time = 0.5f;

	Countdown crack_countdown = Countdown(crack_countdown_starting_time);
	bool countdown_started = false;

	void extra_pre_record(std::ostream& stream) const override;
	void extra_update(Resources& resources, const sf::Time& dt) override;

public:

	Fragile_Tile(LEVEL_THEME arg_theme_id, Resources& arg_resources);

	void start_crack_countdown() noexcept;

	static Destructible_Tile* make_lo(LEVEL_THEME theme_id, Resources& resources);

};

class Interactive_Tile : public LO_Static
{
private:

	inline static const int transparency_value = 165;

	virtual void extra_interactive_setup(std::istream& stream);
	void extra_setup(std::istream& stream, Resources& resources) override;
	void extra_post_record(std::ostream& stream) const override;
	virtual void set_desired_sprite_details() = 0;

public:

	Interactive_Tile() = default;

	virtual sf::FloatRect get_hitbox() const noexcept = 0;

	virtual void player_interaction(Player& player) = 0;

};

// ----------------------------------------------------------------
// Specific Interactive Tile Classes
// ----------------------------------------------------------------

class Hazardous_Tile final : public Interactive_Tile, private Damage_Component
{
private:

	inline static const unsigned int damage_stat = 4u;

	void set_desired_sprite_details() override;
	void extra_pre_record(std::ostream& stream) const override;

public:

	Hazardous_Tile();

	sf::FloatRect get_hitbox() const noexcept override;

	void player_interaction(Player& player) override;

	static Interactive_Tile* make_lo();

};

class Ice_Tile final : public Interactive_Tile
{
private:

	void set_desired_sprite_details() override;
	void extra_pre_record(std::ostream& stream) const override;

public:

	Ice_Tile() = default;

	sf::FloatRect get_hitbox() const noexcept override;

	void player_interaction(Player& player) override;

	static Interactive_Tile* make_lo();

};

class Hook_Tile final : public Interactive_Tile
{
private:

	void set_desired_sprite_details() override;
	void extra_pre_record(std::ostream& stream) const override;

public:

	Hook_Tile() = default;

	sf::FloatRect get_hitbox() const noexcept override;

	void player_interaction(Player& player) override;

	static Interactive_Tile* make_lo();

};

class Cold_Air_Tile final : public Interactive_Tile
{
private:

	void set_desired_sprite_details() override;
	void extra_pre_record(std::ostream& stream) const override;

public:

	Cold_Air_Tile() = default;

	sf::FloatRect get_hitbox() const noexcept override;

	void player_interaction(Player& player) override;

	static Interactive_Tile* make_lo();

};

class Hot_Air_Tile final : public Interactive_Tile
{
private:

	void set_desired_sprite_details() override;
	void extra_pre_record(std::ostream& stream) const override;

public:

	Hot_Air_Tile() = default;

	sf::FloatRect get_hitbox() const noexcept override;

	void player_interaction(Player& player) override;

	static Interactive_Tile* make_lo();

};

class Water_Tile final : public Interactive_Tile
{
private:

	void set_desired_sprite_details() override;
	void extra_pre_record(std::ostream& stream) const override;

public:

	Water_Tile() = default;

	sf::FloatRect get_hitbox() const noexcept override;

	void player_interaction(Player& player) override;

	static Interactive_Tile* make_lo();

};

class Conveyor_Belt_Tile final : public Interactive_Tile
{
private:

	bool moves_left = true;

	void set_desired_sprite_details() override;
	void extra_interactive_setup(std::istream& stream) override;
	void extra_pre_record(std::ostream& stream) const override;
	void extra_post_record(std::ostream& stream) const override;

public:

	Conveyor_Belt_Tile() = default;

	sf::FloatRect get_hitbox() const noexcept override;

	void player_interaction(Player& player) override;

	static Interactive_Tile* make_lo();

};

class Info_Sign_Tile final : public Interactive_Tile
{
private:

	std::string string;

	void set_desired_sprite_details() override;
	void extra_interactive_setup(std::istream& stream) override;
	void extra_pre_record(std::ostream& stream) const override;
	void extra_post_record(std::ostream& stream) const override;

public:

	Info_Sign_Tile() = default;

	sf::FloatRect get_hitbox() const noexcept override;

	void player_interaction(Player& player) override;

	static Interactive_Tile* make_lo();

};

class Anti_Gravity_Tile final : public Interactive_Tile
{
private:

	void set_desired_sprite_details() override;
	void extra_pre_record(std::ostream& stream) const override;

public:

	Anti_Gravity_Tile() = default;

	sf::FloatRect get_hitbox() const noexcept override;

	void player_interaction(Player& player) override;

	static Interactive_Tile* make_lo();

};

class Warp_Tile final : public Interactive_Tile
{
private:

	sf::Vector2f destination;
	int32_t coins_required = 0;

	void set_desired_sprite_details() override;
	void extra_interactive_setup(std::istream& stream) override;
	void extra_pre_record(std::ostream& stream) const override;
	void extra_post_record(std::ostream& stream) const override;

public:

	Warp_Tile() = default;

	sf::FloatRect get_hitbox() const noexcept override;

	void player_interaction(Player& player) override;

	static Interactive_Tile* make_lo();

};

class Finish_Warp_Tile final : public Interactive_Tile
{
private:

	void set_desired_sprite_details() override;
	void extra_pre_record(std::ostream& stream) const override;

public:

	Finish_Warp_Tile() = default;

	sf::FloatRect get_hitbox() const noexcept override;

	void player_interaction(Player& player) override;

	static Interactive_Tile* make_lo();

};

#endif // _LO_TILES_H_