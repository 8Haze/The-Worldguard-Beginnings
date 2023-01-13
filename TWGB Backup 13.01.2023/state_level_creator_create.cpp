#include "state_level_creator_create.h"

#include <algorithm>
#include <fstream>
#include <filesystem>

#include "hct/scope_guard.h"

#include "utility.h"

// ----------------------------------------------------------------
// State_Level_Creator_Create
// ----------------------------------------------------------------

State_Level_Creator_Create::State_Level_Creator_Create(App& arg_app_handle) :
	State(arg_app_handle, arg_app_handle.get_shared_context()), text_field(arg_app_handle.get_shared_context().resources),
	buttons(arg_app_handle.get_shared_context().resources, arg_app_handle.get_shared_context().sfx_manager)
{
	buttons.create_button([](Level_Properties& level, int diff)
		{
			level.theme = static_cast<LEVEL_THEME>(static_cast<int>(level.theme) + diff);
			level.theme = static_cast<LEVEL_THEME>(std::max<unsigned int>(static_cast<unsigned int>(level.theme), 1u));
			level.theme = static_cast<LEVEL_THEME>(std::min<unsigned int>(static_cast<unsigned int>(level.theme), 8u));
		}, "Theme");

	theme_logo.setScale(3.f, 3.f);
	theme_logo.setTexture(context.resources.textures.get(RESOURCE_ID::TEXTURES::THEMES_LOGOS));
	theme_logo.setTextureRect(sf::IntRect(0, 0, logo_size, logo_size));
	set_center(theme_logo, false);

	auto& font = context.resources.fonts.get(RESOURCE_ID::FONTS::MAIN);
	label = make_label(font, "Level Properties", { 0, 0 }, 80);
	info = make_label(font, "Enter the name for your level's file (no extensions!)", { 0, 0 }, 30);
	error_info = make_label(font, "", { 0, 0 }, 30);	
	theme_text = make_label(font, "1", { 0, 0 }, 30);

	adjust_to_resolution();
}

std::string State_Level_Creator_Create::create_file()
{
	namespace fs = std::filesystem;

	if (level.filename_main.empty())
		return { "The name may not be empty."};

	for (auto a : level.filename_main)
		for (auto b : forbidden_filename_symbols)
			if (a == b)
				return { "This name contains at least one forbidden symbol. Try another one." };

	level.filename_main += ".wgl";
	level.filename_data += ".wgld";

	level.path_main = Config_Manager::user_levels_directory.string() + level.filename_main;
	level.path_data = Config_Manager::user_levels_directory.string() + level.filename_data;

	if (fs::exists(level.path_main) || fs::exists(level.path_data))
		return { "The name you've provided is already occupied. Try another one." };

	std::ofstream ofs1(level.path_main);
	auto guard1 = hct::make_guard_exit([&ofs1]() { ofs1.close(); });
	data_writer(ofs1, static_cast<int32_t>(32.f)); 
	data_writer(ofs1, static_cast<int32_t>(96.f));
	std::ofstream ofs2(level.path_data);
	auto guard2 = hct::make_guard_exit([&ofs2]() { ofs2.close(); });

	if (fs::exists(level.path_main) && fs::exists(level.path_data))
		return { "" };
	else
		return { "Failed to generate a file with this name. Try another one." };

	level.campaign_index = 0;
	level.achieved_points = 0;
	level.max_points = 0;
	level.beaten = false;
}

void State_Level_Creator_Create::handle_events(const sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
		app_handle.request_pop();

	switch (phase)
	{
	case(PHASE::FILE_NAME):
		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter)
		{
			level.filename_main = level.filename_data = text_field.get_string();
			auto result_string = create_file();
			if (!result_string.empty())
			{
				error_info.setString(std::move(result_string));
				set_center(error_info);
			}
			else
			{
				destroy_files_on_exit = true;
				text_field.clear();
				error_info.setString("");
				info.setString("Enter the name for your level");
				set_center(info);
				phase = PHASE::LEVEL_NAME;
			}
		}
		text_field.handle_input(event);
		break;

	case(PHASE::LEVEL_NAME):
		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter)
		{
			level.name = text_field.get_string();
			if (level.name.empty())
				level.name = "unnamed";
			info.setString("Press LEFT or RIGHT to change values");
			set_center(info);
			phase = PHASE::NUMERIC_DATA;
		}
		text_field.handle_input(event);
		break;

	case(PHASE::NUMERIC_DATA):
		if (event.type == sf::Event::KeyPressed)
			switch (event.key.code)
			{
			case (sf::Keyboard::Left): buttons.execute(level, -1); break;
			case (sf::Keyboard::Right): buttons.execute(level, 1); break;
			case (sf::Keyboard::Up): buttons.choose_prev(); break;
			case (sf::Keyboard::Down): buttons.choose_next(); break;
			case (sf::Keyboard::Enter):
				destroy_files_on_exit = false;
				save_level_properties(level);
				app_handle.request_pop();
				app_handle.request_push(App::STATE::LEVEL_CREATOR);
				app_handle.register_scpp(std::make_unique<SCPP_Level_Properties>(level));
				break;
			}
		break;
	}
}

void State_Level_Creator_Create::update(const sf::Time& dt)
{
	text_field.update(dt);

	if (phase == PHASE::NUMERIC_DATA)
	{
		theme_text.setString(level_theme_to_string(level.theme));
		set_center(theme_text);
		theme_logo.setTextureRect(sf::IntRect(logo_size * (static_cast<int>(level.theme) - 1), 0, logo_size, logo_size));
	}
}

void State_Level_Creator_Create::render()
{
	context.window.draw(label);
	context.window.draw(info);
	context.window.draw(error_info);

	switch (phase)
	{
	case(PHASE::FILE_NAME): case(PHASE::LEVEL_NAME):
		text_field.draw(context.window);
		break;
	case(PHASE::NUMERIC_DATA):
		buttons.draw(context.window);
		context.window.draw(theme_text);
		context.window.draw(theme_logo);
		break;
	}
}

void State_Level_Creator_Create::adjust_to_resolution()
{
	theme_logo.setPosition(sf::Vector2f(context.view.getSize().x * 0.5f, context.view.getSize().y * 0.35f));

	label.setPosition(sf::Vector2f(context.view.getSize().x * 0.5f, context.view.getSize().y * 0.1f));
	info.setPosition(sf::Vector2f(context.view.getSize().x * 0.5f, context.view.getSize().y * 0.2f));
	error_info.setPosition(sf::Vector2f(context.view.getSize().x * 0.5f, context.view.getSize().y * 0.8f));
	theme_text.setPosition(sf::Vector2f(context.view.getSize().x * 0.65f, context.view.getSize().y * 0.575f));

	text_field.set_position(context.view.getCenter());
	
	buttons.set_button_position(sf::Vector2f(context.view.getSize().x * 0.35f, context.view.getSize().y * 0.575f), 0);
}

constexpr bool State_Level_Creator_Create::request_previous_render()
{
	return false;
}

constexpr bool State_Level_Creator_Create::uses_menu_background()
{
	return true;
}

constexpr bool State_Level_Creator_Create::uses_dark_sheet()
{
	return false;
}

State* State_Level_Creator_Create::make_state(App& app_handle)
{
	return new State_Level_Creator_Create(app_handle);
}

State_Level_Creator_Create::~State_Level_Creator_Create()
{
	if (destroy_files_on_exit)
	{
		namespace fs = std::filesystem;

		fs::remove(level.path_main);
		fs::remove(level.path_data);
	}
}