#ifndef _STATE_CAMPAIGN_H_
#define _STATE_CAMPAIGN_H_

#include "app.h"
#include "gui_campaign_level_list.h"

class State_Campaign final : public State
{
private:

	GUI_Campaign_Level_List level_list;
	sf::Text label;

	State_Campaign(App& arg_app_handle);

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

#endif // _STATE_CAMPAIGN_H_