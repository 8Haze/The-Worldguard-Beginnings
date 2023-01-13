#ifndef _CONTROLS_MANAGER_H_
#define _CONTROLS_MANAGER_H_

#include <map>

#include "resources.h"

class Controls_Manager
{
public:

	enum class ACTION
	{
		LEFT,
		RIGHT,
		JUMP,
		SHOOT, 
		COUNT 
	}; // keep 'COUNT' last

private:

	inline static const char* filename = "resources/data/key_bindings.cfg";
	inline static const char* filename_temp = "resources/data/key_bindings_temp.cfg";

	inline static const sf::Keyboard::Key def_left = sf::Keyboard::Key::Left;
	inline static const sf::Keyboard::Key def_right = sf::Keyboard::Key::Right;
	inline static const sf::Keyboard::Key def_jump = sf::Keyboard::Key::Z;
	inline static const sf::Keyboard::Key def_shoot = sf::Keyboard::Key::X;

	std::map<ACTION, sf::Keyboard::Key> bindings;

	bool load_saved_bindings();
	void load_default_bindings();

public:

	Controls_Manager();

	sf::Keyboard::Key get_key(ACTION action) const;

	void set_key(ACTION action, sf::Keyboard::Key key);

	void save_changes();

};

#endif // _CONTROLS_MANAGER_H_