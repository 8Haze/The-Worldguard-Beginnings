#ifndef _STATE_LCOS_LIGHT_H_
#define _STATE_LCOS_LIGHT_H_

#include "app.h"
#include "gui_buttons_layout.h"
#include "level_object_id.h"

class State_LCOS_Light final : public State_LCOS_Base
{
private:

	inline static const int max_index = 8;

	State_LCOS_Light(App& arg_app_handle);

	int cursor_index(sf::Vector2i pos);

public:

	void handle_events(const sf::Event& event) override;
	void update(const sf::Time& dt) override;

	static State* make_state(App& app_handle);

};

#endif // _STATE_LCOS_LIGHT_H_