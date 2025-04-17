#pragma once

namespace Game
{
	struct CommandArgs
	{
		CommandArgs() = default;
		CommandArgs(int argc, char** argv) : Count(argc), Args(argv) {}

		int Count;
		char** Args;
	};

	struct ApplicationSpecifications
	{
		ApplicationSpecifications() = default;
		ApplicationSpecifications(int width, int height, const char* title, CommandArgs args, bool customTitlebar = false) : WindowWidth(width), WindowHeight(height), Title(title), API(api), CustomRenderer(customRenderer), Args(args), CustomTitlebar(customTitlebar) {}

		uint32_t WindowWidth;
		uint32_t WindowHeight;
		const char* Title;
		CommandArgs Args;
		bool CustomTitlebar;
	};

	class Application
	{
	public:
		Application(ApplicationSpecifications& _specs);
		virtual ~Application();

		Application(const Application&) = delete;
		Application(const Application&&) = delete;

		inline static Application& Get() { return *instance; }
		const inline ApplicationSpecifications& GetSpecifications() { return specs; }
		inline Window& GetWindow() { return *window; }

		void Run();

		static bool IsApplicationRunning() { return ApplicationRunning; }

	protected:
		static inline Application* instance;

		ApplicationSpecifications specs;
		ScopeRef<Window> window;

		bool isRunning = true;
		bool isMinimized = false;
		static inline bool ApplicationRunning = true;
	};

	Application* CreateApplication(CommandArgs args);
}