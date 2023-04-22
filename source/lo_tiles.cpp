#include "lo_tiles.h"

#include "level_object_id.h"
#include "player.h"

// ----------------------------------------------------------------
// Functions
// ----------------------------------------------------------------

RESOURCE_ID::TEXTURES level_theme_to_tileset_id(LEVEL_THEME id)
{
	switch (id)
	{
	case (LEVEL_THEME::VERDANT): return RESOURCE_ID::TEXTURES::VERDANT_TILESET;
	case (LEVEL_THEME::WATER): return RESOURCE_ID::TEXTURES::WATER_TILESET;
	case (LEVEL_THEME::FIRE): return RESOURCE_ID::TEXTURES::FIRE_TILESET;
	case (LEVEL_THEME::ELECTRIC): return RESOURCE_ID::TEXTURES::ELECTRIC_TILESET;
	case (LEVEL_THEME::WIND): return RESOURCE_ID::TEXTURES::WIND_TILESET;
	case (LEVEL_THEME::GROUND): return RESOURCE_ID::TEXTURES::GROUND_TILESET;
	case (LEVEL_THEME::LIFEFORCE): return RESOURCE_ID::TEXTURES::LIFEFORCE_TILESET;
	case (LEVEL_THEME::ICE): return RESOURCE_ID::TEXTURES::ICE_TILESET;
	default: throw std::runtime_error("level_theme_to_tileset_id(): invalid id provided");
	}
}

int max_index_by_tileset_id(LEVEL_THEME id)
{
	switch (id)
	{
	case (LEVEL_THEME::VERDANT): return 137;
	case (LEVEL_THEME::WATER): return 0;
	case (LEVEL_THEME::FIRE): return 0;
	case (LEVEL_THEME::ELECTRIC): return 0;
	case (LEVEL_THEME::WIND): return 0;
	case (LEVEL_THEME::GROUND): return 0;
	case (LEVEL_THEME::LIFEFORCE): return 115;
	case (LEVEL_THEME::ICE): return 153;
	}
	return 0;
}

bool operator<(const Decorative_Tile& lhs, const Decorative_Tile& rhs) noexcept
{
	if (lhs.get_sprite().getPosition().x == rhs.get_sprite().getPosition().x &&
		lhs.get_sprite().getPosition().y == rhs.get_sprite().getPosition().y)
		return lhs.get_layer() < rhs.get_layer();
	const LO_Base<sf::Sprite>& casted_lhs = lhs;
	const LO_Base<sf::Sprite>& casted_rhs = rhs;
	return casted_lhs < casted_rhs;
}

bool operator<(const Animated_Tile& lhs, const Animated_Tile& rhs) noexcept
{
	if (lhs.get_sprite().getPosition().x == rhs.get_sprite().getPosition().x &&
		lhs.get_sprite().getPosition().y == rhs.get_sprite().getPosition().y)
		return lhs.get_layer() < rhs.get_layer();
	const LO_Base<Animatable_Sprite>& casted_lhs = lhs;
	const LO_Base<Animatable_Sprite>& casted_rhs = rhs;
	return casted_lhs < casted_rhs;
}

// ----------------------------------------------------------------
// Tile_Layer_Component
// ----------------------------------------------------------------

void Tile_Layer_Component::read_layer(std::istream& stream)
{
	int32_t temp_layer;
	data_reader(stream, temp_layer);
	layer = static_cast<LAYER>(temp_layer);

	if (layer < static_cast<LAYER>(0) || layer >= LAYER::COUNT)
		throw std::runtime_error("Decorative_Tile::read_layer(): layer out of range");
}

Tile_Layer_Component::LAYER Tile_Layer_Component::get_layer() const noexcept
{
	return layer;
}

// ----------------------------------------------------------------
// Decorative_Tile
// ----------------------------------------------------------------

Decorative_Tile::Decorative_Tile(LEVEL_THEME theme_id, Resources& resources)
{
	set_sprite_texture(resources.textures.get(level_theme_to_tileset_id(theme_id)));
}

