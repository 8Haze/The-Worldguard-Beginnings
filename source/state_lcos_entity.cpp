#include "state_lcos_entity.h"

#include "lo_entities.h"

// ----------------------------------------------------------------
// State_LCOS_Entity
// ----------------------------------------------------------------

State_LCOS_Entity::State_LCOS_Entity(App& arg_app_handle) :
	State_LCOS_Base(arg_app_handle),
	theme(dynamic_cast<SCPP_Level_Theme&>(*arg_app_handle.retrieve_scpp()).theme)
{
	auto& resources = app_handle.get_shared_context().resources;

	label.setString("Which Entity type?");
	set_center(label);

	options.setTexture(resources.textures.get(RESOURCE_ID::TEXTURES::LCOS_ENTITY));
	set_center(options);

	adjust_to_resolution();
}

bool State_LCOS_Entity::can_choose_option(OPTION option)
{
	switch (option)
	{
	case (OPTION::THORN): return Thorn::available_for_level_theme(theme);
	case (OPTION::THORN_SPEAR): return Thorn_Spear_Root::available_for_level_theme(theme);
	case (OPTION::MINE): return Mine::available_for_level_theme(theme);
	case (OPTION::HEALING_PLANT): return Healing_Plant::available_for_level_theme(theme);
	case (OPTION::WIND_BLASTER): return Wind_Blaster::available_for_level_theme(theme);
	case (OPTION::LIVING_SPIKES): return Living_Spikes::available_for_level_theme(theme);
	case (OPTION::ROCK_DISPOSER): return Rock_Disposer::available_for_level_theme(theme);
	case (OPTION::ICE_LAUNCHER_LEFT): return Ice_Launcher::available_for_level_theme(theme);
	case (OPTION::ICE_LAUNCHER_RIGHT): return Ice_Launcher::available_for_level_theme(theme);
	case (OPTION::BUBBLE_MACHINE): return Bubble_Machine::available_for_level_theme(theme);
	case (OPTION::BIONIC_RAY_LEFT): return Bionic_Ray_Device::available_for_level_theme(theme);
	case (OPTION::BIONIC_RAY_DOWN): return Bionic_Ray_Device::available_for_level_theme(theme);
	case (OPTION::BIONIC_RAY_RIGHT): return Bionic_Ray_Device::available_for_level_theme(theme);
	case (OPTION::ELECTRIC_GATE_LEFT): return Electric_Gate_Device::available_for_level_theme(theme);
	case (OPTION::ELECTRIC_GATE_DOWN): return Electric_Gate_Device::available_for_level_theme(theme);
	case (OPTION::ELECTRIC_GATE_RIGHT): return Electric_Gate_Device::available_for_level_theme(theme);
	case (OPTION::FLAMETHROWER): return Flamethrower::available_for_level_theme(theme);
	case (OPTION::JUNGLE_BOOMERANGER): return Jungle_Boomeranger::available_for_level_theme(theme);
	case (OPTION::AQUAVEIL_WEAVER): return Aquaveil_Weaver::available_for_level_theme(theme);
	case (OPTION::SCORCHER): return Scorcher::available_for_level_theme(theme);
	case (OPTION::ENERGIZED_GALVANIZER): return Energized_Galvanizer::available_for_level_theme(theme);
	case (OPTION::AERIAL_BOMBARDIER): return Aerial_Bombardier::available_for_level_theme(theme);
	case (OPTION::ROCK_HURLER): return Rock_Hurler::available_for_level_theme(theme);
	case (OPTION::CRIMSON_LASHER): return Crimson_Lasher::available_for_level_theme(theme);
	case (OPTION::ICEBANER): return Icebaner::available_for_level_theme(theme);
	case (OPTION::BOUNCER): return Bouncer::available_for_level_theme(theme);
	case (OPTION::FALLING_HAZARD): return Falling_Hazard::available_for_level_theme(theme);
	case (OPTION::PRESSURE_ORB): return Pressure_Orb::available_for_level_theme(theme);
	case (OPTION::ROAMING_TRAMPOLINE): return Roaming_Trampoline::available_for_level_theme(theme);
	case (OPTION::SCOUTER): return Scouter::available_for_level_theme(theme);
	default: return true;
	}
}

