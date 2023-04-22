#include "basic_components.h"

// ----------------------------------------------------------------
// Health_Component
// ----------------------------------------------------------------

Health_Component::Health_Component(unsigned int health_) :
	health(health_), max_health(health_) {}

bool Health_Component::can_take_damage() const
{
	return true;
}

void Health_Component::react_to_damage() {}

void Health_Component::react_to_heal() {}

bool Health_Component::is_alive() const noexcept
{
	return health != 0;
}

unsigned int Health_Component::get_health() const noexcept
{
	return health;
}

unsigned int Health_Component::get_max_health() const noexcept
{
	return max_health;
}

void Health_Component::set_health(unsigned int a) noexcept
{
	health = a;
}

void Health_Component::set_max_health(unsigned int a) noexcept
{
	max_health = a;
}

bool Health_Component::take_damage(unsigned int damage)
{
	if (!can_take_damage())
		return false;

	bool result;

	if (health == 0)
		return false;
	else if (damage >= health)
	{
		health = 0;
		result = true;
	}
	else
	{
		health -= damage;
		result = false;
	}

	react_to_damage();

	return result;
}

void Health_Component::receive_heal(unsigned int heal) noexcept
{
	if (health == max_health)
		return;

	health += heal;
	if (health > max_health)
		health = max_health;
	react_to_heal();
}

// ----------------------------------------------------------------
// Damage_Component
// ----------------------------------------------------------------

Damage_Component::Damage_Component(unsigned int damage_) : 
	damage(damage_) {}

unsigned int Damage_Component::get_damage() const noexcept
{
	return damage;
}

void Damage_Component::set_damage(unsigned int a) noexcept
{
	damage = a;
}

bool Damage_Component::deal_damage(Health_Component& target) const
{
	return target.take_damage(damage);
}

// ----------------------------------------------------------------
// Value_Component
// ----------------------------------------------------------------

Value_Component::Value_Component(int value_) : 
	value(value_) {}

int Value_Component::get_value() const noexcept
{
	return value;
}

bool Value_Component::is_harvested() const noexcept
{
	return harvested;
}

void Value_Component::set_value(int a) noexcept
{
	value = a;
}

void Value_Component::set_harvested(bool a) noexcept
{
	harvested = a;
}