#include "state_lcos_tile.h"

#include "lo_tiles.h"

// ----------------------------------------------------------------
// State_LCOS_Tile
// ----------------------------------------------------------------

State_LCOS_Tile::State_LCOS_Tile(App& arg_app_handle) :
	State_LCOS_Base(arg_app_handle),
	theme(dynamic_cast<SCPP_Level_Theme&>(*arg_app_handle.retrieve_scpp()).theme)
{
	auto& resources = app_handle.get_shared_context().resources;

	label.setString("Which Tile type?");
	set_center(label);

	options.setTexture(resources.textures.get(RESOURCE_ID::TEXTURES::LCOS_TILE));
	set_center(options);

	adjust_to_resolution();
}

int State_LCOS_Tile::cursor_index(sf::Vector2i pos)
{
	return (pos.x - static_cast<int>(options.getGlobalBounds().left)) / 64;
}

void State_LCOS_Tile::handle_events(const sf::Event& event)
{
	if (event.type == sf::Event::MouseButtonPressed && event.key.code == sf::Mouse::Left)
		if (auto pos = sf::Mouse::getPosition(); options.getGlobalBounds().contains(pos.x, pos.y))
		{
			switch (static_cast<OPTION>(cursor_index(pos)))
			{
			case(OPTION::DECORATIVE):
				app_handle.request_pop();
				app_handle.register_scpp(std::make_unique<SCPP_Level_Theme>(theme));
				app_handle.request_push(App::STATE::LEVEL_CREATOR_OBJECT_SELECTION_DECORATIVE_TILE);
				break;
			case(OPTION::ANIMATED):
				app_handle.request_pop();
				app_handle.request_push(App::STATE::LEVEL_CREATOR_OBJECT_SELECTION_ANIMATED_TILE);
				break;
			case(OPTION::SOLID):
				app_handle.request_pop();
				app_handle.register_scpp(std::make_unique<SCPP_Level_Theme>(theme));
				app_handle.request_push(App::STATE::LEVEL_CREATOR_OBJECT_SELECTION_SOLID_TILE);
				break;
			case(OPTION::DESTRUCTIBLE):
				app_handle.request_pop();
				app_handle.register_scpp(std::make_unique<SCPP_LCOS_Destructible_Tile>(0, 0, DESTRUCTIBLE_TILE_SUBTYPE::REGULAR));
				app_handle.register_scpp(std::make_unique<SCPP_Level_Theme>(theme));
				app_handle.request_push(App::STATE::LEVEL_CREATOR_OBJECT_SELECTION_DESTRUCTIBLE_TILE);
				break;
			case (OPTION::FRAGILE):
				app_handle.request_pop();
				app_handle.register_scpp(std::make_unique<SCPP_LCOS_Destructible_Tile>(0, 0, DESTRUCTIBLE_TILE_SUBTYPE::FRAGILE));
				app_handle.register_scpp(std::make_unique<SCPP_Level_Theme>(theme));
				app_handle.request_push(App::STATE::LEVEL_CREATOR_OBJECT_SELECTION_DESTRUCTIBLE_TILE);
				break;
			case (OPTION::INTERACTIVE):
				app_handle.request_pop();
				app_handle.request_push(App::STATE::LEVEL_CREATOR_OBJECT_SELECTION_INTERACTIVE_TILE);
				break;
			}
		}
}

void State_LCOS_Tile::update(const sf::Time& dt)
{
	auto pos = sf::Mouse::getPosition();

	if (auto pos = sf::Mouse::getPosition(); options.getGlobalBounds().contains(pos.x, pos.y))
	{
		switch (static_cast<OPTION>(cursor_index(pos)))
		{
		case (OPTION::DECORATIVE): info.setString("Decorative Tile"); break;
		case (OPTION::ANIMATED): info.setString("Animated Tile"); break;
		case (OPTION::SOLID): info.setString("Solid Tile"); break;
		case (OPTION::DESTRUCTIBLE): info.setString("Destructible Tile"); break;
		case (OPTION::FRAGILE): info.setString("Fragile Tile"); break;
		case (OPTION::INTERACTIVE): info.setString("Interactive Tile"); break;
		}

		adjust_text_height(info);
	}
	else
		info.setString("");
}

