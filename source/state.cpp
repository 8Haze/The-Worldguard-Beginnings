#include "state.h"

#include "app.h"
#include "resources.h"

// ----------------------------------------------------------------
// State
// ----------------------------------------------------------------

State::State(App& arg_app_handle, App_Shared_Context& arg_context) :
	app_handle(arg_app_handle), context(arg_context) {}

// ----------------------------------------------------------------
// State_LCOS_Base
// ----------------------------------------------------------------

State_LCOS_Base::State_LCOS_Base(App& arg_app_handle) :
	State(arg_app_handle, arg_app_handle.get_shared_context())
{
	auto& resources = app_handle.get_shared_context().resources;

	label = make_label(context.resources.fonts.get(RESOURCE_ID::FONTS::MAIN), "", { 0, 0 }, 80);
	info = make_label(context.resources.fonts.get(RESOURCE_ID::FONTS::MAIN), "", { 0, 0 }, 32);

	adjust_to_resolution();
}

void State_LCOS_Base::render()
{
	context.window.draw(label);
	context.window.draw(info);
	context.window.draw(options);
}

void State_LCOS_Base::adjust_to_resolution()
{
	label.setPosition(sf::Vector2f(context.view.getSize().x * 0.5f, context.view.getSize().y * 0.1f));
	info.setPosition(sf::Vector2f(context.view.getSize().x * 0.5f, context.view.getSize().y * 0.9f));
	options.setPosition(sf::Vector2f(context.view.getSize().x * 0.5f, context.view.getSize().y * 0.5f));
}

constexpr bool State_LCOS_Base::request_previous_render()
{
	return false;
}

constexpr bool State_LCOS_Base::uses_menu_background()
{
	return true;
}

constexpr bool State_LCOS_Base::uses_dark_sheet()
{
	return false;
}

// ----------------------------------------------------------------
// SCPP_Error
// ----------------------------------------------------------------

SCPP_Error::SCPP_Error(const std::string& arg_str) :
	str(arg_str) {}

// ----------------------------------------------------------------
// SCPP_LCOS_Base
// ----------------------------------------------------------------

SCPP_LCOS_Base::SCPP_LCOS_Base(LO_TYPE arg_lo_type) :
	lo_type(arg_lo_type)
{
	info[0] = "Object type: ";
}

void SCPP_LCOS_Base::record_extra_data(std::ostream& stream) {}

// ----------------------------------------------------------------
// SCPP_LCOS_Decorative_Tile
// ----------------------------------------------------------------

SCPP_LCOS_Decorative_Tile::SCPP_LCOS_Decorative_Tile(int32_t arg_index_x, int32_t arg_index_y, int32_t arg_layer) :
	SCPP_LCOS_Base(LO_TYPE::DECORATIVE_TILE), index_x(arg_index_x), index_y(arg_index_y), layer(arg_layer)
{
	info[0] += "Decorative Tile";
	info[1] = make_string("Index X: ", std::to_string(index_x));
	info[2] = make_string("Index Y: ", std::to_string(index_y));
	info[3] = make_string("Layer: ", std::to_string(layer));
}

void SCPP_LCOS_Decorative_Tile::record_lo_subtype(std::ostream& stream)
{
	data_writer(stream, static_cast<int32_t>(DECORATIVE_TILE_SUBTYPE::REGULAR));
}

void SCPP_LCOS_Decorative_Tile::record_extra_data(std::ostream& stream)
{
	data_writer(stream, layer);
	data_writer(stream, index_x);
	data_writer(stream, index_y);
}

// ----------------------------------------------------------------
// SCPP_LCOS_Animated_Tile
// ----------------------------------------------------------------

SCPP_LCOS_Animated_Tile::SCPP_LCOS_Animated_Tile(int32_t arg_index, int32_t arg_layer) :
	SCPP_LCOS_Base(LO_TYPE::ANIMATED_TILE), index(arg_index), layer(arg_layer)
{
	info[0] += "Animated Tile";
	info[1] = make_string("Index: ", std::to_string(index));
	info[2] = make_string("Layer: ", std::to_string(layer));
}

