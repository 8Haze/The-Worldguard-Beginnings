#ifndef _LO_ITEMS_H_
#define _LO_ITEMS_H_

#include "lo_bases.h"
#include "basic_components.h"
#include "player.h"

// ----------------------------------------------------------------
// Base Item Class
// ----------------------------------------------------------------

class Item : public LO_Animated, public Value_Component
{
private:

	inline static const float speed_y = 10.f;
	inline static const float y_offset_cap = 2.f;

	bool ascending = true;
	bool collected = false;

	virtual void set_desired_sprite(Resources& resources) = 0;

	virtual void award_player(Player& player) = 0;

public:

	Item() = default;

	sf::FloatRect get_hitbox() const noexcept;

	void set_value(int a) noexcept;

	void update(Resources& resources, const sf::Time& dt);
	void player_interaction(Player& player);

};

// ----------------------------------------------------------------
// Concrete Item Classes
// ----------------------------------------------------------------

class Food final : public Item
{
private:

	std::int32_t index_x = 0;
	std::int32_t index_y = 0;

	void set_desired_sprite(Resources& resources) override;

	void extra_setup(std::istream& stream, Resources& resources) override;
	void extra_pre_record(std::ostream& stream) const override;
	void extra_post_record(std::ostream& stream) const override;
	void award_player(Player& player) override;

public:

	Food() = default;

	static Item* make_lo();

};

class Gem final : public Item
{
private:

	std::int32_t index = 0;

	void set_desired_sprite(Resources& resources) override;

	void extra_setup(std::istream& stream, Resources& resources) override;
	void extra_pre_record(std::ostream& stream) const override;
	void extra_post_record(std::ostream& stream) const override;
	void award_player(Player& player) override;

public:

	Gem() = default;

	static Item* make_lo();

};

class Coin final : public Item
{
private:

	std::int32_t index = 0;

	void set_desired_sprite(Resources& resources) override;

	void extra_setup(std::istream& stream, Resources& resources) override;
	void extra_pre_record(std::ostream& stream) const override;
	void extra_post_record(std::ostream& stream) const override;
	void award_player(Player& player) override;

public:

	Coin() = default;

	static Item* make_lo();

};

class Lifeup final : public Item
{
private:

	std::int32_t index = 0;

	void set_desired_sprite(Resources& resources) override;

	void extra_setup(std::istream& stream, Resources& resources) override;
	void extra_pre_record(std::ostream& stream) const override;
	void extra_post_record(std::ostream& stream) const override;
	void award_player(Player& player) override;

public:

	Lifeup() = default;

	static Item* make_lo();

};

#endif // _LO_ITEMS_H_