State* State_LCOS_Tile::make_state(App& app_handle)
{
	return new State_LCOS_Tile(app_handle);
}

// ----------------------------------------------------------------
// State_LCOS_Decorative_Tile
// ----------------------------------------------------------------

State_LCOS_Decorative_Tile::State_LCOS_Decorative_Tile(App& arg_app_handle) :
	State_LCOS_Base(arg_app_handle),
	theme(dynamic_cast<SCPP_Level_Theme&>(*arg_app_handle.retrieve_scpp()).theme)
{
	auto& resources = app_handle.get_shared_context().resources;

	label.setString("Which Decorative Tile?");
	set_center(label);

	options.setTexture(resources.textures.get(level_theme_to_tileset_id(theme)));
	set_center(options);

	adjust_to_resolution();
}

void State_LCOS_Decorative_Tile::handle_events(const sf::Event& event)
{
	if (event.type == sf::Event::MouseButtonPressed && event.key.code == sf::Mouse::Left)
		if (auto pos = sf::Mouse::getPosition(); options.getGlobalBounds().contains(pos.x, pos.y))
		{
			int index_x = (pos.x - static_cast<int>(options.getGlobalBounds().left)) / 32;
			int index_y = (pos.y - static_cast<int>(options.getGlobalBounds().top)) / 32;
			int index = index_x + 10 * index_y;

			if (index <= max_index_by_tileset_id(theme))
			{
				app_handle.request_pop();
				app_handle.request_push(App::STATE::LEVEL_CREATOR_OBJECT_SELECTION_TILE_LAYER);
				app_handle.register_scpp(std::make_unique<SCPP_LCOS_Decorative_Tile>(index_x, index_y, 0));
			}
		}
}

void State_LCOS_Decorative_Tile::update(const sf::Time& dt) {}

void State_LCOS_Decorative_Tile::render()
{
	State_LCOS_Base::render();

	if (options.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition())))
	{
		sf::Vector2i options_pos = sf::Vector2i(options.getPosition());
		options_pos.x -= options.getGlobalBounds().width * 0.5f;
		options_pos.y -= options.getGlobalBounds().height * 0.5f;
		cursor_highlight.draw_at(app_handle.get_shared_context().window, options_pos, sf::Mouse::getPosition() - options_pos);
	}
}

State* State_LCOS_Decorative_Tile::make_state(App& app_handle)
{
	return new State_LCOS_Decorative_Tile(app_handle);
}

// ----------------------------------------------------------------
// State_LCOS_Animated_Tile
// ----------------------------------------------------------------

State_LCOS_Animated_Tile::State_LCOS_Animated_Tile(App& arg_app_handle) :
	State_LCOS_Base(arg_app_handle),
	cursor_highlight(8 * 32, 32)
{
	auto& resources = app_handle.get_shared_context().resources;

	label.setString("Which Animated Tile?");
	set_center(label);

	options.setTexture(resources.textures.get(RESOURCE_ID::TEXTURES::ANIMATED_TILESET));
	set_center(options);

	adjust_to_resolution();
}

void State_LCOS_Animated_Tile::handle_events(const sf::Event& event)
{
	if (event.type == sf::Event::MouseButtonPressed && event.key.code == sf::Mouse::Left)
		if (auto pos = sf::Mouse::getPosition(); options.getGlobalBounds().contains(pos.x, pos.y))
		{
			int index_x = (pos.x - static_cast<int>(options.getGlobalBounds().left)) / 32 / 8;
			int index_y = (pos.y - static_cast<int>(options.getGlobalBounds().top)) / 32;
			int index = index_x + 3 * index_y;

			if (index < static_cast<int>(AID_TILE::COUNT))
			{
				app_handle.request_pop();
				app_handle.request_push(App::STATE::LEVEL_CREATOR_OBJECT_SELECTION_TILE_LAYER);
				app_handle.register_scpp(std::make_unique<SCPP_LCOS_Animated_Tile>(index, 0));
			}
		}
}

void State_LCOS_Animated_Tile::update(const sf::Time& dt) {}

