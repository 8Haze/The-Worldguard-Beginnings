#include "state_level_creator.h"

#include <cmath>
#include <iostream>

// ----------------------------------------------------------------
// State_Level_Creator
// ----------------------------------------------------------------
	
State_Level_Creator::State_Level_Creator(App& arg_app_handle) :
	State(arg_app_handle, arg_app_handle.get_shared_context()), Level_Content(arg_app_handle)
{
	auto& window = app_handle.get_shared_context().window;
	auto& resources = app_handle.get_shared_context().resources;

	camera.setSize(static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y));
	camera.setCenter(camera.getSize().x * 0.5f, camera.getSize().y * 0.5f);

	auto& background_texture = resources.textures.get(RESOURCE_ID::TEXTURES::LEVEL_CREATOR_GRID);
	background_texture.setRepeated(true);
	background.setTexture(background_texture);
	background.setTextureRect(sf::IntRect(0, 0, window.getSize().x, window.getSize().y));

	toolbelt.setTexture(resources.textures.get(RESOURCE_ID::TEXTURES::TOOLBELT));

	toolbelt_info_panel.setTexture(resources.textures.get(RESOURCE_ID::TEXTURES::TOOLBELT_INFO_PANEL));
	toolbelt_info_panel.setOrigin(toolbelt_info_panel.getGlobalBounds().width, 0.f);

	for (int a = 0; a < description_infos.size(); ++a)
		description_infos[a] = make_label(resources.fonts.get(RESOURCE_ID::FONTS::MAIN), "", { 0.f, 0.f }, 16, false);

	option_info = make_label(resources.fonts.get(RESOURCE_ID::FONTS::MAIN), "", { camera.getCenter().x, camera.getCenter().y + camera.getSize().y * 0.4f }, 32);

	load_everything_from_file(app_handle);
	initialize_objects_points_list();
	sort_objects();

	player.set_sprite_origin({ 0.f, 0.f });
	player.move({ 8.f - 32.f, 8.f - 32.f });

	window.setMouseCursorVisible(true);

	objects_stream.setstate(std::ios::binary);

	update_action_info();
	update_points_info();

	adjust_to_resolution();
}

void State_Level_Creator::update_action_info()
{
	for (int a = 0; a < description_infos.size() - 1; ++a)
		description_infos[a].setString("");

	switch (action)
	{
	case(ACTIONS::OBJECT_INSERT): description_infos[0].setString("Object Insertion"); break;
	case(ACTIONS::OBJECT_INSERT_BONUS): description_infos[0].setString("Object Destination Selection"); break;
	case(ACTIONS::OBJECT_REMOVE): description_infos[0].setString("Object Removal"); break;
	case(ACTIONS::MASS_OBJECT_INSERT_1): description_infos[0].setString("Mass Object Insertion (1)"); break;
	case(ACTIONS::MASS_OBJECT_INSERT_2): description_infos[0].setString("Mass Object Insertion (2)"); break;
	case(ACTIONS::MASS_OBJECT_REMOVE_1): description_infos[0].setString("Mass Object Removal (1)"); break;
	case(ACTIONS::MASS_OBJECT_REMOVE_2): description_infos[0].setString("Mass Object Removal (2)"); break;
	case(ACTIONS::PLAYER_START_POSITION_SETTING): description_infos[0].setString("Player Start Position Selection"); break;
	default: description_infos[0].setString("No Action Selected"); break;
	}

	if (object_pack)
		for (int a = 0, b = 1; a < object_pack->info.size() && b < description_infos.size() - 1; ++a, ++b)
			description_infos[b].setString(object_pack->info[a]);
}

void State_Level_Creator::update_points_info()
{
	description_infos.back().setString("Points: " + std::to_string(available_points));
}

