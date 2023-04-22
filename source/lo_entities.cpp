#include "lo_entities.h"

#include "hct/rng.h"

#include "animations.h"
#include "level_object_id.h"

// ----------------------------------------------------------------
// Walking_Entity
// ----------------------------------------------------------------

void Walking_Entity::check_ground(sf::Vector2f point, bool go_left,
	const hct::dynarray<std::unique_ptr<Solid_Tile>>& solid_tiles)
{
	const int max_blocks_distance = WALKING_ENTITY_RENDER_DISTANCE / 32;

	for (int block = 0; block < max_blocks_distance; ++block, point.x += (go_left ? -32.f : 32.f))
	{
		auto [solid_left, solid_right] = get_range_of_closest_objects(point, solid_tiles, 32.f);

		bool found_solid = std::none_of(solid_left, solid_right, [&point](const auto& tile)
			{ return tile->get_sprite().getGlobalBounds().contains(point); });

		if (auto temp = point.x + (go_left ? 16.f : -16.f); found_solid &&
			(go_left ? (temp > limit_left) : (temp < limit_right)))
		{
			if (go_left)
				limit_left = temp;
			else
				limit_right = temp;
			break;
		}
	}
}

void Walking_Entity::check_blockage(sf::Vector2f point, bool go_left,
	const hct::dynarray<std::unique_ptr<Solid_Tile>>& solid_tiles,
	const hct::dynarray<std::unique_ptr<Destructible_Tile>>& destructible_tiles)
{
	const int max_blocks_distance = WALKING_ENTITY_RENDER_DISTANCE / 32;

	for (int block = 0; block < max_blocks_distance; ++block, point.x += (go_left ? -32.f : 32.f))
	{
		auto [solid_left, solid_right] = get_range_of_closest_objects(point, solid_tiles, 32.f);
		auto [destructible_left, destructible_right] = get_range_of_closest_objects(point, destructible_tiles, 32.f);

		bool found_solid = std::any_of(solid_left, solid_right, [&point](const auto& tile)
			{ return tile->get_sprite().getGlobalBounds().contains(point); });
		bool found_destructible = !found_solid && std::any_of(destructible_left, destructible_right, [&point](const auto& tile)
			{ return tile->get_sprite().getGlobalBounds().contains(point); });

		if (auto temp = point.x + (go_left ? 16.f : -16.f); (found_solid || found_destructible) &&
			(go_left ? (temp > limit_left) : (temp < limit_right)))
		{
			if (go_left)
				limit_left = temp;
			else
				limit_right = temp;
			break;
		}
	}
}

void Walking_Entity::update_walking(const sf::Time& dt, float speed)
{
	move(dt.asSeconds() * (walking_left ? -speed : speed), 0.f);

	auto pos = get_sprite().getPosition().x;
	if (walking_left)
	{
		if (pos <= limit_left)
		{
			auto diff = limit_left - pos;
			move(diff * 2.f, 0.f);
			walking_left = false;
			get_sprite_animation().horizontally_flipped = false;
		}
	}
	else
	{
		if (pos >= limit_right)
		{
			auto diff = limit_right - pos;
			move(diff * 2.f, 0.f);
			walking_left = true;
			get_sprite_animation().horizontally_flipped = true;
		}
	}
}

void Walking_Entity::find_walking_limits(const hct::dynarray<std::unique_ptr<Solid_Tile>>& solid_tiles,
	const hct::dynarray<std::unique_ptr<Destructible_Tile>>& destructible_tiles)
{
	center_sprite();

	const int max_blocks_distance = WALKING_ENTITY_RENDER_DISTANCE / 32;

	sf::Vector2f point = sf::Vector2f(static_cast<float>(get_original_position().x), static_cast<float>(get_original_position().y));
	point.x = point.x - static_cast<float>(static_cast<int>(point.x) % 32);
	point.x += 16.f;
	point.y += get_sprite().getGlobalBounds().height + 16.f;

	check_ground(point, true, solid_tiles);
	check_ground(point, false, solid_tiles);

	for (int a = 0, max_height_blocks = (static_cast<int>(get_sprite().getGlobalBounds().height - 1.f) / 32) + 1; a < max_height_blocks; ++a)
	{
		point.y -= 32.f;
		check_blockage(point, true, solid_tiles, destructible_tiles);
		check_blockage(point, false, solid_tiles, destructible_tiles);
	}

	if (auto temp = point.x - (max_blocks_distance * 32); limit_left < temp)
		limit_left = temp;
	if (auto temp = point.x + (max_blocks_distance * 32); limit_right > temp)
		limit_right = temp;

	if (limit_right < limit_left)
		throw std::runtime_error("Walking_Entity::find_walking_limits: left limit was greater than the right limit");
}

// ----------------------------------------------------------------
// Enemy_Component
// ----------------------------------------------------------------

Enemy_Component::Enemy_Component(unsigned int health, unsigned int damage, int points) :
	Health_Component(health), Damage_Component(damage), Value_Component(points) {}

void Enemy_Component::extra_player_interaction(Resources& resources, Player& player, const sf::FloatRect& my_hitbox) {}

void Enemy_Component::react_to_damage()
{
	health_text_display_countdown.restart();
}

void Enemy_Component::react_to_heal()
{
	react_to_damage();
}

void Enemy_Component::player_interaction(Resources& resources, Player& player, const sf::FloatRect& my_hitbox, bool can_deal_contact_damage, bool can_take_damage)
{
	if (can_deal_contact_damage && collision(my_hitbox, player.get_hitbox()))
		deal_damage(player);

	if (can_take_damage)
		for (const auto& bullet : player.get_subentities())
			if (bullet->is_active() && collision(my_hitbox, bullet->get_hitbox()))
			{
				bullet->set_hit(resources);

				if (player.deal_damage(*this))
				{
					set_death_graphics(resources);

					if (!is_harvested())
					{
						set_harvested(true);
						player.add_points(get_value());
					}
				}
			}

	extra_player_interaction(resources, player, my_hitbox);
}

void Enemy_Component::setup_health_info(Resources& resources)
{
	health_text = make_label(resources.fonts.get(RESOURCE_ID::FONTS::MAIN), {}, {}, 24);
	health_text.setOutlineThickness(health_text.getOutlineThickness() * 2.f);
	health_text.setScale(0.5f, 0.5f);
}

void Enemy_Component::update_health_info(const sf::Sprite& sprite, const sf::Time& dt)
{
	health_text_display_countdown.update(dt);

	if (is_alive())
	{
		health_text.setString(make_string(get_health(), " / ", get_max_health()));
		set_center(health_text, false);
		auto pos_x = sprite.getGlobalBounds().left + sprite.getGlobalBounds().width * 0.5f;
		auto pos_y = sprite.getGlobalBounds().top - 12.f;
		health_text.setPosition(pos_x, pos_y);
	}
}

void Enemy_Component::draw_health_info(sf::RenderTexture& rt) const
{
	if (is_alive() && !health_text_display_countdown.is_done())
		rt.draw(health_text);
}

// ----------------------------------------------------------------
// Trampoline
// ----------------------------------------------------------------

void Trampoline::extra_setup(std::istream& stream, Resources& resources)
{
	set_sprite_texture(resources.textures.get(RESOURCE_ID::TEXTURES::TRAMPOLINE));

	int32_t temp_strength;
	data_reader(stream, temp_strength);
	strength = static_cast<STRENGTH>(temp_strength);

	auto animation = get_animation(AID_ENTITY::TRAMPOLINE);

	switch (strength)
	{
	case(STRENGTH::WEAK): animation.row = 0; break;
	case(STRENGTH::STRONG): animation.row = 1; break;
	default: throw std::runtime_error("Trampoline::extra_setup: read strength out of range");
	}

	set_sprite_animation(animation);
}

void Trampoline::extra_pre_record(std::ostream& stream) const
{
	data_writer(stream, static_cast<int32_t>(LO_TYPE::STATIONARY_ENTITY));
	data_writer(stream, static_cast<int32_t>(STATIONARY_ENTITY_SUBTYPE::TRAMPOLINE));
}

void Trampoline::extra_post_record(std::ostream& stream) const
{
	data_writer(stream, static_cast<int32_t>(strength));
}

sf::FloatRect Trampoline::get_hitbox() const
{
	return ::get_hitbox(get_sprite(), 0.5f, 0.2f);
}

void Trampoline::update(Resources& resources, Player& player, const sf::Time& dt)
{
	update_frame(dt);

	if (collision(get_hitbox(), player.get_hitbox()))
	{
		switch (strength)
		{
		case(STRENGTH::WEAK): player.jump(Player::default_jump_gravity * 2.4f, 0.f); break;
		case(STRENGTH::STRONG): player.jump(Player::default_jump_gravity * 3.f, 0.f); break;
		}

		restart_sprite_animation();
	}
}

Stationary_Entity* Trampoline::make_lo(LEVEL_THEME theme_id)
{
	return new Trampoline;
}

// ----------------------------------------------------------------
// Thorn
// ---------------------------------------------------------------- 

Thorn::Thorn(LEVEL_THEME theme_id)
{
	auto animation = get_animation(AID_ENTITY::THORN);

	switch (theme_id)
	{
	case (LEVEL_THEME::VERDANT): animation.row = 0; break;
	case (LEVEL_THEME::ELECTRIC): animation.row = 1; break;
	case (LEVEL_THEME::LIFEFORCE): animation.row = 2; break;
	case (LEVEL_THEME::ICE): animation.row = 3; break;
	default: throw std::runtime_error("Thorn::Thorn: invalid theme_id provided");
	}

	set_sprite_animation(animation);
}

void Thorn::extra_setup(std::istream& stream, Resources& resources)
{
	set_sprite_texture(resources.textures.get(RESOURCE_ID::TEXTURES::THORN));

	data_reader(stream, shape);
	if (shape < 0 || shape >= possible_shapes_count)
		throw std::runtime_error("Thorn::extra_setup: read shape out of range");

	auto animation = get_sprite().get_animation_settings();
	animation.start_frame = shape;
	set_sprite_animation(animation);
}

void Thorn::extra_pre_record(std::ostream& stream) const
{
	data_writer(stream, static_cast<int32_t>(LO_TYPE::STATIONARY_ENTITY));
	data_writer(stream, static_cast<int32_t>(STATIONARY_ENTITY_SUBTYPE::THORN));
}

void Thorn::extra_post_record(std::ostream& stream) const
{
	data_writer(stream, shape);
}

sf::FloatRect Thorn::get_hitbox()
{
	return ::get_hitbox(get_sprite(), 0.6f, 0.6f);
}

void Thorn::update(Resources& resources, Player& player, const sf::Time& dt)
{
	update_frame(dt);

	if (active)
	{
		if (collision(get_hitbox(), player.get_hitbox()))
			player.take_damage(damage_stat);

		for (auto& bullet : player.get_subentities())
			if (collision(get_hitbox(), bullet->get_hitbox()))
			{
				active = false;
				set_sprite_texture(resources.textures.get(RESOURCE_ID::TEXTURES::EXPLOSION_32));
				set_sprite_animation(get_animation(AID_MISC::EXPLOSION_32));
				bullet->set_hit(resources);
			}
	}
}

Stationary_Entity* Thorn::make_lo(LEVEL_THEME theme_id)
{
	return new Thorn(theme_id);
}

bool Thorn::available_for_level_theme(LEVEL_THEME theme_id)
{
	return 
		theme_id == LEVEL_THEME::VERDANT ||
		theme_id == LEVEL_THEME::ELECTRIC ||
		theme_id == LEVEL_THEME::LIFEFORCE ||
		theme_id == LEVEL_THEME::ICE;
}

// ----------------------------------------------------------------
// Thorn_Spear_Root
// ---------------------------------------------------------------- 

Thorn_Spear_Root::Thorn_Spear_Root(LEVEL_THEME theme_id)
{
	if (!available_for_level_theme(theme_id))
		throw std::runtime_error("Thorn_Spear_Root::Thorn_Spear_Root: invalid level theme");
}

void Thorn_Spear_Root::extra_setup(std::istream& stream, Resources& resources)
{
	set_sprite_texture(resources.textures.get(RESOURCE_ID::TEXTURES::THORN_SPEAR_ROOT));
	set_sprite_animation(get_animation(AID_ENTITY::THORN_SPEAR_ROOT));
	thorn_spear.setTexture(resources.textures.get(RESOURCE_ID::TEXTURES::THORN_SPEAR));
	thorn_spear.set_animation(get_animation(AID_MISC::THORN_SPEAR_IN));
	thorn_spear.setOrigin(0.f, thorn_spear.getGlobalBounds().height);
	thorn_spear.setPosition(static_cast<float>(get_original_position().x), static_cast<float>(get_original_position().y + 32.f));
}

void Thorn_Spear_Root::extra_pre_record(std::ostream& stream) const
{
	data_writer(stream, static_cast<int32_t>(LO_TYPE::STATIONARY_ENTITY));
	data_writer(stream, static_cast<int32_t>(STATIONARY_ENTITY_SUBTYPE::THORN_SPEAR));
}

sf::FloatRect Thorn_Spear_Root::get_hitbox()
{
	return active ? ::get_hitbox(thorn_spear, 0.3f, 0.9f) : ::get_hitbox(get_sprite(), 0.3f, 0.95f);
}

void Thorn_Spear_Root::update(Resources& resources, Player& player, const sf::Time& dt)
{
	update_frame(dt);
	thorn_spear.update_frame(dt);

	if (switch_timer.is_ready())
		thorn_spear.set_animation(active ? get_animation(AID_MISC::THORN_SPEAR_OUT) : get_animation(AID_MISC::THORN_SPEAR_IN));

	if (collision(get_hitbox(), player.get_hitbox()))
		player.take_damage(damage_stat);
}