void Decorative_Tile::extra_setup(std::istream& stream, Resources& resources)
{
	read_layer(stream);
	data_reader(stream, index_x);
	data_reader(stream, index_y);

	set_sprite_rect({ 32 * index_x, 32 * index_y, 32, 32 });
}

Decorative_Tile* Decorative_Tile::make_lo(LEVEL_THEME theme_id, Resources& resources)
{
	return new Decorative_Tile(theme_id, resources);
}

void Decorative_Tile::extra_pre_record(std::ostream& stream) const
{
	data_writer(stream, static_cast<int32_t>(LO_TYPE::DECORATIVE_TILE));
	data_writer(stream, static_cast<int32_t>(DECORATIVE_TILE_SUBTYPE::REGULAR));
}

void Decorative_Tile::extra_post_record(std::ostream& stream) const
{
	data_writer(stream, static_cast<int32_t>(get_layer()));
	data_writer(stream, index_x);
	data_writer(stream, index_y);
}

// ----------------------------------------------------------------
// Animated_Tile
// ----------------------------------------------------------------

void Animated_Tile::extra_setup(std::istream& stream, Resources& resources)
{
	read_layer(stream);
	std::int32_t temp_animation_id;
	data_reader(stream, temp_animation_id);
	animation_id = static_cast<AID_TILE>(temp_animation_id);

	set_sprite_texture(resources.textures.get(RESOURCE_ID::TEXTURES::ANIMATED_TILESET));

	auto result = get_animation(static_cast<AID_TILE>(animation_id));
	set_sprite_animation(result.first);
	set_sprite_color({ 255, 255, 255, result.second });;
}

void Animated_Tile::extra_pre_record(std::ostream& stream) const
{
	data_writer(stream, static_cast<int32_t>(LO_TYPE::ANIMATED_TILE));
	data_writer(stream, static_cast<int32_t>(ANIMATED_TILE_SUBTYPE::REGULAR));
}

void Animated_Tile::extra_post_record(std::ostream& stream) const
{
	data_writer(stream, static_cast<int32_t>(get_layer()));
	data_writer(stream, static_cast<int32_t>(animation_id));
}

Animated_Tile* Animated_Tile::make_lo()
{
	return new Animated_Tile;
}

// ----------------------------------------------------------------
// Solid_Tile
// ----------------------------------------------------------------

Solid_Tile::Solid_Tile(LEVEL_THEME theme_id, Resources& resources)
{
	set_sprite_texture(resources.textures.get(level_theme_to_tileset_id(theme_id)));
}

void Solid_Tile::extra_setup(std::istream& stream, Resources& resources)
{
	data_reader(stream, index_x);
	data_reader(stream, index_y);
	set_sprite_rect({ 32 * index_x, 32 * index_y, 32, 32 });
}

void Solid_Tile::extra_pre_record(std::ostream& stream) const
{
	data_writer(stream, static_cast<int32_t>(LO_TYPE::SOLID_TILE));
	data_writer(stream, static_cast<int32_t>(SOLID_TILE_SUBTYPE::REGULAR));
}

void Solid_Tile::extra_post_record(std::ostream& stream) const
{
	data_writer(stream, index_x);
	data_writer(stream, index_y);
}

sf::FloatRect Solid_Tile::get_hitbox() const noexcept
{
	return ::get_hitbox(get_sprite(), 0.85f, 1.f);
}

Solid_Tile* Solid_Tile::make_lo(LEVEL_THEME theme_id, Resources& resources)
{
	return new Solid_Tile(theme_id, resources);
}

// ----------------------------------------------------------------
// Destructible_Tile
// ----------------------------------------------------------------

Destructible_Tile::Destructible_Tile(LEVEL_THEME theme_id, Resources& resources)
{
	set_sprite_texture(resources.textures.get(level_theme_to_tileset_id(theme_id)));
}

void Destructible_Tile::extra_setup(std::istream& stream, Resources& resources)
{
	data_reader(stream, index_x);
	data_reader(stream, index_y);

	set_sprite_animation(Animation_Settings(index_y, index_x, 1, 32, 32, 0.f, false));
}

