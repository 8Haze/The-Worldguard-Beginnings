#ifndef _CURSOR_HIGHLIGHT_H_
#define _CURSOR_HIGHLIGHT_H_

#include "resources.h"

class Cursor_Highlight
{
private:

	inline static const int transparency_value = 85;

	sf::RectangleShape shape;
	int offset_x;
	int offset_y;

public:

	Cursor_Highlight(int arg_offset_x = 32, int arg_offset_y = 32);

	void draw_at(sf::RenderTarget& rt, sf::Vector2i screen_origin, sf::Vector2i mouse_coords);

};

#endif // _CURSOR_HIGHLIGHT_H_