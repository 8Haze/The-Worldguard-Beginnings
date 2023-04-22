#include "utility.h"

#include "resources.h"

// ----------------------------------------------------------------
// Functions
// ----------------------------------------------------------------

void write_error_logs(const std::filesystem::path& path, const std::string& log, bool fatal)
{
	std::ofstream file(path, std::ios::app);
	auto guard = hct::make_guard_exit([&file]() { file.close(); });
	auto now = std::chrono::system_clock::now();
	file << "[" << std::chrono::floor<std::chrono::seconds>(now) << "] " << log << (fatal ? " [Fatal]" : " [Handled]") << "\n";
}

Level_Properties load_level_properties(const std::filesystem::path& path_main, const std::filesystem::path& path_data)
{
	Level_Properties lp;
	std::ifstream file(path_data);
	std::string name;
	int theme, achieved_points, max_points, campaign_index, beaten;

	std::getline(file, name, '\n');
	if (!(file >> campaign_index >> theme >> achieved_points >> max_points >> beaten) ||
		file.bad() || file.fail() || name.empty() || theme < 1 || theme > 8 || achieved_points > max_points)
		throw std::runtime_error("load_level_properties: corrupted file");

	lp.path_main = path_main;
	lp.path_data = path_data;
	lp.filename_main = path_main.filename().string();
	lp.filename_data = path_data.filename().string();
	lp.name = std::move(name);
	lp.theme = static_cast<LEVEL_THEME>(theme);
	lp.achieved_points = achieved_points;
	lp.max_points = max_points;
	lp.campaign_index = campaign_index;
	lp.beaten = (beaten == 0 ? false : true);

	return lp;
}

void save_level_properties(Level_Properties lp)
{
	std::ofstream file(lp.path_data);
	file << 
		lp.name << "\n" <<
		lp.campaign_index << "\n" <<
		static_cast<int>(lp.theme) << "\n" <<
		lp.achieved_points << "\n" <<
		lp.max_points << "\n" <<
		(lp.beaten ? 1 : 0);
	file.close();
}

hct::dynarray<Level_Properties> load_levels(const std::filesystem::path& directory_path, LEVELS_GROUP levels_group)
{
	namespace fs = std::filesystem;

	hct::dynarray<Level_Properties> result;
	hct::deck<fs::path> files;
	hct::deck<fs::path> temp;

	for (const auto& entry : fs::directory_iterator(directory_path))
		files.push_back(entry);

	files.erase(std::remove_if(files.begin(), files.end(), [](const fs::path& path)
		{ return path.extension() != ".wgl" && path.extension() != ".wgld"; }), files.end());

	hct::sort(files.begin(), files.end());

	while (true)
	{
		if (files.size() == 1)
			files.pop_back();
		if (files.empty())
			break;
		if (files[0].stem() != files[1].stem())
			files.pop_front();
		else
		{
			temp.push_back(std::move(files.front()));
			files.pop_front();
			temp.push_back(std::move(files.front()));
			files.pop_front();
		}
	}

	files = std::move(temp);

	while (!files.empty())
	{
		try
		{
			Level_Properties lp = load_level_properties(files[0], files[1]);
			result.push_back(std::move(lp));
		}
		catch (std::runtime_error) {}
		files.pop_front();
		files.pop_front();
	}

	switch (levels_group)
	{
	case (LEVELS_GROUP::CAMPAIGN):
	{
		if (result.size() != CAMPAIGN_LEVELS_COUNT)
			throw (std::runtime_error("load_levels: invalid amount of campaign levels loaded"));
		hct::sort(result.begin(), result.end(), [](const Level_Properties& a, const Level_Properties& b) { return a.campaign_index < b.campaign_index; });
		break;
	}
	case (LEVELS_GROUP::USER_MADE):
	{
		hct::sort(result.begin(), result.end(), [](const Level_Properties& a, const Level_Properties& b) { return a.name < b.name; });
		break;
	}
	default: throw std::runtime_error("load_levels: unrecognized levels group id");
	}

	return result;
}

std::string level_theme_to_string(LEVEL_THEME theme)
{
	switch (theme)
	{
	case(LEVEL_THEME::VERDANT): return "Verdant"; break;
	case(LEVEL_THEME::WATER): return "Water"; break;
	case(LEVEL_THEME::FIRE): return "Fire"; break;
	case(LEVEL_THEME::ELECTRIC): return "Electric"; break;
	case(LEVEL_THEME::WIND): return "Wind"; break;
	case(LEVEL_THEME::GROUND): return "Ground"; break;
	case(LEVEL_THEME::LIFEFORCE): return "Lifeforce"; break;
	case(LEVEL_THEME::ICE): return "Ice"; break;
	default: throw std::runtime_error("level_theme_to_string: invalid theme ID provided"); break;
	}
}

sf::Color level_theme_to_color(LEVEL_THEME theme)
{
	switch (theme)
	{
	case(LEVEL_THEME::VERDANT): return { 20, 255, 0 }; break;
	case(LEVEL_THEME::WATER): return { 0, 100, 255 }; break;
	case(LEVEL_THEME::FIRE): return { 255, 55, 0 }; break;
	case(LEVEL_THEME::ELECTRIC): return { 255, 255, 0 }; break;
	case(LEVEL_THEME::WIND): return { 185, 225, 255 }; break;
	case(LEVEL_THEME::GROUND): return { 210, 90, 0 }; break;
	case(LEVEL_THEME::LIFEFORCE): return { 255, 0, 55 }; break;
	case(LEVEL_THEME::ICE): return { 0, 185, 255 }; break;
	default: throw std::runtime_error("level_theme_to_color: invalid theme ID provided"); break;
	}
}

