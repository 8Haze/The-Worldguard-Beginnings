#include "state_settings_video.h"

#include <algorithm>

#include "hct/sort.h"

#include "utility.h"

// ----------------------------------------------------------------
// State_Settings_Video
// ----------------------------------------------------------------

State_Settings_Video::State_Settings_Video(App& arg_app_handle) :
	State(arg_app_handle, arg_app_handle.get_shared_context()),
	buttons(arg_app_handle.get_shared_context().resources, arg_app_handle.get_shared_context().sfx_manager)
{
	auto& font = context.resources.fonts.get(RESOURCE_ID::FONTS::MAIN);
	label = make_label(font, "Video Settings", { 0, 0 }, 80);
	info_usage = make_label(font, "Press UP and DOWN to browse available resolutions, ENTER to apply", { 0, 0 }, 30);
	info_resolution = make_label(font, "", { 0, 0 }, 30);

	buttons.create_button([](Config_Manager& cm, unsigned int width, unsigned int height) 
		{
			cm.update(Config_Manager::DATA::RES_WIDTH, width);
			cm.update(Config_Manager::DATA::RES_HEIGHT, height);
		}, "");

	const auto& modes_temp = sf::VideoMode::getFullscreenModes();
	modes.reserve(modes_temp.size());
	for (const auto& x : modes_temp)
		modes.push_back(x);
	modes.erase(std::remove_if(modes.begin(), modes.end(), [](sf::VideoMode vm) { return vm.width < 1280 || vm.height < 960; }), modes.end());
	hct::sort(modes.begin(), modes.end(), [](sf::VideoMode a, sf::VideoMode b) { return (a.width == b.width ? a.height < b.height : a.width < b.width); });
	index = modes.size() - 1;

	if (modes.size() == 0)
		throw std::runtime_error("State_Settings_Video::State_Settings_Video(): Failed to find any suitable video modes");

	adjust_to_resolution();
}

void State_Settings_Video::handle_events(const sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed)
		switch (event.key.code)
		{
		case (sf::Keyboard::Escape): app_handle.request_pop(); break;
		case (sf::Keyboard::Up): 
			++index; 
			if (index >= modes.size())
				--index;
			buttons.choose_next();
			break;
		case (sf::Keyboard::Down):
			if (index > 0)
				--index;
			buttons.choose_next();
			break;
		case (sf::Keyboard::Enter):
			buttons.execute(context.config_manager, modes[index].width, modes[index].height);
			app_handle.signal_resolution_change();
			break;
		}
}

void State_Settings_Video::update(const sf::Time& dt)
{
	info_resolution.setString(make_string("Current resolution: ", context.config_manager.get(Config_Manager::DATA::RES_WIDTH),
		" x ", context.config_manager.get(Config_Manager::DATA::RES_HEIGHT)));
	set_center(info_resolution);

	buttons.set_button_text(make_string(modes[index].width, " x ", modes[index].height), 0);
}

void State_Settings_Video::render()
{
	context.window.draw(label);
	context.window.draw(info_usage);
	context.window.draw(info_resolution);
	buttons.draw(context.window);
}

void State_Settings_Video::adjust_to_resolution()
{
	label.setPosition(sf::Vector2f(context.view.getSize().x * 0.5f, context.view.getSize().y * 0.1f));
	info_usage.setPosition(sf::Vector2f(context.view.getSize().x * 0.5f, context.view.getSize().y * 0.2f));
	info_resolution.setPosition(sf::Vector2f(context.view.getSize().x * 0.5f, context.view.getSize().y * 0.25f));

	buttons.set_button_position({ context.view.getSize().x * 0.5f,  context.view.getSize().y * 0.575f }, 0);
}

constexpr bool State_Settings_Video::request_previous_render()
{
	return false;
}

constexpr bool State_Settings_Video::uses_menu_background()
{
	return true;
}

constexpr bool State_Settings_Video::uses_dark_sheet()
{
	return false;
}

State* State_Settings_Video::make_state(App& app_handle)
{
	return new State_Settings_Video(app_handle);
}

State_Settings_Video::~State_Settings_Video()
{
	context.config_manager.save_changes();
}