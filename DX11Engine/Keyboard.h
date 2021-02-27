#pragma once

#include <queue>
#include <bitset>

class Keyboard
{
	friend class Window;

public:
	class Event
	{
	public:
		enum class EType
		{
			Press,
			Release,
			Invalid
		};

	private:
		EType type;
		unsigned char code;

	public:
		Event() noexcept
			:
			type(EType::Invalid),
			code(0u)
		{}
		Event(EType type, unsigned char code) noexcept
			:
			type(type),
			code(code)
		{}

		bool IsPress() const noexcept
		{
			return type == EType::Press;
		}
		bool IsRelease() const noexcept
		{
			return type == EType::Release;
		}
		bool IsValid() const noexcept
		{
			return type != EType::Invalid;
		}
		unsigned char GetCode() const noexcept
		{
			return code;
		}
	};

public:
	Keyboard() = default;
	Keyboard(const Keyboard&) = delete;
	Keyboard& operator=(const Keyboard&) = delete;

	// Key event methods
	bool KeyIsPressed(unsigned char keycode) const noexcept;
	Event ReadKey() noexcept;
	bool KeyIsEmpty() const noexcept;
	void FlushKey() noexcept;

	// Char event methods
	char ReadChar() noexcept;
	bool CharIsEmpty() const noexcept;
	void FlushChar() noexcept;

	// Auto repeat control methods
	void EnableAutorepeat() noexcept;
	void DisableAutorepeat() noexcept;
	bool AutoRepeatIsEnabled() const noexcept;

	// Other methods
	void FlushAll() noexcept;

private:
	// Keystate methods
	void OnKeyPressed(unsigned char keycode) noexcept;
	void OnKeyReleased(unsigned char keycode) noexcept;
	void OnChar(char character) noexcept;
	void ClearState() noexcept;

	template<typename T>
	static void TrimBuffer(std::queue<T>& buffer) noexcept;

private:
	static constexpr unsigned int nKeys = 256u;
	static constexpr unsigned int bufferSize = 16u;
	bool bAutoRepeatEnabled = false;
	std::bitset<nKeys> keystates;
	std::queue<Event> keybuffer;
	std::queue<char> charbuffer;
};

