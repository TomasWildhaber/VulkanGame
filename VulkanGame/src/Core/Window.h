#pragma once
#include "Utilities/Memory.h"
#include "glm/glm.hpp"

struct GLFWwindow;

namespace Rocket
{
	class Window
	{
	public:
		virtual ~Window() = default;

		virtual inline void OnUpdate() const = 0;
		virtual inline void OnRender() const = 0;

		virtual inline int GetWidth() const = 0;
		virtual inline int GetHeight() const = 0;
		virtual inline glm::vec2 GetCurrentResolution() const = 0;
		virtual inline glm::vec2 GetCurrentPosition() const = 0;
		virtual inline void* GetNativeWindow() const = 0;

		virtual inline void SetVSync(bool Enabled) = 0;
		virtual inline bool IsVSync() const = 0;
		virtual inline bool IsMaximized() const = 0;

		virtual inline void MaximizeWindow() const = 0;
		virtual inline void RestoreWindow() const = 0;
		virtual inline void MinimizeWindow() const = 0;
		virtual inline void CloseWindow() const = 0;

		virtual inline void ShowWindow() const = 0;
		virtual inline void HideWindow() const = 0;

		static ScopeRef<Window> Create(int width, int height, const char* title, bool customTitlebar);
	protected:
		struct WindowData
		{
			int X, Y, Width, Height;
			bool TitlebarHovered;
		};
	public:
		WindowData Data;
	};
}