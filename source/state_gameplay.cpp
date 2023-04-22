#include "state_gameplay.h"

#include "hct/scope_guard.h"
#include "hct/sort.h"

State_Gameplay::State_Gameplay(App& arg_app_handle) :
	State(arg_app_handle, arg_app_handle.get_shared_context()),
	Level_Content(arg_app_handle),
	level_background(arg_app_handle.get_shared_context().resources, arg_app_handle.get_shared_context().config_manager, level_properties.theme),
	scores_panel(arg_app_handle.get_shared_context().resources),
	player_lifebar(arg_app_handle.get_shared_context().resources),
	temperature_bar(arg_app_handle.get_shared_context().resources)
{
	auto& music = app_handle.get_shared_context().music_manager;
	auto& font = context.resources.fonts.get(RESOURCE_ID::FONTS::MAIN);

	music.set_pause(true);
	player_read_text = make_label(font, {}, {}, 20);
	player_read_text.setScale(0.5f, 0.5f);

	load_everything_from_file(app_handle);
	tidy_objects();

	adjust_to_resolution();

	int music_id = static_cast<int>(level_properties.theme) - 1 + static_cast<int>(Music_Manager::ID::VERDANT);
	music.play_track(static_cast<Music_Manager::ID>(music_id));
	music.set_pause(false);
}

void State_Gameplay::tidy_objects()
{
	decorative_tiles.shrink_to_fit();
	animated_tiles.shrink_to_fit();
	solid_tiles.shrink_to_fit();
	destructible_tiles.shrink_to_fit();
	interactive_tiles.shrink_to_fit();
	items.shrink_to_fit();
	stationary_entities.shrink_to_fit();
	colliding_entities.shrink_to_fit();
	walking_entities.shrink_to_fit();

	sort_objects();

	for (auto& walking_entity : walking_entities)
		walking_entity->find_walking_limits(solid_tiles, destructible_tiles);
}

void State_Gameplay::update_objects(const sf::Time& dt)
{
	auto& resources = app_handle.get_shared_context().resources;

	for (const auto& x : animated_tiles)
		x->update_frame(dt);
	for (const auto& x : destructible_tiles)
		x->update(resources, dt);
	for (const auto& x : items)
		x->update(resources, dt);
	for (const auto& x : walking_entities)
		x->update(resources, player, dt);
	for (const auto& x : stationary_entities)
		x->update(resources, player, dt);
	for (const auto& x : colliding_entities)
		x->update(resources, player, dt, solid_tiles, destructible_tiles);

	for (auto range = find_iterator_range(interactive_tiles, ITERATOR_RANGE::CLOSE_TO_PLAYER); range.first != range.second; ++range.first)
		(*range.first)->player_interaction(player);
	for (auto range = find_iterator_range(items, ITERATOR_RANGE::CLOSE_TO_PLAYER); range.first != range.second; ++range.first)
		(*range.first)->player_interaction(player);
}

void State_Gameplay::update_player(const sf::Time& dt)
{
	player.update(dt, solid_tiles, destructible_tiles);

	if (player.get_position().y > camera.down_lim + 48)
		player.take_damage(player.get_health());

	if (!player.is_alive())
		app_handle.get_shared_context().music_manager.set_pause(true);

	if (player.ready_for_game_over())
	{
		app_handle.request_push(App::STATE::GAME_OVER);
		app_handle.register_scpp(std::make_unique<SCPP_Game_Over>(level_properties));
	}

	if (player.ready_for_victory())
	{
		app_handle.request_push(App::STATE::VICTORY);
		app_handle.register_scpp(std::make_unique<SCPP_Victory>(level_properties, player.get_points()));
		victory = true;
	}
}

void State_Gameplay::update_static_timers(const sf::Time& dt)
{
	Cannon::update_timer(dt);
	Thorn_Spear_Root::update_timer(dt);
	Bubble_Machine::update_timer(dt);
	Flamethrower::update_timer(dt);
	Electric_Gate_Device::update_timer(dt);
	Wind_Blaster::update_timer(dt);
	Rock_Disposer::update_timer(dt);
	Living_Spikes::update_timer(dt);
	Ice_Launcher::update_timer(dt);
	Caster::update_timer(dt);
	Jungle_Boomeranger::update_timer(dt);
	Aerial_Bombardier::update_timer(dt);
	Rock_Hurler::update_timer(dt);
	Crimson_Lasher::update_timer(dt);
}

