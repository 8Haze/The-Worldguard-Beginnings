#include "state_level_creator_alert.h"

// ----------------------------------------------------------------
// State_Level_Creator_Alert
// ----------------------------------------------------------------

State_Level_Creator_Alert::State_Level_Creator_Alert(App& arg_app_handle) : State(arg_app_handle, arg_app_handle.get_shared_context()),
box(arg_app_handle.get_shared_context().resources, arg_app_handle.get_shared_context().sfx_manager)
{
	box.set_text(hct::dynarray<std::string>({
		"There are unsaved changes in the level file.",
		"Quit anyway?" }));

	box.add_button([](App& app) { app.request_pop(); }, "Cancel");
	box.add_button([](App& app) { app.request_pop(); app.request_pop(); }, "Quit");

	adjust_to_resolution();
}

void State_Level_Creator_Alert::handle_events(const sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed)
		switch (event.key.code)
		{
		case (sf::Keyboard::Escape): app_handle.request_pop(); break;
		case (sf::Keyboard::Left): box.choose_prev(); break;
		case (sf::Keyboard::Right): box.choose_next(); break;
		case (sf::Keyboard::Enter): box.execute(app_handle); break;
		}
}

void State_Level_Creator_Alert::update(const sf::Time& dt) {}

void State_Level_Creator_Alert::render()
{
	box.draw(context.window);
}

void State_Level_Creator_Alert::adjust_to_resolution()
{
	box.set_position(context.view.getCenter());
}

constexpr bool State_Level_Creator_Alert::request_previous_render()
{
	return true;
}

constexpr bool State_Level_Creator_Alert::uses_menu_background()
{
	return true;
}

constexpr bool State_Level_Creator_Alert::uses_dark_sheet()
{
	return true;
}

State* State_Level_Creator_Alert::make_state(App& app_handle)
{
	return new State_Level_Creator_Alert(app_handle);
}