void State_LCOS_Animated_Tile::render()
{
	State_LCOS_Base::render();

	if (options.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition())))
	{
		sf::Vector2i options_pos = sf::Vector2i(options.getPosition());
		options_pos.x -= options.getGlobalBounds().width * 0.5f;
		options_pos.y -= options.getGlobalBounds().height * 0.5f;
		cursor_highlight.draw_at(app_handle.get_shared_context().window, options_pos, sf::Mouse::getPosition() - options_pos);
	}
}

State* State_LCOS_Animated_Tile::make_state(App& app_handle)
{
	return new State_LCOS_Animated_Tile(app_handle);
}

// ----------------------------------------------------------------
// State_LCOS_Tile_Layer
// ----------------------------------------------------------------

State_LCOS_Tile_Layer::State_LCOS_Tile_Layer(App& arg_app_handle) :
	State_LCOS_Base(arg_app_handle)
{
	auto& resources = app_handle.get_shared_context().resources;

	label.setString("Which layer?");
	set_center(label);

	options.setTexture(resources.textures.get(RESOURCE_ID::TEXTURES::LCOS_TILE_LAYER));
	set_center(options);

	adjust_to_resolution();
}

int State_LCOS_Tile_Layer::cursor_index(sf::Vector2i pos)
{
	return (pos.x - static_cast<int>(options.getGlobalBounds().left)) / 64;
}

void State_LCOS_Tile_Layer::handle_events(const sf::Event& event)
{
	if (event.type == sf::Event::MouseButtonPressed && event.key.code == sf::Mouse::Left)
		if (auto pos = sf::Mouse::getPosition(); options.getGlobalBounds().contains(pos.x, pos.y))
		{
			int index = cursor_index(pos);

			if (index < static_cast<int>(Tile_Layer_Component::LAYER::COUNT))
			{
				auto pack = std::move(app_handle.retrieve_scpp());

				if (SCPP_LCOS_Decorative_Tile* casted_pack = dynamic_cast<SCPP_LCOS_Decorative_Tile*>(&(*pack)); casted_pack)
				{
					app_handle.request_pop();
					app_handle.register_scpp(std::make_unique<SCPP_LCOS_Decorative_Tile>(casted_pack->index_x, casted_pack->index_y, index));
				}
				else if (SCPP_LCOS_Animated_Tile* casted_pack = dynamic_cast<SCPP_LCOS_Animated_Tile*>(&(*pack)); casted_pack)
				{
					app_handle.request_pop();
					app_handle.register_scpp(std::make_unique<SCPP_LCOS_Animated_Tile>(casted_pack->index, index));
				}
				else
					throw std::runtime_error("State_LCOS_Tile_Layer::handle_events: an unexpected state communication parameter pack was received");
			}
		}
}

void State_LCOS_Tile_Layer::update(const sf::Time& dt)
{
	auto pos = sf::Mouse::getPosition();

	if (auto pos = sf::Mouse::getPosition(); options.getGlobalBounds().contains(pos.x, pos.y))
	{
		switch (static_cast<Tile_Layer_Component::LAYER>(cursor_index(pos)))
		{
		case (Tile_Layer_Component::LAYER::FARTHEST_BACK): info.setString("Farthest Back"); break;
		case (Tile_Layer_Component::LAYER::FAR_BACK): info.setString("Far Back"); break;
		case (Tile_Layer_Component::LAYER::BACK): info.setString("Back"); break;
		case (Tile_Layer_Component::LAYER::FAR_FRONT): info.setString("Far Front"); break;
		case (Tile_Layer_Component::LAYER::CLOSE_FRONT): info.setString("Close Front"); break;
		}

		adjust_text_height(info);
	}
	else
		info.setString("");
}

State* State_LCOS_Tile_Layer::make_state(App& app_handle)
{
	return new State_LCOS_Tile_Layer(app_handle);
}

// ----------------------------------------------------------------
// State_LCOS_Solid_Tile
// ----------------------------------------------------------------

State_LCOS_Solid_Tile::State_LCOS_Solid_Tile(App& arg_app_handle) :
	State_LCOS_Base(arg_app_handle),
	theme(dynamic_cast<SCPP_Level_Theme&>(*arg_app_handle.retrieve_scpp()).theme)
{
	auto& resources = app_handle.get_shared_context().resources;

	label.setString("Which Solid Tile?");
	set_center(label);

	options.setTexture(resources.textures.get(level_theme_to_tileset_id(theme)));
	set_center(options);

	adjust_to_resolution();
}