void Thorn_Spear_Root::draw(sf::RenderTexture& rt) const
{
	rt.draw(thorn_spear);
	this->LO_Base::draw(rt);
}

void Thorn_Spear_Root::update_timer(const sf::Time& dt)
{
	switch_timer.update(dt);
	if (switch_timer.is_ready())
		active = !active;
}

bool Thorn_Spear_Root::available_for_level_theme(LEVEL_THEME theme_id)
{
	return theme_id == LEVEL_THEME::VERDANT;
}

Stationary_Entity* Thorn_Spear_Root::make_lo(LEVEL_THEME theme_id)
{
	return new Thorn_Spear_Root(theme_id);
}

// ----------------------------------------------------------------
// Mine
// ---------------------------------------------------------------- 

Mine::Mine(LEVEL_THEME theme_id)
{
	auto animation = get_animation(AID_ENTITY::MINE);

	if (!available_for_level_theme(theme_id))
		throw std::runtime_error("Mine::Mine: invalid level theme");

	switch (theme_id)
	{
	case (LEVEL_THEME::WATER): animation.row = 0; break;
	case (LEVEL_THEME::FIRE): animation.row = 1; break;
	case (LEVEL_THEME::GROUND): animation.row = 2; break;
	}

	set_sprite_animation(animation);
}

void Mine::extra_setup(std::istream& stream, Resources& resources)
{
	set_sprite_texture(resources.textures.get(RESOURCE_ID::TEXTURES::MINE));
}

void Mine::extra_pre_record(std::ostream& stream) const
{
	data_writer(stream, static_cast<int32_t>(LO_TYPE::STATIONARY_ENTITY));
	data_writer(stream, static_cast<int32_t>(STATIONARY_ENTITY_SUBTYPE::MINE));
}

sf::FloatRect Mine::get_hitbox() const
{
	return ::get_hitbox(get_sprite(), 0.6f, 0.6f);
}

void Mine::update(Resources& resources, Player& player, const sf::Time& dt)
{
	update_frame(dt);

	if (active && collision(get_hitbox(), player.get_hitbox()))
	{
		player.take_damage(damage_stat);
		active = false;
		set_sprite_texture(resources.textures.get(RESOURCE_ID::TEXTURES::EXPLOSION_48));
		set_sprite_animation(get_animation(AID_MISC::EXPLOSION_48));
		move(-8.f, -8.f);
	}
}

bool Mine::available_for_level_theme(LEVEL_THEME theme_id)
{
	return 
		theme_id == LEVEL_THEME::WATER ||
		theme_id == LEVEL_THEME::FIRE ||
		theme_id == LEVEL_THEME::GROUND;
}

Stationary_Entity* Mine::make_lo(LEVEL_THEME theme_id)
{
	return new Mine(theme_id);
}

// ----------------------------------------------------------------
// Healing Plant
// ---------------------------------------------------------------- 

Healing_Plant::Healing_Plant(LEVEL_THEME theme_id)
{
	auto animation = get_animation(AID_ENTITY::HEALING_PLANT);

	if (!available_for_level_theme(theme_id))
		throw std::runtime_error("Healing_Plant::Healing_Plant: invalid level theme");

	switch (theme_id)
	{
	case (LEVEL_THEME::VERDANT): animation.row = 0; break;
	case (LEVEL_THEME::WATER): animation.row = 1; break;
	case (LEVEL_THEME::LIFEFORCE): animation.row = 2; break;
	}

	set_sprite_animation(animation);
}

void Healing_Plant::extra_setup(std::istream& stream, Resources& resources)
{
	set_sprite_texture(resources.textures.get(RESOURCE_ID::TEXTURES::HEALING_PLANT));
}

void Healing_Plant::extra_pre_record(std::ostream& stream) const
{
	data_writer(stream, static_cast<int32_t>(LO_TYPE::STATIONARY_ENTITY));
	data_writer(stream, static_cast<int32_t>(STATIONARY_ENTITY_SUBTYPE::HEALING_PLANT));
}

sf::FloatRect Healing_Plant::get_hitbox() const
{
	return active ? ::get_hitbox(get_sprite(), 0.3f, 0.3f) : ::get_hitbox(get_sprite(), 1.2f, 1.2f);
}

void Healing_Plant::update(Resources& resources, Player& player, const sf::Time& dt)
{
	update_frame(dt);

	if (active)
	{
		auto popper = [&]()
		{
			active = false;
			center_sprite();
			set_sprite_texture(resources.textures.get(RESOURCE_ID::TEXTURES::HEALING_BLAST));
			auto animation = get_animation(AID_MISC::HEALING_BLAST);
			animation.row = get_sprite_animation().row;
			set_sprite_animation(animation);
		};

		if (collision(get_hitbox(), player.get_hitbox()))
			popper();

		for (auto& x : player.get_subentities())
			if (collision(get_hitbox(), x->get_hitbox()))
			{
				popper();
				x->set_hit(resources);
				break;
			}
	}

	if (!active && !get_sprite().finished() && collision(get_hitbox(), player.get_hitbox()))
		player.receive_heal(heal_stat);
}

bool Healing_Plant::available_for_level_theme(LEVEL_THEME theme_id)
{
	return 
		theme_id == LEVEL_THEME::VERDANT ||
		theme_id == LEVEL_THEME::WATER || 
		theme_id == LEVEL_THEME::LIFEFORCE;
}

Stationary_Entity* Healing_Plant::make_lo(LEVEL_THEME theme_id)
{
	return new Healing_Plant(theme_id);
}

// ----------------------------------------------------------------
// Wind_Blaster
// ---------------------------------------------------------------- 

Wind_Blaster::Wind_Blaster(LEVEL_THEME theme_id)
{
	if (!available_for_level_theme(theme_id))
		throw std::runtime_error("Wind_Blaster::Wind_Blaster: invalid level theme");
}

void Wind_Blaster::extra_setup(std::istream& stream, Resources& resources)
{
	set_sprite_texture(resources.textures.get(RESOURCE_ID::TEXTURES::WIND_BLASTER));
	set_sprite_animation(get_animation(AID_ENTITY::WIND_BLASTER));
}

void Wind_Blaster::extra_pre_record(std::ostream& stream) const
{
	data_writer(stream, static_cast<int32_t>(LO_TYPE::STATIONARY_ENTITY));
	data_writer(stream, static_cast<int32_t>(STATIONARY_ENTITY_SUBTYPE::WIND_BLASTER));
}

sf::FloatRect Wind_Blaster::get_hitbox() const
{
	return ::get_hitbox(get_sprite(), 0.8f, 0.8f);
}

void Wind_Blaster::update(Resources& resources, Player& player, const sf::Time& dt)
{
	update_frame(dt);

	if (switch_timer.is_ready())
		get_sprite_animation().row = active ? 0 : 1;

	if (active && collision(get_hitbox(), player.get_hitbox()))
		player.jump(Player::default_jump_gravity * 2.25f, 0.f);
}

bool Wind_Blaster::available_for_level_theme(LEVEL_THEME theme_id)
{
	return theme_id == LEVEL_THEME::WIND;
}

void Wind_Blaster::update_timer(const sf::Time& dt)
{
	switch_timer.update(dt);
	if (switch_timer.is_ready())
		active = !active;
}

Stationary_Entity* Wind_Blaster::make_lo(LEVEL_THEME theme_id)
{
	return new Wind_Blaster(theme_id);
}

// ----------------------------------------------------------------
// Living_Spikes
// ---------------------------------------------------------------- 

Living_Spikes::Living_Spikes(LEVEL_THEME theme_id)
{
	if (!available_for_level_theme(theme_id))
		throw std::runtime_error("Living_Spikes::Living_Spikes: invalid level theme");
}

void Living_Spikes::extra_setup(std::istream& stream, Resources& resources)
{
	set_sprite_texture(resources.textures.get(RESOURCE_ID::TEXTURES::LIVING_SPIKES));
	set_sprite_animation(get_animation(AID_ENTITY::LIVING_SPIKES));
	set_next_animation();
}

void Living_Spikes::extra_pre_record(std::ostream& stream) const
{
	data_writer(stream, static_cast<int32_t>(LO_TYPE::STATIONARY_ENTITY));
	data_writer(stream, static_cast<int32_t>(STATIONARY_ENTITY_SUBTYPE::LIVING_SPIKES));
}

void Living_Spikes::set_next_animation()
{
	auto animation = get_sprite_animation();
	animation.row = (active ? 0 : 1);
	set_sprite_animation(animation);
}

sf::FloatRect Living_Spikes::get_hitbox() const
{
	return ::get_hitbox(get_sprite(), 0.8f, 0.3f);
}

void Living_Spikes::update(Resources& resources, Player& player, const sf::Time& dt)
{
	update_frame(dt);

	if (switch_timer.is_ready())
		set_next_animation();

	if (active && collision(get_hitbox(), player.get_hitbox()))
		player.take_damage(damage_stat);
}

bool Living_Spikes::available_for_level_theme(LEVEL_THEME theme_id)
{
	return theme_id == LEVEL_THEME::LIFEFORCE;
}

void Living_Spikes::update_timer(const sf::Time& dt)
{
	switch_timer.update(dt);
	if (switch_timer.is_ready())
		active = !active;
}

Stationary_Entity* Living_Spikes::make_lo(LEVEL_THEME theme_id)
{
	return new Living_Spikes(theme_id);
}

// ----------------------------------------------------------------
// Cannon
// ---------------------------------------------------------------- 

Cannon::Cannon(LEVEL_THEME theme_id) :
	Damage_Component(damage_stat), Bullet_Manager_Component(theme_to_bullet_color(theme_id), false)
{
	auto animation = get_animation(AID_ENTITY::CANNON);
	animation.row = static_cast<int>(theme_id) - 1;
	if (animation.row < 0 || theme_id >= LEVEL_THEME::COUNT)
		throw std::runtime_error("Cannon::Cannon: invalid theme_id provided");
	set_sprite_animation(animation);
}

void Cannon::extra_setup(std::istream& stream, Resources& resources)
{
	set_sprite_texture(resources.textures.get(RESOURCE_ID::TEXTURES::CANNON));
	center_sprite();

	int32_t temp_direction;
	data_reader(stream, temp_direction);
	direction = static_cast<Bullet::DIRECTION>(temp_direction);

	switch (direction)
	{
	case(Bullet::DIRECTION::LEFT): set_sprite_rotation(180.f); break;
	case(Bullet::DIRECTION::RIGHT): break;
	case(Bullet::DIRECTION::DOWN): set_sprite_rotation(90.f); break;
	default: throw std::runtime_error("Cannon::extra_setup: read direction out of range");
	}
}

void Cannon::extra_pre_record(std::ostream& stream) const
{
	data_writer(stream, static_cast<int32_t>(LO_TYPE::COLLIDING_ENTITY));
	data_writer(stream, static_cast<int32_t>(COLLIDING_ENTITY_SUBTYPE::CANNON));
}

void Cannon::extra_post_record(std::ostream& stream) const
{
	data_writer(stream, static_cast<int32_t>(direction));
}

void Cannon::update(Resources& resources, Player& player, const sf::Time& dt,
	const hct::dynarray<std::unique_ptr<Solid_Tile>>& solid_tiles,
	const hct::dynarray<std::unique_ptr<Destructible_Tile>>& destructible_tiles)
{
	update_frame(dt);
	update_subentities(resources, dt, solid_tiles, destructible_tiles);

	if (shoot_timer.is_ready())
	{
		sf::Vector2f position = sf::Vector2f(static_cast<float>(get_original_position().x), static_cast<float>(get_original_position().y));
		position.x += 16.f;
		position.y += 16.f;
		switch (direction)
		{
		case(Bullet::DIRECTION::LEFT): position.x -= 16.f; break;
		case(Bullet::DIRECTION::RIGHT): position.x += 16.f; break;
		case(Bullet::DIRECTION::DOWN): position.y += 16.f; break;
		}
		spawn_subentity(resources, position, direction);

		restart_sprite_animation();
	}

	for (const auto& bullet : get_subentities())
		if (bullet->is_active() && player.is_alive() && collision(bullet->get_hitbox(), player.get_hitbox()))
		{
			deal_damage(player);
			bullet->set_hit(resources);
		}
}

void Cannon::draw(sf::RenderTexture& rt) const
{
	draw_subentities(rt);
	this->LO_Base::draw(rt);
}

void Cannon::update_timer(const sf::Time& dt)
{
	shoot_timer.update(dt);
}

Colliding_Entity* Cannon::make_lo(LEVEL_THEME theme_id)
{
	return new Cannon(theme_id);
}

// ----------------------------------------------------------------
// Bouncer
// ---------------------------------------------------------------- 

Bouncer::Bouncer(LEVEL_THEME theme_id) :
	Damage_Component(damage_stat), Vertical_Physics_Component(*this)
{
	if (!available_for_level_theme(theme_id))
		throw std::runtime_error("Bouncer::Bouncer: invalid level theme");

	auto animation = get_animation(AID_ENTITY::BOUNCER);
	switch (theme_id)
	{
	case (LEVEL_THEME::VERDANT): animation.row = 0; break;
	case (LEVEL_THEME::WIND): animation.row = 1; break;
	}
	set_sprite_animation(animation);
}

void Bouncer::extra_setup(std::istream& stream, Resources& resources)
{
	set_sprite_texture(resources.textures.get(RESOURCE_ID::TEXTURES::BOUNCER));
}

void Bouncer::extra_pre_record(std::ostream& stream) const
{
	data_writer(stream, static_cast<int32_t>(LO_TYPE::COLLIDING_ENTITY));
	data_writer(stream, static_cast<int32_t>(COLLIDING_ENTITY_SUBTYPE::BOUNCER));
}

void Bouncer::jump()
{
	set_gravity(-Vertical_Physics_Component::max_gravity);
	set_gravity_cooldown(400.f);
}

