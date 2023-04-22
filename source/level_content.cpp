#include "level_content.h"

Level_Content::Level_Content(App& arg_app_handle) :
	level_properties(dynamic_cast<SCPP_Level_Properties&>(*arg_app_handle.retrieve_scpp()).lp),
	player(arg_app_handle.get_shared_context().resources)
{
	register_objects();
}

void Level_Content::register_objects()
{
	decorative_tile_factory_registry.emplace(DECORATIVE_TILE_SUBTYPE::REGULAR, Decorative_Tile::make_lo);

	animated_tile_factory_registry.emplace(ANIMATED_TILE_SUBTYPE::REGULAR, Animated_Tile::make_lo);

	solid_tile_factory_registry.emplace(SOLID_TILE_SUBTYPE::REGULAR, Solid_Tile::make_lo);

	destructible_tile_factory_registry.emplace(DESTRUCTIBLE_TILE_SUBTYPE::REGULAR, Destructible_Tile::make_lo);
	destructible_tile_factory_registry.emplace(DESTRUCTIBLE_TILE_SUBTYPE::FRAGILE, Fragile_Tile::make_lo);

	interactive_tile_factory_registry.emplace(INTERACTIVE_TILE_SUBTYPE::HAZARDOUS, Hazardous_Tile::make_lo);
	interactive_tile_factory_registry.emplace(INTERACTIVE_TILE_SUBTYPE::INFO_SIGN, Info_Sign_Tile::make_lo);
	interactive_tile_factory_registry.emplace(INTERACTIVE_TILE_SUBTYPE::ICE, Ice_Tile::make_lo);
	interactive_tile_factory_registry.emplace(INTERACTIVE_TILE_SUBTYPE::HOOK, Hook_Tile::make_lo);
	interactive_tile_factory_registry.emplace(INTERACTIVE_TILE_SUBTYPE::COLD_AIR, Cold_Air_Tile::make_lo);
	interactive_tile_factory_registry.emplace(INTERACTIVE_TILE_SUBTYPE::HOT_AIR, Hot_Air_Tile::make_lo);
	interactive_tile_factory_registry.emplace(INTERACTIVE_TILE_SUBTYPE::ANTI_GRAVITY, Anti_Gravity_Tile::make_lo);
	interactive_tile_factory_registry.emplace(INTERACTIVE_TILE_SUBTYPE::CONVEYOR_BELT, Conveyor_Belt_Tile::make_lo);
	interactive_tile_factory_registry.emplace(INTERACTIVE_TILE_SUBTYPE::WARP, Warp_Tile::make_lo);
	interactive_tile_factory_registry.emplace(INTERACTIVE_TILE_SUBTYPE::FINISH_WARP, Finish_Warp_Tile::make_lo);
	interactive_tile_factory_registry.emplace(INTERACTIVE_TILE_SUBTYPE::WATER, Water_Tile::make_lo);

	item_factory_registry.emplace(ITEM_SUBTYPE::FOOD, Food::make_lo);
	item_factory_registry.emplace(ITEM_SUBTYPE::GEM, Gem::make_lo);
	item_factory_registry.emplace(ITEM_SUBTYPE::COIN, Coin::make_lo);
	item_factory_registry.emplace(ITEM_SUBTYPE::LIFEUP, Lifeup::make_lo);

	stationary_entity_factory_registry.emplace(STATIONARY_ENTITY_SUBTYPE::TRAMPOLINE, Trampoline::make_lo);
	stationary_entity_factory_registry.emplace(STATIONARY_ENTITY_SUBTYPE::WIND_BLASTER, Wind_Blaster::make_lo);
	stationary_entity_factory_registry.emplace(STATIONARY_ENTITY_SUBTYPE::MINE, Mine::make_lo);
	stationary_entity_factory_registry.emplace(STATIONARY_ENTITY_SUBTYPE::LIVING_SPIKES, Living_Spikes::make_lo);
	stationary_entity_factory_registry.emplace(STATIONARY_ENTITY_SUBTYPE::HEALING_PLANT, Healing_Plant::make_lo);
	stationary_entity_factory_registry.emplace(STATIONARY_ENTITY_SUBTYPE::THORN, Thorn::make_lo);
	stationary_entity_factory_registry.emplace(STATIONARY_ENTITY_SUBTYPE::THORN_SPEAR, Thorn_Spear_Root::make_lo);

	colliding_entity_factory_registry.emplace(COLLIDING_ENTITY_SUBTYPE::BOUNCER, Bouncer::make_lo);
	colliding_entity_factory_registry.emplace(COLLIDING_ENTITY_SUBTYPE::CANNON, Cannon::make_lo);
	colliding_entity_factory_registry.emplace(COLLIDING_ENTITY_SUBTYPE::FALLING_HAZARD, Falling_Hazard::make_lo);
	colliding_entity_factory_registry.emplace(COLLIDING_ENTITY_SUBTYPE::ROCK_DISPOSER, Rock_Disposer::make_lo);
	colliding_entity_factory_registry.emplace(COLLIDING_ENTITY_SUBTYPE::ICE_LAUNCHER, Ice_Launcher::make_lo);
	colliding_entity_factory_registry.emplace(COLLIDING_ENTITY_SUBTYPE::PRESSURE_ORB, Pressure_Orb::make_lo);
	colliding_entity_factory_registry.emplace(COLLIDING_ENTITY_SUBTYPE::BUBBLE_MACHINE, Bubble_Machine::make_lo);
	colliding_entity_factory_registry.emplace(COLLIDING_ENTITY_SUBTYPE::BIONIC_RAY_DEVICE, Bionic_Ray_Device::make_lo);
	colliding_entity_factory_registry.emplace(COLLIDING_ENTITY_SUBTYPE::ELECTRIC_GATE_DEVICE, Electric_Gate_Device::make_lo);
	colliding_entity_factory_registry.emplace(COLLIDING_ENTITY_SUBTYPE::FLAMETHROWER, Flamethrower::make_lo);
	colliding_entity_factory_registry.emplace(COLLIDING_ENTITY_SUBTYPE::CASTER, Caster::make_lo);
	colliding_entity_factory_registry.emplace(COLLIDING_ENTITY_SUBTYPE::ELEMENTAL, Elemental::make_lo);
	colliding_entity_factory_registry.emplace(COLLIDING_ENTITY_SUBTYPE::ICEBANER, Icebaner::make_lo);
	colliding_entity_factory_registry.emplace(COLLIDING_ENTITY_SUBTYPE::CRIMSON_LASHER, Crimson_Lasher::make_lo);
	colliding_entity_factory_registry.emplace(COLLIDING_ENTITY_SUBTYPE::AQUAVEIL_WEAVER, Aquaveil_Weaver::make_lo);
	colliding_entity_factory_registry.emplace(COLLIDING_ENTITY_SUBTYPE::ROCK_HURLER, Rock_Hurler::make_lo);
	colliding_entity_factory_registry.emplace(COLLIDING_ENTITY_SUBTYPE::AERIAL_BOMBARDIER, Aerial_Bombardier::make_lo);
	colliding_entity_factory_registry.emplace(COLLIDING_ENTITY_SUBTYPE::JUNGLE_BOOMERANGER, Jungle_Boomeranger::make_lo);
	colliding_entity_factory_registry.emplace(COLLIDING_ENTITY_SUBTYPE::ENERGIZED_GALVANIZER, Energized_Galvanizer::make_lo);

	walking_entity_factory_registry.emplace(WALKING_ENTITY_SUBTYPE::ROAMING_TRAMPOLINE, Roaming_Trampoline::make_lo);
	walking_entity_factory_registry.emplace(WALKING_ENTITY_SUBTYPE::SCOUTER, Scouter::make_lo);
	walking_entity_factory_registry.emplace(WALKING_ENTITY_SUBTYPE::WARRIOR, Warrior::make_lo);
	walking_entity_factory_registry.emplace(WALKING_ENTITY_SUBTYPE::GOLEM, Golem::make_lo);
	walking_entity_factory_registry.emplace(WALKING_ENTITY_SUBTYPE::SCORCHER, Scorcher::make_lo);
}

