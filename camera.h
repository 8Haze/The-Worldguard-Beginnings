#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <SFML/Graphics/View.hpp>

class Camera : public sf::View
{
private:

	sf::Vector2f shift;

	void adjust_to_bounds();

public:

	inline static const float left_lim = -50.f * 32.f;
	inline static const float right_lim = 950.f * 32.f;
	inline static const float down_lim = 40.f * 32.f;
	inline static const float up_lim = -50.f * 32.f;

	sf::Vector2f get_shift();

	sf::Vector2f move_within_bounds(float x, float y);
	sf::Vector2f move_within_bounds(sf::Vector2f vector);
	void set_center_within_bounds(float x, float y);
	void set_center_within_bounds(sf::Vector2f vector);

};

#endif // _CAMERA_H_