#ifndef _STATE_LEVEL_CREATOR_H_
#define _STATE_LEVEL_CREATOR_H_

#include "app.h"
#include "gui_dialogue_box.h"
#include "cursor_highlight.h"
#include "level_content.h"

class State_Level_Creator final : public State, public Level_Content
{
private:

	enum class ACTIONS
	{
		OBJECT_INSERT,
		OBJECT_INSERT_BONUS,
		OBJECT_REMOVE,
		MASS_OBJECT_INSERT_1,
		MASS_OBJECT_INSERT_2,
		MASS_OBJECT_REMOVE_1,
		MASS_OBJECT_REMOVE_2,
		PLAYER_START_POSITION_SETTING,
		COUNT
	}; // keep COUNT last

	enum class TOOLBELT_OPTIONS
	{
		LEFT,
		UP,
		RIGHT,
		DOWN,
		TILES,
		ITEMS,
		ENTITIES,
		MASS_INSERT,
		REMOVE,
		MASS_REMOVE,
		PLAYER_START_POSITION,
		SORT,
		SAVE,
		COUNT
	}; // keep COUNT last

	inline static const size_t description_infos_count = 6;

	sf::Sprite background;
	sf::Sprite toolbelt;
	sf::Sprite toolbelt_info_panel;

	std::array<sf::Text, description_infos_count> description_infos;
	sf::Text option_info;

	std::unique_ptr<SCPP_Base> pack;
	SCPP_LCOS_Base* object_pack = nullptr;

	hct::dynarray<std::pair<std::pair<int32_t, int32_t>, int>> objects_points_list;

	ACTIONS action = ACTIONS::COUNT;

	std::stringstream objects_stream;

	std::pair<sf::Vector2<int32_t>, sf::Vector2<int32_t>> mass_operation_rectangle;
	sf::Vector2<int32_t> bonus_click_position;

	Cursor_Highlight cursor_highlight;

	int x_shift = 0;
	int y_shift = 0;

	int available_points = 0;

	int unsorted_objects = 0;

	bool any_unsaved_changes = false;

	State_Level_Creator(App& arg_app_handle);

	void update_action_info();
	void update_points_info();
	void update_option_info();

	void move_everything(const sf::Keyboard::Key);

	void initialize_objects_points_list();

	sf::Vector2f key_to_move_vector(const sf::Keyboard::Key key);

	bool mouse_on_toolbelt();
	bool mouse_on_info_panel();

	TOOLBELT_OPTIONS highlighted_toolbelt_option();

	void handle_keyboard_events(const sf::Event& event);
	void handle_action_events(const sf::Event& event);
	void handle_toolbelt_events(const sf::Event& event);

	void perform_object_insert(int x, int y);
	void perform_object_insert_bonus(int x, int y);
	void perform_object_remove(int x, int y);
	void perform_mass_object_insert_1(int x, int y);
	void perform_mass_object_insert_2(int x, int y);
	void perform_mass_object_remove_1(int x, int y);
	void perform_mass_object_remove_2(int x, int y);
	void perform_player_start_position_setting(int x, int y);

	void record_object(int32_t x, int32_t y);
	void remove_objects(int32_t x, int32_t y);
	void render_objects();

	void save_data();

public:

	void handle_events(const sf::Event& event) override;
	void update(const sf::Time& dt) override;
	void render() override;
	void adjust_to_resolution() override;

	constexpr bool request_previous_render() override;
	constexpr bool uses_menu_background() override;
	constexpr bool uses_dark_sheet() override;

	static State* make_state(App& app_handle);

	~State_Level_Creator();

};

#endif // _STATE_LEVEL_CREATOR_H_