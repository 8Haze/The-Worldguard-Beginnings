#ifndef _LO_ENTITIES_H_
#define _LO_ENTITIES_H_

#include <optional>

#include "lo_bases.h"
#include "lo_tiles.h"
#include "timer.h"
#include "player.h"
#include "bullet.h"
#include "rock.h"
#include "ice_chunk.h"
#include "ball.h"
#include "bubble.h"
#include "boomerang.h"

// ----------------------------------------------------------------
// Base Entity Classes
// ----------------------------------------------------------------

class Stationary_Entity : public LO_Animated
{
public:

	virtual void update(Resources& resources, Player& player, const sf::Time& dt) = 0;

	virtual ~Stationary_Entity() = default;

};

class Colliding_Entity : public LO_Animated
{
public:

	virtual void update(Resources& resources, Player& player, const sf::Time& dt,
		const hct::dynarray<std::unique_ptr<Solid_Tile>>& solid_tiles,
		const hct::dynarray<std::unique_ptr<Destructible_Tile>>& destructible_tiles) = 0;

	virtual ~Colliding_Entity() = default;

};

class Walking_Entity : public LO_Animated
{
private:

	float limit_left = Camera::left_lim - 32.f;
	float limit_right = Camera::right_lim + 32.f;
	bool walking_left = false;

	void check_ground(sf::Vector2f point, bool go_left,
		const hct::dynarray<std::unique_ptr<Solid_Tile>>& solid_tiles);
	void check_blockage(sf::Vector2f point, bool go_left,
		const hct::dynarray<std::unique_ptr<Solid_Tile>>& solid_tiles,
		const hct::dynarray<std::unique_ptr<Destructible_Tile>>& destructible_tiles);

protected:

	void update_walking(const sf::Time& dt, float speed);

public:

	void find_walking_limits(const hct::dynarray<std::unique_ptr<Solid_Tile>>& solid_tiles,
		const hct::dynarray<std::unique_ptr<Destructible_Tile>>& destructible_tiles);
	virtual void update(Resources& resources, Player& player, const sf::Time& dt) = 0;

	virtual ~Walking_Entity() = default;

};

// ----------------------------------------------------------------
// Special Entity Components
// ----------------------------------------------------------------

class Enemy_Component : public Health_Component, public Damage_Component, public Value_Component
{
private:

	inline static const float health_text_display_time = 3.f;

	sf::Text health_text;
	Countdown health_text_display_countdown = Countdown(health_text_display_time);

	virtual void set_death_graphics(Resources&) = 0;

	void react_to_damage() override;
	void react_to_heal() override;
	virtual void extra_player_interaction(Resources& resources, Player& player, const sf::FloatRect& my_hitbox);

protected:

	Enemy_Component(unsigned int health, unsigned int damage, int points);

	void player_interaction(Resources& resources, Player& player, const sf::FloatRect& my_hitbox, bool can_deal_contact_damage, bool can_take_damage);
	void setup_health_info(Resources& resources);
	void update_health_info(const sf::Sprite& sprite, const sf::Time& dt);
	void draw_health_info(sf::RenderTexture& rt) const;

};

// ----------------------------------------------------------------
// Concrete Stationary Entity Classes
// ---------------------------------------------------------------- 

class Trampoline final : public Stationary_Entity
{
public:

	enum class STRENGTH { WEAK, STRONG };

private:

	STRENGTH strength = STRENGTH::WEAK;

	void extra_setup(std::istream& stream, Resources& resources) override;
	void extra_pre_record(std::ostream& stream) const override;
	void extra_post_record(std::ostream& stream) const override;

public:

	Trampoline() = default;

	sf::FloatRect get_hitbox() const;

	void update(Resources& resources, Player& player, const sf::Time& dt) override;

	static Stationary_Entity* make_lo(LEVEL_THEME theme_id);

};

class Thorn final : public Stationary_Entity
{
private:

	inline static const int possible_shapes_count = 15;
	inline static const unsigned int damage_stat = 4u;

	int32_t shape = 0;
	bool active = true;

	void extra_setup(std::istream& stream, Resources& resources) override;
	void extra_pre_record(std::ostream& stream) const override;
	void extra_post_record(std::ostream& stream) const override;

public:

	Thorn(LEVEL_THEME theme_id);

	sf::FloatRect get_hitbox();

	void update(Resources& resources, Player& player, const sf::Time& dt) override;

	static bool available_for_level_theme(LEVEL_THEME theme_id);
	static Stationary_Entity* make_lo(LEVEL_THEME theme_id);

};

