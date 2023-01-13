#include "gui_campaign_level_list.h"

#include <algorithm>
#include <fstream>

#include "hct/deck.h"
#include "hct/sort.h"

#include "config_manager.h"
#include "utility.h"

// ----------------------------------------------------------------
// GUI_Campaign_Level_List
// ----------------------------------------------------------------

GUI_Campaign_Level_List::GUI_Campaign_Level_List(Resources& arg_resources, SFX_Manager& arg_sfx_manager) :
	sfx_manager(arg_sfx_manager)
{
	levels = std::move(load_levels(Config_Manager::campaign_levels_directory, LEVELS_GROUP::CAMPAIGN));

	for (auto max = levels.size() - 1; max_accessible_level_index < max && 
		levels[max_accessible_level_index].beaten; ++max_accessible_level_index);

	sprite.setTexture(arg_resources.textures.get(RESOURCE_ID::TEXTURES::GUI_CAMPAIGN_LEVEL_PANEL));
	set_center(sprite);

	for (auto& x : level_theme_logos)
	{
		x.setTexture(arg_resources.textures.get(RESOURCE_ID::TEXTURES::THEMES_LOGOS));
		x.setTextureRect({ 0, 0, 32, 32 });
		x.setScale(2.f, 2.f);
		set_center(x, false);
	}

	auto& font = arg_resources.fonts.get(RESOURCE_ID::FONTS::MAIN);
	level_title = make_label(font, {}, {}, 30);
	level_points = make_label(font, {}, {}, 30);

	update();
}

void GUI_Campaign_Level_List::set_position(sf::Vector2f position)
{
	sprite.setPosition(position);
	level_title.setPosition(position.x, sprite.getGlobalBounds().top + sprite.getGlobalBounds().height * 0.09f);
	level_points.setPosition(position.x, sprite.getGlobalBounds().top + sprite.getGlobalBounds().height * 0.35f);
	for (int a = 0; a < logos_count; ++a)
		level_theme_logos[a].setPosition(position.x + ((-0.3f + 0.3f * a) * sprite.getGlobalBounds().width),
			sprite.getGlobalBounds().top + sprite.getGlobalBounds().height * 0.65f);
}

void GUI_Campaign_Level_List::update()
{
	const auto& level = levels[level_index];
	std::string string_title, string_points;
	int theme_logo_index;

	if (level_index <= max_accessible_level_index)
	{
		sprite.setColor(level_theme_to_color(level.theme));
		theme_logo_index = (static_cast<int>(level.theme) - 1);
		string_title = make_string(level_index + 1, ": ", level.name);
		auto string_points_part1 = apply_dots(make_string(level.achieved_points));
		auto string_points_part2 = apply_dots(make_string(level.max_points));
		string_points = make_string(string_points_part1, " / ", string_points_part2);
	}
	else
	{
		sprite.setColor({ 255, 255, 255 });
		theme_logo_index = 8;
		string_title = make_string(level_index + 1, ": LOCKED");
		string_points = "? / ?";
	}

	for (auto& x : level_theme_logos)
		x.setTextureRect({ 32 * theme_logo_index, 0, 32, 32 });

	string_title.push_back('|');
	level_title.setString(string_title);
	set_center(level_title);
	string_title.pop_back();
	level_title.setString(string_title);

	level_points.setString(string_points);
	set_center(level_points);
}

bool GUI_Campaign_Level_List::execute(App& app_handle)
{
	if (level_index > max_accessible_level_index)
		return false;
		
	app_handle.register_scpp(std::make_unique<SCPP_Level_Properties>(levels[level_index]));
	return true;
}

void GUI_Campaign_Level_List::choose_prev()
{
	if (level_index == 0)
		return;

	--level_index;
	update();
	sfx_manager.play_sound(sound_select);
}

void GUI_Campaign_Level_List::choose_next()
{
	if (level_index == levels.size() - 1)
		return;

	++level_index;
	update();
	sfx_manager.play_sound(sound_select);
}

void GUI_Campaign_Level_List::draw(sf::RenderWindow& window)
{
	window.draw(sprite);
	for (auto& x : level_theme_logos)
		window.draw(x);
	window.draw(level_title);
	window.draw(level_points);
}