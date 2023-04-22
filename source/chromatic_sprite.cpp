#include "chromatic_sprite.h"

// ----------------------------------------------------------------
// Chromatic_Sprite
// ----------------------------------------------------------------

Chromatic_Sprite::Chromatic_Sprite() : sf::Sprite()
{
    this->set_updated_color();
}

void Chromatic_Sprite::set_updated_color()
{
    this->setColor(sf::Color(rgb[0], rgb[1], rgb[2]));
}

void Chromatic_Sprite::switch_color()
{
    index = (index == 0 ? 2 : index - 1);
    increasing = !increasing;
}

void Chromatic_Sprite::update_color(sf::Time dt)
{
    int value = static_cast<int>(color_change_speed * dt.asSeconds());

    if (increasing)
    {
        if (value > 255 - rgb[index])
        {
            rgb[index] = 255;
            switch_color();
        }
        else
            rgb[index] += value;
    }
    else
    {
        if (value > rgb[index])
        {
            rgb[index] = 0;
            switch_color();
        }
        else
            rgb[index] -= value;
    }
}