void Level_Content::sort_objects()
{
	auto sort_lambda = [](const auto& lhs, const auto& rhs) { return *lhs < *rhs; };
	hct::sort(decorative_tiles.begin(), decorative_tiles.end(), sort_lambda);
	hct::sort(animated_tiles.begin(), animated_tiles.end(), sort_lambda);
	hct::sort(solid_tiles.begin(), solid_tiles.end(), sort_lambda);
	hct::sort(destructible_tiles.begin(), destructible_tiles.end(), sort_lambda);
	hct::sort(interactive_tiles.begin(), interactive_tiles.end(), sort_lambda);
	hct::sort(items.begin(), items.end(), sort_lambda);
	hct::sort(stationary_entities.begin(), stationary_entities.end(), sort_lambda);
	hct::sort(colliding_entities.begin(), colliding_entities.end(), sort_lambda);
	hct::sort(walking_entities.begin(), walking_entities.end(), sort_lambda);
}

void Level_Content::create_new_object(App& app_handle, std::istream& source_stream, int32_t type, int32_t subtype)
{
	auto& resources = app_handle.get_shared_context().resources;
	
	switch (static_cast<LO_TYPE>(type))
	{
	case(LO_TYPE::DECORATIVE_TILE): 
		generate_object<Decorative_Tile>(app_handle, source_stream, decorative_tiles, 
			decorative_tile_factory_registry.at(static_cast<DECORATIVE_TILE_SUBTYPE>(subtype)), level_properties.theme, resources); 
		break;
	case(LO_TYPE::ANIMATED_TILE): 
		generate_object<Animated_Tile>(app_handle, source_stream, animated_tiles,
			animated_tile_factory_registry.at(static_cast<ANIMATED_TILE_SUBTYPE>(subtype)));
		break;
	case(LO_TYPE::SOLID_TILE):
		generate_object<Solid_Tile>(app_handle, source_stream, solid_tiles,
			solid_tile_factory_registry.at(static_cast<SOLID_TILE_SUBTYPE>(subtype)), level_properties.theme, resources);
		break;
	case(LO_TYPE::DESTRUCTIBLE_TILE): 
		generate_object<Destructible_Tile>(app_handle, source_stream, destructible_tiles, 
			destructible_tile_factory_registry.at(static_cast<DESTRUCTIBLE_TILE_SUBTYPE>(subtype)), level_properties.theme, resources); 
		break;
	case(LO_TYPE::INTERACTIVE_TILE):
		generate_object<Interactive_Tile>(app_handle, source_stream, interactive_tiles,
			interactive_tile_factory_registry.at(static_cast<INTERACTIVE_TILE_SUBTYPE>(subtype))); 
		break;
	case(LO_TYPE::ITEM):
		generate_object<Item>(app_handle, source_stream, items, 
			item_factory_registry.at(static_cast<ITEM_SUBTYPE>(subtype))); 
		break;
	case(LO_TYPE::STATIONARY_ENTITY):
		generate_object<Stationary_Entity>(app_handle, source_stream, stationary_entities, 
			stationary_entity_factory_registry.at(static_cast<STATIONARY_ENTITY_SUBTYPE>(subtype)), level_properties.theme); 
		break;
	case(LO_TYPE::COLLIDING_ENTITY): 
		generate_object<Colliding_Entity>(app_handle, source_stream, colliding_entities, 
			colliding_entity_factory_registry.at(static_cast<COLLIDING_ENTITY_SUBTYPE>(subtype)), level_properties.theme);
		break;
	case(LO_TYPE::WALKING_ENTITY):
		generate_object<Walking_Entity>(app_handle, source_stream, walking_entities, 
			walking_entity_factory_registry.at(static_cast<WALKING_ENTITY_SUBTYPE>(subtype)), level_properties.theme);
		break;
	default: throw std::invalid_argument("Level_Content::create_new_object: corrupted data in .wgl file");
	}
}