sf::FloatRect Bouncer::get_hitbox()
{
	return ::get_hitbox(get_sprite(), 0.8f, 0.8f);
}

void Bouncer::update(Resources& resources, Player& player, const sf::Time& dt,
	const hct::dynarray<std::unique_ptr<Solid_Tile>>& solid_tiles,
	const hct::dynarray<std::unique_ptr<Destructible_Tile>>& destructible_tiles)
{
	update_frame(dt);

	move(0, update_vertically(dt));

	auto [solid_left, solid_right] = get_range_of_closest_objects(get_sprite(), solid_tiles);
	auto [destructible_left, destructible_right] = get_range_of_closest_objects(get_sprite(), destructible_tiles);

	if (check_vertical_collisions(solid_left, solid_right, get_hitbox()) == Vertical_Physics_Component::COLLISION::BOTTOM ||
		check_vertical_collisions(destructible_left, destructible_right, get_hitbox()) == Vertical_Physics_Component::COLLISION::BOTTOM)
		jump();

	if (collision(get_hitbox(), player.get_hitbox()))
		deal_damage(player);
}

bool Bouncer::available_for_level_theme(LEVEL_THEME theme_id)
{
	return 
		theme_id == LEVEL_THEME::VERDANT ||
		theme_id == LEVEL_THEME::WIND;
}

Colliding_Entity* Bouncer::make_lo(LEVEL_THEME theme_id)
{
	return new Bouncer(theme_id);
}

// ----------------------------------------------------------------
// Falling_Hazard
// ---------------------------------------------------------------- 

Falling_Hazard::Falling_Hazard(LEVEL_THEME theme_id) :
	Damage_Component(damage_stat), Vertical_Physics_Component(*this)
{
	if (!available_for_level_theme(theme_id))
		throw std::runtime_error("Falling_Hazard::Falling_Hazard: invalid level theme");

	auto animation = get_animation(AID_ENTITY::FALLING_HAZARD);
	switch (theme_id)
	{
	case (LEVEL_THEME::VERDANT): animation.row = 0; break;
	case (LEVEL_THEME::FIRE): animation.row = 1; break;
	case (LEVEL_THEME::GROUND): animation.row = 2; break;
	case (LEVEL_THEME::ICE): animation.row = 3; break;
	}
	set_sprite_animation(animation);
}

void Falling_Hazard::extra_setup(std::istream& stream, Resources& resources)
{
	set_sprite_texture(resources.textures.get(RESOURCE_ID::TEXTURES::FALLING_HAZARD));
}

void Falling_Hazard::extra_pre_record(std::ostream& stream) const
{
	data_writer(stream, static_cast<int32_t>(LO_TYPE::COLLIDING_ENTITY));
	data_writer(stream, static_cast<int32_t>(COLLIDING_ENTITY_SUBTYPE::FALLING_HAZARD));
}

void Falling_Hazard::set_hit(Resources& resources)
{
	if (!active)
		return;

	active = false;
	set_sprite_texture(resources.textures.get(RESOURCE_ID::TEXTURES::EXPLOSION_32));
	set_sprite_animation(get_animation(AID_MISC::EXPLOSION_32));
}

sf::FloatRect Falling_Hazard::get_hitbox()
{
	return ::get_hitbox(get_sprite(), 0.8f, 0.8f);
}

void Falling_Hazard::update(Resources& resources, Player& player, const sf::Time& dt,
	const hct::dynarray<std::unique_ptr<Solid_Tile>>& solid_tiles,
	const hct::dynarray<std::unique_ptr<Destructible_Tile>>& destructible_tiles)
{
	update_frame(dt);

	if (awaiting)
	{
		if (!limit.has_value())
		{
			auto [solid_left, solid_right] = get_range_of_closest_objects(get_sprite(), solid_tiles);
			auto [destructible_left, destructible_right] = get_range_of_closest_objects(get_sprite(), destructible_tiles);

			set_gravity(-1.f);
			while (check_vertical_collisions(solid_left, solid_right, get_hitbox()) == Vertical_Physics_Component::COLLISION::NONE &&
				check_vertical_collisions(destructible_left, destructible_right, get_hitbox()) == Vertical_Physics_Component::COLLISION::NONE &&
				get_sprite().getPosition().y < Camera::down_lim)
				move(0.f, 16.f);
			set_gravity(0.f);
			limit.emplace(get_sprite().getPosition().y - 16.f);
			set_position(static_cast<float>(get_original_position().x), static_cast<float>(get_original_position().y));
		}
		else
		{
			if (std::abs(player.get_position().x - get_sprite().getPosition().x) < reaction_range &&
				player.get_position().y > get_sprite().getPosition().y &&
				player.get_position().y < limit)
				awaiting = false;
		}
	}
	else if (active)
	{
		move(0, update_vertically(dt));
		if (check_vertical_collisions(solid_tiles.begin(), solid_tiles.end(), get_hitbox()) == Vertical_Physics_Component::COLLISION::BOTTOM)
			set_hit(resources);
	}

	if (active && collision(get_hitbox(), player.get_hitbox()))
	{
		deal_damage(player);
		set_hit(resources);
	}
}

bool Falling_Hazard::available_for_level_theme(LEVEL_THEME theme_id)
{
	return 
		theme_id == LEVEL_THEME::VERDANT ||
		theme_id == LEVEL_THEME::FIRE ||
		theme_id == LEVEL_THEME::GROUND ||
		theme_id == LEVEL_THEME::ICE;
}

Colliding_Entity* Falling_Hazard::make_lo(LEVEL_THEME theme_id)
{
	return new Falling_Hazard(theme_id);
}

// ----------------------------------------------------------------
// Rock_Disposer
// ---------------------------------------------------------------- 

Rock_Disposer::Rock_Disposer(LEVEL_THEME theme_id) :
	Damage_Component(damage_stat), Rock_Manager_Component(theme_id)
{
	if (!available_for_level_theme(theme_id))
		throw std::runtime_error("Rock_Disposer::Rock_Disposer: invalid level theme");

	auto animation = get_animation(AID_ENTITY::ROCK_DISPOSER);
	animation.row = static_cast<int>(theme_id) - 1;
	switch (theme_id)
	{
	case (LEVEL_THEME::FIRE): animation.row = 0; break;
	case (LEVEL_THEME::GROUND): animation.row = 1; break;
	case (LEVEL_THEME::ICE): animation.row = 2; break;
	}
	set_sprite_animation(animation);
}

void Rock_Disposer::extra_setup(std::istream& stream, Resources& resources)
{
	set_sprite_texture(resources.textures.get(RESOURCE_ID::TEXTURES::ROCK_DISPOSER));
	center_sprite();
}

void Rock_Disposer::extra_pre_record(std::ostream& stream) const
{
	data_writer(stream, static_cast<int32_t>(LO_TYPE::COLLIDING_ENTITY));
	data_writer(stream, static_cast<int32_t>(COLLIDING_ENTITY_SUBTYPE::ROCK_DISPOSER));
}

void Rock_Disposer::update(Resources& resources, Player& player, const sf::Time& dt,
	const hct::dynarray<std::unique_ptr<Solid_Tile>>& solid_tiles,
	const hct::dynarray<std::unique_ptr<Destructible_Tile>>& destructible_tiles)
{
	update_frame(dt);
	update_subentities(resources, dt, solid_tiles, destructible_tiles);

	if (active && dispose_timer.is_ready())
	{
		sf::Vector2f position = get_sprite().getPosition();
		position.x += (hct::RNG::instance()(-20, 20));
		position.y += 40.f;
		spawn_subentity(resources, position);
	}

	for (const auto& rock : get_subentities())
		if (rock->is_active() && player.is_alive() && collision(rock->get_hitbox(), player.get_hitbox()))
		{
			deal_damage(player);
			rock->set_hit(resources);
		}
}

void Rock_Disposer::draw(sf::RenderTexture& rt) const
{
	draw_subentities(rt);
	this->LO_Base::draw(rt);
}

bool Rock_Disposer::available_for_level_theme(LEVEL_THEME theme_id)
{
	return 
		theme_id == LEVEL_THEME::FIRE ||
		theme_id == LEVEL_THEME::GROUND ||
		theme_id == LEVEL_THEME::ICE;
}

void Rock_Disposer::update_timer(const sf::Time& dt)
{
	switch_timer.update(dt);
	if (switch_timer.is_ready())
	{
		active = !active;
		dispose_timer.restart();
	}
	if (active)
		dispose_timer.update(dt);
}

Colliding_Entity* Rock_Disposer::make_lo(LEVEL_THEME theme_id)
{
	return new Rock_Disposer(theme_id);
}

// ----------------------------------------------------------------
// Ice_Launcher
// ---------------------------------------------------------------- 

Ice_Launcher::Ice_Launcher(LEVEL_THEME theme_id) :
	Damage_Component(damage_stat)
{
	if (!available_for_level_theme(theme_id))
		throw std::runtime_error("Ice_Launcher::Ice_Launcher: invalid level theme");
}

void Ice_Launcher::extra_setup(std::istream& stream, Resources& resources)
{
	set_sprite_texture(resources.textures.get(RESOURCE_ID::TEXTURES::ICE_LAUNCHER));
	set_sprite_animation(get_animation(AID_ENTITY::ICE_LAUNCHER));
	center_sprite();

	int32_t temp_direction;
	data_reader(stream, temp_direction);
	direction = static_cast<Ice_Chunk::DIRECTION>(temp_direction);

	switch (direction)
	{
	case(Ice_Chunk::DIRECTION::LEFT): set_sprite_rotation(180.f); break;
	case(Ice_Chunk::DIRECTION::RIGHT): break;
	default: throw std::runtime_error("Ice_Launcher::_extra_setup: read direction out of range");
	}
}

void Ice_Launcher::extra_pre_record(std::ostream& stream) const
{
	data_writer(stream, static_cast<int32_t>(LO_TYPE::COLLIDING_ENTITY));
	data_writer(stream, static_cast<int32_t>(COLLIDING_ENTITY_SUBTYPE::ICE_LAUNCHER));
}

void Ice_Launcher::extra_post_record(std::ostream& stream) const
{
	data_writer(stream, static_cast<int32_t>(direction));
}

void Ice_Launcher::update(Resources& resources, Player& player, const sf::Time& dt,
	const hct::dynarray<std::unique_ptr<Solid_Tile>>& solid_tiles,
	const hct::dynarray<std::unique_ptr<Destructible_Tile>>& destructible_tiles)
{
	update_frame(dt);
	update_subentities(resources, dt, solid_tiles, destructible_tiles);

	if (launch_timer.is_ready())
	{
		sf::Vector2f position = get_sprite().getPosition();
		switch (direction)
		{
		case(Ice_Chunk::DIRECTION::LEFT): position.x -= 32.f; break;
		case(Ice_Chunk::DIRECTION::RIGHT): position.x += 32.f; break;
		}
		spawn_subentity(resources, position, direction);

		restart_sprite_animation();
	}

	for (const auto& ice_chunk : get_subentities())
		if (ice_chunk->is_active() && player.is_alive() && collision(ice_chunk->get_hitbox(), player.get_hitbox()))
		{
			deal_damage(player);
			ice_chunk->set_hit(resources);
		}
}

void Ice_Launcher::draw(sf::RenderTexture& rt) const
{
	draw_subentities(rt);
	this->LO_Base::draw(rt);
}

bool Ice_Launcher::available_for_level_theme(LEVEL_THEME theme_id)
{
	return theme_id == LEVEL_THEME::ICE;
}

void Ice_Launcher::update_timer(const sf::Time& dt)
{
	launch_timer.update(dt);
}

Colliding_Entity* Ice_Launcher::make_lo(LEVEL_THEME theme_id)
{
	return new Ice_Launcher(theme_id);
}

// ----------------------------------------------------------------
// Pressure_Orb
// ---------------------------------------------------------------- 

Pressure_Orb::Pressure_Orb(LEVEL_THEME theme_id) :
	Damage_Component(damage_stat), Bullet_Manager_Component(theme_to_bullet_color(theme_id), false)
{
	if (!available_for_level_theme(theme_id))
		throw std::runtime_error("Pressure_Orb::Pressure_Orb: invalid level theme");

	auto animation = get_animation(AID_ENTITY::PRESSURE_ORB);
	switch (theme_id)
	{
	case (LEVEL_THEME::WATER): animation.row = 0; break;
	case (LEVEL_THEME::WIND): animation.row = 1; break;
	case (LEVEL_THEME::LIFEFORCE): animation.row = 2; break;
	}
	set_sprite_animation(animation);
}

void Pressure_Orb::extra_setup(std::istream& stream, Resources& resources)
{
	set_sprite_texture(resources.textures.get(RESOURCE_ID::TEXTURES::PRESSURE_ORB));
	center_sprite();
}

void Pressure_Orb::extra_pre_record(std::ostream& stream) const
{
	data_writer(stream, static_cast<int32_t>(LO_TYPE::COLLIDING_ENTITY));
	data_writer(stream, static_cast<int32_t>(COLLIDING_ENTITY_SUBTYPE::PRESSURE_ORB));
}

sf::FloatRect Pressure_Orb::get_hitbox()
{
	return ::get_hitbox(get_sprite(), 0.8f, 0.8f);
}