void Destructible_Tile::extra_pre_record(std::ostream& stream) const
{
	data_writer(stream, static_cast<int32_t>(LO_TYPE::DESTRUCTIBLE_TILE));
	data_writer(stream, static_cast<int32_t>(DESTRUCTIBLE_TILE_SUBTYPE::REGULAR));
}

void Destructible_Tile::extra_post_record(std::ostream& stream) const
{
	data_writer(stream, index_x);
	data_writer(stream, index_y);
}

void Destructible_Tile::extra_update(Resources& resources, const sf::Time& dt) {}

sf::FloatRect Destructible_Tile::get_hitbox() const noexcept
{
	return ::get_hitbox(get_sprite(), 0.85f, 1.f);
}

bool Destructible_Tile::is_broken() const noexcept
{
	return broken;
}

void Destructible_Tile::set_broken(Resources& resources)
{
	broken = true;
	set_sprite_texture(resources.textures.get(RESOURCE_ID::TEXTURES::EXPLOSION_32));
	set_sprite_animation(get_animation(AID_MISC::EXPLOSION_32));
}

void Destructible_Tile::update(Resources& resources, const sf::Time& dt)
{
	if (broken)
		update_frame(dt);
	extra_update(resources, dt);
}

Destructible_Tile* Destructible_Tile::make_lo(LEVEL_THEME theme_id, Resources& resources)
{
	return new Destructible_Tile(theme_id, resources);
}

// ----------------------------------------------------------------
// Fragile_Tile
// ----------------------------------------------------------------

Fragile_Tile::Fragile_Tile(LEVEL_THEME theme_id, Resources& resources) : Destructible_Tile(theme_id, resources) {}

void Fragile_Tile::extra_pre_record(std::ostream& stream) const
{
	data_writer(stream, static_cast<int32_t>(LO_TYPE::DESTRUCTIBLE_TILE));
	data_writer(stream, static_cast<int32_t>(DESTRUCTIBLE_TILE_SUBTYPE::FRAGILE));
}

void Fragile_Tile::extra_update(Resources& resources, const sf::Time& dt)
{
	if (countdown_started && !crack_countdown.is_done())
	{
		crack_countdown.update(dt);
		if (crack_countdown.is_done())
			set_broken(resources);
	}
}

void Fragile_Tile::start_crack_countdown() noexcept
{
	if (!countdown_started)
	{
		crack_countdown.restart();
		countdown_started = true;
	}
}

Destructible_Tile* Fragile_Tile::make_lo(LEVEL_THEME theme_id, Resources& resources)
{
	return new Fragile_Tile(theme_id, resources);
}

// ----------------------------------------------------------------
// Interactive_Tile
// ----------------------------------------------------------------

void Interactive_Tile::extra_interactive_setup(std::istream& stream) {}

void Interactive_Tile::extra_setup(std::istream& stream, Resources& resources)
{
	set_sprite_texture(resources.textures.get(RESOURCE_ID::TEXTURES::INTERACTIVE_TILESET));
	set_sprite_color({ 255, 255, 255, transparency_value });
	extra_interactive_setup(stream);
	set_desired_sprite_details();
}

void Interactive_Tile::extra_post_record(std::ostream& stream) const {}

// ----------------------------------------------------------------
// Hazardous_Tile
// ----------------------------------------------------------------

Hazardous_Tile::Hazardous_Tile() : Damage_Component(damage_stat) {}

void Hazardous_Tile::set_desired_sprite_details()
{
	set_sprite_rect({ 32 * 5, 0, 32, 32 });
}

void Hazardous_Tile::extra_pre_record(std::ostream& stream) const
{
	data_writer(stream, static_cast<int32_t>(LO_TYPE::INTERACTIVE_TILE));
	data_writer(stream, static_cast<int32_t>(INTERACTIVE_TILE_SUBTYPE::HAZARDOUS));
}

sf::FloatRect Hazardous_Tile::get_hitbox() const noexcept
{
	return ::get_hitbox(get_sprite(), 0.7f, 0.5f);
}

void Hazardous_Tile::player_interaction(Player& player)
{
	if (collision(get_hitbox(), player.get_hitbox()))
		deal_damage(player);
}

