#include "../pch.h"
#ifdef PLATFORM_WINDOWS

#include "WindowsWindow.h"
#include "Core/Input.h"
#include "Core/Time.h"	
#include "Debugging/Log.h"

namespace Rocket
{
	float lastTime = 0.0f;

	void OpenGLMessageCallback(uint32_t source, uint32_t type, uint32_t id, uint32_t severity, int length, const char* message, const void* userParam)
	{
		switch (severity)
		{
			case GL_DEBUG_SEVERITY_HIGH:				ERROR(message); return;
			case GL_DEBUG_SEVERITY_MEDIUM:				WARN(message);	return;
			case GL_DEBUG_SEVERITY_LOW:					DEBUG(message); return;
			case GL_DEBUG_SEVERITY_NOTIFICATION:		INFO(message);	return;
		}
	}

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

		if (Renderer::API == Renderer::APIs::OpenGL)
			WindowsWindow::initGL();

#ifdef RC_DEBUG_CONFIG
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif

		glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
		glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
		if (customTitlebar)
			glfwWindowHint(GLFW_CUSTOM_TITLEBAR, GLFW_TRUE);
		window = glfwCreateWindow(Data.Width, Data.Height, title, nullptr, nullptr);
		glfwMakeContextCurrent(window);

		if (Renderer::API == Renderer::APIs::OpenGL)
			WindowsWindow::initGlad();
		WindowsWindow::setCallbacks();

		glfwSetWindowUserPointer(window, &Data);
		WindowsWindow::SetVSync(true);
	}

	WindowsWindow::~WindowsWindow()
	{
		glfwDestroyWindow(window);
		glfwTerminate();
	}

	inline void const WindowsWindow::initGL()
	{
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
	}

	inline void const WindowsWindow::initGlad()
	{
		ASSERT(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress), "Failed to initialize glad!");

		glEnable(GL_DEPTH_TEST);

#ifndef RC_DISTRIBUTION_CONFIG
		if (GLVersion.major >= 4 && GLVersion.minor >= 3)
		{
			glEnable(GL_DEBUG_OUTPUT);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
			glDebugMessageCallback(OpenGLMessageCallback, nullptr);

			glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
		}
#endif
	}

	inline void WindowsWindow::setCallbacks()
	{
		glfwSetErrorCallback([](int error, const char* description) {
			ERROR("GLFW error {0}, {1}", error, description);
			});

		glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int width, int height) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.Width = width;
			data.Height = height;

			WindowResizedEvent event(width, height);
			data.CallbackFunction(event);
			});

		glfwSetWindowCloseCallback(window, [](GLFWwindow* window) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowClosedEvent event;
			data.CallbackFunction(event);
			});

		glfwSetWindowPosCallback(window, [](GLFWwindow* window, int x, int y) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.X = x;
			data.Y = y;

			WindowMovedEvent event(x, y);
			data.CallbackFunction(event);
			});

		glfwSetWindowFocusCallback(window, [](GLFWwindow* window, int focused) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowFocusedEvent event(focused);
			data.CallbackFunction(event);
			});

		glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
			case GLFW_PRESS:
			{
				KeyPressedEvent event(key, 0);
				data.CallbackFunction(event);
				break;
			}
			case GLFW_RELEASE:
			{
				KeyReleasedEvent event(key);
				data.CallbackFunction(event);
				break;
			}
			case GLFW_REPEAT:
			{
				KeyPressedEvent event(key, 1);
				data.CallbackFunction(event);
				break;
			}
			}
			});

		glfwSetCharCallback(window, [](GLFWwindow* window, uint32_t keyChar) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			KeyTypedEvent event(keyChar);
			data.CallbackFunction(event);
			});

		glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
			case GLFW_PRESS:
			{
				MouseButtonPressedEvent event(button);
				data.CallbackFunction(event);
				break;
			}
			case GLFW_RELEASE:
			{
				MouseButtonReleasedEvent event(button);
				data.CallbackFunction(event);
				break;
			}
			}
			});

		glfwSetScrollCallback(window, [](GLFWwindow* window, double xoffset, double yoffset) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			MouseScrolledEvent event((float)xoffset, (float)yoffset);
			data.CallbackFunction(event);
			});

		glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xPos, double yPos) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			MouseMovedEvent event((float)xPos, (float)yPos);
			data.CallbackFunction(event);
			});

		glfwSetTitlebarHitTestCallback(window, [](GLFWwindow* window, int x, int y, int* hit) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			*hit = data.TitlebarHovered;
			});
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