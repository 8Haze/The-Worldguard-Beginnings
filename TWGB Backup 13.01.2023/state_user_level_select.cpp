#include "state_user_level_select.h"

// ----------------------------------------------------------------
// State_User_Level_Select
// ----------------------------------------------------------------

State_User_Level_Select::State_User_Level_Select(App& arg_app_handle) :
	State(arg_app_handle, arg_app_handle.get_shared_context()),
	level_list(app_handle.get_shared_context().resources, app_handle.get_shared_context().sfx_manager)
{
	auto& font = context.resources.fonts.get(RESOURCE_ID::FONTS::MAIN);
	label = make_label(font, "User Level Selection", { 0, 0 }, 80);

	adjust_to_resolution();
}

void State_User_Level_Select::handle_events(const sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed)
		switch (event.key.code)
		{
		case (sf::Keyboard::Escape): app_handle.request_pop(); break;
		case (sf::Keyboard::Up): level_list.choose_prev(); break;
		case (sf::Keyboard::Down): level_list.choose_next(); break;
		case (sf::Keyboard::Enter):
			if (level_list.execute(app_handle))
				app_handle.request_push(App::STATE::GAMEPLAY);
			break;
		}
}

void State_User_Level_Select::update(const sf::Time& dt) {}

void State_User_Level_Select::render()
{
	context.window.draw(label);
	level_list.draw(context.window);
}

void State_User_Level_Select::adjust_to_resolution()
{
	label.setPosition(sf::Vector2f(context.view.getSize().x * 0.5f, context.view.getSize().y * 0.1f));

	level_list.set_position({ context.view.getCenter().x, context.view.getCenter().y + context.view.getSize().y * 0.1f });
}

constexpr bool State_User_Level_Select::request_previous_render()
{
	return false;
}

constexpr bool State_User_Level_Select::uses_menu_background()
{
	return true;
}

constexpr bool State_User_Level_Select::uses_dark_sheet()
{
	return false;
}

State* State_User_Level_Select::make_state(App& app_handle)
{
	return new State_User_Level_Select(app_handle);
}