void Level_Content::load_everything_from_file(App& app_handle)
{
	auto& resources = app_handle.get_shared_context().resources;
	
	std::ifstream file(level_properties.path_main, std::ios::binary);
	if (!file.is_open())
		throw std::runtime_error("Level_Content::load_objects: could not open desired .wgl file");
	auto guard_file_close = hct::make_guard_exit([&file]() { file.close(); });

	int32_t player_pos_x, player_pos_y, type, subtype;
	data_reader(file, player_pos_x);
	data_reader(file, player_pos_y);
	player.set_position({ static_cast<float>(player_pos_x), static_cast<float>(player_pos_y) });

	while (true)
	{
		data_reader(file, type);
		data_reader(file, subtype);

		if (file.eof())
			break;
		if (file.fail())
			throw std::invalid_argument("Level_Content::load_objects: corrupted data in .wgl file");

		create_new_object(app_handle, file, type, subtype);
	}
}

void Level_Content::load_objects_from_string_stream(App& app_handle, std::stringstream& stream)
{
	auto& resources = app_handle.get_shared_context().resources;
	int32_t type, subtype;

	while (true)
	{
		data_reader(stream, type);
		data_reader(stream, subtype);

		if (stream.eof())
			break;
		else if (stream.fail())
			throw std::invalid_argument("Level_Content::load_objects: corrupted data in .wgl file");

		create_new_object(app_handle, stream, type, subtype);
	}
}