void SCPP_LCOS_Animated_Tile::record_lo_subtype(std::ostream& stream)
{
	data_writer(stream, static_cast<int32_t>(ANIMATED_TILE_SUBTYPE::REGULAR));
}

void SCPP_LCOS_Animated_Tile::record_extra_data(std::ostream& stream)
{
	data_writer(stream, layer);
	data_writer(stream, index);
}

// ----------------------------------------------------------------
// SCPP_LCOS_Solid_Tile
// ----------------------------------------------------------------

SCPP_LCOS_Solid_Tile::SCPP_LCOS_Solid_Tile(int32_t arg_index_x, int32_t arg_index_y) :
	SCPP_LCOS_Base(LO_TYPE::SOLID_TILE), index_x(arg_index_x), index_y(arg_index_y)
{
	info[0] += "Solid Tile";
	info[1] = make_string("Index X: ", std::to_string(index_x));
	info[2] = make_string("Index Y: ", std::to_string(index_y));
}

void SCPP_LCOS_Solid_Tile::record_lo_subtype(std::ostream& stream)
{
	data_writer(stream, static_cast<int32_t>(SOLID_TILE_SUBTYPE::REGULAR));
}

void SCPP_LCOS_Solid_Tile::record_extra_data(std::ostream& stream)
{
	data_writer(stream, index_x);
	data_writer(stream, index_y);
}

// ----------------------------------------------------------------
// SCPP_LCOS_Destructible_Tile
// ----------------------------------------------------------------

SCPP_LCOS_Destructible_Tile::SCPP_LCOS_Destructible_Tile(int32_t arg_index_x, int32_t arg_index_y, DESTRUCTIBLE_TILE_SUBTYPE arg_lo_subtype) :
	SCPP_LCOS_Base(LO_TYPE::DESTRUCTIBLE_TILE), index_x(arg_index_x), index_y(arg_index_y), lo_subtype(arg_lo_subtype)
{
	info[0] += "Destructible Tile";
	info[1] = make_string("Index X: ", std::to_string(index_x));
	info[2] = make_string("Index Y: ", std::to_string(index_y));
	info[3] = make_string("Fragile: ", (lo_subtype == DESTRUCTIBLE_TILE_SUBTYPE::FRAGILE ? "Yes" : "No"));
}

void SCPP_LCOS_Destructible_Tile::record_lo_subtype(std::ostream& stream)
{
	data_writer(stream, static_cast<int32_t>(lo_subtype));
}

void SCPP_LCOS_Destructible_Tile::record_extra_data(std::ostream& stream)
{
	data_writer(stream, index_x);
	data_writer(stream, index_y);
}

// ----------------------------------------------------------------
// SCPP_LCOS_Interactive_Tile
// ----------------------------------------------------------------

SCPP_LCOS_Interactive_Tile::SCPP_LCOS_Interactive_Tile(INTERACTIVE_TILE_SUBTYPE arg_lo_subtype) :
	SCPP_LCOS_Base(LO_TYPE::INTERACTIVE_TILE), lo_subtype(arg_lo_subtype)
{
	info[0] += "Interactive Tile";
	info[1] = make_string("Type: ", index_to_interactive_tile_type_name(static_cast<int>(lo_subtype)));
}

void SCPP_LCOS_Interactive_Tile::record_lo_subtype(std::ostream& stream)
{
	data_writer(stream, static_cast<int32_t>(lo_subtype));
}

// ----------------------------------------------------------------
// SCPP_LCOS_Warp
// ----------------------------------------------------------------

