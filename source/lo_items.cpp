#include "lo_items.h"

#include "animations.h"
#include "level_object_id.h"

// ----------------------------------------------------------------
// Item
// ----------------------------------------------------------------

sf::FloatRect Item::get_hitbox() const noexcept
{
	return ::get_hitbox(get_sprite(), 0.7f, 0.7f);
}

void Item::set_value(int a) noexcept
{
	this->Value_Component::set_value(a);
}

void Item::update(Resources& resources, const sf::Time& dt)
{
	update_frame(dt);

	if (collected && !is_harvested())
	{
		set_harvested(true);
		set_sprite_texture(resources.textures.get(RESOURCE_ID::TEXTURES::ITEM_FADEOUT));
		set_sprite_animation(get_animation(AID_MISC::ITEM_FADEOUT));
	}

	if (is_harvested())
		return;

	move(0.f, speed_y * dt.asSeconds() * (ascending ? 1.0f : -1.0f));

	if (ascending && get_sprite().getPosition().y >= get_original_position().y + y_offset_cap)
		ascending = false;
	else if (!ascending && get_sprite().getPosition().y <= get_original_position().y - y_offset_cap)
		ascending = true;
}

void Item::player_interaction(Player& player)
{
	if (!collected && collision(get_hitbox(), player.get_hitbox()))
	{
		award_player(player);
		collected = true;
	}
}

// ----------------------------------------------------------------
// Food
// ----------------------------------------------------------------

void Food::set_desired_sprite(Resources& resources)
{
	set_sprite_texture(resources.textures.get(RESOURCE_ID::TEXTURES::FOOD));
	auto animation = get_animation(AID_ITEM::FOOD);
	animation.start_frame = index_x;
	animation.row = index_y;
	set_sprite_animation(animation);
}

void Food::extra_setup(std::istream& stream, Resources& resources)
{
	data_reader(stream, index_x);
	data_reader(stream, index_y);

	if (index_x < 0 || index_x > 9)
		throw std::runtime_error("Food::extra_setup: index_x out of range");

	set_value(food_value_by_index(index_y));
	if (get_value() == 0)
		throw std::runtime_error("Food::extra_setup: index_y out of range");

	set_desired_sprite(resources);
}

void Food::extra_pre_record(std::ostream& stream) const
{
	data_writer(stream, static_cast<int32_t>(LO_TYPE::ITEM));
	data_writer(stream, static_cast<int32_t>(ITEM_SUBTYPE::FOOD));
}

void Food::extra_post_record(std::ostream& stream) const
{
	data_writer(stream, index_x);
	data_writer(stream, index_y);
}

void Food::award_player(Player& player)
{
	player.add_points(get_value());
}

Item* Food::make_lo()
{
	return new Food;
}

// ----------------------------------------------------------------
// Gem
// ----------------------------------------------------------------

void Gem::set_desired_sprite(Resources& resources)
{
	set_sprite_texture(resources.textures.get(RESOURCE_ID::TEXTURES::GEM));
	auto animation = get_animation(AID_ITEM::GEM);
	animation.row = index;
	set_sprite_animation(animation);
}

void Gem::extra_setup(std::istream& stream, Resources& resources)
{
	data_reader(stream, index);

	set_value(gem_value_by_index(index));
	if (get_value() == 0)
		throw std::runtime_error("Gem::extra_setup: index out of range");

	set_desired_sprite(resources);
}

void Gem::extra_pre_record(std::ostream& stream) const
{
	data_writer(stream, static_cast<int32_t>(LO_TYPE::ITEM));
	data_writer(stream, static_cast<int32_t>(ITEM_SUBTYPE::GEM));
}

void Gem::extra_post_record(std::ostream& stream) const
{
	data_writer(stream, index);
}

void Gem::award_player(Player& player)
{
	player.add_points(get_value());
}

Item* Gem::make_lo()
{
	return new Gem;
}

// ----------------------------------------------------------------
// Coin
// ---------------------------------------------------------------- 

void Coin::set_desired_sprite(Resources& resources)
{
	set_sprite_texture(resources.textures.get(RESOURCE_ID::TEXTURES::COIN));
	auto animation = get_animation(AID_ITEM::COIN);
	animation.row = index;
	set_sprite_animation(animation);
}

void Coin::extra_setup(std::istream& stream, Resources& resources)
{
	data_reader(stream, index);

	set_value(coin_value_by_index(index));
	if (get_value() == 0)
		throw std::runtime_error("Coin::extra_setup: index out of range");

	set_desired_sprite(resources);
}

void Coin::extra_pre_record(std::ostream& stream) const
{
	data_writer(stream, static_cast<int32_t>(LO_TYPE::ITEM));
	data_writer(stream, static_cast<int32_t>(ITEM_SUBTYPE::COIN));
}

void Coin::extra_post_record(std::ostream& stream) const
{
	data_writer(stream, index);
}

void Coin::award_player(Player& player)
{
	player.add_coins(get_value());
}

Item* Coin::make_lo()
{
	return new Coin;
}

// ----------------------------------------------------------------
// Lifeup
// ---------------------------------------------------------------- 

void Lifeup::set_desired_sprite(Resources& resources)
{
	set_sprite_texture(resources.textures.get(RESOURCE_ID::TEXTURES::LIFEUP));
	auto animation = get_animation(AID_ITEM::LIFEUP);
	animation.row = index;
	set_sprite_animation(animation);
}

void Lifeup::extra_setup(std::istream& stream, Resources& resources)
{
	data_reader(stream, index);

	set_value(lifeup_value_by_index(index));
	if (get_value() == 0)
		throw std::runtime_error("Lifeup::extra_setup: index out of range");

	set_desired_sprite(resources);
}

void Lifeup::extra_pre_record(std::ostream& stream) const
{
	data_writer(stream, static_cast<int32_t>(LO_TYPE::ITEM));
	data_writer(stream, static_cast<int32_t>(ITEM_SUBTYPE::LIFEUP));
}

void Lifeup::extra_post_record(std::ostream& stream) const
{
	data_writer(stream, index);
}

void Lifeup::award_player(Player& player)
{
	player.receive_heal(get_value());
}

Item* Lifeup::make_lo()
{
	return new Lifeup;
}