std::string key_to_string(sf::Keyboard::Key key)
{
	using SFK = sf::Keyboard::Key;
	std::string str;
	switch (key)
	{
	case(SFK::Q): str = "Q"; break;
	case(SFK::W): str = "W"; break;
	case(SFK::E): str = "E"; break;
	case(SFK::R): str = "R"; break;
	case(SFK::T): str = "T"; break;
	case(SFK::Y): str = "Y"; break;
	case(SFK::U): str = "U"; break;
	case(SFK::I): str = "I"; break;
	case(SFK::O): str = "O"; break;
	case(SFK::P): str = "P"; break;
	case(SFK::A): str = "A"; break;
	case(SFK::S): str = "S"; break;
	case(SFK::D): str = "D"; break;
	case(SFK::F): str = "F"; break;
	case(SFK::G): str = "G"; break;
	case(SFK::H): str = "H"; break;
	case(SFK::J): str = "J"; break;
	case(SFK::K): str = "K"; break;
	case(SFK::L): str = "L"; break;
	case(SFK::Z): str = "Z"; break;
	case(SFK::X): str = "X"; break;
	case(SFK::C): str = "C"; break;
	case(SFK::V): str = "V"; break;
	case(SFK::B): str = "B"; break;
	case(SFK::N): str = "N"; break;
	case(SFK::M): str = "M"; break;
	case(SFK::Left): str = "Left Arrow"; break;
	case(SFK::Right): str = "Right Arrow"; break;
	case(SFK::Up): str = "Up Arrow"; break;
	case(SFK::Down): str = "Down Arrow"; break;
	case(SFK::LControl): str = "Left Control"; break;
	case(SFK::RControl): str = "Right Control"; break;
	case(SFK::LAlt): str = "Left Alt"; break;
	case(SFK::RAlt): str = "Right Alt"; break;
	case(SFK::Space): str = "Space"; break;
	case(SFK::LShift): str = "Left Shift"; break;
	case(SFK::RShift): str = "Right Shift"; break;
	case(SFK::Enter): str = "Enter"; break;
	case(SFK::Escape): str = "Escape"; break;
	case(SFK::Tab): str = "Tab"; break;
	default: str = "Unrecognized"; break;
	}
	return str;
}

sf::Text make_label(sf::Font& font, const std::string& str, sf::Vector2f pos, int ch_size, bool center)
{
	sf::Text text;
	text.setFont(font);
	text.setString(str);
	text.setCharacterSize(ch_size);
	text.setOutlineThickness(2);
	if (center)
		set_center(text);
	text.setPosition(pos);
	text.setFillColor(sf::Color::White);
	text.setOutlineColor(sf::Color::Black);
	return text;
}

sf::FloatRect get_view_rect(const sf::View& view)
{
	sf::FloatRect res;
	res.width = view.getSize().x;
	res.height = view.getSize().y;
	res.left = view.getCenter().x - res.width * 0.5f;
	res.top = view.getCenter().y - res.height * 0.5f;
	return res;
}

sf::FloatRect get_hitbox(const sf::Sprite& sprite, float width_ratio, float height_ratio)
{
	sf::FloatRect original = sprite.getGlobalBounds();
	sf::FloatRect scaled = original;
	scaled.width *= width_ratio;
	scaled.height *= height_ratio;
	scaled.top += original.height * 0.5f - scaled.height * 0.5f;
	scaled.left += original.width * 0.5f - scaled.width * 0.5f;
	return scaled;
}

bool collision(const sf::FloatRect& a, const sf::FloatRect& b)
{
	return a.intersects(b);
}

void apply_dots(sf::Text& text)
{
	std::string str = text.getString();
	std::string new_str;
	int x = str.size();
	for (auto iter = str.begin(); iter != str.end(); iter++)
	{
		new_str += *iter;
		x--;
		if ((x % 3) == 0 && x != 0)
			new_str += '.';
	}
	text.setString(new_str);
}

std::string apply_dots(const std::string& str)
{
	std::string new_str;
	int x = str.size();
	for (auto iter = str.begin(); iter != str.end(); iter++)
	{
		new_str += *iter;
		x--;
		if ((x % 3) == 0 && x != 0)
			new_str += '.';
	}
	return new_str;
}

void flip_vertically(sf::Sprite& sprite)
{
	sf::IntRect rect = sprite.getTextureRect();
	rect.top += rect.height;
	rect.height -= rect.height * 2;
	sprite.setTextureRect(rect);
}

void flip_horizontally(sf::Sprite& sprite)
{
	sf::IntRect rect = sprite.getTextureRect();
	rect.left += rect.width;
	rect.width -= rect.width * 2;
	sprite.setTextureRect(rect);
}

void adjust_text_height(sf::Text& text, bool center_by_global_bounds)
{
	std::string string = text.getString() + '|';
	text.setString(string);
	set_center(text, center_by_global_bounds);
	string.pop_back();
	text.setString(string);
}