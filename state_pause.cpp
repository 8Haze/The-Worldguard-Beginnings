#include "state_pause.h"

// ----------------------------------------------------------------
// State_Pause
// ----------------------------------------------------------------

State_Pause::State_Pause(App& arg_app_handle) : State(arg_app_handle, arg_app_handle.get_shared_context()),
	box(arg_app_handle.get_shared_context().resources, arg_app_handle.get_shared_context().sfx_manager)
{
	box.set_text("Game paused");

	box.add_button([](App& app) { app.request_pop(); }, "Resume");
	box.add_button([](App& app) { app.request_push(App::STATE::SETTINGS); }, "Settings");
	box.add_button([](App& app) { app.request_pop(); app.request_push(App::STATE::PAUSE_LEAVE); }, "Quit Level");

	adjust_to_resolution();
}

void State_Pause::handle_events(const sf::Event& event)
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

void State_Pause::update(const sf::Time& dt) {}

void State_Pause::render()
{
	box.draw(context.window);
}

void State_Pause::adjust_to_resolution()
{
	box.set_position(context.view.getCenter());
}

constexpr bool State_Pause::request_previous_render()
{
	return true;
}

constexpr bool State_Pause::uses_menu_background()
{
	return true;
}

constexpr bool State_Pause::uses_dark_sheet()
{
	return true;
}

State* State_Pause::make_state(App& app_handle)
{
	return new State_Pause(app_handle);
}

// ----------------------------------------------------------------
// State_Pause_Leave
// ----------------------------------------------------------------

State_Pause_Leave::State_Pause_Leave(App& arg_app_handle) : State(arg_app_handle, arg_app_handle.get_shared_context()),
	box(arg_app_handle.get_shared_context().resources, arg_app_handle.get_shared_context().sfx_manager)
{
	box.set_text(hct::dynarray<std::string>({
		"Are you sure?", 
		"No progress will be saved if you leave the level now." }));

	box.add_button([](App& app) { app.request_pop(); app.request_push(App::STATE::PAUSE); }, "No");
	box.add_button([](App& app) { app.request_pop(); app.request_pop(); }, "Yes");

	adjust_to_resolution();
}

void State_Pause_Leave::handle_events(const sf::Event& event)
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

void State_Pause_Leave::update(const sf::Time& dt) {}

void State_Pause_Leave::render()
{
	box.draw(context.window);
}

void State_Pause_Leave::adjust_to_resolution()
{
	box.set_position(context.view.getCenter());
}

constexpr bool State_Pause_Leave::request_previous_render()
{
	return true;
}

constexpr bool State_Pause_Leave::uses_menu_background()
{
	return true;
}

constexpr bool State_Pause_Leave::uses_dark_sheet()
{
	return true;
}

State* State_Pause_Leave::make_state(App& app_handle)
{
	return new State_Pause_Leave(app_handle);
}