void State_Level_Creator::update_option_info()
{
	auto option = highlighted_toolbelt_option();

	switch (option)
	{
	case(TOOLBELT_OPTIONS::LEFT): option_info.setString("Move Left"); break;
	case(TOOLBELT_OPTIONS::UP): option_info.setString("Move Up"); break;
	case(TOOLBELT_OPTIONS::RIGHT): option_info.setString("Move Right"); break;
	case(TOOLBELT_OPTIONS::DOWN): option_info.setString("Move Down"); break;
	case(TOOLBELT_OPTIONS::TILES): option_info.setString("Tiles"); break;
	case(TOOLBELT_OPTIONS::ITEMS): option_info.setString("Items"); break;
	case(TOOLBELT_OPTIONS::ENTITIES): option_info.setString("Entities"); break;
	case(TOOLBELT_OPTIONS::MASS_INSERT): option_info.setString("Mass Insertion"); break;
	case(TOOLBELT_OPTIONS::REMOVE): option_info.setString("Removal"); break;
	case(TOOLBELT_OPTIONS::MASS_REMOVE): option_info.setString("Mass Removal"); break;
	case(TOOLBELT_OPTIONS::PLAYER_START_POSITION): option_info.setString("Player Start Position"); break;
	case(TOOLBELT_OPTIONS::SORT): option_info.setString("Sort"); break;
	case(TOOLBELT_OPTIONS::SAVE): option_info.setString("Sort & Save"); break;
	default: option_info.setString("");
	}
}

void State_Level_Creator::move_everything(const sf::Keyboard::Key key)
{
	auto shift = camera.move_within_bounds(key_to_move_vector(key));
	if (sf::Vector2i exact_shift(std::round(shift.x), std::round(shift.y));
		(exact_shift.x == 32 || exact_shift.x == -32 || exact_shift.x == 0) &&
		(exact_shift.y == 32 || exact_shift.y == -32 || exact_shift.y == 0))
	{
		x_shift += shift.x;
		y_shift += shift.y;
		background.move(shift);
		toolbelt.move(shift);
		toolbelt_info_panel.move(shift);
		option_info.move(shift);
		for (auto& text : description_infos)
			text.move(shift);
	}
	else
		camera.move(-shift);
}

void State_Level_Creator::initialize_objects_points_list()
{
	auto accumulator = [&](auto& container)
	{
		for (const auto& object : container)
		{
			if (Coin* casted = dynamic_cast<Coin*>(&(*object)); casted)
				continue;
			if (Lifeup* casted = dynamic_cast<Lifeup*>(&(*object)); casted)
				continue;

			if (Value_Component* casted = dynamic_cast<Value_Component*>(&(*object)); casted)
			{
				auto x = object->get_original_position().x;
				auto y = object->get_original_position().y;
				x -= ((static_cast<int32_t>(object->get_sprite().getGlobalBounds().width) % 32) >> 1);
				y -= (static_cast<int>(object->get_sprite().getGlobalBounds().height) % 32);
				objects_points_list.emplace_back(std::pair<int32_t, int32_t>(x, y), casted->get_value());
				available_points += casted->get_value();
			}
		}
	};

	accumulator(items);
	accumulator(stationary_entities);
	accumulator(colliding_entities);
	accumulator(walking_entities);
}

sf::Vector2f State_Level_Creator::key_to_move_vector(const sf::Keyboard::Key key)
{
	switch (key)
	{
	case (sf::Keyboard::Left): return { -32.f, 0.f }; break;
	case (sf::Keyboard::Right): return { 32.f, 0.f }; break;
	case (sf::Keyboard::Up): return { 0.f, -32.f }; break;
	case (sf::Keyboard::Down): return { 0.f, 32.f }; break;
	}
	return { 0.f, 0.f };
}

bool State_Level_Creator::mouse_on_toolbelt()
{
	auto pos = sf::Mouse::getPosition();
	return
		(pos.x >= 0) &&
		(pos.x <= static_cast<int>(toolbelt.getGlobalBounds().width)) &&
		(pos.y >= 0) &&
		(pos.y <= static_cast<int>(toolbelt.getGlobalBounds().height));
}