void State_LCOS_Solid_Tile::handle_events(const sf::Event& event)
{
	if (event.type == sf::Event::MouseButtonPressed && event.key.code == sf::Mouse::Left)
		if (auto pos = sf::Mouse::getPosition(); options.getGlobalBounds().contains(pos.x, pos.y))
		{
			int index_x = (pos.x - static_cast<int>(options.getGlobalBounds().left)) / 32;
			int index_y = (pos.y - static_cast<int>(options.getGlobalBounds().top)) / 32;
			int index = index_x + 10 * index_y;

			if (index <= max_index_by_tileset_id(theme))
			{
				app_handle.request_pop();
				app_handle.register_scpp(std::make_unique<SCPP_LCOS_Solid_Tile>(index_x, index_y));
			}
		}
}

void State_LCOS_Solid_Tile::update(const sf::Time& dt) {}

void State_LCOS_Solid_Tile::render()
{
	State_LCOS_Base::render();

	if (options.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition())))
	{
		sf::Vector2i options_pos = sf::Vector2i(options.getPosition());
		options_pos.x -= options.getGlobalBounds().width * 0.5f;
		options_pos.y -= options.getGlobalBounds().height * 0.5f;
		cursor_highlight.draw_at(app_handle.get_shared_context().window, options_pos, sf::Mouse::getPosition() - options_pos);
	}
}

State* State_LCOS_Solid_Tile::make_state(App& app_handle)
{
	return new State_LCOS_Solid_Tile(app_handle);
}

// ----------------------------------------------------------------
// State_LCOS_Destructible_Tile
// ----------------------------------------------------------------

State_LCOS_Destructible_Tile::State_LCOS_Destructible_Tile(App& arg_app_handle) :
	State_LCOS_Base(arg_app_handle),
	theme(dynamic_cast<SCPP_Level_Theme&>(*arg_app_handle.retrieve_scpp()).theme),
	pack(std::move(arg_app_handle.retrieve_scpp()))
{
	auto& resources = app_handle.get_shared_context().resources;
	SCPP_LCOS_Destructible_Tile& casted_pack = dynamic_cast<SCPP_LCOS_Destructible_Tile&>(*pack);

	label.setString(make_string("Which ", casted_pack.lo_subtype == DESTRUCTIBLE_TILE_SUBTYPE::REGULAR ? "Destructible" : "Fragile", " Tile?"));
	set_center(label);

	options.setTexture(resources.textures.get(level_theme_to_tileset_id(theme)));
	set_center(options);

	adjust_to_resolution();
}

void State_LCOS_Destructible_Tile::handle_events(const sf::Event& event)
{
	if (event.type == sf::Event::MouseButtonPressed && event.key.code == sf::Mouse::Left)
		if (auto pos = sf::Mouse::getPosition(); options.getGlobalBounds().contains(pos.x, pos.y))
		{
			int index_x = (pos.x - static_cast<int>(options.getGlobalBounds().left)) / 32;
			int index_y = (pos.y - static_cast<int>(options.getGlobalBounds().top)) / 32;
			int index = index_x + 10 * index_y;

			if (index <= max_index_by_tileset_id(theme))
			{
				SCPP_LCOS_Destructible_Tile& casted_pack = dynamic_cast<SCPP_LCOS_Destructible_Tile&>(*pack);
				app_handle.request_pop();
				app_handle.register_scpp(std::make_unique<SCPP_LCOS_Destructible_Tile>(index_x, index_y, casted_pack.lo_subtype));
			}
		}
}

void State_LCOS_Destructible_Tile::update(const sf::Time& dt) {}

void State_LCOS_Destructible_Tile::render()
{
	State_LCOS_Base::render();

	if (options.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition())))
	{
		sf::Vector2i options_pos = sf::Vector2i(options.getPosition());
		options_pos.x -= options.getGlobalBounds().width * 0.5f;
		options_pos.y -= options.getGlobalBounds().height * 0.5f;
		cursor_highlight.draw_at(app_handle.get_shared_context().window, options_pos, sf::Mouse::getPosition() - options_pos);
	}
}

