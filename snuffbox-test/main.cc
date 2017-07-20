#include "app.h"

int main(int argc, char** argv)
{
	int exit = 0;
	{
		snuffbox::test::App app;
		exit = app.Exec(argc, argv);
	}

	return exit;
}
