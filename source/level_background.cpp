#include "level_background.h"

// ----------------------------------------------------------------
// Functions
// ----------------------------------------------------------------

RESOURCE_ID::TEXTURES level_theme_to_background_id(LEVEL_THEME theme_id)
{
	switch (theme_id)
	{
	case(LEVEL_THEME::VERDANT): return RESOURCE_ID::TEXTURES::VERDANT_BACKGROUND; break;
	case(LEVEL_THEME::WATER): return RESOURCE_ID::TEXTURES::WATER_BACKGROUND; break;
	case(LEVEL_THEME::FIRE): return RESOURCE_ID::TEXTURES::FIRE_BACKGROUND; break;
	case(LEVEL_THEME::ELECTRIC): return RESOURCE_ID::TEXTURES::ELECTRIC_BACKGROUND; break;
	case(LEVEL_THEME::WIND): return RESOURCE_ID::TEXTURES::WIND_BACKGROUND; break;
	case(LEVEL_THEME::GROUND): return RESOURCE_ID::TEXTURES::GROUND_BACKGROUND; break;
	case(LEVEL_THEME::LIFEFORCE): return RESOURCE_ID::TEXTURES::LIFEFORCE_BACKGROUND; break;
	case(LEVEL_THEME::ICE): return RESOURCE_ID::TEXTURES::ICE_BACKGROUND; break;
	default: throw std::runtime_error("level_theme_to_background_id(): invalid id provided");
	}
}

// ----------------------------------------------------------------
// Level_Background
// ----------------------------------------------------------------

Level_Background::Level_Background(Resources& arg_resources, Config_Manager& arg_config_manager, LEVEL_THEME arg_theme_id)
{
	gradient.setTexture(arg_resources.textures.get(level_theme_to_background_id(arg_theme_id)));
	gradient.setOrigin(0, gradient.getLocalBounds().height);

	filling.setFillColor(gradient.getTexture()->copyToImage().getPixel(0, 0));

	adjust(arg_config_manager);
}

void Level_Background::adjust(Config_Manager& config_manager)
{
	gradient.setTextureRect(sf::IntRect(0, 0, config_manager.get(Config_Manager::DATA::RES_WIDTH), gradient.getTexture()->getSize().y));
	filling.setSize(sf::Vector2f(config_manager.get(Config_Manager::DATA::RES_WIDTH), config_manager.get(Config_Manager::DATA::RES_HEIGHT)));
}

void Level_Background::update(const Camera& camera)
{
	auto y_offset = (camera.down_lim - (camera.getCenter().y + camera.getSize().y * 0.5f)) * gradient_y_ratio;
	gradient.setPosition(camera.getCenter().x - camera.getSize().x * 0.5f, camera.getCenter().y + camera.getSize().y * 0.5f + y_offset);
	filling.setPosition(camera.getCenter().x - camera.getSize().x * 0.5f, camera.getCenter().y - camera.getSize().y * 0.5f);
}

void Level_Background::draw(sf::RenderTexture& rt)
{
	rt.draw(filling);
	rt.draw(gradient);
}