#ifndef _STATE_LEVEL_CREATOR_SELECT_H_
#define _STATE_LEVEL_CREATOR_SELECT_H_

#include "app.h"
#include "gui_buttons_layout.h"

class State_Level_Creator_Select final : public State
{
private:

	GUI_Buttons_Layout<App&> buttons;
	sf::Text label;
	sf::Text info;

	State_Level_Creator_Select(App& arg_app_handle);

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

#endif // _STATE_LEVEL_CREATOR_SELECT_H_