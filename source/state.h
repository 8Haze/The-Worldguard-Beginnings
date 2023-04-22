#ifndef _STATE_H_
#define _STATE_H_

#include <array>
#include <cstdint>

#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "level_object_id.h"
#include "utility.h"

class App;
struct App_Shared_Context;

class State
{
protected:

	App& app_handle;
	App_Shared_Context& context;

	State(const State&) = delete;
	State(State&&) = delete;

public:

	State(App& arg_app_handle, App_Shared_Context& arg_context);

	virtual void handle_events(const sf::Event& event) = 0;
	virtual void update(const sf::Time& dt) = 0;
	virtual void render() = 0;

	virtual void adjust_to_resolution() = 0;

	constexpr virtual bool request_previous_render() = 0;
	constexpr virtual bool uses_menu_background() = 0;
	constexpr virtual bool uses_dark_sheet() = 0;

	virtual ~State() = default;

};

class State_LCOS_Base : public State
{
protected:

	sf::Text label;
	sf::Text info;
	sf::Sprite options;

public:

	State_LCOS_Base(App& arg_app_handle);

	void render() override;
	void adjust_to_resolution() override;

	constexpr bool request_previous_render() override;
	constexpr bool uses_menu_background() override;
	constexpr bool uses_dark_sheet() override;

};

// SCPP - State Communication Parameter Pack

struct SCPP_Base
{
	virtual ~SCPP_Base() = default;
};

struct SCPP_Error final : public SCPP_Base
{
	std::string str;
	SCPP_Error(const std::string& arg_str);
};

struct SCPP_LCOS_Base : public SCPP_Base
{
	std::array<std::string, 4> info;
	LO_TYPE lo_type;
	int size = 32;
	int points_award = 0;
	bool requires_bonus_click = false;

	SCPP_LCOS_Base(LO_TYPE arg_lo_type);

	virtual void record_lo_subtype(std::ostream& stream) = 0;
	virtual void record_extra_data(std::ostream& stream);
};

struct SCPP_LCOS_Decorative_Tile : public SCPP_LCOS_Base
{
	int32_t index_x;
	int32_t index_y;
	int32_t layer;

	SCPP_LCOS_Decorative_Tile(int32_t arg_index_x, int32_t arg_index_y, int32_t arg_layer);

	void record_lo_subtype(std::ostream& stream) override;
	void record_extra_data(std::ostream& stream) override;
};

struct SCPP_LCOS_Animated_Tile : public SCPP_LCOS_Base
{
	int32_t index;
	int32_t layer;

	SCPP_LCOS_Animated_Tile(int32_t arg_index, int32_t arg_layer);

	void record_lo_subtype(std::ostream& stream) override;
	void record_extra_data(std::ostream& stream) override;
};

struct SCPP_LCOS_Solid_Tile : public SCPP_LCOS_Base
{
	int32_t index_x;
	int32_t index_y;

	SCPP_LCOS_Solid_Tile(int32_t arg_index_x, int32_t arg_index_y);

	void record_lo_subtype(std::ostream& stream) override;
	void record_extra_data(std::ostream& stream) override;
};

struct SCPP_LCOS_Destructible_Tile : public SCPP_LCOS_Base
{
	int32_t index_x;
	int32_t index_y;
	DESTRUCTIBLE_TILE_SUBTYPE lo_subtype;

	SCPP_LCOS_Destructible_Tile(int32_t arg_index_x, int32_t arg_index_y, DESTRUCTIBLE_TILE_SUBTYPE arg_lo_subtype);

	void record_lo_subtype(std::ostream& stream) override;
	void record_extra_data(std::ostream& stream) override;
};

struct SCPP_LCOS_Interactive_Tile : public SCPP_LCOS_Base
{
	INTERACTIVE_TILE_SUBTYPE lo_subtype;

	SCPP_LCOS_Interactive_Tile(INTERACTIVE_TILE_SUBTYPE arg_lo_subtype);

	void record_lo_subtype(std::ostream& stream) override;
};

struct SCPP_LCOS_Warp final : public SCPP_LCOS_Interactive_Tile
{
	int32_t coins_required;

	SCPP_LCOS_Warp(int32_t arg_coins_required);

	void record_extra_data(std::ostream& stream) override;
};

struct SCPP_LCOS_Info_Sign final : public SCPP_LCOS_Interactive_Tile
{
	std::string string;

	SCPP_LCOS_Info_Sign(const std::string& arg_string);

	void record_extra_data(std::ostream& stream) override;
};

struct SCPP_LCOS_Conveyor_Belt final : public SCPP_LCOS_Interactive_Tile
{
	bool moves_left;

	SCPP_LCOS_Conveyor_Belt(bool arg_moves_left);

	void record_extra_data(std::ostream& stream) override;
};

