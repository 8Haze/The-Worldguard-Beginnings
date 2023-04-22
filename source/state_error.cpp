#include "state_error.h"

// ----------------------------------------------------------------
// State_Error
// ----------------------------------------------------------------

State_Error::State_Error(App& arg_app_handle) : State(arg_app_handle, arg_app_handle.get_shared_context()),
	box(arg_app_handle.get_shared_context().resources, arg_app_handle.get_shared_context().sfx_manager)
{
	if (app_handle.any_scpp_awaiting())
	{
		auto pack = std::move(app_handle.retrieve_scpp());
		SCPP_Error* casted_pack = dynamic_cast<SCPP_Error*>(&(*pack));

		if (casted_pack)
			box.set_text(hct::dynarray<std::string>({
				"An error occured:",
				casted_pack->str }));
		else
			box.set_text("An unknown error has occured.");
	}
	else
		box.set_text("An unknown error has occured.");

	box.add_button([](App& app) { app.request_pop(); }, "OK");

	adjust_to_resolution();
}

void State_Error::handle_events(const sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed)
		switch (event.key.code)
		{
		case (sf::Keyboard::Escape): box.execute(app_handle); break;
		case (sf::Keyboard::Enter): box.execute(app_handle); break;
		}
}

void State_Error::update(const sf::Time& dt) {}

void State_Error::render()
{
	box.draw(context.window);
}

void State_Error::adjust_to_resolution()
{
	box.set_position(context.view.getCenter());
}

constexpr bool State_Error::request_previous_render()
{
	return true;
}

constexpr bool State_Error::uses_menu_background()
{
	return true;
}

constexpr bool State_Error::uses_dark_sheet()
{
	return true;
}

State* State_Error::make_state(App& app_handle)
{
	return new State_Error(app_handle);
}