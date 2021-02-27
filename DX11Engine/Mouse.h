#pragma once

#include <queue>

class Mouse
{
	friend class Window;

public:
	class Event
	{
	public:
		enum class EType
		{
			LPress,
			LRelease,
			RPress,
			RRelease,
			WheelUp,
			WheelDown,
			Move,
			Enter,
			Leave,
			Invalid
		};

	private:
		EType type;
		bool bLeftIsPressed;
		bool bRightIsPressed;
		int x;
		int y;

	public:
		Event() noexcept
			:
			type(EType::Invalid),
			bLeftIsPressed(false),
			bRightIsPressed(false),
			x(0),
			y(0)
		{}
		Event(EType type, const Mouse& parent) noexcept
			:
			type(type),
			bLeftIsPressed(parent.bLeftIsPressed),
			bRightIsPressed(parent.bRightIsPressed),
			x(parent.x),
			y(parent.y)
		{}

		// Getter methods
		EType GetType() const noexcept
		{
			return type;
		}
		std::pair<int, int> GetPos() const noexcept
		{
			return{ x,y };
		}
		int GetPosX() const noexcept
		{
			return x;
		}
		int GetPosY() const noexcept
		{
			return y;
		}
		bool LeftIsPressed() const noexcept
		{
			return bLeftIsPressed;
		}
		bool RightIsPressed() const noexcept
		{
			return bRightIsPressed;
		}
	};

public:
	Mouse() = default;
	Mouse(const Mouse&) = delete;
	Mouse& operator=(const Mouse&) = delete;

	// Getter methods
	std::pair<int, int> GetPos() const noexcept;
	int GetPosX() const noexcept;
	int GetPosY() const noexcept;
	bool LeftIsPressed() const noexcept;
	bool RightIsPressed() const noexcept;
	bool IsInWindow() const noexcept;

	// Other methods
	Mouse::Event Read() noexcept;
	bool IsEmpty() const noexcept
	{
		return buffer.empty();
	}
	void Flush() noexcept;

private:
	// Methods that register mouse events
	void OnMouseMove(int x, int y) noexcept;
	void OnLeftPressed(int x, int y) noexcept;
	void OnLeftReleased(int x, int y) noexcept;
	void OnRightPressed(int x, int y) noexcept;
	void OnRightReleased(int x, int y) noexcept;
	void OnWheelUp(int x, int y) noexcept;
	void OnWheelDown(int x, int y) noexcept;
	void OnWheelDelta(int x, int y, int delta) noexcept;
	void OnMouseEnter() noexcept;
	void OnMouseLeave() noexcept;

	void TrimBuffer() noexcept;

private:
	static constexpr unsigned int bufferSize = 16u;
	int x;
	int y;
	bool bLeftIsPressed = false;
	bool bRightIsPressed = false;
	bool bIsInWindow = false;
	int wheelDeltaCarry = 0;
	std::queue<Event> buffer;
};

