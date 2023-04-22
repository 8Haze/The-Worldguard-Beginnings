#ifndef _UTILITY_H_
#define _UTILITY_H_

#include <string>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <cstdint>

#include <SFML/Graphics.hpp>

#include "hct/dynarray.h"
#include "hct/deck.h"
#include "hct/sort.h"
#include "hct/scope_guard.h"

enum class LEVEL_THEME
{
	VERDANT = 1,
	WATER,
	FIRE,
	ELECTRIC,
	WIND,
	GROUND,
	LIFEFORCE,
	ICE,
	COUNT
}; // keep COUNT last

enum class LEVELS_GROUP
{
	CAMPAIGN,
	USER_MADE,
	COUNT
}; // keep COUNT last

const float EPSILON = 0.00001f;
const int CAMPAIGN_LEVELS_COUNT = 2; // increase when more levels are added, 24 is the goal
const int PLAYER_PROXIMITY_DISTANCE = 32 * 2;
const int COLLIDING_ENTITY_RENDER_DISTANCE = 32 * 50;
const int WALKING_ENTITY_RENDER_DISTANCE = 32 * 25;

struct Level_Properties
{
	std::filesystem::path path_main;
	std::filesystem::path path_data;
	std::string filename_main;
	std::string filename_data;
	std::string name;
	LEVEL_THEME theme = LEVEL_THEME::VERDANT;
	unsigned int achieved_points = 0;
	unsigned int max_points = 0;
	unsigned int campaign_index = 0;
	bool beaten = false;
};

class No_Copy_No_Move
{
private:

	No_Copy_No_Move(const No_Copy_No_Move&) = delete;
	No_Copy_No_Move& operator=(const No_Copy_No_Move&) = delete;
	No_Copy_No_Move(No_Copy_No_Move&&) = delete;
	No_Copy_No_Move& operator=(No_Copy_No_Move&&) = delete;

public:

	No_Copy_No_Move() = default;

};

void write_error_logs(const std::filesystem::path& path, const std::string& log, bool fatal);

Level_Properties load_level_properties(const std::filesystem::path& path_main, const std::filesystem::path& path_data);

void save_level_properties(Level_Properties lp);

hct::dynarray<Level_Properties> load_levels(const std::filesystem::path& directory_path, LEVELS_GROUP levels_group);

std::string level_theme_to_string(LEVEL_THEME theme);

sf::Color level_theme_to_color(LEVEL_THEME theme);

std::string key_to_string(sf::Keyboard::Key key);

sf::Text make_label(sf::Font& font, const std::string& str, sf::Vector2f pos, int ch_size, bool center = true);

sf::FloatRect get_view_rect(const sf::View& view);

sf::FloatRect get_hitbox(const sf::Sprite& sprite, float width_ratio, float height_ratio);

bool collision(const sf::FloatRect& a, const sf::FloatRect& b);

void apply_dots(sf::Text& text);

std::string apply_dots(const std::string& str);

void flip_vertically(sf::Sprite& sprite);

void flip_horizontally(sf::Sprite& sprite);

void adjust_text_height(sf::Text& text, bool center_by_global_bounds = true);

template <typename... Args>
std::string make_string(Args... args)
{
	std::stringstream ss;
	(ss << ... << args);
	return ss.str();
}

template <typename T>
void set_center(T& obj, bool global = true)
{
	if (global)
		obj.setOrigin(obj.getGlobalBounds().width * 0.5f, obj.getGlobalBounds().height * 0.5f);
	else
		obj.setOrigin(obj.getLocalBounds().width * 0.5f, obj.getLocalBounds().height * 0.5f);
}

template <typename T>
void data_reader(std::istream& ifs, T& obj)
{
	char* dst = reinterpret_cast<char*>(&obj);
	size_t size = sizeof(obj);
	ifs.read(dst, size);
}

template <typename T>
void data_writer(std::ostream& ofs, T obj)
{
	char* src = reinterpret_cast<char*>(&obj);
	size_t size = sizeof(obj);
	ofs.write(src, size);
}

template <typename Container>
auto get_range_of_closest_objects(const sf::Sprite& sprite, const Container& objects, float width_ratio = 1.f)
{
	auto comparing_lambda = [](auto& lhs, auto& rhs) { return *lhs < rhs; };
	sf::Vector2f comparing_vec_left = { sprite.getPosition().x - sprite.getGlobalBounds().width * width_ratio, sprite.getPosition().y };
	sf::Vector2f comparing_vec_right = { sprite.getPosition().x + sprite.getGlobalBounds().width * width_ratio, sprite.getPosition().y };
	auto result_left = std::lower_bound(objects.begin(), objects.end(), comparing_vec_left, comparing_lambda);
	auto result_right = std::lower_bound(objects.begin(), objects.end(), comparing_vec_right, comparing_lambda);

	return std::make_pair(result_left, result_right);
}

template <typename Container>
auto get_range_of_closest_objects(const sf::Vector2f& point, const Container& objects, float offset)
{
	auto comparing_lambda = [](auto& lhs, auto& rhs) { return *lhs < rhs; };
	sf::Vector2f comparing_vec_left = { point.x - offset, point.y };
	sf::Vector2f comparing_vec_right = { point.x + offset, point.y };
	auto result_left = std::lower_bound(objects.begin(), objects.end(), comparing_vec_left, comparing_lambda);
	auto result_right = std::lower_bound(objects.begin(), objects.end(), comparing_vec_right, comparing_lambda);

	return std::make_pair(result_left, result_right);
}

#endif // _UTILITY_H_