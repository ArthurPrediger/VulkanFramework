#include "first_app.h"

void First_App::Run()
{
	while (!window.ShouldClose())
	{
		glfwPollEvents();
	}
}
