#include "state_settings_progress_reset.h"

// ----------------------------------------------------------------
// State_Settings_Progress_Reset
// ----------------------------------------------------------------

State_Settings_Progress_Reset::State_Settings_Progress_Reset(App& arg_app_handle) :
	State(arg_app_handle, arg_app_handle.get_shared_context()),
	box(arg_app_handle.get_shared_context().resources, arg_app_handle.get_shared_context().sfx_manager)
{
	box.set_text(hct::dynarray<std::string>({
		"Are you sure?", 
		"This will erase ALL of your progress in the campaign." }));

	box.add_button([](App& app) { app.request_pop(); }, "Cancel");
	box.add_button([](App& app) { app.request_pop(); app.request_push(App::STATE::SETTINGS_PROGRESS_RESET_PROCEDURE); }, "Reset");

	adjust_to_resolution();
}

void State_Settings_Progress_Reset::handle_events(const sf::Event& event)
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

void State_Settings_Progress_Reset::update(const sf::Time& dt) {}

void State_Settings_Progress_Reset::render()
{
	box.draw(context.window);
}

void State_Settings_Progress_Reset::adjust_to_resolution()
{
	box.set_position(context.view.getCenter());
}

constexpr bool State_Settings_Progress_Reset::request_previous_render()
{
	return true;
}

constexpr bool State_Settings_Progress_Reset::uses_menu_background()
{
	return true;
}

constexpr bool State_Settings_Progress_Reset::uses_dark_sheet()
{
	return true;
}

State* State_Settings_Progress_Reset::make_state(App& app_handle)
{
	return new State_Settings_Progress_Reset(app_handle);
}

// ----------------------------------------------------------------
// State_Settings_Progress_Reset_Deny
// ----------------------------------------------------------------

State_Settings_Progress_Reset_Deny::State_Settings_Progress_Reset_Deny(App& arg_app_handle) :
	State(arg_app_handle, arg_app_handle.get_shared_context()),
	box(arg_app_handle.get_shared_context().resources, arg_app_handle.get_shared_context().sfx_manager)
{
	box.set_text(hct::dynarray<std::string>({
		"You cannot do that right now.", 
		"Leave the level you're currently playing on first.", 
		"Then, access Settings through the Main Menu and try again." }));

	box.add_button([](App& app) { app.request_pop(); }, "OK");

	adjust_to_resolution();
}

void State_Settings_Progress_Reset_Deny::handle_events(const sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed)
		switch (event.key.code)
		{
		case (sf::Keyboard::Escape): app_handle.request_pop(); break;
		case (sf::Keyboard::Enter): box.execute(app_handle); break;
		}
}

void State_Settings_Progress_Reset_Deny::update(const sf::Time& dt) {}

void State_Settings_Progress_Reset_Deny::render()
{
	box.draw(context.window);
}

void State_Settings_Progress_Reset_Deny::adjust_to_resolution()
{
	box.set_position(context.view.getCenter());
}

constexpr bool State_Settings_Progress_Reset_Deny::request_previous_render()
{
	return true;
}

constexpr bool State_Settings_Progress_Reset_Deny::uses_menu_background()
{
	return true;
}

constexpr bool State_Settings_Progress_Reset_Deny::uses_dark_sheet()
{
	return true;
}

State* State_Settings_Progress_Reset_Deny::make_state(App& app_handle)
{
	return new State_Settings_Progress_Reset_Deny(app_handle);
}

// ----------------------------------------------------------------
// State_Settings_Progress_Reset_Procedure
// ----------------------------------------------------------------

State_Settings_Progress_Reset_Procedure::State_Settings_Progress_Reset_Procedure(App& arg_app_handle) :
	State(arg_app_handle, arg_app_handle.get_shared_context()),
	box(arg_app_handle.get_shared_context().resources, arg_app_handle.get_shared_context().sfx_manager)
{
	auto levels = std::move(load_levels(Config_Manager::campaign_levels_directory, LEVELS_GROUP::CAMPAIGN));

	for (auto x : levels)
	{
		x.achieved_points = 0;
		x.beaten = false;
		save_level_properties(x);
	}

	box.set_text("Your data has been reset.");

	box.add_button([](App& app) { app.request_pop(); }, "OK");

	adjust_to_resolution();
}

void State_Settings_Progress_Reset_Procedure::handle_events(const sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed)
		switch (event.key.code)
		{
		case (sf::Keyboard::Escape): app_handle.request_pop(); break;
		case (sf::Keyboard::Enter): box.execute(app_handle); break;
		}
}

void State_Settings_Progress_Reset_Procedure::update(const sf::Time& dt) {}

void State_Settings_Progress_Reset_Procedure::render()
{
	box.draw(context.window);
}

void State_Settings_Progress_Reset_Procedure::adjust_to_resolution()
{
	box.set_position(context.view.getCenter());
}

constexpr bool State_Settings_Progress_Reset_Procedure::request_previous_render()
{
	return true;
}

constexpr bool State_Settings_Progress_Reset_Procedure::uses_menu_background()
{
	return true;
}

constexpr bool State_Settings_Progress_Reset_Procedure::uses_dark_sheet()
{
	return true;
}

State* State_Settings_Progress_Reset_Procedure::make_state(App& app_handle)
{
	return new State_Settings_Progress_Reset_Procedure(app_handle);
}