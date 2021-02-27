#include "Mouse.h"
#include <Windows.h>

// Getters
std::pair<int, int> Mouse::GetPos() const noexcept
{
	return { x,y };
}

int Mouse::GetPosX() const noexcept
{
	return x;
}

int Mouse::GetPosY() const noexcept
{
	return y;
}

bool Mouse::LeftIsPressed() const noexcept
{
	return bLeftIsPressed;
}

bool Mouse::RightIsPressed() const noexcept
{
	return bRightIsPressed;
}

bool Mouse::IsInWindow() const noexcept
{
	return bIsInWindow;
}

// Other
Mouse::Event Mouse::Read() noexcept
{
	if (buffer.size() > 0u)
	{
		Mouse::Event e = buffer.front();
		buffer.pop();
		return e;
	}
	
	return Mouse::Event();
}

void Mouse::Flush() noexcept
{
	buffer = std::queue<Event>();
}

// Methods that register mouse events
void Mouse::OnMouseMove(int newX, int newY) noexcept
{
	x = newX;
	y = newY;

	buffer.push(Mouse::Event(Mouse::Event::EType::Move, *this));
	TrimBuffer();
}

void Mouse::OnLeftPressed(int x, int y) noexcept
{
	bLeftIsPressed = true;

	buffer.push(Mouse::Event(Mouse::Event::EType::LPress, *this));
	TrimBuffer();
}

void Mouse::OnLeftReleased(int x, int y) noexcept
{
	bLeftIsPressed = false;

	buffer.push(Mouse::Event(Mouse::Event::EType::LRelease, *this));
	TrimBuffer();
}

void Mouse::OnRightPressed(int x, int y) noexcept
{
	bRightIsPressed = true;

	buffer.push(Mouse::Event(Mouse::Event::EType::RPress, *this));
	TrimBuffer();
}

void Mouse::OnRightReleased(int x, int y) noexcept
{
	bRightIsPressed = false;

	buffer.push(Mouse::Event(Mouse::Event::EType::RRelease, *this));
	TrimBuffer();
}

void Mouse::OnWheelUp(int x, int y) noexcept
{
	buffer.push(Mouse::Event(Mouse::Event::EType::WheelUp, *this));
	TrimBuffer();
}

void Mouse::OnWheelDown(int x, int y) noexcept
{
	buffer.push(Mouse::Event(Mouse::Event::EType::WheelDown, *this));
	TrimBuffer();
}

void Mouse::OnWheelDelta(int x, int y, int delta) noexcept
{
	wheelDeltaCarry += delta;

	// Generate events for every 120 wheelDeltaCarry
	while (wheelDeltaCarry >= WHEEL_DELTA)
	{
		wheelDeltaCarry -= WHEEL_DELTA;
		OnWheelUp(x, y);
	}
	while (wheelDeltaCarry <= -WHEEL_DELTA)
	{
		wheelDeltaCarry += WHEEL_DELTA;
		OnWheelDown(x, y);
	}
}

void Mouse::OnMouseEnter() noexcept
{
	bIsInWindow = true;
	buffer.push(Mouse::Event(Mouse::Event::EType::Enter, *this));
	TrimBuffer();
}

void Mouse::OnMouseLeave() noexcept
{
	bIsInWindow = false;
	buffer.push(Mouse::Event(Mouse::Event::EType::Leave, *this));
	TrimBuffer();
}

void Mouse::TrimBuffer() noexcept
{
	while (buffer.size() > bufferSize)
	{
		buffer.pop();
	}
}
