#ifndef _STATE_CHARACTER_PANEL_H_
#define _STATE_CHARACTER_PANEL_H_

#include "app.h"

class State_Character_Panel final : public State
{
private:

	sf::Text label;
	sf::Text info_health_left;
	sf::Text info_damage_left;
	sf::Text info_cells_total_left;
	sf::Text info_cells_balance_left;
	sf::Text info_cells_needed_left;
	sf::Text info_health_right;
	sf::Text info_damage_right;
	sf::Text info_cells_total_right;
	sf::Text info_cells_balance_right;
	sf::Text info_cells_needed_right;
	sf::Text info_upgrading1;
	sf::Text info_upgrading2;

	State_Character_Panel(App& arg_app_handle);

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

#endif // _STATE_CHARACTER_PANEL_H_