bool State_Level_Creator::mouse_on_info_panel()
{
	auto pos = sf::Mouse::getPosition();
	return
		(pos.x <= camera.getSize().x) &&
		(pos.x >= camera.getSize().x - static_cast<int>(toolbelt_info_panel.getGlobalBounds().width)) &&
		(pos.y >= 0) &&
		(pos.y <= static_cast<int>(toolbelt_info_panel.getGlobalBounds().height));
}

State_Level_Creator::TOOLBELT_OPTIONS State_Level_Creator::highlighted_toolbelt_option()
{
	if (mouse_on_toolbelt())
		return static_cast<State_Level_Creator::TOOLBELT_OPTIONS>(sf::Mouse::getPosition().x / 64);
	return State_Level_Creator::TOOLBELT_OPTIONS::COUNT;
}

void State_Level_Creator::handle_keyboard_events(const sf::Event& event)
{
	switch (event.key.code)
	{
	case (sf::Keyboard::Escape):
		if (any_unsaved_changes)
			app_handle.request_push(App::STATE::LEVEL_CREATOR_ALERT);
		else
			app_handle.request_pop();
		break;

	case (sf::Keyboard::Left): case (sf::Keyboard::Right): case (sf::Keyboard::Up): case (sf::Keyboard::Down):
		move_everything(event.key.code);
		break;
	}
}

void State_Level_Creator::handle_action_events(const sf::Event& event)
{
	auto x = (sf::Mouse::getPosition().x - sf::Mouse::getPosition().x % 32) + x_shift;
	auto y = (sf::Mouse::getPosition().y - sf::Mouse::getPosition().y % 32) + y_shift;

	switch (action)
	{
	case (ACTIONS::OBJECT_INSERT):
		perform_object_insert(x, y);
		break;
	case (ACTIONS::OBJECT_INSERT_BONUS):
		perform_object_insert_bonus(x, y);
		break;
	case (ACTIONS::OBJECT_REMOVE):
		perform_object_remove(x, y);
		break;
	case (ACTIONS::PLAYER_START_POSITION_SETTING):
		perform_player_start_position_setting(x, y);
		break;
	case (ACTIONS::MASS_OBJECT_INSERT_1):
		perform_mass_object_insert_1(x, y);
		break;
	case (ACTIONS::MASS_OBJECT_INSERT_2):
		perform_mass_object_insert_2(x, y);
		break;
	case (ACTIONS::MASS_OBJECT_REMOVE_1):
		perform_mass_object_remove_1(x, y);
		break;
	case (ACTIONS::MASS_OBJECT_REMOVE_2):
		perform_mass_object_remove_2(x, y);
		break;
	}
}

