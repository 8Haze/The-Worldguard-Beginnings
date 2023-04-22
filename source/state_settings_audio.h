#ifndef _STATE_SETTINGS_AUDIO_H_
#define _STATE_SETTINGS_AUDIO_H_

#include "app.h"
#include "gui_buttons_layout.h"

class State_Settings_Audio final : public State
{
private:

	GUI_Buttons_Layout<App&, float> buttons;
	sf::Text label;
	sf::Text info;
	sf::Text music_volume_text;
	sf::Text sfx_volume_text;

	State_Settings_Audio(App& arg_app_handle);

public:

	void handle_events(const sf::Event& event) override;
	void update(const sf::Time& dt) override;
	void render() override;
	void adjust_to_resolution() override;

	constexpr bool request_previous_render() override;
	constexpr bool uses_menu_background() override;
	constexpr bool uses_dark_sheet() override;

	static State* make_state(App& app_handle);

	~State_Settings_Audio();

};

#endif // _STATE_SETTINGS_AUDIO_H_