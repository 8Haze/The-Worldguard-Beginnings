#ifndef _ANIMATABLE_SPRITE_H_
#define _ANIMATABLE_SPRITE_H_

#include <SFML/Graphics.hpp>

struct Animation_Settings
{
    int row = 0;
    int start_frame = 0;
    int frames_count = 1;
    int x_size = 1;
    int y_size = 1;
    float duration = 1.f;
    bool looped = true;
    bool vertically_flipped = false;
    bool horizontally_flipped = false;

    Animation_Settings() = default;
    Animation_Settings(int arg_row, int arg_start_frame, int arg_frames_count, int arg_x_size, int arg_y_size,
        float arg_duration, bool arg_looped, bool arg_vertically_flipped = false, bool arg_horizontally_flipped = false);
};

class Animatable_Sprite : public sf::Sprite
{
private:

    Animation_Settings animation;
    sf::Time time_elapsed;
    int current_frame = 0;

public:

    Animatable_Sprite() = default;
    Animatable_Sprite(const sf::Texture& arg_texture, Animation_Settings arg_animation);
    Animatable_Sprite(const Animatable_Sprite& arg_other);

    bool is_vertically_flipped() const noexcept;
    bool is_horizontally_flipped() const noexcept;
    bool finished() const noexcept;
    Animation_Settings& get_animation_settings() noexcept;
    const Animation_Settings& get_animation_settings() const noexcept;

    void set_animation(Animation_Settings arg);

    void restart_animation();
    void update_frame(sf::Time dt);

};

#endif // _ANIMATABLE_SPRITE_H_