class Thorn_Spear_Root final : public Stationary_Entity
{
private:

	inline static const unsigned int damage_stat = 3u;
	inline static const float switch_interval = 1.f;
	inline static bool active = true;
	inline static Timer switch_timer = Timer(switch_interval);

	Animatable_Sprite thorn_spear;

	void extra_setup(std::istream& stream, Resources& resources) override;
	void extra_pre_record(std::ostream& stream) const override;

public:

	Thorn_Spear_Root(LEVEL_THEME theme_id);

	sf::FloatRect get_hitbox();

	void update(Resources& resources, Player& player, const sf::Time& dt) override;
	void draw(sf::RenderTexture& rt) const override;

	static bool available_for_level_theme(LEVEL_THEME theme_id);
	static void update_timer(const sf::Time& dt);
	static Stationary_Entity* make_lo(LEVEL_THEME theme_id);

};

class Mine final : public Stationary_Entity
{
private:

	inline static const unsigned int damage_stat = 6u;

	bool active = true;

	void extra_setup(std::istream& stream, Resources& resources) override;
	void extra_pre_record(std::ostream& stream) const override;

public:

	Mine(LEVEL_THEME theme_id);

	sf::FloatRect get_hitbox() const;

	void update(Resources& resources, Player& player, const sf::Time& dt) override;

	static bool available_for_level_theme(LEVEL_THEME theme_id);
	static Stationary_Entity* make_lo(LEVEL_THEME theme_id);

};

class Healing_Plant final : public Stationary_Entity, public Health_Component
{
private:

	inline static const unsigned int heal_stat = 32u;

	bool active = true;

	void extra_setup(std::istream& stream, Resources& resources) override;
	void extra_pre_record(std::ostream& stream) const override;

public:

	Healing_Plant(LEVEL_THEME theme_id);

	sf::FloatRect get_hitbox() const;

	void update(Resources& resources, Player& player, const sf::Time& dt) override;

	static bool available_for_level_theme(LEVEL_THEME theme_id);
	static Stationary_Entity* make_lo(LEVEL_THEME theme_id);

};

class Wind_Blaster final : public Stationary_Entity
{
private:

	inline static const float switch_interval = 1.f;
	inline static bool active = true;
	inline static Timer switch_timer = Timer(switch_interval);

	void extra_setup(std::istream& stream, Resources& resources) override;
	void extra_pre_record(std::ostream& stream) const override;

public:

	Wind_Blaster(LEVEL_THEME theme_id);

	sf::FloatRect get_hitbox() const;

	void update(Resources& resources, Player& player, const sf::Time& dt) override;

	static bool available_for_level_theme(LEVEL_THEME theme_id);
	static void update_timer(const sf::Time& dt);
	static Stationary_Entity* make_lo(LEVEL_THEME theme_id);

};

class Living_Spikes final : public Stationary_Entity, public Damage_Component
{
private:

	inline static const unsigned int damage_stat = 4u;
	inline static const float switch_interval = 1.5f;
	inline static bool active = true;
	inline static Timer switch_timer = Timer(switch_interval);

	void extra_setup(std::istream& stream, Resources& resources) override;
	void extra_pre_record(std::ostream& stream) const override;

	void set_next_animation();

public:

	Living_Spikes(LEVEL_THEME theme_id);

	sf::FloatRect get_hitbox() const;

	void update(Resources& resources, Player& player, const sf::Time& dt) override;

	static bool available_for_level_theme(LEVEL_THEME theme_id);
	static void update_timer(const sf::Time& dt);
	static Stationary_Entity* make_lo(LEVEL_THEME theme_id);

};

// ----------------------------------------------------------------
// Specific Colliding Entity Classes
// ----------------------------------------------------------------

class Cannon final : public Colliding_Entity, public Damage_Component, public Bullet_Manager_Component
{
private:

	inline static const float shoot_interval = 1.25f;
	inline static const unsigned int damage_stat = 3u;
	inline static Timer shoot_timer = Timer(shoot_interval);

	Bullet::DIRECTION direction = Bullet::DIRECTION::LEFT;

	void extra_setup(std::istream& stream, Resources& resources) override;
	void extra_pre_record(std::ostream& stream) const override;
	void extra_post_record(std::ostream& stream) const override;

public:

	Cannon(LEVEL_THEME theme_id);

	void update(Resources& resources, Player& player, const sf::Time& dt,
		const hct::dynarray<std::unique_ptr<Solid_Tile>>& solid_tiles,
		const hct::dynarray<std::unique_ptr<Destructible_Tile>>& destructible_tiles) override;
	void draw(sf::RenderTexture& rt) const override;

