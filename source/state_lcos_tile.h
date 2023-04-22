#ifndef _STATE_LCOS_TILE_H_
#define _STATE_LCOS_TILE_H_

#include "app.h"
#include "gui_text_field.h"
#include "cursor_highlight.h"

class State_LCOS_Tile final : public State_LCOS_Base
{
private:

	enum class OPTION 
	{ 
		DECORATIVE, 
		ANIMATED,
		SOLID,
		DESTRUCTIBLE,
		FRAGILE, 
		INTERACTIVE,
		COUNT
	}; // keep COUNT last

	State_LCOS_Tile(App& arg_app_handle);

	LEVEL_THEME theme;

	int cursor_index(sf::Vector2i pos);

public:

	void handle_events(const sf::Event& event) override;
	void update(const sf::Time& dt) override;

	static State* make_state(App& app_handle);

};

class State_LCOS_Decorative_Tile final : public State_LCOS_Base
{
private:

	State_LCOS_Decorative_Tile(App& arg_app_handle);

	Cursor_Highlight cursor_highlight;
	LEVEL_THEME theme;

public:

	void handle_events(const sf::Event& event) override;
	void update(const sf::Time& dt) override;
	void render() override;

	static State* make_state(App& app_handle);

};

class State_LCOS_Animated_Tile final : public State_LCOS_Base
{
private:

	State_LCOS_Animated_Tile(App& arg_app_handle);

	Cursor_Highlight cursor_highlight;

public:

	void handle_events(const sf::Event& event) override;
	void update(const sf::Time& dt) override;
	void render() override;

	static State* make_state(App& app_handle);

};

class State_LCOS_Tile_Layer final : public State_LCOS_Base
{
private:

	State_LCOS_Tile_Layer(App& arg_app_handle);

	int cursor_index(sf::Vector2i pos);

public:

	void handle_events(const sf::Event& event) override;
	void update(const sf::Time& dt) override;

	static State* make_state(App& app_handle);

};

class State_LCOS_Solid_Tile final : public State_LCOS_Base
{
private:

	State_LCOS_Solid_Tile(App& arg_app_handle);

	Cursor_Highlight cursor_highlight;
	LEVEL_THEME theme;

public:

	void handle_events(const sf::Event& event) override;
	void update(const sf::Time& dt) override;
	void render() override;

	static State* make_state(App& app_handle);

};

class State_LCOS_Destructible_Tile final : public State_LCOS_Base
{
private:

	State_LCOS_Destructible_Tile(App& arg_app_handle);

	Cursor_Highlight cursor_highlight;
	LEVEL_THEME theme;
	std::unique_ptr<SCPP_Base> pack;

public:

	void handle_events(const sf::Event& event) override;
	void update(const sf::Time& dt) override;
	void render() override;

	static State* make_state(App& app_handle);

};

class State_LCOS_Interactive_Tile final : public State_LCOS_Base
{
private:

	inline static const int max_index = 12;

	State_LCOS_Interactive_Tile(App& arg_app_handle);

	INTERACTIVE_TILE_SUBTYPE transform_index(int index);
	int cursor_index(sf::Vector2i pos);

public:

	void handle_events(const sf::Event& event) override;
	void update(const sf::Time& dt) override;

	static State* make_state(App& app_handle);

};

class State_LCOS_Info_Sign final : public State
{
private:

	inline static const int max_index = 14;

	GUI_Text_Field text_field;
	sf::Text label;

	State_LCOS_Info_Sign(App& arg_app_handle);

public:

	void handle_events(const sf::Event& event) override;
	void update(const sf::Time& dt) override;
	void render() override;
	void adjust_to_resolution() override;

	constexpr bool request_previous_render() override;
	constexpr bool uses_menu_background() override;
	constexpr bool uses_dark_sheet() override;

	static State* make_state(App& app_handle);

};

class State_LCOS_Coin_Warp final : public State
{
private:

	inline static const int max_index = 14;

	GUI_Text_Field text_field;
	sf::Text label;

	State_LCOS_Coin_Warp(App& arg_app_handle);

public:

	void handle_events(const sf::Event& event) override;
	void update(const sf::Time& dt) override;
	void render() override;
	void adjust_to_resolution() override;

	constexpr bool request_previous_render() override;
	constexpr bool uses_menu_background() override;
	constexpr bool uses_dark_sheet() override;

	static State* make_state(App& app_handle);

};

#endif // _STATE_LCOS_TILE_H_