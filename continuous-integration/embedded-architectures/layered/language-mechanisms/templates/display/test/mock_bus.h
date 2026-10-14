#ifndef MOCK_BUS_H
#define MOCK_BUS_H

#include <cstdint>
#include <vector>

// Test double: records all calls instead of accessing the hardware
class MockBus
{
	public:
		bool initialized = false;
		std::vector<uint8_t> written;

		void init(void)
		{
			initialized = true;
		}

		void write(const uint8_t data)
		{
			written.push_back(data);
		}
};

#endif /* MOCK_BUS_H */