SCPP_LCOS_Warp::SCPP_LCOS_Warp(int32_t arg_coins_required) :
	SCPP_LCOS_Interactive_Tile(INTERACTIVE_TILE_SUBTYPE::WARP), coins_required(arg_coins_required)
{
	info[2] = "Requires a destination click";
	requires_bonus_click = true;
}

void SCPP_LCOS_Warp::record_extra_data(std::ostream& stream)
{
	data_writer(stream, coins_required);
}

// ----------------------------------------------------------------
// SCPP_LCOS_Info_Sign
// ----------------------------------------------------------------

SCPP_LCOS_Info_Sign::SCPP_LCOS_Info_Sign(const std::string& arg_string) :
	SCPP_LCOS_Interactive_Tile(INTERACTIVE_TILE_SUBTYPE::INFO_SIGN), string(arg_string) {}

void SCPP_LCOS_Info_Sign::record_extra_data(std::ostream& stream)
{
	data_writer(stream, static_cast<int32_t>(string.size()));
	for (const auto& ch : string)
		data_writer(stream, static_cast<int8_t>(ch));;
}

// ----------------------------------------------------------------
// SCPP_LCOS_Conveyor_Belt
// ----------------------------------------------------------------

SCPP_LCOS_Conveyor_Belt::SCPP_LCOS_Conveyor_Belt(bool arg_moves_left) :
	SCPP_LCOS_Interactive_Tile(INTERACTIVE_TILE_SUBTYPE::CONVEYOR_BELT), moves_left(arg_moves_left)
{
	info[2] = make_string("Moves ", arg_moves_left ? "left" : "right");
}

void SCPP_LCOS_Conveyor_Belt::record_extra_data(std::ostream& stream)
{
	data_writer(stream, moves_left);
}

// ----------------------------------------------------------------
// SCPP_LCOS_Item
// ----------------------------------------------------------------

SCPP_LCOS_Item::SCPP_LCOS_Item(ITEM_SUBTYPE arg_lo_subtype) :
	SCPP_LCOS_Base(LO_TYPE::ITEM), lo_subtype(arg_lo_subtype) {}

void SCPP_LCOS_Item::record_lo_subtype(std::ostream& stream)
{
	data_writer(stream, static_cast<int32_t>(lo_subtype));
}

// ----------------------------------------------------------------
// SCPP_LCOS_Food
// ----------------------------------------------------------------

SCPP_LCOS_Food::SCPP_LCOS_Food(int32_t arg_index_x, int32_t arg_index_y) :
	SCPP_LCOS_Item(ITEM_SUBTYPE::FOOD), index_x(arg_index_x), index_y(arg_index_y)
{
	info[0] += "Food";
	info[1] = make_string("Type: ", index_to_food_type_name(index_x + index_y * 10));

	points_award = food_value_by_index(index_y);
}

void SCPP_LCOS_Food::record_extra_data(std::ostream& stream)
{
	data_writer(stream, index_x);
	data_writer(stream, index_y);
}

// ----------------------------------------------------------------
// SCPP_LCOS_Gem
// ----------------------------------------------------------------

SCPP_LCOS_Gem::SCPP_LCOS_Gem(int32_t arg_index) :
	SCPP_LCOS_Item(ITEM_SUBTYPE::GEM), index(arg_index)
{
	info[0] += "Gem";
	info[1] = make_string("Color: ", index_to_gem_color_name(index));

	points_award = gem_value_by_index(index);
}

void SCPP_LCOS_Gem::record_extra_data(std::ostream& stream)
{
	data_writer(stream, index);
}

// ----------------------------------------------------------------
// SCPP_LCOS_Coin
// ----------------------------------------------------------------

SCPP_LCOS_Coin::SCPP_LCOS_Coin(int32_t arg_index) :
	SCPP_LCOS_Item(ITEM_SUBTYPE::COIN), index(arg_index)
{
	info[0] += "Coin";
	info[1] = make_string("Color: ", index_to_coin_color_name(index));
}

void SCPP_LCOS_Coin::record_extra_data(std::ostream& stream)
{
	data_writer(stream, index);
}