void Pressure_Orb::update(Resources& resources, Player& player, const sf::Time& dt,
	const hct::dynarray<std::unique_ptr<Solid_Tile>>& solid_tiles,
	const hct::dynarray<std::unique_ptr<Destructible_Tile>>& destructible_tiles)
{
	update_frame(dt);
	update_subentities(resources, dt, solid_tiles, destructible_tiles);

	if (active)
	{
		bool hit = false;

		if (collision(get_hitbox(), player.get_hitbox()))
			hit = true;
		for (auto& x : player.get_subentities())
			if (active && collision(get_hitbox(), x->get_hitbox()))
			{
				hit = true;
				x->set_hit(resources);
				break;
			}

		if (hit)
		{
			active = false;

			auto animation = get_animation(AID_MISC::PRESSURE_BLAST);
			animation.row = get_sprite().get_animation_settings().row;
			set_sprite_texture(resources.textures.get(RESOURCE_ID::TEXTURES::PRESSURE_BLAST));
			set_sprite_animation(animation);

			spawn_subentity(resources, get_sprite().getPosition(), Bullet::DIRECTION::LEFT);
			spawn_subentity(resources, get_sprite().getPosition(), Bullet::DIRECTION::RIGHT);
			spawn_subentity(resources, get_sprite().getPosition(), Bullet::DIRECTION::UP);
			spawn_subentity(resources, get_sprite().getPosition(), Bullet::DIRECTION::DOWN);
		}
	}

	for (const auto& bullet : get_subentities())
		if (bullet->is_active() && player.is_alive() && collision(bullet->get_hitbox(), player.get_hitbox()))
		{
			deal_damage(player);
			bullet->set_hit(resources);
		}
}

void Pressure_Orb::draw(sf::RenderTexture& rt) const
{
	draw_subentities(rt);
	this->LO_Base::draw(rt);
}

bool Pressure_Orb::available_for_level_theme(LEVEL_THEME theme_id)
{
	return 
		theme_id == LEVEL_THEME::WATER ||
		theme_id == LEVEL_THEME::WIND ||
		theme_id == LEVEL_THEME::LIFEFORCE;
}

Colliding_Entity* Pressure_Orb::make_lo(LEVEL_THEME theme_id)
{
	return new Pressure_Orb(theme_id);
}

// ----------------------------------------------------------------
// Bubble_Machine
// ---------------------------------------------------------------- 

Bubble_Machine::Bubble_Machine(LEVEL_THEME theme_id)
{
	if (!available_for_level_theme(theme_id))
		throw std::runtime_error("Bubble_Machine::Bubble_Machine: invalid level theme");
}

void Bubble_Machine::extra_setup(std::istream& stream, Resources& resources)
{
	set_sprite_texture(resources.textures.get(RESOURCE_ID::TEXTURES::BUBBLE_MACHINE));
	set_sprite_animation(get_animation(AID_ENTITY::BUBBLE_MACHINE));
	center_sprite();
}

void Bubble_Machine::extra_pre_record(std::ostream& stream) const
{
	data_writer(stream, static_cast<int32_t>(LO_TYPE::COLLIDING_ENTITY));
	data_writer(stream, static_cast<int32_t>(COLLIDING_ENTITY_SUBTYPE::BUBBLE_MACHINE));
}

void Bubble_Machine::update(Resources& resources, Player& player, const sf::Time& dt,
	const hct::dynarray<std::unique_ptr<Solid_Tile>>& solid_tiles,
	const hct::dynarray<std::unique_ptr<Destructible_Tile>>& destructible_tiles)
{
	update_frame(dt);
	update_subentities(resources, dt, solid_tiles, destructible_tiles);

	if (dispense_timer.is_ready())
	{
		spawn_subentity(resources, get_sprite().getPosition());
		restart_sprite_animation();
	}

	for (const auto& bubble : get_subentities())
	{
		if (bubble->is_active())
		{
			if (player.is_alive() && collision(bubble->get_hitbox(), player.get_hitbox()))
			{
				if (player.get_position().y < bubble->get_hitbox().top)
					player.jump();
				bubble->set_popped(resources);
			}
			else
				for (const auto& bullet : player.get_subentities())
					if (bullet->is_active() && collision(bubble->get_hitbox(), bullet->get_hitbox()))
					{
						bubble->set_popped(resources);
						bullet->set_hit(resources);
					}
		}
	}
}

void Bubble_Machine::draw(sf::RenderTexture& rt) const
{
	draw_subentities(rt);
	this->LO_Base::draw(rt);
}

bool Bubble_Machine::available_for_level_theme(LEVEL_THEME theme_id)
{
	return theme_id == LEVEL_THEME::WATER;
}

void Bubble_Machine::update_timer(const sf::Time& dt)
{
	dispense_timer.update(dt);
}

Colliding_Entity* Bubble_Machine::make_lo(LEVEL_THEME theme_id)
{
	return new Bubble_Machine(theme_id);
}

// ----------------------------------------------------------------
// Bionic_Ray_Device
// ---------------------------------------------------------------- 

Bionic_Ray_Device::Bionic_Ray_Device(LEVEL_THEME theme_id) :
	Damage_Component(damage_stat)
{
	if (!available_for_level_theme(theme_id))
		throw std::runtime_error("Bionic_Ray_Device::Bionic_Ray_Device: invalid level theme");
}

sf::FloatRect Bionic_Ray_Device::get_device_hitbox() const noexcept
{
	return ::get_hitbox(get_sprite(), 0.8f, 0.8f);
}

sf::FloatRect Bionic_Ray_Device::get_ray_fragment_hitbox(const Animatable_Sprite& ray_fragment) const noexcept
{
	if (direction == DIRECTION::DOWN)
		return ::get_hitbox(ray_fragment, 0.1f, 0.9f);
	else
		return ::get_hitbox(ray_fragment, 0.9f, 0.1f);
}

void Bionic_Ray_Device::set_active() noexcept
{
	active = true;
	for (auto& ray_fragment : ray)
	{
		ray_fragment.get_animation_settings().row += 3;
		ray_fragment.setColor({ 255, 255, 255, active_transparency_value });
	}
}

void Bionic_Ray_Device::extra_setup(std::istream& stream, Resources& resources)
{
	set_sprite_texture(resources.textures.get(RESOURCE_ID::TEXTURES::BIONIC_RAY_DEVICE));
	set_sprite_animation(get_animation(AID_ENTITY::BIONIC_RAY_DEVICE));
	center_sprite();

	int32_t temp_direction;
	data_reader(stream, temp_direction);
	direction = static_cast<DIRECTION>(temp_direction);

	switch (direction)
	{
	case(DIRECTION::DOWN): break;
	case(DIRECTION::LEFT): set_sprite_rotation(90.f); break;
	case(DIRECTION::RIGHT): set_sprite_rotation(270.f); break;
	default: throw std::runtime_error("Bionic_Ray_Device::extra_setup: read direction out of range");
	}
}

void Bionic_Ray_Device::extra_pre_record(std::ostream& stream) const
{
	data_writer(stream, static_cast<int32_t>(LO_TYPE::COLLIDING_ENTITY));
	data_writer(stream, static_cast<int32_t>(COLLIDING_ENTITY_SUBTYPE::BIONIC_RAY_DEVICE));
}

void Bionic_Ray_Device::extra_post_record(std::ostream& stream) const
{
	data_writer(stream, static_cast<int32_t>(direction));
}

void Bionic_Ray_Device::deploy() noexcept
{
	awaiting = false;
	deploy_countdown.restart();
	ray_visibility_timer.restart();
}

void Bionic_Ray_Device::adjust_to_environment(Resources& resources,
	const hct::dynarray<std::unique_ptr<Solid_Tile>>& solid_tiles,
	const hct::dynarray<std::unique_ptr<Destructible_Tile>>& destructible_tiles)
{
	auto [solid_left, solid_right] = get_range_of_closest_objects(get_sprite(), solid_tiles, 15.f);
	auto [destructible_left, destructible_right] = get_range_of_closest_objects(get_sprite(), destructible_tiles, 15.f);
	unsigned int ray_fragments = 0u;

	sf::Vector2f move_vector;
	if (direction == DIRECTION::DOWN)
		move_vector.y = 32.f;
	else
		move_vector.x = (direction == DIRECTION::LEFT ? -32.f : 32.f);
	move(move_vector);
	for (unsigned int a = 1; a < max_ray_fragments; ++a)
	{
		for (auto it = solid_left; it != solid_right; ++it)
			if (collision(get_device_hitbox(), (*it)->get_hitbox()))
			{
				ray_fragments = std::max(a, min_ray_fragments);
				a = max_ray_fragments;
				break;
			}
		for (auto it = destructible_left; it != destructible_right; ++it)
			if (collision(get_device_hitbox(), (*it)->get_hitbox()))
			{
				ray_fragments = std::max(a, min_ray_fragments);
				a = max_ray_fragments;
				break;
			}
		move(move_vector);
	}
	if (ray_fragments == 0)
		ray_fragments = max_ray_fragments;

	set_position(static_cast<float>(get_original_position().x), static_cast<float>(get_original_position().y));
	move(16.f, 16.f);

	sf::Vector2f position = sf::Vector2f(static_cast<float>(get_original_position().x), static_cast<float>(get_original_position().y));
	const auto& texture = resources.textures.get(RESOURCE_ID::TEXTURES::BIONIC_RAY);
	auto animation = get_animation(AID_MISC::BIONIC_RAY);
	animation.row = 1;

	ray.reserve(ray_fragments);
	for (unsigned int a = 0u; a < ray_fragments; ++a)
	{
		ray.emplace_back();
		ray[a].setTexture(texture);
		ray[a].set_animation(animation);
		ray[a].setPosition(position);
		ray[a].setColor({ 255, 255, 255, inactive_transparency_value });
		set_center(ray[a]);
		ray[a].move(16.f, 16.f);
		ray[a].setRotation(get_sprite().getRotation());
		position += move_vector;
	}

	ray.front().get_animation_settings().row = 0;
	ray.back().get_animation_settings().row = 2;
}

void Bionic_Ray_Device::update(Resources& resources, Player& player, const sf::Time& dt,
	const hct::dynarray<std::unique_ptr<Solid_Tile>>& solid_tiles,
	const hct::dynarray<std::unique_ptr<Destructible_Tile>>& destructible_tiles)
{
	update_frame(dt);

	if (awaiting || active)
		visible_ray = true;
	else
	{
		deploy_countdown.update(dt);
		ray_visibility_timer.update(dt);
		if (ray_visibility_timer.is_ready())
			visible_ray = !visible_ray;
	}

	if (!awaiting && !active && deploy_countdown.is_done())
		set_active();

	if (ray.empty())
		adjust_to_environment(resources, solid_tiles, destructible_tiles);
	else
	{
		for (auto& ray_fragment : ray)
		{
			ray_fragment.update_frame(dt);
			if (collision(get_ray_fragment_hitbox(ray_fragment), player.get_hitbox()))
			{
				if (active)
					deal_damage(player);
				else if (awaiting)
					deploy();
			}
		}
	}
}

void Bionic_Ray_Device::draw(sf::RenderTexture& rt) const
{
	if (visible_ray)
		for (const auto& ray_fragment : ray)
			rt.draw(ray_fragment);
	this->LO_Base::draw(rt);
}

bool Bionic_Ray_Device::available_for_level_theme(LEVEL_THEME theme_id)
{
	return theme_id == LEVEL_THEME::LIFEFORCE;
}

Colliding_Entity* Bionic_Ray_Device::make_lo(LEVEL_THEME theme_id)
{
	return new Bionic_Ray_Device(theme_id);
}

// ----------------------------------------------------------------
// Electric_Gate_Device
// ---------------------------------------------------------------- 

Electric_Gate_Device::Electric_Gate_Device(LEVEL_THEME theme_id) :
	Damage_Component(damage_stat)
{
	if (!available_for_level_theme(theme_id))
		throw std::runtime_error("Electric_Gate_Device::Electric_Gate_Device: invalid level theme");
}

sf::FloatRect Electric_Gate_Device::get_device_hitbox() const noexcept
{
	return ::get_hitbox(get_sprite(), 0.8f, 0.8f);
}

sf::FloatRect Electric_Gate_Device::get_gate_fragment_hitbox(const Animatable_Sprite& gate_fragment) const noexcept
{
	if (direction == DIRECTION::DOWN)
		return ::get_hitbox(gate_fragment, 0.2f, 0.9f);
	else
		return ::get_hitbox(gate_fragment, 0.9f, 0.2f);
}

void Electric_Gate_Device::extra_setup(std::istream& stream, Resources& resources)
{
	set_sprite_texture(resources.textures.get(RESOURCE_ID::TEXTURES::ELECTRIC_GATE_DEVICE));
	set_sprite_animation(get_animation(AID_ENTITY::ELECTRIC_GATE_DEVICE));
	center_sprite();

	int32_t temp_direction;
	data_reader(stream, temp_direction);
	direction = static_cast<DIRECTION>(temp_direction);

	switch (direction)
	{
	case(DIRECTION::DOWN): break;
	case(DIRECTION::LEFT): set_sprite_rotation(90.f); break;
	case(DIRECTION::RIGHT): set_sprite_rotation(270.f); break;
	default: throw std::runtime_error("Electric_Gate_Device::extra_setup: read direction out of range");
	}
}

void Electric_Gate_Device::extra_pre_record(std::ostream& stream) const
{
	data_writer(stream, static_cast<int32_t>(LO_TYPE::COLLIDING_ENTITY));
	data_writer(stream, static_cast<int32_t>(COLLIDING_ENTITY_SUBTYPE::ELECTRIC_GATE_DEVICE));
}

void Electric_Gate_Device::extra_post_record(std::ostream& stream) const
{
	data_writer(stream, static_cast<int32_t>(direction));
}

