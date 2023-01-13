#include "state_auto_upgrade.h"

// ----------------------------------------------------------------
// State_Auto_Upgrade
// ----------------------------------------------------------------

State_Auto_Upgrade::State_Auto_Upgrade(App& arg_app_handle) : State(arg_app_handle, arg_app_handle.get_shared_context()),
box(arg_app_handle.get_shared_context().resources, arg_app_handle.get_shared_context().sfx_manager)
{
	auto player_data = load_player_data(app_handle.get_shared_context().config_manager.player_data_path);

	if (player_data.cells_balance >= player_data.cells_for_next_upgrade)
	{
		player_data.cells_balance -= player_data.cells_for_next_upgrade;
		++player_data.level;
		save_player_data(player_data, app_handle.get_shared_context().config_manager.player_data_path);
	}
	else
		app_handle.request_pop();

	box.set_text(hct::dynarray<std::string>({
		"You've accumulated enough Power Cells for a power up!",
		make_string("You've gained +3 Health", (player_data.level % 2 == 0 ? "!" : " & +2 Attack Damage!")) }));

	box.add_button([](App& app) { app.request_pop(); }, "OK");

	adjust_to_resolution();
}

void State_Auto_Upgrade::handle_events(const sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed)
		switch (event.key.code)
		{
		case (sf::Keyboard::Enter): box.execute(app_handle); break;
		}
}

void State_Auto_Upgrade::update(const sf::Time& dt) {}

void State_Auto_Upgrade::render()
{
	box.draw(context.window);
}

void State_Auto_Upgrade::adjust_to_resolution()
{
	box.set_position(context.view.getCenter());
}

constexpr bool State_Auto_Upgrade::request_previous_render()
{
	return true;
}

constexpr bool State_Auto_Upgrade::uses_menu_background()
{
	return true;
}

constexpr bool State_Auto_Upgrade::uses_dark_sheet()
{
	return true;
}

State* State_Auto_Upgrade::make_state(App& app_handle)
{
	return new State_Auto_Upgrade(app_handle);
}