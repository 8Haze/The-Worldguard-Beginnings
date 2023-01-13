#ifndef _STATE_SETTINGS_VIDEO_H_
#define _STATE_SETTINGS_VIDEO_H_

#include "app.h"
#include "gui_buttons_layout.h"

class State_Settings_Video final : public State
{
private:

	GUI_Buttons_Layout<Config_Manager&, unsigned int, unsigned int> buttons;
	hct::dynarray<sf::VideoMode> modes;
	sf::Text label;
	sf::Text info_usage;
	sf::Text info_resolution;
	size_t index;

	State_Settings_Video(App& arg_app_handle);

public:

	void handle_events(const sf::Event& event) override;
	void update(const sf::Time& dt) override;
	void render() override;
	void adjust_to_resolution() override;

	constexpr bool request_previous_render() override;
	constexpr bool uses_menu_background() override;
	constexpr bool uses_dark_sheet() override;

	static State* make_state(App& app_handle);

	~State_Settings_Video();

};

#endif // _STATE_SETTINGS_VIDEO_H_