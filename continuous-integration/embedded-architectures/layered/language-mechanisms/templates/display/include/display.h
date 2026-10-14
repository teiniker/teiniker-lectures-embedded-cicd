#ifndef DISPLAY_H
#define DISPLAY_H

#include <array>
#include <cstddef>
#include <cstdint>

// Bus requirements: void init(void) and void write(uint8_t data)
template <typename Bus, std::size_t Cols, std::size_t Rows>
class Display
{
	static_assert(Cols > 0 && Rows > 0, "Display size must not be zero");

	private:
		Bus _bus;
		std::array<char, Cols * Rows + 1> _buffer; // +1: terminating '\0'
		std::size_t _index;

	public:
		static constexpr std::size_t cols = Cols;
		static constexpr std::size_t rows = Rows;

		Display(void) : _buffer{}, _index{0}
		{
			_bus.init();
		}

		void clear(void)
		{
			_buffer.fill('\0');
			_index = 0;
		}

		void print(const char c)
		{
			if(_index < Cols * Rows)	// ignore characters beyond the display
			{
				_buffer[_index++] = c;
				_bus.write(static_cast<uint8_t>(c));
			}
		}

		const char* buffer(void) const
		{
			return _buffer.data();
		}

		Bus& bus(void)
		{
			return _bus;
		}
};

#endif /* DISPLAY_H */
