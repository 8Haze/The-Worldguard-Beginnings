#include "player_interface.h"

#include <cmath>

#include "animations.h"
#include "utility.h"

// ----------------------------------------------------------------
// Scores_Panel
// ----------------------------------------------------------------

Scores_Panel::Scores_Panel(Resources& arg_resources)
{
	cells_sprite.setTexture(arg_resources.textures.get(RESOURCE_ID::TEXTURES::POWER_CELLS));
	coins_sprite.setTexture(arg_resources.textures.get(RESOURCE_ID::TEXTURES::COIN));
	coins_sprite.set_animation(get_animation(AID_ITEM::COIN));

	cells_label = make_label(arg_resources.fonts.get(RESOURCE_ID::FONTS::MAIN), "0", { 0, 0 }, 15, false);
	coins_label = make_label(arg_resources.fonts.get(RESOURCE_ID::FONTS::MAIN), "0", { 0, 0 }, 15, false);
}

void Scores_Panel::update(const sf::View& view, const sf::Time& dt, int cells_amount, int coins_amount)
{
	cells_sprite.set_updated_color();
	coins_sprite.update_frame(dt);

	cells_label.setString(std::to_string(cells_amount));
	coins_label.setString(std::to_string(coins_amount));

	sf::Vector2f pos(view.getCenter().x - view.getSize().x * 0.5f + 8.f, view.getCenter().y - view.getSize().y * 0.5f + 8.f);
	cells_sprite.setPosition(pos.x, pos.y);
	coins_sprite.setPosition(pos.x, pos.y + 32.f);
	cells_label.setPosition(pos.x + 40.f, pos.y + 5.f);
	coins_label.setPosition(pos.x + 40.f, pos.y + 5.f + 32.f);
}

void Scores_Panel::draw(sf::RenderTexture& rt)
{
	rt.draw(cells_sprite);
	rt.draw(coins_sprite);
	rt.draw(cells_label);
	rt.draw(coins_label);
}

// ----------------------------------------------------------------
// Player_Lifebar
// ----------------------------------------------------------------

Player_Lifebar::Player_Lifebar(Resources& arg_resources)
{
	frame.setTexture(arg_resources.textures.get(RESOURCE_ID::TEXTURES::PLAYER_LIFEBAR_FRAME));
	filling.setTexture(arg_resources.textures.get(RESOURCE_ID::TEXTURES::PLAYER_LIFEBAR_FILLING));
	label = make_label(arg_resources.fonts.get(RESOURCE_ID::FONTS::MAIN), "0 / 0", { 0, 0 }, 30);
	label.setOutlineThickness(label.getOutlineThickness() * 2.f);
	label.setScale(0.5f, 0.5f);
}

void Player_Lifebar::update(const sf::View& view, const sf::Time& dt, int health, int max_health)
{
	float ratio = static_cast<float>(health) / static_cast<float>(max_health);
	filling.setTextureRect(sf::IntRect(0, 0, filling.getTexture()->getSize().x * ratio, filling.getTexture()->getSize().y));

	label.setString(std::to_string(health) + " / " + std::to_string(max_health));
	adjust_text_height(label, false);

	frame.set_updated_color();
	filling.set_updated_color();

	frame.setPosition(view.getCenter().x - view.getSize().x * 0.5f + 8.f, view.getCenter().y + view.getSize().y * 0.5f - frame.getGlobalBounds().height - 8.f);
	filling.setPosition(frame.getPosition());
	label.setPosition(frame.getPosition().x + frame.getGlobalBounds().width * 0.5f, frame.getPosition().y + frame.getGlobalBounds().height * 0.5f);
}

void Player_Lifebar::draw(sf::RenderTexture& rt)
{
	rt.draw(frame);
	rt.draw(filling);
	rt.draw(label);
}

// ----------------------------------------------------------------
// Temperature_Bar
// ----------------------------------------------------------------

Temperature_Bar::Temperature_Bar(Resources& arg_resources)
{
	frame.setTexture(arg_resources.textures.get(RESOURCE_ID::TEXTURES::TEMPERATURE_BAR_FRAME));
	filling.setTexture(arg_resources.textures.get(RESOURCE_ID::TEXTURES::TEMPERATURE_BAR_FILLING));
	label = make_label(arg_resources.fonts.get(RESOURCE_ID::FONTS::MAIN), "0 / 0", { 0, 0 }, 15);

	frame.setOrigin(frame.getGlobalBounds().width, 0);
	filling.setOrigin(filling.getGlobalBounds().width, 0);
}

void Temperature_Bar::update(const sf::View& view, const sf::Time& dt, float temperature)
{
	int temperature_as_int = std::round(temperature);

	if (temperature_as_int == 0)
	{
		display_timer -= dt.asSeconds();
		if (display_timer <= 0.f)
		{
			display_timer = 0.f;
			return;
		}
	}
	else
		display_timer = time_to_vanish;

	if (temperature_as_int < 0)
	{
		frame.setColor({ 47, 166, 249 });
		filling.setColor({ 47, 166, 249 });
	}
	else if (temperature_as_int > 0)
	{
		frame.setColor({ 249, 97, 42 });
		filling.setColor({ 249, 97, 42 });
	}

	float ratio = std::abs(temperature) / 100.f;
	filling.setTextureRect(sf::IntRect(0, 0, filling.getTexture()->getSize().x * ratio, filling.getTexture()->getSize().y));

	label.setString(make_string(temperature_as_int));
	adjust_text_height(label);

	frame.setPosition(view.getCenter().x + view.getSize().x * 0.5f - 8.f, view.getCenter().y + view.getSize().y * 0.5f - frame.getGlobalBounds().height - 8.f);
	filling.setPosition(frame.getPosition());
	label.setPosition(frame.getPosition().x - frame.getGlobalBounds().width * 0.5f, frame.getPosition().y + frame.getGlobalBounds().height * 0.5f);
}

void Temperature_Bar::draw(sf::RenderTexture& rt)
{
	if (display_timer <= 0.0f)
		return;

	rt.draw(frame);
	rt.draw(filling);
	rt.draw(label);
}