#include "gui_user_level_list.h"

#include <algorithm>
#include <fstream>

#include "hct/deck.h"
#include "hct/sort.h"

#include "config_manager.h"

// ----------------------------------------------------------------
// GUI_User_Level_List
// ----------------------------------------------------------------

GUI_User_Level_List::GUI_User_Level_List(Resources& arg_resources, SFX_Manager& arg_sfx_manager) :
	buttons(arg_resources, arg_sfx_manager, RESOURCE_ID::TEXTURES::GUI_BUTTON_USER_LEVEL_LIST)
{
	levels = std::move(load_levels(Config_Manager::user_levels_directory, LEVELS_GROUP::USER_MADE));

	sprite.setTexture(arg_resources.textures.get(RESOURCE_ID::TEXTURES::GUI_USER_LEVEL_LIST));
	set_center(sprite);

	label = make_label(arg_resources.fonts.get(RESOURCE_ID::FONTS::MAIN),
		make_string("Level Name | Points | Theme | Beaten?"), {0, 0}, 30);

	for (int a = 0, count = std::min<int>(4, levels.size()); a < count; ++a)
		buttons.create_button([]() {}, "", { 0, 0 }, 20);

	update_buttons();
}

void GUI_User_Level_List::set_position(sf::Vector2f position)
{
	sprite.setPosition(position);
	label.setPosition(position.x, sprite.getGlobalBounds().top + sprite.getGlobalBounds().height * 0.09f);

	for (int a = 0, count = buttons.get_buttons_count(); a < count; ++a)
		buttons.set_button_position({ position.x, sprite.getGlobalBounds().top + sprite.getGlobalBounds().height * (0.3f + 0.19f * a) }, a);
}

void GUI_User_Level_List::update_buttons()
{
	for (unsigned int a = 0; a < buttons.get_buttons_count(); ++a)
	{
		auto& lp = levels[a + offset];
		buttons.set_button_text(make_string(lp.name, " | ", apply_dots(make_string(lp.max_points)), " | ", 
			level_theme_to_string(lp.theme), " | ", (lp.beaten ? 'Y' : 'N')), a);
	}
}

bool GUI_User_Level_List::execute(App& app_handle)
{
	if (buttons.get_buttons_count() == 0)
		return false;

	app_handle.register_scpp(std::make_unique<SCPP_Level_Properties>(levels[offset + buttons.get_index()]));
	return true;
}

void GUI_User_Level_List::choose_prev()
{
	if (buttons.get_index() > 0)
		buttons.choose_prev();
	else if (offset > 0)
	{
		--offset;
		buttons.play_choose_sound();
	}

	update_buttons();
}

void GUI_User_Level_List::choose_next()
{
	if (buttons.get_index() + 1 < buttons.get_buttons_count())
		buttons.choose_next();
	else if (offset + buttons.get_index() + 1 < levels.size())
	{
		++offset;
		buttons.play_choose_sound();
	}

	update_buttons();
}

void GUI_User_Level_List::draw(sf::RenderWindow& window)
{
	sprite.set_updated_color();
	window.draw(sprite);
	window.draw(label);
	buttons.draw(window);
}