#include "state_settings_audio.h"

#include "utility.h"

// ----------------------------------------------------------------
// State_Settings_Audio
// ----------------------------------------------------------------

State_Settings_Audio::State_Settings_Audio(App& arg_app_handle) :
	State(arg_app_handle, arg_app_handle.get_shared_context()),
	buttons(arg_app_handle.get_shared_context().resources, arg_app_handle.get_shared_context().sfx_manager)
{
	auto& font = context.resources.fonts.get(RESOURCE_ID::FONTS::MAIN);
	label = make_label(font, "Audio Settings", { 0, 0 }, 80);
	info = make_label(font, "Press left and right arrows to change volume values", { 0, 0 }, 30);
	music_volume_text = make_label(font, "", { 0, 0 }, 40);
	sfx_volume_text = make_label(font, "", { 0, 0 }, 40);

	buttons.create_button([](App& app, float value) { app.get_shared_context().music_manager.change_volume(value); }, "Music");
	buttons.create_button([](App& app, float value) { app.get_shared_context().sfx_manager.change_volume(value);  }, "SFX");

	adjust_to_resolution();
}

void State_Settings_Audio::handle_events(const sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed)
		switch (event.key.code)
		{
		case (sf::Keyboard::Escape): app_handle.request_pop(); break;
		case (sf::Keyboard::Up): buttons.choose_prev(); break;
		case (sf::Keyboard::Down): buttons.choose_next(); break;
		case (sf::Keyboard::Left): buttons.execute(app_handle, -5.0f); break;
		case (sf::Keyboard::Right): buttons.execute(app_handle, 5.0f); break;
		}
}

void State_Settings_Audio::update(const sf::Time& dt)
{
	music_volume_text.setString(make_string(context.music_manager.get_volume(), "%"));
	set_center(music_volume_text);
	sfx_volume_text.setString(make_string(context.sfx_manager.get_volume(), "%"));
	set_center(sfx_volume_text);
}

void State_Settings_Audio::render()
{
	context.window.draw(label);
	context.window.draw(info);
	context.window.draw(music_volume_text);
	context.window.draw(sfx_volume_text);
	buttons.draw(context.window);
}

void State_Settings_Audio::adjust_to_resolution()
{
	label.setPosition(sf::Vector2f(context.view.getSize().x * 0.5f, context.view.getSize().y * 0.1f));
	info.setPosition(sf::Vector2f(context.view.getSize().x * 0.5f, context.view.getSize().y * 0.2f));
	music_volume_text.setPosition(sf::Vector2f(context.view.getSize().x * 0.65f, context.view.getSize().y * 0.5f));
	sfx_volume_text.setPosition(sf::Vector2f(context.view.getSize().x * 0.65f, context.view.getSize().y * 0.65f));

	for (size_t a = 0, max = buttons.get_buttons_count(); a < max; ++a)
		buttons.set_button_position(sf::Vector2f(context.view.getSize().x * 0.35f,
			context.view.getSize().y * 0.5f + (context.view.getSize().y * 0.15f * a)), a);
}

constexpr bool State_Settings_Audio::request_previous_render()
{
	return false;
}

constexpr bool State_Settings_Audio::uses_menu_background()
{
	return true;
}

constexpr bool State_Settings_Audio::uses_dark_sheet()
{
	return false;
}

State* State_Settings_Audio::make_state(App& app_handle)
{
	return new State_Settings_Audio(app_handle);
}

State_Settings_Audio::~State_Settings_Audio()
{
	context.config_manager.update(Config_Manager::DATA::MUSIC_VOL, context.music_manager.get_volume());
	context.config_manager.update(Config_Manager::DATA::SFX_VOL, context.sfx_manager.get_volume());
	context.config_manager.save_changes();
}