void State_Level_Creator::handle_toolbelt_events(const sf::Event& event)
{
	switch (highlighted_toolbelt_option())
	{
	case (TOOLBELT_OPTIONS::LEFT):
	{
		move_everything(sf::Keyboard::Key::Left);
		break;
	}
	case (TOOLBELT_OPTIONS::RIGHT):
	{
		move_everything(sf::Keyboard::Key::Right);
		break;
	}
	case (TOOLBELT_OPTIONS::UP):
	{
		move_everything(sf::Keyboard::Key::Up);
		break;
	}
	case (TOOLBELT_OPTIONS::DOWN):
	{
		move_everything(sf::Keyboard::Key::Down);
		break;
	}
	case (TOOLBELT_OPTIONS::TILES):
	{
		app_handle.register_scpp(std::make_unique<SCPP_Level_Theme>(level_properties.theme));
		app_handle.request_push(App::STATE::LEVEL_CREATOR_OBJECT_SELECTION_TILE);
		break;
	}
	case (TOOLBELT_OPTIONS::ITEMS):
	{
		app_handle.request_push(App::STATE::LEVEL_CREATOR_OBJECT_SELECTION_ITEM);
		break;
	}
	case (TOOLBELT_OPTIONS::ENTITIES):
	{
		app_handle.register_scpp(std::make_unique<SCPP_Level_Theme>(level_properties.theme));
		app_handle.request_push(App::STATE::LEVEL_CREATOR_OBJECT_SELECTION_ENTITY);
		break;
	}
	case (TOOLBELT_OPTIONS::MASS_INSERT):
	{
		if (action == ACTIONS::OBJECT_INSERT)
		{
			action = ACTIONS::MASS_OBJECT_INSERT_1;
			update_action_info();
		}
		break;
	}
	case (TOOLBELT_OPTIONS::REMOVE):
	{
		object_pack = nullptr;
		action = ACTIONS::OBJECT_REMOVE;
		update_action_info();
		break;
	}
	case (TOOLBELT_OPTIONS::MASS_REMOVE):
	{
		object_pack = nullptr;
		action = ACTIONS::MASS_OBJECT_REMOVE_1;
		update_action_info();
		break;
	}
	case (TOOLBELT_OPTIONS::PLAYER_START_POSITION):
	{
		action = ACTIONS::PLAYER_START_POSITION_SETTING;
		object_pack = nullptr;
		update_action_info();
		break;
	}
	case (TOOLBELT_OPTIONS::SORT):
	{
		unsorted_objects = 0;
		sort_objects();
		break;
	}
	case (TOOLBELT_OPTIONS::SAVE):
	{
		unsorted_objects = 0;
		sort_objects();
		save_data();
		break;
	}
	}
}

void State_Level_Creator::perform_object_insert(int x, int y)
{
	if (!object_pack->requires_bonus_click)
	{
		record_object(x, y);
		load_objects_from_string_stream(app_handle, objects_stream);
		objects_stream.clear();
	}
	else
	{
		bonus_click_position.x = x;
		bonus_click_position.y = y;

		action = ACTIONS::OBJECT_INSERT_BONUS;
		update_action_info();
	}
}

void State_Level_Creator::perform_object_insert_bonus(int x, int y)
{
	auto old_x = bonus_click_position.x;
	auto old_y = bonus_click_position.y;
	bonus_click_position.x = x;
	bonus_click_position.y = y;
	record_object(old_x, old_y);
	load_objects_from_string_stream(app_handle, objects_stream);
	objects_stream.clear();

	action = ACTIONS::OBJECT_INSERT;
	update_action_info();
}

void State_Level_Creator::perform_object_remove(int x, int y)
{
	remove_objects(x, y);
}

void State_Level_Creator::perform_mass_object_insert_1(int x, int y)
{
	mass_operation_rectangle.first = sf::Vector2i(x, y);

	action = ACTIONS::MASS_OBJECT_INSERT_2;
	update_action_info();
}

void State_Level_Creator::perform_mass_object_insert_2(int x, int y)
{
	mass_operation_rectangle.second = sf::Vector2i(x, y);

	action = ACTIONS::MASS_OBJECT_INSERT_1;
	update_action_info();

	if (mass_operation_rectangle.second.x < mass_operation_rectangle.first.x)
		std::swap(mass_operation_rectangle.first.x, mass_operation_rectangle.second.x);
	if (mass_operation_rectangle.second.y < mass_operation_rectangle.first.y)
		std::swap(mass_operation_rectangle.first.y, mass_operation_rectangle.second.y);

	int object_size_shift = 32 * ((object_pack->size + 16) / 32);
	for (y = mass_operation_rectangle.first.y; y <= mass_operation_rectangle.second.y; y += object_size_shift)
		for (x = mass_operation_rectangle.first.x; x <= mass_operation_rectangle.second.x; x += object_size_shift)
			record_object(x, y);

	load_objects_from_string_stream(app_handle, objects_stream);
	objects_stream.clear();
}

