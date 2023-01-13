#include "state_main_menu.h"

#include "utility.h"

// ----------------------------------------------------------------
// State_Main_Menu
// ----------------------------------------------------------------

State_Main_Menu::State_Main_Menu(App& arg_app_handle) :
	State(arg_app_handle, arg_app_handle.get_shared_context()),
	buttons(arg_app_handle.get_shared_context().resources, arg_app_handle.get_shared_context().sfx_manager)
{
	logo.setTexture(context.resources.textures.get(RESOURCE_ID::TEXTURES::MAIN_MENU_LOGO));
	set_center(logo);

	context.music_manager.play_track(Music_Manager::ID::MENU);

	buttons.create_button([](App& app) { app.request_push(App::STATE::MODE_SELECT); }, "Play");
	buttons.create_button([](App& app) { app.request_push(App::STATE::LEVEL_CREATOR_SELECT); }, "Level Creator");
	buttons.create_button([](App& app) { app.request_push(App::STATE::SETTINGS); }, "Settings");
	buttons.create_button([](App& app) { app.request_clear(); }, "Quit Game");

	adjust_to_resolution();
}

void State_Main_Menu::handle_events(const sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed)
		switch (event.key.code)
		{
		case (sf::Keyboard::Up): buttons.choose_prev(); break;
		case (sf::Keyboard::Down): buttons.choose_next(); break;
		case (sf::Keyboard::Enter): buttons.execute(app_handle); break;
		}
}

void State_Main_Menu::update(const sf::Time& dt) {}

void State_Main_Menu::render()
{
	logo.set_updated_color();

	context.window.draw(logo);
	buttons.draw(context.window);
}

void State_Main_Menu::adjust_to_resolution()
{
	logo.setPosition(context.view.getSize().x * 0.5f, context.view.getSize().y * 0.15f);

	for (size_t a = 0, max = buttons.get_buttons_count(); a < max; ++a)
		buttons.set_button_position(sf::Vector2f(context.view.getSize().x * 0.5f,
			context.view.getSize().y * 0.35f + (context.view.getSize().y * 0.15f * a)), a);
}

constexpr bool State_Main_Menu::request_previous_render()
{
	return false;
}

constexpr bool State_Main_Menu::uses_menu_background()
{
	return true;
}

constexpr bool State_Main_Menu::uses_dark_sheet()
{
	return false;
}

State* State_Main_Menu::make_state(App& app_handle)
{
	return new State_Main_Menu(app_handle);
}