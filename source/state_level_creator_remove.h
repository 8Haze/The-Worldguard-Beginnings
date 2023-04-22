#ifndef _STATE_LEVEL_CREATOR_REMOVE_H_
#define _STATE_LEVEL_CREATOR_REMOVE_H_

#include "app.h"
#include "gui_user_level_list.h"
#include "gui_dialogue_box.h"

#include <filesystem>

class State_Level_Creator_Remove final : public State
{
private:

	GUI_User_Level_List level_list;
	sf::Text label;

	State_Level_Creator_Remove(App& arg_app_handle);

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

class State_Level_Creator_Remove_Confirm : public State
{
private:

	GUI_Dialogue_Box<App&, Level_Properties&> box;
	Level_Properties lp;

	State_Level_Creator_Remove_Confirm(App& arg_app_handle);

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

class State_Level_Creator_Remove_Procedure : public State
{
private:

	GUI_Dialogue_Box<App&> box;

	State_Level_Creator_Remove_Procedure(App& arg_app_handle);

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

#endif // _STATE_LEVEL_CREATOR_REMOVE_H_