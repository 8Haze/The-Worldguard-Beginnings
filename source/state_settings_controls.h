#ifndef _STATE_SETTINGS_CONTROLS_H_
#define _STATE_SETTINGS_CONTROLS_H_

#include <array>

#include "app.h"
#include "gui_buttons_layout.h"

class State_Settings_Controls final : public State
{
private:

	inline static const char* info_string_normal = "Select an action and press ENTER to start registering a new binding";
	inline static const char* info_string_registering = "Now, press the key you want to bind to this action";

	GUI_Buttons_Layout<const sf::Event&, Controls_Manager&> buttons;
	std::array<sf::Text, static_cast<int>(Controls_Manager::ACTION::COUNT)> controls_texts;
	sf::Text label;
	sf::Text info;
	bool registering = false;

	State_Settings_Controls(App& arg_app_handle);

public:

	void handle_events(const sf::Event& event) override;
	void update(const sf::Time& dt) override;
	void render() override;
	void adjust_to_resolution() override;

	constexpr bool request_previous_render() override;
	constexpr bool uses_menu_background() override;
	constexpr bool uses_dark_sheet() override;

	static State* make_state(App& app_handle);

	~State_Settings_Controls();

};

#endif // _STATE_SETTINGS_CONTROLS_H_