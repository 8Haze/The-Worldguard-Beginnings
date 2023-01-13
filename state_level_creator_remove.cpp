#include "state_level_creator_remove.h"

// ----------------------------------------------------------------
// State_Level_Creator_Remove
// ----------------------------------------------------------------

State_Level_Creator_Remove::State_Level_Creator_Remove(App& arg_app_handle) :
	State(arg_app_handle, arg_app_handle.get_shared_context()),
	level_list(arg_app_handle.get_shared_context().resources, arg_app_handle.get_shared_context().sfx_manager)
{
	auto& font = context.resources.fonts.get(RESOURCE_ID::FONTS::MAIN);
	label = make_label(font, "Level Removal", { 0, 0 }, 80);

	adjust_to_resolution();
}

void State_Level_Creator_Remove::handle_events(const sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed)
		switch (event.key.code)
		{
		case (sf::Keyboard::Escape): app_handle.request_pop(); break;
		case (sf::Keyboard::Up): level_list.choose_prev(); break;
		case (sf::Keyboard::Down): level_list.choose_next(); break;
		case (sf::Keyboard::Enter):
			if (level_list.execute(app_handle))
				app_handle.request_push(App::STATE::LEVEL_CREATOR_REMOVE_CONFIRM);
			break;
		}
}

void State_Level_Creator_Remove::update(const sf::Time& dt) {}

void State_Level_Creator_Remove::render()
{
	context.window.draw(label);
	level_list.draw(context.window);
}

void State_Level_Creator_Remove::adjust_to_resolution()
{
	label.setPosition(sf::Vector2f(context.view.getSize().x * 0.5f, context.view.getSize().y * 0.1f));

	level_list.set_position({ context.view.getCenter().x, context.view.getCenter().y + context.view.getSize().y * 0.1f });
}

constexpr bool State_Level_Creator_Remove::request_previous_render()
{
	return false;
}

constexpr bool State_Level_Creator_Remove::uses_menu_background()
{
	return true;
}

constexpr bool State_Level_Creator_Remove::uses_dark_sheet()
{
	return false;
}

State* State_Level_Creator_Remove::make_state(App& app_handle)
{
	return new State_Level_Creator_Remove(app_handle);
}

// ----------------------------------------------------------------
// State_Level_Creator_Remove_Confirm
// ----------------------------------------------------------------

State_Level_Creator_Remove_Confirm::State_Level_Creator_Remove_Confirm(App& arg_app_handle) :
	State(arg_app_handle, arg_app_handle.get_shared_context()),
	box(arg_app_handle.get_shared_context().resources, arg_app_handle.get_shared_context().sfx_manager)
{
	auto pack = std::move(app_handle.retrieve_scpp());
	lp = dynamic_cast<SCPP_Level_Properties&>(*pack).lp;

	box.set_text(hct::dynarray<std::string>({
		"Are you sure that you want to delete",
		make_string(lp.name, "?") }));
	box.add_button([](App& app, Level_Properties&) { app.request_pop(); }, "No");
	box.add_button([](App& app, Level_Properties& lp)
		{
			app.request_pop();
			app.request_push(App::STATE::LEVEL_CREATOR_REMOVE_PROCEDURE);
			app.register_scpp(std::make_unique<SCPP_Level_Properties>(lp));
		}, "Yes");

	adjust_to_resolution();
}

void State_Level_Creator_Remove_Confirm::handle_events(const sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed)
		switch (event.key.code)
		{
		case (sf::Keyboard::Escape): app_handle.request_pop(); break;
		case (sf::Keyboard::Left): box.choose_next(); break;
		case (sf::Keyboard::Right): box.choose_prev(); break;
		case (sf::Keyboard::Enter): box.execute(app_handle, lp); break;
		}
}

void State_Level_Creator_Remove_Confirm::update(const sf::Time& dt) {}

void State_Level_Creator_Remove_Confirm::render()
{
	box.draw(context.window);
}

void State_Level_Creator_Remove_Confirm::adjust_to_resolution()
{
	box.set_position(sf::Vector2f(context.view.getSize().x * 0.5f, context.view.getSize().y * 0.5f));
}

constexpr bool State_Level_Creator_Remove_Confirm::request_previous_render()
{
	return true;
}

constexpr bool State_Level_Creator_Remove_Confirm::uses_menu_background()
{
	return true;
}

constexpr bool State_Level_Creator_Remove_Confirm::uses_dark_sheet()
{
	return true;
}

State* State_Level_Creator_Remove_Confirm::make_state(App& app_handle)
{
	return new State_Level_Creator_Remove_Confirm(app_handle);
}

// ----------------------------------------------------------------
// State_Level_Creator_Remove_Procedure
// ----------------------------------------------------------------

State_Level_Creator_Remove_Procedure::State_Level_Creator_Remove_Procedure(App& arg_app_handle) :
	State(arg_app_handle, arg_app_handle.get_shared_context()), 
	box(arg_app_handle.get_shared_context().resources, arg_app_handle.get_shared_context().sfx_manager)
{
	namespace fs = std::filesystem;

	auto pack = std::move(app_handle.retrieve_scpp());
	auto& level = dynamic_cast<SCPP_Level_Properties&>(*pack).lp;

	try
	{
		fs::remove(level.path_data);
		fs::remove(level.path_main);
		box.set_text(hct::dynarray<std::string>({
			make_string(level.name, " has been deleted."),
			"Press any key to continue." }));
	}
	catch (const fs::filesystem_error& err)
	{
		box.set_text(hct::dynarray<std::string>({
			make_string("Failed to remove ", level.name, '!'),
			"Press any key to continue." }));
	}

	adjust_to_resolution();
}

void State_Level_Creator_Remove_Procedure::handle_events(const sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed)
	{
		app_handle.request_pop();
		app_handle.request_refresh();
	}
}

void State_Level_Creator_Remove_Procedure::update(const sf::Time& dt) {}

void State_Level_Creator_Remove_Procedure::render()
{
	box.draw(context.window);
}

void State_Level_Creator_Remove_Procedure::adjust_to_resolution()
{
	box.set_position(sf::Vector2f(context.view.getSize().x * 0.5f, context.view.getSize().y * 0.5f));
}

constexpr bool State_Level_Creator_Remove_Procedure::request_previous_render()
{
	return true;
}

constexpr bool State_Level_Creator_Remove_Procedure::uses_menu_background()
{
	return true;
}

constexpr bool State_Level_Creator_Remove_Procedure::uses_dark_sheet()
{
	return true;
}

State* State_Level_Creator_Remove_Procedure::make_state(App& app_handle)
{
	return new State_Level_Creator_Remove_Procedure(app_handle);
}