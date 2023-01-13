#include "state_lcos_light.h"

// ----------------------------------------------------------------
// State_LCOS_Light
// ----------------------------------------------------------------

State_LCOS_Light::State_LCOS_Light(App& arg_app_handle) : State_LCOS_Base(arg_app_handle)
{
	auto& resources = app_handle.get_shared_context().resources;

	label.setString("Which Light color?");
	set_center(label);

	options.setTexture(resources.textures.get(RESOURCE_ID::TEXTURES::LCOS_LIGHT));
	set_center(options);

	adjust_to_resolution();
}

int State_LCOS_Light::cursor_index(sf::Vector2i pos)
{
	int index_x = (pos.x - static_cast<int>(options.getGlobalBounds().left)) / 64;
	int index_y = (pos.y - static_cast<int>(options.getGlobalBounds().top)) / 64;
	return index_x + 3 * index_y;
}

void State_LCOS_Light::handle_events(const sf::Event& event)
{
	if (event.type == sf::Event::MouseButtonPressed && event.key.code == sf::Mouse::Left)
		if (auto pos = sf::Mouse::getPosition(); options.getGlobalBounds().contains(pos.x, pos.y))
		{
			if (int index = cursor_index(pos); index >= 0 && index <= max_index)
			{
				app_handle.register_scpp(std::make_unique<SCPP_LCOS_Light>(index));
				app_handle.request_pop();
			}
		}
}

void State_LCOS_Light::update(const sf::Time& dt)
{
	auto pos = sf::Mouse::getPosition();

	if (auto pos = sf::Mouse::getPosition(); options.getGlobalBounds().contains(pos.x, pos.y))
	{
		info.setString(index_to_light_color_name(cursor_index(pos)));
		adjust_text_height(info);
	}
	else
		info.setString("");
}

State* State_LCOS_Light::make_state(App& app_handle)
{
	return new State_LCOS_Light(app_handle);
}