	static void update_timer(const sf::Time& dt);
	static Colliding_Entity* make_lo(LEVEL_THEME theme_id);

};

class Bouncer final : public Colliding_Entity, public Damage_Component, private Vertical_Physics_Component<Bouncer>
{
private:

	inline static const unsigned int damage_stat = 4u;

	void extra_setup(std::istream& stream, Resources& resources) override;
	void extra_pre_record(std::ostream& stream) const override;

	void jump();

public:

	Bouncer(LEVEL_THEME theme_id);

	sf::FloatRect get_hitbox();

	void update(Resources& resources, Player& player, const sf::Time& dt,
		const hct::dynarray<std::unique_ptr<Solid_Tile>>& solid_tiles,
		const hct::dynarray<std::unique_ptr<Destructible_Tile>>& destructible_tiles) override;

	static bool available_for_level_theme(LEVEL_THEME theme_id);
	static Colliding_Entity* make_lo(LEVEL_THEME theme_id);

};

class Falling_Hazard final : public Colliding_Entity, public Damage_Component, private Vertical_Physics_Component<Falling_Hazard>
{
private:

	inline static const float reaction_range = 64.f;
	inline static const unsigned int damage_stat = 3u;

	std::optional<float> limit;

	bool active = true;
	bool awaiting = true;

	void set_hit(Resources& resources);

	void extra_setup(std::istream& stream, Resources& resources) override;
	void extra_pre_record(std::ostream& stream) const override;

public:

	Falling_Hazard(LEVEL_THEME theme_id);

	sf::FloatRect get_hitbox();

	void update(Resources& resources, Player& player, const sf::Time& dt,
		const hct::dynarray<std::unique_ptr<Solid_Tile>>& solid_tiles,
		const hct::dynarray<std::unique_ptr<Destructible_Tile>>& destructible_tiles) override;

	static bool available_for_level_theme(LEVEL_THEME theme_id);
	static Colliding_Entity* make_lo(LEVEL_THEME theme_id);

};

class Rock_Disposer final : public Colliding_Entity, public Damage_Component, public Rock_Manager_Component
{
private:

	inline static const float switch_interval = 1.5f;
	inline static const float dispose_interval = 0.1f;
	inline static const unsigned int damage_stat = 3u;
	inline static bool active = true;
	inline static Timer switch_timer = Timer(switch_interval);
	inline static Timer dispose_timer = Timer(dispose_interval);

	void extra_setup(std::istream& stream, Resources& resources) override;
	void extra_pre_record(std::ostream& stream) const override;

public:

	Rock_Disposer(LEVEL_THEME theme_id);

	void update(Resources& resources, Player& player, const sf::Time& dt,
		const hct::dynarray<std::unique_ptr<Solid_Tile>>& solid_tiles,
		const hct::dynarray<std::unique_ptr<Destructible_Tile>>& destructible_tiles) override;
	void draw(sf::RenderTexture& rt) const override;

	static bool available_for_level_theme(LEVEL_THEME theme_id);
	static void update_timer(const sf::Time& dt);
	static Colliding_Entity* make_lo(LEVEL_THEME theme_id);

};

class Ice_Launcher final : public Colliding_Entity, public Damage_Component, public Ice_Chunk_Manager_Component
{
private:

	inline static const float launch_interval = 1.2f;
	inline static const unsigned int damage_stat = 4u;
	inline static Timer launch_timer = Timer(launch_interval);

	Ice_Chunk::DIRECTION direction = Ice_Chunk::DIRECTION::LEFT;

	void extra_setup(std::istream& stream, Resources& resources) override;
	void extra_pre_record(std::ostream& stream) const override;
	void extra_post_record(std::ostream& stream) const override;

public:

	Ice_Launcher(LEVEL_THEME theme_id);

	void update(Resources& resources, Player& player, const sf::Time& dt,
		const hct::dynarray<std::unique_ptr<Solid_Tile>>& solid_tiles,
		const hct::dynarray<std::unique_ptr<Destructible_Tile>>& destructible_tiles) override;
	void draw(sf::RenderTexture& rt) const override;

	static bool available_for_level_theme(LEVEL_THEME theme_id);
	static void update_timer(const sf::Time& dt);
	static Colliding_Entity* make_lo(LEVEL_THEME theme_id);

};

class Pressure_Orb final : public Colliding_Entity, public Damage_Component, public Bullet_Manager_Component
{
private:

