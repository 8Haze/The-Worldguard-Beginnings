#ifndef _GUI_CAMPAIGN_LEVEL_LIST_H_
#define _GUI_CAMPAIGN_LEVEL_LIST_H_

#include <array>

#include "utility.h"
#include "chromatic_sprite.h"
#include "gui_buttons_layout.h"
#include "app.h"

class GUI_Campaign_Level_List
{
private:

	inline static const auto sound_select = RESOURCE_ID::SOUNDS::GUI_BUTTON_SELECT;
	inline static const int logos_count = 3;
	inline static const int filename_space = 30;

	hct::dynarray<Level_Properties> levels;
	std::array<sf::Sprite, logos_count> level_theme_logos;
	sf::Sprite sprite;
	sf::Text level_title;
	sf::Text level_points;
	size_t level_index = 0;
	size_t max_accessible_level_index = 0;

	void update();

	SFX_Manager& sfx_manager;

public:

	GUI_Campaign_Level_List(Resources& arg_resources, SFX_Manager& arg_sfx_manager);

	void set_position(sf::Vector2f position);

	bool execute(App& app_handle);
	void choose_prev();
	void choose_next();
	void draw(sf::RenderWindow& window);

};

#endif // _GUI_CAMPAIGN_LEVEL_LIST_H_