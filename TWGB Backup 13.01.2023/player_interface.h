#ifndef _PLAYER_INTERFACE_H_
#define _PLAYER_INTERFACE_H_

#include "chromatic_sprite.h"
#include "animatable_sprite.h"
#include "resources.h"

class Scores_Panel
{
private:

	Chromatic_Sprite cells_sprite;
	Animatable_Sprite coins_sprite;

	sf::Text cells_label;
	sf::Text coins_label;

public:

	Scores_Panel(Resources& arg_resources);

	void update(const sf::View& view, const sf::Time& dt, int cells_amount, int coins_amount);
	void draw(sf::RenderTexture& rt);

};

class Player_Lifebar
{
private:

	Chromatic_Sprite frame;
	Chromatic_Sprite filling;
	sf::Text label;

public:

	Player_Lifebar(Resources& arg_resources);

	void update(const sf::View& view, const sf::Time& dt, int health, int max_health);
	void draw(sf::RenderTexture& rt);

};

class Temperature_Bar
{
private:

	inline static const float time_to_vanish = 3.f;

	sf::Sprite frame;
	sf::Sprite filling;
	sf::Text label;

	float display_timer = 0.f;

	int previous_temperature = 0;

public:


	Temperature_Bar(Resources& arg_resources);

	void update(const sf::View& view, const sf::Time& dt, float temperature);
	void draw(sf::RenderTexture& rt);

};

#endif // _PLAYER_INTERFACE_H_