	inline static const unsigned int damage_stat = 4u;

	bool active = true;

	void extra_setup(std::istream& stream, Resources& resources) override;
	void extra_pre_record(std::ostream& stream) const override;

public:

	Pressure_Orb(LEVEL_THEME theme_id);

	sf::FloatRect get_hitbox();

	void update(Resources& resources, Player& player, const sf::Time& dt,
		const hct::dynarray<std::unique_ptr<Solid_Tile>>& solid_tiles,
		const hct::dynarray<std::unique_ptr<Destructible_Tile>>& destructible_tiles) override;
	void draw(sf::RenderTexture& rt) const override;

	static bool available_for_level_theme(LEVEL_THEME theme_id);
	static Colliding_Entity* make_lo(LEVEL_THEME theme_id);

};

class Bubble_Machine final : public Colliding_Entity, public Bubble_Manager_Component
{
private:

	inline static const float dispense_interval = 2.f;
	inline static Timer dispense_timer = Timer(dispense_interval);

	void extra_setup(std::istream& stream, Resources& resources) override;
	void extra_pre_record(std::ostream& stream) const override;

public:

	Bubble_Machine(LEVEL_THEME theme_id);

	void update(Resources& resources, Player& player, const sf::Time& dt,
		const hct::dynarray<std::unique_ptr<Solid_Tile>>& solid_tiles,
		const hct::dynarray<std::unique_ptr<Destructible_Tile>>& destructible_tiles) override;
	void draw(sf::RenderTexture& rt) const override;

	static bool available_for_level_theme(LEVEL_THEME theme_id);
	static void update_timer(const sf::Time& dt);
	static Colliding_Entity* make_lo(LEVEL_THEME theme_id);

};

class Bionic_Ray_Device final : public Colliding_Entity, public Damage_Component
{
public:

	enum class DIRECTION { DOWN, LEFT, RIGHT };

private:

	inline static const float deploy_start_time = 0.5f;
	inline static const float ray_visibility_interval = 0.04f;
	inline static const int inactive_transparency_value = 145;
	inline static const int active_transparency_value = 215;
	inline static const unsigned int min_ray_fragments = 2u;
	inline static const unsigned int max_ray_fragments = 20u;
	inline static const unsigned int damage_stat = 8u;

	hct::dynarray<Animatable_Sprite> ray;

	Timer ray_visibility_timer = Timer(ray_visibility_interval);
	Countdown deploy_countdown = Countdown(deploy_start_time);

	DIRECTION direction = DIRECTION::DOWN;

	bool active = false;
	bool awaiting = true;
	bool visible_ray = true;

	sf::FloatRect get_device_hitbox() const noexcept;
	sf::FloatRect get_ray_fragment_hitbox(const Animatable_Sprite& ray_fragment) const noexcept;

	void set_active() noexcept;

	void extra_setup(std::istream& stream, Resources& resources) override;
	void extra_pre_record(std::ostream& stream) const override;
	void extra_post_record(std::ostream& stream) const override;

	void deploy() noexcept;
	void adjust_to_environment(Resources& resources,
		const hct::dynarray<std::unique_ptr<Solid_Tile>>& solid_tiles,
		const hct::dynarray<std::unique_ptr<Destructible_Tile>>& destructible_tiles);

public:

	Bionic_Ray_Device(LEVEL_THEME theme_id);

	void update(Resources& resources, Player& player, const sf::Time& dt,
		const hct::dynarray<std::unique_ptr<Solid_Tile>>& solid_tiles,
		const hct::dynarray<std::unique_ptr<Destructible_Tile>>& destructible_tiles) override;
	void draw(sf::RenderTexture& rt) const override;

	static bool available_for_level_theme(LEVEL_THEME theme_id);
	static Colliding_Entity* make_lo(LEVEL_THEME theme_id);

};

class Electric_Gate_Device final : public Colliding_Entity, public Damage_Component
{
public:

	enum class DIRECTION { DOWN, LEFT, RIGHT };

private:

	inline static const float switch_interval = 0.75f;
	inline static const int transparency_value = 205;
	inline static const unsigned int min_gate_fragments = 2u;
	inline static const unsigned int max_gate_fragments = 10u;
	inline static const unsigned int damage_stat = 3u;
	inline static bool active = true;
	inline static Timer switch_timer = Timer(switch_interval);

	hct::dynarray<Animatable_Sprite> gate;

	DIRECTION direction = DIRECTION::DOWN;

