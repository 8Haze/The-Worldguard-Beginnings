#ifndef _LEVEL_CONTENT_H_
#define _LEVEL_CONTENT_H_

#include <sstream>

#include "app.h"
#include "camera.h"
#include "level_object_id.h"
#include "lo_bases.h"
#include "lo_tiles.h"
#include "lo_items.h"
#include "lo_entities.h"
#include "player.h"

class Level_Content
{
protected:

	enum class ITERATOR_RANGE
	{
		CLOSE_TO_PLAYER,
		VISIBLE_STATIONARY,
		VISIBLE_COLLIDING,
		VISIBLE_WALKING,
	};

	typedef Decorative_Tile* (*Decorative_Tile_Factory)(LEVEL_THEME, Resources&);
	typedef Animated_Tile* (*Animated_Tile_Factory)();
	typedef Solid_Tile* (*Solid_Tile_Factory)(LEVEL_THEME, Resources&);
	typedef Destructible_Tile* (*Destructible_Tile_Factory)(LEVEL_THEME, Resources&);
	typedef Interactive_Tile* (*Interactive_Tile_Factory)();
	typedef Item* (*Item_Factory)();
	typedef Stationary_Entity* (*Stationary_Entity_Factory)(LEVEL_THEME);
	typedef Colliding_Entity* (*Colliding_Entity_Factory)(LEVEL_THEME);
	typedef Walking_Entity* (*Walking_Entity_Factory)(LEVEL_THEME);

	Level_Properties level_properties;

	std::map<DECORATIVE_TILE_SUBTYPE, Decorative_Tile_Factory> decorative_tile_factory_registry;
	std::map<ANIMATED_TILE_SUBTYPE, Animated_Tile_Factory> animated_tile_factory_registry;
	std::map<SOLID_TILE_SUBTYPE, Solid_Tile_Factory> solid_tile_factory_registry;
	std::map<DESTRUCTIBLE_TILE_SUBTYPE, Destructible_Tile_Factory> destructible_tile_factory_registry;
	std::map<INTERACTIVE_TILE_SUBTYPE, Interactive_Tile_Factory> interactive_tile_factory_registry;
	std::map<ITEM_SUBTYPE, Item_Factory> item_factory_registry;
	std::map<STATIONARY_ENTITY_SUBTYPE, Stationary_Entity_Factory> stationary_entity_factory_registry;
	std::map<COLLIDING_ENTITY_SUBTYPE, Colliding_Entity_Factory> colliding_entity_factory_registry;
	std::map<WALKING_ENTITY_SUBTYPE, Walking_Entity_Factory> walking_entity_factory_registry;

	hct::dynarray<std::unique_ptr<Decorative_Tile>> decorative_tiles;
	hct::dynarray<std::unique_ptr<Animated_Tile>> animated_tiles;
	hct::dynarray<std::unique_ptr<Solid_Tile>> solid_tiles;
	hct::dynarray<std::unique_ptr<Destructible_Tile>> destructible_tiles;
	hct::dynarray<std::unique_ptr<Interactive_Tile>> interactive_tiles;
	hct::dynarray<std::unique_ptr<Item>> items;
	hct::dynarray<std::unique_ptr<Stationary_Entity>> stationary_entities;
	hct::dynarray<std::unique_ptr<Colliding_Entity>> colliding_entities;
	hct::dynarray<std::unique_ptr<Walking_Entity>> walking_entities;

	Player player;

	sf::RenderTexture render_texture;
	sf::Sprite render_sprite;
	Camera camera;

	Level_Content(App& arg_app_handle);

	template <typename T, typename Container, typename Factory, typename... Args>
	void generate_object(App& app_handle, std::istream& source_stream, Container& container, Factory factory, Args&&... args)
	{
		std::unique_ptr<T> object(std::move(factory(std::forward<Args>(args)...)));
		object->setup(source_stream, app_handle.get_shared_context().resources);
		container.push_back(std::move(object));
	}

	template <typename Container, typename Iterator = typename Container::const_iterator>
	std::pair<Iterator, Iterator> find_iterator_range(const Container& container, ITERATOR_RANGE iterator_range, size_t back_offset = 0)
	{
		std::pair<Iterator, Iterator> result;
		float left, right;

		switch (iterator_range)
		{
		case(ITERATOR_RANGE::CLOSE_TO_PLAYER):
		{
			left = player.get_position().x - PLAYER_PROXIMITY_DISTANCE - 32;
			right = player.get_position().x + PLAYER_PROXIMITY_DISTANCE;
			break;
		}
		case(ITERATOR_RANGE::VISIBLE_STATIONARY):
		{
			left = camera.getCenter().x - camera.getSize().x * 0.5f - 64;
			right = camera.getCenter().x + camera.getSize().x * 0.5f + 32;
			break;
		}
		case(ITERATOR_RANGE::VISIBLE_COLLIDING):
		{
			left = camera.getCenter().x - camera.getSize().x * 0.5f - COLLIDING_ENTITY_RENDER_DISTANCE - 32;
			right = camera.getCenter().x + camera.getSize().x * 0.5f + COLLIDING_ENTITY_RENDER_DISTANCE;
			break;
		}
		case(ITERATOR_RANGE::VISIBLE_WALKING):
		{
			left = camera.getCenter().x - camera.getSize().x * 0.5f - WALKING_ENTITY_RENDER_DISTANCE - 32;
			right = camera.getCenter().x + camera.getSize().x * 0.5f + WALKING_ENTITY_RENDER_DISTANCE;
			break;
		}
		default: throw std::runtime_error("Level_Content::get_iterator_range: unrecognized range");
		}

		auto comparing_lambda = [](const auto& lhs, const auto& rhs) { return *lhs < rhs; };
		result.first = std::lower_bound(container.begin(), container.end() - back_offset, left, comparing_lambda);
		result.second = std::lower_bound(container.begin(), container.end() - back_offset, right, comparing_lambda);

		return result;
	}

	void register_objects();
	void sort_objects();
	void create_new_object(App& app_handle, std::istream& source_stream, int32_t type, int32_t subtype);
	void load_everything_from_file(App& app_handle);
	void load_objects_from_string_stream(App& app_handle, std::stringstream& stream);

};

#endif // _LEVEL_CONTENT_H_