std::string State_LCOS_Entity::option_to_string(OPTION option)
{
	switch (option)
	{
	case (OPTION::TRAMPOLINE): return stationary_entity_type_to_name(STATIONARY_ENTITY_SUBTYPE::TRAMPOLINE);
	case (OPTION::STRONG_TRAMPOLINE): return make_string("Strong ", stationary_entity_type_to_name(STATIONARY_ENTITY_SUBTYPE::TRAMPOLINE));
	case (OPTION::THORN): return stationary_entity_type_to_name(STATIONARY_ENTITY_SUBTYPE::THORN);
	case (OPTION::THORN_SPEAR): return stationary_entity_type_to_name(STATIONARY_ENTITY_SUBTYPE::THORN_SPEAR);
	case (OPTION::MINE): return stationary_entity_type_to_name(STATIONARY_ENTITY_SUBTYPE::MINE);
	case (OPTION::HEALING_PLANT): return stationary_entity_type_to_name(STATIONARY_ENTITY_SUBTYPE::HEALING_PLANT);
	case (OPTION::WIND_BLASTER): return stationary_entity_type_to_name(STATIONARY_ENTITY_SUBTYPE::WIND_BLASTER);
	case (OPTION::LIVING_SPIKES): return stationary_entity_type_to_name(STATIONARY_ENTITY_SUBTYPE::LIVING_SPIKES);
	case (OPTION::CANNON_LEFT): return make_string(colliding_entity_type_to_name(COLLIDING_ENTITY_SUBTYPE::CANNON), " (Shotting Left)");
	case (OPTION::CANNON_DOWN): return make_string(colliding_entity_type_to_name(COLLIDING_ENTITY_SUBTYPE::CANNON), " (Shotting Down)");
	case (OPTION::CANNON_RIGHT): return make_string(colliding_entity_type_to_name(COLLIDING_ENTITY_SUBTYPE::CANNON), " (Shotting Right)");
	case (OPTION::ROCK_DISPOSER): return colliding_entity_type_to_name(COLLIDING_ENTITY_SUBTYPE::ROCK_DISPOSER);
	case (OPTION::ICE_LAUNCHER_LEFT): return make_string(colliding_entity_type_to_name(COLLIDING_ENTITY_SUBTYPE::ICE_LAUNCHER), " (Shooting Left)");
	case (OPTION::ICE_LAUNCHER_RIGHT): return make_string(colliding_entity_type_to_name(COLLIDING_ENTITY_SUBTYPE::ICE_LAUNCHER), " (Shooting Right)");
	case (OPTION::BUBBLE_MACHINE): return colliding_entity_type_to_name(COLLIDING_ENTITY_SUBTYPE::BUBBLE_MACHINE);
	case (OPTION::BIONIC_RAY_LEFT): return make_string(colliding_entity_type_to_name(COLLIDING_ENTITY_SUBTYPE::BIONIC_RAY_DEVICE), " (Shooting Left)");
	case (OPTION::BIONIC_RAY_DOWN): return make_string(colliding_entity_type_to_name(COLLIDING_ENTITY_SUBTYPE::BIONIC_RAY_DEVICE), " (Shooting Down)");
	case (OPTION::BIONIC_RAY_RIGHT): return make_string(colliding_entity_type_to_name(COLLIDING_ENTITY_SUBTYPE::BIONIC_RAY_DEVICE), " (Shooting Right)");
	case (OPTION::ELECTRIC_GATE_LEFT): return make_string(colliding_entity_type_to_name(COLLIDING_ENTITY_SUBTYPE::ELECTRIC_GATE_DEVICE), " (Shooting Left)");
	case (OPTION::ELECTRIC_GATE_DOWN): return make_string(colliding_entity_type_to_name(COLLIDING_ENTITY_SUBTYPE::ELECTRIC_GATE_DEVICE), " (Shooting Down)");
	case (OPTION::ELECTRIC_GATE_RIGHT): return make_string(colliding_entity_type_to_name(COLLIDING_ENTITY_SUBTYPE::ELECTRIC_GATE_DEVICE), " (Shooting Right)");
	case (OPTION::FLAMETHROWER): return colliding_entity_type_to_name(COLLIDING_ENTITY_SUBTYPE::FLAMETHROWER);
	case (OPTION::WARRIOR): return walking_entity_type_to_name(WALKING_ENTITY_SUBTYPE::WARRIOR);
	case (OPTION::CASTER): return colliding_entity_type_to_name(COLLIDING_ENTITY_SUBTYPE::CASTER);
	case (OPTION::ELEMENTAL): return colliding_entity_type_to_name(COLLIDING_ENTITY_SUBTYPE::ELEMENTAL);
	case (OPTION::GOLEM): return walking_entity_type_to_name(WALKING_ENTITY_SUBTYPE::GOLEM);
	case (OPTION::JUNGLE_BOOMERANGER): return colliding_entity_type_to_name(COLLIDING_ENTITY_SUBTYPE::JUNGLE_BOOMERANGER);
	case (OPTION::AQUAVEIL_WEAVER): return colliding_entity_type_to_name(COLLIDING_ENTITY_SUBTYPE::AQUAVEIL_WEAVER);
	case (OPTION::SCORCHER): return walking_entity_type_to_name(WALKING_ENTITY_SUBTYPE::SCORCHER);
	case (OPTION::ENERGIZED_GALVANIZER): return colliding_entity_type_to_name(COLLIDING_ENTITY_SUBTYPE::ENERGIZED_GALVANIZER);
	case (OPTION::AERIAL_BOMBARDIER): return colliding_entity_type_to_name(COLLIDING_ENTITY_SUBTYPE::AERIAL_BOMBARDIER);
	case (OPTION::ROCK_HURLER): return colliding_entity_type_to_name(COLLIDING_ENTITY_SUBTYPE::ROCK_HURLER);
	case (OPTION::CRIMSON_LASHER): return colliding_entity_type_to_name(COLLIDING_ENTITY_SUBTYPE::CRIMSON_LASHER);
	case (OPTION::ICEBANER): return colliding_entity_type_to_name(COLLIDING_ENTITY_SUBTYPE::ICEBANER);
	case (OPTION::BOUNCER): return colliding_entity_type_to_name(COLLIDING_ENTITY_SUBTYPE::BOUNCER);
	case (OPTION::FALLING_HAZARD): return colliding_entity_type_to_name(COLLIDING_ENTITY_SUBTYPE::FALLING_HAZARD);
	case (OPTION::PRESSURE_ORB): return colliding_entity_type_to_name(COLLIDING_ENTITY_SUBTYPE::PRESSURE_ORB);
	case (OPTION::ROAMING_TRAMPOLINE): return walking_entity_type_to_name(WALKING_ENTITY_SUBTYPE::ROAMING_TRAMPOLINE);
	case (OPTION::SCOUTER): return walking_entity_type_to_name(WALKING_ENTITY_SUBTYPE::SCOUTER);
	default: return "";
	}
}

