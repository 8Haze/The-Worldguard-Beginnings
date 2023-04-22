#ifndef _COMPONENTS_H_
#define _COMPONENTS_H_

#include "utility.h"

class Health_Component : public No_Copy_No_Move
{
private:

	unsigned int max_health = 0;
	unsigned int health = 0;

	virtual bool can_take_damage() const;
	virtual void react_to_damage();
	virtual void react_to_heal();

public:

	Health_Component() = default;
	Health_Component(unsigned int health_);

	bool is_alive() const noexcept;
	unsigned int get_health() const noexcept;
	unsigned int get_max_health() const noexcept;

	void set_health(unsigned int a) noexcept;
	void set_max_health(unsigned int a) noexcept;

	bool take_damage(unsigned int damage);
	void receive_heal(unsigned int heal) noexcept;
};

class Damage_Component : public No_Copy_No_Move
{
private:

	unsigned int damage = 0;

public:

	Damage_Component() = default;
	Damage_Component(unsigned int damage_);

	unsigned int get_damage() const noexcept;

	void set_damage(unsigned int a) noexcept;

	bool deal_damage(Health_Component& target) const;

};

class Value_Component : public No_Copy_No_Move
{
private:

	int value = 0;
	bool harvested = false;

public:

	Value_Component() = default;
	Value_Component(int value_);

	int get_value() const noexcept;
	bool is_harvested() const noexcept;

	void set_value(int a) noexcept;
	void set_harvested(bool a) noexcept;

};

#endif // _COMPONENTS_H_