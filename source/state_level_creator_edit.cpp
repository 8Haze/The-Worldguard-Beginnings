#include "state_level_creator_edit.h"

// ----------------------------------------------------------------
// State_Level_Creator_Edit
// ----------------------------------------------------------------

State_Level_Creator_Edit::State_Level_Creator_Edit(App& arg_app_handle) :
	State(arg_app_handle, arg_app_handle.get_shared_context()),
	level_list(arg_app_handle.get_shared_context().resources, arg_app_handle.get_shared_context().sfx_manager)
{
	auto& font = context.resources.fonts.get(RESOURCE_ID::FONTS::MAIN);
	label = make_label(font, "Level Edition", { 0, 0 }, 80);

	adjust_to_resolution();
}

void State_Level_Creator_Edit::handle_events(const sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed)
		switch (event.key.code)
		{
		case (sf::Keyboard::Escape): app_handle.request_pop(); break;
		case (sf::Keyboard::Up): level_list.choose_prev(); break;
		case (sf::Keyboard::Down): level_list.choose_next(); break;
		case (sf::Keyboard::Enter):
			if (level_list.execute(app_handle))
				app_handle.request_push(App::STATE::LEVEL_CREATOR);
			break;
		}
}

void State_Level_Creator_Edit::update(const sf::Time& dt) {}

void State_Level_Creator_Edit::render()
{
	context.window.draw(label);
	level_list.draw(context.window);
}

void State_Level_Creator_Edit::adjust_to_resolution()
{
	label.setPosition({ context.view.getSize().x * 0.5f, context.view.getSize().y * 0.1f });
	level_list.set_position({ context.view.getCenter().x, context.view.getCenter().y + context.view.getSize().y * 0.1f });
}

constexpr bool State_Level_Creator_Edit::request_previous_render()
{
	return false;
}

constexpr bool State_Level_Creator_Edit::uses_menu_background()
{
	return true;
}

constexpr bool State_Level_Creator_Edit::uses_dark_sheet()
{
	return false;
}

State* State_Level_Creator_Edit::make_state(App& app_handle)
{
	return new State_Level_Creator_Edit(app_handle);
}