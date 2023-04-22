#include "state_settings.h"

#include "utility.h"

// ----------------------------------------------------------------
// State_Settings
// ----------------------------------------------------------------

State_Settings::State_Settings(App& arg_app_handle) :
	State(arg_app_handle, arg_app_handle.get_shared_context()),
	buttons(arg_app_handle.get_shared_context().resources, arg_app_handle.get_shared_context().sfx_manager)
{
	label = make_label(context.resources.fonts.get(RESOURCE_ID::FONTS::MAIN), "Settings", { 0, 0 }, 80);

	buttons.create_button([](App& app) { app.request_push(App::STATE::SETTINGS_CONTROLS); }, "Controls");
	buttons.create_button([](App& app) { app.request_push(App::STATE::SETTINGS_VIDEO); }, "Video");
	buttons.create_button([](App& app) { app.request_push(App::STATE::SETTINGS_AUDIO); }, "Audio");
	buttons.create_button([](App& app) 
		{
			app.request_push(app.state_exists(App::STATE::GAMEPLAY) ? App::STATE::SETTINGS_PROGRESS_RESET_DENY : App::STATE::SETTINGS_PROGRESS_RESET);
		}, "Reset Progress");

	adjust_to_resolution();
}

void State_Settings::handle_events(const sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed)
		switch (event.key.code)
		{
		case (sf::Keyboard::Escape): app_handle.request_pop(); break;
		case (sf::Keyboard::Up): buttons.choose_prev(); break;
		case (sf::Keyboard::Down): buttons.choose_next(); break;
		case (sf::Keyboard::Enter): buttons.execute(app_handle); break;
		}
}

void State_Settings::update(const sf::Time& dt) {}

void State_Settings::render()
{
	context.window.draw(label);
	buttons.draw(context.window);
}

void State_Settings::adjust_to_resolution()
{
	label.setPosition(sf::Vector2f(context.view.getSize().x * 0.5f, context.view.getSize().y * 0.1f));

	for (size_t a = 0, max = buttons.get_buttons_count(); a < max; ++a)
		buttons.set_button_position(sf::Vector2f(context.view.getSize().x * 0.5f,
			context.view.getSize().y * 0.35f + (context.view.getSize().y * 0.15f * a)), a);
}

constexpr bool State_Settings::request_previous_render()
{
	return false;
}

constexpr bool State_Settings::uses_menu_background()
{
	return true;
}

constexpr bool State_Settings::uses_dark_sheet()
{
	return false;
}

State* State_Settings::make_state(App& app_handle)
{
	return new State_Settings(app_handle);
}