void State_Gameplay::render_objects()
{
	// Utility renderers

	auto renderer = [&](const auto& container, ITERATOR_RANGE visibility_range, const auto& predicate)
	{
		for (auto range = find_iterator_range(container, visibility_range); range.first != range.second; ++range.first)
			if (predicate(*range.first))
				(*range.first)->draw(render_texture);
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

	player.draw(render_texture);

	renderer(solid_tiles, ITERATOR_RANGE::VISIBLE_STATIONARY, condition_always_true);
	renderer(destructible_tiles, ITERATOR_RANGE::VISIBLE_STATIONARY, condition_always_true);

	renderer(stationary_entities, ITERATOR_RANGE::VISIBLE_STATIONARY, condition_always_true);
	renderer(colliding_entities, ITERATOR_RANGE::VISIBLE_COLLIDING, condition_always_true);
	renderer(walking_entities, ITERATOR_RANGE::VISIBLE_WALKING, condition_always_true);

	for (; current_layer < static_cast<int>(Tile_Layer_Component::LAYER::COUNT); ++current_layer)
	{
		renderer(decorative_tiles, ITERATOR_RANGE::VISIBLE_STATIONARY, condition_desired_layer);
		renderer(animated_tiles, ITERATOR_RANGE::VISIBLE_STATIONARY, condition_desired_layer);
	}
}

void State_Gameplay::handle_events(const sf::Event& event)
{
	player.handle_controls(event, app_handle.get_shared_context().controls_manager);
	if (event.type == sf::Event::KeyPressed)
		switch (event.key.code)
		{
		case (sf::Keyboard::Escape): player.reset_controls(); app_handle.request_push(App::STATE::PAUSE); break;
		}
}

void State_Gameplay::update(const sf::Time& dt)
{
	camera.set_center_within_bounds(player.get_sprite().getPosition());

	level_background.update(camera);

	scores_panel.update(camera, dt, player.get_points(), player.get_coins());
	player_lifebar.update(camera, dt, player.get_health(), player.get_max_health());
	temperature_bar.update(camera, dt, player.get_temperature());
	player_read_text.setString(player.retrieve_visible_text());
	player_read_text.setPosition(camera.getCenter().x, camera.getCenter().y - camera.getSize().y * 0.45f);
	adjust_text_height(player_read_text, false);

	update_static_timers(dt);
	update_objects(dt);
	update_player(dt);
}

void State_Gameplay::render()
{
	auto& window = app_handle.get_shared_context().window;

	render_texture.setView(camera);
	render_texture.clear();
	
	level_background.draw(render_texture);
	render_objects();

	render_texture.display();

	scores_panel.draw(render_texture);
	player_lifebar.draw(render_texture);
	temperature_bar.draw(render_texture);
	if (!player_read_text.getString().isEmpty())
		render_texture.draw(player_read_text);

	window.draw(render_sprite);
}

void State_Gameplay::adjust_to_resolution()
{
	auto& window = app_handle.get_shared_context().window;
	auto& config = app_handle.get_shared_context().config_manager;

	camera.setSize(static_cast<float>(config.get(Config_Manager::DATA::RES_WIDTH)) * 0.5f,
		static_cast<float>(config.get(Config_Manager::DATA::RES_HEIGHT)) * 0.5f);

	render_texture.create(window.getSize().x, window.getSize().y);
	render_sprite.setTexture(render_texture.getTexture(), true);

	level_background.adjust(config);
}

constexpr bool State_Gameplay::request_previous_render()
{
	return false;
}

constexpr bool State_Gameplay::uses_menu_background()
{
	return false;
}

constexpr bool State_Gameplay::uses_dark_sheet()
{
	return false;
}

State* State_Gameplay::make_state(App& app_handle)
{
	return new State_Gameplay(app_handle);
}

State_Gameplay::~State_Gameplay()
{
	app_handle.get_shared_context().music_manager.pop_track();
	if (victory)
		app_handle.request_refresh();
}