void State_Level_Creator::perform_mass_object_remove_1(int x, int y)
{
	mass_operation_rectangle.first = sf::Vector2i(x, y);

	action = ACTIONS::MASS_OBJECT_REMOVE_2;
	update_action_info();
}

void State_Level_Creator::perform_mass_object_remove_2(int x, int y)
{
	mass_operation_rectangle.second = sf::Vector2i(x, y);

	action = ACTIONS::MASS_OBJECT_REMOVE_1;
	update_action_info();

	if (mass_operation_rectangle.second.x < mass_operation_rectangle.first.x)
		std::swap(mass_operation_rectangle.first.x, mass_operation_rectangle.second.x);
	if (mass_operation_rectangle.second.y < mass_operation_rectangle.first.y)
		std::swap(mass_operation_rectangle.first.y, mass_operation_rectangle.second.y);

	for (y = mass_operation_rectangle.first.y; y <= mass_operation_rectangle.second.y; y += 32)
		for (x = mass_operation_rectangle.first.x; x <= mass_operation_rectangle.second.x; x += 32)
			remove_objects(x, y);
}

void State_Level_Creator::perform_player_start_position_setting(int x, int y)
{
	player.set_position({ static_cast<float>(x + 8.f), static_cast<float>(y + 8.f) });

	any_unsaved_changes = true;
}

void State_Level_Creator::record_object(int32_t x, int32_t y)
{
	data_writer(objects_stream, static_cast<int32_t>(object_pack->lo_type));

	int shift = (object_pack->size % 32);
	x += shift >> 1;
	y += shift;

	object_pack->record_lo_subtype(objects_stream);

	data_writer(objects_stream, x);
	data_writer(objects_stream, y);

	if (object_pack->requires_bonus_click)
	{
		data_writer(objects_stream, bonus_click_position.x);
		data_writer(objects_stream, bonus_click_position.y);
	}

	object_pack->record_extra_data(objects_stream);

	if (object_pack->points_award > 0)
	{
		available_points += object_pack->points_award;
		int points_x = x - (shift >> 1);
		int points_y = y - shift;
		objects_points_list.emplace_back(std::pair<int32_t, int32_t>(points_x, points_y), object_pack->points_award);
		update_points_info();
	}

	++unsorted_objects;
	any_unsaved_changes = true;
}

void State_Level_Creator::remove_objects(int32_t x, int32_t y)
{
	auto remover = [x, y](auto& container)
	{
		container.erase(std::remove_if(container.begin(), container.end(),
			[x, y](const auto& object)
			{
				int32_t object_x = object->get_original_position().x;
				int32_t object_y = object->get_original_position().y;
				object_x -= ((static_cast<int32_t>(object->get_sprite().getGlobalBounds().width) % 32) >> 1);
				object_y -= (static_cast<int32_t>(object->get_sprite().getGlobalBounds().height) % 32);
				return object_x == x && object_y == y;
			}),
			container.end());
	};

	remover(decorative_tiles);
	remover(animated_tiles);
	remover(solid_tiles);
	remover(destructible_tiles);
	remover(interactive_tiles);
	remover(items);
	remover(stationary_entities);
	remover(colliding_entities);
	remover(walking_entities);

	objects_points_list.erase(std::remove_if(objects_points_list.begin(), objects_points_list.end(),
		[&](const auto& object)
		{
			bool result = object.first.first == x && object.first.second == y;
			if (result)
				available_points -= object.second;
			return result;
		}),
		objects_points_list.end());

	update_points_info();

	any_unsaved_changes = true;
}

