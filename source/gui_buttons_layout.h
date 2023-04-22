#ifndef _GUI_BUTTONS_LAYOUT_H_
#define _GUI_BUTTONS_LAYOUT_H_

#include <functional>

#include "utility.h"
#include "resources.h"
#include "sfx_manager.h"
#include "chromatic_sprite.h"

// Args... is a collection of argument types that must be passed to buttons' functions at their execution
template <typename... Args>
class GUI_Buttons_Layout
{
private:

	class Button
	{
	private:

		std::function<void(Args...)> function;
		mutable Chromatic_Sprite sprite;
		sf::Text text;

	public:

		Button(Resources& arg_resources, void(arg_function)(Args...), const std::string& arg_string,
			sf::Vector2f arg_position, RESOURCE_ID::TEXTURES arg_texture_id, int arg_text_size) :
			function(arg_function)
		{
			sprite.setTexture(arg_resources.textures.get(arg_texture_id));
			set_center(sprite);
			sprite.setPosition(arg_position);

			text = make_label(arg_resources.fonts.get(RESOURCE_ID::FONTS::MAIN), arg_string, arg_position, arg_text_size);
		}

		void set_position(sf::Vector2f position)
		{
			sprite.setPosition(position);
			text.setPosition(position);
		}

		void set_text(const std::string& string)
		{
			text.setString(string);
			set_center(text);
		}

		void select()
		{
			text.setFillColor(sf::Color::Yellow);
		}

		void deselect()
		{
			text.setFillColor(sf::Color::White);
		}

		void execute(Args... args) const
		{
			function(std::forward<Args>(args)...);
		}

		void draw(sf::RenderWindow& window) const
		{
			sprite.set_updated_color();
			window.draw(sprite);
			window.draw(text);
		}

	};

	inline static const auto sound_select = RESOURCE_ID::SOUNDS::GUI_BUTTON_SELECT;
	inline static const auto sound_execute = RESOURCE_ID::SOUNDS::GUI_BUTTON_EXECUTE;

	hct::dynarray<Button> buttons;
	size_t selected_idx;

	Resources& resources;
	SFX_Manager& sfx_manager;

	RESOURCE_ID::TEXTURES button_texture_id;

public:

	GUI_Buttons_Layout(Resources& arg_resources, SFX_Manager& arg_sfx_manager,
		RESOURCE_ID::TEXTURES arg_button_texture_id = RESOURCE_ID::TEXTURES::GUI_BUTTON) :
		resources(arg_resources), sfx_manager(arg_sfx_manager), selected_idx(0), button_texture_id(arg_button_texture_id) {}

	size_t get_buttons_count() const noexcept
	{
		return buttons.size();
	}

	size_t get_index() const noexcept
	{
		return selected_idx;
	}

	void set_button_position(sf::Vector2f position, int index)
	{
		buttons[index].set_position(position);
	}

	void set_button_text(const std::string& text, int index)
	{
		buttons[index].set_text(text);
	}

	void create_button(void(fn)(Args...), const std::string& text, sf::Vector2f position = { 0, 0 }, int text_size = 25)
	{
		buttons.emplace_back(resources, fn, text, position, button_texture_id, text_size);
		if (selected_idx == 0)
			buttons[selected_idx].select();
	}

	void choose_next(bool loop = true)
	{
		sfx_manager.play_sound(sound_select);

		if (buttons.size() < 2)
			return;

		buttons[selected_idx].deselect();
		selected_idx = (selected_idx == buttons.size() - 1 ? 0 : selected_idx + 1);
		buttons[selected_idx].select();
	}

	void choose_prev(bool loop = true)
	{
		sfx_manager.play_sound(sound_select);

		if (buttons.size() < 2)
			return;

		buttons[selected_idx].deselect();
		selected_idx = (selected_idx == 0 ? buttons.size() - 1 : selected_idx - 1);
		buttons[selected_idx].select();
	}

	void play_choose_sound()
	{
		sfx_manager.play_sound(sound_select);
	}

	void execute(Args... args)
	{
		buttons[selected_idx].execute(std::forward<Args>(args)...);

		sfx_manager.play_sound(sound_execute);
	}

	void draw(sf::RenderWindow& window)
	{
		for (const auto& x : buttons)
			x.draw(window);
	}

};

#endif // _GUI_BUTTONS_LAYOUT_H_