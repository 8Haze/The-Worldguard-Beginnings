#include "app.h"

#include "state_main_menu.h"
#include "state_settings.h"
#include "state_settings_controls.h"
#include "state_settings_video.h"
#include "state_settings_audio.h"
#include "state_settings_progress_reset.h"
#include "state_level_creator_select.h"
#include "state_level_creator_create.h"
#include "state_level_creator_edit.h"
#include "state_level_creator_remove.h"
#include "state_level_creator.h"
#include "state_level_creator_alert.h"
#include "state_lcos_tile.h"
#include "state_lcos_item.h"
#include "state_lcos_entity.h"
#include "state_mode_select.h"
#include "state_campaign.h"
#include "state_user_level_select.h"
#include "state_gameplay.h"
#include "state_pause.h"
#include "state_game_over.h"
#include "state_victory.h"
#include "state_error.h"

// ----------------------------------------------------------------
// App_Shared_Context
// ----------------------------------------------------------------

App_Shared_Context::App_Shared_Context() : music_manager(config_manager), sfx_manager(resources.sounds, config_manager) {}

// ----------------------------------------------------------------
// App
// ----------------------------------------------------------------

App::App(STATE arg_init_state = STATE::MAIN_MENU)
{
	sf::Vector2u resolution = { context.config_manager.get(Config_Manager::DATA::RES_WIDTH), context.config_manager.get(Config_Manager::DATA::RES_HEIGHT) };
	context.window.create(sf::VideoMode(resolution.x, resolution.y), "The Worldguard Beginnings", sf::Style::Fullscreen);
	context.window.setMouseCursorVisible(false);
	context.window.setPosition(sf::Vector2i(0, 0));
	context.window.setVerticalSyncEnabled(true);
	context.view.setSize(static_cast<float>(resolution.x), static_cast<float>(resolution.y));
	context.view.setCenter(resolution.x * 0.5f, resolution.y * 0.5f);

	auto& bg_texture = context.resources.textures.get(RESOURCE_ID::TEXTURES::MENU_BG);
	bg_texture.setRepeated(true);
	menu_background_up.setTexture(bg_texture);
	menu_background_up.setTextureRect(sf::IntRect(0, 0, resolution.x, bg_texture.getSize().y));
	menu_background_up.setPosition(0.f, 0.f);
	menu_background_down.setTexture(bg_texture);
	menu_background_down.setTextureRect(sf::IntRect(0, bg_texture.getSize().y - 1, resolution.x, -static_cast<int>(bg_texture.getSize().y)));
	menu_background_down.setOrigin(0.f, static_cast<float>(bg_texture.getSize().y));
	menu_background_down.setPosition(0.f, static_cast<float>(resolution.y));

	dark_sheet.setSize(context.view.getSize());
	dark_sheet.setPosition(0.f, 0.f);
	dark_sheet.setFillColor(sf::Color(0, 0, 0, 225));

	factories.emplace(STATE::MAIN_MENU, State_Main_Menu::make_state);

	factories.emplace(STATE::SETTINGS, State_Settings::make_state);
	factories.emplace(STATE::SETTINGS_CONTROLS, State_Settings_Controls::make_state);
	factories.emplace(STATE::SETTINGS_VIDEO, State_Settings_Video::make_state);
	factories.emplace(STATE::SETTINGS_AUDIO, State_Settings_Audio::make_state);
	factories.emplace(STATE::SETTINGS_PROGRESS_RESET, State_Settings_Progress_Reset::make_state);
	factories.emplace(STATE::SETTINGS_PROGRESS_RESET_DENY, State_Settings_Progress_Reset_Deny::make_state);
	factories.emplace(STATE::SETTINGS_PROGRESS_RESET_PROCEDURE, State_Settings_Progress_Reset_Procedure::make_state);

	factories.emplace(STATE::LEVEL_CREATOR_SELECT, State_Level_Creator_Select::make_state);
	factories.emplace(STATE::LEVEL_CREATOR_CREATE, State_Level_Creator_Create::make_state);
	factories.emplace(STATE::LEVEL_CREATOR_EDIT, State_Level_Creator_Edit::make_state);
	factories.emplace(STATE::LEVEL_CREATOR_REMOVE, State_Level_Creator_Remove::make_state);
	factories.emplace(STATE::LEVEL_CREATOR_REMOVE_CONFIRM, State_Level_Creator_Remove_Confirm::make_state);
	factories.emplace(STATE::LEVEL_CREATOR_REMOVE_PROCEDURE, State_Level_Creator_Remove_Procedure::make_state);

	factories.emplace(STATE::LEVEL_CREATOR, State_Level_Creator::make_state);
	factories.emplace(STATE::LEVEL_CREATOR_ALERT, State_Level_Creator_Alert::make_state);
	factories.emplace(STATE::LEVEL_CREATOR_OBJECT_SELECTION_DECORATIVE_TILE, State_LCOS_Decorative_Tile::make_state);
	factories.emplace(STATE::LEVEL_CREATOR_OBJECT_SELECTION_ANIMATED_TILE, State_LCOS_Animated_Tile::make_state);
	factories.emplace(STATE::LEVEL_CREATOR_OBJECT_SELECTION_TILE_LAYER, State_LCOS_Tile_Layer::make_state);
	factories.emplace(STATE::LEVEL_CREATOR_OBJECT_SELECTION_SOLID_TILE, State_LCOS_Solid_Tile::make_state);
	factories.emplace(STATE::LEVEL_CREATOR_OBJECT_SELECTION_DESTRUCTIBLE_TILE, State_LCOS_Destructible_Tile::make_state);
	factories.emplace(STATE::LEVEL_CREATOR_OBJECT_SELECTION_INTERACTIVE_TILE, State_LCOS_Interactive_Tile::make_state);
	factories.emplace(STATE::LEVEL_CREATOR_OBJECT_SELECTION_COIN_WARP, State_LCOS_Coin_Warp::make_state);
	factories.emplace(STATE::LEVEL_CREATOR_OBJECT_SELECTION_INFO_SIGN, State_LCOS_Info_Sign::make_state);
	factories.emplace(STATE::LEVEL_CREATOR_OBJECT_SELECTION_TILE, State_LCOS_Tile::make_state);
	factories.emplace(STATE::LEVEL_CREATOR_OBJECT_SELECTION_ITEM, State_LCOS_Item::make_state);
	factories.emplace(STATE::LEVEL_CREATOR_OBJECT_SELECTION_FOOD, State_LCOS_Food::make_state);
	factories.emplace(STATE::LEVEL_CREATOR_OBJECT_SELECTION_GEM, State_LCOS_Gem::make_state);
	factories.emplace(STATE::LEVEL_CREATOR_OBJECT_SELECTION_COIN, State_LCOS_Coin::make_state);
	factories.emplace(STATE::LEVEL_CREATOR_OBJECT_SELECTION_LIFEUP, State_LCOS_Lifeup::make_state);
	factories.emplace(STATE::LEVEL_CREATOR_OBJECT_SELECTION_ENTITY, State_LCOS_Entity::make_state);
	factories.emplace(STATE::LEVEL_CREATOR_OBJECT_SELECTION_THORN, State_LCOS_Thorn::make_state);

	factories.emplace(STATE::MODE_SELECT, State_Mode_Select::make_state);
	factories.emplace(STATE::CAMPAIGN, State_Campaign::make_state);
	factories.emplace(STATE::USER_LEVEL_SELECT, State_User_Level_Select::make_state);

	factories.emplace(STATE::GAMEPLAY, State_Gameplay::make_state);
	factories.emplace(STATE::PAUSE, State_Pause::make_state);
	factories.emplace(STATE::PAUSE_LEAVE, State_Pause_Leave::make_state);
	factories.emplace(STATE::GAME_OVER, State_Game_Over::make_state);
	factories.emplace(STATE::VICTORY, State_Victory::make_state);

	factories.emplace(STATE::ERROR, State_Error::make_state);

	states_stack.emplace_back(factories.find(arg_init_state)->second(*this), arg_init_state);
}