State* State_LCOS_Destructible_Tile::make_state(App& app_handle)
{
	return new State_LCOS_Destructible_Tile(app_handle);
}

// ----------------------------------------------------------------
// State_LCOS_Interactive_Tile
// ----------------------------------------------------------------

State_LCOS_Interactive_Tile::State_LCOS_Interactive_Tile(App& arg_app_handle) :
	State_LCOS_Base(arg_app_handle)
{
	auto& resources = app_handle.get_shared_context().resources;

	label.setString("Which Interactive Tile type?");
	set_center(label);

	options.setTexture(resources.textures.get(RESOURCE_ID::TEXTURES::INTERACTIVE_TILESET));
	set_center(options);

	adjust_to_resolution();
}

int State_LCOS_Interactive_Tile::cursor_index(sf::Vector2i pos)
{
	return (pos.x - static_cast<int>(options.getGlobalBounds().left)) / 32;
}

INTERACTIVE_TILE_SUBTYPE State_LCOS_Interactive_Tile::transform_index(int index)
{
	switch (index)
	{
	case (0): return INTERACTIVE_TILE_SUBTYPE::WARP;
	case (1): return INTERACTIVE_TILE_SUBTYPE::WARP;
	case (2): return INTERACTIVE_TILE_SUBTYPE::FINISH_WARP;
	case (3): return INTERACTIVE_TILE_SUBTYPE::INFO_SIGN;
	case (4): return INTERACTIVE_TILE_SUBTYPE::HOOK;
	case (5): return INTERACTIVE_TILE_SUBTYPE::HAZARDOUS;
	case (6): return INTERACTIVE_TILE_SUBTYPE::WATER;
	case (7): return INTERACTIVE_TILE_SUBTYPE::CONVEYOR_BELT;
	case (8): return INTERACTIVE_TILE_SUBTYPE::CONVEYOR_BELT;
	case (9): return INTERACTIVE_TILE_SUBTYPE::ICE;
	case (10): return INTERACTIVE_TILE_SUBTYPE::COLD_AIR;
	case (11): return INTERACTIVE_TILE_SUBTYPE::HOT_AIR;
	case (12): return INTERACTIVE_TILE_SUBTYPE::ANTI_GRAVITY;
	default: throw std::runtime_error("State_LCOS_Interactive_Tile::transform_index: index out of range");
	}
}

void State_LCOS_Interactive_Tile::handle_events(const sf::Event& event)
{
	if (event.type == sf::Event::MouseButtonPressed && event.key.code == sf::Mouse::Left)
		if (auto pos = sf::Mouse::getPosition(); options.getGlobalBounds().contains(pos.x, pos.y))
		{
			int index = cursor_index(pos);

			switch (index)
			{
			case (0):
				app_handle.request_pop();
				app_handle.register_scpp(std::make_unique<SCPP_LCOS_Warp>(0));
				break;
			case (1):
				app_handle.request_pop();
				app_handle.request_push(App::STATE::LEVEL_CREATOR_OBJECT_SELECTION_COIN_WARP);
				break;
			case (3):
				app_handle.request_pop();
				app_handle.request_push(App::STATE::LEVEL_CREATOR_OBJECT_SELECTION_INFO_SIGN);
				break;
			case (7):
				app_handle.request_pop();
				app_handle.register_scpp(std::make_unique<SCPP_LCOS_Conveyor_Belt>(true));
				break;
			case (8):
				app_handle.request_pop();
				app_handle.register_scpp(std::make_unique<SCPP_LCOS_Conveyor_Belt>(false));
				break;
			default:
				if (index <= max_index)
				{
					app_handle.request_pop();
					app_handle.register_scpp(std::make_unique<SCPP_LCOS_Interactive_Tile>(transform_index(index)));
				}
			}
		}
}

