#include "app.h"

int main()	
{
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