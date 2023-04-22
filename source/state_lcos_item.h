#ifndef _STATE_LCOS_ITEM_H_
#define _STATE_LCOS_ITEM_H_

#include <tuple>

#include "app.h"

class State_LCOS_Item final : public State_LCOS_Base
{
private:

	State_LCOS_Item(App& arg_app_handle);

	int cursor_index(sf::Vector2i pos);

public:

	void handle_events(const sf::Event& event) override;
	void update(const sf::Time& dt) override;

	static State* make_state(App& app_handle);

};

class State_LCOS_Food final : public State_LCOS_Base
{
private:

	inline static const int max_index = 29;

	State_LCOS_Food(App& arg_app_handle);

	std::tuple<int, int, int> cursor_index(sf::Vector2i pos);

public:

	void handle_events(const sf::Event& event) override;
	void update(const sf::Time& dt) override;

	static State* make_state(App& app_handle);

};

class State_LCOS_Gem final : public State_LCOS_Base
{
private:

	inline static const int max_index = 7;

	State_LCOS_Gem(App& arg_app_handle);

	int cursor_index(sf::Vector2i pos);

public:

	void handle_events(const sf::Event& event) override;
	void update(const sf::Time& dt) override;

	static State* make_state(App& app_handle);

};

class State_LCOS_Coin final : public State_LCOS_Base
{
private:

	inline static const int max_index = 1;

	State_LCOS_Coin(App& arg_app_handle);

	int cursor_index(sf::Vector2i pos);

public:

	void handle_events(const sf::Event& event) override;
	void update(const sf::Time& dt) override;

	static State* make_state(App& app_handle);

};

class State_LCOS_Lifeup final : public State_LCOS_Base
{
private:

	inline static const int max_index = 1;

	State_LCOS_Lifeup(App& arg_app_handle);

	int cursor_index(sf::Vector2i pos);

public:

	void handle_events(const sf::Event& event) override;
	void update(const sf::Time& dt) override;

	static State* make_state(App& app_handle);

};

#endif // _STATE_LCOS_ITEM_H_