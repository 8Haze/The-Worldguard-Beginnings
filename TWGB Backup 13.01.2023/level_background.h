#ifndef _LEVEL_BACKGROUND_H_
#define _LEVEL_BACKGROUND_H_

#include "resources.h"
#include "config_manager.h"
#include "camera.h"
#include "utility.h"

RESOURCE_ID::TEXTURES level_theme_to_background_id(LEVEL_THEME theme_id);

class Level_Background
{
private:

	inline static const float gradient_y_ratio = 0.15f;

	sf::Sprite gradient;
	sf::RectangleShape filling;

public:
	
	Level_Background(Resources& arg_resources, Config_Manager& arg_config_manager, LEVEL_THEME arg_theme_id);

	void adjust(Config_Manager& config_manager);
	void update(const Camera& camera);
	void draw(sf::RenderTexture& rt);

};

#endif // _LEVEL_BACKGROUND_H_