#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <algorithm>

#include "utility.h"
#include "animations.h"
#include "timer.h"
#include "basic_components.h"
#include "advanced_components.h"
#include "lo_bases.h"
#include "lo_tiles.h"
#include "bullet.h"
#include "controls_manager.h"

class Player :
	public Health_Component,
	public Damage_Component,
	public Bullet_Manager_Component,
	public Vertical_Physics_Component<Player>,
	public Horizontal_Physics_Component<Player>
{
public:

	inline static const float default_jump_gravity = -400.f;
	inline static const float default_jump_gravity_cooldown = 600.f;

private:

	inline static const int default_health = 16;
	inline static const int default_damage = 4;

	inline static const float blank_frame_interval = 0.02f;
	inline static const float shooting_interval = 0.15f;

	inline static const float starting_shock_time = 0.5f;
	inline static const float starting_invincibility_time = 1.5f;
	inline static const float starting_game_over_time = 1.5f;
	inline static const float starting_teleport_out_time = 1.f;
	inline static const float starting_teleport_in_time = 0.3f;

	inline static const float ice_velocity_change_speed = 125.f;
	inline static const float forced_movement_speed = 32.f * 6.f;
	inline static const float water_speed_modifier = 0.60f;

	inline static const float temperature_change_rate = 35.f;
	inline static const int extreme_temperature_damage = 1;

	Animatable_Sprite sprite;
	AID_PLAYER current_frame_set = AID_PLAYER::IDLE;

	std::string visible_text;

	sf::Vector2f teleport_destination;

	Timer blank_frame_timer = Timer(blank_frame_interval);
	Timer shooting_timer = Timer(shooting_interval);

	Countdown shock_countdown = Countdown(starting_shock_time);
	Countdown invincibility_countdown = Countdown(starting_invincibility_time);
	Countdown game_over_countdown = Countdown(starting_game_over_time);
	Countdown teleport_out_countdown = Countdown(starting_teleport_out_time);
	Countdown teleport_in_countdown = Countdown(starting_teleport_in_time);

	Resources& resources;

	float temperature = 0.f;

	int points = 0;
	int coins = 0;

	bool visible_frame = true;
	bool jumping = false;
	bool standing = false;
	bool walking = false;
	bool shooting = false;
	bool on_ice = false;
	bool on_hook = false;
	bool in_water = false;
	bool in_cold_air = false;
	bool in_hot_air = false;
	bool in_enchanted_air = false;
	bool forcefully_moved_left = false;
	bool forcefully_moved_right = false;
	bool victorious = false;

	bool can_take_damage() const override;
	void react_to_damage() override;

	void shoot();

	void update_frame(const sf::Time& dt);
	void update_teleporting(const sf::Time& dt);
	void update_invincibility(const sf::Time& dt);
	void update_temperature(const sf::Time& dt);
	void update_shooting(const sf::Time& dt);
	void update_jumping(const sf::Time& dt);
	void update_hooks(const sf::Time& dt);
	void update_enchanted_air(const sf::Time& dt);
	void update_movement(const sf::Time& dt,
		const hct::dynarray<std::unique_ptr<Solid_Tile>>& solid_tiles,
		const hct::dynarray<std::unique_ptr<Destructible_Tile>>& destructible_tiles);

	template <typename Iter>
	void check_vertical_bounds(Iter iBegin, Iter iEnd)
	{
		if (check_vertical_collisions(iBegin, iEnd, get_hitbox()) == Vertical_Physics_Component::COLLISION::BOTTOM)
		{
			standing = true;

			if constexpr (std::is_same_v<typename std::iterator_traits<Iter>::value_type, std::unique_ptr<Destructible_Tile>>)
			{
				sprite.move(0.0f, 8.0f);
				for (auto it = iBegin; it != iEnd; ++it)
					if (collision(get_hitbox(), (*it)->get_hitbox()))
						if (Fragile_Tile* fragile_tile = dynamic_cast<Fragile_Tile*>(&(*(*it))); fragile_tile)
							fragile_tile->start_crack_countdown();
				sprite.move(0.0f, -8.0f);
			}
		}

		if (sprite.getPosition().y < Camera::up_lim)
		{
			sprite.setPosition(sprite.getPosition().x, Camera::up_lim);
			set_gravity_cooldown(0.f);
			set_gravity(0.f);
		}
	}

	template <typename Iter>
	void check_horizontal_bounds(Iter iBegin, Iter iEnd)
	{
		if (check_horizontal_collisions(iBegin, iEnd, get_hitbox(), forcefully_moved_left, forcefully_moved_right) != Horizontal_Physics_Component::COLLISION::NONE)
			walking = false;

		if (sprite.getPosition().x < Camera::left_lim)
		{
			set_velocity(0.f);
			sprite.setPosition(Camera::left_lim, sprite.getPosition().y);
		}
		else if (sprite.getPosition().x > Camera::right_lim)
		{
			set_velocity(0.f);
			sprite.setPosition(Camera::right_lim, sprite.getPosition().y);
		}
	}

public:

	Player(Resources& arg_resources);

	const Animatable_Sprite& get_sprite() const;
	sf::FloatRect get_hitbox();
	sf::FloatRect get_upper_hitbox();
	sf::Vector2f get_position();
	float get_temperature() const;
	int get_points() const;
	int get_coins() const;
	bool is_standing() const;
	bool is_jumping() const;
	bool is_shocked() const;
	bool is_invincible() const;
	bool is_on_hook() const;
	bool is_teleporting_out() const;
	bool is_teleporting_in() const;
	bool is_teleporting() const;
	bool ready_for_victory();
	bool ready_for_game_over() const;
	std::string retrieve_visible_text();

	void set_stats();
	void set_sprite_texture(sf::Texture& arg);
	void set_sprite_origin(sf::Vector2f arg);
	void set_position(sf::Vector2f arg);
	void set_visible_text(const std::string& arg);
	void set_shooting(bool arg);
	void set_on_ice(bool arg);
	void set_on_hook(bool arg);
	void set_in_cold_air(bool arg);
	void set_in_hot_air(bool arg);
	void set_in_water(bool arg);
	void set_in_enchanted_air(bool arg);
	void set_forcefully_moved_left(bool arg);
	void set_forcefully_moved_right(bool arg);
	void set_victorious(bool arg);

	void move(float x, float y);
	void move(sf::Vector2f vector);

	void add_points(int arg);
	void add_coins(int arg);

	void jump(float gravity = default_jump_gravity, float gravity_cooldown = default_jump_gravity_cooldown);
	void initialize_teleport(sf::Vector2f destination);
	void reset_controls();
	void handle_controls(const sf::Event& event, const Controls_Manager& cm);
	void update(const sf::Time& dt,
		const hct::dynarray<std::unique_ptr<Solid_Tile>>& solid_tiles,
		const hct::dynarray<std::unique_ptr<Destructible_Tile>>& destructible_tiles);
	void draw(sf::RenderTexture& rt) const;

};

#endif // _PLAYER_H_