void Electric_Gate_Device::adjust_to_environment(Resources& resources,
	const hct::dynarray<std::unique_ptr<Solid_Tile>>& solid_tiles,
	const hct::dynarray<std::unique_ptr<Destructible_Tile>>& destructible_tiles)
{
	auto [solid_left, solid_right] = get_range_of_closest_objects(get_sprite(), solid_tiles, 15.f);
	auto [destructible_left, destructible_right] = get_range_of_closest_objects(get_sprite(), destructible_tiles, 15.f);
	unsigned int gate_fragments = 0u;

	sf::Vector2f move_vector;
	if (direction == DIRECTION::DOWN)
		move_vector.y = 32.f;
	else
		move_vector.x = (direction == DIRECTION::LEFT ? -32.f : 32.f);
	move(move_vector);
	for (unsigned int a = 1; a < max_gate_fragments; ++a)
	{
		for (auto it = solid_left; it != solid_right; ++it)
			if (collision(get_device_hitbox(), (*it)->get_hitbox()))
			{
				gate_fragments = std::max(a, min_gate_fragments);
				a = max_gate_fragments;
				break;
			}
		for (auto it = destructible_left; it != destructible_right; ++it)
			if (collision(get_device_hitbox(), (*it)->get_hitbox()))
			{
				gate_fragments = std::max(a, min_gate_fragments);
				a = max_gate_fragments;
				break;
			}
		move(move_vector);
	}
	if (gate_fragments == 0)
		gate_fragments = max_gate_fragments;

	set_position(static_cast<float>(get_original_position().x), static_cast<float>(get_original_position().y));
	move(16.f, 16.f);

	sf::Vector2f position = sf::Vector2f(static_cast<float>(get_original_position().x), static_cast<float>(get_original_position().y));
	const auto& texture = resources.textures.get(RESOURCE_ID::TEXTURES::ELECTRIC_GATE);
	auto animation = get_animation(AID_MISC::ELECTRIC_GATE);
	animation.row = 1;

	gate.reserve(gate_fragments);
	for (unsigned int a = 0u; a < gate_fragments; ++a)
	{
		gate.emplace_back();
		gate[a].setTexture(texture);
		gate[a].set_animation(animation);
		gate[a].setPosition(position);
		gate[a].setColor({ 255, 255, 255, transparency_value });
		set_center(gate[a]);
		gate[a].move(16.f, 16.f);
		gate[a].setRotation(get_sprite().getRotation());
		position += move_vector;
	}

	gate.front().get_animation_settings().row = 0;
	gate.back().get_animation_settings().row = 2;
}

void Electric_Gate_Device::update(Resources& resources, Player& player, const sf::Time& dt,
	const hct::dynarray<std::unique_ptr<Solid_Tile>>& solid_tiles,
	const hct::dynarray<std::unique_ptr<Destructible_Tile>>& destructible_tiles)
{
	update_frame(dt);

	if (gate.empty())
		adjust_to_environment(resources, solid_tiles, destructible_tiles);
	else if (active)
	{
		for (auto& gate_fragment : gate)
		{
			gate_fragment.update_frame(dt);
			if (collision(get_gate_fragment_hitbox(gate_fragment), player.get_hitbox()))
				deal_damage(player);
		}
	}
}

void Electric_Gate_Device::draw(sf::RenderTexture& rt) const
{
	if (active)
		for (const auto& gate_fragment : gate)
			rt.draw(gate_fragment);
	this->LO_Base::draw(rt);
}

bool Electric_Gate_Device::available_for_level_theme(LEVEL_THEME theme_id)
{
	return theme_id == LEVEL_THEME::ELECTRIC;
}

void Electric_Gate_Device::update_timer(const sf::Time& dt)
{
	switch_timer.update(dt);
	if (switch_timer.is_ready())
		active = !active;
}

Colliding_Entity* Electric_Gate_Device::make_lo(LEVEL_THEME theme_id)
{
	return new Electric_Gate_Device(theme_id);
}

// ----------------------------------------------------------------
// Flamethrower
// ----------------------------------------------------------------

Flamethrower::Flamethrower(LEVEL_THEME theme_id) :
	Damage_Component(damage_stat)
{
	if (!available_for_level_theme(theme_id))
		throw std::runtime_error("Flamethrower::Flamethrower: invalid level theme");
}

sf::FloatRect Flamethrower::get_device_hitbox() const noexcept
{
	return ::get_hitbox(get_sprite(), 0.8f, 0.8f);
}

sf::FloatRect Flamethrower::get_flame_fragment_hitbox(const Animatable_Sprite& flame_fragment) const noexcept
{
	return ::get_hitbox(flame_fragment, 0.7f, 0.7f);
}

void Flamethrower::extra_setup(std::istream& stream, Resources& resources)
{
	set_sprite_texture(resources.textures.get(RESOURCE_ID::TEXTURES::FLAMETHROWER));
	set_sprite_animation(get_animation(AID_ENTITY::FLAMETHROWER));
	center_sprite();
}

void Flamethrower::extra_pre_record(std::ostream& stream) const
{
	data_writer(stream, static_cast<int32_t>(LO_TYPE::COLLIDING_ENTITY));
	data_writer(stream, static_cast<int32_t>(COLLIDING_ENTITY_SUBTYPE::FLAMETHROWER));
}

void Flamethrower::adjust_to_environment(Resources& resources,
	const hct::dynarray<std::unique_ptr<Solid_Tile>>& solid_tiles,
	const hct::dynarray<std::unique_ptr<Destructible_Tile>>& destructible_tiles)
{
	auto [solid_left, solid_right] = get_range_of_closest_objects(get_sprite(), solid_tiles, 15.f);
	auto [destructible_left, destructible_right] = get_range_of_closest_objects(get_sprite(), destructible_tiles, 15.f);
	unsigned int flame_fragments = 0u;

	sf::Vector2f move_vector = { 0.f, -32.f };
	for (unsigned int a = 1; a <= max_flame_fragments; ++a)
	{
		for (auto it = solid_left; it != solid_right; ++it)
			if (collision(get_device_hitbox(), (*it)->get_hitbox()))
			{
				flame_fragments = std::max(a - 1, min_flame_fragments);
				a = max_flame_fragments;
				break;
			}
		for (auto it = destructible_left; it != destructible_right; ++it)
			if (collision(get_device_hitbox(), (*it)->get_hitbox()))
			{
				flame_fragments = std::max(a - 1, min_flame_fragments);
				a = max_flame_fragments;
				break;
			}
		move(move_vector);
	}
	if (flame_fragments == 0)
		flame_fragments = max_flame_fragments;

	set_position(static_cast<float>(get_original_position().x), static_cast<float>(get_original_position().y));
	move(16.f, 16.f);

	sf::Vector2f position = sf::Vector2f(static_cast<float>(get_original_position().x), static_cast<float>(get_original_position().y));
	const auto& texture = resources.textures.get(RESOURCE_ID::TEXTURES::FLAMETHROWER_FLAME);
	auto animation = get_animation(AID_MISC::FLAMETHROWER_FLAME);
	animation.row = 1;

	flame.reserve(flame_fragments);
	for (unsigned int a = 0u; a < flame_fragments; ++a)
	{
		flame.emplace_back();
		flame[a].setTexture(texture);
		flame[a].set_animation(animation);
		flame[a].setPosition(position);
		flame[a].setColor({ 255, 255, 255, transparency_value });
		set_center(flame[a]);
		flame[a].move(16.f, 16.f);
		position += move_vector;
	}

	flame.front().get_animation_settings().row = 0;
	flame.back().get_animation_settings().row = 2;
}

void Flamethrower::update(Resources& resources, Player& player, const sf::Time& dt,
	const hct::dynarray<std::unique_ptr<Solid_Tile>>& solid_tiles,
	const hct::dynarray<std::unique_ptr<Destructible_Tile>>& destructible_tiles)
{
	update_frame(dt);

	if (flame.empty())
		adjust_to_environment(resources, solid_tiles, destructible_tiles);
	else
	{
		if (spit_timer.is_ready())
			restart_sprite_animation();
		for (auto& flame_fragment : flame)
		{
			if (spit_timer.is_ready())
				flame_fragment.restart_animation();
			flame_fragment.update_frame(dt);
			if (!flame_fragment.finished() && collision(get_flame_fragment_hitbox(flame_fragment), player.get_hitbox()))
				deal_damage(player);
		}
	}
}

void Flamethrower::draw(sf::RenderTexture& rt) const
{
	for (const auto& flame_fragment : flame)
		rt.draw(flame_fragment);
	this->LO_Base::draw(rt);
}

bool Flamethrower::available_for_level_theme(LEVEL_THEME theme_id)
{
	return theme_id == LEVEL_THEME::FIRE;
}

void Flamethrower::update_timer(const sf::Time& dt)
{
	spit_timer.update(dt);
}

Colliding_Entity* Flamethrower::make_lo(LEVEL_THEME theme_id)
{
	return new Flamethrower(theme_id);
}

// ----------------------------------------------------------------
// Caster
// ----------------------------------------------------------------

Caster::Caster(LEVEL_THEME theme_id) :
	Bullet_Manager_Component(theme_to_bullet_color(theme_id), false), 
	Enemy_Component(health_stat, damage_stat, colliding_entity_value(COLLIDING_ENTITY_SUBTYPE::CASTER))
{
	auto animation = get_animation(AID_ENTITY::CASTERS);
	animation.row = static_cast<int>(theme_id) - 1;
	if (animation.row < 0 || theme_id >= LEVEL_THEME::COUNT)
		throw std::runtime_error("Caster_Base::Caster_Base: invalid theme_id provided");
	set_sprite_animation(animation);
}

void Caster::set_death_graphics(Resources& resources)
{
	set_sprite_texture(resources.textures.get(RESOURCE_ID::TEXTURES::EXPLOSION_48));
	set_sprite_animation(get_animation(AID_MISC::EXPLOSION_48));
}

void Caster::extra_setup(std::istream& stream, Resources& resources)
{
	setup_health_info(resources);
	set_sprite_texture(resources.textures.get(RESOURCE_ID::TEXTURES::CASTER));
	center_sprite();
}

void Caster::extra_pre_record(std::ostream& stream) const
{
	data_writer(stream, static_cast<int32_t>(LO_TYPE::COLLIDING_ENTITY));
	data_writer(stream, static_cast<int32_t>(COLLIDING_ENTITY_SUBTYPE::CASTER));
}

void Caster::extra_player_interaction(Resources& resources, Player& player, const sf::FloatRect& my_hitbox)
{
	if (player.get_position().x < get_sprite().getPosition().x)
		get_sprite_animation().horizontally_flipped = true;
	else if (player.get_position().x > get_sprite().getPosition().x)
		get_sprite_animation().horizontally_flipped = false;

	for (const auto& bullet : get_subentities())
		if (bullet->is_active() && player.is_alive() && collision(bullet->get_hitbox(), player.get_hitbox()))
		{
			deal_damage(player);
			bullet->set_hit(resources);
		}
}

sf::FloatRect Caster::get_hitbox()
{
	return ::get_hitbox(get_sprite(), 0.8f, 0.8f);
}

void Caster::update(Resources& resources, Player& player, const sf::Time& dt,
	const hct::dynarray<std::unique_ptr<Solid_Tile>>& solid_tiles,
	const hct::dynarray<std::unique_ptr<Destructible_Tile>>& destructible_tiles)
{
	update_frame(dt);
	update_subentities(resources, dt, solid_tiles, destructible_tiles);
	update_health_info(get_sprite(), dt);

	if (is_alive() && shoot_timer.is_ready())
	{
		sf::Vector2f position = get_sprite().getPosition();
		position.x += (get_sprite_animation().horizontally_flipped ? -16.f : 16.f);
		spawn_subentity(resources, position, (get_sprite_animation().horizontally_flipped ? Bullet::DIRECTION::LEFT : Bullet::DIRECTION::RIGHT));
		restart_sprite_animation();
	}

	player_interaction(resources, player, get_hitbox(), is_alive(), is_alive());
}

void Caster::draw(sf::RenderTexture& rt) const
{
	draw_subentities(rt);
	draw_health_info(rt);
	this->LO_Base::draw(rt);
}

void Caster::update_timer(const sf::Time& dt)
{
	shoot_timer.update(dt);
}

Colliding_Entity* Caster::make_lo(LEVEL_THEME theme_id)
{
	return new Caster(theme_id);
}

// ----------------------------------------------------------------
// Elemental
// ----------------------------------------------------------------

Elemental::Elemental(LEVEL_THEME theme_id) :
	Bullet_Manager_Component(theme_to_bullet_color(theme_id), false),
	Enemy_Component(health_stat, damage_stat, colliding_entity_value(COLLIDING_ENTITY_SUBTYPE::ELEMENTAL)), 
	shoot_countdown(shoot_countdown_time)
{
	auto animation = get_animation(AID_ENTITY::ELEMENTALS);
	animation.row = static_cast<int>(theme_id) - 1;
	if (animation.row < 0 || theme_id >= LEVEL_THEME::COUNT)
		throw std::runtime_error("Elemental_Base::Elemental_Base: invalid theme_id provided");
	set_sprite_animation(animation);
}

void Elemental::set_death_graphics(Resources& resources)
{
	set_sprite_texture(resources.textures.get(RESOURCE_ID::TEXTURES::EXPLOSION_32));
	set_sprite_animation(get_animation(AID_MISC::EXPLOSION_32));
}


void Elemental::extra_setup(std::istream& stream, Resources& resources)
{
	setup_health_info(resources);
	set_sprite_texture(resources.textures.get(RESOURCE_ID::TEXTURES::ELEMENTAL));
	center_sprite();
}

void Elemental::extra_pre_record(std::ostream& stream) const
{
	data_writer(stream, static_cast<int32_t>(LO_TYPE::COLLIDING_ENTITY));
	data_writer(stream, static_cast<int32_t>(COLLIDING_ENTITY_SUBTYPE::ELEMENTAL));
}

