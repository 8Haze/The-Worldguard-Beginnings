#include "light_manager.h"

#include "hct/sort.h"

#include "level_object_id.h"
#include "utility.h"

// ----------------------------------------------------------------
// Light_Manager::Light_Source
// ----------------------------------------------------------------

Light_Manager::Light_Source::Light_Source(sf::RenderTexture& arg_texture, sf::Vector2f arg_position, Light_Manager::COLOR arg_color_id, float arg_radius, float arg_falloff) :
    texture(&arg_texture), position(arg_position), color_id(arg_color_id), radius(arg_radius), falloff(arg_falloff)
{
    switch (arg_color_id)
    {
    case(Light_Manager::COLOR::RED): color = { 255, 0, 0 }; break;
    case(Light_Manager::COLOR::GREEN): color = { 0, 255, 0 }; break;
    case(Light_Manager::COLOR::BLUE): color = { 0, 0, 255 }; break;
    case(Light_Manager::COLOR::YELLOW): color = { 255, 255, 0 }; break;
    case(Light_Manager::COLOR::MINT): color = { 0, 255, 255 }; break;
    case(Light_Manager::COLOR::PINK): color = { 255, 0, 255 }; break;
    case(Light_Manager::COLOR::ORANGE): color = { 255, 128, 0 }; break;
    case(Light_Manager::COLOR::HOT_PINK): color = { 255, 0, 128 }; break;
    case(Light_Manager::COLOR::WHITE): color = { 255, 255, 255 }; break;
    default: throw(std::invalid_argument("Light_Manager::Light_Source: color id out of range"));
    }
}

sf::Vector2f Light_Manager::Light_Source::get_position() const noexcept
{
    return position;
}

sf::Glsl::Vec3 Light_Manager::Light_Source::get_position(float x_shift, float y_shift, bool zoom_in) const
{
    return sf::Glsl::Vec3(
        position.x * (zoom_in ? 2.f : 1.f) - x_shift * (zoom_in ? 1.f : 2.f), 
        texture->getSize().y - position.y * (zoom_in ? 2.f : 1.f) + y_shift * (zoom_in ? 1.f : 2.f),
        10.f);
}

Light_Manager::COLOR Light_Manager::Light_Source::get_color_id() const noexcept
{
    return color_id;
}

sf::Glsl::Vec3 Light_Manager::Light_Source::get_colors() const noexcept
{
    return {
        static_cast<float>(color.r) / 255.f,
        static_cast<float>(color.g) / 255.f,
        static_cast<float>(color.b) / 255.f };
}

float Light_Manager::Light_Source::get_radius() const noexcept
{
    return radius;
}

float Light_Manager::Light_Source::get_falloff() const noexcept
{
    return falloff;
}

bool Light_Manager::Light_Source::operator<(const Light_Source& other) const noexcept
{
    return position.x < other.position.x;
}

bool Light_Manager::Light_Source::operator<(int lim) const noexcept
{
    return position.x < lim;
}

// ----------------------------------------------------------------
// Light_Manager
// ----------------------------------------------------------------

Light_Manager::Light_Manager(sf::RenderTexture& arg_texture, RM_Shaders& arg_shaders) :
    texture(arg_texture)
{
    shader = &arg_shaders.get(RESOURCE_ID::SHADERS::LIGHTS);
    shader->setUniform("texture", texture.getTexture());
}

void Light_Manager::set_ambient_light(float arg) noexcept
{
    if (arg >= 0.f && arg <= 1.f)
        ambient_light = arg;
}

void Light_Manager::read_light(std::istream& file)
{
    std::int32_t color;
    int pos_x, pos_y;
    data_reader(file, pos_x);
    data_reader(file, pos_y);
    data_reader(file, color);
    if (color < 0 || color >= static_cast<int>(COLOR::COUNT))
        throw std::runtime_error("Light_Manager::read_light(): color out of range");
    add_light({ static_cast<float>(pos_x) + 16.f, static_cast<float>(pos_y) + 16.f }, static_cast<COLOR>(color), 64.f, 0.1f);
}

