#include "player.h"

// ----------------------------------------------------------------
// Player
// ----------------------------------------------------------------

Player::Player(Resources& arg_resources) :
	resources(arg_resources),
	Vertical_Physics_Component(*this),
	Horizontal_Physics_Component(*this),
	Bullet_Manager_Component(Bullet::TYPE::PLAYER, true)
{
	sprite.setTexture(resources.textures.get(RESOURCE_ID::TEXTURES::NAZAR));
	sprite.set_animation(Animation_Settings(0, 0, 6, 48, 48, 0.75f, true));
	set_center(sprite);
	set_stats();
}

bool Player::can_take_damage() const
{
	return invincibility_countdown.is_done() && !is_teleporting();
}

void Player::react_to_damage()
{
	shock_countdown.restart();
	invincibility_countdown.restart();

	set_velocity(0.f);
	set_gravity(0.f);

	if (!is_alive())
	{
		game_over_countdown.restart();
		sprite.setTexture(resources.textures.get(RESOURCE_ID::TEXTURES::EXPLOSION_48));
		sprite.set_animation(get_animation(AID_MISC::EXPLOSION_48));
	}
}

void Player::shoot()
{
	if (!is_alive() || is_shocked() || is_teleporting())
		return;

	sf::Vector2f bullet_pos = { sprite.getPosition().x + (is_faced_left() ? -16.f : 16.f), sprite.getPosition().y };
	spawn_subentity(resources, bullet_pos, is_faced_left() ? Bullet::DIRECTION::LEFT : Bullet::DIRECTION::RIGHT);
}

void Player::update_frame(const sf::Time& dt)
{
	AID_PLAYER old_frame_set = current_frame_set;

	if (!is_alive())
	{
		sprite.update_frame(dt);
		return;
	}

	if (is_shocked()) current_frame_set = AID_PLAYER::SHOCKED;
	else if (is_teleporting_out()) current_frame_set = AID_PLAYER::TELEPORT_OUT;
	else if (is_teleporting_in()) current_frame_set = AID_PLAYER::TELEPORT_IN;
	else if (standing) current_frame_set = (is_going_left() == is_going_right() ? AID_PLAYER::IDLE : AID_PLAYER::WALKING);
	else if (on_hook) current_frame_set = AID_PLAYER::ON_HOOK;
	else current_frame_set = AID_PLAYER::IN_AIR;

	if (old_frame_set != current_frame_set)
		sprite.set_animation(get_animation(current_frame_set));

	sprite.get_animation_settings().horizontally_flipped = is_faced_left();
	sprite.get_animation_settings().row = sprite.get_animation_settings().row + (shooting && !is_shocked() && !is_teleporting() ? 1 : 0);
	sprite.update_frame(dt);
	sprite.get_animation_settings().row = sprite.get_animation_settings().row - (shooting && !is_shocked() && !is_teleporting() ? 1 : 0);
}

void Player::update_teleporting(const sf::Time& dt)
{
	if (is_teleporting_out())
	{
		teleport_out_countdown.update(dt);
		if (teleport_out_countdown.is_done() && !victorious)
		{
			teleport_in_countdown.restart();
			sprite.setPosition(teleport_destination);
			teleport_destination.x = 0.f;
			teleport_destination.y = 0.f;
		}
		return;
	}
	else if (is_teleporting_in())
	{
		teleport_in_countdown.update(dt);
		return;
	}
}

void Player::update_invincibility(const sf::Time& dt)
{
	shock_countdown.update(dt);
	invincibility_countdown.update(dt);

	if (is_invincible())
	{
		blank_frame_timer.update(dt);
		if (blank_frame_timer.is_ready())
			visible_frame = !visible_frame;
	}
	else
		visible_frame = true;
}

void Player::update_temperature(const sf::Time& dt)
{
	if (in_cold_air && temperature > -100.f)
	{
		temperature -= dt.asSeconds() * temperature_change_rate;
		if (temperature < -100.f)
			temperature = -100.f;
	}
	else if (in_hot_air && temperature < 100.f)
	{
		temperature += dt.asSeconds() * temperature_change_rate;
		if (temperature > 100.f)
			temperature = 100.f;
	}
	else if (temperature != 0.0f)
	{
		if (temperature < 0.0f)
		{
			temperature += dt.asSeconds() * temperature_change_rate * 0.5f;
			if (temperature > 0.0f)
				temperature = 0.0f;
		}
		else
		{
			temperature -= dt.asSeconds() * temperature_change_rate * 0.5f;
			if (temperature < 0.0f)
				temperature = 0.0f;
		}
	}

	set_in_cold_air(false);
	set_in_hot_air(false);

	if (temperature <= -100.f || temperature >= 100.f)
		take_damage(extreme_temperature_damage);
}