void Elemental::extra_player_interaction(Resources& resources, Player& player, const sf::FloatRect& my_hitbox)
{
	if (player.get_position().x < get_sprite().getPosition().x)
		get_sprite_animation().horizontally_flipped = true;
	else if (player.get_position().x > get_sprite().getPosition().x)
		get_sprite_animation().horizontally_flipped = false;

	for (const auto& bullet : get_subentities())
		if (bullet->is_active() && player.is_alive() && collision(bullet->get_hitbox(), player.get_hitbox()))
		{
			deal_damage(player);
			bullet->set_hit(resources);
		}
}

sf::FloatRect Elemental::get_hitbox()
{
	return ::get_hitbox(get_sprite(), 0.9f, 0.9f);
}

void Elemental::update(Resources& resources, Player& player, const sf::Time& dt,
	const hct::dynarray<std::unique_ptr<Solid_Tile>>& solid_tiles,
	const hct::dynarray<std::unique_ptr<Destructible_Tile>>& destructible_tiles)
{
	update_frame(dt);
	update_subentities(resources, dt, solid_tiles, destructible_tiles);

	if (is_alive())
	{
		if (moving_down)
		{
			move(0.f, speed_stat * dt.asSeconds());
			if (float limit = get_original_position().y + 16.f; get_sprite().getPosition().y >= limit)
			{
				moving_down = false;
				set_position(get_sprite().getPosition().x, limit);
				shoot_countdown.restart();
			}
		}
		else if (moving_up)
		{
			move(0.f, -speed_stat * dt.asSeconds());
			if (float limit = get_original_position().y + 16.f - max_travel_distance; get_sprite().getPosition().y <= limit)
			{
				moving_up = false;
				set_position(get_sprite().getPosition().x, limit);
				shoot_countdown.restart();
			}
		}
		else
		{
			shoot_countdown.update(dt);
			if (shoot_countdown.is_done())
			{
				sf::Vector2f position = get_sprite().getPosition();
				position.x += get_sprite_animation().horizontally_flipped ? -12.f : 12.f;
				spawn_subentity(resources, position, (get_sprite_animation().horizontally_flipped ? Bullet::DIRECTION::LEFT : Bullet::DIRECTION::RIGHT));

				if (get_sprite().getPosition().y > get_original_position().y)
					moving_up = true;
				else
					moving_down = true;
			}
		}
	}

	update_health_info(get_sprite(), dt);
	player_interaction(resources, player, get_hitbox(), is_alive(), is_alive());
}

void Elemental::draw(sf::RenderTexture& rt) const
{
	draw_subentities(rt);
	draw_health_info(rt);
	this->LO_Base::draw(rt);
}

Colliding_Entity* Elemental::make_lo(LEVEL_THEME theme_id)
{
	return new Elemental(theme_id);
}

// ----------------------------------------------------------------
// Jungle_Boomeranger
// ----------------------------------------------------------------

Jungle_Boomeranger::Jungle_Boomeranger(LEVEL_THEME theme_id) :
	Boomerang_Manager_Component(LEVEL_THEME::VERDANT),
	Enemy_Component(health_stat, damage_stat, colliding_entity_value(COLLIDING_ENTITY_SUBTYPE::JUNGLE_BOOMERANGER))
{
	if (!available_for_level_theme(theme_id))
		throw std::runtime_error("Jungle_Boomeranger::Jungle_Boomeranger: invalid level theme");
}

void Jungle_Boomeranger::set_death_graphics(Resources& resources)
{
	set_sprite_texture(resources.textures.get(RESOURCE_ID::TEXTURES::EXPLOSION_48));
	set_sprite_animation(get_animation(AID_MISC::EXPLOSION_48));
}

void Jungle_Boomeranger::extra_player_interaction(Resources& resources, Player& player, const sf::FloatRect& my_hitbox)
{
	if (player.get_position().x < get_sprite().getPosition().x)
		get_sprite_animation().horizontally_flipped = true;
	else if (player.get_position().x > get_sprite().getPosition().x)
		get_sprite_animation().horizontally_flipped = false;

	for (const auto& boomerang : get_subentities())
		if (boomerang->is_active())
			if (player.is_alive() && collision(boomerang->get_hitbox(), player.get_hitbox()))
				deal_damage(player);
}

void Jungle_Boomeranger::extra_setup(std::istream& stream, Resources& resources)
{
	setup_health_info(resources);
	set_sprite_texture(resources.textures.get(RESOURCE_ID::TEXTURES::JUNGLE_BOOMERANGER));
	set_sprite_animation(get_animation(AID_ENTITY::JUNGLE_BOOMERANGER));
	center_sprite();
}

void Jungle_Boomeranger::extra_pre_record(std::ostream& stream) const
{
	data_writer(stream, static_cast<int32_t>(LO_TYPE::COLLIDING_ENTITY));
	data_writer(stream, static_cast<int32_t>(COLLIDING_ENTITY_SUBTYPE::JUNGLE_BOOMERANGER));
}

sf::FloatRect Jungle_Boomeranger::get_hitbox()
{
	return ::get_hitbox(get_sprite(), 0.8f, 0.8f);
}

void Jungle_Boomeranger::update(Resources& resources, Player& player, const sf::Time& dt,
	const hct::dynarray<std::unique_ptr<Solid_Tile>>& solid_tiles,
	const hct::dynarray<std::unique_ptr<Destructible_Tile>>& destructible_tiles)
{
	update_frame(dt);
	update_subentities(resources, dt, solid_tiles, destructible_tiles);
	update_health_info(get_sprite(), dt);

	if (is_alive() && throw_timer.is_ready())
	{
		sf::Vector2f position = get_sprite().getPosition();
		position.x += (get_sprite_animation().horizontally_flipped ? -16.f : 16.f);
		spawn_subentity(resources, position, (get_sprite_animation().horizontally_flipped ? Boomerang::DIRECTION::LEFT : Boomerang::DIRECTION::RIGHT));
		restart_sprite_animation();
	}

	player_interaction(resources, player, get_hitbox(), is_alive(), is_alive());
}

void Jungle_Boomeranger::draw(sf::RenderTexture& rt) const
{
	draw_subentities(rt);
	draw_health_info(rt);
	this->LO_Base::draw(rt);
}

bool Jungle_Boomeranger::available_for_level_theme(LEVEL_THEME theme_id)
{
	return theme_id == LEVEL_THEME::VERDANT;
}

void Jungle_Boomeranger::update_timer(const sf::Time& dt)
{
	throw_timer.update(dt);
}

Colliding_Entity* Jungle_Boomeranger::make_lo(LEVEL_THEME theme_id)
{
	return new Jungle_Boomeranger(theme_id);
}

// ----------------------------------------------------------------
// Aquaveil_Weaver
// ----------------------------------------------------------------

Aquaveil_Weaver::Aquaveil_Weaver(LEVEL_THEME theme_id) :
	Bullet_Manager_Component(Bullet::TYPE::AQUA_WAVE, false),
	Enemy_Component(health_stat, damage_stat, colliding_entity_value(COLLIDING_ENTITY_SUBTYPE::AQUAVEIL_WEAVER))
{
	if (!available_for_level_theme(theme_id))
		throw std::runtime_error("Aquaveil_Weaver::Aquaveil_Weaver: invalid level theme");
}

void Aquaveil_Weaver::set_death_graphics(Resources& resources)
{
	set_sprite_texture(resources.textures.get(RESOURCE_ID::TEXTURES::EXPLOSION_48));
	set_sprite_animation(get_animation(AID_MISC::EXPLOSION_48));
}

void Aquaveil_Weaver::extra_player_interaction(Resources& resources, Player& player, const sf::FloatRect& my_hitbox)
{
	for (const auto& bullet : get_subentities())
		if (bullet->is_active())
		{
			if (player.is_alive() && collision(bullet->get_hitbox(), player.get_hitbox()))
			{
				deal_damage(player);
				bullet->set_hit(resources);
			}

			for (const auto& player_bullet : player.get_subentities())
			{
				if (collision(bullet->get_hitbox(), player_bullet->get_hitbox()))
					player_bullet->set_hit(resources);
			}
		}
}

void Aquaveil_Weaver::extra_setup(std::istream& stream, Resources& resources)
{
	setup_health_info(resources);
	set_sprite_texture(resources.textures.get(RESOURCE_ID::TEXTURES::AQUAVEIL_WEAVER));
	set_sprite_animation(get_animation(AID_ENTITY::AQUAVEIL_WEAVER_FORM));
	center_sprite();
}

void Aquaveil_Weaver::extra_pre_record(std::ostream& stream) const
{
	data_writer(stream, static_cast<int32_t>(LO_TYPE::COLLIDING_ENTITY));
	data_writer(stream, static_cast<int32_t>(COLLIDING_ENTITY_SUBTYPE::AQUAVEIL_WEAVER));
}

sf::FloatRect Aquaveil_Weaver::get_hitbox()
{
	return ::get_hitbox(get_sprite(), 0.8f, 0.8f);
}

void Aquaveil_Weaver::update(Resources& resources, Player& player, const sf::Time& dt,
	const hct::dynarray<std::unique_ptr<Solid_Tile>>& solid_tiles,
	const hct::dynarray<std::unique_ptr<Destructible_Tile>>& destructible_tiles)
{
	update_frame(dt);
	update_subentities(resources, dt, solid_tiles, destructible_tiles);
	update_health_info(get_sprite(), dt);

	if (veils_created)
	{
		veil_release_countdown.update(dt);
		if (veil_release_countdown.is_done())
		{
			veils_created = false;
			veil_creation_countdown.restart();
			set_subentities_speed(Bullet::default_speed); // PROBLJEM
			if (is_alive())
				set_sprite_animation(get_animation(AID_ENTITY::AQUAVEIL_WEAVER_RELEASE));
		}
	}
	else if (is_alive())
	{
		veil_creation_countdown.update(dt);
		if (veil_creation_countdown.is_done())
		{
			veils_created = true;
			veil_release_countdown.restart();
			sf::Vector2f position = get_sprite().getPosition();
			position.x -= 32.f;
			spawn_subentity(resources, position, Bullet::DIRECTION::LEFT, 0.0f, Bullet::default_max_distance * 1.5f);
			position.x += 64.f;
			spawn_subentity(resources, position, Bullet::DIRECTION::RIGHT, 0.0f, Bullet::default_max_distance * 1.5f);
			set_sprite_animation(get_animation(AID_ENTITY::AQUAVEIL_WEAVER_FORM));
		}
	}

	player_interaction(resources, player, get_hitbox(), is_alive(), is_alive());
}

void Aquaveil_Weaver::draw(sf::RenderTexture& rt) const
{
	draw_subentities(rt);
	draw_health_info(rt);
	this->LO_Base::draw(rt);
}

bool Aquaveil_Weaver::available_for_level_theme(LEVEL_THEME theme_id)
{
	return theme_id == LEVEL_THEME::WATER;
}

Colliding_Entity* Aquaveil_Weaver::make_lo(LEVEL_THEME theme_id)
{
	return new Aquaveil_Weaver(theme_id);
}

// ----------------------------------------------------------------
// Energized_Galvanizer
// ----------------------------------------------------------------

Energized_Galvanizer::Energized_Galvanizer(LEVEL_THEME theme_id) :
	Ball_Manager_Component(LEVEL_THEME::ELECTRIC), 
	Enemy_Component(health_stat, damage_stat, colliding_entity_value(COLLIDING_ENTITY_SUBTYPE::ENERGIZED_GALVANIZER)),
	Vertical_Physics_Component(*this)
{
	if (!available_for_level_theme(theme_id))
		throw std::runtime_error("Energized_Galvanizer::Energized_Galvanizer: invalid level theme");
}

void Energized_Galvanizer::set_death_graphics(Resources& resources)
{
	set_sprite_texture(resources.textures.get(RESOURCE_ID::TEXTURES::EXPLOSION_32));
	set_sprite_animation(get_animation(AID_MISC::EXPLOSION_32));
}

void Energized_Galvanizer::extra_player_interaction(Resources& resources, Player& player, const sf::FloatRect& my_hitbox)
{
	for (const auto& ball : get_subentities())
		if (ball->is_active())
			if (player.is_alive() && collision(ball->get_hitbox(), player.get_hitbox()))
			{
				deal_damage(player);
				ball->set_hit(resources);
			}
}

void Energized_Galvanizer::extra_setup(std::istream& stream, Resources& resources)
{
	setup_health_info(resources);
	set_sprite_texture(resources.textures.get(RESOURCE_ID::TEXTURES::ENERGIZED_GALVANIZER));
	set_sprite_animation(get_animation(AID_ENTITY::ENERGIZED_GALVANIZER));
	center_sprite();
}

void Energized_Galvanizer::extra_pre_record(std::ostream& stream) const
{
	data_writer(stream, static_cast<int32_t>(LO_TYPE::COLLIDING_ENTITY));
	data_writer(stream, static_cast<int32_t>(COLLIDING_ENTITY_SUBTYPE::ENERGIZED_GALVANIZER));
}

void Energized_Galvanizer::jump()
{
	set_gravity(-Vertical_Physics_Component::max_gravity);
	set_gravity_cooldown(320.f);
}

sf::FloatRect Energized_Galvanizer::get_hitbox()
{
	return ::get_hitbox(get_sprite(), 0.8f, 0.8f);
}

void Energized_Galvanizer::update(Resources& resources, Player& player, const sf::Time& dt,
	const hct::dynarray<std::unique_ptr<Solid_Tile>>& solid_tiles,
	const hct::dynarray<std::unique_ptr<Destructible_Tile>>& destructible_tiles)
{
	update_frame(dt);
	update_subentities(resources, dt, solid_tiles, destructible_tiles);

	player_interaction(resources, player, get_hitbox(), is_alive(), is_alive());

	if (is_alive())
	{
		auto [solid_left, solid_right] = get_range_of_closest_objects(get_sprite(), solid_tiles);
		auto [destructible_left, destructible_right] = get_range_of_closest_objects(get_sprite(), destructible_tiles);

		move(0, update_vertically(dt));

		if (check_vertical_collisions(solid_left, solid_right, get_hitbox()) == Vertical_Physics_Component::COLLISION::BOTTOM ||
			check_vertical_collisions(destructible_left, destructible_right, get_hitbox()) == Vertical_Physics_Component::COLLISION::BOTTOM)
			jump();
	}
	else if (!balls_thrown)
	{
		balls_thrown = true;
		spawn_subentity(resources, get_sprite().getPosition(), -160.f, -300.f, 400.f, 4);
		spawn_subentity(resources, get_sprite().getPosition(), 160.f, -300.f, 400.f, 4);
	}

	update_health_info(get_sprite(), dt);
}

