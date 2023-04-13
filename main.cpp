#include <cstdlib>		// EXIT_FAILURE, EXIT_SUCCESS
#include <cstdio>		// printf
#include <exception>	// std::exception

#include "src/App.h"

int main(int argc, char** argv)
{
	try
	{	
		App app;

		if (app.init(argc, argv))
		{
			app.run();
		}

		app.unit();
	}
	catch (const std::exception& e)
	{
		printf("Exception %s\n", e.what());

		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}