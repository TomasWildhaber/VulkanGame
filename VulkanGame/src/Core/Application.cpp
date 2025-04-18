#include "pch.h"
#include "Application.h"
#include "Debugging/Log.h"
#include "GLFW/glfw3.h"

namespace Game
{
	Application::Application(ApplicationSpecifications& _specs) : specs(_specs)
	{
		ASSERT(instance == nullptr, "Application already exists!");

		instance = this;

		window = Window::Create(specs.WindowWidth, specs.WindowHeight, specs.Title, specs.CustomTitlebar);
		window->ShowWindow();
	}

	Application::~Application()
	{
		instance = nullptr;
	}

	void Application::Run()
	{
		while (window->IsRunning())
		{
			window->OnUpdate();
			window->OnRender();
		}
	}
}