Interactive_Tile* Hazardous_Tile::make_lo()
{
	return new Hazardous_Tile;
}

// ----------------------------------------------------------------
// Ice_Tile
// ----------------------------------------------------------------

void Ice_Tile::set_desired_sprite_details()
{
	set_sprite_rect({ 32 * 9, 0, 32, 32 });
}

void Ice_Tile::extra_pre_record(std::ostream& stream) const
{
	data_writer(stream, static_cast<int32_t>(LO_TYPE::INTERACTIVE_TILE));
	data_writer(stream, static_cast<int32_t>(INTERACTIVE_TILE_SUBTYPE::ICE));
}

sf::FloatRect Ice_Tile::get_hitbox() const noexcept
{
	return ::get_hitbox(get_sprite(), 1.f, 0.5f);
}

void Ice_Tile::player_interaction(Player& player)
{
	if (collision(get_hitbox(), player.get_hitbox()))
		player.set_on_ice(true);
}

Interactive_Tile* Ice_Tile::make_lo() { return new Ice_Tile; }

// ----------------------------------------------------------------
// Hook_Tile
// ----------------------------------------------------------------

void Hook_Tile::set_desired_sprite_details()
{
	set_sprite_rect({ 32 * 4, 0, 32, 32 });
}

void Hook_Tile::extra_pre_record(std::ostream& stream) const
{
	data_writer(stream, static_cast<int32_t>(LO_TYPE::INTERACTIVE_TILE));
	data_writer(stream, static_cast<int32_t>(INTERACTIVE_TILE_SUBTYPE::HOOK));
}

sf::FloatRect Hook_Tile::get_hitbox() const noexcept
{
	auto res = ::get_hitbox(get_sprite(), 0.15f, 1.f);
	res.top += res.height * 0.8f;
	res.height = 8.f;
	return res;
}

void Hook_Tile::player_interaction(Player& player)
{
	if (player.is_descending() && collision(get_hitbox(), player.get_upper_hitbox()))
	{
		player.set_on_hook(true);
		player.set_position({ player.get_sprite().getPosition().x, get_original_position().y + 24.f + player.get_sprite().getGlobalBounds().height * 0.5f });
	}
}

Interactive_Tile* Hook_Tile::make_lo()
{
	return new Hook_Tile;
}

// ----------------------------------------------------------------
// Cold_Air_Tile
// ----------------------------------------------------------------

void Cold_Air_Tile::set_desired_sprite_details()
{
	set_sprite_rect({ 32 * 10, 0, 32, 32 });
}

void Cold_Air_Tile::extra_pre_record(std::ostream& stream) const
{
	data_writer(stream, static_cast<int32_t>(LO_TYPE::INTERACTIVE_TILE));
	data_writer(stream, static_cast<int32_t>(INTERACTIVE_TILE_SUBTYPE::COLD_AIR));
}

sf::FloatRect Cold_Air_Tile::get_hitbox() const noexcept
{
	return ::get_hitbox(get_sprite(), 0.8f, 0.8f);
}

void Cold_Air_Tile::player_interaction(Player& player)
{
	if (collision(get_hitbox(), player.get_hitbox()))
		player.set_in_cold_air(true);
}

Interactive_Tile* Cold_Air_Tile::make_lo()
{
	return new Cold_Air_Tile;
}

// ----------------------------------------------------------------
// Hot_Air_Tile
// ----------------------------------------------------------------

void Hot_Air_Tile::set_desired_sprite_details()
{
	set_sprite_rect({ 32 * 11, 0, 32, 32 });
}

void Hot_Air_Tile::extra_pre_record(std::ostream& stream) const
{
	data_writer(stream, static_cast<int32_t>(LO_TYPE::INTERACTIVE_TILE));
	data_writer(stream, static_cast<int32_t>(INTERACTIVE_TILE_SUBTYPE::HOT_AIR));
}

sf::FloatRect Hot_Air_Tile::get_hitbox() const noexcept
{
	return ::get_hitbox(get_sprite(), 0.8f, 0.8f);
}

