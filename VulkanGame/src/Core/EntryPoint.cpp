#pragma once
#include "pch.h"
#include "Application.h"
#include "Utilities/Memory.h"

int Main(int argc, char** argv)
{
	Game::ApplicationSpecifications specs(1280, 720, "Vulkan Game", { argc, argv });

	ScopeRef<Game::Application> app = new Game::Application(specs);
	app->Run();

	return 0;
}

#ifdef PLATFORM_WINDOWS

#ifndef DISTRIBUTION_CONFIG
int main(int argc, char** argv)
{
	return Main(argc, argv);
}
#else
int WinMain(int argc, char** argv)
{
	return Main(argc, argv);
}
#endif

#endif