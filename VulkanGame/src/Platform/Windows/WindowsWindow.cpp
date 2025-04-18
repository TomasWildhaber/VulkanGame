#include "pch.h"
#ifdef PLATFORM_WINDOWS

#include "WindowsWindow.h"
#include "Core/Input.h"
#include "Core/Time.h"	
#include "Debugging/Log.h"

namespace Game
{
	float lastTime = 0.0f;

	ScopeRef<Window> Window::Create(int width, int height, const char* title, bool customTitlebar)
	{
		return new WindowsWindow(width, height, title, customTitlebar);
	}

	WindowsWindow::WindowsWindow(int width, int height, const char* title, bool customTitlebar)
	{
		Data.Width = width;
		Data.Height = height;
		ASSERT(glfwInit(), "Failed to initialize glfw!");
		ASSERT(glfwGetCurrentContext() == nullptr, "Windows alredy created (multiple windows is not supported yet) => graphics API is initialized!");

#ifdef RC_DEBUG_CONFIG
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif

		glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
		// glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
		if (customTitlebar)
			glfwWindowHint(GLFW_CUSTOM_TITLEBAR, GLFW_TRUE);
		window = glfwCreateWindow(Data.Width, Data.Height, title, nullptr, nullptr);
		glfwMakeContextCurrent(window);

		glfwSetWindowUserPointer(window, &Data);
		WindowsWindow::SetVSync(true);
	}

	WindowsWindow::~WindowsWindow()
	{
		glfwDestroyWindow(window);
		glfwTerminate();
	}

	inline void WindowsWindow::OnUpdate() const
	{
		glfwPollEvents();

		float currentTime = (float)glfwGetTime();
		DeltaTime = currentTime - lastTime;
		lastTime = currentTime;
	}

	inline void WindowsWindow::OnRender() const
	{
		glfwSwapBuffers(window);
	}
}

#endif