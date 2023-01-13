#include "state_game_over.h"

// ----------------------------------------------------------------
// State_Game_Over
// ----------------------------------------------------------------

State_Game_Over::State_Game_Over(App& arg_app_handle) : State(arg_app_handle, arg_app_handle.get_shared_context()),
box(arg_app_handle.get_shared_context().resources, arg_app_handle.get_shared_context().sfx_manager)
{
	auto pack = std::move(app_handle.retrieve_scpp());
	lp = dynamic_cast<SCPP_Game_Over&>(*pack).lp;

	box.set_text(hct::dynarray<std::string>({
		"Game over!",
		"Would you like to try again?" }));

	box.add_button([](App& app, Level_Properties& lp) { app.register_scpp(std::make_unique<SCPP_Level_Properties>(lp)); app.request_pop(); app.request_refresh(); }, "Yes");
	box.add_button([](App& app, Level_Properties&) { app.request_pop(); app.request_pop(); }, "No");

	adjust_to_resolution();
}

void State_Game_Over::handle_events(const sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed)
		switch (event.key.code)
		{
		case (sf::Keyboard::Left): box.choose_prev(); break;
		case (sf::Keyboard::Right): box.choose_next(); break;
		case (sf::Keyboard::Enter): box.execute(app_handle, lp); break;
		}
}

void State_Game_Over::update(const sf::Time& dt) {}

void State_Game_Over::render()
{
	box.draw(context.window);
}

void State_Game_Over::adjust_to_resolution()
{
	box.set_position(context.view.getCenter());
}

constexpr bool State_Game_Over::request_previous_render()
{
	return true;
}

constexpr bool State_Game_Over::uses_menu_background()
{
	return true;
}

constexpr bool State_Game_Over::uses_dark_sheet()
{
	return true;
}

State* State_Game_Over::make_state(App& app_handle)
{
	return new State_Game_Over(app_handle);
}