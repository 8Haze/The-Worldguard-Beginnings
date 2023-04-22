#include "state_victory.h"

// ----------------------------------------------------------------
// State_Victory
// ----------------------------------------------------------------

State_Victory::State_Victory(App& arg_app_handle) : State(arg_app_handle, arg_app_handle.get_shared_context()),
box(arg_app_handle.get_shared_context().resources, arg_app_handle.get_shared_context().sfx_manager)
{
	auto pack = std::move(app_handle.retrieve_scpp());
	SCPP_Victory& casted_pack = dynamic_cast<SCPP_Victory&>(*pack);
	
	unsigned int previous_points = casted_pack.lp.achieved_points;
	unsigned int new_cells = 0;

	if (casted_pack.points > casted_pack.lp.max_points)
		throw std::runtime_error("State_Victory::State_Victory: gathered points were greater than the possible maximum");

	if (casted_pack.points > casted_pack.lp.achieved_points)
	{
		casted_pack.lp.achieved_points = casted_pack.points;
		casted_pack.lp.beaten = true;
		save_level_properties(casted_pack.lp);
	}

	box.set_text(hct::dynarray<std::string>({
		"Good job! You've cleared the level!",
		make_string("Points Gathered: ", casted_pack.points),
		make_string("Previous Best: ", previous_points) }));

	box.add_button([](App& app) { app.request_pop(); app.request_pop(); }, "OK");

	adjust_to_resolution();
}

void State_Victory::handle_events(const sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed)
		switch (event.key.code)
		{
		case (sf::Keyboard::Enter): box.execute(app_handle); break;
		}
}

void State_Victory::update(const sf::Time& dt) {}

void State_Victory::render()
{
	box.draw(context.window);
}

void State_Victory::adjust_to_resolution()
{
	box.set_position(context.view.getCenter());
}

constexpr bool State_Victory::request_previous_render()
{
	return true;
}

constexpr bool State_Victory::uses_menu_background()
{
	return true;
}

constexpr bool State_Victory::uses_dark_sheet()
{
	return true;
}

State* State_Victory::make_state(App& app_handle)
{
	return new State_Victory(app_handle);
}