int State_LCOS_Entity::cursor_index(sf::Vector2i pos)
{
	int index_x = (pos.x - static_cast<int>(options.getGlobalBounds().left)) / 64;
	int index_y = (pos.y - static_cast<int>(options.getGlobalBounds().top)) / 64;
	return index_x + 7 * index_y;
}

void State_LCOS_Entity::handle_events(const sf::Event& event)
{
	if (event.type == sf::Event::MouseButtonPressed && event.key.code == sf::Mouse::Left)
		if (auto pos = sf::Mouse::getPosition(); options.getGlobalBounds().contains(pos.x, pos.y))
		{
			int index = cursor_index(pos);

			if (!can_choose_option(static_cast<OPTION>(index)) || !(index < static_cast<int>(OPTION::COUNT)))
				return;

			app_handle.request_pop();

			switch (static_cast<OPTION>(index))
			{
			case (OPTION::TRAMPOLINE):
				app_handle.register_scpp(std::make_unique<SCPP_LCOS_Trampoline>(static_cast<int32_t>(Trampoline::STRENGTH::WEAK)));
				break;
			case (OPTION::STRONG_TRAMPOLINE):
				app_handle.register_scpp(std::make_unique<SCPP_LCOS_Trampoline>(static_cast<int32_t>(Trampoline::STRENGTH::STRONG)));
				break;
			case (OPTION::THORN):
				app_handle.request_push(App::STATE::LEVEL_CREATOR_OBJECT_SELECTION_THORN);
				break;
			case (OPTION::THORN_SPEAR):
				app_handle.register_scpp(std::make_unique<SCPP_LCOS_Stationary_Entity>(STATIONARY_ENTITY_SUBTYPE::THORN_SPEAR, 32));
				break;
			case (OPTION::MINE):
				app_handle.register_scpp(std::make_unique<SCPP_LCOS_Stationary_Entity>(STATIONARY_ENTITY_SUBTYPE::MINE, 32));
				break;
			case (OPTION::HEALING_PLANT):
				app_handle.register_scpp(std::make_unique<SCPP_LCOS_Stationary_Entity>(STATIONARY_ENTITY_SUBTYPE::HEALING_PLANT, 32));
				break;
			case (OPTION::WIND_BLASTER):
				app_handle.register_scpp(std::make_unique<SCPP_LCOS_Stationary_Entity>(STATIONARY_ENTITY_SUBTYPE::WIND_BLASTER, 32));
				break;
			case (OPTION::LIVING_SPIKES):
				app_handle.register_scpp(std::make_unique<SCPP_LCOS_Stationary_Entity>(STATIONARY_ENTITY_SUBTYPE::LIVING_SPIKES, 32));
				break;
			case (OPTION::CANNON_LEFT):
				app_handle.register_scpp(std::make_unique<SCPP_LCOS_Cannon>(static_cast<int32_t>(Bullet::DIRECTION::LEFT)));
				break;
			case (OPTION::CANNON_DOWN):
				app_handle.register_scpp(std::make_unique<SCPP_LCOS_Cannon>(static_cast<int32_t>(Bullet::DIRECTION::DOWN)));
				break;
			case (OPTION::CANNON_RIGHT):
				app_handle.register_scpp(std::make_unique<SCPP_LCOS_Cannon>(static_cast<int32_t>(Bullet::DIRECTION::RIGHT)));
				break;
			case (OPTION::ROCK_DISPOSER):
				app_handle.register_scpp(std::make_unique<SCPP_LCOS_Colliding_Entity>(COLLIDING_ENTITY_SUBTYPE::ROCK_DISPOSER, 64));
				break;
			case (OPTION::ICE_LAUNCHER_LEFT):
				app_handle.register_scpp(std::make_unique<SCPP_LCOS_Ice_Launcher>(static_cast<int32_t>(Ice_Chunk::DIRECTION::LEFT)));
				break;
			case (OPTION::ICE_LAUNCHER_RIGHT):
				app_handle.register_scpp(std::make_unique<SCPP_LCOS_Ice_Launcher>(static_cast<int32_t>(Ice_Chunk::DIRECTION::RIGHT)));
				break;
			case (OPTION::BUBBLE_MACHINE):
				app_handle.register_scpp(std::make_unique<SCPP_LCOS_Colliding_Entity>(COLLIDING_ENTITY_SUBTYPE::BUBBLE_MACHINE, 32));
				break;
			case (OPTION::BIONIC_RAY_LEFT):
				app_handle.register_scpp(std::make_unique<SCPP_LCOS_Bionic_Ray_Device>(static_cast<int32_t>(Bionic_Ray_Device::DIRECTION::LEFT)));
				break;
			case (OPTION::BIONIC_RAY_DOWN):
				app_handle.register_scpp(std::make_unique<SCPP_LCOS_Bionic_Ray_Device>(static_cast<int32_t>(Bionic_Ray_Device::DIRECTION::DOWN)));
				break;
			case (OPTION::BIONIC_RAY_RIGHT):
				app_handle.register_scpp(std::make_unique<SCPP_LCOS_Bionic_Ray_Device>(static_cast<int32_t>(Bionic_Ray_Device::DIRECTION::RIGHT)));
				break;
			case (OPTION::ELECTRIC_GATE_LEFT):
				app_handle.register_scpp(std::make_unique<SCPP_LCOS_Electric_Gate_Device>(static_cast<int32_t>(Electric_Gate_Device::DIRECTION::LEFT)));
				break;
			case (OPTION::ELECTRIC_GATE_DOWN):
				app_handle.register_scpp(std::make_unique<SCPP_LCOS_Electric_Gate_Device>(static_cast<int32_t>(Electric_Gate_Device::DIRECTION::DOWN)));
				break;
			case (OPTION::ELECTRIC_GATE_RIGHT):
				app_handle.register_scpp(std::make_unique<SCPP_LCOS_Electric_Gate_Device>(static_cast<int32_t>(Electric_Gate_Device::DIRECTION::RIGHT)));
				break;
			case (OPTION::FLAMETHROWER):
				app_handle.register_scpp(std::make_unique<SCPP_LCOS_Colliding_Entity>(COLLIDING_ENTITY_SUBTYPE::FLAMETHROWER, 32));
				break;
			case (OPTION::WARRIOR):
				app_handle.register_scpp(std::make_unique<SCPP_LCOS_Walking_Entity>(WALKING_ENTITY_SUBTYPE::WARRIOR, 48));
				break;
			case (OPTION::CASTER):
				app_handle.register_scpp(std::make_unique<SCPP_LCOS_Colliding_Entity>(COLLIDING_ENTITY_SUBTYPE::CASTER, 48));
				break;
			case (OPTION::ELEMENTAL):
				app_handle.register_scpp(std::make_unique<SCPP_LCOS_Colliding_Entity>(COLLIDING_ENTITY_SUBTYPE::ELEMENTAL, 32));
				break;
			case (OPTION::GOLEM):
				app_handle.register_scpp(std::make_unique<SCPP_LCOS_Walking_Entity>(WALKING_ENTITY_SUBTYPE::GOLEM, 48));
				break;
			case (OPTION::JUNGLE_BOOMERANGER):
				app_handle.register_scpp(std::make_unique<SCPP_LCOS_Colliding_Entity>(COLLIDING_ENTITY_SUBTYPE::JUNGLE_BOOMERANGER, 48));
				break;
			case (OPTION::AQUAVEIL_WEAVER):
				app_handle.register_scpp(std::make_unique<SCPP_LCOS_Colliding_Entity>(COLLIDING_ENTITY_SUBTYPE::AQUAVEIL_WEAVER, 48));
				break;
			case (OPTION::SCORCHER):
				app_handle.register_scpp(std::make_unique<SCPP_LCOS_Walking_Entity>(WALKING_ENTITY_SUBTYPE::SCORCHER, 48));
				break;
			case (OPTION::ENERGIZED_GALVANIZER):
				app_handle.register_scpp(std::make_unique<SCPP_LCOS_Colliding_Entity>(COLLIDING_ENTITY_SUBTYPE::ENERGIZED_GALVANIZER, 32));
				break;
			case (OPTION::AERIAL_BOMBARDIER):
				app_handle.register_scpp(std::make_unique<SCPP_LCOS_Colliding_Entity>(COLLIDING_ENTITY_SUBTYPE::AERIAL_BOMBARDIER, 48));
				break;
			case (OPTION::ROCK_HURLER):
				app_handle.register_scpp(std::make_unique<SCPP_LCOS_Colliding_Entity>(COLLIDING_ENTITY_SUBTYPE::ROCK_HURLER, 48));
				break;
			case (OPTION::CRIMSON_LASHER):
				app_handle.register_scpp(std::make_unique<SCPP_LCOS_Colliding_Entity>(COLLIDING_ENTITY_SUBTYPE::CRIMSON_LASHER, 48));
				break;
			case (OPTION::ICEBANER):
				app_handle.register_scpp(std::make_unique<SCPP_LCOS_Colliding_Entity>(COLLIDING_ENTITY_SUBTYPE::ICEBANER, 48));
				break;
			case (OPTION::BOUNCER):
				app_handle.register_scpp(std::make_unique<SCPP_LCOS_Colliding_Entity>(COLLIDING_ENTITY_SUBTYPE::BOUNCER, 32));
				break;
			case (OPTION::FALLING_HAZARD):
				app_handle.register_scpp(std::make_unique<SCPP_LCOS_Colliding_Entity>(COLLIDING_ENTITY_SUBTYPE::FALLING_HAZARD, 32));
				break;
			case (OPTION::PRESSURE_ORB):
				app_handle.register_scpp(std::make_unique<SCPP_LCOS_Colliding_Entity>(COLLIDING_ENTITY_SUBTYPE::PRESSURE_ORB, 32));
				break;
			case (OPTION::ROAMING_TRAMPOLINE):
				app_handle.register_scpp(std::make_unique<SCPP_LCOS_Walking_Entity>(WALKING_ENTITY_SUBTYPE::ROAMING_TRAMPOLINE, 32));
				break;
			case (OPTION::SCOUTER):
				app_handle.register_scpp(std::make_unique<SCPP_LCOS_Walking_Entity>(WALKING_ENTITY_SUBTYPE::SCOUTER, 32));
			}
		}
}

