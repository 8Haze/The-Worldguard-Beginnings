#ifndef _LIGHT_MANAGER_H_
#define _LIGHT_MANAGER_H_

#include <fstream>
#include <cstdint>

#include <SFML/Graphics.hpp>

#include "resources.h"

class Light_Manager
{
public:

    enum class COLOR 
    {
        RED, 
        GREEN, 
        BLUE,
        YELLOW,
        MINT,
        PINK,
        ORANGE,
        HOT_PINK, 
        WHITE,
        COUNT 
    }; // keep COUNT last

private:

    inline static const int max_visible_lights = 25;
    inline static const float ambient_light_chaning_speed = 1.5f;

    class Light_Source
    {
    private:

        sf::Vector2f position;
        COLOR color_id;
        sf::Color color;
        float radius;
        float falloff;

        sf::RenderTexture* texture;

    public:

        Light_Source(sf::RenderTexture& arg_texture, sf::Vector2f arg_position, Light_Manager::COLOR arg_color_id, float arg_radius, float arg_falloff);

        sf::Vector2f get_position() const noexcept;
        sf::Glsl::Vec3 get_position(float x_shift, float y_shift, bool zoom_in) const;
        COLOR get_color_id() const noexcept;
        sf::Glsl::Vec3 get_colors() const noexcept;
        float get_radius() const noexcept;
        float get_falloff() const noexcept;

        bool operator<(const Light_Source& other) const noexcept;
        bool operator<(int lim) const noexcept;

    };

    hct::dynarray<Light_Source> lights;

    sf::RenderTexture& texture;
    sf::Shader* shader;

    float ambient_light = 1.f;
    float current_ambient_light = 1.f;

public:

    Light_Manager(sf::RenderTexture& arg_texture, RM_Shaders& arg_shaders);

    void set_ambient_light(float arg) noexcept;

    void read_light(std::istream& file);
    void add_light(sf::Vector2f position, Light_Manager::COLOR color, float radius, float falloff);
    void remove_light(sf::Vector2f position);

    void update_ambient_light(const sf::Time& dt);
    void record_lights(std::ostream& stream);

    void draw(int x_lim_first, int x_lim_last, int y_lim_first, int y_lim_last, float x_shift, float y_shift, bool zoom_in);

};

#endif // _LIGHT_MANAGER_H_