void Energized_Galvanizer::draw(sf::RenderTexture& rt) const
{
	draw_subentities(rt);
	draw_health_info(rt);
	this->LO_Base::draw(rt);
}

bool Energized_Galvanizer::available_for_level_theme(LEVEL_THEME theme_id)
{
	return theme_id == LEVEL_THEME::ELECTRIC;
}

Colliding_Entity* Energized_Galvanizer::make_lo(LEVEL_THEME theme_id)
{
	return new Energized_Galvanizer(theme_id);
}

// ----------------------------------------------------------------
// Aerial_Bombardier
// ----------------------------------------------------------------

Aerial_Bombardier::Aerial_Bombardier(LEVEL_THEME theme_id) :
	Ball_Manager_Component(LEVEL_THEME::WIND),
	Enemy_Component(health_stat, damage_stat, colliding_entity_value(COLLIDING_ENTITY_SUBTYPE::AERIAL_BOMBARDIER))
{
	if (!available_for_level_theme(theme_id))
		throw std::runtime_error("Aerial_Bombardier::Aerial_Bombardier: invalid level theme");
}

void Aerial_Bombardier::set_death_graphics(Resources& resources)
{
	set_sprite_texture(resources.textures.get(RESOURCE_ID::TEXTURES::EXPLOSION_48));
	set_sprite_animation(get_animation(AID_MISC::EXPLOSION_48));
}

void Aerial_Bombardier::extra_player_interaction(Resources& resources, Player& player, const sf::FloatRect& my_hitbox)
{
	if (player.get_position().x < get_sprite().getPosition().x)
		get_sprite_animation().horizontally_flipped = true;
	else if (player.get_position().x > get_sprite().getPosition().x)
		get_sprite_animation().horizontally_flipped = false;

	for (const auto& ball : get_subentities())
		if (ball->is_active())
			if (player.is_alive() && collision(ball->get_hitbox(), player.get_hitbox()))
			{
				deal_damage(player);
				ball->set_hit(resources);
			}
}

void Aerial_Bombardier::extra_setup(std::istream& stream, Resources& resources)
{
	setup_health_info(resources);
	set_sprite_texture(resources.textures.get(RESOURCE_ID::TEXTURES::AERIAL_BOMBARDIER));
	set_sprite_animation(get_animation(AID_ENTITY::AERIAL_BOMBARDIER));
	center_sprite();
}

void Aerial_Bombardier::extra_pre_record(std::ostream& stream) const
{
	data_writer(stream, static_cast<int32_t>(LO_TYPE::COLLIDING_ENTITY));
	data_writer(stream, static_cast<int32_t>(COLLIDING_ENTITY_SUBTYPE::AERIAL_BOMBARDIER));
}

sf::FloatRect Aerial_Bombardier::get_hitbox()
{
	return ::get_hitbox(get_sprite(), 0.8f, 0.8f);
}

void Aerial_Bombardier::update(Resources& resources, Player& player, const sf::Time& dt,
	const hct::dynarray<std::unique_ptr<Solid_Tile>>& solid_tiles,
	const hct::dynarray<std::unique_ptr<Destructible_Tile>>& destructible_tiles)
{
	update_frame(dt);
	update_subentities(resources, dt, solid_tiles, destructible_tiles);
	update_health_info(get_sprite(), dt);

	if (is_alive() && throw_timer.is_ready())
	{
		sf::Vector2f position = get_sprite().getPosition();
		bool left = get_sprite_animation().horizontally_flipped;
		position.x += (left ? -16.f : 16.f);
		position.y += 8.f;
		spawn_subentity(resources, position, 160.f * (left ? -1.f : 1.f), -400.f, 400.f, 5);
		restart_sprite_animation();
	}

	player_interaction(resources, player, get_hitbox(), is_alive(), is_alive());
}

void Aerial_Bombardier::draw(sf::RenderTexture& rt) const
{
	draw_subentities(rt);
	draw_health_info(rt);
	this->LO_Base::draw(rt);
}

bool Aerial_Bombardier::available_for_level_theme(LEVEL_THEME theme_id)
{
	return theme_id == LEVEL_THEME::WIND;
}

void Aerial_Bombardier::update_timer(const sf::Time& dt)
{
	throw_timer.update(dt);
}

Colliding_Entity* Aerial_Bombardier::make_lo(LEVEL_THEME theme_id)
{
	return new Aerial_Bombardier(theme_id);
}

// ----------------------------------------------------------------
// Rock_Hurler
// ----------------------------------------------------------------

Rock_Hurler::Rock_Hurler(LEVEL_THEME theme_id) :
	Rock_Manager_Component(LEVEL_THEME::GROUND),
	Enemy_Component(health_stat, damage_stat, colliding_entity_value(COLLIDING_ENTITY_SUBTYPE::ROCK_HURLER))
{
	if (!available_for_level_theme(theme_id))
		throw std::runtime_error("Rock_Hurler::Rock_Hurler: invalid level theme");
}

void Rock_Hurler::set_death_graphics(Resources& resources)
{
	set_sprite_texture(resources.textures.get(RESOURCE_ID::TEXTURES::EXPLOSION_48));
	set_sprite_animation(get_animation(AID_MISC::EXPLOSION_48));
}

void Rock_Hurler::extra_player_interaction(Resources& resources, Player& player, const sf::FloatRect& my_hitbox)
{
	if (player.get_position().x < get_sprite().getPosition().x)
		get_sprite_animation().horizontally_flipped = true;
	else if (player.get_position().x > get_sprite().getPosition().x)
		get_sprite_animation().horizontally_flipped = false;

	for (const auto& rock : get_subentities())
		if (rock->is_active())
			if (player.is_alive() && collision(rock->get_hitbox(), player.get_hitbox()))
			{
				deal_damage(player);
				rock->set_hit(resources);
			}
}

void Rock_Hurler::extra_setup(std::istream& stream, Resources& resources)
{
	setup_health_info(resources);
	set_sprite_texture(resources.textures.get(RESOURCE_ID::TEXTURES::ROCK_HURLER));
	set_sprite_animation(get_animation(AID_ENTITY::ROCK_HURLER));
	center_sprite();
}

void Rock_Hurler::extra_pre_record(std::ostream& stream) const
{
	data_writer(stream, static_cast<int32_t>(LO_TYPE::COLLIDING_ENTITY));
	data_writer(stream, static_cast<int32_t>(COLLIDING_ENTITY_SUBTYPE::ROCK_HURLER));
}

sf::FloatRect Rock_Hurler::get_hitbox()
{
	return ::get_hitbox(get_sprite(), 0.8f, 0.8f);
}

void Rock_Hurler::update(Resources& resources, Player& player, const sf::Time& dt,
	const hct::dynarray<std::unique_ptr<Solid_Tile>>& solid_tiles,
	const hct::dynarray<std::unique_ptr<Destructible_Tile>>& destructible_tiles)
{
	update_frame(dt);
	update_subentities(resources, dt, solid_tiles, destructible_tiles);
	update_health_info(get_sprite(), dt);

	if (is_alive() && hurl_timer.is_ready())
	{
		sf::Vector2f position = get_sprite().getPosition();
		position.x += (get_sprite_animation().horizontally_flipped ? -16.f : 16.f);
		position.y -= 16.f;
		for (int a = 0; a < rocks_per_hurl; ++a)
		{
			float velocity = (100.f + 100.f * a) * (get_sprite_animation().horizontally_flipped ? -1.f : 1.f);
			spawn_subentity(resources, position, velocity, -900.f);
		}
		restart_sprite_animation();
	}

	player_interaction(resources, player, get_hitbox(), is_alive(), is_alive());
}

void Rock_Hurler::draw(sf::RenderTexture& rt) const
{
	draw_subentities(rt);
	draw_health_info(rt);
	this->LO_Base::draw(rt);
}

bool Rock_Hurler::available_for_level_theme(LEVEL_THEME theme_id)
{
	return theme_id == LEVEL_THEME::GROUND;
}

void Rock_Hurler::update_timer(const sf::Time& dt)
{
	hurl_timer.update(dt);
}

Colliding_Entity* Rock_Hurler::make_lo(LEVEL_THEME theme_id)
{
	return new Rock_Hurler(theme_id);
}

// ----------------------------------------------------------------
// Crimson_Lasher
// ----------------------------------------------------------------

Crimson_Lasher::Crimson_Lasher(LEVEL_THEME theme_id) :
	Bullet_Manager_Component(Bullet::TYPE::CRIMSON_WAVE, false),
	Enemy_Component(health_stat, damage_stat, colliding_entity_value(COLLIDING_ENTITY_SUBTYPE::CRIMSON_LASHER))
{
	if (!available_for_level_theme(theme_id))
		throw std::runtime_error("Crimson_Lasher::Crimson_Lasher: invalid level theme");
}

void Crimson_Lasher::set_death_graphics(Resources& resources)
{
	set_sprite_texture(resources.textures.get(RESOURCE_ID::TEXTURES::EXPLOSION_48));
	set_sprite_animation(get_animation(AID_MISC::EXPLOSION_48));
}

void Crimson_Lasher::extra_player_interaction(Resources& resources, Player& player, const sf::FloatRect& my_hitbox)
{
	if (player.get_position().x < get_sprite().getPosition().x)
		get_sprite_animation().horizontally_flipped = true;
	else if (player.get_position().x > get_sprite().getPosition().x)
		get_sprite_animation().horizontally_flipped = false;

	for (const auto& bullet : get_subentities())
		if (bullet->is_active())
		{
			if (player.is_alive() && collision(bullet->get_hitbox(), player.get_hitbox()))
			{
				deal_damage(player);
				bullet->set_hit(resources);
			}

			for (const auto& player_bullet : player.get_subentities())
			{
				if (collision(bullet->get_hitbox(), player_bullet->get_hitbox()))
					player_bullet->set_hit(resources);
			}
		}
}

void Crimson_Lasher::extra_setup(std::istream& stream, Resources& resources)
{
	setup_health_info(resources);
	set_sprite_texture(resources.textures.get(RESOURCE_ID::TEXTURES::CRIMSON_LASHER));
	set_sprite_animation(get_animation(AID_ENTITY::CRIMSON_LASHER));
	center_sprite();
}

void Crimson_Lasher::extra_pre_record(std::ostream& stream) const
{
	data_writer(stream, static_cast<int32_t>(LO_TYPE::COLLIDING_ENTITY));
	data_writer(stream, static_cast<int32_t>(COLLIDING_ENTITY_SUBTYPE::CRIMSON_LASHER));
}

sf::FloatRect Crimson_Lasher::get_hitbox()
{
	return ::get_hitbox(get_sprite(), 0.8f, 0.8f);
}

void Crimson_Lasher::update(Resources& resources, Player& player, const sf::Time& dt,
	const hct::dynarray<std::unique_ptr<Solid_Tile>>& solid_tiles,
	const hct::dynarray<std::unique_ptr<Destructible_Tile>>& destructible_tiles)
{
	update_frame(dt);
	update_subentities(resources, dt, solid_tiles, destructible_tiles);
	update_health_info(get_sprite(), dt);

	if (is_alive() && shoot_timer.is_ready())
	{
		sf::Vector2f position = get_sprite().getPosition();
		position.x += (get_sprite_animation().horizontally_flipped ? -16.f : 16.f);
		spawn_subentity(resources, position, (get_sprite_animation().horizontally_flipped ? Bullet::DIRECTION::LEFT : Bullet::DIRECTION::RIGHT),
			Bullet::default_speed * 0.35f, Bullet::default_max_distance * 1.5f);
		restart_sprite_animation();
	}

	player_interaction(resources, player, get_hitbox(), is_alive(), is_alive());
}

void Crimson_Lasher::draw(sf::RenderTexture& rt) const
{
	draw_subentities(rt);
	draw_health_info(rt);
	this->LO_Base::draw(rt);
}


bool Crimson_Lasher::available_for_level_theme(LEVEL_THEME theme_id)
{
	return theme_id == LEVEL_THEME::LIFEFORCE;
}
void Crimson_Lasher::update_timer(const sf::Time& dt)
{
	shoot_timer.update(dt);
}

Colliding_Entity* Crimson_Lasher::make_lo(LEVEL_THEME theme_id)
{
	return new Crimson_Lasher(theme_id);
}

// ----------------------------------------------------------------
// Icebaner
// ----------------------------------------------------------------

Icebaner::Icebaner(LEVEL_THEME theme_id) :
	Bullet_Manager_Component(Bullet::TYPE::ICE_MASS, false),
	Enemy_Component(health_stat, damage_stat, colliding_entity_value(COLLIDING_ENTITY_SUBTYPE::ICEBANER))
{
	if (!available_for_level_theme(theme_id))
		throw std::runtime_error("Icebaner::Icebaner: invalid level theme");
}

void Icebaner::set_death_graphics(Resources& resources)
{
	set_sprite_texture(resources.textures.get(RESOURCE_ID::TEXTURES::EXPLOSION_48));
	set_sprite_animation(get_animation(AID_MISC::EXPLOSION_48));
}