void Player::update_shooting(const sf::Time& dt)
{
	if (shooting)
	{
		shooting_timer.update(dt);
		if (shooting_timer.is_ready())
			shoot();
	}
	else
		shooting_timer.restart();
}

void Player::update_jumping(const sf::Time& dt)
{
	if (!is_jumping())
		set_gravity_cooldown(0.f);
	else if ((is_standing() || is_on_hook()))
		jump(default_jump_gravity, default_jump_gravity_cooldown);
}

void Player::update_hooks(const sf::Time& dt)
{
	if (is_on_hook())
	{
		set_gravity(0.0f);
		set_velocity(0.0f);
	}
}

void Player::update_enchanted_air(const sf::Time& dt)
{
	if (in_enchanted_air)
	{
		set_reversed_gravity(true);
		in_enchanted_air = false;
	}
	else
		set_reversed_gravity(false);
}

void Player::update_movement(const sf::Time& dt,
	const hct::dynarray<std::unique_ptr<Solid_Tile>>& solid_tiles,
	const hct::dynarray<std::unique_ptr<Destructible_Tile>>& destructible_tiles)
{
	auto [collidable_left, collidable_right] = get_range_of_closest_objects(sprite, solid_tiles, 1.5f);
	auto [destructible_left, destructible_right] = get_range_of_closest_objects(sprite, destructible_tiles, 1.5f);

	sprite.move(0.f, (in_water ? water_speed_modifier : 1.0f) * update_vertically(dt));

	check_vertical_bounds(collidable_left, collidable_right);
	check_vertical_bounds(destructible_left, destructible_right);

	set_velocity_change_speed((on_ice && standing) ? ice_velocity_change_speed : get_default_velocity_change_speed());

	update_jumping(dt);

	if ((forcefully_moved_left || forcefully_moved_right) && is_standing() && !is_on_hook() && !is_shocked())
	{
		sprite.move((in_water ? water_speed_modifier : 1.0f) * (forcefully_moved_left ? -forced_movement_speed : forced_movement_speed) * dt.asSeconds(), 0.f);
		check_horizontal_bounds(collidable_left, collidable_right);
		check_horizontal_bounds(destructible_left, destructible_right);
	}

	sprite.move((in_water ? water_speed_modifier : 1.0f) * update_horizontally(dt), 0.f);

	check_horizontal_bounds(collidable_left, collidable_right);
	check_horizontal_bounds(destructible_left, destructible_right);

	forcefully_moved_left = false;
	forcefully_moved_right = false;
}

const Animatable_Sprite& Player::get_sprite() const
{
	return sprite;
}

sf::FloatRect Player::get_hitbox()
{
	return ::get_hitbox(sprite, 0.4f, 1.f);
}

sf::FloatRect Player::get_upper_hitbox()
{
	auto res = get_hitbox(); res.height = 1.f; return res;
}

sf::Vector2f Player::get_position()
{
	return sprite.getPosition();
}

float Player::get_temperature() const
{
	return temperature;
}

int Player::get_points() const
{
	return points;
}

int Player::get_coins() const
{
	return coins;
}

bool Player::is_standing() const
{
	return standing;
}

bool Player::is_jumping() const
{
	return jumping;
}

bool Player::is_shocked() const
{
	return !shock_countdown.is_done();
}

bool Player::is_invincible() const
{
	return !invincibility_countdown.is_done();
}

bool Player::is_on_hook() const
{
	return on_hook;
}

bool Player::is_teleporting_out() const
{
	return !teleport_out_countdown.is_done();
}

bool Player::is_teleporting_in() const
{
	return !teleport_in_countdown.is_done();
}

bool Player::is_teleporting() const
{
	return is_teleporting_in() || is_teleporting_out();
}

bool Player::ready_for_victory()
{
	return victorious && !is_teleporting_out();
}

bool Player::ready_for_game_over() const
{
	return !is_alive() && game_over_countdown.is_done();
}

