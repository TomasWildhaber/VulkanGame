#pragma once
#include "Core/Window.h"

#include <GLFW/glfw3.h>

namespace Game
{
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(int width, int height, const char* title, bool customTitlebar);
		~WindowsWindow();

		virtual inline int GetWidth() const override { return Data.Width; }
		virtual inline int GetHeight() const override { return Data.Height; }
		virtual inline glm::vec2 GetCurrentResolution() const override { return { Data.Width, Data.Height }; }
		virtual inline glm::vec2 GetCurrentPosition() const override { return { Data.X, Data.Y }; }
		virtual inline void* GetNativeWindow() const override { return window; }

		virtual inline void OnUpdate() const override;
		virtual inline void OnRender() const override;

		virtual inline bool IsRunning() const override { return !glfwWindowShouldClose(window); }

		virtual inline void SetVSync(bool Enabled) override { glfwSwapInterval(Enabled); vsync = Enabled; }
		virtual inline bool IsVSync() const override { return vsync; }
		virtual inline bool IsMaximized() const override { return glfwGetWindowAttrib(window, GLFW_MAXIMIZED); }

		virtual inline void MaximizeWindow() const override { glfwMaximizeWindow(window); }
		virtual inline void RestoreWindow() const override { glfwRestoreWindow(window); }
		virtual inline void MinimizeWindow() const override { glfwIconifyWindow(window); }
		virtual inline void CloseWindow() const override { glfwSetWindowShouldClose(window, GLFW_TRUE); }

		virtual inline void ShowWindow() const override { glfwShowWindow(window); }
		virtual inline void HideWindow() const override { glfwHideWindow(window); }
	private:
		GLFWwindow* window;
		bool vsync;
	};
}