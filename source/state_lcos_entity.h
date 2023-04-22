#ifndef _STATE_LCOS_ENTITY_H_
#define _STATE_LCOS_ENTITY_H_

#include "app.h"
#include "cursor_highlight.h"

class State_LCOS_Entity final : public State_LCOS_Base
{
private:

	enum class OPTION 
	{
		TRAMPOLINE,
		STRONG_TRAMPOLINE,
		THORN,
		THORN_SPEAR,
		MINE,
		HEALING_PLANT,
		WIND_BLASTER,
		LIVING_SPIKES,
		CANNON_LEFT,
		CANNON_DOWN,
		CANNON_RIGHT,
		ROCK_DISPOSER,
		ICE_LAUNCHER_LEFT,
		ICE_LAUNCHER_RIGHT,
		BUBBLE_MACHINE,
		BIONIC_RAY_LEFT,
		BIONIC_RAY_DOWN,
		BIONIC_RAY_RIGHT,
		ELECTRIC_GATE_LEFT,
		ELECTRIC_GATE_DOWN,
		ELECTRIC_GATE_RIGHT,
		FLAMETHROWER,
		WARRIOR,
		CASTER,
		ELEMENTAL,
		GOLEM,
		JUNGLE_BOOMERANGER,
		AQUAVEIL_WEAVER,
		SCORCHER,
		ENERGIZED_GALVANIZER,
		AERIAL_BOMBARDIER,
		ROCK_HURLER,
		CRIMSON_LASHER,
		ICEBANER,
		BOUNCER,
		FALLING_HAZARD,
		PRESSURE_ORB,
		ROAMING_TRAMPOLINE,
		SCOUTER,
		COUNT
	}; // keep COUNT last

	State_LCOS_Entity(App& arg_app_handle);

	bool can_choose_option(OPTION option);
	std::string option_to_string(OPTION option);
	int cursor_index(sf::Vector2i pos);

	LEVEL_THEME theme;

public:

	void handle_events(const sf::Event& event) override;
	void update(const sf::Time& dt) override;

	static State* make_state(App& app_handle);

};

class State_LCOS_Thorn final : public State_LCOS_Base
{
private:

	inline static const int max_index = 14;

	Cursor_Highlight cursor_highlight;

	State_LCOS_Thorn(App& arg_app_handle);

public:

	void handle_events(const sf::Event& event) override;
	void update(const sf::Time& dt) override;
	void render() override;

	static State* make_state(App& app_handle);

};

#endif // _STATE_LCOS_ENTITY_H_