// ----------------------------------------------------------------
// SCPP_LCOS_Lifeup
// ----------------------------------------------------------------

SCPP_LCOS_Lifeup::SCPP_LCOS_Lifeup(int32_t arg_index) :
	SCPP_LCOS_Item(ITEM_SUBTYPE::LIFEUP), index(arg_index)
{
	info[0] += "Lifeup";
	info[1] = make_string("Type: ", index_to_lifeup_type_name(index));
}

void SCPP_LCOS_Lifeup::record_extra_data(std::ostream& stream)
{
	data_writer(stream, index);
}

// ----------------------------------------------------------------
// SCPP_LCOS_Stationary_Entity
// ----------------------------------------------------------------

SCPP_LCOS_Stationary_Entity::SCPP_LCOS_Stationary_Entity(STATIONARY_ENTITY_SUBTYPE arg_lo_subtype, int arg_size) :
	SCPP_LCOS_Base(LO_TYPE::STATIONARY_ENTITY), lo_subtype(arg_lo_subtype)
{
	size = arg_size;
	info[0] += "Stationary Entity";
	info[1] = make_string("Type: ", stationary_entity_type_to_name(lo_subtype));
}

void SCPP_LCOS_Stationary_Entity::record_lo_subtype(std::ostream& stream)
{
	data_writer(stream, static_cast<int32_t>(lo_subtype));
}

// ----------------------------------------------------------------
// SCPP_LCOS_Colliding_Entity
// ----------------------------------------------------------------

SCPP_LCOS_Colliding_Entity::SCPP_LCOS_Colliding_Entity(COLLIDING_ENTITY_SUBTYPE arg_lo_subtype, int arg_size) :
	SCPP_LCOS_Base(LO_TYPE::COLLIDING_ENTITY), lo_subtype(arg_lo_subtype)
{
	size = arg_size;
	info[0] += "Colliding Entity";
	info[1] = make_string("Type: ", colliding_entity_type_to_name(lo_subtype));

	points_award = colliding_entity_value(lo_subtype);
}

void SCPP_LCOS_Colliding_Entity::record_lo_subtype(std::ostream& stream)
{
	data_writer(stream, static_cast<int32_t>(lo_subtype));
}

// ----------------------------------------------------------------
// SCPP_LCOS_Walking_Entity
// ----------------------------------------------------------------

SCPP_LCOS_Walking_Entity::SCPP_LCOS_Walking_Entity(WALKING_ENTITY_SUBTYPE arg_lo_subtype, int arg_size) :
	SCPP_LCOS_Base(LO_TYPE::WALKING_ENTITY), lo_subtype(arg_lo_subtype)
{
	size = arg_size;
	info[0] += "Stationary Entity";
	info[1] = make_string("Type: ", walking_entity_type_to_name(lo_subtype));

	points_award = walking_entity_value(lo_subtype);
}

void SCPP_LCOS_Walking_Entity::record_lo_subtype(std::ostream& stream)
{
	data_writer(stream, static_cast<int32_t>(lo_subtype));
}

// ----------------------------------------------------------------
// SCPP_LCOS_Trampoline
// ----------------------------------------------------------------

SCPP_LCOS_Trampoline::SCPP_LCOS_Trampoline(int32_t arg_strength_index) :
	SCPP_LCOS_Stationary_Entity(STATIONARY_ENTITY_SUBTYPE::TRAMPOLINE, 32), strength_index(arg_strength_index)
{
	info[2] = make_string("Strength index: ", strength_index);
}

void SCPP_LCOS_Trampoline::record_extra_data(std::ostream& stream)
{
	data_writer(stream, strength_index);
}

// ----------------------------------------------------------------
// SCPP_LCOS_Thorn
// ----------------------------------------------------------------