void State_LCOS_Interactive_Tile::update(const sf::Time& dt)
{
	auto pos = sf::Mouse::getPosition();

	if (auto pos = sf::Mouse::getPosition(); options.getGlobalBounds().contains(pos.x, pos.y))
	{
		int index = cursor_index(pos);

		info.setString(index_to_interactive_tile_type_name(static_cast<int>(transform_index(index))));

		switch (index)
		{
		case (1): info.setString(info.getString() + " (Coin variant)"); break;
		case (7): info.setString(info.getString() + " (moving left)"); break;
		case (8): info.setString(info.getString() + " (moving right)"); break;
		}

		adjust_text_height(info);
	}
	else
		info.setString("");
}

State* State_LCOS_Interactive_Tile::make_state(App& app_handle)
{
	return new State_LCOS_Interactive_Tile(app_handle);
}

// ----------------------------------------------------------------
// State_LCOS_Info_Sign
// ----------------------------------------------------------------

State_LCOS_Info_Sign::State_LCOS_Info_Sign(App& arg_app_handle) :
	State(arg_app_handle, arg_app_handle.get_shared_context()),
	text_field(arg_app_handle.get_shared_context().resources, 64)
{
	label = make_label(context.resources.fonts.get(RESOURCE_ID::FONTS::MAIN), "Input the text you want to be shown", { 0, 0 }, 50);

	adjust_to_resolution();
}

void State_LCOS_Info_Sign::handle_events(const sf::Event& event)
{
	text_field.handle_input(event);
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter)
	{
		app_handle.register_scpp(std::make_unique<SCPP_LCOS_Info_Sign>(text_field.get_string()));
		app_handle.request_pop();
	}
}

void State_LCOS_Info_Sign::update(const sf::Time& dt)
{
	text_field.update(dt);
}

void State_LCOS_Info_Sign::render()
{
	text_field.draw(context.window);
	context.window.draw(label);
}

void State_LCOS_Info_Sign::adjust_to_resolution()
{
	label.setPosition(sf::Vector2f(context.view.getSize().x * 0.5f, context.view.getSize().y * 0.1f));
	text_field.set_position(context.view.getCenter());
}

constexpr bool State_LCOS_Info_Sign::request_previous_render()
{
	return false;
}

constexpr bool State_LCOS_Info_Sign::uses_menu_background()
{
	return true;
}

constexpr bool State_LCOS_Info_Sign::uses_dark_sheet()
{
	return false;
}

State* State_LCOS_Info_Sign::make_state(App& app_handle)
{
	return new State_LCOS_Info_Sign(app_handle);
}

// ----------------------------------------------------------------
// State_LCOS_Coin_Warp
// ----------------------------------------------------------------

State_LCOS_Coin_Warp::State_LCOS_Coin_Warp(App& arg_app_handle) :
	State(arg_app_handle, arg_app_handle.get_shared_context()),
	text_field(arg_app_handle.get_shared_context().resources)
{
	label = make_label(context.resources.fonts.get(RESOURCE_ID::FONTS::MAIN), "Input the required amount of Coins (10/20/50 only)", { 0, 0 }, 40);

	adjust_to_resolution();
}

void State_LCOS_Coin_Warp::handle_events(const sf::Event& event)
{
	text_field.handle_input(event);
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter)
	{
		int result = 0;

		try { result = std::stoi(text_field.get_string()); }
		catch (...) { return; }

		if (result != 10 && result != 20 && result != 50)
			return;

		app_handle.register_scpp(std::make_unique<SCPP_LCOS_Warp>(result));
		app_handle.request_pop();
	}
}

void State_LCOS_Coin_Warp::update(const sf::Time& dt)
{
	text_field.update(dt);
}

void State_LCOS_Coin_Warp::render()
{
	text_field.draw(context.window);
	context.window.draw(label);
}

void State_LCOS_Coin_Warp::adjust_to_resolution()
{
	label.setPosition(sf::Vector2f(context.view.getSize().x * 0.5f, context.view.getSize().y * 0.1f));
	text_field.set_position(context.view.getCenter());
}

constexpr bool State_LCOS_Coin_Warp::request_previous_render()
{
	return false;
}

constexpr bool State_LCOS_Coin_Warp::uses_menu_background()
{
	return true;
}

constexpr bool State_LCOS_Coin_Warp::uses_dark_sheet()
{
	return false;
}

State* State_LCOS_Coin_Warp::make_state(App& app_handle)
{
	return new State_LCOS_Coin_Warp(app_handle);
}