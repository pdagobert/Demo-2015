#include "Application.hpp"
#include <exception>
#include <iostream>

int main( int, char** )
{
	try
	{
		Application app;
		app.run();
	}
	catch( std::exception& e )
	{
		std::cout << e.what() << std::endl;
	}
	
	return 0;
}