SCPP_LCOS_Thorn::SCPP_LCOS_Thorn(int32_t arg_shape_index) :
	SCPP_LCOS_Stationary_Entity(STATIONARY_ENTITY_SUBTYPE::THORN, 32), shape_index(arg_shape_index)
{
	info[2] = make_string("Shape index: ", shape_index);
}

void SCPP_LCOS_Thorn::record_extra_data(std::ostream& stream)
{
	data_writer(stream, shape_index);
}

// ----------------------------------------------------------------
// SCPP_LCOS_Cannon
// ----------------------------------------------------------------

SCPP_LCOS_Cannon::SCPP_LCOS_Cannon(int32_t arg_direction_index) :
	SCPP_LCOS_Colliding_Entity(COLLIDING_ENTITY_SUBTYPE::CANNON, 32), direction_index(arg_direction_index)
{
	info[2] = make_string("Direction index: ", direction_index);
}

void SCPP_LCOS_Cannon::record_extra_data(std::ostream& stream)
{
	data_writer(stream, direction_index);
}

// ----------------------------------------------------------------
// SCPP_LCOS_Ice_Launcher
// ----------------------------------------------------------------

SCPP_LCOS_Ice_Launcher::SCPP_LCOS_Ice_Launcher(int32_t arg_direction_index) :
	SCPP_LCOS_Colliding_Entity(COLLIDING_ENTITY_SUBTYPE::ICE_LAUNCHER, 32), direction_index(arg_direction_index)
{
	info[2] = make_string("Direction index: ", direction_index);
}

void SCPP_LCOS_Ice_Launcher::record_extra_data(std::ostream& stream)
{
	data_writer(stream, direction_index);
}

// ----------------------------------------------------------------
// SCPP_LCOS_Bionic_Ray_Device
// ----------------------------------------------------------------

SCPP_LCOS_Bionic_Ray_Device::SCPP_LCOS_Bionic_Ray_Device(int32_t arg_direction_index) :
	SCPP_LCOS_Colliding_Entity(COLLIDING_ENTITY_SUBTYPE::BIONIC_RAY_DEVICE, 32), direction_index(arg_direction_index)
{
	info[2] = make_string("Direction index: ", direction_index);
}

void SCPP_LCOS_Bionic_Ray_Device::record_extra_data(std::ostream& stream)
{
	data_writer(stream, direction_index);
}

// ----------------------------------------------------------------
// SCPP_LCOS_Electric_Gate_Device
// ----------------------------------------------------------------

SCPP_LCOS_Electric_Gate_Device::SCPP_LCOS_Electric_Gate_Device(int32_t arg_direction_index) :
	SCPP_LCOS_Colliding_Entity(COLLIDING_ENTITY_SUBTYPE::ELECTRIC_GATE_DEVICE, 32), direction_index(arg_direction_index)
{
	info[2] = make_string("Direction index: ", direction_index);
}

void SCPP_LCOS_Electric_Gate_Device::record_extra_data(std::ostream& stream)
{
	data_writer(stream, direction_index);
}

// ----------------------------------------------------------------
// SCPP_Level_Properties
// ----------------------------------------------------------------

SCPP_Level_Properties::SCPP_Level_Properties(Level_Properties arg_lp) :
	lp(arg_lp) {}

// ----------------------------------------------------------------
// SCPP_Victory
// ----------------------------------------------------------------

SCPP_Victory::SCPP_Victory(Level_Properties arg_lp, unsigned int arg_points) :
	lp(arg_lp), points(arg_points) {}

// ----------------------------------------------------------------
// SCPP_Game_Over
// ----------------------------------------------------------------

SCPP_Game_Over::SCPP_Game_Over(Level_Properties arg_lp) :
	lp(arg_lp) {}

// ----------------------------------------------------------------
// SCPP_Level_Theme
// ----------------------------------------------------------------

SCPP_Level_Theme::SCPP_Level_Theme(LEVEL_THEME arg_theme) :
	theme(arg_theme) {}