	sf::FloatRect get_device_hitbox() const noexcept;
	sf::FloatRect get_gate_fragment_hitbox(const Animatable_Sprite& gate_fragment) const noexcept;

	void extra_setup(std::istream& stream, Resources& resources) override;
	void extra_pre_record(std::ostream& stream) const override;
	void extra_post_record(std::ostream& stream) const override;

	void adjust_to_environment(Resources& resources,
		const hct::dynarray<std::unique_ptr<Solid_Tile>>& solid_tiles,
		const hct::dynarray<std::unique_ptr<Destructible_Tile>>& destructible_tiles);

public:

	Electric_Gate_Device(LEVEL_THEME theme_id);

	void update(Resources& resources, Player& player, const sf::Time& dt,
		const hct::dynarray<std::unique_ptr<Solid_Tile>>& solid_tiles,
		const hct::dynarray<std::unique_ptr<Destructible_Tile>>& destructible_tiles) override;
	void draw(sf::RenderTexture& rt) const override;

	static bool available_for_level_theme(LEVEL_THEME theme_id);
	static void update_timer(const sf::Time& dt);
	static Colliding_Entity* make_lo(LEVEL_THEME theme_id);

};

class Flamethrower final : public Colliding_Entity, public Damage_Component
{
private:

	inline static const float spit_interval = 1.25f;
	inline static const int transparency_value = 215;
	inline static const unsigned int min_flame_fragments = 2u;
	inline static const unsigned int max_flame_fragments = 8u;
	inline static const unsigned int damage_stat = 3u;
	inline static Timer spit_timer = Timer(spit_interval);

	hct::dynarray<Animatable_Sprite> flame;

	sf::FloatRect get_device_hitbox() const noexcept;
	sf::FloatRect get_flame_fragment_hitbox(const Animatable_Sprite& flame_fragment) const noexcept;

	void extra_setup(std::istream& stream, Resources& resources) override;
	void extra_pre_record(std::ostream& stream) const override;

	void adjust_to_environment(Resources& resources,
		const hct::dynarray<std::unique_ptr<Solid_Tile>>& solid_tiles,
		const hct::dynarray<std::unique_ptr<Destructible_Tile>>& destructible_tiles);

public:

	Flamethrower(LEVEL_THEME theme_id);

	void update(Resources& resources, Player& player, const sf::Time& dt,
		const hct::dynarray<std::unique_ptr<Solid_Tile>>& solid_tiles,
		const hct::dynarray<std::unique_ptr<Destructible_Tile>>& destructible_tiles) override;
	void draw(sf::RenderTexture& rt) const override;

	static bool available_for_level_theme(LEVEL_THEME theme_id);
	static void update_timer(const sf::Time& dt);
	static Colliding_Entity* make_lo(LEVEL_THEME theme_id);

};

class Caster final : public Colliding_Entity, public Bullet_Manager_Component, public Enemy_Component
{
private:

	inline static const float shoot_interval = 1.0f;
	inline static const unsigned int health_stat = 24u;
	inline static const unsigned int damage_stat = 3u;
	inline static Timer shoot_timer = Timer(shoot_interval);

	void set_death_graphics(Resources& resources) override;

	void extra_setup(std::istream& stream, Resources& resources) override;
	void extra_pre_record(std::ostream& stream) const override;
	void extra_player_interaction(Resources& resources, Player& player, const sf::FloatRect& my_hitbox) override;

public:

	Caster(LEVEL_THEME theme_id);

	sf::FloatRect get_hitbox();

	void update(Resources& resources, Player& player, const sf::Time& dt,
		const hct::dynarray<std::unique_ptr<Solid_Tile>>& solid_tiles,
		const hct::dynarray<std::unique_ptr<Destructible_Tile>>& destructible_tiles) override;
	void draw(sf::RenderTexture& rt) const override;

	static void update_timer(const sf::Time& dt);
	static Colliding_Entity* make_lo(LEVEL_THEME theme_id);

};

class Elemental final : public Colliding_Entity, public Bullet_Manager_Component, public Enemy_Component
{
private:

	inline static const float max_travel_distance = 32.f * 5.f;
	inline static const float shoot_countdown_time = 0.4f;
	inline static const float speed_stat = 280.f;
	inline static const unsigned int health_stat = 12u;
	inline static const unsigned int damage_stat = 3u;

	Countdown shoot_countdown;
	bool moving_down = false;
	bool moving_up = false;

	void set_death_graphics(Resources& resources) override;

