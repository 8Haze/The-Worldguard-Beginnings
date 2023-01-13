#include "state_lcos_item.h"

#include "level_object_id.h"

// ----------------------------------------------------------------
// State_LCOS_Item
// ----------------------------------------------------------------

State_LCOS_Item::State_LCOS_Item(App& arg_app_handle) : State_LCOS_Base(arg_app_handle)
{
	auto& resources = app_handle.get_shared_context().resources;

	label.setString("Which Item type?");
	set_center(label);

	options.setTexture(resources.textures.get(RESOURCE_ID::TEXTURES::LCOS_ITEM));
	set_center(options);

	adjust_to_resolution();
}

int State_LCOS_Item::cursor_index(sf::Vector2i pos)
{
	return (pos.x - static_cast<int>(options.getGlobalBounds().left)) / 64;
}

void State_LCOS_Item::handle_events(const sf::Event& event)
{
	if (event.type == sf::Event::MouseButtonPressed && event.key.code == sf::Mouse::Left)
		if (auto pos = sf::Mouse::getPosition(); options.getGlobalBounds().contains(pos.x, pos.y))
		{
			switch (static_cast<ITEM_SUBTYPE>(cursor_index(pos)))
			{
			case (ITEM_SUBTYPE::FOOD):
				app_handle.request_pop(); 
				app_handle.request_push(App::STATE::LEVEL_CREATOR_OBJECT_SELECTION_FOOD);
				break;
			case (ITEM_SUBTYPE::GEM):
				app_handle.request_pop(); 
				app_handle.request_push(App::STATE::LEVEL_CREATOR_OBJECT_SELECTION_GEM);
				break;
			case (ITEM_SUBTYPE::COIN): 
				app_handle.request_pop(); 
				app_handle.request_push(App::STATE::LEVEL_CREATOR_OBJECT_SELECTION_COIN); 
				break;
			case (ITEM_SUBTYPE::LIFEUP):
				app_handle.request_pop(); 
				app_handle.request_push(App::STATE::LEVEL_CREATOR_OBJECT_SELECTION_LIFEUP);
				break;
			}
		}
}

void State_LCOS_Item::update(const sf::Time& dt)
{
	auto pos = sf::Mouse::getPosition();

	if (auto pos = sf::Mouse::getPosition(); options.getGlobalBounds().contains(pos.x, pos.y))
	{
		switch (static_cast<ITEM_SUBTYPE>(cursor_index(pos)))
		{
		case (ITEM_SUBTYPE::FOOD): info.setString("Food"); break;
		case (ITEM_SUBTYPE::GEM): info.setString("Gems"); break;
		case (ITEM_SUBTYPE::COIN): info.setString("Coins"); break;
		case (ITEM_SUBTYPE::LIFEUP): info.setString("Lifeups"); break;
		}
		adjust_text_height(info);
	}
	else
		info.setString("");
}

State* State_LCOS_Item::make_state(App& app_handle)
{
	return new State_LCOS_Item(app_handle);
}

// ----------------------------------------------------------------
// State_LCOS_Food
// ----------------------------------------------------------------

State_LCOS_Food::State_LCOS_Food(App& arg_app_handle) : State_LCOS_Base(arg_app_handle)
{
	auto& resources = app_handle.get_shared_context().resources;

	label.setString("Which Food type?");
	set_center(label);

	options.setTexture(resources.textures.get(RESOURCE_ID::TEXTURES::LCOS_FOOD));
	set_center(options);

	adjust_to_resolution();
}

std::tuple<int, int, int> State_LCOS_Food::cursor_index(sf::Vector2i pos)
{
	int index_x = (pos.x - static_cast<int>(options.getGlobalBounds().left)) / 32;
	int index_y = (pos.y - static_cast<int>(options.getGlobalBounds().top)) / 32;
	int index = index_x + 10 * index_y;
	return { index_x, index_y, index };
}

void State_LCOS_Food::handle_events(const sf::Event& event)
{
	if (event.type == sf::Event::MouseButtonPressed && event.key.code == sf::Mouse::Left)
		if (auto pos = sf::Mouse::getPosition(); options.getGlobalBounds().contains(pos.x, pos.y))
		{
			auto [index_x, index_y, index] = cursor_index(pos);
			if (index >= 0 && index <= max_index)
			{
				app_handle.register_scpp(std::make_unique<SCPP_LCOS_Food>(index_x, index_y));
				app_handle.request_pop();
			}
		}
}

void State_LCOS_Food::update(const sf::Time& dt)
{
	auto pos = sf::Mouse::getPosition();

	if (auto pos = sf::Mouse::getPosition(); options.getGlobalBounds().contains(pos.x, pos.y))
	{
		auto [index_x, index_y, index] = cursor_index(pos);
		info.setString(make_string(index_to_food_type_name(index), " (worth ", std::to_string(food_value_by_index(index_y)), ")"));
		adjust_text_height(info);
	}
	else
		info.setString("");
}

State* State_LCOS_Food::make_state(App& app_handle)
{
	return new State_LCOS_Food(app_handle);
}

// ----------------------------------------------------------------
// State_LCOS_Gem
// ----------------------------------------------------------------

State_LCOS_Gem::State_LCOS_Gem(App& arg_app_handle) : State_LCOS_Base(arg_app_handle)
{
	auto& resources = app_handle.get_shared_context().resources;

	label.setString("Which Gem type?");
	set_center(label);

	options.setTexture(resources.textures.get(RESOURCE_ID::TEXTURES::LCOS_GEM));
	set_center(options);

	adjust_to_resolution();
}