void State_LCOS_Entity::update(const sf::Time& dt)
{
	auto pos = sf::Mouse::getPosition();

	if (auto pos = sf::Mouse::getPosition(); options.getGlobalBounds().contains(pos.x, pos.y))
	{
		int index = cursor_index(pos);
		info.setString(option_to_string(static_cast<OPTION>(index)));
		adjust_text_height(info);
	}
	else
		info.setString("");
}

State* State_LCOS_Entity::make_state(App& app_handle)
{
	return new State_LCOS_Entity(app_handle);
}

// ----------------------------------------------------------------
// State_LCOS_Thorn
// ----------------------------------------------------------------

State_LCOS_Thorn::State_LCOS_Thorn(App& arg_app_handle) : 
	State_LCOS_Base(arg_app_handle),
	cursor_highlight(32, 128)
{
	auto& resources = app_handle.get_shared_context().resources;

	label.setString("Which Thorn shape?");
	set_center(label);

	options.setTexture(resources.textures.get(RESOURCE_ID::TEXTURES::THORN));
	set_center(options);

	adjust_to_resolution();
}

void State_LCOS_Thorn::handle_events(const sf::Event& event)
{
	if (event.type == sf::Event::MouseButtonPressed && event.key.code == sf::Mouse::Left)
		if (auto pos = sf::Mouse::getPosition(); options.getGlobalBounds().contains(pos.x, pos.y))
		{
			int index = (pos.x - static_cast<int>(options.getGlobalBounds().left)) / 32;
			if (index >= 0 && index <= max_index)
			{
				app_handle.register_scpp(std::make_unique<SCPP_LCOS_Thorn>(index));
				app_handle.request_pop();
			}
		}
}

void State_LCOS_Thorn::update(const sf::Time& dt) {}

void State_LCOS_Thorn::render()
{
	State_LCOS_Base::render();

	if (options.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition())))
	{
		sf::Vector2i options_pos = sf::Vector2i(options.getPosition());
		options_pos.x -= options.getGlobalBounds().width * 0.5f;
		options_pos.y -= options.getGlobalBounds().height * 0.5f;
		cursor_highlight.draw_at(app_handle.get_shared_context().window, options_pos, sf::Mouse::getPosition() - options_pos);
	}
}

State* State_LCOS_Thorn::make_state(App& app_handle)
{
	return new State_LCOS_Thorn(app_handle);
}