struct SCPP_LCOS_Item : public SCPP_LCOS_Base
{
	ITEM_SUBTYPE lo_subtype;

	SCPP_LCOS_Item(ITEM_SUBTYPE arg_lo_subtype);

	void record_lo_subtype(std::ostream& stream) override;
};

struct SCPP_LCOS_Food final : public SCPP_LCOS_Item
{
	int32_t index_x;
	int32_t index_y;

	SCPP_LCOS_Food(int32_t arg_index_x, int32_t arg_index_y);

	void record_extra_data(std::ostream& stream) override;
};

struct SCPP_LCOS_Gem final : public SCPP_LCOS_Item
{
	int32_t index;

	SCPP_LCOS_Gem(int32_t arg_index);

	void record_extra_data(std::ostream& stream) override;
};

struct SCPP_LCOS_Coin final : public SCPP_LCOS_Item
{
	int32_t index;

	SCPP_LCOS_Coin(int32_t arg_index);

	void record_extra_data(std::ostream& stream) override;
};

struct SCPP_LCOS_Lifeup final : public SCPP_LCOS_Item
{
	int32_t index;

	SCPP_LCOS_Lifeup(int32_t arg_index);

	void record_extra_data(std::ostream& stream) override;
};

struct SCPP_LCOS_Stationary_Entity : public SCPP_LCOS_Base
{
	STATIONARY_ENTITY_SUBTYPE lo_subtype;

	SCPP_LCOS_Stationary_Entity(STATIONARY_ENTITY_SUBTYPE arg_lo_subtype, int arg_size);

	void record_lo_subtype(std::ostream& stream) override;
};

struct SCPP_LCOS_Colliding_Entity : public SCPP_LCOS_Base
{
	COLLIDING_ENTITY_SUBTYPE lo_subtype;

	SCPP_LCOS_Colliding_Entity(COLLIDING_ENTITY_SUBTYPE arg_lo_subtype, int arg_size);

	void record_lo_subtype(std::ostream& stream) override;
};

struct SCPP_LCOS_Walking_Entity : public SCPP_LCOS_Base
{
	WALKING_ENTITY_SUBTYPE lo_subtype;

	SCPP_LCOS_Walking_Entity(WALKING_ENTITY_SUBTYPE arg_lo_subtype, int arg_size);

	void record_lo_subtype(std::ostream& stream) override;
};

struct SCPP_LCOS_Trampoline final : public SCPP_LCOS_Stationary_Entity
{
	int32_t strength_index;

	SCPP_LCOS_Trampoline(int32_t arg_strength_index);

	void record_extra_data(std::ostream& stream) override;
};

struct SCPP_LCOS_Thorn final : public SCPP_LCOS_Stationary_Entity
{
	int32_t shape_index;

	SCPP_LCOS_Thorn(int32_t arg_shape_index);

	void record_extra_data(std::ostream& stream) override;
};

struct SCPP_LCOS_Cannon final : public SCPP_LCOS_Colliding_Entity
{
	int32_t direction_index;

	SCPP_LCOS_Cannon(int32_t arg_direction_index);

	void record_extra_data(std::ostream& stream) override;
};

struct SCPP_LCOS_Ice_Launcher final : public SCPP_LCOS_Colliding_Entity
{
	int32_t direction_index;

	SCPP_LCOS_Ice_Launcher(int32_t arg_direction_index);

	void record_extra_data(std::ostream& stream) override;
};

struct SCPP_LCOS_Bionic_Ray_Device final : public SCPP_LCOS_Colliding_Entity
{
	int32_t direction_index;

	SCPP_LCOS_Bionic_Ray_Device(int32_t arg_direction_index);

	void record_extra_data(std::ostream& stream) override;
};

struct SCPP_LCOS_Electric_Gate_Device final : public SCPP_LCOS_Colliding_Entity
{
	int32_t direction_index;

	SCPP_LCOS_Electric_Gate_Device(int32_t arg_direction_index);

	void record_extra_data(std::ostream& stream) override;
};

struct SCPP_Level_Properties final : public SCPP_Base
{
	Level_Properties lp;
	SCPP_Level_Properties(Level_Properties arg_lp);
};

struct SCPP_Victory final : public SCPP_Base
{
	Level_Properties lp;
	unsigned int points;
	SCPP_Victory(Level_Properties arg_lp, unsigned int arg_points);
};

struct SCPP_Game_Over final : public SCPP_Base
{
	Level_Properties lp;
	SCPP_Game_Over(Level_Properties arg_lp);
};

struct SCPP_Level_Theme final : public SCPP_Base
{
	LEVEL_THEME theme;
	SCPP_Level_Theme(LEVEL_THEME arg_theme);
};

#endif // _STATE_H_