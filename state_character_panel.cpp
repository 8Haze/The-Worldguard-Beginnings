#include "state_character_panel.h"

#include "utility.h"

// ----------------------------------------------------------------
// State_Character_Panel
// ----------------------------------------------------------------

State_Character_Panel::State_Character_Panel(App& arg_app_handle) :
	State(arg_app_handle, arg_app_handle.get_shared_context())
{
	auto& font = context.resources.fonts.get(RESOURCE_ID::FONTS::MAIN);

	auto player_data = load_player_data(app_handle.get_shared_context().config_manager.player_data_path);

	label = make_label(font, "Character Panel", { 0, 0 }, 80);

	info_health_left = make_label(font, "Current Health:", { 0, 0 }, 40);
	info_damage_left = make_label(font, "Current Damage:", { 0, 0 }, 40);
	info_cells_total_left = make_label(font, "Total Cells:", { 0, 0 }, 40);
	info_cells_balance_left = make_label(font, "Cells Balance:", { 0, 0 }, 40);
	info_cells_needed_left = make_label(font, "Cells For Next Upgrade: ", { 0, 0 }, 40);

	info_health_right = make_label(font, make_string(player_data.health), { 0, 0 }, 40);
	info_damage_right = make_label(font, make_string(player_data.damage), { 0, 0 }, 40);
	info_cells_total_right = make_label(font, make_string(player_data.total_cells), { 0, 0 }, 40);
	info_cells_balance_right = make_label(font, make_string(player_data.cells_balance), { 0, 0 }, 40);
	info_cells_needed_right = make_label(font, make_string(player_data.cells_for_next_upgrade), { 0, 0 }, 40);

	apply_dots(info_cells_total_right);
	apply_dots(info_cells_balance_right);
	apply_dots(info_cells_needed_right);

	info_upgrading1 = make_label(font, "Collect Power Cells by achieving better scores in Campaign levels.", { 0, 0 }, 25);
	info_upgrading2 = make_label(font, "Your character will be permanently powered up automatically when you have enough Cells.", { 0, 0 }, 25);

	adjust_to_resolution();
}

void State_Character_Panel::handle_events(const sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed)
		switch (event.key.code)
		{
		case (sf::Keyboard::Escape): case(sf::Keyboard::Space): app_handle.request_pop(); break;
		}
}

void State_Character_Panel::update(const sf::Time& dt) {}

void State_Character_Panel::render()
{
	context.window.draw(label);

	context.window.draw(info_health_left);
	context.window.draw(info_damage_left);
	context.window.draw(info_cells_total_left);
	context.window.draw(info_cells_balance_left);
	context.window.draw(info_cells_needed_left);

	context.window.draw(info_health_right);
	context.window.draw(info_damage_right);
	context.window.draw(info_cells_total_right);
	context.window.draw(info_cells_balance_right);
	context.window.draw(info_cells_needed_right);

	context.window.draw(info_upgrading1);
	context.window.draw(info_upgrading2);
}

void State_Character_Panel::adjust_to_resolution()
{
	label.setPosition(sf::Vector2f(context.view.getSize().x * 0.5f, context.view.getSize().y * 0.1f));

	info_health_left.setPosition(sf::Vector2f(context.view.getSize().x * 0.35f, context.view.getSize().y * 0.25f));
	info_damage_left.setPosition(sf::Vector2f(context.view.getSize().x * 0.35f, context.view.getSize().y * 0.35f));
	info_cells_total_left.setPosition(sf::Vector2f(context.view.getSize().x * 0.35f, context.view.getSize().y * 0.45f));
	info_cells_balance_left.setPosition(sf::Vector2f(context.view.getSize().x * 0.35f, context.view.getSize().y * 0.55f));
	info_cells_needed_left.setPosition(sf::Vector2f(context.view.getSize().x * 0.35f, context.view.getSize().y * 0.65f));

	info_health_right.setPosition(sf::Vector2f(context.view.getSize().x * 0.65f, context.view.getSize().y * 0.25f));
	info_damage_right.setPosition(sf::Vector2f(context.view.getSize().x * 0.65f, context.view.getSize().y * 0.35f));
	info_cells_total_right.setPosition(sf::Vector2f(context.view.getSize().x * 0.65f, context.view.getSize().y * 0.45f));
	info_cells_balance_right.setPosition(sf::Vector2f(context.view.getSize().x * 0.65f, context.view.getSize().y * 0.55f));
	info_cells_needed_right.setPosition(sf::Vector2f(context.view.getSize().x * 0.65f, context.view.getSize().y * 0.65f));

	info_upgrading1.setPosition(sf::Vector2f(context.view.getSize().x * 0.5f, context.view.getSize().y * 0.8f));
	info_upgrading2.setPosition(sf::Vector2f(context.view.getSize().x * 0.5f, context.view.getSize().y * 0.85f));
}

constexpr bool State_Character_Panel::request_previous_render()
{
	return false;
}

constexpr bool State_Character_Panel::uses_menu_background()
{
	return true;
}

constexpr bool State_Character_Panel::uses_dark_sheet()
{
	return false;
}

State* State_Character_Panel::make_state(App& app_handle)
{
	return new State_Character_Panel(app_handle);
}