std::string Player::retrieve_visible_text()
{
	std::string temp = visible_text;
	visible_text.clear();
	return temp;
}

void Player::set_stats()
{
	set_max_health(default_health);
	set_health(default_health);
	set_damage(default_damage);
}

void Player::set_sprite_texture(sf::Texture& arg)
{
	sprite.setTexture(arg);
}

void Player::set_sprite_origin(sf::Vector2f arg)
{
	sprite.setOrigin(arg);
}

void Player::set_position(sf::Vector2f arg)
{
	sprite.setPosition(arg);
}

void Player::set_visible_text(const std::string& arg)
{
	visible_text = arg;
}

void Player::set_shooting(bool arg)
{
	if (arg && !shooting)
		shoot();
	shooting = arg;
}

void Player::set_on_ice(bool arg)
{
	on_ice = arg;
}

void Player::set_on_hook(bool arg)
{
	on_hook = arg;
}

void Player::set_in_cold_air(bool arg)
{
	in_cold_air = arg;
}

void Player::set_in_hot_air(bool arg)
{
	in_hot_air = arg;
}

void Player::set_in_water(bool arg)
{
	in_water = arg;
}

void Player::set_in_enchanted_air(bool arg)
{
	in_enchanted_air = arg;
}

void Player::set_forcefully_moved_left(bool arg)
{
	forcefully_moved_left = arg;
	if (arg)
		forcefully_moved_right = false;
}

void Player::set_forcefully_moved_right(bool arg)
{
	forcefully_moved_right = arg;
	if (arg)
		forcefully_moved_left = false;
}

void Player::set_victorious(bool arg)
{
	victorious = arg;
}

void Player::move(float x, float y)
{
	sprite.move(x, y);
}

void Player::move(sf::Vector2f vector)
{
	sprite.move(vector);
}

void Player::add_points(int arg)
{
	points += arg;
}

void Player::add_coins(int arg)
{
	coins += arg;
}

void Player::jump(float gravity, float gravity_cooldown)
{
	set_gravity(gravity);
	set_gravity_cooldown(gravity_cooldown);
	standing = false;
	on_hook = false;
}

void Player::initialize_teleport(sf::Vector2f destination)
{
	if (is_shocked() || is_teleporting())
		return;

	teleport_destination = destination;
	teleport_destination.x += 16.f;
	teleport_destination.y += 16.f;
	teleport_out_countdown.restart();
	invincibility_countdown.stop();
	set_gravity(0.f);
	set_gravity_cooldown(0.f);
	set_velocity(0.f);
}

void Player::reset_controls()
{
	jumping = false;
	set_going_left(false);
	set_going_right(false);
	set_shooting(false);
}

void Player::handle_controls(const sf::Event& event, const Controls_Manager& cm)
{
	bool press;

	switch (event.type)
	{
	case (sf::Event::KeyPressed): press = true; break;
	case (sf::Event::KeyReleased): press = false; break;
	default: return;
	}

	if (event.key.code == cm.get_key(Controls_Manager::ACTION::JUMP)) jumping = press;
	if (event.key.code == cm.get_key(Controls_Manager::ACTION::LEFT)) set_going_left(press);
	if (event.key.code == cm.get_key(Controls_Manager::ACTION::RIGHT)) set_going_right(press);
	if (event.key.code == cm.get_key(Controls_Manager::ACTION::SHOOT)) set_shooting(press);
}

void Player::update(const sf::Time& dt,
	const hct::dynarray<std::unique_ptr<Solid_Tile>>& solid_tiles,
	const hct::dynarray<std::unique_ptr<Destructible_Tile>>& destructible_tiles)
{
	update_frame(dt);
	update_subentities(resources, dt, solid_tiles, destructible_tiles);

	standing = false;
	walking = is_going();

	game_over_countdown.update(dt);

	if (is_alive())
	{
		update_teleporting(dt);

		if (victorious)
			return;

		update_invincibility(dt);
		update_temperature(dt);
		update_shooting(dt);

		if (!(is_on_hook() && !is_jumping()) && !is_shocked() && !is_teleporting())
		{
			update_hooks(dt);
			update_enchanted_air(dt);
			update_movement(dt, solid_tiles, destructible_tiles);
		}

		on_ice = false;
		in_water = false;
	}
}

void Player::draw(sf::RenderTexture& rt) const
{
	if (visible_frame)
		rt.draw(sprite);
	draw_subentities(rt);
}