void Icebaner::extra_player_interaction(Resources& resources, Player& player, const sf::FloatRect& my_hitbox)
{
	if (player.get_position().x < get_sprite().getPosition().x)
		get_sprite_animation().horizontally_flipped = true;
	else if (player.get_position().x > get_sprite().getPosition().x)
		get_sprite_animation().horizontally_flipped = false;

	for (const auto& bullet : get_subentities())
		if (bullet->is_active() && player.is_alive() && collision(bullet->get_hitbox(), player.get_hitbox()))
		{
			deal_damage(player);
			bullet->set_hit(resources);
		}
}

void Icebaner::extra_setup(std::istream& stream, Resources& resources)
{
	setup_health_info(resources);
	set_sprite_texture(resources.textures.get(RESOURCE_ID::TEXTURES::ICEBANER));
	set_sprite_animation(get_animation(AID_ENTITY::ICEBANER));
	center_sprite();
}

void Icebaner::extra_pre_record(std::ostream& stream) const
{
	data_writer(stream, static_cast<int32_t>(LO_TYPE::COLLIDING_ENTITY));
	data_writer(stream, static_cast<int32_t>(COLLIDING_ENTITY_SUBTYPE::ICEBANER));
}

sf::FloatRect Icebaner::get_hitbox()
{
	return ::get_hitbox(get_sprite(), 0.8f, 0.8f);
}

void Icebaner::update(Resources& resources, Player& player, const sf::Time& dt,
	const hct::dynarray<std::unique_ptr<Solid_Tile>>& solid_tiles,
	const hct::dynarray<std::unique_ptr<Destructible_Tile>>& destructible_tiles)
{
	update_frame(dt);
	update_subentities(resources, dt, solid_tiles, destructible_tiles);
	update_health_info(get_sprite(), dt);

	if (is_alive())
	{
		if (remaining_bullets > 0)
		{
			shoot_timer.update(dt);
			if (shoot_timer.is_ready())
			{
				--remaining_bullets;
				sf::Vector2f position = get_sprite().getPosition();
				position.x += (get_sprite_animation().horizontally_flipped ? -24.f : 24.f);
				spawn_subentity(resources, position, (get_sprite_animation().horizontally_flipped ? Bullet::DIRECTION::LEFT : Bullet::DIRECTION::RIGHT),
					Bullet::default_speed * 0.4f, Bullet::default_max_distance * 3.f);
				restart_sprite_animation();
			}
		}
		else
		{
			shoot_break.update(dt);
			if (shoot_break.is_done())
			{
				shoot_break.restart();
				remaining_bullets = bullets_per_assault;
			}
		}
	}

	player_interaction(resources, player, get_hitbox(), is_alive(), is_alive());
}

void Icebaner::draw(sf::RenderTexture& rt) const
{
	draw_subentities(rt);
	draw_health_info(rt);
	this->LO_Base::draw(rt);
}

bool Icebaner::available_for_level_theme(LEVEL_THEME theme_id)
{
	return theme_id == LEVEL_THEME::ICE;
}

Colliding_Entity* Icebaner::make_lo(LEVEL_THEME theme_id)
{
	return new Icebaner(theme_id);
}

// ----------------------------------------------------------------
// Roaming_Trampoline
// ----------------------------------------------------------------

Roaming_Trampoline::Roaming_Trampoline(LEVEL_THEME theme_id)
{
	if (!available_for_level_theme(theme_id))
		throw std::runtime_error("Roaming_Trampoline::Roaming_Trampoline: invalid level theme");

	auto animation = get_animation(AID_ENTITY::ROAMING_TRAMPOLINE);
	switch (theme_id)
	{
	case (LEVEL_THEME::ELECTRIC): animation.row = 0; break;
	case (LEVEL_THEME::WIND): animation.row = 1; break;
	}
	set_sprite_animation(animation);
}

void Roaming_Trampoline::extra_setup(std::istream& stream, Resources& resources)
{
	set_sprite_texture(resources.textures.get(RESOURCE_ID::TEXTURES::ROAMING_TRAMPOLINE));
}

void Roaming_Trampoline::extra_pre_record(std::ostream& stream) const
{
	data_writer(stream, static_cast<int32_t>(LO_TYPE::WALKING_ENTITY));
	data_writer(stream, static_cast<int32_t>(WALKING_ENTITY_SUBTYPE::ROAMING_TRAMPOLINE));
}

sf::FloatRect Roaming_Trampoline::get_hitbox() const
{
	return ::get_hitbox(get_sprite(), 0.7f, 0.3f);
}

void Roaming_Trampoline::update(Resources& resources, Player& player, const sf::Time& dt)
{
	update_frame(dt);
	update_walking(dt, speed_stat);

	if (collision(get_hitbox(), player.get_hitbox()))
	{
		player.jump(Player::default_jump_gravity * 2.5f, 0.f);
		restart_sprite_animation();
	}
}

bool Roaming_Trampoline::available_for_level_theme(LEVEL_THEME theme_id)
{
	return 
		theme_id == LEVEL_THEME::ELECTRIC ||
		theme_id == LEVEL_THEME::WIND;
}

Walking_Entity* Roaming_Trampoline::make_lo(LEVEL_THEME theme_id)
{
	return new Roaming_Trampoline(theme_id);
}

// ----------------------------------------------------------------
// Scouter
// ----------------------------------------------------------------

Scouter::Scouter(LEVEL_THEME theme_id) :
	Damage_Component(damage_stat)
{
	if (!available_for_level_theme(theme_id))
		throw std::runtime_error("Scouter::Scouter: invalid level theme");

	auto animation = get_animation(AID_ENTITY::SCOUTER);
	switch (theme_id)
	{
	case (LEVEL_THEME::ELECTRIC): animation.row = 0; break;
	case (LEVEL_THEME::WIND): animation.row = 1; break;
	case (LEVEL_THEME::GROUND): animation.row = 2; break;
	case (LEVEL_THEME::LIFEFORCE): animation.row = 3; break;
	}
	set_sprite_animation(animation);
}

void Scouter::extra_setup(std::istream& stream, Resources& resources)
{
	set_sprite_texture(resources.textures.get(RESOURCE_ID::TEXTURES::SCOUTER));
}

void Scouter::extra_pre_record(std::ostream& stream) const
{
	data_writer(stream, static_cast<int32_t>(LO_TYPE::WALKING_ENTITY));
	data_writer(stream, static_cast<int32_t>(WALKING_ENTITY_SUBTYPE::SCOUTER));
}

sf::FloatRect Scouter::get_hitbox()
{
	return ::get_hitbox(get_sprite(), 0.7f, 0.9f);
}

void Scouter::update(Resources& resources, Player& player, const sf::Time& dt)
{
	update_frame(dt);
	update_walking(dt, speed_stat);

	if (collision(get_hitbox(), player.get_hitbox()))
		deal_damage(player);
}

bool Scouter::available_for_level_theme(LEVEL_THEME theme_id)
{
	return 
		theme_id == LEVEL_THEME::ELECTRIC ||
		theme_id == LEVEL_THEME::WIND ||
		theme_id == LEVEL_THEME::GROUND ||
		theme_id == LEVEL_THEME::LIFEFORCE;
}

Walking_Entity* Scouter::make_lo(LEVEL_THEME theme_id)
{
	return new Scouter(theme_id);
}

// ----------------------------------------------------------------
// Warrior
// ----------------------------------------------------------------

Warrior::Warrior(LEVEL_THEME theme_id) :
	Enemy_Component(health_stat, damage_stat, walking_entity_value(WALKING_ENTITY_SUBTYPE::WARRIOR))
{
	auto animation = get_animation(AID_ENTITY::WARRIORS);
	animation.row = static_cast<int>(theme_id) - 1;
	if (animation.row < 0 || theme_id >= LEVEL_THEME::COUNT)
		throw std::runtime_error("Warrior_Base::Warrior_Base: invalid theme_id provided");
	set_sprite_animation(animation);
}

void Warrior::set_death_graphics(Resources& resources)
{
	set_sprite_texture(resources.textures.get(RESOURCE_ID::TEXTURES::EXPLOSION_48));
	set_sprite_animation(get_animation(AID_MISC::EXPLOSION_48));
}

void Warrior::extra_setup(std::istream& stream, Resources& resources)
{
	setup_health_info(resources);
	set_sprite_texture(resources.textures.get(RESOURCE_ID::TEXTURES::WARRIOR));
}

void Warrior::extra_pre_record(std::ostream& stream) const
{
	data_writer(stream, static_cast<int32_t>(LO_TYPE::WALKING_ENTITY));
	data_writer(stream, static_cast<int32_t>(WALKING_ENTITY_SUBTYPE::WARRIOR));
}

sf::FloatRect Warrior::get_hitbox()
{
	return ::get_hitbox(get_sprite(), 0.8f, 0.8f);
}

void Warrior::update(Resources& resources, Player& player, const sf::Time& dt)
{
	update_frame(dt);

	if (is_alive())
	{
		update_walking(dt, speed_stat);
		player_interaction(resources, player, get_hitbox(), true, true);
	}

	update_health_info(get_sprite(), dt);
}

void Warrior::draw(sf::RenderTexture& rt) const
{
	draw_health_info(rt);
	this->LO_Base::draw(rt);
}

Walking_Entity* Warrior::make_lo(LEVEL_THEME theme_id)
{
	return new Warrior(theme_id);
}

// ----------------------------------------------------------------
// Golem
// ----------------------------------------------------------------

Golem::Golem(LEVEL_THEME theme_id) :
	Enemy_Component(health_stat, damage_stat, walking_entity_value(WALKING_ENTITY_SUBTYPE::GOLEM))
{
	auto animation = get_animation(AID_ENTITY::GOLEMS);
	animation.row = static_cast<int>(theme_id) - 1;
	if (animation.row < 0 || theme_id >= LEVEL_THEME::COUNT)
		throw std::runtime_error("Golem::Golem: invalid theme_id provided");
	set_sprite_animation(animation);
}

void Golem::set_death_graphics(Resources& resources)
{
	set_sprite_texture(resources.textures.get(RESOURCE_ID::TEXTURES::EXPLOSION_48));
	set_sprite_animation(get_animation(AID_MISC::EXPLOSION_48));
}

void Golem::extra_setup(std::istream& stream, Resources& resources)
{
	setup_health_info(resources);
	set_sprite_texture(resources.textures.get(RESOURCE_ID::TEXTURES::GOLEM));
}

void Golem::extra_pre_record(std::ostream& stream) const
{
	data_writer(stream, static_cast<int32_t>(LO_TYPE::WALKING_ENTITY));
	data_writer(stream, static_cast<int32_t>(WALKING_ENTITY_SUBTYPE::GOLEM));
}

sf::FloatRect Golem::get_hitbox()
{
	return ::get_hitbox(get_sprite(), 0.7f, 0.8f);
}

void Golem::update(Resources& resources, Player& player, const sf::Time& dt)
{
	update_frame(dt);
	regeneration_timer.update(dt);

	if (is_alive())
	{
		update_walking(dt, speed_stat);
		player_interaction(resources, player, get_hitbox(), true, true);

		if (is_alive())
		{
			if (regeneration_timer.is_ready())
				receive_heal(regeneration_stat);
			if (get_health() == get_max_health())
				regeneration_timer.restart();
		}
	}

	update_health_info(get_sprite(), dt);
}

void Golem::draw(sf::RenderTexture& rt) const
{
	draw_health_info(rt);
	this->LO_Base::draw(rt);
}

Walking_Entity* Golem::make_lo(LEVEL_THEME theme_id)
{
	return new Golem(theme_id);
}

// ----------------------------------------------------------------
// Scorcher
// ----------------------------------------------------------------

Scorcher::Scorcher(LEVEL_THEME theme_id) :
	Enemy_Component(health_stat, damage_stat, walking_entity_value(WALKING_ENTITY_SUBTYPE::SCORCHER))
{
	if (!available_for_level_theme(theme_id))
		throw std::runtime_error("Scorcher::Scorcher: invalid level theme");
}

void Scorcher::set_death_graphics(Resources& resources)
{
	set_sprite_texture(resources.textures.get(RESOURCE_ID::TEXTURES::SCORCHER_AFTERMATH));
	set_sprite_animation(get_animation(AID_ENTITY::SCORCHER_AFTERMATH_1));
}

void Scorcher::extra_setup(std::istream& stream, Resources& resources)
{
	setup_health_info(resources);
	set_sprite_texture(resources.textures.get(RESOURCE_ID::TEXTURES::SCORCHER));
	set_sprite_animation(get_animation(AID_ENTITY::SCORCHER));
}

void Scorcher::extra_pre_record(std::ostream& stream) const
{
	data_writer(stream, static_cast<int32_t>(LO_TYPE::WALKING_ENTITY));
	data_writer(stream, static_cast<int32_t>(WALKING_ENTITY_SUBTYPE::SCORCHER));
}

sf::FloatRect Scorcher::get_hitbox()
{
	return ::get_hitbox(get_sprite(), 0.7f, 0.9f);
}

void Scorcher::update(Resources& resources, Player& player, const sf::Time& dt)
{
	update_frame(dt);

	if (is_alive())
		update_walking(dt, speed_stat);

	player_interaction(resources, player, get_hitbox(), true, is_alive());

	if (get_sprite().finished())
		set_sprite_animation(get_animation(AID_ENTITY::SCORCHER_AFTERMATH_2));

	update_health_info(get_sprite(), dt);
}

void Scorcher::draw(sf::RenderTexture& rt) const
{
	draw_health_info(rt);
	this->LO_Base::draw(rt);
}

bool Scorcher::available_for_level_theme(LEVEL_THEME theme_id)
{
	return theme_id == LEVEL_THEME::FIRE;
}

Walking_Entity* Scorcher::make_lo(LEVEL_THEME theme_id)
{
	return new Scorcher(theme_id);
}