void App::perform_queued_actions()
{
	while (!actions_queue.empty())
	{
		ACTION id = actions_queue.front();
		actions_queue.pop();

		switch (id)
		{
		case(ACTION::PUSH):
		{
			auto state = states_queue.front();
			states_queue.pop();
			states_stack.emplace_back(factories.find(state)->second(*this), state);
			break;
		}
		case(ACTION::POP):
		{
			if (!states_stack.empty())
				states_stack.pop_back();
			break;
		}
		case(ACTION::CLEAR):
		{
			states_stack.clear();
			break;
		}
		case(ACTION::REFRESH):
		{
			if (!states_stack.empty())
			{
				App::STATE state_id = states_stack.back().second;
				states_stack.pop_back();
				states_stack.emplace_back(factories.find(state_id)->second(*this), state_id);
			}
		}
		}
	}
}

void App::handle_events()
{
	sf::Event event;
	while (context.window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			this->request_clear();

		states_stack.back().first->handle_events(event);
		break;
	}
}

void App::update()
{
	while (time_passed > time_per_frame)
	{
		time_passed -= time_per_frame;
		states_stack.back().first->update(time_per_frame);

		Chromatic_Sprite::update_color(time_per_frame);
	}
}

void App::draw()
{
	context.window.setView(context.view);
	context.window.clear();

	if (states_stack.back().first->uses_menu_background())
	{
		menu_background_up.set_updated_color();
		menu_background_down.set_updated_color();
		context.window.draw(menu_background_up);
		context.window.draw(menu_background_down);
	}

	size_t index, max_index;
	index = max_index = states_stack.size() - 1;

	while (index > 0 && states_stack[index].first->request_previous_render())
		--index;

	for (; index <= max_index; ++index)
	{
		if (states_stack[index].first->uses_dark_sheet())
			context.window.draw(dark_sheet);
		states_stack[index].first->render();
	}
	context.window.display();
}

