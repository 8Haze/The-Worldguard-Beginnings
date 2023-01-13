#include "app.h"
#include "utility.h"
#include "config_manager.h"

// TODO Now
// pokomentuj WSZEDZIE jakos ladnie
// user level list should sort by levels' names, not by their files' names
// tidy up the levels in filesystem

// BUGS
// Shit glitches on PC because of the shader, but no problem like this on the laptop

// Future update:
// much more SFX
// Additional info to indicate having beaten a user made level 100%
// clicking on mass insert again should disable it
// new action - pointer - clicking on objects displays their data (problem: what if there are many on the same tile?)
// current cursor x and y (immediately corrected) shown at all times (at the right panel)
// weather/particle effects in general (snow/rain/leaves blown by wind etc)
// Move from dynamic SFML linking to static linking (make a backup in case I fuck something up)
// secret levels

//#include "packages/VisualLeakDetector.2.5.0.0/lib/native/include/vld.h" // Only include when checking

int main()
{
	if (!sf::Shader::isAvailable())
		throw std::runtime_error("Game cannot run due to shaders being unsupported on this device");

	App app(App::STATE::MAIN_MENU);

	try
	{
		app.run();
	}
	catch (const std::exception& error)
	{
		write_error_logs(Config_Manager::error_logs_path, error.what(), true);
		app.request_clear();
		app.request_push(App::STATE::ERROR);
		app.register_scpp(std::make_unique<SCPP_Error>("A fatal error has occured and the game needs to close."));
		app.run();
	}
	catch (...)
	{
		write_error_logs(Config_Manager::error_logs_path, "an uncregonized error occured", true);
		app.request_clear();
		app.request_push(App::STATE::ERROR);
		app.register_scpp(std::make_unique<SCPP_Error>("A fatal error has occured and the game needs to close."));
		app.run();
	}

	return 0;
}