#include "state_settings_controls.h"

// ----------------------------------------------------------------
// State_Settings_Controls
// ----------------------------------------------------------------

State_Settings_Controls::State_Settings_Controls(App& arg_app_handle) :
	State(arg_app_handle, arg_app_handle.get_shared_context()),
	buttons(arg_app_handle.get_shared_context().resources, arg_app_handle.get_shared_context().sfx_manager)
{
	auto& font = context.resources.fonts.get(RESOURCE_ID::FONTS::MAIN);
	label = make_label(font, "Controls Settings", { 0, 0 }, 80);
	info = make_label(font, info_string_normal, { 0, 0 }, 30);
	for (int a = 0, max = static_cast<int>(Controls_Manager::ACTION::COUNT); a < max; ++a)
		controls_texts[a] = make_label(font, key_to_string(context.controls_manager.get_key(static_cast<Controls_Manager::ACTION>(a))), { 0, 0 }, 30);

	buttons.create_button([](const sf::Event& event, Controls_Manager& controls_manager) 
		{ controls_manager.set_key(Controls_Manager::ACTION::LEFT, event.key.code); }, "Walk Left");
	buttons.create_button([](const sf::Event& event, Controls_Manager& controls_manager) 
		{ controls_manager.set_key(Controls_Manager::ACTION::RIGHT, event.key.code); }, "Walk Right");
	buttons.create_button([](const sf::Event& event, Controls_Manager& controls_manager) 
		{ controls_manager.set_key(Controls_Manager::ACTION::JUMP, event.key.code); }, "Jump");
	buttons.create_button([](const sf::Event& event, Controls_Manager& controls_manager) 
		{ controls_manager.set_key(Controls_Manager::ACTION::SHOOT, event.key.code); }, "Shoot");

	adjust_to_resolution();
}

void State_Settings_Controls::handle_events(const sf::Event& event)
{
	if (registering && event.type == sf::Event::KeyPressed)
	{
		buttons.execute(event, context.controls_manager);
		registering = false;
		info.setString(info_string_normal);
		set_center(info);
	}
	else if (event.type == sf::Event::KeyPressed)
		switch (event.key.code)
		{
		case (sf::Keyboard::Escape): app_handle.request_pop(); break;
		case (sf::Keyboard::Up): buttons.choose_prev(); break;
		case (sf::Keyboard::Down): buttons.choose_next(); break;
		case (sf::Keyboard::Enter): 
			registering = true; 
			info.setString(info_string_registering);
			set_center(info); 
			break;
		}
}

void State_Settings_Controls::update(const sf::Time& dt)
{
	for (int a = 0, max = static_cast<int>(Controls_Manager::ACTION::COUNT); a < max; ++a)
	{
		controls_texts[a].setString(key_to_string(context.controls_manager.get_key(static_cast<Controls_Manager::ACTION>(a))));
		set_center(controls_texts[a]);
	}
}

void State_Settings_Controls::render()
{
	context.window.draw(label);
	context.window.draw(info);
	for (auto& x : controls_texts)
		context.window.draw(x);
	buttons.draw(context.window);
}

void State_Settings_Controls::adjust_to_resolution()
{
	label.setPosition(sf::Vector2f(context.view.getSize().x * 0.5f, context.view.getSize().y * 0.1f));
	info.setPosition(sf::Vector2f(context.view.getSize().x * 0.5f, context.view.getSize().y * 0.2f));

	for (size_t a = 0, max = static_cast<size_t>(Controls_Manager::ACTION::COUNT); a < max; ++a)
		controls_texts[a].setPosition(sf::Vector2f(context.view.getSize().x * 0.65f,
			context.view.getSize().y * 0.35f + (context.view.getSize().y * 0.15f * a)));

	for (size_t a = 0, max = buttons.get_buttons_count(); a < max; ++a)
		buttons.set_button_position(sf::Vector2f(context.view.getSize().x * 0.35f,
			context.view.getSize().y * 0.35f + (context.view.getSize().y * 0.15f * a)), a);
}

constexpr bool State_Settings_Controls::request_previous_render()
{
	return false;
}

constexpr bool State_Settings_Controls::uses_menu_background()
{
	return true;
}

constexpr bool State_Settings_Controls::uses_dark_sheet()
{
	return false;
}

State* State_Settings_Controls::make_state(App& app_handle)
{
	return new State_Settings_Controls(app_handle);
}

State_Settings_Controls::~State_Settings_Controls()
{
	context.controls_manager.save_changes();
}