void Hot_Air_Tile::player_interaction(Player& player)
{
	if (collision(get_hitbox(), player.get_hitbox()))
		player.set_in_hot_air(true);
}

Interactive_Tile* Hot_Air_Tile::make_lo()
{
	return new Hot_Air_Tile;
}

// ----------------------------------------------------------------
// Water_Tile
// ----------------------------------------------------------------

void Water_Tile::set_desired_sprite_details()
{
	set_sprite_rect({ 32 * 6, 0, 32, 32 });
}

void Water_Tile::extra_pre_record(std::ostream& stream) const
{
	data_writer(stream, static_cast<int32_t>(LO_TYPE::INTERACTIVE_TILE));
	data_writer(stream, static_cast<int32_t>(INTERACTIVE_TILE_SUBTYPE::WATER));
}

sf::FloatRect Water_Tile::get_hitbox() const noexcept
{
	return ::get_hitbox(get_sprite(), 0.7f, 0.7f);
}

void Water_Tile::player_interaction(Player& player)
{
	if (collision(get_hitbox(), player.get_hitbox()))
		player.set_in_water(true);
}

Interactive_Tile* Water_Tile::make_lo()
{
	return new Water_Tile;
}

// ----------------------------------------------------------------
// Conveyor_Belt_Tile
// ----------------------------------------------------------------

void Conveyor_Belt_Tile::set_desired_sprite_details()
{
	set_sprite_rect({ 32 * 7 + (moves_left ? 0 : 32), 0, 32, 32 });
}

void Conveyor_Belt_Tile::extra_interactive_setup(std::istream& stream)
{
	data_reader(stream, moves_left);
}

void Conveyor_Belt_Tile::extra_pre_record(std::ostream& stream) const
{
	data_writer(stream, static_cast<int32_t>(LO_TYPE::INTERACTIVE_TILE));
	data_writer(stream, static_cast<int32_t>(INTERACTIVE_TILE_SUBTYPE::CONVEYOR_BELT));
}

void Conveyor_Belt_Tile::extra_post_record(std::ostream& stream) const
{
	data_writer(stream, moves_left);
}

sf::FloatRect Conveyor_Belt_Tile::get_hitbox() const noexcept
{
	return ::get_hitbox(get_sprite(), 0.85f, 0.8f);
}

void Conveyor_Belt_Tile::player_interaction(Player& player)
{
	if (player.is_standing() && collision(get_hitbox(), player.get_hitbox()))
	{
		if (moves_left)
			player.set_forcefully_moved_left(true);
		else
			player.set_forcefully_moved_right(true);
	}
}

Interactive_Tile* Conveyor_Belt_Tile::make_lo()
{
	return new Conveyor_Belt_Tile;
}

// ----------------------------------------------------------------
// Info_Sign_Tile
// ----------------------------------------------------------------

void Info_Sign_Tile::set_desired_sprite_details()
{
	set_sprite_rect({ 32 * 3, 0, 32, 32 });
}

void Info_Sign_Tile::extra_interactive_setup(std::istream& stream)
{
	int32_t size = 0;
	string.reserve(size);
	char temp;

	data_reader(stream, size);
	for (int32_t a = 0; a < size; ++a)
	{
		data_reader(stream, temp);
		string.push_back(temp);
	}
}

void Info_Sign_Tile::extra_pre_record(std::ostream& stream) const
{
	data_writer(stream, static_cast<int32_t>(LO_TYPE::INTERACTIVE_TILE));
	data_writer(stream, static_cast<int32_t>(INTERACTIVE_TILE_SUBTYPE::INFO_SIGN));
}

void Info_Sign_Tile::extra_post_record(std::ostream& stream) const
{
	data_writer(stream, static_cast<int32_t>(string.size()));
	for (const auto& ch : string)
		data_writer(stream, static_cast<int8_t>(ch));
}

sf::FloatRect Info_Sign_Tile::get_hitbox() const noexcept
{
	return ::get_hitbox(get_sprite(), 0.8f, 0.8f);
}

