#include "cursor_highlight.h"

Cursor_Highlight::Cursor_Highlight(int arg_offset_x, int arg_offset_y) :
	offset_x(arg_offset_x), offset_y(arg_offset_y)
{
	shape.setFillColor(sf::Color(255, 255, 255, transparency_value));
	shape.setSize(sf::Vector2f(arg_offset_x, arg_offset_y));
}

void Cursor_Highlight::draw_at(sf::RenderTarget& rt, sf::Vector2i screen_origin, sf::Vector2i mouse_coords)
{
	shape.setPosition(screen_origin.x + mouse_coords.x - (mouse_coords.x % offset_x),
		screen_origin.y + mouse_coords.y - (mouse_coords.y % offset_y));
	rt.draw(shape);
}