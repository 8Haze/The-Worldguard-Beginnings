#ifndef _ADVANCED_COMPONENTS_H_
#define _ADVANCED_COMPONENTS_H_

#include "lo_tiles.h"
#include "camera.h"

// Declare inheritance by passing the inheriting type to this template argument
template <typename Derived>
class Vertical_Physics_Component : public No_Copy_No_Move
{
public:

	inline static const float max_gravity = 500.f;

private:

	inline static const float gravity_change_speed = 1800.f;

	Derived& derived;
	float gravity = 0.f;
	float gravity_cooldown = 0.f;
	bool reversed_gravity = false;

public:

	enum class COLLISION 
	{ 
		BOTTOM,
		TOP,
		NONE,
		COUNT
	}; // keep COUNT last

	Vertical_Physics_Component(Derived& arg_derived) : derived(arg_derived) {}

	bool is_descending() const noexcept
	{
		if (reversed_gravity)
			return gravity < -EPSILON;
		else
			return gravity > EPSILON;
	}

	bool is_ascending() const noexcept
	{
		if (reversed_gravity)
			return gravity > EPSILON;
		else
			return gravity < -EPSILON;
	}

	bool is_actually_descending() const noexcept
	{
		return gravity > EPSILON;
	}

	bool is_actually_ascending() const noexcept
	{
		return gravity < -EPSILON;
	}

	bool is_reversed_gravity() const noexcept
	{
		return reversed_gravity;
	}

	void set_gravity(float a) noexcept
	{
		gravity = a;
	}

	void set_gravity_cooldown(float a) noexcept
	{
		gravity_cooldown = a;
	}

	void set_reversed_gravity(bool a) noexcept
	{
		reversed_gravity = a;
	}

	float update_vertically(const sf::Time& dt)
	{
		auto offset = dt.asSeconds() * gravity_change_speed;

		if (gravity_cooldown > EPSILON)
			gravity_cooldown -= offset;
		else
		{
			if (reversed_gravity)
			{
				if (gravity > -max_gravity + EPSILON)
					gravity -= offset;
				if (gravity < -max_gravity - EPSILON)
					gravity = -max_gravity;
			}
			else
			{
				if (gravity < max_gravity - EPSILON)
					gravity += offset;
				if (gravity > max_gravity + EPSILON)
					gravity = max_gravity;
			}
		}
		if (gravity_cooldown < EPSILON)
			gravity_cooldown = 0.f;

		return gravity * dt.asSeconds();
	}

	void adjust_bottom_collision(sf::FloatRect tile_hitbox)
	{
		auto a = tile_hitbox.top;
		auto b = derived.get_hitbox().top + derived.get_hitbox().height;
		derived.move({ 0.f, a - b - EPSILON });
	}

	void adjust_top_collision(sf::FloatRect tile_hitbox)
	{
		auto a = tile_hitbox.top + tile_hitbox.height;
		auto b = derived.get_hitbox().top;
		derived.move({ 0.f, a - b + EPSILON });
	}

	template <typename Iter>
	COLLISION check_vertical_collisions(Iter iBegin, Iter iEnd, sf::FloatRect hitbox)
	{
		constexpr bool destructible = std::is_same_v<typename std::iterator_traits<Iter>::value_type, std::unique_ptr<Destructible_Tile>>;

		for (; iBegin != iEnd; ++iBegin)
		{
			if constexpr (destructible)
				if ((*iBegin)->is_broken())
					continue;

			if (auto tile_hitbox = (*iBegin)->get_hitbox(); ::collision(hitbox, tile_hitbox))
			{
				COLLISION collision = COLLISION::NONE;
				if (is_actually_descending())
				{
					adjust_bottom_collision(tile_hitbox);
					collision = COLLISION::BOTTOM;
				}
				else if (is_actually_ascending())
				{
					adjust_top_collision(tile_hitbox);
					collision = COLLISION::TOP;
				}

				set_gravity(0.f);
				set_gravity_cooldown(0.f);

				return collision;
			}
		}

		return COLLISION::NONE;
	}

};

// Declare inheritance by passing the inheriting type to this template argument
template <typename Derived>
class Horizontal_Physics_Component : public No_Copy_No_Move
{
private:

	inline static const float default_max_velocity = 250.f;
	inline static const float default_velocity_change_speed = 1800.f;

	Derived& derived;
	float velocity = 0.f;
	float max_velocity = default_max_velocity;
	float velocity_change_speed = default_velocity_change_speed;
	bool going_left = false;
	bool going_right = false;
	bool faced_left = false;

public:

	enum class COLLISION 
	{ 
		LEFT, 
		RIGHT,
		NONE,
		COUNT
	}; // keep COUNT last

	Horizontal_Physics_Component(Derived& arg_derived) : derived(arg_derived) {}

	float get_velocity() const noexcept
	{
		return velocity;
	}

	float get_max_velocity() const noexcept
	{
		return max_velocity;
	}

	float get_velocity_change_speed() const noexcept
	{
		return velocity_change_speed;
	}

	float get_default_max_velocity() const noexcept
	{
		return default_max_velocity;
	}

	float get_default_velocity_change_speed() const noexcept
	{
		return default_velocity_change_speed;
	}