void State_Level_Creator::render_objects()
{
	// Utility renderers

	auto renderer = [&](const auto& container, ITERATOR_RANGE visibility_range, const auto& predicate)
	{
		auto limit_left = camera.getCenter().x - camera.getSize().x * 0.5f - 64;
		auto limit_right = camera.getCenter().x + camera.getSize().x * 0.5f + 64;
		auto limit_up = camera.getCenter().y - camera.getSize().y * 0.5f - 64;
		auto limit_down = camera.getCenter().y + camera.getSize().y * 0.5f + 64;

		if (container.size() > unsorted_objects)
		{
			for (auto range = find_iterator_range(container, visibility_range, unsorted_objects); range.first != range.second; ++range.first)
				if (predicate(*range.first))
					(*range.first)->draw(render_texture);

			for (auto it = container.end() - unsorted_objects; it != container.end(); ++it)
				if (predicate(*it))
					if (auto pos = (*it)->get_original_position(); pos.x > limit_left && pos.x < limit_right && pos.y > limit_up && pos.y < limit_down)
						(*it)->draw(render_texture);
		}
		else
		{
			for (const auto& object : container)
				if (auto pos = object->get_original_position(); pos.x > limit_left && pos.x < limit_right && pos.y > limit_up && pos.y < limit_down)
					if (predicate(object))
						object->draw(render_texture);
		}
	};

	int current_layer = 0;
	auto condition_desired_layer = [&current_layer](const auto& obj)
	{
		return obj->get_layer() == static_cast<Tile_Layer_Component::LAYER>(current_layer);
	};

	auto condition_always_true = [](const auto& obj)
	{
		return true;
	};

	for (; current_layer < static_cast<int>(Tile_Layer_Component::LAYER::FAR_FRONT); ++current_layer)
	{
		renderer(decorative_tiles, ITERATOR_RANGE::VISIBLE_STATIONARY, condition_desired_layer);
		renderer(animated_tiles, ITERATOR_RANGE::VISIBLE_STATIONARY, condition_desired_layer);
	}

	// Actual rendering

	renderer(items, ITERATOR_RANGE::VISIBLE_STATIONARY, condition_always_true);

	renderer(solid_tiles, ITERATOR_RANGE::VISIBLE_STATIONARY, condition_always_true);
	renderer(destructible_tiles, ITERATOR_RANGE::VISIBLE_STATIONARY, condition_always_true);

	renderer(stationary_entities, ITERATOR_RANGE::VISIBLE_STATIONARY, condition_always_true);
	renderer(colliding_entities, ITERATOR_RANGE::VISIBLE_STATIONARY, condition_always_true);
	renderer(walking_entities, ITERATOR_RANGE::VISIBLE_STATIONARY, condition_always_true);

	for (; current_layer < static_cast<int>(Tile_Layer_Component::LAYER::COUNT); ++current_layer)
	{
		renderer(decorative_tiles, ITERATOR_RANGE::VISIBLE_STATIONARY, condition_desired_layer);
		renderer(animated_tiles, ITERATOR_RANGE::VISIBLE_STATIONARY, condition_desired_layer);
	}

	renderer(interactive_tiles, ITERATOR_RANGE::VISIBLE_STATIONARY, condition_always_true);

	player.draw(render_texture);
}

void State_Level_Creator::save_data()
{
	std::string temp_string = level_properties.path_main.string();
	while (temp_string.back() != '.')
		temp_string.pop_back();
	temp_string.pop_back();
	temp_string += "___temp.wgl";
	std::filesystem::path temp_path = temp_string;

	{
		std::ofstream ofs;
		ofs.open(std::filesystem::path(temp_path), std::ios::binary);
		if (!ofs.is_open())
			throw std::runtime_error("State_Level_Creator::save_data: temporary level save file could not be opened");
		auto guard = hct::make_guard_exit([&ofs]() { ofs.close(); });

		any_unsaved_changes = false;

		data_writer(ofs, static_cast<int32_t>(static_cast<int32_t>(player.get_position().x - 8.f + 32.f)));
		data_writer(ofs, static_cast<int32_t>(static_cast<int32_t>(player.get_position().y - 8.f + 32.f)));

		level_properties.max_points = available_points;
		level_properties.achieved_points = 0;
		level_properties.beaten = false;
		save_level_properties(level_properties);

		for (const auto& decorative_tile : decorative_tiles)
			decorative_tile->record(ofs);
		for (const auto& animated_tile : animated_tiles)
			animated_tile->record(ofs);
		for (const auto& solid_tile : solid_tiles)
			solid_tile->record(ofs);
		for (const auto& destructible_tile : destructible_tiles)
			destructible_tile->record(ofs);
		for (const auto& interactive_tile : interactive_tiles)
			interactive_tile->record(ofs);
		for (const auto& item : items)
			item->record(ofs);
		for (const auto& stationary_entity : stationary_entities)
			stationary_entity->record(ofs);
		for (const auto& colliding_entity : colliding_entities)
			colliding_entity->record(ofs);
		for (const auto& walking_entity : walking_entities)
			walking_entity->record(ofs);
	}

	std::remove(level_properties.path_main.string().c_str());
	std::rename(temp_path.string().c_str(), level_properties.path_main.string().c_str());
}

