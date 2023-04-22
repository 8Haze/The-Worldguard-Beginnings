#ifndef _APP_H_
#define _APP_H_

#include <map>

#include "hct/stack.h"
#include "hct/queue.h"

#include "utility.h"
#include "state.h"
#include "resources.h"
#include "config_manager.h"
#include "controls_manager.h"
#include "music_manager.h"
#include "sfx_manager.h"
#include "chromatic_sprite.h"

class App;

struct App_Shared_Context
{
private:

	friend class App;

	App_Shared_Context();

public:

	Resources resources;
	sf::RenderWindow window;
	sf::View view;
	Config_Manager config_manager;
	Controls_Manager controls_manager;
	Music_Manager music_manager;
	SFX_Manager sfx_manager;

};

class App
{
public:

	enum class STATE
	{
		// The program's entry point state
		MAIN_MENU,

		// All settings menus states
		SETTINGS,
		SETTINGS_CONTROLS,
		SETTINGS_VIDEO,
		SETTINGS_AUDIO,
		SETTINGS_PROGRESS_RESET,
		SETTINGS_PROGRESS_RESET_DENY,
		SETTINGS_PROGRESS_RESET_PROCEDURE,

		// Level Creator menus states
		LEVEL_CREATOR_SELECT,
		LEVEL_CREATOR_CREATE,
		LEVEL_CREATOR_EDIT,
		LEVEL_CREATOR_REMOVE,
		LEVEL_CREATOR_REMOVE_CONFIRM,
		LEVEL_CREATOR_REMOVE_PROCEDURE,

		// Level Creator functionality states
		LEVEL_CREATOR,
		LEVEL_CREATOR_ALERT,
		LEVEL_CREATOR_OBJECT_SELECTION_TILE,
		LEVEL_CREATOR_OBJECT_SELECTION_DECORATIVE_TILE,
		LEVEL_CREATOR_OBJECT_SELECTION_ANIMATED_TILE,
		LEVEL_CREATOR_OBJECT_SELECTION_TILE_LAYER,
		LEVEL_CREATOR_OBJECT_SELECTION_SOLID_TILE,
		LEVEL_CREATOR_OBJECT_SELECTION_DESTRUCTIBLE_TILE,
		LEVEL_CREATOR_OBJECT_SELECTION_INTERACTIVE_TILE,
		LEVEL_CREATOR_OBJECT_SELECTION_COIN_WARP,
		LEVEL_CREATOR_OBJECT_SELECTION_INFO_SIGN,
		LEVEL_CREATOR_OBJECT_SELECTION_ITEM,
		LEVEL_CREATOR_OBJECT_SELECTION_FOOD,
		LEVEL_CREATOR_OBJECT_SELECTION_GEM,
		LEVEL_CREATOR_OBJECT_SELECTION_COIN,
		LEVEL_CREATOR_OBJECT_SELECTION_LIFEUP,
		LEVEL_CREATOR_OBJECT_SELECTION_ENTITY,
		LEVEL_CREATOR_OBJECT_SELECTION_THORN,

		// Game mode menus
		MODE_SELECT,
		CAMPAIGN, 
		USER_LEVEL_SELECT,

		// Gameplay functionality states
		GAMEPLAY,
		PAUSE,
		PAUSE_LEAVE,
		GAME_OVER,
		VICTORY,

		// Error-handling state
		ERROR
	};

private:

	enum class ACTION
	{
		PUSH, 
		POP, 
		CLEAR, 
		REFRESH 
	};

	typedef State* (*Factory)(App&);

	inline static const sf::Time time_per_frame = sf::seconds(1.f / 60.f);

	App_Shared_Context context;

	std::map<STATE, Factory> factories;

	hct::dynarray<std::pair<std::unique_ptr<State>, STATE>> states_stack;
	hct::queue<STATE> states_queue;
	hct::queue<ACTION> actions_queue;
	hct::stack<std::unique_ptr<SCPP_Base>> scpp_stack;

	Chromatic_Sprite menu_background_up;
	Chromatic_Sprite menu_background_down;
	sf::RectangleShape dark_sheet;

	sf::Time time_passed = sf::Time::Zero;

	void perform_queued_actions();
	void handle_events();
	void update();
	void draw();

public:

	App(STATE arg_init_state);

	App_Shared_Context& get_shared_context() noexcept;
	const App_Shared_Context& get_shared_context() const noexcept;
	bool any_states_queued() const noexcept;
	bool any_actions_queued() const noexcept;
	bool state_exists(STATE state_id) const noexcept;
	
	void request_push(STATE state_id);
	void request_pop();
	void request_clear();
	void request_refresh();

	void register_scpp(std::unique_ptr<SCPP_Base>&& scpp);
	std::unique_ptr<SCPP_Base> retrieve_scpp();
	bool any_scpp_awaiting();

	void signal_resolution_change();

	void run();

};

#endif // _APP_H_