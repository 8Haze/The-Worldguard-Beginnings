#ifndef _STATE_GAMEPLAY_H_
#define _STATE_GAMEPLAY_H_

#include <algorithm>

#include "player_interface.h"
#include "level_background.h"
#include "level_content.h"

class State_Gameplay final : public State, public Level_Content
{
private:

	Level_Background level_background;

	Scores_Panel scores_panel;
	Player_Lifebar player_lifebar;
	Temperature_Bar temperature_bar;
	sf::Text player_read_text;

	bool victory = false;

	State_Gameplay(App& arg_app_handle);
	
	void tidy_objects();
	void update_objects(const sf::Time& dt);
	void update_player(const sf::Time& dt);
	void update_static_timers(const sf::Time& dt);
	void render_objects();

public:

	void handle_events(const sf::Event& event) override;
	void update(const sf::Time& dt) override;
	void render() override;
	void adjust_to_resolution() override;

	constexpr bool request_previous_render() override;
	constexpr bool uses_menu_background() override;
	constexpr bool uses_dark_sheet() override;

	static State* make_state(App& app_handle);

	~State_Gameplay();

};

#endif // _STATE_GAMEPLAY_H_