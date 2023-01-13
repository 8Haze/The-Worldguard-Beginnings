#include "state_level_creator_select.h"

// ----------------------------------------------------------------
// State_Level_Creator_Select
// ----------------------------------------------------------------

State_Level_Creator_Select::State_Level_Creator_Select(App& arg_app_handle) :
	State (arg_app_handle, arg_app_handle.get_shared_context()),
	buttons(arg_app_handle.get_shared_context().resources, arg_app_handle.get_shared_context().sfx_manager)
{
	auto& font = context.resources.fonts.get(RESOURCE_ID::FONTS::MAIN);
	label = make_label(font, "Level Creator Selection", { 0, 0 }, 80);
	info = make_label(font, "Select what you would like to do", { 0, 0 }, 30);

	buttons.create_button([](App& app) { app.request_push(App::STATE::LEVEL_CREATOR_CREATE); }, "Create new level");
	buttons.create_button([](App& app) { app.request_push(App::STATE::LEVEL_CREATOR_EDIT); }, "Edit level");
	buttons.create_button([](App& app) { app.request_push(App::STATE::LEVEL_CREATOR_REMOVE); }, "Remove level");

	adjust_to_resolution();
}

void State_Level_Creator_Select::handle_events(const sf::Event& event)
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

void State_Level_Creator_Select::update(const sf::Time& dt) {}

void State_Level_Creator_Select::render()
{
	context.window.draw(label);
	context.window.draw(info);
	buttons.draw(context.window);
}

void State_Level_Creator_Select::adjust_to_resolution()
{
	label.setPosition(sf::Vector2f(context.view.getSize().x * 0.5f, context.view.getSize().y * 0.1f));
	info.setPosition(sf::Vector2f(context.view.getSize().x * 0.5f, context.view.getSize().y * 0.2f));

	for (size_t a = 0, max = buttons.get_buttons_count(); a < max; ++a)
		buttons.set_button_position(sf::Vector2f(context.view.getSize().x * 0.5f,
			context.view.getSize().y * 0.425f + (context.view.getSize().y * 0.15f * a)), a);
}

constexpr bool State_Level_Creator_Select::request_previous_render()
{
	return false;
}

constexpr bool State_Level_Creator_Select::uses_menu_background()
{
	return true;
}

constexpr bool State_Level_Creator_Select::uses_dark_sheet()
{
	return false;
}

State* State_Level_Creator_Select::make_state(App& app_handle)
{
	return new State_Level_Creator_Select(app_handle);
}