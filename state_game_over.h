#ifndef _STATE_GAME_OVER_H_
#define _STATE_GAME_OVER_H_

#include "app.h"
#include "gui_dialogue_box.h"

class State_Game_Over final : public State
{
private:

	GUI_Dialogue_Box<App&, Level_Properties&> box;
	Level_Properties lp;

	State_Game_Over(App& arg_app_handle);

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

#endif // _STATE_GAME_OVER_H_