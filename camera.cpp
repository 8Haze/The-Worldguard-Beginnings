#include "camera.h"

// ----------------------------------------------------------------
// Camera
// ----------------------------------------------------------------

void Camera::adjust_to_bounds()
{
	shift.x = 0;
	shift.y = 0;

	if (getCenter().x - getSize().x * 0.5f < left_lim)
	{
		shift.x = left_lim - (getCenter().x - getSize().x * 0.5f);
		setCenter(left_lim + getSize().x * 0.5f, getCenter().y);
	}
	else if (getCenter().x + getSize().x * 0.5f > right_lim)
	{
		shift.x = right_lim - (getCenter().x + getSize().x * 0.5f);
		setCenter(right_lim - getSize().x * 0.5f, getCenter().y);
	}
	if (getCenter().y - getSize().y * 0.5f < up_lim)
	{
		shift.y = up_lim - (getCenter().y - getSize().y * 0.5f);
		setCenter(getCenter().x, up_lim + getSize().y * 0.5f);
	}
	else if (getCenter().y + getSize().y * 0.5f > down_lim)
	{
		shift.y = down_lim - (getCenter().y + getSize().y * 0.5f);
		setCenter(getCenter().x, down_lim - getSize().y * 0.5f);
	}
}

sf::Vector2f Camera::get_shift()
{
	return shift;
}

sf::Vector2f Camera::move_within_bounds(float x, float y)
{
	move(x, y);
	adjust_to_bounds();
	auto result = shift;
	result.x += x;
	result.y += y;
	return result;
}

sf::Vector2f Camera::move_within_bounds(sf::Vector2f vector)
{
	move(vector);
	adjust_to_bounds();
	auto result = shift;
	result.x += vector.x;
	result.y += vector.y;
	return result;
}

void Camera::set_center_within_bounds(float x, float y)
{
	setCenter(x, y);
	adjust_to_bounds();
}

void Camera::set_center_within_bounds(sf::Vector2f vector)
{
	setCenter(vector);
	adjust_to_bounds();
}