	void extra_setup(std::istream& stream, Resources& resources) override;
	void extra_pre_record(std::ostream& stream) const override;
	void extra_player_interaction(Resources& resources, Player& player, const sf::FloatRect& my_hitbox) override;

public:

	Elemental(LEVEL_THEME theme_id);

	sf::FloatRect get_hitbox();

	void update(Resources& resources, Player& player, const sf::Time& dt,
		const hct::dynarray<std::unique_ptr<Solid_Tile>>& solid_tiles,
		const hct::dynarray<std::unique_ptr<Destructible_Tile>>& destructible_tiles) override;
	void draw(sf::RenderTexture& rt) const override;

	static Colliding_Entity* make_lo(LEVEL_THEME theme_id);

};

class Jungle_Boomeranger final : public Colliding_Entity, public Boomerang_Manager_Component, public Enemy_Component
{
private:

	inline static const float throw_interval = 1.2f;
	inline static const unsigned int health_stat = 24u;
	inline static const unsigned int damage_stat = 4u;
	inline static Timer throw_timer = Timer(throw_interval);

	void set_death_graphics(Resources& resources) override;

	void extra_player_interaction(Resources& resources, Player& player, const sf::FloatRect& my_hitbox) override;
	void extra_setup(std::istream& stream, Resources& resources) override;
	void extra_pre_record(std::ostream& stream) const override;

public:

	Jungle_Boomeranger(LEVEL_THEME theme_id);

	sf::FloatRect get_hitbox();

	void update(Resources& resources, Player& player, const sf::Time& dt,
		const hct::dynarray<std::unique_ptr<Solid_Tile>>& solid_tiles,
		const hct::dynarray<std::unique_ptr<Destructible_Tile>>& destructible_tiles) override;
	void draw(sf::RenderTexture& rt) const override;

	static bool available_for_level_theme(LEVEL_THEME theme_id);
	static void update_timer(const sf::Time& dt);
	static Colliding_Entity* make_lo(LEVEL_THEME theme_id);

};

class Aquaveil_Weaver final : public Colliding_Entity, public Bullet_Manager_Component, public Enemy_Component
{
private:

	inline static const float veil_creation_countdown_time = 1.f;
	inline static const float veil_release_countdown_time = 1.f;
	inline static const unsigned int health_stat = 48u;
	inline static const unsigned int damage_stat = 6u;

	Countdown veil_creation_countdown = Countdown(veil_creation_countdown_time);
	Countdown veil_release_countdown = Countdown(veil_release_countdown_time);
	bool veils_created = false;

	void set_death_graphics(Resources& resources) override;

	void extra_player_interaction(Resources& resources, Player& player, const sf::FloatRect& my_hitbox) override;
	void extra_setup(std::istream& stream, Resources& resources) override;
	void extra_pre_record(std::ostream& stream) const override;

public:

	Aquaveil_Weaver(LEVEL_THEME theme_id);

	sf::FloatRect get_hitbox();

	void update(Resources& resources, Player& player, const sf::Time& dt,
		const hct::dynarray<std::unique_ptr<Solid_Tile>>& solid_tiles,
		const hct::dynarray<std::unique_ptr<Destructible_Tile>>& destructible_tiles) override;
	void draw(sf::RenderTexture& rt) const override;

	static bool available_for_level_theme(LEVEL_THEME theme_id);
	static Colliding_Entity* make_lo(LEVEL_THEME theme_id);

};

class Energized_Galvanizer final : public Colliding_Entity, public Ball_Manager_Component, public Enemy_Component, private Vertical_Physics_Component<Energized_Galvanizer>
{
private:

	inline static const float veil_creation_countdown_time = 1.f;
	inline static const float veil_release_countdown_time = 1.f;
	inline static const unsigned int health_stat = 16u;
	inline static const unsigned int damage_stat = 3u;

	bool balls_thrown = false;

	void set_death_graphics(Resources& resources) override;

	void extra_player_interaction(Resources& resources, Player& player, const sf::FloatRect& my_hitbox) override;
	void extra_setup(std::istream& stream, Resources& resources) override;
	void extra_pre_record(std::ostream& stream) const override;

	void jump();

public:

	Energized_Galvanizer(LEVEL_THEME theme_id);

	sf::FloatRect get_hitbox();

	void update(Resources& resources, Player& player, const sf::Time& dt,
		const hct::dynarray<std::unique_ptr<Solid_Tile>>& solid_tiles,
		const hct::dynarray<std::unique_ptr<Destructible_Tile>>& destructible_tiles) override;
	void draw(sf::RenderTexture& rt) const override;

