#ifndef _GUI_DIALOGUE_BOX_H_
#define _GUI_DIALOGUE_BOX_H_

#include "gui_buttons_layout.h"

// See "gui_buttons_layout.h" for an explanation of this template argument's purpose
template <typename... Args>
class GUI_Dialogue_Box
{
private:

	GUI_Buttons_Layout<Args...> buttons;
	hct::dynarray<sf::Text> texts;
	Chromatic_Sprite sprite;
	sf::Font& font;

	void arrange_buttons()
	{
		float x = sprite.getPosition().x;
		float y = sprite.getPosition().y + sprite.getGlobalBounds().height * 0.25f;

		switch (buttons.get_buttons_count())
		{
		case (1):
			buttons.set_button_position({ x, y }, 0);
			break;
		case (2):
			buttons.set_button_position({ x - sprite.getGlobalBounds().width * 0.2f, y }, 0);
			buttons.set_button_position({ x + sprite.getGlobalBounds().width * 0.2f, y }, 1);
			break;
		case (3):
			buttons.set_button_position({ x - sprite.getGlobalBounds().width * 0.3f, y }, 0);
			buttons.set_button_position({ x, y }, 1);
			buttons.set_button_position({ x + sprite.getGlobalBounds().width * 0.3f, y }, 2);
			break;
		}
	}

public:

	GUI_Dialogue_Box(Resources& arg_resources, SFX_Manager& arg_sfx_manager) :
		buttons(arg_resources, arg_sfx_manager), font(arg_resources.fonts.get(RESOURCE_ID::FONTS::MAIN))
	{
		sprite.setTexture(arg_resources.textures.get(RESOURCE_ID::TEXTURES::GUI_DIALOGUE_BOX));
		sprite.setOrigin(sprite.getGlobalBounds().width * 0.5f, sprite.getGlobalBounds().height * 0.5f);
	}

	void set_position(sf::Vector2f position)
	{
		sprite.setPosition(position);
		float start_offset = 0.3f - static_cast<float>(texts.size() / 2) * 0.1f;
		for (int a = 0; a < texts.size(); ++a)
			texts[a].setPosition(sf::Vector2f(sprite.getPosition().x, sprite.getGlobalBounds().top + sprite.getGlobalBounds().height * (start_offset + 0.1f * a)));
		arrange_buttons();
	}

	void set_text(const std::string& text)
	{
		texts.clear();
		texts.emplace_back();
		texts.back() = make_label(font, text, { 0, 0 }, 30);
		set_center(texts.back());
	}

	void set_text(const hct::dynarray<std::string>& strings)
	{
		for (int a = 0; a < strings.size(); ++a)
		{
			texts.emplace_back();
			texts.back() = make_label(font, strings[a], {0, 0}, 30);
		}
	}

	void add_button(void(fn)(Args...), const std::string& text)
	{
		if (buttons.get_buttons_count() == 3)
			throw std::runtime_error("GUI_Dialogue_Box::add_button: cannot contain more than 3 buttons");

		buttons.create_button(fn, text);
		arrange_buttons();
	}
	
	void choose_next()
	{
		buttons.choose_next();
	}
	
	void choose_prev()
	{
		buttons.choose_prev();
	}

	void execute(Args... args)
	{
		buttons.execute(std::forward<Args>(args)...);
	}

	void draw(sf::RenderWindow& window)
	{
		sprite.set_updated_color();
		window.draw(sprite);
		for (auto& x : texts)
			window.draw(x);
		buttons.draw(window);
	}

};

#endif // _GUI_DIALOGUE_BOX_H_