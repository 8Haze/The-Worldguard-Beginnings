#include "state_campaign.h"

// ----------------------------------------------------------------
// State_Campaign
// ----------------------------------------------------------------

State_Campaign::State_Campaign(App& arg_app_handle) :
	State(arg_app_handle, arg_app_handle.get_shared_context()),
	level_list(app_handle.get_shared_context().resources, app_handle.get_shared_context().sfx_manager)
{
	auto& font = context.resources.fonts.get(RESOURCE_ID::FONTS::MAIN);
	label = make_label(font, "Campaign", { 0, 0 }, 80);

	adjust_to_resolution();
}

void State_Campaign::handle_events(const sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed)
		switch (event.key.code)
		{
		case (sf::Keyboard::Escape): app_handle.request_pop(); break;
		case (sf::Keyboard::Left): level_list.choose_prev(); break;
		case (sf::Keyboard::Right): level_list.choose_next(); break;
		case (sf::Keyboard::Enter):
			if (level_list.execute(app_handle))
				app_handle.request_push(App::STATE::GAMEPLAY);
			break;
		}
}

void State_Campaign::update(const sf::Time& dt) {}

void State_Campaign::render()
{
	context.window.draw(label);
	level_list.draw(context.window);
}

void State_Campaign::adjust_to_resolution()
{
	label.setPosition(sf::Vector2f(context.view.getSize().x * 0.5f, context.view.getSize().y * 0.1f));

	level_list.set_position({ context.view.getCenter().x, context.view.getCenter().y + context.view.getSize().y * 0.1f });
}

constexpr bool State_Campaign::request_previous_render()
{
	return false;
}

constexpr bool State_Campaign::uses_menu_background()
{
	return true;
}

constexpr bool State_Campaign::uses_dark_sheet()
{
	return false;
}

State* State_Campaign::make_state(App& app_handle)
{
	return new State_Campaign(app_handle);
}