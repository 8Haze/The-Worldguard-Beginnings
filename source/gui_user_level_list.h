#ifndef _GUI_USER_LEVEL_LIST_H_
#define _GUI_USER_LEVEL_LIST_H_

#include <vector>

#include "utility.h"
#include "chromatic_sprite.h"
#include "gui_buttons_layout.h"
#include "app.h"

class GUI_User_Level_List
{
private:

	hct::dynarray<Level_Properties> levels;
	GUI_Buttons_Layout<> buttons;
	Chromatic_Sprite sprite;
	sf::Text label;
	unsigned int offset = 0;

	void update_buttons();

public:

	GUI_User_Level_List(Resources& arg_resources, SFX_Manager& arg_sfx_manager);

	void set_position(sf::Vector2f position);

	bool execute(App& app_handle);
	void choose_prev();
	void choose_next();
	void draw(sf::RenderWindow& window);

};

#endif // _GUI_USER_LEVEL_LIST_H_