void State_Level_Creator::handle_events(const sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed)
		handle_keyboard_events(event);
	else if (event.type == sf::Event::MouseButtonPressed && event.key.code == sf::Mouse::Left)
	{
		if (!mouse_on_toolbelt() && !mouse_on_info_panel())
			handle_action_events(event);
		else if (mouse_on_toolbelt())
			handle_toolbelt_events(event);
	}
}

void State_Level_Creator::update(const sf::Time& dt)
{
	if (app_handle.any_scpp_awaiting() && !app_handle.any_states_queued())
	{
		pack.reset();
		pack = std::move(app_handle.retrieve_scpp());
		object_pack = dynamic_cast<SCPP_LCOS_Base*>(&(*pack));

		action = ACTIONS::OBJECT_INSERT;
		update_action_info();
	}

	for (const auto& animated_tile : animated_tiles)
		animated_tile->update_frame(dt);

	update_option_info();
	adjust_text_height(option_info);
}

void State_Level_Creator::render()
{
	auto& window = app_handle.get_shared_context().window;

	render_texture.setView(camera);
	render_texture.clear();

	render_texture.draw(background);
	render_objects();

	render_texture.display();

	cursor_highlight.draw_at(render_texture, sf::Vector2i(toolbelt.getPosition()), sf::Mouse::getPosition());

	render_texture.draw(toolbelt);
	render_texture.draw(toolbelt_info_panel);
	render_texture.draw(option_info);
	for (auto& text : description_infos)
		render_texture.draw(text);

	window.draw(render_sprite);
}

void State_Level_Creator::adjust_to_resolution()
{
	auto& window = app_handle.get_shared_context().window;
	auto& config = app_handle.get_shared_context().config_manager;

	render_texture.create(window.getSize().x, window.getSize().y);
	render_sprite.setTexture(render_texture.getTexture(), true);

	background.setPosition(camera.getCenter() - camera.getSize() * 0.5f);
	toolbelt.setPosition(camera.getCenter() - camera.getSize() * 0.5f);
	toolbelt_info_panel.setPosition(camera.getCenter().x + camera.getSize().x * 0.5f, camera.getCenter().y - camera.getSize().y * 0.5f);

	for (int a = 0; a < description_infos.size(); ++a)
	{
		description_infos[a].setPosition(toolbelt_info_panel.getPosition());
		description_infos[a].move(8.f - toolbelt_info_panel.getGlobalBounds().width, 8.f + 38.f * a);
	}
}

constexpr bool State_Level_Creator::request_previous_render()
{
	return false;
}

constexpr bool State_Level_Creator::uses_menu_background()
{
	return false;
}

constexpr bool State_Level_Creator::uses_dark_sheet()
{
	return false;
}

State* State_Level_Creator::make_state(App& app_handle)
{
	return new State_Level_Creator(app_handle);
}

State_Level_Creator::~State_Level_Creator()
{
	app_handle.get_shared_context().window.setMouseCursorVisible(false);
	app_handle.request_refresh();
}