	static bool available_for_level_theme(LEVEL_THEME theme_id);
	static Colliding_Entity* make_lo(LEVEL_THEME theme_id);

};

class Aerial_Bombardier final : public Colliding_Entity, public Ball_Manager_Component, public Enemy_Component
{
private:

	inline static const float throw_interval = 0.85f;
	inline static const unsigned int health_stat = 24u;
	inline static const unsigned int damage_stat = 3u;
	inline static Timer throw_timer = Timer(throw_interval);

	void set_death_graphics(Resources& resources) override;

	void extra_player_interaction(Resources& resources, Player& player, const sf::FloatRect& my_hitbox) override;
	void extra_setup(std::istream& stream, Resources& resources) override;
	void extra_pre_record(std::ostream& stream) const override;

public:

	Aerial_Bombardier(LEVEL_THEME theme_id);

	sf::FloatRect get_hitbox();

	void update(Resources& resources, Player& player, const sf::Time& dt,
		const hct::dynarray<std::unique_ptr<Solid_Tile>>& solid_tiles,
		const hct::dynarray<std::unique_ptr<Destructible_Tile>>& destructible_tiles) override;
	void draw(sf::RenderTexture& rt) const override;

	static bool available_for_level_theme(LEVEL_THEME theme_id);
	static void update_timer(const sf::Time& dt);
	static Colliding_Entity* make_lo(LEVEL_THEME theme_id);

};

class Rock_Hurler final : public Colliding_Entity, public Rock_Manager_Component, public Enemy_Component
{
private:

	inline static const float hurl_interval = 1.25f;
	inline static const unsigned int health_stat = 48u;
	inline static const unsigned int damage_stat = 3u;
	inline static const int rocks_per_hurl = 4;

	inline static Timer hurl_timer = Timer(hurl_interval);

	void set_death_graphics(Resources& resources) override;

	void extra_player_interaction(Resources& resources, Player& player, const sf::FloatRect& my_hitbox) override;
	void extra_setup(std::istream& stream, Resources& resources) override;
	void extra_pre_record(std::ostream& stream) const override;

public:

	Rock_Hurler(LEVEL_THEME theme_id);

	sf::FloatRect get_hitbox();

	void update(Resources& resources, Player& player, const sf::Time& dt,
		const hct::dynarray<std::unique_ptr<Solid_Tile>>& solid_tiles,
		const hct::dynarray<std::unique_ptr<Destructible_Tile>>& destructible_tiles) override;
	void draw(sf::RenderTexture& rt) const override;

	static bool available_for_level_theme(LEVEL_THEME theme_id);
	static void update_timer(const sf::Time& dt);
	static Colliding_Entity* make_lo(LEVEL_THEME theme_id);

};

class Crimson_Lasher final : public Colliding_Entity, public Bullet_Manager_Component, public Enemy_Component
{
private:

	inline static const float shoot_interval = 1.25f;
	inline static const unsigned int health_stat = 32u;
	inline static const unsigned int damage_stat = 6u;
	inline static Timer shoot_timer = Timer(shoot_interval);

	void set_death_graphics(Resources& resources) override;

	void extra_player_interaction(Resources& resources, Player& player, const sf::FloatRect& my_hitbox) override;
	void extra_setup(std::istream& stream, Resources& resources) override;
	void extra_pre_record(std::ostream& stream) const override;

public:

	Crimson_Lasher(LEVEL_THEME theme_id);

	sf::FloatRect get_hitbox();

	void update(Resources& resources, Player& player, const sf::Time& dt,
		const hct::dynarray<std::unique_ptr<Solid_Tile>>& solid_tiles,
		const hct::dynarray<std::unique_ptr<Destructible_Tile>>& destructible_tiles) override;
	void draw(sf::RenderTexture& rt) const override;

	static bool available_for_level_theme(LEVEL_THEME theme_id);
	static void update_timer(const sf::Time& dt);
	static Colliding_Entity* make_lo(LEVEL_THEME theme_id);

};

class Icebaner : public Colliding_Entity, public Bullet_Manager_Component, public Enemy_Component
{
private:

	inline static const float shoot_interval = 0.6f;
	inline static const float shoot_break_time = 1.2f;
	inline static const unsigned int health_stat = 32u;
	inline static const unsigned int damage_stat = 4u;
	inline static const int bullets_per_assault = 8;

	Timer shoot_timer = Timer(shoot_interval);
	Countdown shoot_break = Countdown(shoot_break_time);
	int remaining_bullets = 0;

	void set_death_graphics(Resources& resources) override;

