#include "animatable_sprite.h"

#include "utility.h"

// ----------------------------------------------------------------
// Animation_Settings
// ----------------------------------------------------------------

Animation_Settings::Animation_Settings(int arg_row, int arg_start_frame, int arg_frames_count, int arg_x_size, int arg_y_size,
    float arg_duration, bool arg_looped, bool arg_vertically_flipped, bool arg_horizontally_flipped) :
    row(arg_row), start_frame(arg_start_frame), frames_count(arg_frames_count), x_size(arg_x_size), y_size(arg_y_size),
    duration(arg_duration), looped(arg_looped), vertically_flipped(arg_vertically_flipped), horizontally_flipped(arg_horizontally_flipped) {}

// ----------------------------------------------------------------
// Animatable_Sprite
// ----------------------------------------------------------------

Animatable_Sprite::Animatable_Sprite(const sf::Texture& arg_texture, Animation_Settings arg_animation) : animation(arg_animation)
{
    this->setTexture(arg_texture);
    int left = animation.start_frame * animation.x_size;
    int top = animation.row * animation.y_size;
    this->setTextureRect(sf::IntRect(left, top, animation.x_size, animation.y_size));
    time_elapsed = sf::Time::Zero;
}

Animatable_Sprite::Animatable_Sprite(const Animatable_Sprite& arg_other) : Sprite(arg_other), animation(arg_other.animation)
{
    this->setTexture(*arg_other.getTexture());
    this->setTextureRect(arg_other.getTextureRect());
}

bool Animatable_Sprite::is_vertically_flipped() const noexcept
{
    return animation.vertically_flipped;
}

bool Animatable_Sprite::is_horizontally_flipped() const noexcept
{
    return animation.horizontally_flipped;
}

bool Animatable_Sprite::finished() const noexcept
{
    return (!animation.looped && (current_frame == (animation.frames_count - 1)));
}

Animation_Settings& Animatable_Sprite::get_animation_settings() noexcept
{
    return animation;
}

const Animation_Settings& Animatable_Sprite::get_animation_settings() const noexcept
{
    return animation;
}

void Animatable_Sprite::set_animation(Animation_Settings arg)
{
    animation = arg;

    current_frame = 0;
    int left = animation.start_frame * animation.x_size;
    int top = animation.row * animation.y_size;
    this->setTextureRect(sf::IntRect(left, top, animation.x_size, animation.y_size));
    if (animation.vertically_flipped)
        flip_vertically(*this);
    if (animation.horizontally_flipped)
        flip_horizontally(*this);
    time_elapsed = sf::Time::Zero;
}

void Animatable_Sprite::restart_animation()
{
    current_frame = 0;
    int left = animation.start_frame * animation.x_size;
    int top = animation.row * animation.y_size;
    this->setTextureRect(sf::IntRect(left, top, animation.x_size, animation.y_size));
    if (animation.vertically_flipped)
        flip_vertically(*this);
    if (animation.horizontally_flipped)
        flip_horizontally(*this);
    time_elapsed = sf::Time::Zero;
}

void Animatable_Sprite::update_frame(sf::Time dt)
{
    time_elapsed += dt;

    current_frame = static_cast<int>((time_elapsed.asSeconds() / animation.duration) * animation.frames_count);
    if (animation.looped)
        current_frame %= animation.frames_count;
    else if (current_frame >= animation.frames_count)
        current_frame = animation.frames_count - 1;

    int left = animation.start_frame * animation.x_size + current_frame * animation.x_size;
    int top = animation.row * animation.y_size;
    this->setTextureRect(sf::IntRect(left, top, animation.x_size, animation.y_size));
    if (animation.vertically_flipped)
        flip_vertically(*this);
    if (animation.horizontally_flipped)
        flip_horizontally(*this);
}