#ifndef _STATE_SETTINGS_PROGRESS_RESET_H_
#define _STATE_SETTINGS_PROGRESS_RESET_H_

#include "app.h"
#include "gui_dialogue_box.h"

class State_Settings_Progress_Reset final : public State
{
private:

	GUI_Dialogue_Box<App&> box;

	State_Settings_Progress_Reset(App& arg_app_handle);

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

class State_Settings_Progress_Reset_Deny final : public State
{
private:

	GUI_Dialogue_Box<App&> box;

	State_Settings_Progress_Reset_Deny(App& arg_app_handle);

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

class State_Settings_Progress_Reset_Procedure final : public State
{
private:

	GUI_Dialogue_Box<App&> box;

	State_Settings_Progress_Reset_Procedure(App& arg_app_handle);

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

#endif // _STATE_SETTINGS_PROGRESS_RESET_H_