	void extra_player_interaction(Resources& resources, Player& player, const sf::FloatRect& my_hitbox) override;
	void extra_setup(std::istream& stream, Resources& resources) override;
	void extra_pre_record(std::ostream& stream) const override;


public:

	Icebaner(LEVEL_THEME theme_id);

	sf::FloatRect get_hitbox();

	void update(Resources& resources, Player& player, const sf::Time& dt,
		const hct::dynarray<std::unique_ptr<Solid_Tile>>& solid_tiles,
		const hct::dynarray<std::unique_ptr<Destructible_Tile>>& destructible_tiles) override;
	void draw(sf::RenderTexture& rt) const override;

	static bool available_for_level_theme(LEVEL_THEME theme_id);
	static Colliding_Entity* make_lo(LEVEL_THEME theme_id);

};

// ----------------------------------------------------------------
// Specific Walking Entity Classes
// ----------------------------------------------------------------

class Roaming_Trampoline : public Walking_Entity
{
private:

	inline static const float speed_stat = 240.f;

	void extra_setup(std::istream& stream, Resources& resources) override;
	void extra_pre_record(std::ostream& stream) const override;

public:

	Roaming_Trampoline(LEVEL_THEME theme_id);

	sf::FloatRect get_hitbox() const;

	void update(Resources& resources, Player& player, const sf::Time& dt) override;

	static bool available_for_level_theme(LEVEL_THEME theme_id);
	static Walking_Entity* make_lo(LEVEL_THEME theme_id);

};

class Scouter : public Walking_Entity, public Damage_Component
{
private:

	inline static const float speed_stat = 320.f;
	inline static const unsigned int damage_stat = 3u;

	void extra_setup(std::istream& stream, Resources& resources) override;
	void extra_pre_record(std::ostream& stream) const override;

public:

	Scouter(LEVEL_THEME theme_id);

	sf::FloatRect get_hitbox();

	void update(Resources& resources, Player& player, const sf::Time& dt) override;

	static bool available_for_level_theme(LEVEL_THEME theme_id);
	static Walking_Entity* make_lo(LEVEL_THEME theme_id);

};

class Warrior : public Walking_Entity, public Enemy_Component
{
private:

	inline static const float speed_stat = 320.f;
	inline static const unsigned int health_stat = 16u;
	inline static const unsigned int damage_stat = 4u;

	void set_death_graphics(Resources& resources) override;

	void extra_setup(std::istream& stream, Resources& resources) override;
	void extra_pre_record(std::ostream& stream) const override;

public:

	Warrior(LEVEL_THEME theme_id);

	sf::FloatRect get_hitbox();

	void update(Resources& resources, Player& player, const sf::Time& dt) override;
	void draw(sf::RenderTexture& rt) const override;

	static Walking_Entity* make_lo(LEVEL_THEME theme_id);

};

class Golem : public Walking_Entity, public Enemy_Component
{
private:

	inline static const float speed_stat = 200.f;
	inline static const float regeneration_interval = 1.f;
	inline static const unsigned int health_stat = 40u;
	inline static const unsigned int damage_stat = 3u;
	inline static const unsigned int regeneration_stat = 4u;

	Timer regeneration_timer = Timer(regeneration_interval);

	void set_death_graphics(Resources& resources) override;

	void extra_setup(std::istream& stream, Resources& resources) override;
	void extra_pre_record(std::ostream& stream) const override;

public:

	Golem(LEVEL_THEME theme_id);

	sf::FloatRect get_hitbox();

	void update(Resources& resources, Player& player, const sf::Time& dt) override;
	void draw(sf::RenderTexture& rt) const override;

	static Walking_Entity* make_lo(LEVEL_THEME theme_id);

};

class Scorcher : public Walking_Entity, public Enemy_Component
{
private:

	inline static const float speed_stat = 280.f;
	inline static const unsigned int health_stat = 8u;
	inline static const unsigned int damage_stat = 4u;

	void set_death_graphics(Resources& resources) override;

	void extra_setup(std::istream& stream, Resources& resources) override;
	void extra_pre_record(std::ostream& stream) const override;

public:

	Scorcher(LEVEL_THEME theme_id);

	sf::FloatRect get_hitbox();

	void update(Resources& resources, Player& player, const sf::Time& dt) override;
	void draw(sf::RenderTexture& rt) const override;

	static bool available_for_level_theme(LEVEL_THEME theme_id);
	static Walking_Entity* make_lo(LEVEL_THEME theme_id);

};

#endif // _LO_ENTITIES_H_