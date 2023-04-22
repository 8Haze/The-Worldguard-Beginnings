#ifndef _CHROMATIC_SPRITE_H_
#define _CHROMATIC_SPRITE_H_

#include <array>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

class Chromatic_Sprite : public sf::Sprite
{
private:

	inline static const float color_change_speed = 250.f;

	inline static std::array<unsigned char, 3> rgb = { 255, 0, 0 };
	inline static int index = 1;
	inline static bool increasing = true;

	static void switch_color();

public:

	Chromatic_Sprite();

	void set_updated_color();

	static void update_color(sf::Time dt);

};

#endif // _CHROMATIC_SPRITE_H_