#ifndef _STATE_LEVEL_CREATOR_CREATE_H_
#define _STATE_LEVEL_CREATOR_CREATE_H_

#include "app.h"
#include "gui_text_field.h"
#include "gui_buttons_layout.h"

class State_Level_Creator_Create final : public State
{
private:

	enum class PHASE { FILE_NAME, LEVEL_NAME, NUMERIC_DATA };

	inline static const std::string forbidden_filename_symbols = "\\/:*?\"<>";
	inline static const int logo_size = 32;

	GUI_Text_Field text_field;
	GUI_Buttons_Layout<Level_Properties&, int> buttons;
	sf::Sprite theme_logo;
	sf::Text label;
	sf::Text info;
	sf::Text error_info;
	sf::Text theme_text;
	Level_Properties level;
	PHASE phase = PHASE::FILE_NAME;
	bool destroy_files_on_exit = false;

	std::string create_file();

	State_Level_Creator_Create(App& arg_app_handle);

public:

	void handle_events(const sf::Event& event) override;
	void update(const sf::Time& dt) override;
	void render() override;
	void adjust_to_resolution() override;

	constexpr bool request_previous_render() override;
	constexpr bool uses_menu_background() override;
	constexpr bool uses_dark_sheet() override;

	static State* make_state(App& app_handle);

	~State_Level_Creator_Create();

};

#endif // _STATE_LEVEL_CREATOR_CREATE_H_