int State_LCOS_Gem::cursor_index(sf::Vector2i pos)
{
	int index_x = (pos.x - static_cast<int>(options.getGlobalBounds().left)) / 64;
	int index_y = (pos.y - static_cast<int>(options.getGlobalBounds().top)) / 64;
	return index_x + 4 * index_y;
}

void State_LCOS_Gem::handle_events(const sf::Event& event)
{
	if (event.type == sf::Event::MouseButtonPressed && event.key.code == sf::Mouse::Left)
		if (auto pos = sf::Mouse::getPosition(); options.getGlobalBounds().contains(pos.x, pos.y))
		{
			int index = cursor_index(pos);
			if (index >= 0 && index <= max_index)
			{
				app_handle.register_scpp(std::make_unique<SCPP_LCOS_Gem>(index));
				app_handle.request_pop();
			}
		}
}

void State_LCOS_Gem::update(const sf::Time& dt)
{
	auto pos = sf::Mouse::getPosition();

	if (auto pos = sf::Mouse::getPosition(); options.getGlobalBounds().contains(pos.x, pos.y))
	{
		int index = cursor_index(pos);
		info.setString(make_string(index_to_gem_color_name(index), " (worth ", std::to_string(gem_value_by_index(index)), ")"));
		adjust_text_height(info);
	}
	else
		info.setString("");
}

State* State_LCOS_Gem::make_state(App& app_handle)
{
	return new State_LCOS_Gem(app_handle);
}

// ----------------------------------------------------------------
// State_LCOS_Coin
// ----------------------------------------------------------------

State_LCOS_Coin::State_LCOS_Coin(App& arg_app_handle) : State_LCOS_Base(arg_app_handle)
{
	auto& resources = app_handle.get_shared_context().resources;

	label.setString("Which Coin type?");
	set_center(label);

	options.setTexture(resources.textures.get(RESOURCE_ID::TEXTURES::LCOS_COIN));
	set_center(options);

	adjust_to_resolution();
}

int State_LCOS_Coin::cursor_index(sf::Vector2i pos)
{
	return (pos.x - static_cast<int>(options.getGlobalBounds().left)) / 64;
}

void State_LCOS_Coin::handle_events(const sf::Event& event)
{
	if (event.type == sf::Event::MouseButtonPressed && event.key.code == sf::Mouse::Left)
		if (auto pos = sf::Mouse::getPosition(); options.getGlobalBounds().contains(pos.x, pos.y))
		{
			int index = cursor_index(pos);
			if (index >= 0 && index <= max_index)
			{
				app_handle.register_scpp(std::make_unique<SCPP_LCOS_Coin>(index));
				app_handle.request_pop();
			}
		}
}

void State_LCOS_Coin::update(const sf::Time& dt)
{
	auto pos = sf::Mouse::getPosition();

	if (auto pos = sf::Mouse::getPosition(); options.getGlobalBounds().contains(pos.x, pos.y))
	{
		int index = cursor_index(pos);
		info.setString(make_string(index_to_coin_color_name(index), " (worth ", std::to_string(coin_value_by_index(index)), ")"));
		adjust_text_height(info);
	}
	else
		info.setString("");
}

State* State_LCOS_Coin::make_state(App& app_handle)
{
	return new State_LCOS_Coin(app_handle);
}

// ----------------------------------------------------------------
// State_LCOS_Lifeup
// ----------------------------------------------------------------

State_LCOS_Lifeup::State_LCOS_Lifeup(App& arg_app_handle) : State_LCOS_Base(arg_app_handle)
{
	auto& resources = app_handle.get_shared_context().resources;

	label.setString("Which Lifeup type?");
	set_center(label);

	options.setTexture(resources.textures.get(RESOURCE_ID::TEXTURES::LCOS_LIFEUP));
	set_center(options);

	adjust_to_resolution();
}

int State_LCOS_Lifeup::cursor_index(sf::Vector2i pos)
{
	return (pos.x - static_cast<int>(options.getGlobalBounds().left)) / 64;
}

void State_LCOS_Lifeup::handle_events(const sf::Event& event)
{
	if (event.type == sf::Event::MouseButtonPressed && event.key.code == sf::Mouse::Left)
		if (auto pos = sf::Mouse::getPosition(); options.getGlobalBounds().contains(pos.x, pos.y))
		{
			int index = cursor_index(pos);
			if (index >= 0 && index <= max_index)
			{
				app_handle.register_scpp(std::make_unique<SCPP_LCOS_Lifeup>(index));
				app_handle.request_pop();
			}
		}
}

void State_LCOS_Lifeup::update(const sf::Time& dt)
{
	auto pos = sf::Mouse::getPosition();

	if (auto pos = sf::Mouse::getPosition(); options.getGlobalBounds().contains(pos.x, pos.y))
	{
		int index = cursor_index(pos);
		info.setString(make_string(index_to_lifeup_type_name(index), " (heals by ", std::to_string(lifeup_value_by_index(index)), ")"));
		adjust_text_height(info);
	}
	else
		info.setString("");
}

State* State_LCOS_Lifeup::make_state(App& app_handle)
{
	return new State_LCOS_Lifeup(app_handle);
}