App_Shared_Context& App::get_shared_context() noexcept
{
	return context;
}

const App_Shared_Context& App::get_shared_context() const noexcept
{
	return context;
}

bool App::any_actions_queued() const noexcept
{
	return !actions_queue.empty();
}

bool App::any_states_queued() const noexcept
{
	return !states_queue.empty();
}

bool App::state_exists(STATE state_id) const noexcept
{
	for (const auto& entry : states_stack)
		if (entry.second == state_id)
			return true;
	return false;
}

void App::request_push(STATE state_id)
{
	for (const auto& entry : states_queue)
		if (entry == state_id)
			return;

	actions_queue.emplace(ACTION::PUSH);
	states_queue.emplace(state_id);
}

void App::request_pop()
{
	actions_queue.emplace(ACTION::POP);
}

void App::request_clear()
{
	actions_queue.emplace(ACTION::CLEAR);
}

void App::request_refresh()
{
	actions_queue.emplace(ACTION::REFRESH);
}

void App::register_scpp(std::unique_ptr<SCPP_Base>&& scpp)
{
	scpp_stack.push(std::move(scpp));
}

std::unique_ptr<SCPP_Base> App::retrieve_scpp()
{
	auto result = std::move(scpp_stack.top());
	scpp_stack.pop();
	return result;
}

bool App::any_scpp_awaiting()
{
	return !scpp_stack.empty();
}

void App::signal_resolution_change()
{
	sf::Vector2u resolution = { context.config_manager.get(Config_Manager::DATA::RES_WIDTH), context.config_manager.get(Config_Manager::DATA::RES_HEIGHT) };
	const auto* bg_texture = menu_background_up.getTexture();

	context.window.create(sf::VideoMode(resolution.x, resolution.y), "The Worldguard Beginnings", sf::Style::Fullscreen);
	context.window.setMouseCursorVisible(false);
	context.view.setSize(static_cast<float>(resolution.x), static_cast<float>(resolution.y));
	context.view.setCenter(static_cast<float>(resolution.x) * 0.5f, static_cast<float>(resolution.y) * 0.5f);

	menu_background_up.setTextureRect(sf::IntRect(0, 0, resolution.x, bg_texture->getSize().y));
	menu_background_down.setTextureRect(sf::IntRect(0, bg_texture->getSize().y - 1, resolution.x, -static_cast<int>(bg_texture->getSize().y)));
	menu_background_down.setPosition(0.f, static_cast<float>(resolution.y));

	dark_sheet.setSize(context.view.getSize());
	dark_sheet.setPosition(0.f, 0.f);

	for (auto& x : states_stack)
		x.first->adjust_to_resolution();
}

void App::run()
{
	sf::Clock clock;

	while (context.window.isOpen() && !states_stack.empty())
	{
		time_passed += clock.restart();

		handle_events();
		if (!context.window.isOpen())
			break;
		update();
		draw();
		perform_queued_actions();
	}
}