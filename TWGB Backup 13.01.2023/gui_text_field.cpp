#include "gui_text_field.h"

#include "utility.h"

// ----------------------------------------------------------------
// GUI_Text_Field
// ----------------------------------------------------------------

GUI_Text_Field::GUI_Text_Field(Resources& arg_resources, int arg_max_characters) : 
	resources(arg_resources), max_characters(arg_max_characters)
{
	sprite.setTexture(resources.textures.get(RESOURCE_ID::TEXTURES::GUI_TEXT_FIELD));
	sprite.setOrigin(sprite.getGlobalBounds().width * 0.5f, sprite.getGlobalBounds().height * 0.5f);

	text = make_label(resources.fonts.get(RESOURCE_ID::FONTS::MAIN), "", { 0, 0 }, 20);
}

const std::string& GUI_Text_Field::get_string() const noexcept
{
	return string;
}

void GUI_Text_Field::set_position(sf::Vector2f position)
{
	sprite.setPosition(position);
	text.setPosition(position);
}

void GUI_Text_Field::clear()
{
	string.clear();
}

void GUI_Text_Field::handle_input(const sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Backspace && !string.empty())
		string.pop_back();
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Backspace) ||
		sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) ||
		sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		return;
	else if (event.type == sf::Event::TextEntered && string.size() < max_characters)
		string.push_back(static_cast<char>(event.text.unicode));
}

void GUI_Text_Field::update(const sf::Time& dt)
{
	cursor_timer += dt;
	if (cursor_timer.asSeconds() > cursor_interval)
	{
		cursor_timer = sf::Time::Zero;
		cursor_visible = !cursor_visible;
	}

	text.setString(string);
	adjust_text_height(text);

	if (cursor_visible)
	{
		string.push_back('|');
		text.setString(string);
		string.pop_back();
	}
}

void GUI_Text_Field::draw(sf::RenderWindow& window)
{
	sprite.set_updated_color();
	window.draw(sprite);
	window.draw(text);
}