void Light_Manager::add_light(sf::Vector2f position, Light_Manager::COLOR color, float radius, float falloff)
{
    lights.emplace_back(texture, position, color, radius, falloff);
    hct::sort(lights.begin(), lights.end());
}

void Light_Manager::remove_light(sf::Vector2f position)
{
    lights.erase(std::remove_if(lights.begin(), lights.end(), [position](const auto& light) 
        { 
            int x = static_cast<int>(light.get_position().x);
            int y = static_cast<int>(light.get_position().y);
            x -= 16;
            y -= 16;
            return static_cast<int>(position.x) == x && static_cast<int>(position.y) == y;
        }), 
        lights.end());
}

void Light_Manager::update_ambient_light(const sf::Time& dt)
{
    if (current_ambient_light - ambient_light < -0.001f)
        current_ambient_light += dt.asSeconds() * ambient_light_chaning_speed;
    else if (current_ambient_light - ambient_light > 0.001f)
        current_ambient_light -= dt.asSeconds() * ambient_light_chaning_speed;

    if (current_ambient_light > 1.f)
        current_ambient_light = 1.f;
    if (current_ambient_light < 0.f)
        current_ambient_light = 0.f;
}

void Light_Manager::record_lights(std::ostream& stream)
{
    for (const auto& light : lights)
    {
        data_writer(stream, static_cast<int32_t>(LO_TYPE::LIGHT_SOURCE));
        data_writer(stream, static_cast<int32_t>(0));
        data_writer(stream, static_cast<int32_t>(light.get_position().x - 16.f));
        data_writer(stream, static_cast<int32_t>(light.get_position().y - 16.f));
        data_writer(stream, static_cast<int32_t>(light.get_color_id()));
    }
}

void Light_Manager::draw(int x_lim_first, int x_lim_last, int y_lim_first, int y_lim_last, float x_shift, float y_shift, bool zoom_in)
{
    auto first = std::lower_bound(lights.begin(), lights.end(), x_lim_first);
    auto last = std::lower_bound(lights.begin(), lights.end(), x_lim_last);

    int oob = 0;

    for (auto it = first; it < last; ++it)
        if (auto pos = it->get_position().y; pos > y_lim_first || pos < y_lim_last)
            ++oob;

    int lights_count = std::min<int>(std::distance(first, last) - oob, max_visible_lights);

    shader->setUniform("LightsCount", lights_count);
    shader->setUniform("AmbientLight", sf::Glsl::Vec3(current_ambient_light, current_ambient_light, current_ambient_light));

    for (int a = 0; a < lights_count; ++a, ++first)
    {
        if (auto pos = first->get_position().y; pos > y_lim_first || pos < y_lim_last)
        {
            --a;
            continue;
        }

        std::string id = "Lights[" + std::to_string(a) + "]";
        shader->setUniform(id + ".position", first->get_position(x_shift, y_shift, zoom_in));
        shader->setUniform(id + ".color", first->get_colors());
        shader->setUniform(id + ".radius", first->get_radius() * (zoom_in ? 1.0f : 0.5f));
        shader->setUniform(id + ".falloff", first->get_falloff());
    }

    auto size = texture.getSize();
    sf::VertexArray vertices(sf::TriangleStrip, 4);
    vertices[0] = sf::Vertex(sf::Vector2f(0, 0), sf::Vector2f(0, 1));
    vertices[1] = sf::Vertex(sf::Vector2f(size.x, 0), sf::Vector2f(1, 1));
    vertices[2] = sf::Vertex(sf::Vector2f(0, size.y), sf::Vector2f(0, 0));
    vertices[3] = sf::Vertex(sf::Vector2f(size), sf::Vector2f(1, 0));

    texture.draw(vertices, shader);
}