#pragma once

namespace Game
{
	class Time
	{
	public:
		Time() = default;
		Time(float time) : deltaTime(time) {}

		operator float() const { return deltaTime; }

		inline const float GetDeltaTime() const { return deltaTime; }
		inline const float GetDeltaTimeMillis() const { return deltaTime * 1000; }
	private:
		float deltaTime = 0;
	};

	inline Time DeltaTime;
}