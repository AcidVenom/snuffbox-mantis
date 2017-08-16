#include <snuffbox-engine/application/application.h>

using namespace snuffbox;
using namespace snuffbox::engine;

#include <string>

int main(int argc, char** argv)
{
	int exit = 0;
	{
		SnuffboxApp app;
		exit = app.Exec(argc, argv);
	}

	return exit;
}