void Info_Sign_Tile::player_interaction(Player& player)
{
	if (collision(get_hitbox(), player.get_hitbox()))
		player.set_visible_text(string);
}

Interactive_Tile* Info_Sign_Tile::make_lo()
{
	return new Info_Sign_Tile;
}

// ----------------------------------------------------------------
// Anti_Gravity_Tile
// ----------------------------------------------------------------

void Anti_Gravity_Tile::set_desired_sprite_details()
{
	set_sprite_rect({ 32 * 12, 0, 32, 32 });
}

void Anti_Gravity_Tile::extra_pre_record(std::ostream& stream) const
{
	data_writer(stream, static_cast<int32_t>(LO_TYPE::INTERACTIVE_TILE));
	data_writer(stream, static_cast<int32_t>(INTERACTIVE_TILE_SUBTYPE::ANTI_GRAVITY));
}

sf::FloatRect Anti_Gravity_Tile::get_hitbox() const noexcept
{
	return ::get_hitbox(get_sprite(), 0.5f, 0.5f);
}

void Anti_Gravity_Tile::player_interaction(Player& player)
{
	if (collision(get_hitbox(), player.get_hitbox()))
		player.set_in_enchanted_air(true);
}

Interactive_Tile* Anti_Gravity_Tile::make_lo()
{
	return new Anti_Gravity_Tile;
}

// ----------------------------------------------------------------
// Warp_Tile
// ----------------------------------------------------------------

void Warp_Tile::set_desired_sprite_details()
{
	set_sprite_rect({ 32 * (coins_required > 0 ? 1 : 0), 0, 32, 32 });
}

void Warp_Tile::extra_interactive_setup(std::istream& stream)
{
	int32_t pos_x, pos_y;
	data_reader(stream, pos_x);
	data_reader(stream, pos_y);
	data_reader(stream, coins_required);
	destination.x = static_cast<float>(pos_x);
	destination.y = static_cast<float>(pos_y);
}

void Warp_Tile::extra_pre_record(std::ostream& stream) const
{
	data_writer(stream, static_cast<int32_t>(LO_TYPE::INTERACTIVE_TILE));
	data_writer(stream, static_cast<int32_t>(INTERACTIVE_TILE_SUBTYPE::WARP));
}

void Warp_Tile::extra_post_record(std::ostream& stream) const
{
	data_writer(stream, static_cast<int32_t>(destination.x));
	data_writer(stream, static_cast<int32_t>(destination.y));
	data_writer(stream, coins_required);
}

sf::FloatRect Warp_Tile::get_hitbox() const noexcept
{
	return ::get_hitbox(get_sprite(), 0.6f, 0.6f);
}

void Warp_Tile::player_interaction(Player& player)
{
	if (static_cast<unsigned int>(player.get_coins()) >= coins_required && collision(get_hitbox(), player.get_hitbox()))
	{
		player.add_coins(-static_cast<int>(coins_required));
		player.initialize_teleport(destination);
	}
}

Interactive_Tile* Warp_Tile::make_lo()
{
	return new Warp_Tile;
}

// ----------------------------------------------------------------
// Finish_Warp_Tile
// ----------------------------------------------------------------

void Finish_Warp_Tile::set_desired_sprite_details()
{
	set_sprite_rect({ 32 * 2, 0, 32, 32 });
}

void Finish_Warp_Tile::extra_pre_record(std::ostream& stream) const
{
	data_writer(stream, static_cast<int32_t>(LO_TYPE::INTERACTIVE_TILE));
	data_writer(stream, static_cast<int32_t>(INTERACTIVE_TILE_SUBTYPE::FINISH_WARP));
}

sf::FloatRect Finish_Warp_Tile::get_hitbox() const noexcept
{
	return ::get_hitbox(get_sprite(), 0.6f, 0.6f);
}

void Finish_Warp_Tile::player_interaction(Player& player)
{
	if (collision(get_hitbox(), player.get_hitbox()))
	{
		player.initialize_teleport({ 0, 0 });
		player.set_victorious(true);
	}
}

Interactive_Tile* Finish_Warp_Tile::make_lo()
{
	return new Finish_Warp_Tile;
}