#ifndef _GUI_TEXT_FIELD_H_
#define _GUI_TEXT_FIELD_H_

#include "resources.h"
#include "chromatic_sprite.h"
#include "sfx_manager.h"

class GUI_Text_Field
{
private:

	inline static const auto sound = RESOURCE_ID::SOUNDS::GUI_TEXT_ENTERED;
	inline static const float cursor_interval = 0.3f;
	inline static const int default_max_characters = 25;

	Chromatic_Sprite sprite;
	sf::Text text;
	std::string string;
	sf::Time cursor_timer;
	Resources& resources;
	int max_characters;
	bool cursor_visible = false;

public:

	GUI_Text_Field(Resources& arg_resources, int arg_max_characters = default_max_characters);

	const std::string& get_string() const noexcept;

	void set_position(sf::Vector2f position);

	void clear();
	void handle_input(const sf::Event& event);
	void update(const sf::Time& dt);
	void draw(sf::RenderWindow& window);

};

#endif // _GUI_TEXT_FIELD_H_