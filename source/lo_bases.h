#ifndef _LO_BASES_H_
#define _LO_BASES_H_

#include "animatable_sprite.h"
#include "resources.h"
#include "utility.h"

// Forward declarations for friendship declaration purposes
class LO_Static;
class LO_Animated;

// LO - Level Object, ST - Sprite Type
template <typename ST>
class LO_Base : public No_Copy_No_Move
{
private:

	friend class LO_Static;
	friend class LO_Animated;

	ST sprite;
	sf::Vector2<int32_t> original_position;

	virtual void extra_setup(std::istream& ifs, Resources& resources) = 0;
	virtual void extra_pre_record(std::ostream& stream) const = 0;
	virtual void extra_post_record(std::ostream& stream) const {}
	virtual void extra_move(float x, float y) {}
	virtual void extra_set_position(float x, float y) {}

public:

	LO_Base() = default;

	const ST& get_sprite() const noexcept
	{
		return sprite;
	}

	sf::Vector2<int32_t> get_original_position() const noexcept
	{
		return original_position;
	}

	void set_sprite_texture(sf::Texture& texture)
	{
		sprite.setTexture(texture);
	}

	void set_sprite_color(sf::Color color)
	{
		sprite.setColor(color);
	}

	void set_sprite_rotation(float rotation)
	{
		sprite.setRotation(rotation);
	}

	void set_position(float x, float y)
	{
		sprite.setPosition(x, y);
		extra_set_position(x, y);
	}

	void set_position(sf::Vector2f position)
	{
		sprite.setPosition(position);
		extra_set_position(position.x, position.y);
	}

	void move(float x, float y)
	{
		sprite.move(x, y);
		extra_move(x, y);
	}

	void move(sf::Vector2f vector)
	{
		sprite.move(vector);
		extra_move(vector.x, vector.y);
	}

	void center_sprite()
	{
		set_center(sprite);
		move(sprite.getGlobalBounds().width * 0.5f, sprite.getGlobalBounds().height * 0.5f);
	}

	void reset_sprite_origin()
	{
		sprite.setOrigin(0.f, 0.f);
		move(-sprite.getGlobalBounds().width * 0.5f, -sprite.getGlobalBounds().height * 0.5f);
	}

	void setup(std::istream& stream, Resources& resources)
	{
		data_reader(stream, original_position.x);
		data_reader(stream, original_position.y);
		set_position(static_cast<float>(original_position.x), static_cast<float>(original_position.y));

		extra_setup(stream, resources);
	}

	void record(std::ostream& stream) const
	{
		extra_pre_record(stream);

		data_writer(stream, original_position.x);
		data_writer(stream, original_position.y);

		extra_post_record(stream);
	}

	virtual void draw(sf::RenderTexture& rt) const
	{
		rt.draw(sprite);
	}

	friend bool operator<(const LO_Base& lhs, const LO_Base& rhs) noexcept
	{
		return lhs.original_position.x == rhs.original_position.x ?
			lhs.original_position.y < rhs.original_position.y :
			lhs.original_position.x < rhs.original_position.x;
	}

	friend bool operator<(const LO_Base& lhs, sf::Vector2f rhs) noexcept
	{
		return lhs.original_position.x == rhs.x ?
			lhs.original_position.y < rhs.y :
			lhs.original_position.x < rhs.x;
	}

	friend bool operator<(const LO_Base& lhs, float rhs) noexcept
	{
		return lhs.original_position.x < rhs;
	}

	virtual ~LO_Base() = default;

};

class LO_Static : public LO_Base<sf::Sprite>
{
public:

	LO_Static() = default;

	void set_sprite_rect(sf::IntRect rect);

};

class LO_Animated : public LO_Base<Animatable_Sprite>
{
public:

	LO_Animated() = default;

	Animation_Settings& get_sprite_animation();

	void set_sprite_animation(Animation_Settings animation);

	void restart_sprite_animation();
	void update_frame(const sf::Time& dt);

};

#endif // _LO_BASES_H_