#ifndef _STATE_SETTINGS_H_
#define _STATE_SETTINGS_H_

#include "app.h"
#include "gui_buttons_layout.h"

class State_Settings final : public State
{
private:

	GUI_Buttons_Layout<App&> buttons;
	sf::Text label;

	State_Settings(App& arg_app_handle);

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

#endif // _STATE_SETTINGS_H_