	bool is_going_left() const noexcept
	{
		return going_left && !going_right;
	}

	bool is_going_right() const noexcept
	{
		return going_right && !going_left;
	}

	bool is_actually_moving_left() const noexcept
	{
		return velocity < 0.f - EPSILON;
	}

	bool is_actually_moving_right() const noexcept
	{
		return velocity > 0.f + EPSILON;
	}

	bool is_going() const noexcept
	{
		return ((going_left || going_right) && going_left != going_right);
	}

	bool is_faced_left() const noexcept
	{
		return faced_left;
	}

	bool is_faced_right() const noexcept
	{
		return !faced_left;
	}

	void set_velocity(float a) noexcept
	{
		velocity = a;
	}

	void set_max_velocity(float a) noexcept
	{
		max_velocity = a;
	}

	void set_velocity_change_speed(float a) noexcept
	{
		velocity_change_speed = a;
	}

	void set_going_left(bool a) noexcept
	{
		going_left = a;
	}

	void set_going_right(bool a) noexcept
	{
		going_right = a;
	}

	float update_horizontally(const sf::Time& dt)
	{
		if (going_left != going_right)
			faced_left = going_left;
		auto offset = dt.asSeconds() * velocity_change_speed;
		if (!is_going())
		{
			if (velocity < 0.f - EPSILON)
			{
				velocity += offset;
				if (velocity > 0.f - EPSILON)
					velocity = 0.f;
			}
			else
			{
				velocity -= offset;
				if (velocity < 0.f + EPSILON)
					velocity = 0.f;
			}
		}
		else if (going_left)
		{
			if (velocity > (-max_velocity) + EPSILON)
				velocity -= offset;
			if (velocity < (-max_velocity) - EPSILON)
				velocity = -max_velocity;
		}
		else if (going_right)
		{
			if (velocity < max_velocity - EPSILON)
				velocity += offset;
			if (velocity > max_velocity + EPSILON)
				velocity = max_velocity;
		}
		return velocity * dt.asSeconds();
	}

	void adjust_left_collision(sf::FloatRect tile_hitbox)
	{
		auto a = tile_hitbox.left + tile_hitbox.width;
		auto b = derived.get_hitbox().left;
		derived.move({ a - b + EPSILON + EPSILON , 0.f });
	}

	void adjust_right_collision(sf::FloatRect tile_hitbox)
	{
		auto a = tile_hitbox.left;
		auto b = derived.get_hitbox().left + derived.get_hitbox().width;
		derived.move({ a - b - EPSILON - EPSILON , 0.f });
	}

	template <typename Iter>
	COLLISION check_horizontal_collisions(Iter iBegin, Iter iEnd, sf::FloatRect hitbox, bool forcefully_moved_left, bool forcefully_moved_right)
	{
		constexpr bool destructible = std::is_same_v<typename std::iterator_traits<Iter>::value_type, std::unique_ptr<Destructible_Tile>>;

		for (; iBegin != iEnd; ++iBegin)
		{
			if constexpr (destructible)
				if ((*iBegin)->is_broken())
					continue;

			if (auto tile_hitbox = (*iBegin)->get_hitbox(); collision(hitbox, tile_hitbox))
			{
				COLLISION collision = COLLISION::NONE;

				if ((is_actually_moving_left() && !forcefully_moved_right) || forcefully_moved_left)
				{
					adjust_left_collision(tile_hitbox);
					collision = COLLISION::LEFT;
				}
				else if ((is_actually_moving_right() && !forcefully_moved_left) || forcefully_moved_right)
				{
					adjust_right_collision(tile_hitbox);
					collision = COLLISION::RIGHT;
				}
				if (!forcefully_moved_left && !forcefully_moved_right)
					set_velocity(0.f);

				return collision;
			}
		}

		return COLLISION::NONE;
	}

};

template <typename T, size_t MAX>
class Subentity_Manager_Component : public No_Copy_No_Move
{
private:

	hct::deck<std::unique_ptr<T>> subentities;

protected:

	template <typename... Args>
	void create_subentity(Resources& resources, sf::Vector2f position, Args&&... extra_args)
	{
		if (subentities.size() < MAX)
		{
			std::unique_ptr<T> new_subentity = std::make_unique<T>(resources, position, std::forward<Args>(extra_args)...);
			subentities.push_back(std::move(new_subentity));
		}
	}

public:

	Subentity_Manager_Component() = default;

	const hct::deck<std::unique_ptr<T>>& get_subentities() const noexcept
	{
		return subentities;
	}

	void update_subentities(Resources& resources, const sf::Time& dt,
		const hct::dynarray<std::unique_ptr<Solid_Tile>>& solid_tiles,
		const hct::dynarray<std::unique_ptr<Destructible_Tile>>& destructible_tiles)
	{
		while (!subentities.empty() && subentities.front()->is_finished())
			subentities.pop_front();

		for (auto& subentity : subentities)
			subentity->update(resources, dt, solid_tiles, destructible_tiles);
	}

	void draw_subentities(sf::RenderTexture& rt) const
	{
		for (auto& subentity : subentities)
			subentity->draw(rt);
	}

};

#endif // _ADVANCED_COMPONENTS_H_