#pragma once
#include "KeyCodes.h"
#include <glm/glm.hpp>

namespace Rocket
{
	class Input
	{
		Input() = delete;
	public:
		static bool IsKeyPressed(int keyCode);
		static bool IsMouseButtonPressed(int button);
		static glm::vec2 GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};
}